/* CPP pp_defs.h - Miscellaneous C definitions */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  07.21.89  DGM  001  Added EOS.
 *  02.08.89  DGM  000  Original.
 *
 ***********************************************************************/

#ifndef _CDEF_H_
#define _CDEF_H_

/* ---------------------------------------------------------------------
** Macro definitions
** ------------------------------------------------------------------- */

#define NULL	0	/* null pointer constant */
#define EOS	'\0'	/* end-of-string indicator */

/* ---------------------------------------------------------------------
** Type definitions
** ------------------------------------------------------------------- */

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned long	ulong;

#endif  /* _CDEF_H_ */

