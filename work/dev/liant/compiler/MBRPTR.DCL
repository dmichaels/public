/* C++ CXX_MEMBER_POINTER_PKG - member pointer interface */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  03.26.92  DGM  004  New value-node scheme.
 *  05.22.91  PKT  003  Added FIXUP_OVERLOADED_MEMBER.
 *  03.08.91  PKT  002  Updated; COMPARE_MEMBER_POINTERS added.
 *  02.21.91  PKT  001  Added several important functions.
 *  01.18.91  PKT  000  Original.
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

GET_MEMBER_POINTER_VALUE	entry	 (type (NID_T),
					  type (NID_T))
				returns	 (type (CXX_VID_T))
				external (X_GET_MEMBER_POINTER_VALUE),

GET_SIMPLE_MEMBER_VALUE		entry	 (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_SIMPLE_MEMBER_VALUE),

INITIALIZE_MEMBER_VALUE		entry    (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_INITIALIZE_MEMBER_VALUE),

TRANSMUTE_MEMBER_VALUE		entry    (type (CXX_VID_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_TRANSMUTE_MEMBER_VALUE),

MEMBER_POINTER_CONSTANTS	entry	 (type (CXX_VID_T),
					  type (BOOL_T),
					  type (BOOL_T),
					  type (CXX_VID_T), 
					  type (CXX_VID_T),
					  type (CXX_VID_T))
				external (X_MEMBER_POINTER_CONSTANTS),

MEMBER_POINTER_INFO		entry    (type (CXX_VID_T),
					  type (BOOL_T),
					  type (BOOL_T),
					  type (BOOL_T),
					  type (BOOL_T),
					  type (LONG_T),
					  type (LONG_T),
					  type (LONG_T),
					  type (NID_T))
				external (X_MEMBER_POINTER_INFO),

IS_CONSTANT_MEMBER_POINTER	entry    (type (CXX_VID_T),
					  type (BOOL_T))
				returns  (type (BOOL_T))
				external (X_IS_CONSTANT_MEMBER_POINTER),

MEMBER_POINTER_TO_LOGICAL	entry    (type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_MEMBER_POINTER_TO_LOGICAL),

MEMBER_POINTER_TO_INTEGRAL	entry    (type (CXX_VID_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_MEMBER_POINTER_TO_INTEGRAL),

GET_LOW_LEVEL_TYPE_INFO		entry    (type (CXX_VID_T),
					  type (TYPE_INFO_T),
				          type (BOOL_T))
				external (X_GET_LOW_LEVEL_TYPE_INFO),

COMPARE_MEMBER_POINTERS		entry    (type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_COMPARE_MEMBER_POINTERS),

FIXUP_OVERLOADED_MEMBER		entry	 (type (CXX_VID_T),
					  type (NID_T))
				returns	 (type (CXX_VID_T))
				external (X_FIXUP_OVERLOADED_MEMBER);

