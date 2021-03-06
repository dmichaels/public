/* CPP CPP_DUMP_MACRO_PKG - Dump preprocessor macros package interface */

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
 *  06.17.91  DGM  002  Changed DUMP_PP_HEADER.
 *  09.25.90  DGM  001  Include file name changes.
 *  06.11.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* None */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CPP_DUMP_MACRO_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

DUMP_PP_MACROS		entry
			external (X_DUMP_PP_MACROS),

DUMP_PP_HEADER		entry    (character (*) varying)
			external (X_DUMP_PP_HEADER);

