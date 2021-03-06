/* C++ CXX_PRAGMA_WEAK_PKG - #pragma weak handling interface */

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
 *  05.19.92  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

REGISTER_PRAGMA_WEAK_NAME	entry    (type (NID_T),
					  type (NID_T))
				external (X_REGISTER_PRAGMA_WEAK_NAME),

PROCESS_PRAGMA_WEAK_NAMES	entry    ()
				external (X_PROCESS_PRAGMA_WEAK_NAMES);

