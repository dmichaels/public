/* CPP pp_src.c - Source file manipulation routines */

/***********************************************************************
 * This product is the property of Liant Software Corporation and is   *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Liant Software Corporation except pursuant to the license agreement.*
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  11.12.91  DGM  004  Updated.
 *  07.31.91  DGM  003  Added SOURCE.line_of_first_char.
 *  05.29.91  DGM  002  Fixed bug with -exp -noincludes.
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
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
/* Version and copyright stamp
/* ------------------------------------------------------------------- */

static char *VERSION__ = "@(#)LPI 11.12.91 004 ppsrc";

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include "pp_src.h"
#include "pp_char.h"

/* ---------------------------------------------------------------------
/* External static data definitions
/* ------------------------------------------------------------------- */

SOURCE	*SSRC	= NULL;

/* ---------------------------------------------------------------------
/* Internal static data definitions
/* ------------------------------------------------------------------- */

static SOURCE	MainSourceFile =
{
		  NULL_STRING		/* physical_name */
		, NULL_STRING		/* assumed_name */
		, (ulong) 0		/* physical_line */
		, (ulong) 0		/* line_adjustment */
		, (ushort) 0		/* include_level */
		, (SOURCE *) NULL	/* next_down */
		, (ushort) 0		/* flag */
		, (IFILE *) NULL	/* file_pointer */
		, (ulong) 0		/* file_position */
		, (ulong) 0		/* line_of_first_char */
		, (short) 0		/* enclosing_macro */
		, (short) 0		/* enclosing_macro_level */
		, (short) 0		/* enclosing_macro_flag */
};

static int	StackDepth = 0;

/* ---------------------------------------------------------------------
/* Exported external function declarations
/* ------------------------------------------------------------------- */

extern short XSOPEN  ();  /* Open source file; return nesting level */
extern short XSCLOSE ();  /* Close source file; return nesting level */

/* ---------------------------------------------------------------------
/* Local macro function definitions
/* ------------------------------------------------------------------- */

#define SRC_EMPTY_STACK()	(StackDepth <= 0)

/* ---------------------------------------------------------------------
/* save_string 
/*
/* Given a (pointer to a) PL/I character varying type string, allocate
/* space for it, copy it elsewhere, AND add a null byte (EOS) onto
/* the end of the text of the string so it may be easily used by both C
/* and PL/I code.  Return a pointer to the saved string.
/* ------------------------------------------------------------------- */

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
/* temporarily_close_current_source_file
/* ------------------------------------------------------------------- */

static
void
temporarily_close_current_source_file ()
{
	SRC_FILE_POSITION ()= igetpos (SRC_FILE_POINTER ());
	iclose (SRC_FILE_POINTER ());
	SRC_FILE_POINTER () = NULL;
	SRC_SET_CLOSED ();
}

/* ---------------------------------------------------------------------
/* reopen_current_source_file
/* ------------------------------------------------------------------- */

static
int
reopen_current_source_file ()
{
	if ((SRC_FILE_POINTER () =
	     iopen (SRC_PHYSICAL_NAME ()->text,
		    SRC_IN_LISTING_MODE ())) == NULL)
		return (0);
	isetpos (SRC_FILE_POINTER (), SRC_FILE_POSITION ());
	SRC_SET_OPEN ();
	return (1);
}

/* ---------------------------------------------------------------------
/* XSOPEN
/*
/* Opens the source file specified by the given name (which is assumed
/* to be a PL/I character varying string) and allocates a SOURCE
/* record for it and pushes it onto the top of the SOURCE stack.
/* All subsequent reads (via the scan.c routines) will be from the
/* source file on the top of the stack.
/*
/* If all is well, return the (greater-than-zero) number of "open" files
/* (i.e. the depth of the source file stack), otherwise returns zero.
/* ------------------------------------------------------------------- */

