/* C++ CXX_LEXER_PKG - Interface to the lexer package */

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
 *  09.14.90  DGM  003  Changed include file names and VOP names.
 *  06.08.90  DGM  002  Changed to INTIALIZE_LEXER to BEGIN_LEXER,
 *			and added END_LEXER.
 *  11.28.89  DGM  001  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_LEXER_TOKENS_PKG;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

BEGIN_LEXER			entry
				external (X_BEGIN_LEXER),

END_LEXER			entry
				external (X_END_LEXER),

ADVANCE_TOKEN			entry
				external (X_ADVANCE_TOKEN),

RESET_PEEK_TOKEN		entry
				external (X_RESET_PEEK_TOKEN),
SET_PEEK_TOKEN			entry
				external (X_SET_PEEK_TOKEN),
ADVANCE_PEEK_TOKEN		entry
				external (X_ADVANCE_PEEK_TOKEN),
SET_RAW_PEEK_TOKEN		entry
				external (X_SET_RAW_PEEK_TOKEN),
ADVANCE_RAW_PEEK_TOKEN		entry
				external (X_ADVANCE_RAW_PEEK_TOKEN),

BEGIN_LEXER_LOOK_AHEAD		entry
				external (X_BEGIN_LEXER_LOOK_AHEAD),
END_LEXER_LOOK_AHEAD		entry
				external (X_END_LEXER_LOOK_AHEAD),

BEGIN_LEXER_TOKEN_COLLECTION	entry
				external (X_BEGIN_LEXER_TOKEN_COLLECTION),
END_LEXER_TOKEN_COLLECTION	entry
				external (X_END_LEXER_TOKEN_COLLECTION),

PUSH_BACK_TOKENS		entry    (pointer, pointer)
				external (X_PUSH_BACK_TOKENS);

