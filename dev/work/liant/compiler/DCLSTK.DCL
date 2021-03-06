/* C++ CXX_DECLARATION_STACK_PKG - Declaration semantic stack interface */

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
 *  01.10.91  DGM  008  Removed SAVE/RESTORE_SCOPE_STATE.
 *  09.14.90  DGM  007  Changed include file names and VOP names.
 *  08.14.90  DGM  006  Moved the type related stacks to CXX_TYPE_STACK_PKG.
 *  05.25.90  DGM  005  Added PUSH_OVER_xxx (see pardcl.pl1 for usage).
 *  04.04.90  DGM  004  Updated.
 *  02.15.90  DGM  003  Updated.
 *  10.02.89  DGM  000  Original.
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

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

PUSH_NODE			entry    (type (NID_T))
				external (X_PUSH_NODE),
POP_NODE			entry
				returns  (type (NID_T))
				external (X_POP_NODE),

SAVE_DECLARATION_STATE		entry
				external (X_SAVE_DECLARATION_STATE),
RESTORE_DECLARATION_STATE	entry
				external (X_RESTORE_DECLARATION_STATE),

SAVE_NAME_STATE			entry
				external (X_SAVE_NAME_STATE),
RESTORE_NAME_STATE		entry
				external (X_RESTORE_NAME_STATE),

SAVE_SIGNATURE_STATE		entry
				external (X_SAVE_SIGNATURE_STATE),
RESTORE_SIGNATURE_STATE		entry
				external (X_RESTORE_SIGNATURE_STATE),

SAVE_CLASS_STATE		entry
				external (X_SAVE_CLASS_STATE),
RESTORE_CLASS_STATE		entry
				external (X_RESTORE_CLASS_STATE),

SAVE_LINKAGE_STATE		entry
				external (X_SAVE_LINKAGE_STATE),
RESTORE_LINKAGE_STATE		entry
				external (X_RESTORE_LINKAGE_STATE);

