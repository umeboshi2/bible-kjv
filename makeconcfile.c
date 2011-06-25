/* -*-C-*-
*******************************************************************************
*
* File:         makeconcfile.c
* RCS:          $Header: /home/matthew/cvs/bible-kjv-4.10/makeconcfile.c,v 2.7 2005/01/23 11:27:00 matthew Exp $
* Description:  Generate Concordance Data File
* Author:       Chip Chapin
* Created:      Thu Dec 17 11:28:20 1992
* Modified:     Mon Apr 26 11:14:38 1993 (Chip Chapin) chip@hpclbis
* Language:     C
* Package:      Text Storage Library (Bible Retrieval System)
* Status:       Experimental (Do Not Distribute)
*
*******************************************************************************
*
* Revisions:
*
* Thu Jan  7 12:07:05 1993 (Chip Chapin) chip@hpclbis
*  Revised fileheader.
*  Implemented cumulative index entries to save space (24kbytes for KJV).
* Wed Jan  6 10:08:12 1993 (Chip Chapin) chip@hpclbis
*  Began implementation of list compression.
* Wed Dec 23 13:43:11 1992 (Chip Chapin) chip@hpclbis
*  Unlink tmp files when done.
*******************************************************************************
* $Log: makeconcfile.c,v $
* Revision 2.7  2005/01/23 11:27:00  matthew
* include more standard header files
*
* Revision 2.6  2005/01/22 18:07:28  matthew
* prototype main properly
*
* Revision 2.5  2005/01/22 17:17:54  matthew
* cast return value of sizeof
*
* Revision 2.4  2005/01/22 16:28:28  matthew
* initialise cmpnum.
*
* Revision 2.3  2005/01/22 16:27:34  matthew
* sort out function prototyping, and include util.h
*
* Revision 2.2  2003/07/26 10:02:53  matthew
* fprintf("\0") is not very useful
*
* Revision 2.1  2003/02/01 02:39:53  matthew
* make main int main ..., and accordingly return 0 at the end of it, if
* all is well
*
* Revision 2.0  2003/01/08 15:29:52  matthew
* versions collected from the net
*
 * Revision 1.5  93/04/26  11:18:17  11:18:17  chip (Chip Chapin)
 * Release 4.00
 * Public release of portable datafile version.
 * 
 * Revision 1.4  93/04/23  13:08:07  13:08:07  chip (Chip Chapin)
 * PORTABILITY RELEASE
 * This version supports portable data files, usable on machines with
 * differing native byte-orders.
 * Also, this version compiles and runs on non-HPUX systems.  It has been
 * tested on SunOS 4.? and ULTRIX 4.?, using SPARC and DEC 3100 hardware
 * respectively.  Note that the data file format has rolled again.
 * 
 * Revision 1.3  93/01/07  12:18:06  12:18:06  chip (Chip Chapin)
 * Release 3.01: Greatly improved compression of concordance data file.
 * 
*
*******************************************************************************
*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "tsl.h"
#include "util.h"

#define FALSE	(0)
#define TRUE	(1)

#define INDEXTMPF	"/tmp/mci"
#define DATATMPF	"/tmp/mcd"

static void outerr(int n);
static void print_header(void);

char *myname;			/* Program Name */

/*
  Structure of Input Data ...

    Input data is a sorted ASCII file.
    Each line is a record.
    Each record is a variable-length list of fields, separated by blanks,
    as follows:

       <word> <ref> {<ref>...}

    where <word> is a lower-case alpha string, and <ref> is an integer
    numeric string.  Each <word> is guaranteed to be unique.  <ref> is
    expected to describe a location in the subject document where
    <word> occurs, though this program doesn't actually care about
    that.

  Structure of Output Data ...

    Please refer to description in tsl.h.
 */

struct tsl_conc_fileheader fh;


