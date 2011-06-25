/* $Id: cmp.h,v 2.3 2005/01/22 18:47:45 matthew Exp $*/
#ifndef __CMP_H__
#define __CMP_H__

#ifdef NO_UCHAR
 typedef char   char_type;
#else
 typedef        unsigned char   char_type;
#endif /* UCHAR */

void cmp_init(void);
int cmp_decompress(char_type *inb, char_type  *outb, int insize );
int cmp_checkheader(char_type *inb);

#endif /*__CMP_H__*/
