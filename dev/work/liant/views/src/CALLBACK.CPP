// $Header:   Q:/views/common/vcs/CALLBACK.CPV   1.13   Mar 24 1997 16:08:20   scottt  $ 

//	callback.cpp
//
//	VCallback implementation [Common]
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
//	08/25/93 pmcm	original.
//	09/01/93 dgm	initialization additions in non-default constructor.
//	09/02/93 dgm	(pmcm) added extern VCallbackList *callbackList;.
//	09/10/93 dgm	(pmcm) added one to "len" for null byte in setName().
//	01/13/94 dgm	merged and preened
//	02/25/94 pmcm	added CV_NEARDATA attribute to VInitDLL object
//	02/24/94 dgm	old C preprocessor changes
//	09/04/94 pat	removed share flag on memory alloc under CV_WIN32
//	08/17/94 dgm	Added paranoia to destructor.
//	05/19/95 pkt	selected Windows as common source.
//	07/31/95 evc	Got rid of references to memhndl and initDLL for WIN32
//	09/06/95 pkt	fixed ifdef on declaration of initDLL.
//  11/30/95 evc    Removed special case code for allocation of callback names
//					because memory manager now allocates from a single pool.
//	06/12/96 pkt	updated HANDLE -> VHANDLE.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
//---------------------------------------------------------------------------

#include "callback.h"
#include "cllbckls.h"	/* for VCallbackList::addCallback() */
#include "memry.h"

extern "C" {
#include "string.h"		/* for strlen() and strcpy() */
}


extern VCallbackList *callbackList;	// pointer to global table for DLL use.

// ---------------------------------------------------------------------------
//
VCallback::VCallback()
		:	callbackClassName(0),
			callbackMemberName(0),
			callbackMethod(0),
			callbackIndex(-1L),
			callbackNext(0),
			callbackReg(CallbackNone)
			
{
 	;
}

// ---------------------------------------------------------------------------
//
VCallback::VCallback(	const char *className, 
						const char *memberName,
						method 		m,
						integer		index,
						AutoRegister reg)
		:	callbackClassName(0),
			callbackMemberName(0),
			callbackMethod(0),
			callbackIndex(-1L),
			callbackNext(0),
			callbackReg(CallbackNone)
{
	set(className, memberName, m, index);
	callbackReg = reg;
	setNext(0);
	
	switch (reg) {
	default:
	case CallbackNone:
		break;

	case Real:
		(*callbackList)->addCallback(this);
		break;
		
	case Ghost:
		(*callbackList)->addGhostCallback(this);
		break;

	case Import:
		{
			//
			// Need this extra block to work-around ObjectCenter
			// C++ compiler bug; it won't hurt in general.
			//
			method importMethod = (*callbackList)->exportCallback(className, 
															   	memberName);
			if (importMethod != 0) {
			 	setMethod(importMethod);
				(*callbackList)->addCallback(this);
			}
			else {
			 	callbackReg = CallbackNone;
			}
		}
		break;
	}
}

// ---------------------------------------------------------------------------
//
VCallback::~VCallback()
{
	if (callbackList != 0) {

		switch (callbackReg) {

		case Ghost:
			(*callbackList)->removeGhostCallback(this);
			break;

		case Real:	//	fall through
		case Import:
			(*callbackList)->removeCallback(this);
			break;

		case CallbackNone:	//	fall through
		default:
			break;
		}
	}

	if (callbackClassName != 0) {
	 	delete [] callbackClassName;
		callbackClassName = 0;
	}

	if (callbackMemberName != 0) {
	 	delete [] callbackMemberName;
	 	callbackMemberName = 0;
	}
}

// ---------------------------------------------------------------------------
boolean VCallback::set(	const char *className, 
						const char *memberName,
						method m,						
						integer index)
{
	if (className == 0 || memberName == 0 || callbackReg != CallbackNone) {
		return FALSE;
	}

	setName(callbackClassName, hClassName, className);
	setName(callbackMemberName, hMemberName, memberName);
	setMethod(m);
	setIndex(index);
	return TRUE;
}

// ---------------------------------------------------------------------------
//
void VCallback::setName(pointer& p, VHANDLE& h, const char *name)
{
	if (name == 0) {
		return;
	}

	int len = strlen((char *)name) + 1;

	h = 0;

	p = new char[len];

	if (p != 0) {
		strcpy(p, name);
	}
}
