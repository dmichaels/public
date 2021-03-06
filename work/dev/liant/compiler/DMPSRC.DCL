/* C++ CXX_DUMP_SOURCE_MAP_PKG - Source map dumping routines */

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
 *  02.05.92  DGM  001  Added DUMP_MAKE_DEPEND.
 *  03.08.91  DGM  000  Original.
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

DUMP_SOURCE_MAP			entry    ()
				external (X_DUMP_SOURCE_MAP),

DUMP_MAKE_DEPEND		entry    ()
				external (X_DUMP_MAKE_DEPEND);

