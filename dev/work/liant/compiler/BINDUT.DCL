/* C++ C_BIND_EXPRESSIN_UTL_PKG - Expression bind utility interface */

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
 *  04.03.92  PKT  013	Added GENERAL_VALUE_OF.
 *  03.26.92  DGM  012  New value-node scheme.
 *  01.10.92  PKT  011	Removed type class parameters from
 *			BINARY_CHECKS, UNARY_CHECKS, and
 *			CHECK_BINARY_OVERLOAD.
 *  11.12.91  DGM  010  Changed GET_INTEGRAL_CONSTANT_VALUE.
 *  08.23.91  DGM  009  Added CHECK_IF_USED_BEFORE_SET.
 *  07.08.91  PKT  008  Added NOTE_SYMBOL_REFERENCE.
 *  02.27.91  DGM  007  Removed AUTO_DEREF.
 *  12.17.90  DGM  006  Added GET_INTEGRAL_CONSTANT_VALUE.
 *  09.14.90  DGM  005  Changed include file names and VOP names.
 *  09.05.90  TLF  004  Updated.
 *  08.27.90  TLF  003  Updated.
 *  08.21.90  DGM  002  Updated.
 *  06.15.90  TLF  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

VALUE_OF			entry    (type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_VALUE_OF),

GENERAL_VALUE_OF		entry    (type (CXX_VID_T),
					  type (BOOL_T), type (BOOL_T))
				returns  (type (CXX_VID_T))
				external (X_GENERAL_VALUE_OF),

BINARY_CHECKS			entry    (type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (BOOL_T))
				external (X_BINARY_CHECKS),

UNARY_CHECKS			entry    (type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (BOOL_T))
				external (X_UNARY_CHECKS),

CHECK_BINARY_OVERLOAD		entry    (type (CXX_VID_T),
				          type (BOOL_T))
				external (X_CHECK_BINARY_OVERLOAD),

CHECK_CALL_OVERLOAD		entry    (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (BOOL_T))
				external (X_CHECK_CALL_OVERLOAD),

CHECK_ARROW_OVERLOAD		entry    (type (CXX_VID_T))
				external (X_CHECK_ARROW_OVERLOAD),

GET_INTEGRAL_CONSTANT_VALUE	entry    (type (CXX_VID_T),
					  type (LONG_T),
					  type (SHORT_T))
				returns  (type (BOOL_T))
				external (X_GET_INTEGRAL_CONSTANT_VALUE),

NOTE_SYMBOL_REFERENCE		entry	 (type (NID_T))
				external (X_NOTE_SYMBOL_REFERENCE),

CHECK_IF_USED_BEFORE_SET	entry    (type (CXX_VID_T),
					  type (SHORT_T))
				external (X_CHECK_IF_USED_BEFORE_SET);

