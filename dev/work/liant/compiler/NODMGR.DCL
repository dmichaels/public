/* C++ CXX_NODE_MGR_PKG - Node manager interface package */

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
 *  06.18.92  DGM  023	Added DATA_SECTION_NODE routines.
 *  06.11.92  DGM  022	Added SEQUENCE_NUMBER routines.
 *  03.08.92  PKT  021	Added PARAMETER_NODE utilities.
 *  01.21.92  DGM  020  Added LIST_NODE utilities.
 *  05.15.91  DGM  019  Added DERIVED_CLASS_NODE utilities.
 *  02.01.91  DGM  018  Added NEW_TOKEN_NODE & ALLOC_TOKEN_NODE.
 *  12.07.90  DGM  017  Added VBC_NODE utilities.
 *  09.14.90  DGM  016  Changed include file names and VOP names.
 *  08.31.90  DGM  015  Added COPY_TYPE_NODE.
 *  08.29.90  DGM  014  Added DUMP_TYPE_NODE_STATS.
 *  08.08.90  DGM  013  New type-node.
 *  06.05.90  DGM  012  Added FORM_TOKEN.
 *  05.25.90  DGM  011  Added MAKE_SYM_NODE_P, MAKE_SYM_NODE (renames
 *			MAKE_SYMBOL), and COPY_TYPE_NODE_P.
 *  05.16.90  DGM  010  Added COPY_DIM_NODE_LIST & COPY_SIG_NODE_LIST.
 *  03.02.90  DGM  009  Updated.
 *  02.21.90  DGM  008  Updated.
 *  02.20.90  PKT  007  Added MAKE_SYMBOL.
 *  02.15.90  DGM  006  Updated.
 *  02.05.90  DGM  005  Updated.
 *  12.12.89  DGM  003  Original.
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

INITIALIZE_NODE_MGR		entry
				external (X_INITIALIZE_NODE_MGR),

/* TYPE_NODE utilities */

ALLOC_TYPE_NODE			entry    (type (SHORT_T))
				external (X_ALLOC_TYPE_NODE),
NEW_TYPE_NODE			entry    (type (SHORT_T),
					  type (NID_T),
					  type (POINTER_T))
				external (X_NEW_TYPE_NODE),
FREE_TYPE_NODE			entry    (type (NID_T))
				external (X_FREE_TYPE_NODE),
FREE_TYPE_NODE_P		entry    (type (NID_T), type (POINTER_T))
				external (X_FREE_TYPE_NODE_P),
SET_TYPE_NODE			entry    (type (NID_T))
				external (X_SET_TYPE_NODE),
SET_TYPE_NODE_R			entry    (type (NID_T))
				external (X_SET_TYPE_NODE_R),
GET_TYPE_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_TYPE_NODE),
GET_TYPE_NODE_R			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_TYPE_NODE_R),
RELEASE_TYPE_NODE		entry    (type (NID_T))
				external (X_RELEASE_TYPE_NODE),

/* SYM_NODE utilities */

ALLOC_SYM_NODE			entry
				external (X_ALLOC_SYM_NODE),
NEW_SYM_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_SYM_NODE),
FREE_SYM_NODE			entry    (type (NID_T))
				external (X_FREE_SYM_NODE),
FREE_SYM_NODE_P			entry    (type (NID_T), type (POINTER_T))
				external (X_FREE_SYM_NODE_P),
SET_SYM_NODE			entry    (type (NID_T))
				external (X_SET_SYM_NODE),
SET_SYM_NODE_R			entry    (type (NID_T))
				external (X_SET_SYM_NODE_R),
GET_SYM_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_SYM_NODE),
GET_SYM_NODE_R			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_SYM_NODE_R),
RELEASE_SYM_NODE		entry    (type (NID_T))
				external (X_RELEASE_SYM_NODE),

MAKE_SYM_NODE			entry    (type (LONG_BITS_T))
				returns  (type (NID_T))
				external (X_MAKE_SYM_NODE),

MAKE_SYM_NODE_P			entry    (type (LONG_BITS_T),
					  type (NID_T),
					  type (POINTER_T))
				external (X_MAKE_SYM_NODE_P),

MAKE_SYMBOL			entry    (type (LONG_BITS_T))
				returns  (type (NID_T))
				external (X_MAKE_SYMBOL),

COPY_SYM_NODE			entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_COPY_SYM_NODE),

SYMBOL_SEQUENCE_NUMBER		entry    ()
				returns  (type (LONG_T))
				external (X_SYMBOL_SEQUENCE_NUMBER),
