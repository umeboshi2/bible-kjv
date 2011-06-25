/* -*-C-*-
*******************************************************************************
*
* File:         util.h
* RCS:          $Header: /home/matthew/cvs/bible-kjv-4.10/util.h,v 2.6 2005/01/23 11:12:18 matthew Exp $
* Description:  Header file for util.c
* Author:       Chip Chapin
* Created:      Thu Dec 24 11:27:53 1992
* Modified:     Mon Apr 26 11:14:17 1993 (Chip Chapin) chip@hpclbis
* Language:     C
* Package:      N/A
* Status:       Experimental (Do Not Distribute)
*
*******************************************************************************
*
* Revisions:
*
* Fri Apr 23 10:33:49 1993 (Chip Chapin) chip@hpclbis
*  Added Univ_Int stuff.
*******************************************************************************
* $Log: util.h,v $
* Revision 2.6  2005/01/23 11:12:18  matthew
* explicit cast
*
* Revision 2.5  2005/01/22 16:27:17  matthew
* Add shortuniv_assign, as that's called in makeconcfile.c
*
* Revision 2.4  2005/01/22 00:34:18  matthew
* make this header multiple-include-proof
*
* Revision 2.3  2005/01/22 00:31:08  matthew
* add univ_assign prototype
*
* Revision 2.2  2005/01/22 00:21:50  matthew
* correct case error
*
* Revision 2.1  2005/01/22 00:20:39  matthew
* prototype functions
*
* Revision 2.0  2003/01/08 15:29:52  matthew
* versions collected from the net
*
 * Revision 1.4  93/04/26  11:18:27  11:18:27  chip (Chip Chapin)
 * Release 4.00
 * Public release of portable datafile version.
 * 
*
*/
#ifndef __UTIL_H__
#define __UTIL_H__

#define get_nonblank(S)    while (isspace((int)*(S))) (S)++;

    
/*
  The Univ_Int data type is stored as a sequence of four bytes, from
  high-order to low-order.  This allows data files to be independent
  of the native byte-order of any machine.
 */
typedef unsigned char Univ_Int[4];/* Universal, byte-order-independent integer*/
typedef unsigned char Short_Univ_Int[2]; /* shorter Univ_Int */

int univ2int(Univ_Int src);
int shortuniv2int(Short_Univ_Int src);
void univ_assign(Univ_Int dst,int src);
void shortuniv_assign(Univ_Int dst,int src);

FILE *findfile(char *dfname,char *pathlist);

#endif /*__UTIL_H__*/
