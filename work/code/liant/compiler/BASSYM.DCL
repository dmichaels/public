/* C++ C_BASED_SYM_PKG - Based symbol reference utilities interface */

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
 *  03.26.92  DGM  001  New value-node scheme.
 *  10.08.90  TLF  000  Original.
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

BASED_SYM            	entry 		(type (SHORT_T))
			returns 	(type (NID_T))
			external 	(X_BASED_SYM),

REF_WITH_LOCATOR      	entry		(type (CXX_VID_T),
					 type(TYPE_INFO_T))
			returns		(type (CXX_VID_T))
			external 	(X_REF_WITH_LOCATOR),

REF_PLUS_OFFSET         entry 		(type (CXX_VID_T),
					 type (LONG_T),
					 type (SHORT_T),
					 type (TYPE_INFO_T))
			returns		(type (CXX_VID_T))
			external 	(X_REF_PLUS_OFFSET);

