// $Header:   Q:/views/win/vcs/popupmen.cpv   1.21   Mar 04 1997 17:55:34   gregm  $ 

//  popupmenu.cpp
//
//  VPopupMenu implementation
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
//  04/10/93 jmd    preened, added prompt, setPromptAt()
//  05/12/93 jmd    fixed cast in setPromptAt
//  06/28/93 pat    added support for VArchiver;
//  06/28/93 dgm    added setLabelAt().
//  06/30/93 dgm    new VMenuData interface.
//  07/20/93 jmd    made setPrompt take const VString&, preened, removed free()
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//  07/27/93 pat    added client and method indices;
//                  updated getFrom() and putTo() to use client list
//                  updated VClass::findCallback() calls;
//                  created GUI object under getFrom().
//  08/06/93 pat    added check for GUI object in getLabel() and popupAt()
//                  !!! pat -- setLabel() should affect GUI, if present !!!
//  08/06/93 jmd    made VMenuData constructor create GUI object
//                  added const args
//  08/08/93 jmd    added idNo (and archiving)
//  09/02/93 pat    added cllbckls.h; updated findCallback() calls
//  09/13/93 dgm    M8 changes; changed "const VMenuData&" parameters in
//                  variable argument list functions to "VMenuData".
//  09/22/93 pat    added archive tag
//  10/04/93 jmd    reset notifier->popup in destructor
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    added ClientCallbackMismatch check
//  02/09/94 pat    updated defineArchiveTag use
//  03/08/94 pat    STRICT:
//                  changed unsigned to HMENU in VPopupMenu();
//                  included notifier.h before menu.h to prevent multiple
//                  windows.h inclusion.
//  08/17/94 dgm    Added enable(), disable(), and isEnabled() and archiving
//                  support this new attribute.
//  08/24/94 jld    made setAllClients() recurse submenus
//  08/24/94 jld    setClientMethod() won't change ClntMeth for VPopupMenu
//  09/08/94 jld    changed popupAt() to track last mouse button pressed
//  11/03/94 dgm    backed out new revision for saving.
//  11/11/94 dss    Archiver tags macros.
//  09/25/95 pkt    changed 'getLabel' not to return a modifiable string.
//  10/25/95 evc    Changes associated with adding names and helpIds.
//  11/08/95 dss    ~VPopupMenu(): test menu->hmenu() before natively
//                  destroying menu.
//  11/08/95 evc    Small problem in GetMenuItem
//  11/16/95 dss    Moved base class member initializers into ctor. Was
//                  causing wacky compiler errors in MS 16 bit compile.
//  12/29/95 evc    Added check(), uncheck(), isChecked() 
//  01/09/96 dgm    Non-inlined getPopupMenu().
//  01/31/96 evc    Changed setLabel so it changes the label of the
//                  realized menu (if one exists) instead simply assigning
//                  to VMenu::label.
//  03/04/96 dgm    Added sanity checks to popupAt().
//  04/08/96 dgm    Support for enabling/disabling popup-menu-items.
//	04/23/97 dgm	Fixed enableAll() to consider VPopupMenus too.
// ---------------------------------------------------------------------------

#include "notifier.h"
#include "popupmen.h"
#include "window.h"
#include "menuitem.h"
#include "set.h"
#include "iterator.h"
#include <stdarg.h>

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "cllbckls.h"
#endif

defineClass(VPopupMenu, VMenu)
//
// revision 0   original
// revision 1   added bEnable
// revision 2   names  and helpId
// revision 3   added bChecked
//
defineArchiveRevision(VPopupMenu, 3)

// ---------------------------------------------------------------------------
// Construct a VPopupMenu with no presentation menu.
//
VPopupMenu::VPopupMenu()
{ 
	appendOrInsertMenuDone = 0;
    client = 0;
    mthd = 0;
    bEnabled = TRUE;
#ifndef CV_NOARCHIVER
    clientIndex = -1L;
    methodIndex = -1L;
#endif
    menu = 0;

    setIdNo(-1),
    setName(name);
    setHelpId(helpId);
}

// ---------------------------------------------------------------------------
// Construct a VPopupMenu with label 'label' and a 'Name'
//
VPopupMenu::VPopupMenu(const VString& theLabel, 
const VString& name, int id, int helpId) :
    label(theLabel)
{
    label.scanTilde(theLabel.gets());
    client = 0;
    mthd = 0;
    bEnabled = TRUE;
#ifndef CV_NOARCHIVER
    clientIndex = -1L;
    methodIndex = -1L;
#endif
    menu = 0;

    setIdNo(id),
    setName(name);
    setHelpId(helpId);
    hMenu = CreatePopupMenu();
}

