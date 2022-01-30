/* CPP pp_ifile.c - Lowest level file input routines */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  10.27.92  DGM  002  Changed ushort type to ulong to avoid
 *			unsigned-preserving vs. value-preserving
 *			rule conflict (caught by MetaWare compiler).
 *  11.12.91  DGM  001  Updated.
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  05.02.90  DGM  (005) Increased MAX_PUSH_BACK from 3 to 6 for handling
 *			 of arbitrarily long string-literal, see pp.pl1.
 *  07.14.89  DGM  (004) Minor reorganization.
 *  06.05.89  DGM  (003) Minor reorganization.
 *  08.09.89  DGM  (002) Miscellaneous changes for new buffering.
 *  08.10.88  DGM  (001) Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Version and copyright stamp
/* ------------------------------------------------------------------- */

static char *VERSION__ = "@(#)LPI 10.27.92 002 ppifile";

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include "pp_ifile.h"
#include "pp_char.h"

/* ---------------------------------------------------------------------
/* Local macro definitions
/* ------------------------------------------------------------------- */

#define MAX_OPEN_FILES		8
#define MAX_PUSH_BACK		17
#define PUSH_BACK_BUFFER_SIZE	(MAX_PUSH_BACK - 1)
#define DEFAULT_BUFFER_SIZE	8192
#define TOTAL_BUFFER_SIZE	(DEFAULT_BUFFER_SIZE+PUSH_BACK_BUFFER_SIZE)
#define SMALL_BUFFER_SIZE	128

/* ---------------------------------------------------------------------
/* Local static data
/* ------------------------------------------------------------------- */

static IFILE	FileTable [MAX_OPEN_FILES] =
{
	  { NULL , 0 , NULL , NULL , 0 , 0 }
	, { NULL , 0 , NULL , NULL , 0 , 0 }
	, { NULL , 0 , NULL , NULL , 0 , 0 }
	, { NULL , 0 , NULL , NULL , 0 , 0 }
	, { NULL , 0 , NULL , NULL , 0 , 0 }
	, { NULL , 0 , NULL , NULL , 0 , 0 }
	, { NULL , 0 , NULL , NULL , 0 , 0 }
	, { NULL , 0 , NULL , NULL , 0 , 0 }
};

static IFILE	*EndFileTable  = & FileTable [MAX_OPEN_FILES];
static int	 OpenFileCount = 0;

static uchar	 SmallBuffer [MAX_OPEN_FILES]
			     [SMALL_BUFFER_SIZE + PUSH_BACK_BUFFER_SIZE];

/* ---------------------------------------------------------------------
/* iopen
/*
/* Open the file specified by the "name" argument for input.  Returns
/* an input file pointer corresponding to the opened file.  If the file
/* could not be opened (either because the file could not be found, or
/* because the maximum number of open files have already been opened),
/* then a null pointer is returned.
** ------------------------------------------------------------------- */

extern
IFILE *
iopen (name, listing_mode)
char	*name;
ushort	 listing_mode;
{
	register IFILE *f;

	/* Search for an empty slot in the file table */

	for (f = FileTable ; f->current_char != NULL ; f++)
		if (f >= EndFileTable)
			return (NULL);

	/* Try to open the file */

	if ((f->fid = SYS_OPEN (name)) < 0)
		return (NULL);

	/* Allocate an input buffer if this slot doesn't have one */

	if (f->buffer_base == NULL) {
		if ((f->buffer_base =
		    (uchar *) SYS_ALLOC (TOTAL_BUFFER_SIZE)) == NULL) {
			f->buffer_base = SmallBuffer [f - FileTable];
			f->buffer_size = SMALL_BUFFER_SIZE;
		}
		else	f->buffer_size = DEFAULT_BUFFER_SIZE;
		f->buffer_start = f->buffer_base + PUSH_BACK_BUFFER_SIZE;
	}

	/* Initialize the input file data */

	f->listing_mode		= listing_mode;
	f->original_chars_left	= 0;
	f->chars_left		= 0;
	f->current_char		= f->buffer_start + 1;

	/* Increment the open file count */

	OpenFileCount++;

	/* Return a pointer to the new IFILE */

	return (f);
}

/* ---------------------------------------------------------------------
/* iclose
/*
/* Closes the file specified by the "f" argument.
/* If no more files are open, then deallocate all of the buffers.
/* ------------------------------------------------------------------- */

extern
void
iclose (f)
register IFILE *f;
{
	/* Close the file */

	SYS_CLOSE (f->fid);
	f->current_char = NULL;

	/* Decrement the open file count */

	if (--OpenFileCount > 0)
		return;

	/* Deallocate all buffers if this is no more files are open */

	for (f = FileTable ; f <= EndFileTable ; f++) {
		if ((f->buffer_base != NULL) &&
		    (f->buffer_size == DEFAULT_BUFFER_SIZE)) {
			SYS_FREE (f->buffer_base);
			f->buffer_base = NULL;
		}
	}
}

