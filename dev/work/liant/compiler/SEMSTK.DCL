/* C++ - CXX_SEMANTIC_STACK_PKG - Semantic stack package */

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
 *  03.26.92  DGM  000  Original.
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

%include CXX_SEMANTIC_STACK_IN;

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

PUSH_CXX_VALUE_NODE		entry    (type (CXX_VID_T))
				external (X_PUSH_CXX_VALUE_NODE),

POP_CXX_VALUE_NODE		entry    ()
				returns  (type (CXX_VID_T))
				external (X_POP_CXX_VALUE_NODE),

PUSH_BOOLEAN			entry    (type (BOOL_T))
				external (X_PUSH_BOOLEAN),

POP_BOOLEAN			entry    ()
				returns  (type (BOOL_T))
				external (X_POP_BOOLEAN);

