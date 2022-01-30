/* C++ - CXX_VALUE_NODE_MGR_PKG - Interface to value-node utilities */

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
 *  04.01.92  PKT  004	Updated interface to ALLOC_DELETE_CXX_VALUE_NODE.
 *  03.26.92  DGM  003  New value-node scheme.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  01.25.90  TLF  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* External function declarations
/* ------------------------------------------------------------------- */

declare

ALLOC_AND_COPY_CXX_VALUE_NODE	entry    (type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_AND_COPY_CXX_VALUE_NODE),

ALLOC_EMPTY_CXX_VALUE_NODE	entry    ()
				returns  (type (CXX_VID_T))
				external (X_ALLOC_EMPTY_CXX_VALUE_NODE),

ALLOC_CXX_VALUE_NODE		entry    (type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_CXX_VALUE_NODE),

ALLOC_CXX_VALUE_NODE_L		entry    (type (SHORT_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_CXX_VALUE_NODE_L),

ALLOC_CXX_VALUE_NODE_LR		entry    (type (SHORT_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_CXX_VALUE_NODE_LR),

ALLOC_CXX_VALUE_NODE_LRT	entry    (type (SHORT_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_CXX_VALUE_NODE_LRT),

ALLOC_SYMBOL_CXX_VALUE_NODE	entry    (type (SHORT_T),
					  type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_SYMBOL_CXX_VALUE_NODE),

ALLOC_REF_CXX_VALUE_NODE	entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_REF_CXX_VALUE_NODE),

ALLOC_LABEL_REF_CXX_VALUE_NODE	entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_LABEL_REF_CXX_VALUE_NODE),

ALLOC_DOT_CXX_VALUE_NODE	entry    (type (CXX_VID_T),
					  type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_DOT_CXX_VALUE_NODE),

ALLOC_MEMBER_CXX_VALUE_NODE	entry    (type (NID_T),
					  type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_MEMBER_CXX_VALUE_NODE),

ALLOC_NEW_CXX_VALUE_NODE	entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_NEW_CXX_VALUE_NODE),

ALLOC_DELETE_CXX_VALUE_NODE	entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_DELETE_CXX_VALUE_NODE),

ALLOC_INIT_CXX_VALUE_NODE	entry    (type (NID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_INIT_CXX_VALUE_NODE),

ALLOC_INIT_REF_CXX_VALUE_NODE	entry    (type (NID_T),
					  type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_INIT_REF_CXX_VALUE_NODE),

ALLOC_CONSTANT_CXX_VALUE_NODE	entry    (type (CXX_VID_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_ALLOC_CONSTANT_CXX_VALUE_NODE),

FREE_CXX_VALUE_NODE		entry    (type (CXX_VID_T))
				external (X_FREE_CXX_VALUE_NODE);


