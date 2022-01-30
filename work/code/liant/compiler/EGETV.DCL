/* C++ CXX_GET_EXPANDED_VALUE_PKG - Interface value-node expanding routines */

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
 *  03.26.92  DGM  002  New value-node scheme.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  01.18.90  TLF  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* External entry point declartions
/* ------------------------------------------------------------------- */

declare

GET_VALUE_AND_EXPAND		entry    (type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_GET_VALUE_AND_EXPAND),

GET_VALUE_AND_EXPAND_L		entry    (type (SHORT_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_VALUE_AND_EXPAND_L),


GET_VALUE_AND_EXPAND_LR		entry    (type (SHORT_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_VALUE_AND_EXPAND_LR),

GET_VALUE_AND_EXPAND_LRT	entry    (type (SHORT_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_VALUE_AND_EXPAND_LRT),

GET_REF_AND_EXPAND		entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_REF_AND_EXPAND),

GET_VALUE_REF_AND_EXPAND	entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_VALUE_REF_AND_EXPAND),

GET_LABEL_REF_AND_EXPAND	entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_LABEL_REF_AND_EXPAND),

GET_BRANCH_REF_AND_EXPAND	entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_BRANCH_REF_AND_EXPAND),

GET_BRANCH_SAVE_AND_EXPAND	entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_BRANCH_SAVE_AND_EXPAND),

GET_DOT_AND_EXPAND		entry    (type (CXX_VID_T),
					  type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_DOT_AND_EXPAND),

GET_ENTRY_AND_EXPAND		entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_ENTRY_AND_EXPAND),

GET_PROCEDURE_AND_EXPAND	entry    (type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_PROCEDURE_AND_EXPAND),

GET_INIT_AND_EXPAND		entry    (type (NID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_INIT_AND_EXPAND),

GET_INIT_REF_AND_EXPAND		entry    (type (NID_T),
					  type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_GET_INIT_REF_AND_EXPAND),

GET_CONSTANT_AND_EXPAND		entry    (type (CXX_VID_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_GET_CONSTANT_AND_EXPAND),

GET_STATEMENT_ID_AND_EXPAND	entry    ()
				returns  (type (CXX_VID_T))
				external (X_GET_STATEMENT_ID_AND_EXPAND);

