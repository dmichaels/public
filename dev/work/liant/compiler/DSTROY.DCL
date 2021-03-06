/* C++ CXX_CALL_DESTRUCTOR_PKG - Automatic destruction support interface */

/***********************************************************************
 * This product is the property of Liant Software Corporation and is   *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Liant Software Corporation except pursuant to the license agreement.*
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  03.26.92  DGM  006  New value-node scheme.
 *  07.30.91  TLF  005  Added MAKE_FIRST_TIME_FLAG, changed interface
 *			to NOTE_DESTRUCTOR.
 *  03.05.91  PKT  004  Updated for destructor access fix.
 *  11.01.90  PKT  003  Moved VALUE_BEFORE_EFFECT to clsutl.dcl.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  09.05.90  PKT  001  Added parameters to dstroy.dcl.
 *  07.24.90  PKT  000  Original.
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

%include CXX_CALL_DESTRUCTOR_IN;

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

DESTROY_OBJECT			entry    (type (CXX_VID_T),
					  type (CXX_VID_T),
				          type (TYPE_DATA_T),
					  type (BOOL_T),
				          type (NID_T))
				returns  (type (CXX_VID_T))
				external (X_DESTROY_OBJECT),

HAS_DESTRUCTOR			entry    (type (NID_T))
				returns  (type (BOOL_T))
				external (X_HAS_DESTRUCTOR),

DESTRUCTOR_OF_TYPE		entry    (type (NID_T),
					  type (SHORT_T))
				returns  (type (NID_T))
				external (X_DESTRUCTOR_OF_TYPE),

NOTE_CURRENT_DESTRUCTOR		entry    ()
				external (X_NOTE_CURRENT_DESTRUCTOR),

NOTE_DESTRUCTOR			entry    (type (CXX_VID_T),
					  type (CXX_VID_T),
				          type (BOOL_T),
					  type (CXX_VID_T))
				external (X_NOTE_DESTRUCTOR),

DESTROY_DIFFERENCE		entry    (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_DESTROY_DIFFERENCE),

FLUSH_TEMPORARIES		entry    ()
				external (X_FLUSH_TEMPORARIES),

EMIT_BRANCH_VALUE_WITH_DTORS	entry    (type (SHORT_T),
					  type (CXX_VID_T),
				          type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (SHORT_T))
				external (X_EMIT_BRANCH_VALUE_WITH_DTORS),

EMIT_BRANCH_WITH_DTORS		entry    (type (NID_T),
				          type (CXX_VID_T))
				returns  (type (SHORT_T))
				external (X_EMIT_BRANCH_WITH_DTORS),

EMIT_BRANCH_BETWEEN_DTORS	entry    (type (NID_T),
					  type (CXX_VID_T), 
				          type (CXX_VID_T))
				external (X_EMIT_BRANCH_BETWEEN_DTORS),

CURRENT_DESTRUCTORS		entry    ()
				returns  (type (CXX_VID_T))
				external (X_CURRENT_DESTRUCTORS),

COUNT_DOWN_DESTRUCTORS		entry    (type (CXX_VID_T))
				external (X_COUNT_DOWN_DESTRUCTORS),

RELEASE_FUNCTION_DTORS		entry    ()
				external (X_RELEASE_FUNCTION_DTORS),

POP_DESTRUCTORS			entry    (type (CXX_VID_T))
				external (X_POP_DESTRUCTORS),

COMMON_DESTRUCTORS		entry    (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_COMMON_DESTRUCTORS),

MAKE_FIRST_TIME_FLAG		entry    (type (NID_T),
					  type (BOOL_T))
				returns  (type (NID_T))
				external (X_MAKE_FIRST_TIME_FLAG);

