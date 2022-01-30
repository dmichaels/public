/* C++ CXX_TYPE_CLASS_PKG - type class utility package */

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
 *  01.09.92  PKT  002	Added support for lvalue indication.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  06.29.90  TLF  000  Original.
 *
 ***********************************************************************/

%include CXX_TYPE_CLASS_IN;

declare

IS_TYPE_CLASS		entry (type (SHORT_T), type (SHORT_T))
			returns (type (BOOL_T))
			external (X_IS_TYPE_CLASS),

LVALUE_TYPE_CLASS	entry (type (SHORT_T), type (SHORT_T))
			returns (type (SHORT_T))
			external (X_LVALUE_TYPE_CLASS),

TYPE_HAS_RESTRICTION	entry (type (SHORT_T), type (SHORT_T))
			returns (type (BOOL_T))
			external (X_TYPE_HAS_RESTRICTION),

UNRESTRICTED_TYPE	entry (type (SHORT_T))
			returns (type (SHORT_T))
			external (X_UNRESTRICTED_TYPE);

