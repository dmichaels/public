/* C++ CXX_DECLARE_TYPE_PKG - Miscellaneous type declaration routines */

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
 *  11.12.91  DGM  012  Added GET_WCHAR_T_TYPE.
 *  05.29.91  PKT  011  Added GET_NULL_FUNCTION_TYPE.
 *  05.29.91  DGM  010  Removed GET_SIMPLE_TYPE; added GET_CLASS_TYPE.
 *  10.25.90  TLF  009  Added GET_XLINK_STRUCT_TYPE.
 *  09.14.90  DGM  008  Changed include file names and VOP names.
 *  08.31.90  DGM  007	Added GET_SIZET_TYPE.
 *  08.29.90  DGM  006	Added GET_BASIC_TYPE, GET_BASIC_POINTER_TYPE,
 *			and GET_LABEL_TYPE; meant to eventually replace
 *			GET_SIMPLE_TYPE and GET_SIMPLE_POINTER_TYPE.
 *  08.08.90  DGM  005	New type-node.
 *  07.11.90  DGM  004	Updated.
 *  06.25.90  DGM  003	Updated.
 *  05.25.90  DGM  002	Added GET_VFT_POINTER_TYPE.
 *  05.17.90  DGM  001  Added GET_VFT_TYPE.
 *  04.19.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

GET_INT_TYPE			entry    ()
				returns  (type (NID_T))
				external (X_GET_INT_TYPE),

GET_VOID_TYPE			entry    ()
				returns  (type (NID_T))
				external (X_GET_VOID_TYPE),

GET_VOID_POINTER_TYPE		entry    ()
				returns  (type (NID_T))
				external (X_GET_VOID_POINTER_TYPE),

GET_SIZET_TYPE			entry    ()
				returns  (type (NID_T))
				external (X_GET_SIZET_TYPE),

GET_WCHAR_T_TYPE		entry    ()
				returns  (type (NID_T))
				external (X_GET_WCHAR_T_TYPE),

GET_WCHAR_T_TYPE_CODE		entry    ()
				returns  (type (SHORT_T))
				external (X_GET_WCHAR_T_TYPE_CODE),

GET_LABEL_TYPE			entry    ()
				returns  (type (NID_T))
				external (X_GET_LABEL_TYPE),

GET_BASIC_TYPE			entry    (type (SHORT_T))
				returns  (type (NID_T))
				external (X_GET_BASIC_TYPE),

GET_BASIC_POINTER_TYPE		entry    (type (SHORT_T))
				returns  (type (NID_T))
				external (X_GET_BASIC_POINTER_TYPE),

GET_CLASS_POINTER_TYPE		entry    (type (SHORT_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_GET_CLASS_POINTER_TYPE),

GET_CLASS_TYPE			entry    (type (SHORT_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_GET_CLASS_TYPE),

GET_SCALAR_TYPE			entry    (type (SHORT_T),
					  type (BYTE_BITS_T),
					  type (SHORT_T),
					  type (NID_T),
					  type (BYTE_BITS_T))
				returns  (type (NID_T))
				external (X_GET_SCALAR_TYPE),

GET_SIMPLE_ARRAY_TYPE		entry    (type (SHORT_T),
					  type (LONG_T))
				returns  (type (NID_T))
				external (X_GET_SIMPLE_ARRAY_TYPE),

GET_MEMBER_POINTER_TYPE		entry    (type (TYPE_DATA_T))
				returns  (type (NID_T))
				external (X_GET_MEMBER_POINTER_TYPE),

GET_VFT_ENTRY_TYPE		entry    (type (TYPE_DATA_T))
				returns  (type (NID_T))
				external (X_GET_VFT_ENTRY_TYPE),

GET_VFT_TYPE			entry    (type (TYPE_DATA_T))
				returns  (type (NID_T))
				external (X_GET_VFT_TYPE),

GET_VFT_POINTER_TYPE		entry    (type (TYPE_DATA_T))
				returns  (type (NID_T))
				external (X_GET_VFT_POINTER_TYPE),

GET_XLINK_STRUCT_TYPE		entry    (type (TYPE_DATA_T))
				returns  (type (NID_T))
				external (X_GET_XLINK_STRUCT_TYPE),

GET_NULL_FUNCTION_TYPE		entry
				returns	 (type (NID_T))
				external (X_GET_NULL_FUNCTION_TYPE);

