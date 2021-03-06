/* CPP pp_sys.h - System interface header (unix) */

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
 *  05.30.91  DGM  001  Added SYS_FILE_EXISTS and SYS_FILE_IS_READABLE.
 *  06.08.90  DGM  000  Original.
 *
 ***********************************************************************/

#ifndef _SYS_H_
#define _SYS_H_

/* ---------------------------------------------------------------------
** Imported external function declarations
** ------------------------------------------------------------------- */

extern int	  access ();
extern int	  close ();
extern int	  free ();
extern long	  lseek ();
extern char	* malloc ();
extern int	  open ();
extern int	  read ();
extern int	  write ();

/* ---------------------------------------------------------------------
** Type declarations
** ------------------------------------------------------------------- */

typedef int	FID;	/* File identifier (descriptor) type */

/* ---------------------------------------------------------------------
** Macro function declarations
** ------------------------------------------------------------------- */

#define SYS_ALLOC(n)		(malloc((unsigned int)(n)))
#define SYS_CLOSE(f)		((void)close((FID)(f)))
#define SYS_FILE_EXISTS(s)	(!access(s,0))
#define SYS_FILE_IS_READABLE(s)	(!access(s,4))
#define SYS_FREE(p)		((void)free((char *)(p)))
#define SYS_GETPOS(f)		(lseek((FID)(f),0L,1))
#define SYS_OPEN(s)		(open((char *)(s),0))
#define SYS_READ(f,s,n)		(read((FID)(f),(char *)(s),(int)(n)))
#define SYS_SETPOS(f,o)		((void)lseek((FID)(f),(long)(o),0))
#define SYS_WRITE(f,s,n)	(write((FID)(f),(char *)(s),(int)(n)))

/* ---------------------------------------------------------------------
** Macro definitions
** ------------------------------------------------------------------- */

#define STDIN	0
#define STDOUT	1
#define STDERR	2

/* ---------------------------------------------------------------------
/* EOF_MARKER
/*
/* UNIX has no special end-of-file character within the file.
/* ------------------------------------------------------------------- */

#undef EOF_MARKER

/* ---------------------------------------------------------------------
/* EOL_IS_CRLF
/*
/* UNIX just uses a plain linefeed (newline) to indicate end-of-line.
/* ------------------------------------------------------------------- */

#undef EOL_IS_CRLF

/* Allow minimal buffering (see pp_cfile.[ch]) */

#define ALLOW_MINIMAL_BUFFERING

/* Target system identification definitions */

#define UNIX

#endif  /* _SYS_H_ */