extern
short
XSOPEN (name, listing_mode)
STRING *name;
short *listing_mode;
{
	STRING	*sname;		/* source file name pointer */
	SOURCE	*snode;		/* source file stack node pointer */
	IFILE	*fp;		/* file pointer */

	/* Save the file name in a C-style string */

	sname = save_string (name);

	/* See if the file exists */

	if (!SYS_FILE_EXISTS (sname->text))
		return (0);

	/* See if the file is readable */

	if (!SYS_FILE_IS_READABLE (sname->text))
		return (0);

	/* Try to open the file */

	if ((fp = iopen (sname->text, *listing_mode)) == NULL) {
		/*
		/* Here, the open failed.  Close a previously opened
		/* file (while saving its current state) and try again.
		/**/
		if (SRC_EMPTY_STACK ()) {
			SYS_FREE (sname);
			return (0);
		}
		temporarily_close_current_source_file ();
		if ((fp = iopen (sname->text, *listing_mode)) == NULL) {
			/*
			/* Still can't open it.  Reopen the one just closed.
			/**/
			(void) reopen_current_source_file ();
			SYS_FREE (sname);
			return (0);
		}
	}

	/* If the stack is empty use the static main source node */

	if (SRC_EMPTY_STACK ()) {
		SSRC = (SOURCE *)&MainSourceFile;
		SSRC->next_down = NULL;
	}

	/* Otherwise, allocate a new source stack node */

	else {
		if ((snode = (SOURCE *) SYS_ALLOC (sizeof (SOURCE))) == NULL) {
			iclose (fp);
			return (0);
		}
		snode->next_down = SSRC;
		SSRC = snode;
	}

	/* Update the stack depth and fill in the source file data */

	SSRC->include_level		= StackDepth++;
	SSRC->physical_name		= sname;
	SSRC->assumed_name		= NULL_STRING;
	SSRC->physical_line		= (ulong)0;
	SSRC->line_adjustment		= (ulong)0;
	SSRC->flag			= (ushort)0;
	SSRC->file_pointer		= fp;
	SSRC->file_position		= (ulong)0;
	SSRC->line_of_first_char	= (ulong)0;
	SSRC->enclosing_macro		= (short)0;
	SSRC->enclosing_macro_level	= (short)0;
	SSRC->enclosing_macro_flag	= (short)0;

	if (*listing_mode)
		SRC_SET_LISTING_MODE ();
	
	/*
	/* Now, push back a new-line onto the beginning-of-file.
	/* This a (nice?) hack so that the SSCAN () routine (scan.c)
	/* doesn't have to check for beginning-of-file on every call.
	/**/

	iungetc (fp, NEWLINE_CHAR);

	/* Return the depth of the stack (i.e. the number of "open" files) */

	return (StackDepth);
}

/* ---------------------------------------------------------------------
/* XSCLOSE
/*
/* Closes the file on the top of the source file state stack.
/*
/* If there are no more files on the stack or if an error occured such
/* such that the new current source file on top of stack file cannot be
/* read, then return zero, otherwise return the (non-zero) depth of the
/* the source file stack (i.e the number of "open" files).
/* ------------------------------------------------------------------- */

short
XSCLOSE ()
{
	register SOURCE *p;

	/* Check for stack underflow */

	if (SRC_EMPTY_STACK ())
		return (0);

	/* Close the file */

	iclose (SRC_FILE_POINTER ());

	/* Pop the stack */

	StackDepth--;

	if (SRC_EMPTY_STACK ()) {
		/*
		/* Here, the main source file was just closed.
		/**/
		SSRC->include_level = StackDepth;
		SRC_FILE_POINTER () = NULL;
		return (0);
	}
	else {
		p = SSRC;
		SSRC = p->next_down;
		SYS_FREE (p);
	}

	/*
	/* If the now-current source file was previously
	/* (temporarily) closed (in XSOPEN) then re-open it now.
	/**/

	if (SRC_IS_CLOSED ()) {
		if (!reopen_current_source_file ())
			return (0);
	}

	/* Return the depth of the stack (i.e. the number of "open" files) */

	return (StackDepth);
}

