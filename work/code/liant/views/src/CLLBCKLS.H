// $Header:   Q:/views/common/vcs/CLLBCKLS.H_V   1.10   Mar 24 1997 16:08:28   scottt  $ 

//	cllbckls.h
//
//	VCallbackList class interface [Common]
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
//	09/02/93 pmcm	made some methods static; preened.
//	10/18/93 dgm	UNIX changes.
//	11/03/93 pkt	moved exportCallback to cllbckls.cpp.
//	11/09/93 pkt	added extern declaration of VCallbackListCls.
//	01/13/94 dgm	merged and preened
//	01/17/94 dgm	cfront C++ changes
//	06/12/96 pkt	updated HANDLE -> VHANDLE.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VCLLBCKLS_H
#define VCLLBCKLS_H

// ---------------------------------------------------------------------------
//
#include "global.h"
#include "winclass.h"
#include "callback.h"

// ---------------------------------------------------------------------------
//
CLASS VCallback;
CLASS VWindow;
CLASS VObject;

// ---------------------------------------------------------------------------
//
#define GLOBALcallbackList 3	// VGlobal object type identifier

// ---------------------------------------------------------------------------
//
extern VClass *VCallbackListCls;

// ---------------------------------------------------------------------------
// VCallbackList, a VGlobal list of VCallbacks
//
CLASS VCallbackList : public VGlobal {

public:			
								VCallbackList();
								~VCallbackList();
	VClass 					   *iam();
	
	static const VCallback	   *getCallbackList();
	static const VCallback	   *getGhostCallbackList();

	static boolean				addGhostCallback(VCallback *callback);
	static boolean				removeGhostCallback(VCallback *callback);

	static method 				findCallback(	const char 	   *className, 
												const char 	   *memberName, 
												integer	   	   *index);

	static boolean 				findCallback(	method	 		m,
			      								const char *& 	className, 
												const char *& 	memberName,
												integer 		index);

	static method				findCallback(	const char *className,
												const char *memberName);
	
	static VObject* 			findClientForCallback(method m,
												VWindow* win);
	
	boolean						addCallback(VCallback *callback);
	boolean						removeCallback(VCallback *callback);

	method						exportCallback(	const char *className,
												const char *memberName);

	VCallbackList 	   		   *operator->();


public:
	struct CallbackList {
		VCallback  	   *real;		
		VCallback  	   *ghost;
		VHANDLE			hTask;
	};

protected:
	short 				top; 	  	//	number of entries in table
	short 				max; 	  	// 	remaining space in table 
	CallbackList  	   *table;		// 	table of callback lists
	VHANDLE				hTable;	  	// 	handle to table

	CallbackList 	   *getList(VHANDLE h);
	CallbackList 	   *getList();

private:

	void 				initTable();
	void 				freeTable();

	boolean				expandTo(int n);

	VCallback 		   *getCallbackList(VHANDLE h, boolean ghost);
	boolean 			addCallback(VCallback *callback, boolean ghost);
	boolean 			removeCallback(VCallback *callback, boolean ghost);

	const VCallback    *findCallback(	const char *className,
										const char *memberName,
										CallbackList *list,
										boolean ghost = FALSE);
	
};

// ---------------------------------------------------------------------------
//	This non-static method is for use by VCallback::VCallback() under the 
//	defineCallback() macro (see callback.h).
//
inline boolean VCallbackList::addCallback(VCallback *callback)
{
 	return addCallback(callback, FALSE);
}

// ---------------------------------------------------------------------------
//	This non-static method is for use by VCallback::~VCallback()
//
inline boolean VCallbackList::removeCallback(VCallback *callback)
{
 	return removeCallback(callback, FALSE);
}

// ---------------------------------------------------------------------------
//
#ifdef IN_DLL
extern VCallbackList *callbackList;	//	pointer to global table for DLL use.
#endif

#endif // VCLLBCKLS_H
