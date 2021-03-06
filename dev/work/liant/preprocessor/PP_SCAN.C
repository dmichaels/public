/* CPP pp_scan.c - Lexical scanning routines */

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
 *  09.17.92  DGM  004  Don't increment line in virtual_getc (from 003).
 *  09.11.92  DGM  003  Long standing fix for detecting line splices
 *			between the first two characters of the start
 *			of a comment.
 *  11.12.91  DGM  002  Updated.
 *  06.17.91  DGM  001  Support for old-style token-pasting.
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  11.21.89  DGM  010  Optionally handle C++ style comments (//).
 *	 	        Hooked on SRC_CXX_COMMENTS() (& C_PLUS_PLUS).
 *  07.12.89  DGM  009  Fix in scomment() for the sequence / * \ * /.
 *  06.05.89  DGM  008  Optionally refrain from trigraph mapping
 *		        (see SRC_NO_TRIGRAPH() in src.h).
 *  06.02.89  DGM  007  Minor updates to pass lint cleanly.
 *  08.10.88  DGM  006  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Version and copyright stamp
/* ------------------------------------------------------------------- */

static char *VERSION__ = "@(#)LPI 09.17.92 004 ppscan";

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include "pp_ifile.h"
#include "pp_src.h"
#include "pp_char.h"

/* ---------------------------------------------------------------------
/* Exported external function declarations
/* ------------------------------------------------------------------- */

short	SNEXTC ();  /* Get the very next pertinent character of text */
short	SSCANC ();  /* Scan vanilla C text */
short	SSCANP ();  /* Scan vanilla preprocessing directive text */
short	SSCAND ();  /* Scan #define preprocessing directive text */
short	SSCANA ();  /* Scan function-like macro argument text */
short	SSCANX ();  /* Scan (conditionally) excluded text */
void	SBACKC ();  /* Push back a character */

/* =====================================================================
/*
/* SNEXTC returns from the current source file, one of:
/*
/*	EOF_CHAR (eof)
/*	EOF_IN_LINE_CHAR (eof)
/*	Next relevant character
/*
/* SSCANC returns from the current source file, one of:
/*
/*	BEGIN_PP_DIRECTIVE_CHAR
/*	EOF (eof)
/*	EOF_IN_LINE_CHAR (eof)
/*	EOF_IN_COMMENT_CHAR (eof)
/*	Next relevant character
/*
/* SSCANP returns from the current source file, one of:
/*
/*	END_PP_DIRECTIVE_CHAR
/*	GRAY_SPACE_CHAR (vertical-tab|form-feed)
/*	EOF_IN_DIRECTIVE_CHAR (eof)
/*	Next relevant character
/*
/* SSCAND returns from the current source file, one of:
/*
/*	END_PP_DIRECTIVE_CHAR
/*	WHITE_SPACE_CHAR (space|tab)
/*	GRAY_SPACE_CHAR (vertical-tab|form-feed)
/*	EOF_IN_DIRECTIVE_CHAR (eof)
/*	Next relevant character
/*
/* SSCANA returns from the current source file, one of:
/*
/*	WHITE_SPACE_CHAR (space|tab|vertical-tab|form-feed|new-line)
/*	EOF_IN_ARGUMENT_CHAR (eof)
/*	Next relevant character
/*
/* SSCANX returns from the current source file, one of:
/*
/*	BEGIN_PP_DIRECTIVE_CHAR
/*	EOF_IN_SKIP_GROUP_CHAR (eof)
/*
/* N.B. All of these routines perform trigraph mapping & line splicing.
/*
/* =================================================================== */

/* ---------------------------------------------------------------------
/* map_trigraph
/*
/* Map the given trigraph sequence character indicator (i.e. the last
/* (3rd) character of a trigraph sequence) to the corresponding ANSI-C
/* character.  If it is not a valid trigraph sequence character, then
/* return EOF_CHAR (of course, this is *not* an error).
/* ------------------------------------------------------------------- */

static
int
map_trigraph (c)
register int c;
{
	switch (c) {
		case EQUAL_CHAR	 : return (POUND_CHAR);
		case LPAREN_CHAR : return (LSQUARE_CHAR);
		case RPAREN_CHAR : return (RSQUARE_CHAR);
		case LANGLE_CHAR : return (LCURLY_CHAR);
		case RANGLE_CHAR : return (RCURLY_CHAR);
		case SQUOTE_CHAR : return (CARAT_CHAR);
		case BANG_CHAR	 : return (PIPE_CHAR);
		case MINUS_CHAR	 : return (TILDE_CHAR);
		case SLASH_CHAR	 : return (BACKSLASH_CHAR);
		default		 : return (EOF_CHAR);
	}
}

/* ---------------------------------------------------------------------
/* trigraph_getc
/*
/* Assuming that a trigraph sequence prefix (i.e. ONE question-mark)
/* has just been read from the file associated with the given file
/* pointer, return the next virtual character from the input stream.
/* ------------------------------------------------------------------- */

static
int
trigraph_getc (f)
register IFILE *f;
{
	register int c, nextc;

	if ((c = igetc (f)) == IEOF)
		return (QUESTION_CHAR);
	else if ((c != QUESTION_CHAR) || SRC_NO_TRIGRAPHS ()) {
		iungetc (f, c);
		return (QUESTION_CHAR);
	}
	else if ((c = igetc (f)) == IEOF) {
		iungetc (f, QUESTION_CHAR);
		return (QUESTION_CHAR);
	}
	else if ((nextc = map_trigraph (c)) == EOF_CHAR) {
		iungetc (f, c);
		iungetc (f, QUESTION_CHAR);
		return (QUESTION_CHAR);
	}
	else	return (nextc);
}

/* ---------------------------------------------------------------------
/* virtual_getc
/*
/* Return the next virtual character from the given input stream.
/* Performs trigraph mapping *and* line splicing, as well as source
/* line incrementing.
/* ------------------------------------------------------------------- */

short
virtual_getc (f)
register IFILE *f;
{
	register int c;

	c = igetc (f);

	ProcessCharacter:

	if (c == BACKSLASH_CHAR) {
		CheckForLineSplice:
		if (ipeekc (f) == NEWLINE_CHAR) {
			SRC_INCREMENT_LINE ();
			(void) igetc (f);
			if ((c = igetc (f)) == IEOF)
				return (EOF_IN_LINE_CHAR);
			else	goto ProcessCharacter;
		}
	}
	else if (c == QUESTION_CHAR) {
		if (ipeekc (f) == QUESTION_CHAR) {
			if ((c = trigraph_getc (f)) == BACKSLASH_CHAR)
				goto CheckForLineSplice;
		}
	}
	else if (c == IEOF)
		return (EOF_CHAR);

	return (c);
}

/* ---------------------------------------------------------------------
/* skip_comment
/*
/* Skips past a comment in current source file.  This routine *assumes*
/* that we have just read an opening comment sequence (i.e. SLASH STAR).
/*
/* We will scan to the closing comment (i.e. STAR SLASH) sequence.
/*
/* If we successfully skipped a comment, SPACE_CHAR will be returned,
/* and the next character read will be the one immediately following
/* the end of the comment.
/*
/* Otherwise, If the end of file was encountered within the comment,
/* then EOF_IN_COMMENT will be returned.
/* ------------------------------------------------------------------- */

static
int
skip_comment (f)
register IFILE *f;
{
	register int c, star;

	SRC_BEGIN_COMMENT ();

	for (star = 0 ; 1 ; ) {
		if ((c = igetc (f)) == NEWLINE_CHAR) {
			SRC_INCREMENT_LINE  ();
			star = 0;
		}
		else if (c == STAR_CHAR)
			star = 1;
		else if (c == QUESTION_CHAR) {
			if ((ipeekc (f) == QUESTION_CHAR) &&
			    (trigraph_getc (f) == BACKSLASH_CHAR))
				goto CheckForLineSplice;
			else	star = 0;
		}
		else if (c == BACKSLASH_CHAR) {
			CheckForLineSplice:
			if (ipeekc (f) == NEWLINE_CHAR) {
				(void) igetc (f);
				SRC_INCREMENT_LINE  ();
			}
			else	star = 0;
		}
		else if ((c == SLASH_CHAR) && star)
			break;
		else if (c == IEOF)
			return (EOF_IN_COMMENT_CHAR);
		else	star = 0;
	}

	SRC_END_COMMENT ();

	return (SPACE_CHAR);
}

/* ---------------------------------------------------------------------
/* skip_cxx_comment
/*
/* Skips past a C++ style comment in current source file.  This routine
/* *assumes* that we have just read an opening comment sequence
/* (i.e. SLASH/SLASH).
/*
/* We will scan to the closing comment (i.e. NEWLINE) sequence.
/*
/* If we successfully skipped a comment, SPACE_CHAR will be returned,
/* and the next character read will be the one immediately following
/* the end of the comment.
/*
/* Otherwise, if the end of file was encountered within the comment,
/* then EOF_IN_COMMENT will be returned.
/* ------------------------------------------------------------------- */

static
int
skip_cxx_comment (f)
register IFILE *f;
{
	register int c;

	SRC_BEGIN_COMMENT ();

	while (1) {
		if ((c = igetc (f)) == NEWLINE_CHAR) {
			iungetc (f, NEWLINE_CHAR);	/* note */
			break;
		}
		else if (c == QUESTION_CHAR) {
			if ((ipeekc (f) == QUESTION_CHAR) &&
			    (trigraph_getc (f) == BACKSLASH_CHAR))
				goto CheckForLineSplice;
		}
		else if (c == BACKSLASH_CHAR) {
			CheckForLineSplice:
			if (ipeekc (f) == NEWLINE_CHAR) {
				(void) igetc (f);
				SRC_INCREMENT_LINE  ();
			}
		}
		else if (c == IEOF)
			return (EOF_IN_COMMENT_CHAR);
	}

	SRC_END_COMMENT ();

	return (SPACE_CHAR);
}

/* ---------------------------------------------------------------------
/* SNEXTC
/*
/* This routine is used to get the next relevant input character
/* when the lexer is processing within a preprocessing token.
/*
/* Returns from the current source file, one of:
/*
/*	EOF_CHAR (eof)
/*	EOF_IN_LINE_CHAR (eof)
/*	Next relevant character (after translation phases 1 & 2)
/*
/* ------------------------------------------------------------------- */

short int
SNEXTC ()
{
	register IFILE *f;
	register int	c;

	/* Get the current file pointer */

	f = SRC_FILE_POINTER ();

	/* Get a character */

	c = igetc (f);

	/* Map the character; check for a trigraph sequence & line splicing */

	ProcessCharacter:

	if (c == NEWLINE_CHAR)
		SRC_INCREMENT_LINE ();
	else if (c == QUESTION_CHAR) {
		if ((ipeekc (f) == QUESTION_CHAR) &&
		    ((c = trigraph_getc (f)) == BACKSLASH_CHAR))
			goto CheckForLineSplice;
	}
	else if (c == BACKSLASH_CHAR) {
		CheckForLineSplice:
		if (ipeekc (f) == NEWLINE_CHAR) {
			(void) igetc (f);
			SRC_INCREMENT_LINE ();
			if ((c = igetc (f)) == IEOF)
				return (EOF_IN_LINE_CHAR);
			else	goto ProcessCharacter;
		}
	}
	else if (c == IEOF)
		return (EOF_CHAR);
	return (c);
}

/* ---------------------------------------------------------------------
/* SSCANC
/*
/* This routine is used to scan to the next relevant input character
/* when the lexer is processing straight vanilla ANSI-C text.
/*
/* Returns from the current source file, one of:
/*
/*	BEGIN_PP_DIRECTIVE_CHAR (white-space-with-newline #)
/*	EOF_CHAR (eof)
/*	EOF_IN_LINE_CHAR (eof)
/*	EOF_IN_COMMENT_CHAR (eof)
/*	Next relevant character
/*
/* ------------------------------------------------------------------- */

short
SSCANC ()
{
	register IFILE	*f;
	register int	c;
	register int	lastc;
	register int	newline;
	register int	nextc;

	/* Get the current file pointer */

	f = SRC_FILE_POINTER ();

	/* Skip white-space loop */

	for (lastc = EOF_CHAR, newline = 0 ; 1 ; lastc = c) {

		if ((c = igetc (f)) == SPACE_CHAR)
			;
		else if (c == HORIZONTAL_TAB_CHAR)
			;
		else if (c == NEWLINE_CHAR) {
			SRC_INCREMENT_LINE ();
			newline = 1;
		}
		else if (c == SLASH_CHAR) {
			if ((nextc = virtual_getc (f)) == STAR_CHAR) {
				if ((c = skip_comment (f)) != SPACE_CHAR)
					return (c);
			}
			else if ((nextc == SLASH_CHAR) && SRC_CXX_COMMENTS ()) {
				if ((c = skip_cxx_comment (f)) != SPACE_CHAR)
					return (c);
			}
			else {	iungetc (f, nextc);
				break;
			}
		}
		else if (c == QUESTION_CHAR) {
			if ((ipeekc (f) == QUESTION_CHAR) &&
			    ((c = trigraph_getc (f)) == BACKSLASH_CHAR))
				goto CheckForLineSplice;
			else	break;
		}
		else if (c == BACKSLASH_CHAR) {
			CheckForLineSplice:
			if (ipeekc (f) == NEWLINE_CHAR) {
				(void) igetc (f);
				SRC_INCREMENT_LINE ();
			}
			else	break;
		}
		else if (c == FORMFEED_CHAR)
			;
		else if (c == VERTICAL_TAB_CHAR)
			;
		else if (c == IEOF) {
			if (lastc != NEWLINE_CHAR)
				return (EOF_IN_LINE_CHAR);
			else	return (EOF_CHAR);
		}
		else	break;
	}

	/* Here, we have a non-white-space/non-comment character in "c" */

	if ((c == POUND_CHAR) && newline)
		return (BEGIN_PP_DIRECTIVE_CHAR);
	else	return (c);
}

/* ---------------------------------------------------------------------
/* SSCANP
/*
/* This routine is used to scan to the next relevant input character
/* when the lexer is processing any preprocessing directive line.
/*
/* Returns from the current source file, one of:
/*
/*	END_PP_DIRECTIVE_CHAR (newline)
/*	GRAY_SPACE_CHAR (vertical-tab|form-feed)
/*	EOF_IN_DIRECTIVE_CHAR (eof)
/*	Next relevant character
/*
/* ------------------------------------------------------------------- */

short
SSCANP ()
{
	register IFILE *f;
	register int	c;
	register int	nextc;

	/* Get the current file pointer */

	f = SRC_FILE_POINTER ();

	/* Skip pure-white-space (i.e. space/tab) loop */

	for ( ; 1 ; ) {

		if ((c = igetc (f)) == SPACE_CHAR)
			;
		else if (c == HORIZONTAL_TAB_CHAR)
			;
		else if (c == NEWLINE_CHAR) {
			iungetc (f, NEWLINE_CHAR);	 /* note */
			return (END_PP_DIRECTIVE_CHAR);
		}
		else if (c == SLASH_CHAR) {
			if ((nextc = virtual_getc (f)) == STAR_CHAR) {
				if ((c = skip_comment (f)) != SPACE_CHAR)
					return (EOF_IN_DIRECTIVE_CHAR);
			}
			else if ((nextc == SLASH_CHAR) && SRC_CXX_COMMENTS ()) {
				if ((c = skip_cxx_comment (f)) != SPACE_CHAR)
					return (EOF_IN_DIRECTIVE_CHAR);
			}
			else {	iungetc (f, nextc);
				break;
			}
		}
		else if (c == QUESTION_CHAR) {
			if ((ipeekc (f) == QUESTION_CHAR) &&
			    ((c = trigraph_getc (f)) == BACKSLASH_CHAR))
				goto CheckForLineSplice;
			else	break;
		}
		else if (c == BACKSLASH_CHAR) {
			CheckForLineSplice:
			if (ipeekc (f) == NEWLINE_CHAR) {
				(void) igetc (f);
				SRC_INCREMENT_LINE ();
			}
			else	break;
		}
		else if (c == IEOF)
			return (EOF_IN_DIRECTIVE_CHAR);
		else	break;
	}
	if ((c == FORMFEED_CHAR) || (c == VERTICAL_TAB_CHAR))
		return (GRAY_SPACE_CHAR);
	return (c);
}

/* ---------------------------------------------------------------------
/* SSCAND
/*
/* This routine is used to scan to the next relevant input character
/* when the lexer is processing a macro definition preprocessing
/* directive line (i.e a #define).
/*
/* Returns from the current source file, one of:
/*
/*	END_PP_DIRECTIVE_CHAR (newline)
/*	WHITE_SPACE_CHAR (space|tab)
/*	GRAY_SPACE_CHAR (vertical-tab|form-feed)
/*	EOF_IN_DIRECTIVE_CHAR (eof)
/*	Next relevant character
/*
/* ------------------------------------------------------------------- */

short
SSCAND ()
{
	register IFILE *f;
	register int	c;
	register int	nextc;
	register int	space;
	register int	comment_space;

	/* Get the current file pointer */

	f = SRC_FILE_POINTER ();

	/* Skip pure-white-space (i.e. space/tab) loop */

	for (space = comment_space = 0 ; 1 ; ) {

		if ((c = igetc (f)) == SPACE_CHAR)
			space = 1;
		else if (c == HORIZONTAL_TAB_CHAR)
			space = 1;
		else if (c == NEWLINE_CHAR) {
			iungetc (f, NEWLINE_CHAR);	/* note */
			return (END_PP_DIRECTIVE_CHAR);
		}
		else if (c == SLASH_CHAR) {
			if ((nextc = virtual_getc (f)) == STAR_CHAR) {
				if ((c = skip_comment (f)) == SPACE_CHAR) {
					if (SRC_OLD_TOKEN_PASTING ())
						comment_space = 1;
					else	space = 1;
				}
				else	return (EOF_IN_DIRECTIVE_CHAR);
			}
			else if ((nextc == SLASH_CHAR) && SRC_CXX_COMMENTS ()) {
				if ((c = skip_cxx_comment (f)) == SPACE_CHAR)
					space = 1;
				else	return (EOF_IN_DIRECTIVE_CHAR);
			}
			else {	iungetc (f, nextc);
				break;
			}
		}
		else if (c == QUESTION_CHAR) {
			if ((ipeekc (f) == QUESTION_CHAR) &&
			    ((c = trigraph_getc (f)) == BACKSLASH_CHAR))
				goto CheckForLineSplice;
			else	break;
		}
		else if (c == BACKSLASH_CHAR) {
			CheckForLineSplice:
			if (ipeekc (f) == NEWLINE_CHAR) {
				(void) igetc (f);
				SRC_INCREMENT_LINE ();
			}
			else	break;
		}
		else if (c == IEOF)
			return (EOF_IN_DIRECTIVE_CHAR);
		else	break;
	}
	if (space) {
		iungetc (f, c);
		return (WHITE_SPACE_CHAR);
	}
	else if (comment_space) {
		iungetc (f, c);
		iungetc (f, POUND_CHAR);
		return (POUND_CHAR);
	}
	else if ((c == FORMFEED_CHAR) || (c == VERTICAL_TAB_CHAR))
		return (GRAY_SPACE_CHAR);
	return (c);
}

/* ---------------------------------------------------------------------
/* SSCANA
/*
/* This routine is used to scan to the next relevant input character
/* when the lexer is processing the argument list of a function-like
/* macro invocation.
/*
/* Returns from the current source file, one of:
/*
/*	Next relevant character
/*	WHITE_SPACE_CHAR (space|tab|vertical-tab|form-feed|new-line)
/*	EOF_IN_ARGUMENT_CHAR (eof)
/*
/* ------------------------------------------------------------------- */

short
SSCANA ()
{
	register IFILE *f;
	register int	c;
	register int	space;
	register int	nextc;

	/* Get the current file pointer */

	f = SRC_FILE_POINTER ();

	/* Skip white-space loop */

	for (space = 0 ; 1 ; ) {

		if ((c = igetc (f)) == SPACE_CHAR)
			space = 1;
		else if (c == HORIZONTAL_TAB_CHAR)
			space = 1;
		else if (c == NEWLINE_CHAR) {
			SRC_INCREMENT_LINE ();
			space = 1;
		}
		else if (c == SLASH_CHAR) {
			if ((nextc = virtual_getc (f)) == STAR_CHAR) {
				if ((c = skip_comment (f)) != SPACE_CHAR)
					return (EOF_IN_ARGUMENT_CHAR);
			}
			else if ((nextc == SLASH_CHAR) && SRC_CXX_COMMENTS ()) {
				if ((c = skip_cxx_comment (f)) != SPACE_CHAR)
					return (EOF_IN_ARGUMENT_CHAR);
			}
			else {	iungetc (f, nextc);
				break;
			}
		}
		else if (c == QUESTION_CHAR) {
			if ((ipeekc (f) == QUESTION_CHAR) &&
			    ((c = trigraph_getc (f)) == BACKSLASH_CHAR))
				goto CheckForLineSplice;
			else	break;
		}
		else if (c == BACKSLASH_CHAR) {
			CheckForLineSplice:
			if (ipeekc (f) == NEWLINE_CHAR) {
				(void) igetc (f);
				SRC_INCREMENT_LINE ();
			}
			else	break;
		}
		else if (c == FORMFEED_CHAR)
			space = 1;
		else if (c == VERTICAL_TAB_CHAR)
			space = 1;
		else if (c == IEOF)
			return (EOF_IN_ARGUMENT_CHAR);
		else	break;
	}
	if (space) {
		iungetc (f, c);
		return (WHITE_SPACE_CHAR);
	}
	else	return (c);
}

/* ---------------------------------------------------------------------
/* SSCANX
/*
/* This routine is used to scan to the next relevant input character
/* when the lexer is skipping past conditionally excluded source code.
/*
/* Returns from the current source file, one of:
/*
/*	BEGIN_PP_DIRECTIVE_CHAR (white-space-with-newline #)
/*	EOF_IN_SKIP_GROUP_CHAR (eof)
/*
/* ------------------------------------------------------------------- */

short
SSCANX ()
{
	register int c;

	/* Scan to the next preprocessing directive */

	SRC_BEGIN_EXCLUSION ();

	GetNextCharacter:

	while ((c = SSCANC()) != BEGIN_PP_DIRECTIVE_CHAR) {
		if (c <= EOF_CHAR)
			return (EOF_IN_SKIP_GROUP_CHAR);
	}

	/* return (BEGIN_PP_DIRECTIVE_CHAR); ---> This is the easy way out */

	/* ------------------------------------------------------------------
	 * Here, we are at the start of a preprocessing directive.  See if
	 * this preprocessing directive is (likely) a #if, #ifdef, #ifndef
	 * #elif, #else, or #endif.
	 *
	 * -NOTE- THIS IS ONLY FOR SPEED! everything would still work if
	 * we simply returned END_PP_DIRECTIVE_CHAR (above) not knowing
	 * what kind of preprocessing directive we had encountered.
	 * ---------------------------------------------------------------- */

	while ((c = SSCANP()) == GRAY_SPACE_CHAR)
		;
	if (c == END_PP_DIRECTIVE_CHAR)
		goto GetNextCharacter;
	else if (c == EOF_IN_DIRECTIVE_CHAR) {
		SRC_END_EXCLUSION ();
		return (EOF_IN_SKIP_GROUP_CHAR);
	}
	else if (c == LOWER_E_CHAR) {
		if (((c = SNEXTC ()) == LOWER_L_CHAR) || (c == LOWER_N_CHAR)) {
			iungetc (SRC_FILE_POINTER (), c);
			iungetc (SRC_FILE_POINTER (), LOWER_E_CHAR);
			SRC_END_EXCLUSION ();
			return (BEGIN_PP_DIRECTIVE_CHAR);
		}
	}
	else if (c == LOWER_I_CHAR) {
		if ((c = SNEXTC ()) == LOWER_F_CHAR) {
			iungetc (SRC_FILE_POINTER (), c);
			iungetc (SRC_FILE_POINTER (), LOWER_I_CHAR);
			SRC_END_EXCLUSION ();
			return (BEGIN_PP_DIRECTIVE_CHAR);
		}
	}

	goto GetNextCharacter;
}

/* ---------------------------------------------------------------------
/* SBACKC
/*
/* Push the given character "c" back onto the input stream of the file
/* which is on the top of the source file stack.  The pushed back
/* character must NOT be a special character code (as defined in
/* charset.h).  The line count is updated accordingly if a newline
/* is pushed back.  Up to MAX_PUSH_BACK characters (defined in
/* pp_ifile.h) may be safely pushed back.
/* ------------------------------------------------------------------- */

void
SBACKC (c)
register short *c;
{
	if (*c < 0) return;	/* paranoid */

	if (*c == NEWLINE_CHAR)
		SRC_DECREMENT_LINE ();

	iungetc (SRC_FILE_POINTER (), (int)(*c));
}

