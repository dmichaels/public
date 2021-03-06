/* C++ C_BE_SIZE_PKG - Interface to BE_SIZE routine  */

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
 *  01.17.90  TLF  000  Original.
 *
 ***********************************************************************/


/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include C_STANDARD_DEFS_IN;     */
/* %include C_EXTERNAL_NAME_MAP_IN; */


declare

SIZE_FROM_TYPE        		entry ( type(SHORT_T), type(SHORT_T),
				        type(LONG_T), type(SHORT_T) )
				external ( X_SIZE_FROM_TYPE ),

BACK_END_SIZE			entry ( type(SHORT_T), type(SHORT_T),
				        type(SHORT_T) )
				external ( X_BACK_END_SIZE );
