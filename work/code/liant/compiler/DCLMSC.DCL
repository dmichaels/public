/* C++ C_DECLARE_MISCELLANY_PKG - Miscellaneous declaration routines */

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
 *  04.02.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include C_STANDARD_DEFS_IN;     */
/* %include C_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include Files
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

DECLARE_IMPLICIT		entry    (type (NID_T), type (SHORT_T),
					  type (SHORT_T), type (SHORT_T))
				returns  (type (NID_T))
				external (X_DECLARE_IMPLICIT),

DECLARE_IMPLICIT_FUNCTION	entry    (type (NID_T), type (SHORT_T),
					  type (SHORT_T), type (SHORT_T))
				returns  (type (NID_T))
				external (X_DECLARE_IMPLICIT_FUNCTION),

DECLARE_LABEL			entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_DECLARE_LABEL),

GET_SIMPLE_TYPE			entry    (type (SHORT_T))
				returns  (type (NID_T))
				external (X_GET_SIMPLE_TYPE),

GET_POINTER_TYPE		entry    (type (SHORT_T))
				returns  (type (NID_T))
				external (X_GET_POINTER_TYPE),


GET_MEMBER_POINTER_TYPE		entry    (type (SHORT_T))
				returns  (type (NID_T))
				external (X_GET_MEMBER_POINTER_TYPE);

