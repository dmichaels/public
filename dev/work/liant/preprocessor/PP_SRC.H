/* CPP pp_src.h - Source file state data definitions */

/***********************************************************************
 * This product is the property of Liant Software Corporation and is   *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Liant Software Corporation except pursuant to the license agreement.*
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  11.12.91  DGM  004  Updated.
 *  07.31.91  DGM  003  Added SOURCE.line_of_first_char.
 *  06.17.91  DGM  002  Added SRC_OLD_TOKEN_PASTING
 *			to support old-style token-pasting.
 *  05.29.91  DGM  001  Updated.
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  11.21.89  DGM  (006) Added SRC_CXX_COMMENTS().
 *  06.05.89  DGM  (005) Minor reorganization; added SRC_NOTRIGRAPH.
 *  08.10.88  DGM  (004) Original.
 *
 ***********************************************************************/

#ifndef _SRC_H_
#define _SRC_H_

/* ---------------------------------------------------------------------
** Include files
** ------------------------------------------------------------------- */

#include "pp_defs.h"
#include "pp_str.h"
#include "pp_ifile.h"

/* ---------------------------------------------------------------------
/* Type definitions (ACHTUNG! must correspond with based type in src.in)
/* ------------------------------------------------------------------- */

typedef struct source {
	STRING		* physical_name;
	STRING		* assumed_name;
	ulong		  physical_line;
	ulong		  line_adjustment;
	ushort		  include_level;
	struct source	* next_down;
	ushort		  flag;
	IFILE		* file_pointer;
	ulong		  file_position;
	ulong		  line_of_first_char;
	short		  enclosing_macro;
	short		  enclosing_macro_level;
	short		  enclosing_macro_flag;
} SOURCE;

/* ---------------------------------------------------------------------
/* External static data declaration (ACHTUNG! definition in src.in)
/* ------------------------------------------------------------------- */

extern SOURCE		*SSRC;	/* pointer to current SOURCE data */

/* ---------------------------------------------------------------------
/* Source state definitions (ACHTUNG! equivalent definitions in src.in)
/* ------------------------------------------------------------------- */

#define _SRC_INITIAL	       0x0000 /* initial state */
#define _SRC_CLOSED	       0x0001 /* is currently/temporarly closed */
#define _SRC_COMMENT	       0x0002 /* in a comment */
#define _SRC_EXCLUSION         0x0004 /* in conditionally excluded text */
#define _SRC_LISTING	       0x0008 /* make a listing file */
#define _SRC_NO_TRIGRAPHS      0x0010 /* do not do trigraph mapping */
#define _SRC_CXX_COMMENTS      0x0020 /* allow C++ style comments */
#define _SRC_OLD_TOKEN_PASTING 0x0040 /* allow old-style token-pasting */

/* ---------------------------------------------------------------------
/* Macro definitions
/* ------------------------------------------------------------------- */

#define SRC_FILE_POINTER()	(SSRC->file_pointer)
#define SRC_FILE_POSITION()	(SSRC->file_position)
#define SRC_PHYSICAL_NAME()	(SSRC->physical_name)

#define SRC_BEGIN_COMMENT()	(SSRC->flag |=  _SRC_COMMENT)
#define SRC_END_COMMENT()	(SSRC->flag &= ~_SRC_COMMENT)
#define SRC_BEGIN_EXCLUSION()	(SSRC->flag |=  _SRC_EXCLUSION)
#define SRC_END_EXCLUSION()	(SSRC->flag &= ~_SRC_EXCLUSION)

#define SRC_IS_CLOSED()		(SSRC->flag &   _SRC_CLOSED)
#define SRC_IN_LISTING_MODE()	(SSRC->flag &   _SRC_LISTING)
#define SRC_NO_TRIGRAPHS()	(SSRC->flag &   _SRC_NO_TRIGRAPHS)
#define SRC_CXX_COMMENTS()	(SSRC->flag &   _SRC_CXX_COMMENTS)
#define SRC_OLD_TOKEN_PASTING()	(SSRC->flag &   _SRC_OLD_TOKEN_PASTING)

#define SRC_SET_CLOSED()	(SSRC->flag |=  _SRC_CLOSED)
#define SRC_SET_OPEN()		(SSRC->flag &= ~_SRC_CLOSED)
#define SRC_FLAG()		(SSRC->flag)
#define SRC_SET_LISTING_MODE()	(SSRC->flag |=  _SRC_LISTING)

#define SRC_INCREMENT_LINE()	(SSRC->physical_line++)
#define SRC_DECREMENT_LINE()	(SSRC->physical_line--)

#endif  /* _SRC_H_ */

