/* C++ RUN_TIME_CALL_PKG - Set up call to a run-time library routine. */

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
 *  03.26.92  DGM  003  New value-node scheme.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  09.05.90  PKT  001  Added RUN_TIME_SYMBOL.
 *  06.29.90  PKT  000  Original.
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

%include CXX_RUN_TIME_CALL_IN;

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

RUN_TIME_SYMBOL			entry	 (type (NID_T))
				returns	 (type (NID_T))
				external (X_RUN_TIME_SYMBOL),

RUN_TIME_CALL			entry	 (type (NID_T),
					  type (CXX_VID_T))
				returns	 (type (CXX_VID_T))
				external (X_RUN_TIME_CALL),

RUN_TIME_ARGS			entry	 (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns	 (type (CXX_VID_T))
				external (X_RUN_TIME_ARGS);

