/* C++ C_PUTDBG_PKG - Interface to PUTDBG routine  */

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
 *  01.23.90  TLF  000  Original.
 *
 ***********************************************************************/


/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include C_STANDARD_DEFS_IN;     */
/* %include C_EXTERNAL_NAME_MAP_IN; */


declare

PUTDBG                		entry ( char(*) varying )
				external ( X_PUTDBG );