int main(int argc,char **argv)
/*----------------------------------------------------------------------
|   NAME:
|       main
|
|   ALGORITHM:
|       Main Program.
|
|   HISTORY:
|       921221 cc Initial creation.
|
\*----------------------------------------------------------------------*/
{
    FILE	*outfp, *indexfp, *datafp;
    int		ref;		/* NOT a ref_t, because it must be signed */
    ref_t	rbuf[SELECTSZ];
    unsigned char obuf[SELECTSZ*sizeof(ref_t)];
    int		n, oinx, cnt;
    unsigned char bhi, blo;
    int		refs_left;
    int		no_comp;
    int		cmpnum=0;
    int		base, curbase;
    int		m_n, m_ref, m_cmpnum, m_cmpsize;
    file_ptr_t	word_index, data_index;
    int		entry_size;
    short int	this_index;
    Short_Univ_Int indextmp;
#define WRDSZ    100
    char word[WRDSZ];
    
    myname = *argv++; argc--;	/* Program name */
    
    if (argc < 1) {
	fprintf( stderr, "%s: Missing output file name\n", myname );
	fprintf( stderr, "Usage: %s datafile\n", myname );
	exit( 1 );
    }

    outfp = fopen( *argv, "w" );
    if (outfp == NULL) {
	fprintf( stderr, "%s: Cannot open output file %s\n", myname, *argv );
	exit( 1 );
    }
    indexfp = fopen( INDEXTMPF, "w+" );
    datafp = fopen( DATATMPF, "w+" );
    if ((indexfp == NULL) || (datafp == NULL)) {
	fprintf( stderr, "%s: Cannot open temp files.\n", myname );
	exit( 1 );
    }

    /* Initialize the header and write it out... */
    fh.magic[0] = TSL_CONCMAGIC1;
    fh.magic[1] = TSL_CONCMAGIC2;
    fh.version[0] = TSL_CONCFVERSION1;
    fh.version[1] = TSL_CONCFVERSION2;
    strncpy( fh.name, "KJV Concordance", TSL_DESCRSZ );
    univ_assign(fh.word_ptr, sizeof( fh ));
    univ_assign(fh.word_cnt, 0);
    univ_assign(fh.index_ptr, 0);
    univ_assign(fh.data_ptr, 0);
    fwrite( &fh, sizeof(fh), 1, outfp );

    
    /* Process input data.
       Each line consists of a word, followed by a list of numeric
       references where the word occurs.  Separated by blanks.

       We will write the strings for the word list into the output file
       as we go, meanwhile counting the words and building both the reference
       index and the reference data pool in temp files.

       To save space, the reference index entries are not actually pointers
       into the reference data pool, but are the SIZE of the entry's ref data
       pool.
       
       So the offset in the ref data pool for entry N is the
       sum over i=0..N-1 of index(i).

       To complicate things more, there is a special case when an
       entry N has only a single ref.  In that case, the ref is NOT
       entered into the ref data pool but the ref itself is stored as
       index(N).  It is negated to distinguish it from a regular index
       entry.
       
     */
    word_index = 0;	/* The index of each word, 0..N */
    data_index = 0;	/* The offset in the ref data pool of current entry */
    while (scanf( "%s", word) > 0) {
	/* Append string to word list */
	if ((n=fputs( word, outfp )) <= 0)
	    outerr(n);
	putc( 0, outfp );

	/* Start reading references */
	cnt = 0;
	while (scanf( "%d", &n) > 0) {
	    /* Read all refs into buffer */
	    rbuf[cnt++] = (ref_t) n;
	}
	if (cnt <= 0) {
	    /* this should never happen */
	    fprintf( stderr, "%s: Bad input format.  No refs for %s\n",
		    myname, word );
	}
	
	entry_size = 0;		/* size (bytes) of current ref data entry */
	if (cnt == 1) {
	    /* Special handling for this case: word has a SINGLE REFERENCE.
	       Instead of putting the ref into the data pool, just keep it
	       in the index.  Encode it by negating it.
	     */
	    this_index = -rbuf[0];
	} else {
	    /* Write reference list to reference data pool */
	    for (n=oinx=0, refs_left=cnt; refs_left--; n++) {
		ref = rbuf[n];
		no_comp = TRUE;
		if (refs_left && (rbuf[n+1]-ref <= 16)) {
		    /* We can compress at least one (though possibly to
		       no advantage).  How many more might we compress?
		     */
		    cmpnum=1;
		    while ((cmpnum < refs_left) &&
			   (rbuf[n+cmpnum+1]-rbuf[n+cmpnum] <= 16)) {
			cmpnum++;
		    }

		    /*
		       How much will we really save?
		       Model the actual algorithm and compare...
		     */
		    m_cmpsize = 0;
		    curbase = ref;
		    m_n = n;
		    m_ref = rbuf[++m_n]-curbase-1;
		    m_cmpnum = cmpnum-1;
		    while (m_ref >= 0) {
			/* For each byte in bitmap... */
			while ((m_ref < 8) && (m_ref >= 0)) {
			    /* For each ref that fits in this byte... */
			    if (m_cmpnum > 0) {
				m_cmpnum--;
				m_ref = rbuf[++m_n]-curbase-1;
			    } else {
				m_ref = -1;	/* term. flag, both loops */
			    }
			}
			m_cmpsize++;
			curbase += 8;
			m_ref -= 8;
		    }
		    m_cmpsize += 2;		/* Add terminator size */

		    /* Well, did it pay off? */
		    no_comp = (m_cmpsize > (cmpnum*(int)sizeof(ref_t)));
		}
		if (no_comp) {
		    /* No compression */
		    bhi = (ref >> 8);
		    blo = ref & 0x00ff;
		    obuf[oinx++] = bhi;
		    obuf[oinx++] = blo;
		} else {
		    /* Do the compression.
		       Format:
		         <base address><bitmap>
		       Where <base address> is the NEGATED reference to the
		       line BEFORE the start of the bitmap refs.
		     */
		    /* Write base address */
		    base = ref;
		    ref = -ref;
		    bhi = (ref >> 8);
		    blo = ref & 0x00ff;
		    obuf[oinx++] = bhi;
		    obuf[oinx++] = blo;

		    /* Write bitmap.
		         ref is always with respect to curbase.
			 It is the number of verses, MINUS ONE, that
			 separate them.  The MINUS ONE is so the
			 left-shift works correctly.
		     */
		    curbase = base;
		    ref = rbuf[++n]-curbase-1;
		    refs_left -= cmpnum;
		    cmpnum--;
		    while (ref >= 0) {
			/* For each byte in bitmap... */
			blo = 0;
			while ((ref < 8) && (ref >= 0)) {
			    /* For each ref that fits in this byte... */
			    blo |= 0x01 << ref;
			    if (cmpnum > 0) {
				cmpnum--;
				ref = rbuf[++n]-curbase-1;
			    } else {
				ref = -1;	/* term. flag, both loops */
			    }
			}
			obuf[oinx++] = blo;
			curbase += 8;
			ref -= 8;
		    }
		    
		    /* Write TWO terminator bytes */
		    /* Might be better to use ONE, and change the requirement
		       to being within 8 lines, not 16 */
		    obuf[oinx++] = 0;
		    obuf[oinx++] = 0;
		} /* compression */
	    } /* for */
	    if ((n=fwrite( obuf, 1, oinx, datafp )) <= 0)
		outerr(n);

	    entry_size = oinx;
	    if (entry_size > 32767) {
		fprintf( stderr, "%s: Too many entries for \"%s\"!\n",
			myname, word );
		exit(1);
	    }
	    this_index = entry_size;
	} /* write ref list */
	
	/* Write the index entry for this word */
	shortuniv_assign( indextmp, this_index );
	if ((n=fwrite( indextmp, sizeof(Short_Univ_Int), 1, indexfp )) <= 0)
	    outerr(n);
	
	data_index += entry_size;
	word_index++;
    }

    /* It makes searching easier if we guarantee an ending string */
    if ((n=fprintf( outfp, "~" )) <= 0)
	outerr(n);
    shortuniv_assign( indextmp, 0 );
    if ((n=fwrite( indextmp, sizeof(Short_Univ_Int), 1, indexfp )) <= 0)
	outerr(n);
    
    /* OK, now finish putting the output file together. */
    /* First, another header field */
    univ_assign(fh.word_cnt, word_index);

    /* Now copy the reference index from temp file to output file */
    univ_assign(fh.index_ptr, ftell( outfp ));
    rewind( indexfp );
    while (fread( obuf, sizeof(Short_Univ_Int), 1, indexfp ) > 0) {
	fwrite( obuf, sizeof(Short_Univ_Int), 1, outfp );
    }

    /* Then copy the reference data from temp file to output file */
    univ_assign(fh.data_ptr, ftell( outfp ));
    rewind( datafp );
    while (fread( obuf, 1, 1, datafp ) > 0) {
	fwrite( obuf, 1, 1, outfp );
    }

    /* Finally, write out the updated file header */
    rewind( outfp );
    fwrite( &fh, sizeof(fh), 1, outfp );

    /* Prove that we've been working hard... */
    print_header();

    fclose(outfp);
    fclose(indexfp);
    fclose(datafp);
    unlink( INDEXTMPF );
    unlink( DATATMPF );
    return(0);
} /* main */


static void outerr(int n)
{
    fprintf( stderr, "%s: File I/O error #%d, %d\n", __FILE__, n, errno );
}


static void print_header(void)
{
    printf( "Concordance data file:\n" );
    printf( "  Version:  %c%c\n", fh.version[0], fh.version[1] );
    printf( "  Name:     %s\n", fh.name );
    printf( "  Contents: %d words\n", univ2int(fh.word_cnt) );
    printf( "  Word list at file offset %d\n", univ2int(fh.word_ptr) );
    printf( "  Index at file offset %d\n", univ2int(fh.index_ptr) );
    printf( "  Data at file offset %d\n", univ2int(fh.data_ptr) );
}
