/* C++ CXX_INTERNAL_PARSE_PKG - Internal (by-hand) parse interface */

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
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  09.05.90  PKT  001  Fix.
 *  09.05.90  DGM  000	Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_INTERNAL_PARSE_IN;

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;           */
/* %include CXX_EXTERNAL_NAME_MAP_IN;       */
/* %include CXX_SYMBOL_TABLE_IN;            */
/* %include CXX_GLOBAL_SEMANTIC_DATA_IN;    */
/* %include CXX_GLOBAL_DECLARATION_DATA_IN; */
/* %include CXX_GLOBAL_TYPE_DATA_IN;        */
/* %include CXX_LEXER_TOKENS_PKG;           */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

BEGIN_INTERNAL_PARSE		entry    (type (POINTER_T),
					  type (NID_T))
				external (X_BEGIN_INTERNAL_PARSE),

END_INTERNAL_PARSE		entry    (type (POINTER_T))
				external (X_END_INTERNAL_PARSE);


