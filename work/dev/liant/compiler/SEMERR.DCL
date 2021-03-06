/* C++ C_SEMERR_PKG - Interface to SEMERR routine  */

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
 *  02.05.90  TLF  000  Original.
 *
 ***********************************************************************/


/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include C_STANDARD_DEFS_IN;     */
/* %include C_EXTERNAL_NAME_MAP_IN; */


declare

SEMERR      		entry ( type(SHORT_T), type(SHORT_T),
                                char(LINE_SIZE) varying )
			external ( X_SEMERR );

