/* C++ CXX_GLOBAL_NAMES_PKG - Initialize global names */

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
 *  02.01.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;       */
/* %include CXX_EXTERNAL_NAME_MAP_IN;   */
/* %include CXX_PP_TOKEN_TYPE_CODES_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_GLOBAL_NAMES_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

INITIALIZE_GLOBAL_NAMES		entry
				external (X_INITIALIZE_GLOBAL_NAMES),

GET_OPERATOR_NAME_TOKEN_CODE	entry    (type (NID_T))
				returns  (type (SHORT_T))
				external (X_GET_OPERATOR_NAME_TOKEN_CODE);

