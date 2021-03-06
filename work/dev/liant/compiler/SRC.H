/* ANSI-C src.h - Source file state data definitions */

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

#include "cdef.h"
#include "str.h"

/* ---------------------------------------------------------------------
** Type definitions (ACHTUNG! must correspond with based type in src.in)
** ------------------------------------------------------------------- */

typedef struct source {
	STRING		* physical_name;
	STRING		* assumed_name;
	ulong		  physical_line;
	ulong		  line_adjustment;
	ushort		  include_level;
	struct source	* next_down;
	ushort		  flag;
	FILE		* file_pointer;
	ulong		  file_position;
} SOURCE;

/* ---------------------------------------------------------------------
** External static data declaration (ACHTUNG! definition in src.in)
** ------------------------------------------------------------------- */

extern SOURCE		*SSRC;	/* pointer to current SOURCE data */

/* ---------------------------------------------------------------------
** Source state definitions (ACHTUNG! equivalent definitions in src.in)
** ------------------------------------------------------------------- */

#define _SRC_INITIAL	  0x0000  /* initial state */
#define _SRC_CLOSED	  0x0001  /* is currently closed */
#define _SRC_COMMENT	  0x0002  /* within a comment */
#define _SRC_EXCLUSION    0x0004  /* within conditionally excluded text */
#define _SRC_LISTING	  0x0008  /* do make a listing file */
#define _SRC_NO_TRIGRAPHS 0x0010  /* do not do trigraph mapping */
#define _SRC_CXX_COMMENTS 0x0020  /* allow C++ style comments */

/* ---------------------------------------------------------------------
** Macro definitions
** ------------------------------------------------------------------- */

#define SRC_INCREMENT_LINE()	(SSRC->physical_line++)
#define SRC_DECREMENT_LINE()	(SSRC->physical_line--)
#define SRC_FILE_POINTER()	(SSRC->file_pointer)

#define SRC_BEGIN_COMMENT()	(SSRC->flag |=  _SRC_COMMENT)
#define SRC_END_COMMENT()	(SSRC->flag &= ~_SRC_COMMENT)
#define SRC_BEGIN_EXCLUSION()	(SSRC->flag |=  _SRC_EXCLUSION)
#define SRC_END_EXCLUSION()	(SSRC->flag &= ~_SRC_EXCLUSION)
#define SRC_NO_TRIGRAPHS()	(SSRC->flag &   _SRC_NO_TRIGRAPHS)
#define SRC_CXX_COMMENTS()	(SSRC->flag &   _SRC_CXX_COMMENTS)

#endif  /* _SRC_H_ */

