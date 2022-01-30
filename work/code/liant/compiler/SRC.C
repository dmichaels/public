/* ANSI-C src.c - Source file manipulation routines */

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
 *  11.21.89  DGM  (010) Minor name changes.
 *  10.17.89  PLB  (009) Changes for New Globals/Utilities.
 *  07.21.89  DGM  (008) Minor changes to pass lint cleanly.
 *  06.05.89  DGM  (007) Minor reorganization.
 *  02.06.89  DGM  (006) Added "listing_flag".
 *  02.02.89  DGM  (005) Nothing really.
 *  01.26.89  DGM  (004) Removed code to deallocate "assumed_name".
 *  08.10.88  DGM  (003) Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
** Version and copyright stamp
** ------------------------------------------------------------------- */

static char *_VERSION_ =

"@(#)src.c  010  11/21/89  (c) 1989 Language Processors, Inc.";

/* ---------------------------------------------------------------------
** Include files
** ------------------------------------------------------------------- */

#include "cfile.h"
#include "src.h"
#include "charset.h"

/* ---------------------------------------------------------------------
** External static data definitions
** ------------------------------------------------------------------- */

SOURCE	*SSRC	= NULL;

/* ---------------------------------------------------------------------
** Internal static data definitions
** ------------------------------------------------------------------- */

static SOURCE	MainSourceFile =
{
		  NULL_STRING		/* physical_name */
		, NULL_STRING		/* assumed_name */
		, (ulong) 0		/* physical_line */
		, (ulong) 0		/* line_adjustment */
		, (ushort) 0		/* include_level */
		, (SOURCE *) NULL	/* next_down */
		, (ushort) 0		/* flag */
		, (FILE *) NULL		/* file_pointer */
		, (ulong) 0		/* file_position */
};

static int	StackDepth = 0;

/* ---------------------------------------------------------------------
** Exported external function declarations
** ------------------------------------------------------------------- */

short	XSOPEN ();	/* Open a source file; return nesting level */
short	XSCLOSE ();	/* Close a source file; return nesting level */

/* ---------------------------------------------------------------------
** Local macro function definitions
** ------------------------------------------------------------------- */

#define SRC_EMPTY_STACK()	(StackDepth <= 0)

/* ---------------------------------------------------------------------
** save_string 
**
** Given a (pointer to a) PL/I character varying type string, allocate
** space for it, copy it elsewhere, AND add a null byte (EOS) onto
** the end of the text of the string so it may be easily used by both C
** and PL/I code.  Return a pointer to the saved string.
** ------------------------------------------------------------------- */

static
STRING *
save_string (s)
register STRING *s;
{
	register STRING *p;
	register int i, n;

	if ((s == NULL_STRING) || (s->length <= 0) || (s->text[0] == EOS))
		return (NULL_STRING);
	if ((p = (STRING *) SYS_ALLOC (sizeof(STRING)+s->length)) == NULL)
		return (NULL_STRING);
	for (p->length = s->length, n = p->length, i = 0 ; i < n ; i++)
		p->text[i] = s->text[i];
	p->text[i] = EOS;
	return (p);
}

/* ---------------------------------------------------------------------
** XSOPEN
**
** Opens the source file specified by the given name (which is assumed
** to be a PL/I character varying string) and allocates a SOURCE
** record for it and pushes it onto the top of the SOURCE stack.
** All subsequent reads (via the scan.c routines) will be from the
** source file on the top of the stack.
**
** If all is well, return the (greater-than-zero) number of "open" files
** (i.e. the depth of the source file stack), otherwise returns zero.
** ------------------------------------------------------------------- */

