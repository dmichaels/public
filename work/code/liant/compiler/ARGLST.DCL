/* C++ CXX_CHECK_ARGUMENT_LIST_PKG - Expand argument list interface */

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
 *  03.26.92  DGM  005  New value-node scheme.
 *  11.09.90  PKT  004  Added CHECK_DEFAULT_ARGUMENT.
 *  09.14.90  DGM  003  Changed include file names and VOP names.
 *  07.10.90  PKT  002  Added VICTIM parameter to CHECK_ARGUMENTS.
 *  05.31.90  PKT  001  Added CHECK_ARG_SAVE.
 *  03.28.90  PKT  000  Original.
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
 
CHECK_ARGUMENTS			entry    (type (CXX_VID_T),
					  type (NID_T), 
					  type (NID_T),
					  type (CXX_VID_T))
				returns  (type (SHORT_T))
				external (X_CHECK_ARGUMENTS),

CHECK_ARG_SAVE			entry	 (type (CXX_VID_T))
				returns	 (type (SHORT_T))
				external (X_CHECK_ARG_SAVE),

CHECK_DEFAULT_ARGUMENT		entry	 (type (NID_T))
				external (X_CHECK_DEFAULT_ARGUMENT);

