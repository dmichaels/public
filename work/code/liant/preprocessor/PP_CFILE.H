/* CPP pp_cfile.h - Upper (generic) level C source file input header */

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
 *  07.14.89  DGM  (002) Minor reorganization.
 *  06.05.89  DGM  (001) Minor reorganization.
 *  02.08.89  DGM  (000) Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
**
** The sole purpose of this upper (generic) level C source file input
** layer is to allow the by-passing of the intermediate level line
** buffered file input layer (defined in lfile.[ch]), at run time, and
** go straight for the (maximaly efficient) lowest level file input
** layer (defined in ifile.[ch]).  It is the intent that the
** intermediate layer would be used only when a listing file is desired.
**
** The *only* way this can be done is if the following entities
** defined respectively in "lfile.h" and "ifile.h" are virtually
** functionally *identical*:
**
** (1) The data structures  LFILE & IFILE (except for the type of "fid").
** (2) The constants        LEOF & IEOF.
** (3) The macro functions  _lgetc_, _lungetc_, _lpeekc_
**                     and  _igetc_, _iungetc_, ipeekc_.
**
** --> If one of these is *not* true and you just don't want to deal
** --> with any of this, then just #undef ALLOW_MIDDLE_LAYER_BYPASS here.
**
** -------------------------------------------------------------------- */

#ifndef _CFILE_H_
#define _CFILE_H_

#include "pp_lfile.h"

/* ---------------------------------------------------------------------
** Macro definitions (generic names)
** ------------------------------------------------------------------- */

#undef  EOF
#undef  FILE

#undef  GETC
#undef  PEEKC
#undef  UNGETC

#undef  CLOSE
#undef  FILLUP
#undef 	GETPOS
#undef  OPEN
#undef 	SETPOS

#define EOF		LEOF
#define FILE		LFILE

#define GETC(f)		(lgetc(f))
#define	PEEKC(f)	(lpeekc(f))
#define UNGETC(f,c)	(lungetc((f),(c)))

#define CLOSE(f)	(lclose(f))
#define FILLUP(f)	(lfillup(f))
#define	GETPOS(f)	(lgetpos(f))
#define OPEN(s)		(lopen(s))
#define	SETPOS(f,p)	(lsetpos((f),(p)))

/* ---------------------------------------------------------------------
** External function declarations
** ------------------------------------------------------------------- */

extern void		   clfile ();

/* ---------------------------------------------------------------------
** Bypass surgery
** ------------------------------------------------------------------- */

#define ALLOW_MIDDLE_LAYER_BYPASS	/* yes, allow it */

#ifdef  ALLOW_MIDDLE_LAYER_BYPASS

#undef  CLOSE
#undef  FILLUP
#undef 	GETPOS
#undef  OPEN
#undef 	SETPOS

#define CLOSE(f)	((*cclose)(f))
#define FILLUP(f)	((*cfillup)(f))
#define	GETPOS(f)	((*cgetpos)(f))
#define OPEN(s)		((*copen)(s))
#define	SETPOS(f,p)	((*csetpos)((f),(p)))

extern void		 (*cclose)  ();
extern int		 (*cfillup) ();
extern ulong		 (*cgetpos) ();
extern FILE		*(*copen)   ();
extern void		 (*csetpos) ();

#endif  /* ALLOW_MIDDLE_LAYER_BYPASS */

#endif  /* _CFILE_H_ */

