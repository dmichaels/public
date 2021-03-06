/* CPP pp_lfile.h - Intermediate level line buffered file input header */

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
 *  07.14.89  DGM  (001) Minor reorganization.
 *  06.05.89  DGM  (000) Minor reorganization (name changed from bfile).
 *  02.09.89  DGM  (000) Original.
 *
 ***********************************************************************/

#ifndef _LFILE_H_
#define _LFILE_H_

/* ---------------------------------------------------------------------
** Include files
** ------------------------------------------------------------------- */

#include "pp_defs.h"
#include "pp_ifile.h"

/* ---------------------------------------------------------------------
** Macro object definitions
** ------------------------------------------------------------------- */

#define LEOF		(-1)	/* End of file character */

/* ---------------------------------------------------------------------
** Type definitions
** ------------------------------------------------------------------- */

typedef struct {
	uchar	*current_char;	/* pointer to current character in buffer */
	int 	 chars_left;	/* number of characters left in buffer */
	uchar	*buffer_start;	/* virtual start of buffer */
	uchar	*buffer_base;	/* physical base of buffer */
	int	 buffer_size;	/* total physical buffer size */
	IFILE	*fid;		/* file identifier (descriptor) */
} LFILE;

/* ---------------------------------------------------------------------
** External function declarations
** ------------------------------------------------------------------- */

extern void		 lclose  ();
extern int		 lfillup ();
extern ulong	 	 lgetpos ();
extern LFILE		*lopen   ();
extern void		 lsetpos ();

/* ---------------------------------------------------------------------
** Macro function definitions
** ---------------------------------------------------------------------
** lgetc	- Advances the input stream pointer of the input file
**		  specified by the "f" argument, and returns the next
**		  character.  This may be called after BEOF has been
**		  reached, in this case BEOF will simply be returned.
** ---------------------------------------------------------------------
** lpeekc	- Returns the next character which will be read by the
**		  next "igetc(f)" call.  It does not advance the input
**		  stream pointer.
** ---------------------------------------------------------------------
** lungetc	- Pushes the character specified by the "c" argument
**		  back onto the input stream of the input file specified
**		  by the "f" argument.
**
**		  An end-of-file character will not be be pushed back
**		  onto the input stream, however it is not an error to
**		  request this, it will simply be ignored.
**
**		  If no more characters can be pushed back, then this
**		  push back character "c" will be ignored, and BEOF will
**		  be returned, otherwise, "c" will be returned.
**
**		  A maximum of MAX_PUSH_BACK characters may *safely* be
**		  pushed back (guaranteed); more (up to (buffer_size +
**		  PUSH_BACK_BUFFER_SIZE)) may work depending on the state
**		  of the buffer (see lfile.c).
**
**		  Note that characters MAY be pushed back onto the input
**		  stream if it is at either the end-of-file or at the
**		  beginning-of-file.
** ------------------------------------------------------------------- */

#define	_lgetc_(f)	((((f)->chars_left) > 0) ? \
			 ((f)->chars_left--, \
			  (int)(*(f)->current_char++)) : \
			  FILLUP(f))

#define	_lpeekc_(f)	((((f)->chars_left) > 0) ? \
			 ((int)(*(f)->current_char)) : \
			 (FILLUP(f) == EOF ? \
			  EOF : \
			  ((f)->chars_left++, \
			   (int)(*(--((f)->current_char))))))

#define _lungetc_(f,c)	(((c) == EOF) || \
			 ((f)->current_char <= (f)->buffer_base) ? \
			 EOF : \
			 ((f)->chars_left++, \
			  (int)(*(--((f)->current_char)) = (c))))

#ifdef  MSDOS

#define lgetc(f)	((_lpeekc_(f) == EOF_MARKER) ? \
			 (EOF) :  \
			 ((_lpeekc_(f) == '\r') ? \
			   ((_lgetc_(f), _lpeekc_(f) == '\n') ? \
			     (_lgetc_(f)) : \
			     ('\r')) : \
			   (_lgetc_(f)))) 

#define lpeekc(f)	((_lpeekc_(f) == EOF_MARKER) ? \
			 (EOF) : \
			 ((_lpeekc_(f) == '\r') ? \
			   ((_lgetc_(f), _lungetc_((f),_lgetc_(f)) == '\n') ? \
			     ('\n') : \
			     (_lungetc_((f),'\r'))) : \
			   (_lpeekc_(f))))

#define lungetc(f,c)	(_lungetc_(f,c))

#else   /* !defined(MSDOS) */

#define lgetc(f)	_lgetc_(f)
#define lungetc(f,c)	_lungetc_(f,c)
#define lpeekc(f)	_lpeekc_(f)

#endif  /* MSDOS */

#endif  /* _LFILE_H_ */

