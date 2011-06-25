/* -*-C-*-
*******************************************************************************
*
* File:         makeindex.c
* RCS:          $Header: /home/matthew/cvs/bible-kjv-4.10/makeindex.c,v 2.6 2005/01/23 11:23:15 matthew Exp $
* Description:  Create Line Index to Text File
* Author:       Chip Chapin, Hewlett Packard Company
* Created:      Jan 14 1989
* Modified:     Thu Sep 14 12:37:15 1989 (Chip Chapin) chip@hpcllcc
* Language:     C
* Package:      Bible Retrieval System
* Status:       Experimental (Do Not Distribute)
*
* $Log: makeindex.c,v $
* Revision 2.6  2005/01/23 11:23:15  matthew
* include standard header files
*
* Revision 2.5  2005/01/22 17:48:08  matthew
* prototype main correctly
*
* Revision 2.4  2005/01/21 19:39:36  matthew
* remove unused variables
*
* Revision 2.3  2003/07/26 09:22:12  matthew
* Correct arguments to printf
*
* Revision 2.2  2003/02/01 02:40:43  matthew
* Make main() return an int, and prototype accordingly.
*
* Revision 2.1  2003/01/08 15:52:50  matthew
* line_locator[] is int not long (from debian patch)
*
* Revision 2.0  2003/01/08 15:29:52  matthew
* versions collected from the net
*
 * Revision 1.2  89/09/14  20:34:03  20:34:03  chip (Chip Chapin)
 * Release 1-2.  Supports -f and -l options for formatting the output.
 * Updates primarily brl.c, bible.c, and bible.1.
 * 
 * Revision 1.1  89/09/05  17:49:34  17:49:34  chip (Chip Chapin)
 * Initial revision
 * 
*
*******************************************************************************
*/

/*----------------------------------------------------------------------
|   NAME:
|       makeindex.c
|
|   PURPOSE:
|       Create an index to each line of a text file.
|       
|       The table thus produced may be indexed by line number to
|       produce the absolute byte number of the file at which that
|       line begins.
|       
|       An extra entry is guaranteed at the end, so that
|       table[n+1], where n is the total number of lines in the
|       file, yields the byte number just *after* the last byte of
|       the last line (i.e. the total number of bytes in the
|       file).
|       
|       By convention, I assume that "line 0" of the text file is
|       an informational string describing the data.  So the first
|       line of data is actually "line 1".  But there's really
|       nothing in the construction of the index that mandates
|       that convention.
|       
|       For the Bible Retrieval System, each verse starts on a new
|       line.  So the index produced by this program is an index
|       to each Bible verse.
|
|   FUNCTIONS:
|       N/A
|
|   HISTORY:
|       890114 cc Creation.
|       890904 cc Tidy it up, and read filenames from command line.
|
\*----------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Size of text buffer.  Better be enough for one line */
#define  TBSIZE		2048



int main(int argc,char **argv)
{
    FILE	*fp;
    long	t;
    long	cur_line;
    long	offset;
    char	textbuff[TBSIZE];

    if (--argc < 1) {
	fprintf( stderr, "%s: Missing input file name\n", *argv );
	fprintf( stderr, "Usage: %s textfile\n", *argv );
	exit( 1 );
    }
    
    fp = fopen( argv[1], "r" );
    if (fp == NULL) {
	fprintf( stderr, "%s: Cannot open file %s\n", *argv, argv[1] );
	return 1;
    }

    time(&t);
    printf( "/* Text index to file: %s\n", argv[1] );
    printf( "   Created %s\n", ctime(&t) );
    printf( "*/\n" );
    printf( "int line_locator[] = {" );
    
    offset = cur_line = 0L;
    while (!feof(fp)) {
	if (cur_line % 8 == 0) printf( "\n\t" );	/* pretty */
	/* print offset of current line */
	printf( "%ld, ", offset );

	/* Determine offset of next line */
	fgets( textbuff, TBSIZE, fp);
	offset += strlen(textbuff);
	cur_line++;
    }
    /* print offset of last line */
    printf( "%ld\n\t};\n", offset );	/* end of array initialization */
    printf( "\t/* textfile: %s, %ld lines. */\n", argv[1], cur_line );
    printf( "int max_line = %ld;\n", cur_line-1 );

    fclose(fp);
    return(0);
} /* main */