UPDATE_SYMBOL_SEQUENCE_NUMBER	entry    ()
				returns  (type (LONG_T))
				external (X_UPDATE_SYMBOL_SEQUENCE_NUMBER),


/* SIG_NODE utilities */

ALLOC_SIG_NODE			entry
				external (X_ALLOC_SIG_NODE),
NEW_SIG_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_SIG_NODE),
SET_SIG_NODE			entry    (type (NID_T))
				external (X_SET_SIG_NODE),
SET_SIG_NODE_R			entry    (type (NID_T))
				external (X_SET_SIG_NODE_R),
GET_SIG_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_SIG_NODE),
GET_SIG_NODE_R			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_SIG_NODE_R),
RELEASE_SIG_NODE		entry    (type (NID_T))
				external (X_RELEASE_SIG_NODE),

/* PARAMETER_NODE utilities */

ALLOC_PARAMETER_NODE		entry
				external (X_ALLOC_PARAMETER_NODE),
NEW_PARAMETER_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_PARAMETER_NODE),
SET_PARAMETER_NODE		entry    (type (NID_T))
				external (X_SET_PARAMETER_NODE),
SET_PARAMETER_NODE_R		entry    (type (NID_T))
				external (X_SET_PARAMETER_NODE_R),
GET_PARAMETER_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_PARAMETER_NODE),
GET_PARAMETER_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_PARAMETER_NODE_R),
RELEASE_PARAMETER_NODE		entry    (type (NID_T))
				external (X_RELEASE_PARAMETER_NODE),

/* LINK_NODE utilities */

ALLOC_LINK_NODE			entry
				external (X_ALLOC_LINK_NODE),
NEW_LINK_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_LINK_NODE),
SET_LINK_NODE			entry    (type (NID_T))
				external (X_SET_LINK_NODE),
SET_LINK_NODE_R			entry    (type (NID_T))
				external (X_SET_LINK_NODE_R),
GET_LINK_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_LINK_NODE),
GET_LINK_NODE_R			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_LINK_NODE_R),
RELEASE_LINK_NODE		entry    (type (NID_T))
				external (X_RELEASE_LINK_NODE),

/* DIM_NODE utilities */

ALLOC_DIM_NODE			entry
				external (X_ALLOC_DIM_NODE),
NEW_DIM_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_DIM_NODE),
SET_DIM_NODE			entry    (type (NID_T))
				external (X_SET_DIM_NODE),
SET_DIM_NODE_R			entry    (type (NID_T))
				external (X_SET_DIM_NODE_R),
GET_DIM_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_DIM_NODE),
GET_DIM_NODE_R			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_DIM_NODE_R),
RELEASE_DIM_NODE		entry    (type (NID_T))
				external (X_RELEASE_DIM_NODE),
/* SCOPE_NODE utilities */

ALLOC_SCOPE_NODE		entry
				external (X_ALLOC_SCOPE_NODE),
NEW_SCOPE_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_SCOPE_NODE),
FREE_SCOPE_NODE			entry    (type (NID_T))
				external (X_FREE_SCOPE_NODE),
FREE_SCOPE_NODE_P		entry    (type (NID_T), type (POINTER_T))
				external (X_FREE_SCOPE_NODE_P),
SET_SCOPE_NODE			entry    (type (NID_T))
				external (X_SET_SCOPE_NODE),
SET_SCOPE_NODE_R		entry    (type (NID_T))
				external (X_SET_SCOPE_NODE_R),
GET_SCOPE_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_SCOPE_NODE),
GET_SCOPE_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_SCOPE_NODE_R),
RELEASE_SCOPE_NODE		entry    (type (NID_T))
				external (X_RELEASE_SCOPE_NODE),

/* FRIEND_NODE utilities */

ALLOC_FRIEND_NODE		entry
				external (X_ALLOC_FRIEND_NODE),
NEW_FRIEND_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_FRIEND_NODE),
SET_FRIEND_NODE			entry    (type (NID_T))
				external (X_SET_FRIEND_NODE),
SET_FRIEND_NODE_R		entry    (type (NID_T))
				external (X_SET_FRIEND_NODE_R),
GET_FRIEND_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_FRIEND_NODE),
GET_FRIEND_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_FRIEND_NODE_R),
RELEASE_FRIEND_NODE		entry    (type (NID_T))
				external (X_RELEASE_FRIEND_NODE),

/* VBC_NODE utilities */

ALLOC_VBC_NODE			entry
				external (X_ALLOC_VBC_NODE),
NEW_VBC_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_VBC_NODE),
SET_VBC_NODE			entry    (type (NID_T))
				external (X_SET_VBC_NODE),
