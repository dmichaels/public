// $Header:   Q:/views/common/vcs/GLOBAL.H_V   1.14   Jun 16 1997 13:52:52   scottt  $ 

//	global.h
//
//	VGlobal class interface [Common]
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
//	11/08/92 jmd	preened
//	03/04/93 pmcm	preened
//	04/09/93 pmcm	changed id to VObject *
//	07/31/93 jmd	made count, indexOf, hash, etc. const
//	01/13/94 dgm	merged and preened
//	04/20/94 pmcm	CV_WIN32:  excluded delGlobs()
//	11/07/94 dss	STRICT: HGLOBAL.
//	05/19/95 pkt	merged for common source.
//	05/29/95 atb	fixed typo for "#define CV_DLL_MODIFIER"
//  01/19/85 evc	Removed unused VGlobal constructor.
//	06/12/96 pkt	added support for port of library to ISAPI. 
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
//	05/02/97 pkt	added CV_NSAPI support.
// ---------------------------------------------------------------------------

#ifndef VGLOBAL_H
#define VGLOBAL_H

// ---------------------------------------------------------------------------
//
#include "object.h"

extern VClass *VGlobalCls;

// ---------------------------------------------------------------------------
//	VGlobal is an abstract class designed to provide a mechanism 
//	for supporting the creation and manipulation of global objects 
//	within an MS Windows or OS/2 DLL.  A global object has application 
//	scope and is an instance of a class derived from the VGlobal class.
//
//	The Allegris DLL maintains a table of global objects and the application
//	with which each global object is associated.  When an application starts,
//	the global objects are constructed.  During	construction, they add 
//	themselves to the table.  They are able to do this because of the mechanisms
//	that they inherit from the VGlobal class. 
//
//	Three global objects are defined in Allegris  'mem' is the global
//	VMemory object that performs memory management.  It is defined in
//	memry.cpp.  'notifier' is the global VNotifier object that provides the
//	controller layer in the MVC paradigm and is defined	in globals.cpp.  
//	'classTable' is the global VClassTable which keeps track of meta-class
//	information for each application. 
//
//	How you refer to these objects depends upon the context.  Classes in a 
//	DLL must refer to these objects indirectly, through a pointer.  Classes not
//	in a DLL may use a direct or indirect reference.  For example, to use 
//	VNotifier::startModal(), a DLL must use notifier->startModal(); references
//	outside the DLL may be either notifier.startModal() or 
//	notifier->startModal().  VGlobal objects overload operator->() to 
//	looked up the global object that is associated with the current task
//	in the table of globals maintained in the DLL.
//
//	When an application terminates, the VGlobal class removes the 
//	object from the DLL'a table of globals.  Each VGlobal class has an a
//	ssociated integer value which is used to identify it relative to the 
//	other VGlobal classes.  To define a subclass VGlobal,  two things are 
//	required: (1) a unique integer value must be supplied to the 
//	constructor of the the VGlobal class, and (2) the ::operator->
//	must be overloaded in the subclass to perform a lookup of the 
//	global object.  
//
//	In a non-DLL context, the VGlobal class is non-operational.
//

CLASS VGlobal : public VObject {
public:
	// Constructor/Deconstructor
				VGlobal();
#if defined(DLL) || defined(CV_THREADSAFE)
				VGlobal(unsigned);
#endif
				~VGlobal();

	VClass	   *iam();

#if defined(DLL) || defined(CV_THREADSAFE)
	// equivalence and lookup
	boolean 	isEqual(VObject *) const;
	HashKey 	hash() const;

	// Returns the object of this class
	VGlobal    *lookup(unsigned);
static 			VGlobal*  returnFirstLikeThis(unsigned);
#endif

protected:
#if defined(DLL) || defined(CV_THREADSAFE)
# if defined(CV_ISAPI) || defined(CV_NSAPI)
	DWORD		hMe;		//	application thread handle
# elif defined(PM)
	ushort 		hMe;		//	application task handle
# elif defined(CV_WIN32)
	HANDLE 		hMe;		//	application task handle
# else
	HTASK 		hMe;		//	application task handle
# endif
	short 		wCls;		//	VGlobal class type (set by derived classes)
	boolean 	isDll;
#endif
};

#if defined(DLL)
# if !defined(CV_DLL_MODIFIER)
#  define CV_DLL_MODIFIER
# endif

#endif

#endif // VGLOBAL_H
