/* C++ mon.c - Monitor front-end performance */

/* Copyright (c) 1991 by Liant Software Corp. */

/**********************************************************************
 * This product is the property of Liant Software Corporation         *
 * and is licensed pursuant to a written license agreement.           *
 * No portion of this product may be reproduced without the written   *
 * permission of Liant Software Corp. except pursuant to the          *
 * license agreement.                                                 *
 **********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  02.06.92  BA   003  Add support for i386 targets.  Fixed copyright.
 *  12.16.91  DGM  002  Updated.
 *  04.18.91  DGM  001  Conditional compile for SUN/3 only.
 *  04.01.91  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* External declarations
/* ------------------------------------------------------------------- */

#if defined (sun) && defined (unix)
#if defined (__STDC__) || defined (__cplusplus)
extern monstartup (int (*) (), int (*) ());
extern monitor (int (*) ());
#else
extern monstartup ();
extern monitor ();
#endif
extern int etext ();
extern int _lpistart ();
#endif

#if defined (i386) && defined (unix)
#include <mon.h>
#define buffer_size 400000
#define bufsize 400000 / 2
#define nfunc 3072
WORD buffer[buffer_size];
extern int etext ();
WORD *_countbase = 0;
#endif

/* ---------------------------------------------------------------------
/* BEGIN_MONITOR
/* ------------------------------------------------------------------- */

void
BEGIN_MONITOR ()
{
#if defined (sun) && defined (unix)
	monstartup ((int (*) ()) _lpistart, etext);
#endif
#if defined (i386) && defined (unix)
	_countbase = buffer;
	monitor ((int (*) ()) 2, etext, buffer, bufsize, nfunc);

#endif
}

/* ---------------------------------------------------------------------
/* END_MONITOR
/* ------------------------------------------------------------------- */

void
END_MONITOR ()
{
#if defined (sun) && defined (unix)
	monitor (0);
#endif
#if defined (i386) && defined (unix)
	monitor (0,0,0,0,0);
#endif
}

