/* LPI-C++ CXX_FLOAT_UNSIGNED_CONVERT_PKG - float to/from unsigned */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  08.19.91  DGM  000	Original; from CFE c_f4u4.c.
 *
 ***********************************************************************/

declare

CFTOUL		entry    (float binary (23))
		returns  (fixed binary (31))
		external,

CDTOUL		entry    (float binary (52))
		returns  (fixed binary (31))
		external,

CULTOF		entry    (fixed binary (31))
		returns  (float binary (23))
		external,

CULTOD		entry    (fixed binary (31))
		returns  (float binary (52))
		external;

