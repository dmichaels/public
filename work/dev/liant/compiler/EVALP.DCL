/* C++ CXX_EVALUATE_POINTER_PKG - Interface to EVALUATE_POINTER routine */

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
 *  03.01.90  PKT  000  Original.
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

EVALUATE_POINTER  		entry    (type (CXX_VID_T),
					  type (NID_T), 
					  type (LONG_T),
					  type (SHORT_T))
				returns  (type (BOOL_T))
				external (X_EVALUATE_POINTER);

