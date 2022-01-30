/* C++ C_EXPRESSION_COMPATIBILITY_PKG - expression type compatibility intrfc */

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
 *  04.18.92  PKT  013	Merged 010->011 with 010->012.
 *  03.26.92  DGM  012  New value-node scheme.
 *  03.30.92  DGM  011  New INCOMPATIBLE_xxx interface.
 *  10.24.91  PKT  010  Made IS_TD_BASE_CLASS_OF external.
 *  10.22.90  DGM  009  Changed SHORT_BITS_T to TCM_T.
 *  09.28.90  PKT  008  Added COMPATIBLE_INITIAL_REFERENCES and
 *			INCOMPATIBLE_REFERENCE_ERROR.
 *  09.14.90  DGM  007  Changed include file names and VOP names.
 *  09.12.90  DGM  006  Changed COMPATIBLE_INITIAL_CLASSES
 *			to COMPATIBLE_CLASSES.
 *  08.21.90  TLF  005  Added IS_GENERIC_POINTER/IS_NULL_POINTER_CONSTANT.
 *  08.08.90  DGM  004  New type-node.
 *  07.20.90  PKT  003  Added IDENTICAL_CONDITIONAL_LVALUES.
 *  06.29.90  DGM  002  Changed COMPATIBLE_RELATIONAL_POINTERS.
 *  06.18.90  TLF  001  Updated to add COMPATIBLE_UNQUALIFIED_TYPES.
 *  05.17.90  PKT  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_TYPE_COMPATIBILITY_IN; */

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

COMPATIBLE_ASSIGNMENT_POINTERS	entry	 (type (TYPE_INFO_T),
					  type (CXX_VID_T),
					  type (SHORT_T))
				returns	 (type (BOOL_T))
				external (X_COMPATIBLE_ASSIGNMENT_POINTERS),

COMPATIBLE_INITIAL_REFERENCES	entry	 (type (TYPE_INFO_T),
					  type (TYPE_INFO_T),
					  type (SHORT_T))
				returns	 (type (BOOL_T))
				external (X_COMPATIBLE_INITIAL_REFERENCES),

COMPATIBLE_CLASSES		entry	 (type (TYPE_INFO_T),
					  type (CXX_VID_T),
					  type (BOOL_T),
					  type (SHORT_T))
				returns	 (type (BOOL_T))
				external (X_COMPATIBLE_CLASSES),

COMPATIBLE_POINTERS		entry    (type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (SHORT_T))
				returns	 (type (BOOL_T))
				external (X_COMPATIBLE_POINTERS),

COMPATIBLE_RELATIONAL_POINTERS	entry	 (type (CXX_VID_T), 
					  type (CXX_VID_T),
					  type (SHORT_T))
				returns	 (type (BOOL_T))
				external (X_COMPATIBLE_RELATIONAL_POINTERS),

COMPATIBLE_CONDITIONAL_POINTERS	entry	 (type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (BOOL_T))
				returns	 (type (BOOL_T))
				external (X_COMPATIBLE_COND_POINTERS),

COMPATIBLE_CONDITIONAL_CLASSES	entry	 (type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (BOOL_T))
				returns	 (type (BOOL_T))
				external (X_COMPATIBLE_CONDITIONAL_CLASSES),

IDENTICAL_CONDITIONAL_LVALUES	entry	 (type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T))
				returns	 (type (BOOL_T))
				external (X_IDENTICAL_CONDITIONAL_LVALUES),

COMPATIBLE_TYPE_INFO		entry    (type (TYPE_INFO_T),
					  type (TYPE_INFO_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_TYPE_INFO),

COMPATIBLE_SPECIAL_TYPE_INFO	entry    (type (TYPE_INFO_T),
					  type (TCM_T),
					  type (TYPE_INFO_T),
					  type (TCM_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_SPECIAL_TYPE_INFO),

INCOMPATIBLE_POINTER_ERROR	entry    (type (SHORT_T),
					  type (TYPE_INFO_T),
					  type (TYPE_INFO_T))
				external (X_INCOMPATIBLE_POINTER_ERROR),

INCOMPATIBLE_REFERENCE_ERROR	entry    (type (SHORT_T),
					  type (TYPE_INFO_T),
					  type (TYPE_INFO_T))
				external (X_INCOMPATIBLE_REFERENCE_ERROR),

IS_NULL_POINTER_CONSTANT	entry    (type (CXX_VID_T))
				returns  (type (BOOL_T))
				external (X_IS_NULL_POINTER_CONSTANT),

IS_GENERIC_POINTER		entry    (type (TYPE_INFO_T))
				returns  (type (BOOL_T))
				external (X_IS_GENERIC_POINTER),

IS_TD_BASE_CLASS_OF		entry	 (type (TYPE_DATA_T), 
					  type (TYPE_DATA_T))
				returns	 (type (BOOL_T))
				external (X_IS_TD_BASE_CLASS_OF);

