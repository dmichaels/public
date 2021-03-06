/* C++ CXX_ARGUMENT_PROMOTION_PKG - Interface to ARGUMENT_PROMOTION routine */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  03.28.90  PKT  000  Original.
 *
 ***********************************************************************/


/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */


declare

ARGUMENT_PROMOTION		entry    (type (SHORT_T))
				returns  (type (SHORT_T))
				external (X_ARGUMENT_PROMOTION);

