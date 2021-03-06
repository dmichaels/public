/* C++ fixdtl.dcl - Utility to fixup the destructor list for the
/*                  global cleanup procedure
/**/

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 * 
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  11.15.90  TLF  000  Original.
 *
 ************************************************************************/

declare

FIXUP_DESTRUCTOR_LIST		entry (type (NID_T))
				external (X_FIXUP_DESTRUCTOR_LIST);
