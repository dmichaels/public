/* C++ CXX_CHECK_DECLARATION_PKG - Miscellaneous declaration checking */

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
 *  07.03.92  PKT  006  Added CHECK_NESTED_TYPES.
 *  10.26.91  PKT  005  Added VALID_COPY_ASSIGNMENT.
 *  05.15.91  DGM  004  Added CHECK_MAIN_DECLARATION.
 *  01.31.91  DGM  003  Added CHECK_DEFAULT_ARGUMENT_ORDER.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  09.12.90  DGM  001  Added VALID_COPY_CONSTRUCTOR.
 *  08.31.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include GLOBAL_IN;              */
/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_CHECK_DECLARATION_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

VALID_SPECIAL_FUNCTION		entry    ()
				returns  (type (BOOL_T))
				external (X_VALID_SPECIAL_FUNCTION),

VALID_COPY_CONSTRUCTOR		entry    (type (NID_T))
				returns  (type (BOOL_T))
				external (X_VALID_COPY_CONSTRUCTOR),

VALID_COPY_ASSIGNMENT		entry    (type (NID_T))
				returns  (type (BOOL_T))
				external (X_VALID_COPY_ASSIGNMENT),

VALID_DECLARATOR_NAME		entry    (type (SHORT_T))
				returns  (type (BOOL_T))
				external (X_VALID_DECLARATOR_NAME),

VALID_BASE_TYPE			entry    ()
				returns  (type (BOOL_T))
				external (X_VALID_BASE_TYPE),

CHECK_UNSPECIFIED_BASE_TYPE	entry
				external (X_CHECK_UNSPECIFIED_BASE_TYPE),

CHECK_NULL_DECLARATOR		entry    ()
				external (X_CHECK_NULL_DECLARATOR),

CHECK_DEFAULT_ARGUMENT_ORDER	entry    (type (POINTER_T))
				external (X_CHECK_DEFAULT_ARGUMENT_ORDER),

CHECK_MAIN_DECLARATION		entry    ()
				external (X_CHECK_MAIN_DECLARATION),

CHECK_NON_VISIBLE_DECLARATIONS	entry    ()
				external (X_CHECK_NON_VISIBLE_DECLARATIONS),

CHECK_NESTED_TYPES		entry	 (type (POINTER_T))
				external (X_CHECK_NESTED_TYPES);