/* ---------------------------------------------------------------------
/* igetpos
/*
/* Returns in an unsigned long integer, the current file position
/* indicator of the input file specified to by the "f" argument.
/* If an error occurs, then return 0.
/* ------------------------------------------------------------------- */

extern
ulong
igetpos (f)
register IFILE *f;
{
	long	position;
	ulong	chars_left;

	if ((position = SYS_GETPOS (f->fid)) < 0)
		return (0);
	if (f->listing_mode)
		chars_left = (f->original_chars_left -
			      (f->current_char - f->buffer_start));
	else	chars_left = f->chars_left;
	if ((chars_left > 0) && (chars_left <= position))
		position -= chars_left;
	return ((ulong)position);
}

/* ---------------------------------------------------------------------
/* isetpos
/*
/* Sets the file position indicator of the input file specified
/* by the "f" argument, to the value specified by the "position"
/* argument.  If an error occurs, then the file is set to end-of-file.
/* This routine erases all memory of any pushed back characters.
/* ------------------------------------------------------------------- */

void
isetpos (f, position)
register IFILE *f;
ulong position;
{
	f->chars_left = 0;
	f->current_char = f->buffer_start + 1;
	SYS_SETPOS (f->fid, position);
}

/* ---------------------------------------------------------------------
/* print_line_to_listing_file
/* ------------------------------------------------------------------- */

static
void
print_line_to_listing_file (line_ptr, line_length) 
uchar *line_ptr;
long line_length;
{
	extern void PPLSLN ();

	PPLSLN (&line_ptr, &line_length);
}

/* ---------------------------------------------------------------------
/* ifillup
/*
/* Fills up the (presumably empty) input buffer of the input file
/* specified by the "f" argument, and returns the first character
/* in the buffer (and updates the buffer accordingly).  If an error
/* occurs, then the file is set to end-of-file, and IEOF is returned.
/* ------------------------------------------------------------------- */

extern
int
ifillup (f)
register IFILE *f;
{
	int	 n;
	uchar	*p;

	if (f->listing_mode) {
		f->chars_left = (f->original_chars_left -
				 (f->current_char - f->buffer_start));
		if (f->chars_left > 0) {
			p = f->current_char;
			for (n = 0 ; n < f->chars_left ; n++) {
				if (*p++ == NEWLINE_CHAR) {
					n++;
					break;
				}
			}
			print_line_to_listing_file (f->current_char, n);
			f->chars_left = n - 1;
			return (*f->current_char++);
		}
		else {
			f->chars_left =
			f->original_chars_left =
			SYS_READ (f->fid, f->buffer_start, f->buffer_size);
			if (f->chars_left <= 0) {
				print_line_to_listing_file (0, 0);
				return (IEOF);
			}
			p = f->buffer_start;
			for (n = 0 ; n < f->chars_left ; n++) {
				if (*p++ == NEWLINE_CHAR) {
					n++;
					break;
				}
			}
			print_line_to_listing_file (f->buffer_start, n);
			f->chars_left = n;
		}
	}
	else {
		f->chars_left =
		SYS_READ (f->fid, f->buffer_start, f->buffer_size);
		if (f->chars_left <= 0)
			return (IEOF);
	}
	f->chars_left--;
	f->current_char = f->buffer_start;
	return (*f->current_char++);
}

/* ---------------------------------------------------------------------
/* igetc
/* ipeekc
/* iungetc
/* ------------------------------------------------------------------- */

#ifndef __USE_INLINE_IFILE_ROUTINES__

#include <stdio.h>

extern
int
igetc (f)
register IFILE *f;
{
	register int c;

	/* return (__igetc__(f)); */

	if (f->chars_left > 0) {
		if (f->current_char > (f->buffer_start + f->buffer_size - 1))
			printf ("igetc: overflow!\n");
		f->chars_left--;
		c = *f->current_char;
		f->current_char++;
		return (c);
	}
	return (ifillup (f));
}

extern
int
ipeekc (f)
register IFILE *f;
{
	register int c;

	/* return (__ipeekc__(f)); */

	if (f->chars_left > 0)
		return (*f->current_char);
	if (ifillup (f) == IEOF)
		return (IEOF);
	f->chars_left++;
	f->current_char--;
	c = *f->current_char;
	return (c);
}

extern
int
iungetc (f, c)
register IFILE *f;
register int	c;
{
	/* return (__iungetc__(f, c)); */

	if ((c == IEOF) || (f->current_char <= f->buffer_base))
		return (IEOF);
	f->chars_left++;
	f->current_char--;
	*f->current_char = (uchar)c;
	return (c);
}

#endif /* __USE_INLINE_IFILE_ROUTINES__ */