// ---------------------------------------------------------------------------
// Constructor used by VAppView::rscMenu()
//
VPopupMenu::VPopupMenu(HMENU h, const VString& theLabel, 
    const VString& name, int id, int helpId) :
    label(theLabel)
{

    client = 0;
    mthd = 0;
    bEnabled = TRUE;
    bChecked = FALSE;
	appendOrInsertMenuDone = 0;
#ifndef CV_NOARCHIVER
    clientIndex = -1L;
    methodIndex = -1L;
#endif
    menu = 0;

    setIdNo(id),
    setName(name);
    setHelpId(helpId);
    hMenu = h;
}

// ---------------------------------------------------------------------------
//
VPopupMenu::VPopupMenu(const VMenuData &menu_data)
{

    client = 0;
    mthd = 0;
    bEnabled = TRUE;
    bChecked = FALSE;
	appendOrInsertMenuDone = 0;
#ifndef CV_NOARCHIVER
    clientIndex = -1L;
    methodIndex = -1L;
#endif
    menu = 0;

    // jmd
    hMenu = CreatePopupMenu();

    if (!(menu_data.flags & VMenuData::Normal)) {
        return;
    }
    this->setLabel(menu_data.label);
    this->uponSelect(menu_data.clientObject, menu_data.clientMethod);
    this->setPrompt(menu_data.prompt);

    this->setIdNo(menu_data.identifier);
    this->setName(menu_data.name);
    this->setHelpId(menu_data.helpId);
}

// ---------------------------------------------------------------------------
// Set or clear a check mark next to the item at index 'i' within this
// VPopupMenu, if 'b' is TRUE or FALSE, respectively. 
//
void VPopupMenu::checkAt(boolean b, int i)
{
    VObject *m = (VMenuItem *)items.idAt(i);
    if (m && m->isA(VMenuItemCls)) {
        ((VMenuItem *)m)->check(b);
    }
}

// ---------------------------------------------------------------------------
// Enable or disable the item at index 'i' within this VPopupMenu
// if 'b' is TRUE or FALSE, respectively. 
//
void VPopupMenu::enableAt(boolean b, int i)
{
    VObject *object = items.idAt(i);
    if (object != 0) {
		if (object->isA(VMenuItemCls)) {
	        ((VMenuItem *)object)->enable(b);
		}
		else if (object->isA(VPopupMenuCls)) {
	        ((VPopupMenu *)object)->enable(b);
		}
    }
}

// ---------------------------------------------------------------------------
// Return TRUE if the item at index 'i' within this VPopupMenu is
// checked, otherwise return FALSE. 
//
boolean VPopupMenu::checkedAt(int i)
{
    VMenuItem *m = (VMenuItem *)items.idAt(i);
    if (m) {
        return m->checked();
    }
    return FALSE;
}

// ---------------------------------------------------------------------------
// Return TRUE if the item at index 'i' within this VPopupMenu is
// enabled, otherwise return FALSE. 
//
boolean VPopupMenu::enabledAt(int i)
{
    VMenuItem *m = (VMenuItem *)items.idAt(i);
    if (m) {
        return m->enabled();
    }
    return FALSE;
}

// --------------------------------------------------------------------------- 
//  Set the client object for all the VMenuItems of this VPopupMenu 
//  and any of its submenus to 'client'. 
//
void VPopupMenu::setAllClients(VObject *client) 
{
    DO( items, VMenuItem, m )
        if ( m->isA( VPopupMenuCls ) )
            ((VPopupMenu*) m)->setAllClients( client );
        else
            m->setClient( client );
    END
}

// ---------------------------------------------------------------------------
// Set the client and method for the VMenuItem at index 'i' in this
// VPopupMenu to 'clnt' and 'mthd', respectively. 
//
void VPopupMenu::setClientMethod(VObject *clnt, method mthd, int i)
{
    VMenuItem *menu = (VMenuItem*)  menuAt(i);

    // Make sure this isn't another PopupMenu
    if( menu->isA( VMenuItemCls ) )
        menu->setClientMethod(clnt, mthd);
}

