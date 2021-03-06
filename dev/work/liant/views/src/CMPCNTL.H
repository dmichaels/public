// $Header:   Q:/views/win/vcs/cmpcntl.h_v   1.10   Jun 03 1997 09:51:36   scottt  $

//  cmpprog.h
//
//  VComponentControl class interface
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  01/29/96 dss    Initial Revision.
//  05/07/96 evc    Made VComponentItem a friend
//  06/07/96 evc    Added new constructor
//
// ----------------------------------------------------------------------------

#ifndef VCOMPONENTCONTROL_H
#define VCOMPONENTCONTROL_H

#include "cmpitem.h"
#include "cmpprog.h"


#include "iterator.h"
#include "ordcllct.h"

class VComponentControl;
class VOleControlEvents;


CLASS VComponentCallback :public VObject
{
public:
    VComponentCallback();
    VComponentCallback(VString name, DISPID dispId, VObject* client, method callback);
    VClass* iam();

    VString  m_eventName;
    VObject* m_client;
    method   m_callmeth;
    DISPID   m_dispid;


};

CLASS VComponentControl : public VComponentItem
{
public:

                            VComponentControl();

    // construct an empty VComponentControl from an Object type.
                            VComponentControl(const VString& objectType, 
							  VComponentWindow& CDParent, // unused
							  VWindow& win);

							
                            VComponentControl(const VString& objectType, 
                                              VWindow& win);


                            VComponentControl(const VString& objectType, 
                                              const VFrame& frame,
                                              VComponentWindow& CDParent, 
                                              const VStyle& style = StyleDefault);

    // construct a VComponentItem from storage.
                            VComponentControl(const VPathString& storage,
                                              VComponentWindow& CDParent,
                                              VWindow& win, 
                                              int itemType = EmbeddedItem);
                            VComponentControl(const VPathString& storage, 
                                              const VFrame& frame,
                                              VComponentWindow& CDParent, 
                                              const VStyle& style = StyleDefault,
                                              int itemType = EmbeddedItem);

                            ~VComponentControl();
                            virtual VClass* iam();
                            void            uponEvent(VObject *clnt, method mthd); 
                            VObject        *client;         // client object for callback method
                            method          eventMthd;      // Callback used when a button is clicked on


    

    // Return a collection of Event names as new VStrings
    boolean onEvent(VString eventName, VObject* client, method mth);
    VOrdCollect getEvents();

    // expose methods of programmable
    boolean         setProperty(VString propertyName, VVariable value);
    boolean         getProperty(VString propertyName, VVariable value);

	// Set a property in the extended case (more than one property argument)
	boolean			setProperty(VMethodData& methodData);
	// Get a property in the extended case (more than one property argument)
	boolean			getProperty(VMethodData& methodData);

    // This method invokes the method described by methodData
    boolean         invokeMethod(VMethodData& methodData);

    // Return a text message describing error that may of occured
    VString         getErrorMsg();

    // Return the current status of object (i.e. status of last operation)
    VStatus         getStatus();


// methods
protected:
    VComponentProgrammable  *getProgrammable(void);
    void                    setProgrammable(const VComponentProgrammable&);

// data
protected:
    VComponentProgrammable  *m_programmable;
public:
    VOrdCollect m_eventList;
// methods
private:
    void                    init(void);

friend VOleControlEvents;
friend VComponentItem;

};

extern VClass *VComponentControlCls;

#endif  // VCOMPONENTCONTROL_H