short
XSOPEN (name, listing)
STRING *name;
short *listing;
{
	STRING	*sname;		/* source file name pointer */
	SOURCE	*snode;		/* source file stack node pointer */
	FILE	*fp;		/* file pointer */

	/*
	/* Check the listing flag.
	/*
	/* If the listing flag is on, then open this source file
	/* in listing mode; i.e. use the intermediate line buffered
	/* source file input layer (LFILE).
	/*
	/* If the listing flag is off, then open this source file
	/* in non-listing mode; i.e. may by-pass the intermediate
	/* line buffered source file input layer (LFILE).
	/**/

	if (*listing)
		clfile (1);
	else	clfile (0);

	/* Save the file name in a C-style string */

	sname = save_string (name);

	/* Try to open the file */

	if ((fp = OPEN (sname->text)) == NULL) {
		/*
		/* Here, the open failed.  Close a previously opened
		/* file (while saving its current state) and try again.
		/**/
		if (SRC_EMPTY_STACK ()) {
			SYS_FREE (sname);
			return (0);
		}
		SSRC->file_position = GETPOS (SSRC->file_pointer);
		CLOSE (SSRC->file_pointer);
		if ((fp = OPEN (sname->text)) == NULL) {
			SSRC->file_pointer = OPEN (SSRC->physical_name->text);
			SETPOS (SSRC->file_pointer, SSRC->file_position);
			SYS_FREE (sname);
			return (0);
		}
		SSRC->flag |= _SRC_CLOSED;
		SSRC->file_pointer = NULL;
	}

	/* If the stack is empty use the static main source node */

	if (SRC_EMPTY_STACK ()) {
		SSRC = (SOURCE *)&MainSourceFile;
		SSRC->next_down = NULL;
	}

	/* Otherwise, allocate a new source stack node */

	else {
		if ((snode = (SOURCE *) SYS_ALLOC (sizeof(SOURCE))) == NULL) {
			CLOSE (fp);
			return (0);
		}
		snode->next_down = SSRC;
		SSRC = snode;
	}

	/* Update the stack depth and fill in the source file data */

	SSRC->include_level	= StackDepth++;
	SSRC->physical_name	= sname;
	SSRC->assumed_name	= NULL_STRING;
	SSRC->physical_line	= (ulong)0;
	SSRC->line_adjustment	= (ulong)0;
	SSRC->flag		= (ushort)0;
	SSRC->file_pointer	= fp;
	SSRC->file_position	= (ulong)0;

	/* Turn on the source file listing flag if appropriate */

	if (*listing) SSRC->flag |= _SRC_LISTING;

	/*
	/* Now, push back a new-line onto the beginning-of-file.
	/* This a (nice?) hack so that the SSCAN () routine (scan.c)
	/* doesn't have to check for beginning-of-file on every call.
	/**/

	UNGETC (fp, NEWLINE_CHAR);

	/* Return the depth of the stack (i.e. the number of "open" files) */

	return (StackDepth);
}

/* ---------------------------------------------------------------------
** XSCLOSE
**
** Closes the file on the top of the source file state stack.
**
** If there are no more files on the stack or if an error occured such
** such that the new current source file on top of stack file cannot be
** read, then return zero, otherwise return the (non-zero) depth of the
** the source file stack (i.e the number of "open" files).
** ------------------------------------------------------------------- */

short
XSCLOSE ()
{
	register SOURCE *p;

	/* Check for stack underflow */

	if (SRC_EMPTY_STACK ())
		return (0);

	/* Close the file */

	CLOSE (SSRC->file_pointer);

	/* Pop the stack */

	StackDepth--;

	if (SRC_EMPTY_STACK ()) {
		/*
		/* Here, the main source file was just closed.
		/**/
		SSRC->include_level = StackDepth;
		SSRC->file_pointer = NULL;
		return (0);
	}
	else {
		p = SSRC;
		SSRC = p->next_down;
		SYS_FREE (p);
	}

	/*
	/* If the now-current source file was previously
	/* closed (i.e. in XSOPEN) then re-open it now.
	/**/

	if (SSRC->flag & _SRC_CLOSED) {
		if ((SSRC->file_pointer =
		     OPEN ((SSRC->physical_name)->text)) == NULL)
			return (0);
		SETPOS (SSRC->file_pointer, SSRC->file_position);
		SSRC->flag &= ~_SRC_CLOSED;
	}

	/* Check the listing flag for this file */

	if (SSRC->flag & _SRC_LISTING)
		clfile (1);
	else	clfile (0);

	/* Return the depth of the stack (i.e. the number of "open" files) */

	return (StackDepth);
}