// ---------------------------------------------------------------------------
// Create and append a list of VMenuItem objects to this VPopupMenu, 
// setting the item label for item 'i' to 'names[i]', the callback
// method to 'sels[i]'and the client to 'clnt'.  If 'sels[i]' is a NIL_METHOD, 
// then make the associated VMenuItem a separator. The last item in the 'names'
// array must be a NULL pointer.
//
void VPopupMenu::addItems(char **names, method *sels, VObject *clnt,
                            char **prompts)

{
    int        i = 0;
    VMenuItem *mi;

    for (i = 0; names[i]; i++) {
        if (sels[i] == NIL_METHOD) {
            addSeparator();
        }
        else {
            addItem(mi = new VMenuItem(names[i], clnt, sels[i]));
            if (prompts != 0 && mi != 0) {
                mi->setPrompt(VString(prompts[i]));
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Append a list of VMenuItem objects to this VPopupMenu, setting
// the item label for item 'i' to 'names[i]', the callback method to
// 'sels[i]', the accelerator key to 'keys[i]', and the client to 'clnt'. 
// If 'sels[i]' is a NIL_METHOD, then make the associated VMenuItem a
// separator.  The last item in the 'names' array must be a NULL pointer.
//
void VPopupMenu::addItems(char **names, method *sels, int *keys,
                            VObject *clnt, char **prompts)
{
    int        i = 0;
    VMenuItem *mi;

    for (i = 0; names[i]; i++) {
        if (sels[i] == NIL_METHOD) {
            addSeparator();
        }
        else {
            addItem(mi = new VMenuItem(names[i], clnt, sels[i], keys[i]));
            if (prompts != 0 && mi != 0) {
                mi->setPrompt(VString(prompts[i]));
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Enable or disable every item of this VPopupMenu if 'bEnable' is
// TRUE or FALSE, respectively. 
//
void VPopupMenu::enableAll(boolean b)
{
	DO (items, VObject, object)
		if (object->isA(VMenuItemCls)) {
			((VMenuItem *)object)->enable(b);
		}
		else if (object->isA(VPopupMenuCls)) {
			((VPopupMenu *)object)->enable(b);
		}
	END
}

// ---------------------------------------------------------------------------
// Check or uncheck every item of this VPopupMenu, if 'bEnable' is
// TRUE or FALSE, respectively. 
//
void VPopupMenu::checkAll(boolean bEnable)
{
    DO(items, VObject, anId)
    if (anId->isA(VMenuItemCls)) {
        ((VMenuItem *)anId)->check(bEnable);
    }
    END 
}

// ---------------------------------------------------------------------------
//
VPopupMenu::~VPopupMenu()
{
    // if my parent's native handle is zero, that means its been 
    // natively destroyed, which means I've been natively destroyed, 
    // also, so just zero my native handle.
    if (menu && menu->hMenu == 0) {
        hMenu = 0;
    }

    if (menu) {
        menu->removeItem(this);
    }

    // reset notifer popup  jmd
    notifier->popupMenu(0);
}

// ---------------------------------------------------------------------------
// Display (popup) this VPopupMenu at the screen location 'x', 'y'. 
// When the user makes a selection, or clicks the left mouse button anywhere on
// the screen, the VPopupMenu will disappear from the display.  The 'x', 'y'
// position must be in screen units.  Note, these coordinates can be obtained
// from a window\'s local (or client coordinates) by using the mapToScreen
// method inherited by all subclasses of VWindow.
//
void VPopupMenu::popupAt(int x, int y)
{
    VWindow *appWin = notifier->whereIsMouse();
    if (appWin != 0) {
        appWin = appWin->getTopLevelParent(); // Kerstin's  fix
        if (appWin != 0) {
            notifier->popupMenu(this);
            if (appWin != 0 && hMenu != 0) {
                TrackPopupMenu((HMENU)hMenu, notifier->buttonToTrack, 
                                x, y, 0, appWin->hwnd(), (LPRECT)0L);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Upon selection of this VPopupMenu by the user, arrange to call
// method 'm' on object 'obj' before the menu drops down.
//
void VPopupMenu::uponSelect(VObject *obj, method m)
{
    if (obj) {
        client = obj;
    }
    if (m) {
        mthd = m;
    }
}

// ---------------------------------------------------------------------------
// Called by the VNotifier when this PopupNenu is about to be
// dropped down.
//
void VPopupMenu::selected()
{
    if (client && mthd != NIL_METHOD) {
        client->perform(mthd, (long)this);
    }
}

// ---------------------------------------------------------------------------
//
boolean VPopupMenu::copyFrom(VObject *obj)
{
    if (obj->isA(VStringCls)) {
        setLabel(*(VString *)obj);
    }
    else {
        return FALSE;
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
// Set the label of this VPopupMenu to 's'.
//
void VPopupMenu::setLabel(const VString& s)
{
    label.scanTilde(s.gets());
    if (menu != 0 && menu->hmenu() != 0) {
        //  set label in GUI
        int n = (int)(menu->mitems().indexOf(this));
        ModifyMenu(menu->hmenu(), 
        n,                          // menu item to modify
        MF_BYPOSITION | MF_POPUP,   // menu item flags 
        (UINT) hmenu(),             // menu item identifier or pop-up menu handle
        s.gets());                  // menu item content )
    }

}

// ---------------------------------------------------------------------------
// Return the label of this VPopupMenu.
//
const VString& VPopupMenu::getLabel()
{ 
    if (menu != 0 && menu->hmenu() != 0) {
        //  get label stored in GUI

        char b[MAXmenuLabel];
        int n = (int)(menu->mitems().indexOf(this));
        // We look at the first item in the menu bar. If the first item
        // has no label, this is the Close Box icon and we are in the case
        // where we maximized the child window. To get the correct string, we 
        // need to ignore the Close Box icon and therefore add 1 to n.
        char firstLabel[MAXmenuLabel];
        GetMenuString(menu->hmenu(), 0, firstLabel, MAXmenuLabel, MF_BYPOSITION);
        if( firstLabel[0] == '\0') {
          n ++;
         }
        GetMenuString(menu->hmenu(), n, b, MAXmenuLabel, MF_BYPOSITION);
        label = b;
    }

    return label; 
}

// ---------------------------------------------------------------------------
// Set the prompt for the item at index 'i' within this VPopupMenu
//
void VPopupMenu::setPromptAt(const VString& p, int i)
{
    VObject *m = (VObject *) items.idAt(i);
    if (m && m->isA(VMenuItemCls)) {
        ((VMenuItem *)m)->setPrompt(p);
    }
}

// ---------------------------------------------------------------------------
// Set the label for the item at index 'i' within this VPopupMenu
//
void VPopupMenu::setLabelAt(const VString& label, int i)
{
    VObject *m = (VObject *) items.idAt(i);
    if (m && m->isA(VMenuItemCls)) {
        ((VMenuItem *)m)->setLabel(label);
    }
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::addMenuItem(const VMenuData &menu_data,
                             VObject *client_object)
{
    VMenuItem *menu_item;

    if (menu_data.flags & VMenuData::Separator) {
        this->addSeparator();
        return;
    }
    else if (menu_data.flags & VMenuData::Cascade) {
        if (menu_data.popupMenu != 0) {
            this->addItem(menu_data.popupMenu);
        }
        return;
    }
    else if (!(menu_data.flags & VMenuData::Normal)) {
        return;
    }

    menu_item = new VMenuItem((VString&)menu_data.label,
                              client_object != 0 ? client_object :
                                                   menu_data.clientObject,
                              menu_data.clientMethod,
                              menu_data.accelerator);
    menu_item->setPrompt((VString&)menu_data.prompt);
    menu_item->setIdNo(menu_data.identifier);
    menu_item->enable(boolean(!(menu_data.flags & VMenuData::Disabled)));
    menu_item->check(boolean(menu_data.flags & VMenuData::Checked));
    menu_item->setName(menu_data.name);
    this->addItem(menu_item);
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::addMenuItems(const VMenuData *menu_data,
                              VObject *client_object)
{
    const VMenuData *md;

    if ((md = menu_data) == 0) {
        return;
    }

    while (1) {
        if (md->flags & VMenuData::EndOfList) {
            break;
        }
        else if (md->flags & VMenuData::ClientObject) {
            continue;
        }
        this->addMenuItem(*md++, client_object);
    }
}

// ---------------------------------------------------------------------------
//
#if defined(_MSC_VER)
void VPopupMenu::addMenuItems(VMenuData menu_data, ...)
#else
void VPopupMenu::addMenuItems(const VMenuData& menu_data, ...)
#endif
{
    VObject *client_object = 0;
    va_list vap;

    if (menu_data.flags & VMenuData::EndOfList) {
        return;
    }
    else if (menu_data.flags & VMenuData::ClientObject) {
        client_object = menu_data.clientObject;
    }
    else {
        this->addMenuItem(menu_data);
    }

    va_start(vap, menu_data);

    while (1) {
        VMenuData &md = va_arg(vap, VMenuData);
        if (md.flags & VMenuData::EndOfList) {
            break;
        }
        else if (md.flags & VMenuData::ClientObject) {
            client_object = md.clientObject;
        }
        else {
            this->addMenuItem(md);
        }
    }

    if (client_object != 0) {
        DO (mitems(), VMenuItem, mi)
            mi->setClient(client_object);
        END
    }

    va_end(vap);
}

// ---------------------------------------------------------------------------
//
VMenuItem *VPopupMenu::getMenuItem(int menu_item_id)
{
    if (menu_item_id < 0) {
        return 0;
    }
    DO (mitems(), VMenuItem, menu_item)
        if (!menu_item->isA(VMenuItemCls))
            continue;

        if (menu_item->getIdNo() == menu_item_id) {
            return menu_item;
        }
    END
    return 0;
}
// ---------------------------------------------------------------------------
//
VMenuItem *VPopupMenu::getMenuItem(const VString & name)
{
    DO (mitems(), VMenuItem, menu_item)
        if (!menu_item->isA(VMenuItemCls))
            continue;

        if (menu_item->getName() == name) {
            return menu_item;
        }
    END
    return 0;
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::setMenuItemLabel(int menu_item_id, const VString &label)
{
    VMenuItem *menu_item = this->getMenuItem(menu_item_id);
    
    if (menu_item != 0) {
        menu_item->setLabel(label);
    }
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::setMenuItemPrompt(int menu_item_id, const VString &prompt)
{
    VMenuItem *menu_item = this->getMenuItem(menu_item_id);
    
    if (menu_item != 0) {
        menu_item->setPrompt(prompt);
    }
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::setMenuItemClient(int menu_item_id, VObject *client_object)
{
    VMenuItem *menu_item = this->getMenuItem(menu_item_id);
    
    if (menu_item != 0) {
        menu_item->setClient(client_object);
    }
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::setMenuItemCallback(int menu_item_id,
                                     method client_method,
                                     VObject *client_object)
{
    VMenuItem *menu_item = this->getMenuItem(menu_item_id);
    
    if (menu_item != 0) {
        menu_item->setClientMethod(client_object, client_method);
    }
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::enableMenuItem(int menu_item_id, boolean b)
{
    VMenuItem *menu_item = this->getMenuItem(menu_item_id);
    
    if (menu_item != 0) {
        menu_item->enable(b);
    }
    else {
        DO (mitems(), VObject, object)
            if (object->isA(VPopupMenuCls)) {
                VPopupMenu *popup_menu_item = (VPopupMenu *)object;
                if (popup_menu_item->getIdNo() == menu_item_id) {
                    popup_menu_item->enable(b);
                    break;
                }
            }
        END
    }
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::checkMenuItem(int menu_item_id, boolean b)
{
    VMenuItem *menu_item = this->getMenuItem(menu_item_id);
    
    if (menu_item != 0) {
        menu_item->check(b);
    }
    else {
        DO (mitems(), VObject, object)
            if (object->isA(VPopupMenuCls)) {
                VPopupMenu *popup_menu_item = (VPopupMenu *)object;
                if (popup_menu_item->getIdNo() == menu_item_id) {
                    popup_menu_item->check(b);
                    break;
                }
            }
        END
    }
}
// ---------------------------------------------------------------------------
//
void VPopupMenu::check(boolean b)
{
    bChecked = b;
    if ((menu != 0) && (menu->hMenu != 0) && (hMenu != 0)) {
        int index = int(menu->mitems().indexOfSame(this));
        if (index >= 0) {
            CheckMenuItem(menu->hMenu, UINT(index),
                           MF_BYPOSITION |
                           (b ?  MF_CHECKED:MF_UNCHECKED));
        }
    }
}
//---------------------------------------------------------------------------
void VPopupMenu::uncheck(boolean b){
    check(!b);
}
// ---------------------------------------------------------------------------
boolean VPopupMenu::isChecked() const
{
    return bChecked;
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::enable(boolean b)
{
    bEnabled = b;
    if ((menu != 0) && (menu->hMenu != 0) && (hMenu != 0)) {
        int index = int(menu->mitems().indexOfSame(this));
        if (index >= 0) {
            EnableMenuItem(menu->hMenu, UINT(index),
                           MF_BYPOSITION |
                           (b ? MF_ENABLED : MF_DISABLED | MF_GRAYED));
        }
    }
}

// ---------------------------------------------------------------------------
//
void VPopupMenu::disable(boolean b)
{
    enable(!b);
}

// ---------------------------------------------------------------------------
//
boolean VPopupMenu::isEnabled() const
{
    return bEnabled;
}

// ---------------------------------------------------------------------------
// a non-recursive search for a VPopupMenu within the 
// given popup by id.
//
VPopupMenu* VPopupMenu::getPopupMenu(int id) {
    if (id < 0) {
        return 0;
    }
    DO (mitems(), VPopupMenu , pop)
        if (!pop->isA(VPopupMenuCls))
            continue;

        if (pop->getIdNo() == id) {
            return pop;
        }
    END
    return 0;
}

// ---------------------------------------------------------------------------
// a non-recursive search for a VPopupMenu within the 
// given popup by logical name
//
VPopupMenu* VPopupMenu::getPopupMenu(const VString & name) {
    DO (mitems(), VPopupMenu , pop)
        if (!pop->isA(VPopupMenuCls))
            continue;

        if (pop->getName() == name) {
            return pop;
        }
    END
    return 0;
}

// ===========================================================================
#ifndef CV_NOARCHIVER   

// ---------------------------------------------------------------------------
//  Put this object to the archive.  This method is called from 
//  VArchiver::putObject().
//
void VPopupMenu::putTo(VArchiver& a)
{
    VPopupMenu::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//  Put this object to the archive by archiving the base class layer, 
//  then the object data of this class layer.
//
//  The 'most_derived' parameter is irrelevant to VMenu and VPopupMenu.
//
void VPopupMenu::putTo(VArchiver& a, boolean most_derived)
{
    VMenu::putTo(a, FALSE);         //  base object data

    a << VPopupMenuArchiveTag;

    a.putObject(&label);                //  title (or label) of this popup
    a.putObject(&prompt);           //  prompt
    a << helpId;

    // idNo is now archived in VMenu

    a << boolean(bEnabled);
    a << boolean(bChecked);

    //  name of client
    a.putObject(notifier->getClientName(client, clientIndex), TRUE);

    //  name and class of callback method

    const char *class_name  = 0;            
    const char *method_name = 0;            
    VCallbackList::findCallback(mthd, class_name, method_name, methodIndex);
    a << class_name;                //  change method's class name
    a << method_name;               //  change method name
    
}

// ---------------------------------------------------------------------------
//  Get this object from the archive by retrieving the object 
//  data of the base class and then the object data of this class.
//
//  This method is invoked by the getFrom() of this most-derived class or by
//  the getFrom() of a derived class.
//
void VPopupMenu::getFrom(VArchiver& a)
{
    VMenu::getFrom(a);                  //  base object data
    
    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VPopupMenuArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VPopupMenuArchiveTag));
        return;
    }

    VString localLabel;
    (void) a.getObject(&localLabel);   //    title (or label) of this popup      
    label.scanTilde(localLabel.gets());
    (void) a.getObject(&prompt);  //    prompt

    if (getArchiveTagRev(tag) >=2) {    // new for >=2
        a >> helpId;                //  helpId
    }

    if (getArchiveTagRev(tag)< 2){     // moved to VMenu when >= 2
        a >> idNo;                          //  data idNo
    }

    if (getArchiveTagRev(tag) > 0) {
		boolean b;
        a >> b; bEnabled = b;
    }

    if (getArchiveTagRev(tag) >= 3) {
		boolean b;
        a >> b; bChecked = b;
    }
    
    //  name of client  

    VString client_name;
    (void) a.getObject(&client_name);
    client = notifier->getClient(client_name, &clientIndex);
    
    //  class name of method and name of method

    char    method_name[256];
    char   *method_name_ptr = method_name;
    char    class_name[256];
    char   *class_name_ptr = class_name;

    a >> class_name_ptr;
    a >> method_name_ptr;

    mthd = VCallbackList::findCallback(class_name_ptr, method_name_ptr, &methodIndex);

    if (client != 0 && mthd != 0) {
        if (!client->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }
}

// ---------------------------------------------------------------------------
//  Get this object from the archive.  The 'data' parameter is a pointer
//  to the parent menu.
//
//  This method is invoked by VArchiver::getObject() or VMenu::getFrom().
//
void VPopupMenu::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VPopupMenu::getFrom(a);             
    menu = (VMenu *)data;

    if (do_realize) {
        hMenu = CreatePopupMenu();
        VOrdCollect itemsCopy(mitems());
        DO(itemsCopy, VObject, item)
            addItem(item, -1);
        END
        if (!bEnabled) {
            enable(FALSE);
        }
    }
}

#endif // ARCHIVER  
