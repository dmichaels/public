/* CPP CPP_MACRO_UTL_PKG - Macro utilities interface */

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
 *  11.12.91  DGM  004  Added LOOKUP_MACRO_BY_TOKEN.
 *  07.08.91  DGM  003  Added DEFINE_ASSERT, et. al.
 *  03.08.91  DGM  002  Added IS_DEFINED_MACRO.
 *  09.25.90  DGM  001  Include file name changes.
 *  06.13.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CPP_EXTERNAL_NAME_MAP_IN; */
/* %include CPP_TOKEN_LIMITS_IN;       */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

DEFINE_MACRO			entry    (type (NID_T))
				returns  (type (POINTER_T))
				external (X_DEFINE_MACRO),

LOOKUP_MACRO			entry    (type (NID_T))
				returns  (type (POINTER_T))
				external (X_LOOKUP_MACRO),

LOOKUP_MACRO_BY_TOKEN		entry    (type (POINTER_T))
				returns  (type (POINTER_T))
				external (X_LOOKUP_MACRO_BY_TOKEN),

IS_DEFINED_MACRO		entry    (type (NID_T))
				returns  (type (BOOL_T))
				external (X_IS_DEFINED_MACRO),

UNDEF_MACRO			entry    (type (NID_T))
				external (X_UNDEF_MACRO),

PURGE_MACROS			entry    ()
				external (X_PURGE_MACROS),

DEFINE_ASSERT			entry    (type (NID_T),
					  type (POINTER_T))
				external (X_DEFINE_ASSERT),

UNDEF_ASSERT			entry    (type (NID_T),
					  type (POINTER_T))
				external (X_UNDEF_ASSERT),

LOOKUP_ASSERT			entry    (type (NID_T),
					  type (POINTER_T))
				returns  (type (POINTER_T))
				external (X_LOOKUP_ASSERT),

MACRO_NAME			entry    (type (POINTER_T))
				returns  (character
					  (MAX_TOKEN_SPELLING_LENGTH) varying)
				external (X_MACRO_NAME),

ASSERT_MACRO_NAME		entry    (type (POINTER_T))
				returns  (character
					  (MAX_TOKEN_SPELLING_LENGTH) varying)
				external (X_ASSERT_MACRO_NAME),

SAME_MACRO_REPLACEMENT_LIST	entry    (type (POINTER_T),
					  type (POINTER_T))
				returns  (type (BOOL_T))
				external (X_SAME_MACRO_REPLACEMENT_LIST),

SAME_MACRO_PARAMETER_LIST	entry    (type (POINTER_T),
					  type (POINTER_T),
					  type (SHORT_T))
				returns  (type (BOOL_T))
				external (X_SAME_MACRO_PARAMETER_LIST),

FREE_MACRO_REPLACEMENT_LIST	entry    (type (POINTER_T))
				external (X_FREE_MACRO_REPLACEMENT_LIST),

FREE_MACRO_PARAMETER_LIST	entry    (type (POINTER_T))
				external (X_FREE_MACRO_PARAMETER_LIST),

MAKE_DEBUGGER_MACROS		entry    ()
				external (X_MAKE_DEBUGGER_MACROS);

declare

COLLECT_ASSERT_TOKENS		entry    (type (POINTER_T))
				returns  (type (POINTER_T))
				external (X_COLLECT_ASSERT_TOKENS),

COLLECT_PREDICATE_TOKENS	entry    (type (POINTER_T))
				returns  (type (POINTER_T))
				external (X_COLLECT_PREDICATE_TOKENS);

