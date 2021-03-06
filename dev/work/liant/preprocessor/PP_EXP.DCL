/* CPP CPP_EXPRESSION_PKG - Expression handler interface */

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
 *  06.18.92  DGM  002  Added PP_EVALUATE_EXPRESION.
 *  09.25.90  DGM  001  Include file name changes.
 *  06.13.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CPP_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

PP_EXPRESSION			entry    ()
				returns  (fixed binary (15))
				external (X_PP_EXPRESSION),

PP_EVALUATE_EXPRESSION		entry    (fixed binary (31), pointer)
				returns  (bit (1) aligned)
				external (X_PP_EVALUATE_EXPRESSION);


