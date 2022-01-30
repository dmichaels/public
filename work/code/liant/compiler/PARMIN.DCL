/* C++ CXX_PROCESS_BASE_MBR_INIT_PKG - Base/member initialiation actions */

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
 *  04.14.92  PKT  004	Added GET_UNION_INIT and IMPLICIT_INITIALIZER.
 *  03.26.92  DGM  003  New value-node scheme.
 *  03.05.91  PKT  002  Updated for destructor access fix.
 *  01.11.91  PKT  001  Changed interface to GET_BASE_MEMBER_INIT.
 *  10.03.90  PKT  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */
/* %include CXX_TYPE_INFO_IN;         */

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

BASE_MEMBER_VALUE		entry    ()
				external (X_BASE_MEMBER_VALUE),

BEGIN_MEMBER_INITIAL_LIST	entry    ()
				external (X_BEGIN_MEMBER_INITIAL_LIST),

CHECK_MEMBER_INITIAL_LIST	entry    ()
				external (X_CHECK_MEMBER_INITIAL_LIST),

SET_MEMBER_INITIALIZER		entry    ()
				external (X_SET_MEMBER_INITIALIZER),

SET_MEMBER_INITIAL_LIST		entry    (type (NID_T))
				external (X_SET_MEMBER_INITIAL_LIST),

GET_BASE_MEMBER_INIT		entry    (type (NID_T),
					  type (LONG_T),
				          type (SHORT_T),
					  type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_BASE_MEMBER_INIT),

GET_UNION_INIT			entry	 (type (NID_T))
				returns	 (type (CXX_VID_T))
				external (X_GET_UNION_INIT),

GET_BASE_MEMBER_FINI		entry    (type (NID_T),
				          type (LONG_T),
					  type (SHORT_T),
				          type (TYPE_DATA_T))
				returns  (type (CXX_VID_T))
				external (X_GET_BASE_MEMBER_FINI),

IMPLICIT_INITIALIZER		entry	 (type (NID_T),
					  type (NID_T),
					  type (BOOL_T), type (BOOL_T),
					  type (NID_T))
				returns	 (type (CXX_VID_T))
				external (X_IMPLICIT_INITIALIZER);

