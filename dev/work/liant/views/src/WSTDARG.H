/************************************************************************
 *
 *    Copyright (c) 1991 Microsoft Corporation.  All Rights Reserved.
 *
 *-----------------------------------------------------------------------
 *
 *      Module:  wstdarg.h
 *
 *      Author:  baw
 *
 *     Remarks:  stdarg.h for use with DLLs
 *
 *   Revisions:  
 *     01.00.000  2/23/91 baw   Initial version, initial build
 *
 ************************************************************************/

typedef char _far *wva_list ;

#define wva_start( ap, v )  (ap = (wva_list) &v + sizeof( v ))
#define wva_arg( ap, t )    (((t _far *)(ap += sizeof( t )))[-1])
#define wva_end( ap )       (ap = NULL)

/************************************************************************
 * End of File: wstdarg.h
 ************************************************************************/

