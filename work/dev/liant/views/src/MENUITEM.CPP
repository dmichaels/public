// $Header:   Q:/views/win/vcs/menuitem.cpv   1.10   Mar 04 1997 17:55:24   gregm  $ 

//  menuitem.cpp
//
//  VMenuItem implementation
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1995-1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  04/10/93 jmd    preened, added prompt
//  04/14/93 jmd    wKey is now initalized to K_NULL
//  04/15/93 jmd    removed setAccel() in addClientMethod()
//  06/25/93 pat    added support for VArchiver.
//  07/20/93 jmd    removed free
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//  07/27/93 pat    added client and method indices;
//                  updated getFrom() and putTo() to use client list
//                  updated VClass::findCallback() calls
//  07/29/93 pat    added value member to archive data
//  08/02/93 jmd    initialized value (merge with menuId !!!)
//  08/06/93 pat    added check for GUI object in setLabel()
//  08/06/93 jmd    added some consts, preened
//  08/07/93 jmd    added copy constructor
//  08/08/93 jmd    added assignment operator
//  08/08/93 jmd    removed value (use idNo)
//  08/21/93 jmd    made checked set flag when there is no menu
//  09/02/93 pat    added cllbckls.h; updated findCallback() calls
//  09/02/93 dgm    M8 changes; minor.
//  09/22/93 pat    added archive tag
//  10/09/93 jmd    preened
//  10/11/93 jmd    added enable() and check calls to getFrom
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    added ClientCallbackMismatch check
//  02/09/94 pat    updated defineArchiveTag use
//  02/26/94 jmd    added tests for menuId
//  08/17/94 dgm    Split up setClientMethod() into two parts;
//                  added setAccel().
//  09/28/94 jld    made setClientMethod() and setAccel() behave
//                  better with accelerator keys
//  10/28/94 dgm    Fixed setAccel() calls.
//  11/11/94 dss    Archiver tags macros.
//  10/24/95 evc    added name and help id.  Note what used to be 'name'
//                  is now 'label'; name is an id to find a menuitem, label
//                  is what you see in the pulldown.
//  01/10/96 evc    Added  flag 'let_ole_delete' to defer deletion of ole
//                  provided menuitems and popups to compound doc 
//                  support.(windows only)
//	09/14/96 dgm	Fixed erroneous resource reference (BoundsChecker).
// ----------------------------------------------------------------------------

#include "menuitem.h"
#include "menu.h"
#include "notifier.h"
#include "window.h"
#include "set.h"

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "cllbckls.h"
#endif

defineClass(VMenuItem, VObject)
defineArchiveRevision(VMenuItem, 1)

// ---------------------------------------------------------------------------
// Construct a VMenuItem object with no client, callback, or
// accelerator key defined.
//
VMenuItem::VMenuItem()
	: helpId(0), name(VString())
{ 
    VMenuItem::init(0, 0, K_NULL, -1);
}

// ---------------------------------------------------------------------------
// Construct a VMenuItem with the given label, client 'clnt' and
// callback method 'mthd'.
//
VMenuItem::VMenuItem(unsigned idOfMenu, const VString& label,
                    VObject *clnt, method mthd, int accelKey , 
                    int helpId, const VString& name) :
    wKey(K_NULL),
    label(label),
    helpId(helpId),
    name(name)
{
    init(clnt, mthd, accelKey, idOfMenu);
}

// ---------------------------------------------------------------------------
// Construct a VMenuItem with the given label 's', client 'clnt' and 
// callback method 'mthd'.  Also, set 'accelKey' as the keystroke accelerator.
//
VMenuItem::VMenuItem(const VString& label, VObject *clnt,
                        method mthd, int accelKey, 
                        int helpId, const VString& name) :
    wKey(K_NULL),
    label(label),
    helpId(helpId),
    name(name)
{
    init(clnt, mthd, accelKey);
}

// ---------------------------------------------------------------------------
// Copy constructor
//
VMenuItem::VMenuItem(const VMenuItem& mi)
{
	appendOrInsertMenuDone = 0;

    label = mi.label;
    prompt = mi.prompt;

    menu = 0;
    menuId = mi.menuId;
    idNo = mi.idNo;
    bEnabled = mi.bEnabled;
    bChecked = mi.bChecked;

    client = mi.client;
    selector = mi.selector;
    wKey = mi.wKey;

    helpId = mi.helpId;
    name = mi.name;

#ifndef CV_NOARCHIVER
    methodIndex = mi.methodIndex;
    clientIndex = mi.clientIndex;
#endif
}

// ---------------------------------------------------------------------------
//
VMenuItem::~VMenuItem() 
{ 
    if (wKey != K_NULL) {
        notifier->unsetAccel(wKey);
    }
    if (menu) {
        menu->removeItem(this);
    }
}

