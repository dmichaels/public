/* C++ CXX_C_HEADER_MODE_PKG - C header mode set/unset utilities */

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
 *  03.20.91  DGM  000  Original.
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

SET_C_HEADER_MODE	entry
			external (X_SET_C_HEADER_MODE),
UNSET_C_HEADER_MODE	entry
			external (X_UNSET_C_HEADER_MODE),
IN_C_HEADER_MODE	entry
			returns  (type (BOOL_T))
			external (X_IN_C_HEADER_MODE);

