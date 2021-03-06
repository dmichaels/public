/* CPP pp_ifile.h - Lowest level file input header */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  11.12.91  DGM  001  Updated.
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  07.14.89  DGM  (006) Minor reorganization.
 *  06.05.89  DGM  (005) Minor reorganization.
 *  02.09.89  DGM  (004) Miscellaneous changes for new buffering.
 *  08.10.88  DGM  (003) Original.
 *
 ***********************************************************************/

#ifndef __IFILE_H__
#define __IFILE_H__

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include "pp_defs.h"
#include "pp_sys.h"

/* ---------------------------------------------------------------------
/* Configuration
/* ------------------------------------------------------------------- */

#define __USE_INLINE_IFILE_ROUTINES__

/* ---------------------------------------------------------------------
/* Macro object definitions
/* ------------------------------------------------------------------- */

#define IEOF		(-1)	/* End of file character */

/* ---------------------------------------------------------------------
/* Type definitions
/* ------------------------------------------------------------------- */

typedef struct {
	uchar	*current_char;	      /* ptr to current char in buffer */
	int 	 chars_left;	      /* # of chars left in buffer */
	int 	 original_chars_left; /* # of chars left in buffer */
	uchar	*buffer_start;	      /* virtual start of buffer */
	uchar	*buffer_base;	      /* physical base of buffer */
	int	 buffer_size;	      /* total physical buffer size */
	FID	 fid;		      /* file descriptor */
	ushort	 listing_mode;
} IFILE;

/* ---------------------------------------------------------------------
/* External function declarations
/* ------------------------------------------------------------------- */

extern void	 iclose  ( /* IFILE * */ );
extern int	 ifillup ( /* IFILE * */ );
extern ulong 	 igetpos ( /* IFILE * */ );
extern IFILE	*iopen   ( /* IFILE *, ushort */ );
extern void	 isetpos ( /* IFILE *, ulong */ );

/* ---------------------------------------------------------------------
/* Macro function definitions
/* ---------------------------------------------------------------------
/* igetc	- Advances the input stream pointer of the input file
/*		  specified by the "f" argument, and returns the next
/*		  character.  This may be called after IEOF has been
/*		  reached, in this case IEOF will simply be returned.
/* ---------------------------------------------------------------------
/* ipeekc	- Returns the next character which will be read by the
/*		  next "igetc(f)" call.  It does not advance the input
/*		  stream pointer.
/* ---------------------------------------------------------------------
/* iungetc	- Pushes the character specified by the "c" argument
/*		  back onto the input stream of the input file specified
/*		  by the "f" argument.
/*
/*		  An end-of-file character will not be be pushed back
/*		  onto the input stream, however it is not an error to
/*		  request this, it will simply be ignored.
/*
/*		  If no more characters can be pushed back, then this
/*		  push back character "c" will be ignored, and IEOF will
/*		  be returned, otherwise, "c" will be returned.
/*
/*		  A maximum of MAX_PUSH_BACK characters may *safely* be
/*		  pushed back (guaranteed); more (up to (buffer_size +
/*		  PUSH_BACK_BUFFER_SIZE)) may work depending on the state
/*		  of the buffer (see ifile.c).
/*
/*		  Note that characters MAY be pushed back onto the input
/*		  stream if it is at either the end-of-file or at the
/*		  beginning-of-file.
/* ------------------------------------------------------------------- */

#define	__igetc__(f)		((((f)->chars_left) > 0) ? \
				 ((f)->chars_left--, \
				  (int)(*(f)->current_char++)) : \
				   ifillup(f))

#define	__ipeekc__(f)		((((f)->chars_left) > 0) ? \
				 ((int)(*(f)->current_char)) : \
				 (ifillup(f) == IEOF ? \
				  IEOF : \
				  ((f)->chars_left++, \
				   (int)(*(--((f)->current_char))))))

#define __iungetc__(f,c)		(((c) == IEOF) || \
				 ((f)->current_char <= (f)->buffer_base) ? \
				 IEOF : \
				 ((f)->chars_left++, \
				  (int)(*(--((f)->current_char)) = (c))))

#ifdef __USE_INLINE_IFILE_ROUTINES__

#define igetc(f)		__igetc__(f)
#define ipeekc(f)		__ipeekc__(f)
#define iungetc(f,c)		__iungetc__(f,c)

#else 

extern int	 igetc	 ( /* IFILE * */ );
extern int	 ipeekc	 ( /* IFILE * */ );
extern int	 iungetc ( /* IFILE *, int */ );

#endif

#endif /* __IFILE_H__ */