// ---------------------------------------------------------------------------
void VMenuItem::init(VObject *clnt, method mthd, 
                        int accelKey, int mid)
{
	appendOrInsertMenuDone = 0;
    menu = 0;
    menuId = mid;
    idNo = mid;
    bEnabled = (menuId >= 0) ? TRUE : FALSE;
    bChecked = FALSE;
    setClientMethod(clnt, mthd, accelKey);
    let_ole_delete = FALSE;

#ifndef CV_NOARCHIVER
    methodIndex = -1L;
    clientIndex = -1L;
#endif
}

// ---------------------------------------------------------------------------
VMenuItem& VMenuItem::operator= (const VMenuItem& mi)
{
    label = mi.label;
    name = mi.name;
    prompt = mi.prompt;


    menu = 0;
    menuId = mi.menuId;

    helpId = mi.helpId;


    idNo = mi.idNo;
    bEnabled = mi.bEnabled;
    bChecked = mi.bChecked;

    client = mi.client;
    selector = mi.selector;
    wKey = mi.wKey;

#ifndef CV_NOARCHIVER
    methodIndex = mi.methodIndex;
    clientIndex = mi.clientIndex;
#endif

    return *this;
}

// ---------------------------------------------------------------------------
// Enable or disable this VMenuItem if 'b' is TRUE or FALSE, respectively.
//
boolean VMenuItem::enable(boolean b)
{
    bEnabled = b;
    if (menu == 0 || menu->hMenu == 0) {
        return FALSE;
    }

    // jmd 2/26/94
    if ((menuId >= 0) && appendOrInsertMenuDone) {
        EnableMenuItem(menu->hMenu, (UINT)(HMENU)menuId, 
                    MF_BYCOMMAND | (b ? MF_ENABLED: MF_GRAYED));
    }

    return TRUE;
}

// ---------------------------------------------------------------------------
// Return TRUE if this VMenuItem is checked, otherwise return FALSE.
//
boolean VMenuItem::checked()
{
    return bChecked;
}

// ---------------------------------------------------------------------------
// Return TRUE if this VMenuItem is enabled, otherwise return FALSE.
//
boolean VMenuItem::enabled()
{
    return bEnabled;
}

