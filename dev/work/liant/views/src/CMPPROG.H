// $Header:   Q:/views/win/vcs/CMPPROG.H_v   1.13   Mar 04 1997 14:12:00   gregm  $

//	cmpprog.h
//
//	VComponentProgrammable class interface
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
//	01/23/96 rhw	Initial Revision.
//	01/26/96 dss	cmpcprg.h changed to cmpprog.h.
//					Name change: VComponentProgrammable.
//	01/26/96 dss	corrected typo in define.
//	01/31/96 dss	added ctor that takes IDispatch.
//	02/06/96 dss	use quotes and not angle brackets.
//	05/09/96 rhw	Added event support
// ----------------------------------------------------------------------------

#ifndef VCOMPONENTPROGRAMMABLE_H
#define VCOMPONENTPROGRAMMABLE_H

#include <stdarg.h>		// This include needs to be made portable

#include "object.h"
#include "str.h"
#include "vlocale.h"

extern VClass *VComponentProgrammableCls;

#include "methdata.h"
#include "variable.h"
#include "status.h"

//-----------------------------------------------------------------------------
// This class defines technology independent interface to a programmable
// object.  Programmable objects are possible for technologies like Microsoft's
// COM, OpenDoc, and IBM's SOM.  Currently, this class only supports
// Microsoft's COM (commonly referred to as OLE).  When we determine how we are
// going to handle other object models we will make necessary changes to this
// file.  This should not affect client code since all COM specific code is
// provate to this class.
//-----------------------------------------------------------------------------
CLASS VComponentProgrammable : public VObject
{
public:
	enum InvokationType
	{
		GetProperty,
		SetProperty,
		SetPropertyReference,
		Invocation
	};

	VComponentProgrammable();
	VComponentProgrammable(IDispatch FAR *pDispatch,
				  const VLocale& locale = VLocale("en_US"));
	VComponentProgrammable(const VString& programID,
				  const VLocale& locale = VLocale("en_US"),
				  boolean postponeServerCreation = TRUE);
	// Sanjay Start
	VComponentProgrammable(const VComponentProgrammable& source); 
    VComponentProgrammable& operator=  (const VComponentProgrammable& source);
    VComponentProgrammable* operator=  (const VComponentProgrammable* source);
	// Sanjay End
	~VComponentProgrammable();

	// Set a property in the simple case (one property argument)
	boolean			setProperty(VString propertyName, VVariable value);
	// Set a property in the extended case (more than one property argument)
	boolean			setProperty(VMethodData& methodData);
	// Get a property in the simple case (one property argument)
	boolean			getProperty(VString propertyName, VVariable value);
	// Get a property in the extended case (more than one property argument)
	boolean			getProperty(VMethodData& methodData);

	// This method invokes the method described by methodData
	boolean			invokeMethod(VMethodData& methodData);

	// Return a text message describing error that may of occured
	VString			getErrorMsg();

	// Return the current status of object (i.e. status of last operation)
	VStatus			getStatus();

	// --------------------- Event Support ----------------------
	// Register callback method, mth of instance client, for event, eventName
    boolean onEvent(VString eventName, VObject* client, method mth);
    // Return a collection of VStrings representing event names.  Caller is
	// responsible for calling VOrdCollect::freeContents() to free list.
    VOrdCollect getEvents();

	// ----------- Overridden Views framework methods -----------
	// Required meta-method
	virtual VClass*	iam();
	// Framework equivalent to the assignment operator
	virtual boolean	copyFrom(VObject *pObject);
#	ifndef CV_NOARCHIVER
	virtual void	getFrom(VArchiver& a);  //Override
	virtual void	putTo(VArchiver& a);    //Override
#	endif

	// Sanjay Start
	// Made them public for prototyping
	LPDISPATCH		getIDispatch();
	boolean			setIDispatch(LPDISPATCH dispatch);
	// Sanjay End
private:
	// Give VVariable access to OLE specific routines, but no one else
	friend CLASS VVariable;

	boolean			createServer();
	
	// COM-specific methods
//	unsigned short	getMethodType(LPDISPATCH pIDispatch);
	boolean			invoke(VMethodData& methodData, INVOKEKIND invocationType);

	// Non-COM private methods
	void			initializeServer(boolean postponeServerCreation);
	void			initializeObjectSystem();
	void			uninitializeObjectSystem();

	// Persistent members
	VString			m_programID;
	VLocale			m_locale;

	// Non-persistent members
	VStatus			m_status;			// Current status of object
	boolean			m_isInitialized;	// Indicates initialization status
	CLSID			m_clsID;			// COM-specific member
	LPDISPATCH		m_pIDispatch;		// COM-specific member
	VOrdCollect		m_eventList;		// Collection of active VComponentCallback's
										// for this VComponentProgrammable
	boolean			m_isSinkConnected;	// Indicates status of sink connection
};

inline VStatus VComponentProgrammable::getStatus()
{
	return m_status;
}

#endif	// VCOMPONENTPROGRAMMABLE_H
