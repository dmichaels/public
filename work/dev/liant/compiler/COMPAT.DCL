/* C++ CXX_TYPE_COMPATIBILITY_PKG - Type compatibility utility package */

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
 *  08.19.92  DGM  012  Added COMPATIBLE_ENOUGH_RETURN_TYPES.
 *  06.11.92  DGM  011  Pass pointers by value for efficiency.
 *  06.17.91  DGM  010  Added COMPATIBLE_PROTOTYPED_TYPES.
 *  05.17.91  PKT  009  Changed DISTINCT_FUNCTION_TYPES.
 *  09.22.90  DGM  007  Changed SHORT_BITS_T to TCM_T.
 *  09.14.90  DGM  006  Changed include file names and VOP names.
 *  08.29.90  DGM  005  Added DUMP_TYPE_COMPATIBILITY_STATS.
 *  08.08.90  DGM  005  New type-node.
 *  05.15.90  DGM  004  Rearranged.
 *  03.27.90  DGM  003  Added MATCH_OVERLOAD.
 *  02.26.90  TLF  002  Updated.
 *  02.09.90  DGM  000  Updated.
 *  01.04.90  DGM  000  Original.
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

%include CXX_TYPE_COMPATIBILITY_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

COMPATIBLE_SYMBOLS		entry    (type (NID_T),
					  type (NID_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_SYMBOLS),

COMPATIBLE_TYPES		entry    (type (NID_T),
					  type (NID_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_TYPES),

COMPATIBLE_TYPES_P		entry    (type (POINTER_T) value,
					  type (POINTER_T) value)
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_TYPES_P),

COMPATIBLE_ANY_TYPES		entry    (type (NID_T),
					  type (SHORT_T),
					  type (NID_T),
					  type (SHORT_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_ANY_TYPES),

COMPATIBLE_ANY_TYPES_P		entry    (type (POINTER_T) value,
					  type (SHORT_T),
					  type (POINTER_T) value,
					  type (SHORT_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_ANY_TYPES_P),

COMPATIBLE_SPECIAL_TYPES	entry    (type (NID_T),
					  type (TCM_T),
					  type (NID_T),
					  type (TCM_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_SPECIAL_TYPES),

COMPATIBLE_SPECIAL_TYPES_P	entry    (type (POINTER_T) value,
					  type (TCM_T),
					  type (POINTER_T) value,
					  type (TCM_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_SPECIAL_TYPES_P),

COMPATIBLE_ANY_SPECIAL_TYPES	entry    (type (NID_T),
					  type (SHORT_T),
					  type (TCM_T),
					  type (NID_T),
					  type (SHORT_T),
					  type (TCM_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_ANY_SPECIAL_TYPES),

COMPATIBLE_ANY_SPECIAL_TYPES_P	entry    (type (POINTER_T) value,
					  type (SHORT_T),
					  type (TCM_T),
					  type (POINTER_T) value,
					  type (SHORT_T),
					  type (TCM_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_ANY_SPECIAL_TYPES_P),

COMPATIBLE_TYPE_DATA		entry    (type (TYPE_DATA_T),
					  type (TYPE_DATA_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_TYPE_DATA),

COMPATIBLE_PROTOTYPED_TYPES	entry    (type (NID_T),
					  type (NID_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_PROTOTYPED_TYPES),

COMPATIBLE_DIMENSIONS 		entry   (type (NID_T),
					 type (NID_T))
				returns (type (BOOL_T))
				external (X_COMPATIBLE_DIMENSIONS),

COMPATIBLE_SIGNATURES		entry   (type (NID_T),
					 type (NID_T))
				returns (type (BOOL_T))
				external (X_COMPATIBLE_SIGNATURES),

DISTINCT_FUNCTION_TYPES		entry    (type (NID_T),
					  type (NID_T),
					  type (BOOL_T))
				returns  (type (BOOL_T))
				external (X_DISTINCT_FUNCTION_TYPES),

HAS_ALL_TYPE_QUALIFIERS_OF	entry	 (type (BYTE_BITS_T), 
					  type (BYTE_BITS_T))
				returns  (type (BOOL_T))
				external (X_HAS_ALL_TYPE_QUALIFIERS_OF),

COMPATIBLE_ENOUGH_RETURN_TYPES	entry	 (type (POINTER_T), 
					  type (POINTER_T))
				returns  (type (BOOL_T))
				external (X_COMPATIBLE_ENOUGH_RETURN_TYPES),

DUMP_TYPE_COMPATIBILITY_STATS	entry	 ()
				external (X_DUMP_TYPE_COMPATIBILITY_STATS);

