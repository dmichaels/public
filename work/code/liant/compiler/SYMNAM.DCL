/* C++ CXX_SYMBOL_TABLE_NAMES_PKG - Symbol table names interface */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  09.09.92  DGM  014	Added TOKEN_MEMBER_NAME.
 *  04.18.92  PKT  013	Oops, same as 012.
 *  03.30.92  DGM  012  Added COMPLETE_SYMBOL_NAME.
 *  05.29.91  DGM  011  Added SYMBOL_LOCATION_NAME.
 *  04.29.91  DGM  010  Added EXPANDED_xxx & QUALIFYING_SYMBOL_NAME.
 *  01.11.91  DGM  009  Added FUNCTION_SYMBOL_TYPE_NAME. Added FULL_xxx.
 *  09.25.90  DGM  008  Many changes.
 *  09.14.90  DGM  007  Changed include file names and VOP names.
 *  09.13.90  DGM  006  Added ANY_TYPE_NAME.
 *  08.08.90  DGM  005  New type-node; removed CLASS_NAME.
 *  05.25.90  DGM  004  Added QUALIFIED_SYMBOL_TYPE_NAME.
 *  05.15.90  DGM  003  Added BASE_CLASS_NAME.
 *  03.27.90  DGM  002  Updated.
 *  03.02.90  DGM  001  Updated.
 *  02.21.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include GLOBAL_IN;                */
/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_SYMBOL_TABLE_NAMES_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

DECLARATION_STRING		entry    (type (NID_T),
					  type (LONG_BITS_T))
				returns  (character (512) varying)
				external (X_DECLARATION_STRING),

QUALIFIED_SYMBOL_TYPE_NAME	entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_QUALIFIED_SYMBOL_TYPE_NAME),

SYMBOL_TYPE_NAME		entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_SYMBOL_TYPE_NAME),

FULL_SYMBOL_NAME		entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_FULL_SYMBOL_NAME),

FULL_QUALIFIED_SYMBOL_NAME	entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_FULL_QUALIFIED_SYMBOL_NAME),

COMPLETE_SYMBOL_NAME		entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_COMPLETE_SYMBOL_NAME),

COMPLETE_QUALIFIED_SYMBOL_NAME	entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_COMPLETE_QUALIFIED_SYMBOL_NAME),

EXPANDED_SYMBOL_NAME		entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_EXPANDED_SYMBOL_NAME),

EXPANDED_QUALIFIED_SYMBOL_NAME	entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_EXPANDED_QUALIFIED_SYMBOL_NAME),

QUALIFIED_SYMBOL_NAME		entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_QUALIFIED_SYMBOL_NAME),

QUALIFYING_SYMBOL_NAME		entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_QUALIFYING_SYMBOL_NAME),

SYMBOL_NAME			entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_SYMBOL_NAME),

TOKEN_NAME			entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_TOKEN_NAME),

TOKEN_MEMBER_NAME		entry    (type (NID_T),
					  type (NID_T))
				returns  (character (512) varying)
				external (X_TOKEN_MEMBER_NAME),

TYPE_NAME			entry    (type (NID_T),
					  character (512) varying)
				returns  (character (512) varying)
				external (X_TYPE_NAME),

ANY_TYPE_NAME			entry    (type (POINTER_T),
					  type (SHORT_T),
					  type (LONG_BITS_T),
					  character (512) varying)
				returns  (character (512) varying)
				external (X_ANY_TYPE_NAME),

BASE_CLASS_NAME			entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_BASE_CLASS_NAME),

BASE_CLASS_NAMES		entry    (type (POINTER_T))
				returns  (character (512) varying)
				external (X_BASE_CLASS_NAMES),

BASE_MEMBER_NAME		entry    (type (POINTER_T))
				returns  (character (512) varying)
				external (X_BASE_MEMBER_NAME),

STORAGE_CLASS_CODE_NAME		entry    (type (SHORT_T))
				returns  (character (512) varying)
				external (X_STORAGE_CLASS_CODE_NAME),

STORAGE_CLASS_NAME		entry    (type (POINTER_T))
				returns  (character (512) varying)
				external (X_STORAGE_CLASS_NAME),

FUNCTION_SPECIFIER_NAME		entry    (type (POINTER_T))
				returns  (character (512) varying)
				external (X_FUNCTION_SPECIFIER_NAME),

MEMBER_ACCESS_CODE_NAME		entry    (type (SHORT_T))
				returns  (character (512) varying)
				external (X_MEMBER_ACCESS_CODE_NAME),

MEMBER_ACCESS_NAME		entry    (type (POINTER_T))
				returns  (character (512) varying)
				external (X_MEMBER_ACCESS_NAME),

SYMBOL_LOCATION_NAME		entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_SYMBOL_LOCATION_NAME);

