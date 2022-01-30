/* C++ CXX_OPERATOR_NAME_PKG - Interface to GET_VOP_NAME routine  */

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
 *  02.21.91  DGM  002  Updated.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  02.12.90  TLF  000  Original.
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

GET_VOP_NAME      		entry    (type (SHORT_T))
				returns  (character (10) varying)
				external (X_GET_VOP_NAME);

