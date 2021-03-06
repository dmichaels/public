/* C++ CXX_SYMBOL_TABLE_INSTALL_PKG - Symbol table install interface */

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
 *  04.17.92  PKT  013	Modified CREATE_SIGNATURE for 'this' qualifier
 *			as part of signature.
 *  03.09.92  PKT  012	Added FREE_PROTOTYPE_SCOPE.
 *  03.26.92  DGM  011  New value-node scheme.
 *  01.28.92  DGM  010  Added MAKE_BACK_END_PARAMETER_LIST.
 *  04.29.91  DGM  009  Changed INSTALL_ANONYMOUS_UNION.
 *  10.22.90  DGM  008  Added CHANGE_SYMBOL_NAME.
 *  09.14.90  DGM  007  Changed include file names and VOP names.
 *  09.05.90  PKT  006  Updated.  DGM.
 *  08.29.90  DGM  005  Removed include of CXX_SYMBOL_TABLE_IN;
 *			changed CREATE_DIMENSION and CREATE_SIGNATURE.
 *  08.08.90  DGM  004  New type-node; added CREATE_DIMENSION.
 *  07.26.90  DGM  003  Added CREATE_SPECIAL_PARAMETERS;
 *			removed INSTALL_THIS_PARAMETER.
 *  07.13.90  DGM  002  Added INSTALL_ANONYMOUS_UNION.
 *  05.25.90  DGM  001  Added INSTALL_PARAMETERS & INSTALL_THIS_PARAMETER.
 *  05.15.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include GLOBAL_IN;              */
/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */
/* %include CXX_SYMBOL_TABLE_IN;      */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

INSTALL_SYMBOL			entry    (type (NID_T),
					  type (NID_T))
				external (X_INSTALL_SYMBOL),

CREATE_DIMENSION		entry    (type (CXX_VID_T),
					  type (BOOL_T))
				returns  (type (NID_T))
				external (X_CREATE_DIMENSION),

CREATE_CONSTANT_DIMENSION	entry    (type (LONG_T),
					  type (BOOL_T))
				returns  (type (NID_T))
				external (X_CREATE_CONSTANT_DIMENSION),

CREATE_SIGNATURE		entry    (type (NID_T),
					  type (BOOL_T),
					  type (BOOL_T),
					  type (BYTE_BITS_T),
					  type (BOOL_T),
					  type (BOOL_T))
				returns  (type (NID_T))
				external (X_CREATE_SIGNATURE),

CREATE_SPECIAL_PARAMETERS	entry    (type (NID_T),
					  type (NID_T))
				external (X_CREATE_SPECIAL_PARAMETERS),

INSTALL_PARAMETERS		entry    (type (NID_T), type (NID_T))
				external (X_INSTALL_PARAMETERS),

FREE_PROTOTYPE_SCOPE		entry	 (type (NID_T))
				external (X_FREE_PROTOTYPE_SCOPE),


INSTALL_ANONYMOUS_UNION		entry    (type (POINTER_T))
				external (X_INSTALL_ANONYMOUS_UNION),

CHANGE_SYMBOL_NAME		entry    (type (NID_T),
					  type (POINTER_T),
					  type (NID_T))
				external (X_CHANGE_SYMBOL_NAME),

OVERLOAD_SYMBOL			entry    (type (NID_T),
					  type (NID_T))
				external (X_OVERLOAD_SYMBOL),

BEFRIEND_SYMBOL			entry    (type (NID_T),
					  type (NID_T))
				external (X_BEFRIEND_SYMBOL),

MAKE_BACK_END_PARAMETER_LIST	entry    (type (NID_T),
					  type (NID_T))
				external (X_MAKE_BACK_END_PARAMETER_LIST);

