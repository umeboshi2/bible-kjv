/* $Id: bible.h,v 2.2 2005/01/21 18:32:13 matthew Exp $ */
#ifndef __BIBLE_H__
#define __BIBLE_H__

#define FALSE	(0)
#define TRUE	(1)

#define REFSZ     15
#define VSPECSZ 132
#define LINESZ 1024
#define VBUFFSZ 32768

char *myname;			/* Program Name */
int  mem_limit;			/* Limit on buffer space used by BRL */
int  pretty_printing=FALSE;	/* Format the output */
int  list_always=FALSE;		/* List refs after a search */
int  line_width=0;		/* Line width to use in formatting */
int  skip_inc=1;		/* Scrolling direction through text */
int  select_count=0;		/* Count of verses selected via concordance */
ref_t selectbuf[SELECTSZ];	/* List of selected verses */
ref_t inrange_start=0,
      inrange_end=0;		/* Range limits on concordance selection */
FILE *outf=NULL;		/* Output file */

int do_concordance(char *word,ref_t *buf );
void cmd_ANDconcordance(char *word);
void cmd_ORconcordance(char *word);
void cmd_concordance(char *word);
void cmd_help(void);
void cmd_inrange(char *range);
void cmd_list(void);
void cmd_pretty_print(int verbose);
void cmd_view(void);
void cmd_write(char *fname);
void do_command(char *cmd);
void user_input(char  *cmd);
char *getprompt(void);
void usage(void);


#endif /*__BIBLE_H__*/
