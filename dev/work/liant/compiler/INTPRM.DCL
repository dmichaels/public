/* C++ CXX_INTEGRAL_PROMOTION_PKG - Interface to INTEGER_PROMOTION routine  */

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
 *  02.12.90  TLF  000  Original.
 *
 ***********************************************************************/


/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */


declare

INTEGRAL_PROMOTION		entry ( type(SHORT_T) )
				returns ( type(SHORT_T) )
				external ( X_INTEGRAL_PROMOTION );

