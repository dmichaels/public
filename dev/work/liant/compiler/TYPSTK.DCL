/* C++ CXX_TYPE_STACK_PKG - Type semantic stack interface */

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
 *  03.13.92  PKT  009	Added TOP_DERIVED_TYPE_DATA.
 *  01.21.92  DGM  008  Added POKE_DERIVED_TYPE
 *  03.08.91  DGM  007  Changed TOP_DERIVED_TYPE.
 *  12.17.90  DGM  006  Added TOP_DERIVED_TYPE.
 *  09.25.90  DGM  005  Removed PUSH_OVER_DERIVED_TYPE,
 *			CHANGE_DERIVED_TYPE, and TOP_DERIVED_TYPE.
 *  09.14.90  DGM  004  Changed include file names and VOP names.
 *  08.29.90  DGM  003  Removed SAVE/RESTORE_TYPE_STATE.
 *  08.21.90  DGM  002  Added CHANGE_DERIVED_TYPE.
 *  08.14.90  DGM  001  Simplified.
 *  08.08.90  DGM  000  Original; spliced out of dclstk.dcl.
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

PUSH_DERIVED_TYPE		entry    (type (TYPE_DATA_T))
				external (X_PUSH_DERIVED_TYPE),

POKE_DERIVED_TYPE		entry    (type (TYPE_DATA_T),
					  type (SHORT_T))
				external (X_POKE_DERIVED_TYPE),

POP_DERIVED_TYPE		entry    (type (TYPE_DATA_T))
				external (X_POP_DERIVED_TYPE),

TOP_DERIVED_TYPE		entry    (type (SHORT_T))
				returns  (type (SHORT_T))
				external (X_TOP_DERIVED_TYPE),

TOP_DERIVED_TYPE_DATA		entry    (type (SHORT_T), type (TYPE_DATA_T))
				external (X_TOP_DERIVED_TYPE_DATA),

PUSH_TYPE_QUALIFIER		entry    (type (BYTE_BITS_T))
				external (X_PUSH_TYPE_QUALIFIER),

POP_TYPE_QUALIFIER		entry
				returns  (type (BYTE_BITS_T))
				external (X_POP_TYPE_QUALIFIER),

PUSH_CLASS_QUALIFIER		entry    (type (NID_T))
				external (X_PUSH_CLASS_QUALIFIER),

POP_CLASS_QUALIFIER		entry
				returns  (type (NID_T))
				external (X_POP_CLASS_QUALIFIER);

