/* C++ CXX_SYMBOL_TABLE_PKG - Symbol table package interface */

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
 *  09.14.90  DGM  006  Changed include file names and VOP names.
 *  08.29.90  DGM  005  Removed all entry point declarations which were
 *			duplicated from CXX_SYMBOL_TABLE_INSTALL_PKG and
 *			CXX_SYMBOL_TABLE_LOOKUP_PKG; note that anyone who
 *			wants to include CXX_SYMBOL_TABLE_INSTALL_PKG alone
 *			(unusual) must include CXX_SYMBOL_TABLE_IN first.
 *  08.08.90  DGM  004  New type-node; added CREATE_DIMENSION.
 *  07.26.90  DGM  003  Added CREATE_SPECIAL_PARAMETERS;
 *			removed INSTALL_THIS_PARAMETER.
 *			Added LOOKUP_[CD]TOR_FLAG.
 *  07.23.90  DGM  016  Added GET/DELETE_VIRTUAL_BASE_CLASS_LIST.
 *  07.10.90  DGM  015  Added LOOKUP_VFT_POINTER, LOOKUP_THIS_PARAMETER,
 *			INSTALL_ANONYMOUS_UNION, CHECK_DUPLICATE_BASE_CLASSES.
 *  07.03.90  DGM  014  Updated.
 *  06.29.90  DGM  013  Updated.
 *  06.25.90  DGM  012  Added IS_xxx_BASE_CLASS_OF.
 *  05.25.90  DGM  011  Added INSTALL_PARAMETERS & INSTALL_THIS_PARAMETER.
 *  05.15.90  DGM  010  Updated.  Divided this up into two packages,
 *			CXX_SYMBOL_TABLE_LOOKUP_PKG (lookup.{dcl,pl1}) &
 *			CXX_SYMBOL_TABLE_INSTALL_PKG (instal.{dcl,pl1}).
 *			This file includes both (but by hand to prevent
 *			duplicate includes, need conditional compilation!).
 *  03.27.90  DGM  009  Updated.
 *  03.02.90  DGM  008  Updated.
 *  02.22.90  DGM  007  Updated.
 *  02.15.90  DGM  006  Updated.
 *  02.05.90  DGM  004  Updated.
 *  12.11.89  DGM  002  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include GLOBAL_IN;              */
/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_SYMBOL_TABLE_LOOKUP_PKG;	/* includes CXX_SYMBOL_TABLE_IN */
%include CXX_SYMBOL_TABLE_INSTALL_PKG;

