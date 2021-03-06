/* CodeWatch UNIX REG_DCL - Declarations for machine register handling
/* routines.
/**********************************************************************/
/* This product is the property of Liant Software Corp.               */
/* and is licensed pursuant to a written license agreement.           */
/* No portion of this product may be reproduced without the written   */
/* permission of Liant Software Corp. except pursuant to the          */
/* license agreement.                                                 */
/**********************************************************************/
/**********************************************************************
 *
 *  LPI EDIT HISTORY
 *  11.20.91  VTF  (000) Initial version.
 **********************************************************************/

declare

SET_REGISTER_VALUE   entry(char (*) varying, pointer)
                     external (X_SET_REGISTER_VALUE),

GET_REGISTER_VALUE   entry(char (*) varying, pointer)
                     external (X_GET_REGISTER_VALUE);
