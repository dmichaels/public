/* CPP CPP_KEYWORD_PKG - Keyword lookup interface */

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
 *  11.12.91  DGM  003  Added %include CPP_KEYWORD_IN.
 *			Removed IS_DEFINED_PP_KEYWORD.
 *  10.10.90  DGM  002  IS_RESERVED_PP_KEYWORD & IS_DEFINED_PP_KEYWORD.
 *  09.25.90  DGM  001  Include file name changes.
 *  06.13.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CPP_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

%include CPP_KEYWORD_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

INITIALIZE_KEYWORD_TABLES	entry    ()
				external (X_INITIALIZE_KEYWORD_TABLES),

LOOKUP_KEYWORD			entry    (type (NID_T))
				returns  (type (SHORT_T))
				external (X_LOOKUP_KEYWORD),

LOOKUP_PP_KEYWORD		entry    (type (NID_T))
				returns  (type (SHORT_T))
				external (X_LOOKUP_PP_KEYWORD),

IS_RESERVED_PP_KEYWORD		entry    (type (NID_T))
				returns  (type (BOOL_T))
				external (X_IS_RESERVED_PP_KEYWORD);

