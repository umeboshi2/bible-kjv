/* -*-C-*-
*******************************************************************************
*
* File:         brl.h
* RCS:          $Header: /home/matthew/cvs/bible-kjv-4.10/brl.h,v 2.2 2005/01/22 17:47:23 matthew Exp $
* Description:  Bible Retrieval Library header file
* Author:       Chip Chapin, Hewlett Packard Company
* Created:      Jan 21 1989
* Modified:     Mon Apr 26 11:12:49 1993 (Chip Chapin) chip@hpclbis
* Language:     C
* Package:      Bible Retrieval System
* Status:       Experimental (Do Not Distribute)
*
*******************************************************************************
*
* Revisions:
*
* Tue Jan  5 16:23:25 1993 (Chip Chapin) chip@hpclbis
*  Moved get_nonblank into util.h
*******************************************************************************
*
* $Log: brl.h,v $
* Revision 2.2  2005/01/22 17:47:23  matthew
* add some more function prototypes
*
* Revision 2.1  2005/01/21 19:05:38  matthew
* prototype functions properly
*
* Revision 2.0  2003/01/08 15:29:52  matthew
* versions collected from the net
*
 * Revision 1.6  93/04/26  11:18:23  11:18:23  chip (Chip Chapin)
 * Release 4.00
 * Public release of portable datafile version.
 * 
 * Revision 1.5  93/01/05  19:06:21  19:06:21  chip (Chip Chapin)
 * Release 3.00: (not for distribution)
 * Fixed errors (blank lines) in bible.data file.  Data file is not compatible
 * with previous (1.x and 2.x) distributions.  Further changes pending.
 * Rewrote context handling, and added "<" and ">" commands.
 * Tools for building brl-index are now part of release.
 * 
 * Revision 1.4  93/01/04  16:21:01  16:21:01  chip (Chip Chapin)
 * Release 2.1, implements ?in and ?or commands.
 * 
 * Revision 1.3  92/12/24  11:09:19  11:09:19  chip (Chip Chapin)
 * Release 2.04.  Include verse ref in prompt line.
 * 
 * Revision 1.2  89/09/14  20:34:11  20:34:11  chip (Chip Chapin)
 * Release 1-2.  Supports -f and -l options for formatting the output.
 * Updates primarily brl.c, bible.c, and bible.1.
 * 
 * Revision 1.1  89/09/05  17:49:45  17:49:45  chip (Chip Chapin)
 * Initial revision
 * 
*
*/


#define BADBOOK		"Bad Book: '%s'"
#define NO_VERSE	"Missing verse: '%s'"

enum books { GENESIS, EXODUS, LEVIT, NUM, DEUT, JOSHUA, JUDGES, RUTH,
	SAM1, SAM2, KINGS1, KINGS2, CHRON1, CHRON2, EZRA, NEHEM, ESTHER,
	JOB, PSALMS, PROV, ECCL, SONG, ISAIAH, JEREM, LAMENT,
	EZEKIEL, DANIEL, HOSEA, JOEL, AMOS, OBADIAH, JONAH, MICAH,
	NAHUM, HABAK, ZEPH, HAGGAI, ZECH, MALACHI,
	MATT, MARK, LUKE, JOHN, ACTS, ROMANS, COR1, COR2, GAL, EPH,
	PHILIP, COLOS, THESS1, THESS2, TIM1, TIM2, TITUS, PHILEM,
	HEBREWS, JAMES, PET1, PET2, JOHN1, JOHN2, JOHN3, JUDE,
	REV };

/* Defined in "brl-index.c" */
extern short start_verse[];
extern short start_chapter[];

/* Defined in "brl.c" */
extern char brl_textname[];
extern ref_t brl_cur_vnum;

ref_t brl_extract_num(ref_t absverse,int *bp,int *cp,int *vp);
ref_t brl_verse_spec(char **s,int *n);
ref_t brl_printverse(char *vs,int pretty,int lwidth,FILE *outf);
char* brl_num_to_ref(char *vbuf,ref_t *absversep);

void brl_init(char *dfname,char *dfpath,int memlimit);
void brl_close(void);

int get_book(char **s,int book);
int verse_continuation(char **s,int book,int chapter,int verse,ref_t absverse);
ref_t verse_id(char **s,int *bookp,int *chapterp,int *versep );
ref_t verse_num(int b,int c,int v);


