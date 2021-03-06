/* C++ CXX_DUPLICATE_VALUE_PKG - duplicate expression tree interface */

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
 *  03.26.92  DGM  001  New value-node scheme.
 *  11.02.90  PKT  000  Original.
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

DUPLICATE_VALUE			entry	 (type (CXX_VID_T),
					  type (BOOL_T))
				returns	 (type (CXX_VID_T))
				external (X_DUPLICATE_VALUE);