SET_VBC_NODE_R			entry    (type (NID_T))
				external (X_SET_VBC_NODE_R),
GET_VBC_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_VBC_NODE),
GET_VBC_NODE_R			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_VBC_NODE_R),
RELEASE_VBC_NODE		entry    (type (NID_T))
				external (X_RELEASE_VBC_NODE),

/* BASE_CLASS_NODE utilities */

ALLOC_BASE_CLASS_NODE		entry
				external (X_ALLOC_BASE_CLASS_NODE),
NEW_BASE_CLASS_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_BASE_CLASS_NODE),
SET_BASE_CLASS_NODE		entry    (type (NID_T))
				external (X_SET_BASE_CLASS_NODE),
SET_BASE_CLASS_NODE_R		entry    (type (NID_T))
				external (X_SET_BASE_CLASS_NODE_R),
GET_BASE_CLASS_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_BASE_CLASS_NODE),
GET_BASE_CLASS_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_BASE_CLASS_NODE_R),
RELEASE_BASE_CLASS_NODE		entry    (type (NID_T))
				external (X_RELEASE_BASE_CLASS_NODE),

/* DERIVED_CLASS_NODE utilities */

ALLOC_DERIVED_CLASS_NODE	entry
				external (X_ALLOC_DERIVED_CLASS_NODE),
NEW_DERIVED_CLASS_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_DERIVED_CLASS_NODE),
SET_DERIVED_CLASS_NODE		entry    (type (NID_T))
				external (X_SET_DERIVED_CLASS_NODE),
SET_DERIVED_CLASS_NODE_R	entry    (type (NID_T))
				external (X_SET_DERIVED_CLASS_NODE_R),
GET_DERIVED_CLASS_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_DERIVED_CLASS_NODE),
GET_DERIVED_CLASS_NODE_R	entry    (type (NID_T), type (POINTER_T))
				external (X_GET_DERIVED_CLASS_NODE_R),
RELEASE_DERIVED_CLASS_NODE	entry    (type (NID_T))
				external (X_RELEASE_DERIVED_CLASS_NODE),

/* BASE_MEMBER_NODE utilities */

ALLOC_BASE_MEMBER_NODE		entry
				external (X_ALLOC_BASE_MEMBER_NODE),
NEW_BASE_MEMBER_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_BASE_MEMBER_NODE),
SET_BASE_MEMBER_NODE		entry    (type (NID_T))
				external (X_SET_BASE_MEMBER_NODE),
SET_BASE_MEMBER_NODE_R		entry    (type (NID_T))
				external (X_SET_BASE_MEMBER_NODE_R),
GET_BASE_MEMBER_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_BASE_MEMBER_NODE),
GET_BASE_MEMBER_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_BASE_MEMBER_NODE_R),
RELEASE_BASE_MEMBER_NODE	entry    (type (NID_T))
				external (X_RELEASE_BASE_MEMBER_NODE),

/* SOURCE_NODE utilities */

ALLOC_SOURCE_NODE		entry
				external (X_ALLOC_SOURCE_NODE),
NEW_SOURCE_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_SOURCE_NODE),
SET_SOURCE_NODE			entry    (type (NID_T))
				external (X_SET_SOURCE_NODE),
SET_SOURCE_NODE_R		entry    (type (NID_T))
				external (X_SET_SOURCE_NODE_R),
GET_SOURCE_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_SOURCE_NODE),
GET_SOURCE_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_SOURCE_NODE_R),
RELEASE_SOURCE_NODE		entry    (type (NID_T))
				external (X_RELEASE_SOURCE_NODE),

/* LIST_NODE utilities */

ALLOC_LIST_NODE			entry
				external (X_ALLOC_LIST_NODE),
NEW_LIST_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_LIST_NODE),
SET_LIST_NODE			entry    (type (NID_T))
				external (X_SET_LIST_NODE),
SET_LIST_NODE_R			entry    (type (NID_T))
				external (X_SET_LIST_NODE_R),
GET_LIST_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_LIST_NODE),
GET_LIST_NODE_R			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_LIST_NODE_R),
RELEASE_LIST_NODE		entry    (type (NID_T))
				external (X_RELEASE_LIST_NODE),

/* TOKEN_NODE utilities */

FORM_IDENTIFIER_TOKEN		entry    (character (*) varying)
				returns  (type (NID_T))
				external (X_FORM_IDENTIFIER_TOKEN),
FORM_TOKEN			entry    (character (*) varying,
					  type (SHORT_T))
				returns  (type (NID_T))
				external (X_FORM_TOKEN),

