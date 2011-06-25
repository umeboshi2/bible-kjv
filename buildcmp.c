/* -*-C-*-
********************************************************************************
*
* File:         buildcmp.c
* RCS:          $Header: /home/matthew/cvs/bible-kjv-4.10/buildcmp.c,v 2.6 2005/01/22 18:23:16 matthew Exp $
* Description:  Build compressed data file
* Author:       Chip Chapin, Hewlett Packard Company
* Created:      Mon May 29 17:34:58 1989
* Modified:     Mon Apr 26 11:15:20 1993 (Chip Chapin) chip@hpclbis
* Language:     C
* Package:      Bible Retrieval System
* Status:       Experimental (Do Not Distribute)
*
********************************************************************************
*
* Revisions:
*
* Fri Apr 23 09:34:07 1993 (Chip Chapin) chip@hpclbis
*  put_header modified to use Univ_Int portable data type.
* Wed Dec 23 13:57:38 1992 (Chip Chapin) chip@hpclbis
*  Tweak to eliminate compile warning.
*******************************************************************************
* $Log: buildcmp.c,v $
* Revision 2.6  2005/01/22 18:23:16  matthew
* include stdlib.h
*
* Revision 2.5  2005/01/22 17:57:33  matthew
* prototype functions properly
*
* Revision 2.4  2005/01/22 16:54:50  matthew
* we don't use command-line arguments so declare main (void)
*
* Revision 2.3  2005/01/22 00:34:42  matthew
* tidy up, and pass univ_assign *winx++, as that's the right type
*
* Revision 2.2  2003/02/22 16:00:03  matthew
* correct prototype of main(), and make it return correctly
*
* Revision 2.1  2003/01/09 12:59:10  matthew
* Add error checking to the fopen calls
*
* Revision 2.0  2003/01/08 15:29:52  matthew
* versions collected from the net
*
 * Revision 1.5  93/04/26  11:18:16  11:18:16  chip (Chip Chapin)
 * Release 4.00
 * Public release of portable datafile version.
 * 
 * Revision 1.4  93/04/23  13:08:06  13:08:06  chip (Chip Chapin)
 * PORTABILITY RELEASE
 * This version supports portable data files, usable on machines with
 * differing native byte-orders.
 * Also, this version compiles and runs on non-HPUX systems.  It has been
 * tested on SunOS 4.? and ULTRIX 4.?, using SPARC and DEC 3100 hardware
 * respectively.  Note that the data file format has rolled again.
 * 
 * Revision 1.3  92/12/23  14:10:45  14:10:45  chip (Chip Chapin)
 * Release 2.03: minor tweaks and bug fixes.
 * 
 * Revision 1.2  89/09/14  20:33:56  20:33:56  chip (Chip Chapin)
 * Release 1-2.  Supports -f and -l options for formatting the output.
 * Updates primarily brl.c, bible.c, and bible.1.
 * 
 * Revision 1.1  89/09/05  17:49:23  17:49:23  chip (Chip Chapin)
 * Initial revision
 * 
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "tsl.h"
#include "util.h"

static void put_header(void);
static void put_data(void);

struct tsl_fileheader fh;
FILE *sf, *df, *of;	/* stats file, data file, output file */

static void put_header(void)
{
    int  i, j, count, d;
    int headersize, tablesize;
    int *w_table;
    Univ_Int *winx;
    
    fh.magic[0] = TSL_MAGIC1;
    fh.magic[1] = TSL_MAGIC2;
    fh.version[0] = TSL_FVERSION1;
    fh.version[1] = TSL_FVERSION2;
    strncpy( fh.description, "Compressed Data File", TSL_DESCRSZ );
    
    /* Process the squish statistics */
    fread( &d, sizeof(int), 1, sf );
    printf( "Window size (bytes): %d\n", d );
    univ_assign(fh.wsize, d);
    fread( &d, sizeof(int), 1, sf );
    printf( "Number of windows: %d\n", d );
    count = d;
    univ_assign(fh.wnum, d);

    fwrite( &fh, sizeof(fh), 1, of );

    tablesize = sizeof(int)*(count+1);		/* +1 for terminating entry */
    headersize = sizeof(fh) + tablesize;
    w_table = (int*) malloc( tablesize );
    winx = (Univ_Int *) w_table;

    for (i=0; i <= count; i++) {
	fread( &d, sizeof(int), 1, sf );
	univ_assign(*winx++, j=d+headersize);
	printf( "Window[%d] starts at %d\n", i, j );
    }
    fwrite( w_table, tablesize, 1, of );
}

static void put_data(void)
{
    int c;
    
    while ( (c = getc(df)) != EOF ) putc(c, of);
}

int main(void)
{
    
  if(NULL==(sf = fopen( "squish.stats",  "r" ))){
    fprintf(stderr,"Failed to open squish.stats: %s\n",strerror(errno));
    exit(1);
  }
  if(NULL==(df = fopen( "squish.data",   "r" ))){
    fprintf(stderr,"Failed to open squish.data: %s\n",strerror(errno));
    exit(1);
  }
  if(NULL==(of = fopen( "squish.output", "w" ))){
    fprintf(stderr,"Failed to open squish.output: %s\n",strerror(errno));
    exit(1);
  }

    put_header();
    put_data();

    fclose( sf );
    fclose( df );
    fclose( of );
    exit(0);
}

