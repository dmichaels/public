/* C++ CXX_CHECK_FLOW_PKG - control flow tracking interface */

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
 *  07.23.90  PKT  000  Original.
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

NOTE_BYPASSING_GOTOS		entry    (type (NID_T))
				external (X_NOTE_BYPASSING_GOTOS),

CHECK_FLOW_AT_BLOCK_EXIT	entry    ()
				external (X_CHECK_FLOW_AT_BLOCK_EXIT),

CHECK_LABEL_DEFINITION		entry    ()
				external (X_CHECK_LABEL_DEFINITION),

CHECK_GOTO			entry    ()
				external (X_CHECK_GOTO),

CHECK_CASE_DEFINITION		entry    (type (SHORT_T))
				external (X_CHECK_CASE_DEFINITION);

