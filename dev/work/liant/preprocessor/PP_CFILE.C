/* CPP pp_cfile.c - Upper (generic) level C source file input routines */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  07.14.89  DGM  (002) Minor reorganization.
 *  06.05.89  DGM  (001) Minor reorganization.
 *  02.09.89  DGM  (000) Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
** Version and copyright stamp
** ------------------------------------------------------------------- */

static char *VERSION__ =

"@(#)pp_cfile.c  000  6/5/90  (c) 1990 Language Processors, Inc.";

/* ---------------------------------------------------------------------
** Include files
** ------------------------------------------------------------------- */

#include "pp_cfile.h"

/* ---------------------------------------------------------------------
** See the comments in "pp_cfile.c".
** ------------------------------------------------------------------- */

#ifdef ALLOW_MIDDLE_LAYER_BYPASS

static void	 (*default_close)  ()	= (void (*)())  iclose;
static int	 (*default_fillup) ()	= (int (*)())   ifillup;
static ulong 	 (*default_getpos) ()	= (ulong (*)()) igetpos;
static FILE	*(*default_open)   ()	= (FILE *(*)()) iopen;
static void	 (*default_setpos) ()	= (void (*)())  isetpos;

void		 (*cclose)  ()		= (void (*)())  iclose;
int		 (*cfillup) ()		= (int (*)())   ifillup;
ulong	 	 (*cgetpos) ()		= (ulong (*)()) igetpos;
FILE		*(*copen)   ()		= (FILE *(*)()) iopen;
void		 (*csetpos) ()		= (void (*)())  isetpos;

#else  /* !defined(ALLOW_MIDDLE_LAYER_BYPASS) */

static void	 (*default_close)  ()	= (void (*)())  lclose;
static int	 (*default_fillup) ()	= (int (*)())   lfillup;
static ulong 	 (*default_getpos) ()	= (ulong (*)()) lgetpos;
static FILE	*(*default_open)   ()	= (FILE *(*)()) lopen;
static void	 (*default_setpos) ()	= (void (*)())  lsetpos;

void		 (*cclose)  ()		= (void (*)())  lclose;
int		 (*cfillup) ()		= (int (*)())   lfillup;
ulong	 	 (*cgetpos) ()		= (ulong (*)()) lgetpos;
FILE		*(*copen)   ()		= (FILE *(*)()) lopen;
void		 (*csetpos) ()		= (void (*)())  lsetpos;

#endif /* ALLOW_MIDDLE_LAYER_BYPASS */

/* ---------------------------------------------------------------------
** clfile
**
** If the given argument "on" is non-zero then enable the intermediate
** fully line buffered file input layer (i.e. cause all input file
** accesses to go through the "lfile.[ch]" routines.
**
** If the given argument "on" is zero then disable the intermediate
** fully line buffered file input layer and cause all input file
** accesses to bypass the "lfile.[ch]" routines and go directly through
** to the "ifile.[ch]" routines.
** ------------------------------------------------------------------- */

void
clfile (on)
int on;
{
	if (on) {
		cclose	= lclose;
		cfillup	= lfillup;
		cgetpos	= lgetpos;
		copen	= lopen;
		csetpos	= lsetpos;
	}
	else {
		cclose	= default_close;
		cfillup	= default_fillup;
		cgetpos	= default_getpos;
		copen	= default_open;
		csetpos	= default_setpos;
	}
}

