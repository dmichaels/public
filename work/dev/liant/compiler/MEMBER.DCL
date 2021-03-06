/* C++ CXX_MEMBER_VALUE_PKG - member value interface */

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
 *  03.26.92  DGM  005  New value-node scheme.
 *  10.24.91  PKT  004  Modified DEREFERENCE_MEMBER.
 *  06.27.91  PKT  003  Added FIXUP_MEMBER_CALL.
 *  02.21.91  PKT  002  Added MEMBER_OF_OBJECT; 
 *			changed MEMBER_FUNCTION_CALL.
 *  01.27.91  PKT  001  Updated for member pointers.
 *  12.11.90  PKT  000  Original.
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

ACCESS_DATA_MEMBER		entry    (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_ACCESS_DATA_MEMBER),

MEMBER_FUNCTION_CALL		entry    (type (CXX_VID_T),
				          type (CXX_VID_T),
					  type (CXX_VID_T),
				          type (SHORT_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_MEMBER_FUNCTION_CALL),

FIXUP_MEMBER_CALL		entry    (type (LONG_T),
					  type (CXX_VID_T), 
					  type (CXX_VID_T))
				external (X_FIXUP_MEMBER_CALL),

DEREFERENCE_MEMBER		entry    (type (CXX_VID_T),
					  type (BOOL_T))
				returns  (type (CXX_VID_T))
				external (X_DEREFERENCE_MEMBER),

MEMBER_OF_OBJECT		entry	 (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns	 (type (CXX_VID_T))
				external (X_MEMBER_OF_OBJECT);

