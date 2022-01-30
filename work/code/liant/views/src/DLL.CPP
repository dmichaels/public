// $Header:   Q:/views/win/vcs/DLL.CPv   1.6   Mar 04 1997 14:12:24   gregm  $ 

//
//	dll.cpp
//
//	Entry point for Allegris DLL.
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	03/05/93 pat	preened, commented
//	08/25/93 pat	added CPPV_DATA_DLL and initDLL
//	11/30/93 pat	moved initDLL extern from "C" for M80
//	02/25/94 pat	added CV_NEARDATA attribute to VInitDLL object
//	04/20/94 pat	CV_WIN32:  added DllMain()
//	08/13/94 evc	Added WEP for Borland
//	02/03/95 jld	deleted includes for windows.h to avoid MS boolean conflict
//  07/24/95 evc    Borland wants DLLmain to be called  DllEntryPoint.
//					Got rid of initDLL for 32 bit, and added thread safty
// ----------------------------------------------------------------------------

#include "classtbl.h"


//==========================================================================
//      Win32 version

#if defined(CV_WIN32)
// --------------------------------------------------------------------------
//
extern "C" {

DWORD g_dwTlsIndex = TLS_OUT_OF_INDEXES;


#ifdef __BCPLUSPLUS__
BOOL WINAPI DllEntryPoint(HINSTANCE   hinstDLL, 
					DWORD           fdwReason, 
					LPVOID          lpvReserved)
#else
BOOL WINAPI DllMain(HINSTANCE   hinstDLL, 
					DWORD           fdwReason, 
					LPVOID          lpvReserved)
#endif

{
	switch (fdwReason) {
	
	case DLL_PROCESS_ATTACH:
    {
		//
		// DLL is attaching to the address space of the current process.
		//

		break;
    }

	case DLL_THREAD_ATTACH:
	{
		//
		// A new thread is being created in the current process.
		//

		break;
	}

	case DLL_PROCESS_DETACH:
    {

         // The calling process is detaching the DLL
         // from its address space.
         break;

    }

	case DLL_THREAD_DETACH:
	{

   // A thread is exiting 

		break;
	}

	default:
		break;
	}

	return TRUE;
}

}       //       extern C

#endif  //      CV_WIN32



// ==========================================================================
//      Win16 version
// ==========================================================================

#if !defined(CV_WIN32)
extern VInitDLL CV_NEARDATA initDLL;
// --------------------------------------------------------------------------
extern "C" {
int FAR PASCAL LibMain(HINSTANCE h, WORD d, WORD sz, LPSTR p)
//
//      Dynamic-link library main routine.
//      
//      When MS Windows first loads the DLL, it calls the compiler
//      vendor's DLL startup code (LibEntry).  Among other things, the startup 
//      code initializes our local heap and our static data.  Any static objects
//      that live in the DLL (such as the notifier, the classTable, and the 
//      meta-class instances of the Views library classes) are constructed at this
//      time.  After all of this, the startup code calls LibMain (this routine).
//                                                 
//      Returns 1, if successful; 0, otherwise.
//
{
	initDLL.clear();        //      done initializing
	return(1);
}

#if defined(__BCPLUSPLUS__)
int FAR PASCAL  WEP(int n)
{

// After the WEP is called, the static destructors for objects
// declared in the DLL are called.  Should that happen, it is important 
// that we re-set the DLL initialization flag so we can delete these
// objects correctly.   BORLAND compilers will always calls a WEP even 
// if one is not explicitly linked in. This causes problems because
// the DLL initialization flag will not be properly set.  
// The solution is to always link in this explicit WEP when building
// with the BORLAND tools.

// We do not specify a WEP for the MICROSOFT compiler.
// MICROSOFT expects the user to specify WEP processing in a 
// routine called _WEP that is called by its runtime WEP.  This runtime
// WEP WILL NOT be linked in, (and therefore cannot call _WEP)
// unless you included it in the DEF file.  
// Microsoft documemtation recommends that the DEF file's SEGMENT section
// should include the following line: 
// 						SEGMENTS 'WEP_TEXT' FIXED PRELOAD

// The WEP must be EXPORTed and forced into the resident name
// table: i.e. 
// 						WEP @1 RESIDENTNAME.
// (Note that the ordinal doesnot have to be 1.)
//
// This means a WEP is typically not called for Microsoft DLLs and
// neither are the static destructors.  This is OK because the memory
// resources will be deleted by Windows during its cleanup.  If
// you need to force WEP processing, you should set the
// the DLL initialization flag in _WEP.  

	// re-set the DLL initialization flag so we can destruct global objects
	// declared in the DLL correctly;
	
	initDLL.set(); 
    //OutputDebugString("In the BORLAND _WEP\n");

	if (n == WEP_SYSTEM_EXIT) {
		return(1);
	}
	else if(n == WEP_FREE_DLL) {
		return(1);
	}

	return(0);
}
#endif   

#if  defined( _MSC_VER )
int FAR PASCAL  _WEP(int n)
{
	// You must EXPORT the WEP in the def file to
	// ensure that this routine will be called.

    initDLL.set();  

    // OutputDebugString("In the _WEP\n");
	if (n == WEP_SYSTEM_EXIT) {
		return(1);
	}
	else if(n == WEP_FREE_DLL) {
		return(1);
	}

	return(0);
}
#endif


}       //       extern C

#endif  //      !CV_WIN32