ALLOC_TOKEN_NODE		entry
				external (X_ALLOC_TOKEN_NODE),
NEW_TOKEN_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_TOKEN_NODE),
SET_TOKEN_NODE			entry    (type (NID_T))
				external (X_SET_TOKEN_NODE),
SET_TOKEN_NODE_R		entry    (type (NID_T))
				external (X_SET_TOKEN_NODE_R),
GET_TOKEN_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_TOKEN_NODE),
GET_TOKEN_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_TOKEN_NODE_R),
RELEASE_TOKEN_NODE		entry    (type (NID_T))
				external (X_RELEASE_TOKEN_NODE),

/* BLOCK_NODE utilities */

ALLOC_BLOCK_NODE		entry
				external (X_ALLOC_BLOCK_NODE),
NEW_BLOCK_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_BLOCK_NODE),
SET_BLOCK_NODE			entry    (type (NID_T))
				external (X_SET_BLOCK_NODE),
SET_BLOCK_NODE_R		entry    (type (NID_T))
				external (X_SET_BLOCK_NODE_R),
GET_BLOCK_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_BLOCK_NODE),
GET_BLOCK_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_BLOCK_NODE_R),
RELEASE_BLOCK_NODE		entry    (type (NID_T))
				external (X_RELEASE_BLOCK_NODE),

/* ARRAY_NODE utilities */

ALLOC_ARRAY_NODE		entry
				external (X_ALLOC_ARRAY_NODE),
NEW_ARRAY_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_ARRAY_NODE),
SET_ARRAY_NODE			entry    (type (NID_T))
				external (X_SET_ARRAY_NODE),
SET_ARRAY_NODE_R		entry    (type (NID_T))
				external (X_SET_ARRAY_NODE_R),
GET_ARRAY_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_ARRAY_NODE),
GET_ARRAY_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_ARRAY_NODE_R),
RELEASE_ARRAY_NODE		entry    (type (NID_T))
				external (X_RELEASE_ARRAY_NODE),

/* DATA_INITIAL_NODE utilities */

ALLOC_DATA_INITIAL_NODE		entry
				external (X_ALLOC_DATA_INITIAL_NODE),
NEW_DATA_INITIAL_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_DATA_INITIAL_NODE),
SET_DATA_INITIAL_NODE		entry    (type (NID_T))
				external (X_SET_DATA_INITIAL_NODE),
SET_DATA_INITIAL_NODE_R		entry    (type (NID_T))
				external (X_SET_DATA_INITIAL_NODE_R),
GET_DATA_INITIAL_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_DATA_INITIAL_NODE),
GET_DATA_INITIAL_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_DATA_INITIAL_NODE_R),
RELEASE_DATA_INITIAL_NODE	entry    (type (NID_T))
				external (X_RELEASE_DATA_INITIAL_NODE),

/* DATA_SECTION_NODE utilities */

ALLOC_DATA_SECTION_NODE		entry
				external (X_ALLOC_DATA_SECTION_NODE),
NEW_DATA_SECTION_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_DATA_SECTION_NODE),
SET_DATA_SECTION_NODE		entry    (type (NID_T))
				external (X_SET_DATA_SECTION_NODE),
SET_DATA_SECTION_NODE_R		entry    (type (NID_T))
				external (X_SET_DATA_SECTION_NODE_R),
GET_DATA_SECTION_NODE		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_DATA_SECTION_NODE),
GET_DATA_SECTION_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_DATA_SECTION_NODE_R),
RELEASE_DATA_SECTION_NODE	entry    (type (NID_T))
				external (X_RELEASE_DATA_SECTION_NODE),

/* XXREF_NODE utilities */

ALLOC_XXREF_NODE		entry
				external (X_ALLOC_XXREF_NODE),
NEW_XXREF_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_NEW_XXREF_NODE),
SET_XXREF_NODE			entry    (type (NID_T))
				external (X_SET_XXREF_NODE),
SET_XXREF_NODE_R		entry    (type (NID_T))
				external (X_SET_XXREF_NODE_R),
GET_XXREF_NODE			entry    (type (NID_T), type (POINTER_T))
				external (X_GET_XXREF_NODE),
GET_XXREF_NODE_R		entry    (type (NID_T), type (POINTER_T))
				external (X_GET_XXREF_NODE_R),
RELEASE_XXREF_NODE		entry    (type (NID_T))
				external (X_RELEASE_XXREF_NODE),

/* Node statistics dumping routine */

DUMP_NODE_STATS			entry    ()
				external (X_DUMP_NODE_STATS);

