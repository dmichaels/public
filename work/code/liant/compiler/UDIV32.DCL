/* C++ C_UDIV32  - Interface to UDIV32 routine  */

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
 *  01.24.90  TLF  000  Original.  Note that UDIV32 is written in C.
 *                      Therefore we are not using an external name
 *                      redefinition, or type defs in the declaration
 *                      of the parameters, because we cannot re-define
 *                      the entry name or type of parameters in the C
 *                      code.  
 *
 ***********************************************************************/


/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */



declare

UDIV32				entry (fixed binary(31), fixed binary(31))
				returns (fixed binary(31));

