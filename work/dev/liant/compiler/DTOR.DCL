/* C++ CXX_EMIT_DESTRUCTOR_CODE_PKG - Interface to destructor package */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  03.26.92  DGM  003  New value-node scheme.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  09.05.90  PKT  001  Added EMIT_DESTRUCTOR_FINISHUP_CODE.
 *  07.13.90  DGM  000  Original.
 *
 ***********************************************************************/

declare

EMIT_DESTRUCTOR_STARTUP_CODE	entry    (type (NID_T))
				external (X_EMIT_DESTRUCTOR_STARTUP_CODE),

EMIT_DESTRUCTOR_FINISHUP_CODE	entry    (type (NID_T))
				external (X_EMIT_DESTRUCTOR_FINISHUP_CODE);


