/* C++ CXX_PARSER_PKG - Interface to parser driver */

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
 *  05.08.92  DGM  010  Added AT_DESTRUCTOR_NAME.
 *  01.24.92  DGM  009  Removed PAR_DRIVER.
 *  01.22.92  DGM  008  Added AT_MEMBER_POINTER.
 *  07.31.91  DGM  007  Added AT_FUNCTION_DEFINITION.
 *  07.08.91  DGM  006  Added SKIP_FUNCTION_BODY.
 *  06.13.91  DGM  005  Added AT_START_OF_TAG_DEFINITION.
 *  05.15.91  DGM  004  Added AT_END_OF_xxx.
 *  04.29.91  DGM  003  Added PEEK_ANONYMOUS_UNION_BODY.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  06.08.90  DGM  001  Changed INITIALIZE_PARSER to BEGIN_PARSER
 *			and added END_PARSER.
 *  10.17.89  DGM  000  Original.
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

/* None */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

COLLECT_FUNCTION_BODY		entry    (pointer, pointer, pointer)
				external (X_COLLECT_FUNCTION_BODY),

PARSE_COMPILATION_UNIT		entry    ()
				external (X_PARSE_COMPILATION_UNIT),

PARSE_INITIALIZER_ITEM		entry    ()
				external (X_PARSE_INITIALIZER_ITEM),

PARSE_FUNCTION_BODY		entry    (pointer, pointer, pointer)
				external (X_PARSE_FUNCTION_BODY),

SKIP_FUNCTION_BODY		entry    ()
				external (X_SKIP_FUNCTION_BODY),

PEEK_ANONYMOUS_UNION_BODY	entry    ()
				returns  (type (BOOL_T))
				external (X_PEEK_ANONYMOUS_UNION_BODY),

AT_DESTRUCTOR_NAME		entry
				returns  (type (BOOL_T))
				external (X_AT_DESTRUCTOR_NAME),

AT_END_OF_DECLARATION		entry    ()
				returns  (type (BOOL_T))
				external (X_AT_END_OF_DECLARATION),

AT_END_OF_FUNCTION_DECLARATOR	entry
				returns  (type (BOOL_T))
				external (X_AT_END_OF_FUNCTION_DECLARATOR),

AT_FUNCTION_DEFINITION		entry
				returns  (type (BOOL_T))
				external (X_AT_FUNCTION_DEFINITION),

AT_START_OF_TAG_DEFINITION	entry
				returns  (type (BOOL_T))
				external (X_AT_START_OF_TAG_DEFINITION),

AT_MEMBER_POINTER		entry    (type (BOOL_T))
				returns  (type (BOOL_T))
				external (X_AT_MEMBER_POINTER);

