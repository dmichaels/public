/* C++ CXX_COUNT_DOWN_VALUE_PKG - Interface to COUNT_DOWN_VALUE routine  */

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
 *  03.26.92  DGM  004  New value-node scheme.
 *  10.16.90  PKT  003  COUNT no longer goes to 0 for active value nodes.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  07.05.90  PKT  001  Added COUNT_DOWN_VALUE_0.
 *  01.18.90  TLF  000  Original.
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

COUNT_DOWN_VALUE      		entry    (type (CXX_VID_T))
				external (X_COUNT_DOWN_VALUE);

