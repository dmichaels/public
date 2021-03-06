/* CPP pp_lfile.c - Intermediate level line buffered file input routines */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  08.23.91  DGM  001  Fix (from PLB) in lfillup.
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  05.02.90  DGM  (002) Increased MAX_PUSH_BACK from 3 to 6 for handling
 *			 of arbitrarily long string-literal, see pp.pl1.
 *  07.14.89  DGM  (001) Minor reorganization.
 *  06.05.89  DGM  (000) Minor reorganization (name changed from bfile).
 *  02.09.89  DGM  (000) Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
** Version and copyright stamp
** ------------------------------------------------------------------- */

static char *_VERSION_ = "@(#)LPI 08.23.91 001 pplfile.c";

/* ---------------------------------------------------------------------
** Include files
** ------------------------------------------------------------------- */

#include "pp_lfile.h"
#include "pp_char.h"
#include "pp_sys.h"

/* ---------------------------------------------------------------------
** Exported external function declarations
** ------------------------------------------------------------------- */

void	 lclose ();
int	 lfillup ();
ulong	 lgetpos ();
LFILE	*lopen ();
void	 lsetpos ();

/* ---------------------------------------------------------------------
** Local macro definitions
** ------------------------------------------------------------------- */

#define MAX_OPEN_LFILE		MAX_OPEN_IFILE
#define MAX_PUSH_BACK		6
#define PUSH_BACK_BUFFER_SIZE	(MAX_PUSH_BACK - 1)
#define DEFAULT_BUFFER_SIZE	1024
#define SMALL_BUFFER_SIZE	256
#define TOTAL_BUFFER_SIZE	(DEFAULT_BUFFER_SIZE+PUSH_BACK_BUFFER_SIZE)

/* ---------------------------------------------------------------------
** Local static data
** ------------------------------------------------------------------- */

static LFILE	FileTable [MAX_OPEN_LFILE] =
{
	  { NULL , 0 , NULL , NULL , 0 , NULL }
	, { NULL , 0 , NULL , NULL , 0 , NULL }
	, { NULL , 0 , NULL , NULL , 0 , NULL }
	, { NULL , 0 , NULL , NULL , 0 , NULL }
	, { NULL , 0 , NULL , NULL , 0 , NULL }
	, { NULL , 0 , NULL , NULL , 0 , NULL }
	, { NULL , 0 , NULL , NULL , 0 , NULL }
	, { NULL , 0 , NULL , NULL , 0 , NULL }
};

static LFILE	*EndFileTable  = & FileTable [MAX_OPEN_LFILE];
static int	 OpenFileCount = 0;

static uchar	 SmallBuffer [MAX_OPEN_LFILE]
			     [SMALL_BUFFER_SIZE + PUSH_BACK_BUFFER_SIZE];

/* ---------------------------------------------------------------------
** lopen
**
** Open the file specified by the "name" argument for input.  Returns
** an input file pointer corresponding to the opened file.  If the file
** could not be opened (either because the file could not be found, or
** because the maximum number of open files have already been opened),
** then a null pointer is returned.
** ------------------------------------------------------------------- */

LFILE *
lopen (name)
char *name;
{
	register LFILE *f;

	/* Search for an empty slot in the file table */

	for (f = FileTable ; f->current_char != NULL ; f++)
		if (f >= EndFileTable)
			return (NULL);

	/* Try to open the file */

	if ((f->fid = iopen(name)) == NULL)
		return (NULL);

	/* Allocate an input buffer if this slot doesn't have one */

	if (f->buffer_base == NULL) {
		if ((f->buffer_base =
		    (uchar *) SYS_ALLOC (TOTAL_BUFFER_SIZE)) == NULL) {
			f->buffer_base = SmallBuffer[f-FileTable];
			f->buffer_size = SMALL_BUFFER_SIZE;
		}
		else	f->buffer_size = DEFAULT_BUFFER_SIZE;
	}

	/* Initialize the input file data */

	f->buffer_start = f->buffer_base + PUSH_BACK_BUFFER_SIZE;
	f->chars_left = 0;
	f->current_char = f->buffer_start + 1;
	OpenFileCount++;
	return (f);
}

/* ---------------------------------------------------------------------
** lclose
**
** Closes the file specified by the "f" argument.
** If no more files are open, then deallocate all of the buffers.
** ------------------------------------------------------------------- */

void
lclose (f)
register LFILE *f;
{
	iclose (f->fid);
	f->current_char = NULL;

	/* Deallocate all buffers if this is no more files are open */

	if (--OpenFileCount > 0) return;

	for (f = FileTable ; f <= EndFileTable ; f++) {
		if ((f->buffer_base != NULL) &&
		    (f->buffer_size == DEFAULT_BUFFER_SIZE)) {
			SYS_FREE (f->buffer_base);
			f->buffer_base = NULL;
		}
	}
}

/* ---------------------------------------------------------------------
** lgetpos
**
** Returns in an unsigned long integer, the current file position
** indicator of the input file specified to by the "f" argument.
** If an error occurs, then return 0.
** ------------------------------------------------------------------- */

ulong
lgetpos (f)
register LFILE *f;
{
	ulong position;

	if ((position = igetpos (f->fid)) < 0)
		return (0);
	if ((f->chars_left > 0) && (f->chars_left <= position))
		position -= f->chars_left;
	return (position);
}

/* ---------------------------------------------------------------------
** lsetpos
**
** Sets the file position indicator of the input file specified
** by the "f" argument, to the value specified by the "position"
** argument.  If an error occurs, then the file is set to end-of-file.
** This routine erases all memory of any pushed back characters.
** ------------------------------------------------------------------- */

void
lsetpos (f, position)
register LFILE *f;
ulong position;
{
	f->chars_left = 0;
	f->current_char = f->buffer_start + 1;
	isetpos (f->fid, position);
}

/* ---------------------------------------------------------------------
** lfillup
**
** Fills up the (presumably empty) input buffer of the input file
** specified by the "f" argument with the next physical *line* of
** input, and returns the first character in the buffer (and updates
** the buffer accordingly).  If an error occurs, then the file is
** set to end-of-file, and LEOF is returned.
**
** In addition, the which was line just read is passed along with its
** length to the routine "void PPLSLN (char *, short *)", which will
** (presumably) print the given source line to a listing file.
** ------------------------------------------------------------------- */

int
lfillup (f)
LFILE *f;
{
	register int	  c;
	register int	  n	= 0;
	register IFILE	* fid	= f->fid;
	register int	  max	= f->buffer_size;
	register uchar	* p	= f->buffer_start;
	short		  fb15;

	extern void	  PPLSLN ();	/* Print source line routine */

	/* See if we've already hit end-of-file */

	if (p == NULL) return (LEOF);

	/* Fill up the buffer up to a new-line */

	while (1) {

		if (n >= max)
			break;

		if ((c = igetc (fid)) == IEOF)
			break;

		n++;

		if ((*p++ = c) == NEWLINE_CHAR)
			break;
	}

	/* Set the number of character left in the buffer (zero if EOF) */

	f->chars_left = n;

	/* Dump the line to the listing file */

	fb15 = f->chars_left;
	PPLSLN (f->buffer_start, &fb15);

	/* Initialize the rest of the LFILE */

	if (f->chars_left <= 0) {
		f->chars_left = 0;
		f->buffer_start = NULL;
		return (LEOF);
	}

	else {
		f->current_char = f->buffer_start;
		f->chars_left--;
		return (*f->current_char++);
	}
}

