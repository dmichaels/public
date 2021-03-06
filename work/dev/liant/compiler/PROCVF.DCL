/* C++ CXX_PROCESS_VIRTUAL_FUNCTION_PKG - Virtual function processing */

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
 *  11.06.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

PROCESS_VIRTUAL_FUNCTION	entry    (type (NID_T),
					  type (POINTER_T),
					  type (POINTER_T))
				external (X_PROCESS_VIRTUAL_FUNCTION),

INHERITE_VIRTUAL_FUNCTIONS	entry    (type (POINTER_T))
				external (X_INHERITE_VIRTUAL_FUNCTIONS),

OVERRIDING_VIRTUAL_FUNCTION	entry    (type (POINTER_T),
					  type (POINTER_T))
				returns  (type (NID_T))
				external (X_OVERRIDING_VIRTUAL_FUNCTION);