// ---------------------------------------------------------------------------
// Called from the VNotifier when this VMenuItem has been selected. 
// This method responds by performing the callback message on the client object.
//
boolean VMenuItem::selected()
{
    if (enabled() && client && selector != NIL_METHOD) {
        client->perform(selector, (long)this);
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
// Set the client object, the callback method, 
// and the accelerator keycode of this VMenuItem to 'clnt', 'mthd' and
// 'accelKey', respectively.
//  
void VMenuItem::setClientMethod(VObject *clnt, method mthd, int accelKey)
{
    selector    = mthd;
    client      = clnt;

    if( K_NULL != (wKey = accelKey) ) {
        notifier->setAccel( wKey, methodOf(VMenuItem, selected), this );
    }
    if ((selector == NIL_METHOD) && menu) {
        enable(FALSE);
    }
}

// ---------------------------------------------------------------------------
//  
void VMenuItem::setClientMethod(VObject *clnt, method mthd)
{
    client = clnt;
    selector = mthd;
    if( wKey != K_NULL ) {
        notifier->setAccel( wKey, methodOf(VMenuItem, selected), this );
    }
    if ((selector == 0) && (menu != 0)) {
        enable(FALSE);
    }
}

// ---------------------------------------------------------------------------
//
void VMenuItem::setAccel(int key)
{
    if ( K_NULL != (wKey = key) ) {
        notifier->setAccel( wKey, methodOf(VMenuItem, selected), this );
    }
}

// ---------------------------------------------------------------------------
//
int VMenuItem::unsetAccel()
{
    int tempKey = K_NULL;
    if (wKey != K_NULL) {
        notifier->unsetAccel(tempKey = wKey);
        wKey = K_NULL;
    }
    return tempKey;
}

// ---------------------------------------------------------------------------
// Set the label of this VMenuItem to the string 's'.
//
void VMenuItem::setLabel(const VString& s)
{
    label.scanTilde(s.gets());
    if ((menu != 0) && (menu->hMenu != 0) && appendOrInsertMenuDone) {
        ModifyMenu((HMENU)menu->hMenu, (UINT)(HMENU)menuId, 
                    MF_BYCOMMAND, (UINT)(HMENU)menuId, (LPCSTR)(label.gets()));
    }
}

// ---------------------------------------------------------------------------
// Place or Remove a check mark next to this VMenuItem if
// 'b' is TRUE or FALSE, respectively.
//
boolean VMenuItem::check(boolean b)
{
    bChecked = b;

    if (menu == 0 || menu->hMenu == 0) {
        return FALSE;
    }

    // jmd 2/26/94
    if ((menuId >= 0) && appendOrInsertMenuDone) {
        CheckMenuItem(menu->hMenu, (UINT)(HMENU)menuId, 
                MF_BYCOMMAND | (b ? MF_CHECKED : MF_UNCHECKED));
    }

    return TRUE;
}

// ---------------------------------------------------------------------------
// Return the label of this VMenuItem.
//
VString& VMenuItem::getLabel() 
{ 
    if ((menu != 0) && appendOrInsertMenuDone) {
        char b[MAXmenuLabel];
        GetMenuString(menu->hMenu, (UINT)(HMENU)menuId, 
                                    (LPSTR)b, MAXmenuLabel, MF_BYCOMMAND);
        label = b;
    }

    return label; 
}

// ---------------------------------------------------------------------------
// Set the client of this VMenuItem to 'client'.
//
boolean VMenuItem::setClient(VObject *client)
{
    this->client = client;
    notifier->setAccel( wKey, methodOf(VMenuItem, selected), this );
    return TRUE;
}

// ---------------------------------------------------------------------------


// ===========================================================================
#ifndef CV_NOARCHIVER   

// --------------------------------------------------------------------------
//  Put this object to the archive.  This method is called from 
//  VArchiver::putObject().
//
void VMenuItem::putTo(VArchiver& a)
{
    VMenuItem::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//  Put this object data of this object to the archive.
//
//  If 'most_derived' is FALSE, then this method has been called from a 
//  derived class.
//
//  If 'most_derived' is TRUE, then this method is being called directly from a 
//  VArchiver.
//
void VMenuItem::putTo(VArchiver& a, boolean most_derived)
{
    a << VMenuItemArchiveTag;

    a.putObject(&label);            //  VString label of menu item
    a.putObject(&prompt);       //  VString prompt for menu item
    a << wKey;                  //  associated accelerator key 
    a << bEnabled;              //  enabled state
    a << bChecked;              //  checked state
    a << idNo;                  //  data idNo

    //  name of client  
    a.putObject(notifier->getClientName(client, clientIndex), TRUE);
    
    //  class name of selector method and name of method

    const char *class_name  = 0;            
    const char *method_name = 0;            
    VCallbackList::findCallback(selector, class_name, method_name, methodIndex);
    a << (char *) class_name;           //  method's class name
    a << (char *) method_name;          //  method name

    a.putObject(&name);         //  VString name of menu
    a << helpId;                    //  helpId
}

// --------------------------------------------------------------------------
//  Get this object from the archive by retrieving the object data.
//
//  This method is invoked by the getFrom() of this most-derived class or by
//  the getFrom() of a derived class.
//
void VMenuItem::getFrom(VArchiver& a)
{
    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VMenuItemArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VMenuItemArchiveTag));
        return;
    }

    VString localLabel;
    (void) a.getObject(&localLabel);//  VString label of menu item
    label.scanTilde(localLabel.gets());
    (void) a.getObject(&prompt);    //  VString prompt for menu item
    a >> wKey;                      //  associated accelerator key 
    a >> bEnabled;                  //  enabled state
    a >> bChecked;                  //  checked state
    a >> idNo;                      //  data idNo

    //  name of client  

    VString client_name;
    (void) a.getObject(&client_name);
    client = notifier->getClient(client_name, &clientIndex);
    
    //  class name of selector method and name of method

    char    method_name[256];
    char   *method_name_ptr = method_name;
    char    class_name[256];
    char   *class_name_ptr  = class_name;
 
    a >> class_name_ptr;
    a >> method_name_ptr;

    if (getArchiveTagRev(tag) > 0) {
        a.getObject(&name);         //  VString name of menu
        a >> helpId;                    //  helpId
    }
    selector = VCallbackList::findCallback(class_name_ptr, method_name_ptr, 
                                                                &methodIndex);

    if (client != 0 && selector != 0) {
        if (!client->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }
}

// --------------------------------------------------------------------------
//  Get this object from the archive.  The 'data' parameter is a pointer
//  to the VMenu object to which this belongs.  The 'do_realize' parameter
//  is irrelevant, since VMenuItems have no underlying GUI component.
//
//  This method is invoked by VMenu::getObject() or VArchiver::getObject().
//
void VMenuItem::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VMenuItem::getFrom(a);              
    menu = (VMenu *) data;
    if (data != 0) {
        enable(bEnabled);
        check(bChecked);
    }
}

#endif // ARCHIVER  
