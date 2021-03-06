/* C++ C_GLOBAL_UTILITY_PKG - Declare compiler utilities */

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
 *  12.12.89  DGM  001  Original; cloned & modified from ANSI-C.
 *
 ***********************************************************************/

/* Only the following Utilities are used by the C++ Front-End */

%include UTL_EXT_NAMES_IN;

%include XSTAT_DCL;
%include XABORT_DCL;
%include ADD_XREF_DCL;
%include ROUNDU_DCL;
%include TRIM_DCL;

/*
/* %include CREATS_DCL;
/* %include FINDR_DCL;
/* %include FINDW_DCL;
/* %include FNDTOK_DCL;
/* %include STORE_TOKEN_DCL;
/**/
