/* C++ CXX_NODE_LIST_PKG - Dynamic node list package interface */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  11.06.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_NODE_LIST_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

APPEND_TO_NODE_LIST		entry    (type (POINTER_T),
					  type (NID_T))
				external (X_APPEND_TO_NODE_LIST),

APPEND_TO_NODE_LIST_WITH_CODE	entry    (type (POINTER_T),
					  type (NID_T),
					  type (SHORT_T))
				external (X_APPEND_TO_NODE_LIST_WITH_CODE),

APPEND_TO_NODE_LIST_WITH_VALUE	entry    (type (POINTER_T),
					  type (NID_T),
					  type (LONG_T))
				external (X_APPEND_TO_NODE_LIST_WITH_VALUE),

ALREADY_ON_NODE_LIST		entry    (type (POINTER_T),
					  type (NID_T))
				returns  (type (BOOL_T))
				external (X_ALREADY_ON_NODE_LIST),

ALREADY_ON_NODE_LIST_WITH_CODE	entry    (type (POINTER_T),
					  type (NID_T),
					  type (SHORT_T))
				returns  (type (BOOL_T))
				external (X_ALREADY_ON_NODE_LIST_WITH_CODE),

REMOVE_LAST_FROM_NODE_LIST	entry    (type (POINTER_T))
				external (X_REMOVE_LAST_FROM_NODE_LIST),

DELETE_NODE_LIST		entry    (type (POINTER_T))
				external (X_DELETE_NODE_LIST);

