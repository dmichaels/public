/* C++ C_MANGLED_NAME_PKG - Symbol table name (de)mangler interface */

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
 *  03.02.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include GLOBAL_IN;              */
/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */


declare

MANGLED_SYMBOL_NAME		entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_MANGLED_SYMBOL_NAME);

