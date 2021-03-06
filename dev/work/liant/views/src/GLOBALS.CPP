// $Header:   Q:/views/common/vcs/GLOBALS.CPV   1.14   Mar 24 1997 16:09:18   scottt  $ 

//	globals.cpp
//
//	storage definition of global objects [Common]
//
//	Allegris Foundation 1.1.00
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	09/09/92 jmd	made the notifier object CV_NEARDATA
//	09/09/92 jmd	made WIN31 auto define MS_WINDOWS
//	11/16/92 gjl    added #ifdef UNIX section
//	10/19/93 dgm	3.0 changes.
//	03/05/93 pmcm	preened, commented
//	08/25/93 pmcm	added initDLL, a global VInitDLL object.
//	07/30/93 pkt	eliminated #ifdef macintosh.
//	09/16/93 pmcm	VInitDLL ifdef specific to IN_DLL
//	10/19/93 pkt	merged Macintosh and 3.0, removed NULL_METHOD.
//	11/17/93 pkt	reenabled definition of VNotifier::notifier.
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	merged with Macintosh.
//	01/31/94 pmcm	added initDLL for DLL client to fix VMemory cleanup.
//	02/22/94 pmcm	added CV_NEARDATA attribute to VInitDLL object
//	02/24/94 dgm	old C preprocessor changes
//	05/19/95 pkt	merged for common source.
//  07/27/95 evc    removed the initDLL static from WIN32
//					Updated comments a bit.
//	09/06/95 pkt	updated Mac discriminator to V_MACINTOSH.
//	06/12/96 pkt	added support for port of library to ISAPI.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//
#	include "defs.h"		// for define of CV_WINDOWS

// ---------------------------------------------------------------------------
//
#if defined(CV_WINDOWS) || defined(V_MACINTOSH)

#	include "notifier.h"	/* for VNotifier class */
#	include "eventmgr.h"	/* for VNotifier class */
#	include "classtbl.h"	/* for VInitDLL class */

#	if defined(DLL) || defined(CV_THREADSAFE)
#		if !defined (CV_WIN32) // initDLL is not needed for WIN32
#			ifdef IN_DLL
			//	pre-set to indicate that our DLL is initializing
				VInitDLL CV_NEARDATA initDLL(1);
#			else
				//	pre-clear to start application on the right foot
				VInitDLL CV_NEARDATA initDLL(0);
#			endif
#		endif

		//	In a DLL context, multiple VNotifier objects exist. 
		//	A notifier is constructed in the DLL when the DLL is loaded.
		//	DLL code invokes the VNotifier methods and, since these 
		//	methods are not static, requires a VNotifier object to satisfy
		//  the linker.
		//	Each application has a VNotifier object too for the same reason.  
		//  But in every notifier call, the correct notifier object is 
		//  gotten out of the "globals" table (see VGlobal) 
		//  via the overloading of  operator ->()

		// For this reason, one must ALWAYS use -> to refernce the notifier
		// object.

		// In other words, while the application and its DLL(s) might all 
		// have a notifier object, they all use the same notifier.
		// (the one in the table.)  And the one on the table gets there 
		// after a construction of the form notifier(1). 

		//	The application's notifier is built when the startup code for
		//	application executes--this is after the DLL loads and initialize.
		// So that means that the DLL puts a notifier object into the globals 
		// table, and then the client overwrites it (the rule is, no
		// task (in the 16 bit case) or thread id (in the 32 bit case)
		// can contribute more than 1 notifier to the table.)  So the app
		// overwrites the notifier belonging to the DLL, and its the
		// notifier that all reference for that task, be they in the DLL 
		// or the client use.

	 	VNotifier CV_NEARDATA notifier(boolean(1));
	 	VEventManager CV_NEARDATA eventManager(boolean(1));

#	else
		//	In a non-DLL context, only one VNotifier object exists.
		//	Either "." or "->" may be used when invoking the methods 
		//	of the VNotifier but, for portability, use "->".

		VNotifier CV_NEARDATA notifier;
		VEventManager CV_NEARDATA eventManager;

#	endif

// ---------------------------------------------------------------------------
//
#elif defined(UNIX) || defined(__VMS)

#	include "notifier.h"
#	include "memry.h"

#	ifdef MOTIF
	VNotifier notifier;
#	endif

#	ifdef SCO 			  // This may be a due to a SCO library problem.
		char Error[128];  // referenced in system header <macros.h>
#	endif

// ---------------------------------------------------------------------------
//
#elif defined(PM)
#	include "notifier.h"
#	ifdef DLL
		VNotifier notifier(1);
#	else
		VNotifier notifier;
#	endif	 // DLL
#endif // PM
