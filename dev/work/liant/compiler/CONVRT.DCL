/* C++ CXX_CONVERT_VALUE_PKG - Interface to CONVRT routine  */

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
 *  04.30.92  PKT  012	Added CONVERT_OVERLOADED.
 *  03.26.92  DGM  011  New value-node scheme.
 *  01.10.92  PKT  010	Added CHECK_CTOR_DTOR_ACCESS.
 *  06.28.91  PKT  009  Bug fix.
 *  01.21.91  PKT  008  Added IS_CAST_VOP.
 *  11.01.90  PKT  007  Added CONVERT_TO_VOID.
 *  09.14.90  DGM  006  Changed include file names and VOP names.
 *  09.05.90  PKT  005  Fixed declaration of CONVERT_TO_POINTER_VALUE.
 *  08.08.90  DGM  004  New type-node; changed GENERAL_CONVERT_VALUE.
 *  06.29.90  DGM  003  More CAST_TO_xxx routines.
 *  06.25.90  DGM  002  Added CAST_TO_xxx routines.
 *  05.17.90  PKT  001  Added GENERAL_CONVERT_VALUE.
 *  01.24.90  TLF  000  Original.
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

CONVERT_VALUE      		entry    (type (CXX_VID_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_CONVERT_VALUE),

GENERAL_CONVERT_VALUE		entry    (type (CXX_VID_T),
					  type (TYPE_INFO_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_GENERAL_CONVERT_VALUE),

CONVERT_TO_POINTER_VALUE	entry    (type (CXX_VID_T),
					  type (TYPE_INFO_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_CONVERT_TO_POINTER_VALUE),

CAST_TO_TYPE			entry    (type (CXX_VID_T),
					  type (TYPE_INFO_T))
				returns  (type (CXX_VID_T))
				external (X_CAST_TO_TYPE),

CAST_TO_SIMPLE_TYPE		entry    (type (CXX_VID_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_CAST_TO_SIMPLE_TYPE),

CAST_TO_SIMPLE_POINTER		entry    (type (CXX_VID_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_CAST_TO_SIMPLE_POINTER),

CAST_TO_CLASS_POINTER		entry    (type (CXX_VID_T),
					  type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_CAST_TO_CLASS_POINTER),

CAST_TO_CLASS_POINTER_POINTER	entry    (type (CXX_VID_T),
					  type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_CAST_TO_CLASS_POINTER_POINTER),

CONVERT_TO_VOID			entry	 (type (CXX_VID_T))
				returns	 (type (CXX_VID_T))
				external (X_CONVERT_TO_VOID),

IS_CAST_VOP			entry    (type (SHORT_T))
				returns  (type (BOOL_T))
				external (X_IS_CAST_VOP),

CHECK_CTOR_DTOR_ACCESS		entry	 (type (TYPE_INFO_T))
				external (X_CHECK_CTOR_DTOR_ACCESS),

CONVERT_OVERLOADED		entry    (type (TYPE_INFO_T),
					  type (CXX_VID_T),
					  type (SHORT_T))
				returns  (type (BOOL_T))
				external (X_CONVERT_OVERLOADED);
