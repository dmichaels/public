/* C++ CXX_PROCESS_CONSTANT_PKG - Interface to PROCESS_CONSTANT routine  */

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
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  01.18.90  TLF  000  Original.
 *
 ***********************************************************************/


/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_EXTERNAL_NAME_MAP_IN; */


declare

	PROCESS_CONSTANT       	entry   (character (*) varying, 
                                fixed binary (31), 
                                fixed binary (15), 
                                fixed binary (15))
			        external (X_PROCESS_CONSTANT);


