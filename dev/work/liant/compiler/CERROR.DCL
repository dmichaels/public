/* C++ CXX_ERROR_MGR_PKG - Interface to diagnostic reporting package */

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
 *  06.22.92  DGM  013  Added IGNORE_DIAGNOSTIC.
 *  07.08.91  DGM  012  Added SIMPLE_DIAGNOSTIC.
 *  11.15.90  DGM  011  More routines!
 *  11.06.90  DGM  010  Added the SEMANTIC_ERROR_AT routines.
 *  09.14.90  DGM  009  Changed include file names and VOP names.
 *  06.06.90  DGM  008  Updated for the new utilities.
 *  05.15.90  DGM  007  New interface; CERROR no longer necessary; use
 *			SEMANTIC_ERROR,SEMANTIC_ERROR_I,SEMANTIC_ERROR_II,
 *			SYNTAX_ERROR,SYNTAX_ERROR_I,SYNTAX_ERROR_II,
 *			LEXICAL_ERROR,LEXICAL_ERROR_I,LEXICAL_ERROR_II,
 *			COMPILER_ERROR,COMPILER_ERROR_I,COMPILER_ERROR_II.
 *  03.02.90  DGM  002  Updated.
 *  11.21.89  DGM  001  Original.
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

%include CXX_ERROR_CODES_IN;
%include CXX_ERROR_MGR_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

LEXICAL_ERROR		entry    (type (SHORT_T))
			external (X_LEXICAL_ERROR),

LEXICAL_ERROR_I		entry    (type (SHORT_T),
				  character (*) varying)
			external (X_LEXICAL_ERROR_I),

LEXICAL_ERROR_II	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying)
			external (X_LEXICAL_ERROR_II),

LEXICAL_ERROR_III	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying,
				  character (*) varying)
			external (X_LEXICAL_ERROR_III),

LEXICAL_ERROR_IIII	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying,
				  character (*) varying,
				  character (*) varying)
			external (X_LEXICAL_ERROR_IIII),

SYNTAX_ERROR		entry    (type (SHORT_T))
			external (X_SYNTAX_ERROR),

SYNTAX_ERROR_I		entry    (type (SHORT_T),
				  character (*) varying)
			external (X_SYNTAX_ERROR_I),

SYNTAX_ERROR_II		entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying)
			external (X_SYNTAX_ERROR_II),

SYNTAX_ERROR_III	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying,
				  character (*) varying)
			external (X_SYNTAX_ERROR_III),

SYNTAX_ERROR_IIII	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying,
				  character (*) varying,
				  character (*) varying)
			external (X_SYNTAX_ERROR_IIII),

SEMANTIC_ERROR		entry    (type (SHORT_T))
			external (X_SEMANTIC_ERROR),

SEMANTIC_ERROR_I	entry    (type (SHORT_T),
				  character (*) varying)
			external (X_SEMANTIC_ERROR_I),

SEMANTIC_ERROR_II	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying)
			external (X_SEMANTIC_ERROR_II),

SEMANTIC_ERROR_III	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying,
				  character (*) varying)
			external (X_SEMANTIC_ERROR_III),

SEMANTIC_ERROR_IIII	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying,
				  character (*) varying,
				  character (*) varying)
			external (X_SEMANTIC_ERROR_IIII),

SEMANTIC_ERROR_AT	entry    (type (SHORT_T),
				  type (LONG_T),
				  type (NID_T))
			external (X_SEMANTIC_ERROR_AT),

SEMANTIC_ERROR_AT_I	entry    (type (SHORT_T),
				  type (LONG_T),
				  type (NID_T),
				  character (*) varying)
			external (X_SEMANTIC_ERROR_AT_I),

SEMANTIC_ERROR_AT_II	entry    (type (SHORT_T),
				  type (LONG_T),
				  type (NID_T),
				  character (*) varying,
				  character (*) varying)
			external (X_SEMANTIC_ERROR_AT_II),

COMPILER_ERROR		entry    (type (SHORT_T))
			external (X_COMPILER_ERROR),

COMPILER_ERROR_I	entry    (type (SHORT_T),
				  character (*) varying)
			external (X_COMPILER_ERROR_I),

COMPILER_ERROR_II	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying)
			external (X_COMPILER_ERROR_II),

COMPILER_ERROR_III	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying,
				  character (*) varying)
			external (X_COMPILER_ERROR_III),

COMPILER_ERROR_IIII	entry    (type (SHORT_T),
				  character (*) varying,
				  character (*) varying,
				  character (*) varying,
				  character (*) varying)
			external (X_COMPILER_ERROR_IIII),

COMPILER_ERROR_AT_I	entry    (type (SHORT_T),
				  type (LONG_T),
				  type (NID_T),
				  character (*) varying)
			external (X_COMPILER_ERROR_AT_I),

COMPILER_ERROR_AT_II	entry    (type (SHORT_T),
				  type (LONG_T),
				  type (NID_T),
				  character (*) varying,
				  character (*) varying)
			external (X_COMPILER_ERROR_AT_II),

CERROR			entry   (type (LONG_T),
				 type (NID_T),
				 type (SHORT_T))
			external (X_CERROR),

SIMPLE_DIAGNOSTIC	entry    (type (SHORT_T))
			external (X_SIMPLE_DIAGNOSTIC);

declare

IGNORE_DIAGNOSTIC		entry    (type (LONG_T))
				external (X_IGNORE_DIAGNOSTIC),

FREE_IGNORE_DIAGNOSTIC_LIST	entry    ()
				external (X_FREE_IGNORE_DIAGNOSTIC_LIST);


