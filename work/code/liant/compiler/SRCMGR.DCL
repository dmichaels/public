/* C++ CXX_SOURCE_MGR_PKG - Interface to the front-end source manager */

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
 *  05.19.92  DGM  005  Updated POP_SOURCE.
 *  03.08.91  DGM  004  Removed GET_SOURCE_NODE_LIST.
 *  09.14.90  DGM  003  Changed include file names and VOP names.
 *  06.08.90  DGM  002  Updated for the new utilities.
 *  11.08.89  DGM  001  Original.
 *
 ***********************************************************************/

%include CXX_SOURCE_MGR_IN;

declare

PUSH_SOURCE		entry    (type (NID_T), type (LONG_T))
			external (X_PUSH_SOURCE),

POP_SOURCE		entry    (type (LONG_T))
			external (X_POP_SOURCE),

GET_SOURCE_STATE	entry
			returns  (pointer)
			external (X_GET_SOURCE_STATE),

SET_SOURCE_STATE	entry    (pointer)
			external (X_SET_SOURCE_STATE),

COPY_SOURCE_STATE	entry
			returns  (pointer)
			external (X_COPY_SOURCE_STATE),

PURGE_SOURCE_STATE	entry    (pointer)
			external (X_PURGE_SOURCE_STATE),

SET_SOURCE_MAP		entry    (type (LONG_T))
			external (X_SET_SOURCE_MAP),

GET_SOURCE_MAP		entry    ()
			returns  (type (NID_T))
			external (X_GET_SOURCE_MAP);

