// $Header:   Q:/views/win/vcs/menu.cpv   1.20   Mar 04 1997 17:55:24   gregm  $ 

//  menu.cpp
//
//  VMenu implementation
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
//  04/07/93 pat    changed id to VObject *
//  04/15/93 jmd    improved the accelerator add/remove logic
//  05/20/93 jmd    preened
//  06/28/93 pat    added support for VArchiver; preened.
//  07/20/93 jmd    removed free
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//  08/02/93 pat    added default argument to default constructor
//  08/03/93 jmd    made two constructors
//  08/04/93 jmd    set hMenu to 0 for nobuild option
//  08/04/93 jmd    added tests for hMenu == 0
//  08/06/93 jmd    back to one constructor: (so patrick was right...)
//  08/12/93 pat    made two constructors (...just kidding).
//                  fixed removeItem() to remove separators MF_BYPOSITION.
//  08/31/93 jmd    added setPopupClients
//  09/01/93 dgm    M8 changes; miscellaneous integral casts.
//  09/15/93 dgm    added realize().
//  09/22/93 dgm    added realizeMenuItem() (private).
//  09/22/93 pat    added archive tag
//  09/26/93 jmd    removed ARCHIVE condition for realize
//  10/11/93 pat    added VMenuItem::enable() call to realizeMenuItem() and
//                  addItem()
//  10/11/93 jmd    added VMenuItem::check() call to realizeMenuItem() and
//                  addItem()
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  02/26/94 jmd    change removeItem (BC)
//  03/07/94 pat    included notifier.h before menu.h to prevent
//                  multiple windows.h inclusion
//  03/08/94 pat    STRICT:
//                  used HMENU, not HANDLE;
//                  cast appropriately for AppendMenu(), insertMenu().
//  08/15/94 dgm    Added special check to ~VMenu() to not delete
//                  the special MDI "Window" pulldown VPopupMenu.
//  08/17/94 dgm    Added support for disabled VPopupMenus.
//  11/11/94 dss    Archiver tags macros.
//  11/18/94 dss    removeItem(): made count signed. GetMenuItemCount() was
//                  returning -1 causing count to 4 billion.
//  10/25/95 evc    Added names, helpid, and routines to find menuitems and
//                  popups.  Tinkered with the name of some class data
//  11/08/95 dss    Added bindItem(). ~VMenu(): test app->hwnd() before
//                  natively destroying menu.
//  11/08/95 evc    Small prob in getPopupByName/ID
//  11/14/95 evc    getPopup and getName functions changed to conform
//                  to menber function naming standard.
//  01/10/96 evc    Added  flag 'let_ole_delete' to defer deletion of ole
//                  provided menuitems and popups to compound doc 
//                  support.(windows only)
//	09/14/96 dgm	Fixed erroneous resource reference (BoundsChecker).
// ----------------------------------------------------------------------------

#include "notifier.h"
#include "menu.h"
#include "popupmen.h"
#include "appview.h"
#include "ordcllct.h"
#include "menuitem.h"
#include "window.h"
#include "iterator.h"
#include "set.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

defineClass(VMenu, VObject)
defineArchiveRevision(VMenu, 1)

// ---------------------------------------------------------------------------
// Construct a new VMenu with no items.
//
VMenu::VMenu(boolean build, 
    const VString& name, int id): name(name), idNo(id)
{
    let_ole_delete = FALSE;
    app = 0;
    menuId = 0;
    hMenu = 0;
    if (build && !(hMenu = CreateMenu()))  {
        print("Cannot create menu");
    }
}

// ---------------------------------------------------------------------------
// Construct the menu bar and popup menus from the GUI system
// resource 'name' and set the callbacks for the menu items to 'client'.
// The three arrays 'ids', 'mthds', and 'accels' are used to parameterize
// the menus: 'ids' is an array of integer values corresponding to the menu
// resource ids as they occur in the resource; 'mthds' points to an array of
// methods to be called when these items are selected.
//
VMenu::VMenu(const char *resource_name, int *ids, method *mthds, 
VObject *client, const VString& logicalName, int Id): name(logicalName),
idNo(Id)

{
    let_ole_delete = FALSE;
    hMenu = LoadMenu(notifier->inst(), (char *) resource_name);
    if (!hMenu) {
        print("No menu resource %s", resource_name);
        return;
    }
    menuId = 0;
    app = 0;
    buildPopup(hMenu, ids, mthds, client);
}

// ---------------------------------------------------------------------------
// Append a VPopupMenu or a VMenuItem object 'item' to the end
// of this VMenu.
//
void VMenu::addItem(VObject *item)
{
    addItem(item, -1);
}

// ---------------------------------------------------------------------------
// Append a separator to this VMenu.
//
void VMenu::addSeparator()
{
    VMenuItem *mi = new VMenuItem;
    mi->menu = this;
    items.add(mi);

    // jmd
    if (hMenu != 0) {
        (void)AppendMenu(hMenu, MF_SEPARATOR, 0, (LPCSTR) 0);
    }
}

// ---------------------------------------------------------------------------
// Remove the menu-items for the items in this menu.
//
VMenu::~VMenu()
{
    // if my parent's native handle is zero, that means its been natively
    // destroyed, which means I've been natively destroyed, also, 
    // so just zero my native handle.
    //
    // NOTE: do this BEFORE the recursive delete to reflect this
    // in the parent/child relationship.
    //
    if (app && app->hwnd() == 0) {
        hMenu = 0;
    }
        
    REVERSEDO(items, VObject, obj)
        removeItem(obj);
        if (obj->isA(VPopupMenuCls)) {
            //
            // Special case; do not delete the MDI "Window"
            // pulldown menu; the VMdiClient object will do it.
            //
            extern VClass *VMdiClientCls;
            VPopupMenu *pm = (VPopupMenu *)obj;
            VObject *pm_client = pm->getClient();
            if ((pm_client != 0) && pm_client->isA(VMdiClientCls)) {
                continue;
            }
        }
        if (!let_ole_delete){
            delete obj;
        }
    END

    if (hMenu) {
        DestroyMenu(hMenu);
    }
}

// ---------------------------------------------------------------------------
// Free the VMenuItem and VPopupMenu objects comprising this VMenu.
//
void VMenu::freeContents()
{
    items.freeContents();
}

// ---------------------------------------------------------------------------
// Set a new client object for all the items of this VMenu to 'client'. 
//
void VMenu::setPopupClients(VObject *client)
{
    DO (items, VObject, item)
        if (item->isA(VPopupMenuCls)) {
            ((VPopupMenu *)item)->setAllClients(client);
        }
    END
}


// ---------------------------------------------------------------------------
// Remove a VMenuItem or VPopupMenu 'item' from this VMenu and
// return TRUE, otherwise, return FALSE if 'item' is not a sub-menu or item
// of this VMenu.
//
boolean VMenu::removeItem(VObject *item)
{
    int     i, count;
    int     idx = (int) items.indexOf(item);

    if (items.removeSame(item)) {
        // jmd
        if (hMenu == 0) {
            return TRUE;
        }

        if (item->isA(VMenuItemCls)) {
            VMenuItem *itm = (VMenuItem *)item;
            int midx = itm->menuId;
            if (midx == -1 && itm->getLabel() == "") {
                //  separator
                RemoveMenu(hMenu, (UINT) idx, MF_BYPOSITION);
            }
            else {
                if (midx >= 0) {
                    RemoveMenu(hMenu, (UINT) midx, MF_BYCOMMAND);
                }
            }
            itm->menu = NIL;

            // remove accelerator
            if (itm->wKey != K_NULL) {
                notifier->unsetAccel(itm->wKey);
            }

        }
        else {
            VPopupMenu *p = (VPopupMenu *)item;
            count = GetMenuItemCount(hMenu);
            for (i = 0; i < count; i++) {
                if (p->hMenu == GetSubMenu(hMenu, i)) {
                    RemoveMenu(hMenu, i, MF_BYPOSITION);
                    break;
                }
            }
            if (count == i) {
                return FALSE;
            }

            p->menu = NIL;

            // remove accelerators for menuitems
            count = unsigned(p->items.count());
            for (i = 0; i < count; i++) {
                VMenuItem *mi = (VMenuItem *)(p->menuAt(i));
                if (mi->wKey != K_NULL) {
                    notifier->unsetAccel(mi->wKey);
                }
            }

        }
        if (app && app->menuUpdate) {
            app->drawMenu();
        }
        return TRUE;
    }
    return FALSE;
}

// ---------------------------------------------------------------------------
//
void VMenu::bindItem(VObject *item, int index, int flag)
{
    VObject *oldItem;

    if (!item) {
        return;
    }

	if (flag <= 0) {
	    if (oldItem = items.findSame(item)) {
	        items.remove(oldItem);
	    }
	    if (index < 0) {
	        items.add(item);
	    }
	    else {
	        items.insertAt(index, item);
	    }
	}

	if ((flag < 0) || (flag > 0)) {
	    if (item->isA(VMenuItemCls)) {
	        VMenuItem *mitem = (VMenuItem *)item;
	        mitem->menu = this;
	        // activate accelerator key for menuitem
	        if (mitem->wKey != K_NULL) {
	            notifier->setAccel
					(mitem->wKey, methodOf(VMenuItem, selected), mitem);
	        }
	        mitem->enable(mitem->enabled());
	        mitem->check(mitem->checked());
	    }
	    else {
	        VPopupMenu *menu = (VPopupMenu *)item;
	        menu->menu = this;
			if (!menu->isEnabled()) {
	            menu->enable(FALSE);
	        }
	        // add accelerators
	        int count, i;
	        count = unsigned(menu->items.count());
	        for (i = 0; i < count; i++) {
	            VMenuItem *mi = (VMenuItem *)(menu->menuAt(i));
	            if (mi->wKey != K_NULL) {
	                notifier->setAccel
						(mi->wKey, methodOf(VMenuItem, selected), mi);
	            }
	        }
	    }
	}
}

// ---------------------------------------------------------------------------
// Add a VPopupMenu or a VMenuItem object 'item' to this VMenu.
// If the 'index' parameter is less than zero, then the 'item' will be
// appended onto the menu, otherwise it will be inserted before the menu
// item at 'index'.
//
void VMenu::addItem(VObject *item, int index)
{
    VMenuItem *itm;
    VPopupMenu *menu;
    int idx, original_index = index;

    if (!item) {
        return;
    }

    bindItem(item, index, 0);
    
    if (hMenu == 0) {
        return;
    }

    if (item->isA(VMenuItemCls)) {
        itm = (VMenuItem *)item;
        const char *p = itm->label;
        itm->menu = this;

        if (!p || !*p) {
            (void)AppendMenu(hMenu, MF_SEPARATOR, 0, (LPCSTR)0);
        }
        else {
            idx = itm->menuId = notifier->getMenuId();
            if (index < 0) {
                (void)AppendMenu(hMenu, 
                                 MF_STRING, 
                                 (UINT)(HMENU)idx, 
                                 (LPCSTR)itm->label.gets());
            }
            else  {
                (void)InsertMenu(hMenu, index, 
                                 MF_BYPOSITION | MF_STRING, 
                                 (UINT)(HMENU)idx, 
                                 (LPCSTR)itm->label.gets());
            }
        }
		itm->appendOrInsertMenuDone = 1;
    }
    else {
        menu = (VPopupMenu *)item;
        menu->menu = this;
#if 0
		//
		// This makes no sense; we know this is a popup-menu-item
		// not a regular menu-item, so GetMenuItemID does not apply.
		//
        if (index < 0) {
            int numPops = GetMenuItemCount((HMENU)hMenu);
            if (numPops > 0 && SC_RESTORE == GetMenuItemID((HMENU)hMenu, 
                numPops -1)) {
                index = numPops -1;
            }
        }
#endif
        if (index < 0) {
            (void)AppendMenu((HMENU)hMenu, MF_POPUP, (UINT)(HMENU)menu->hMenu, 
                             (LPCSTR)menu->label.gets());
        }
        else {
            char txt[22]; // This and the next statement support MDI
            if (0 >= GetMenuString((HMENU)hMenu, 0, (LPSTR)txt, 20, 
                MF_BYPOSITION)) {
                index++;
            }
            (void)InsertMenu((HMENU)hMenu, (UINT)index,
                             MF_BYPOSITION | MF_POPUP,
                             (UINT)(HMENU)menu->hMenu,
                             (LPSTR)menu->label.gets());
        }
//      //  The following supports VPopupMenus which contain previously added
//      //  items whose items' GUI components have not yet been added.
//      //  The popup menu that we are adding may contain C++/Views menu 
//      //  objects where the connection between the menu component and the
//      //  popup has been established only at the C++/Views level, and not yet
//      //  at the GUI level.  Such is the case with menu trees retrieved
//      //  whole from an archive. 
//
//      DO (menu->mitems(), VObject, item)
//          if ((item->isA(VMenuItemCls) && ((VMenuItem *) item)->menu != 0)) {
//              continue;   //  menu item already associated with popup
//          }
//          else if (((VPopupMenu *) item)->menu != 0) {
//              continue;   //  sub menu already added to popup
//          }       
//          menu->addItem(item, -1);
//      END
    }

	bindItem(item, original_index, 1);

    if (app && app->menuUpdate) {
        app->drawMenu();
    }
}

// ---------------------------------------------------------------------------
// Insert a VPopupMenu object 'item' or a VMenuItem object 'item' in
// this VMenu before the menu item at 'index'.
//
void VMenu::insertItem(VObject *item, int index)
{
    addItem(item, index);
}

// ---------------------------------------------------------------------------
// Return the VMenu object with GUI system handle 'hMenu'.  NOTE: This
// method may not be portable.
//
VMenu *VMenu::menuOf(HMENU hMenu)
{
    if (this->hMenu == hMenu) {
        return this;
    }

    DO(items, VMenu, m)
        if (m->isA(VMenuCls)) {
            VMenu *mp;
            if (mp = m->menuOf(hMenu)) {
                return mp;
            }
        }
    END

    return (VMenu *)0;
}

// ---------------------------------------------------------------------------
// Return the VMenuItem object with a GUI system id of 'menuId'.
// NOTE: This method may not be portable.
//
VMenuItem *VMenu::itemOf(unsigned menuId)
{
    VMenuItem *mitm;

    DO(items, VMenu, obj)
        if (obj->isA(VMenuItemCls)) {
            mitm = (VMenuItem *)obj;
            if ((unsigned)mitm->menuId == menuId) {
                return mitm;
            }
        }
        else  {
            if (mitm = obj->itemOf(menuId)) {
                return mitm;
            }
        }
    END

    return 0;
}

// ---------------------------------------------------------------------------
// Construct a popup menu and its items recursively
//
void VMenu::buildPopup(HMENU h, int *ids, method *mthds, VObject *client)
{
    int mid, i, n = GetMenuItemCount(h);
    char b[100];
    HMENU sh;

    for (i = 0; i < n; i++) {
        GetMenuString(h, i, b, 99, MF_BYPOSITION);
        if (sh = GetSubMenu(h, i)) { // a popup-menu
            VPopupMenu *m;
            items.add(m = new VPopupMenu(sh, b));
            m->menu = this;
            m->buildPopup(sh, ids, mthds, client);
        }
        else { // a menu item
            mid = GetMenuItemID(h, i);
            int *ip, nid;
            if (mid == 0) {
                VMenuItem *m = new VMenuItem;
                m->menu = this;
                items.add(m);
                continue;
            }

            for (ip = ids, nid = 0; *ip != -1; ip++ , nid++) {
                if (*ip == mid) {
                    break;
                }
            }

            if (*ip == -1) {
                print("Menu Item %d not found", mid);
                return;
            }
            VMenuItem *m = new VMenuItem((unsigned)mid, b, client, mthds[nid]);
            m->menu = this;
            items.add(m);
        }
    }
}

// ---------------------------------------------------------------------------

// ===========================================================================
#ifndef CV_NOARCHIVER   

// --------------------------------------------------------------------------
//  Put this object to the archive.  This method is called from 
//  VArchiver::putObject().
//
void VMenu::putTo(VArchiver& a)
{
    VMenu::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//  Put this object to the archive.
//
//  The 'most_derived' parameter is irrelevant for VMenu.
//
void VMenu::putTo(VArchiver& a, boolean most_derived)
{
    a << VMenuArchiveTag;

    // new in 4.0 - named menus and helpId
    (void) a.putObject(&name);          //  name
    a << idNo;                          //  data idNo

    //  we archive the contents of the collection by hand, so that
    //  getFrom() has control over realization (see getFrom()).

    a << items.count();
    DO (items, VObject, item) 
        a.putObject(item);
    END
}

// --------------------------------------------------------------------------
//  Get this VMenu object from the archive.
//
//  This method is invoked by the getFrom() of this most-derived class or by
//  the getFrom() of a derived class (such as VPopupMenu).
//
//  Note that, under MS-Windows, the GUI menubar already exists by now (per
//  VMenu()).
//
void VMenu::getFrom(VArchiver& a)
{
    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VMenuArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VMenuArchiveTag));
        return;
    }

    if (getArchiveTagRev(tag) >0) {
        (void) a.getObject(&name);          //  name
        a >> idNo;                          //  data idNo
    }

    //  we do the collection by hand, so tha twe have control over
    //  realization

    integer count;
    a >> count;
    items.expandTo(count);
    for (int i = 0; i < count; i++) {
        items.add(a.getObject(0, this, TRUE));
    }
}

// --------------------------------------------------------------------------
//  Get this object from the archive.  The 'data' and 'do_realize' parameters
//  are ignored.
//
//  Note that VMenu::getFrom() does not realize a GUI menu object.
//
//  A top-level VMenu (the application "menubar") is realized 
//  by the application under VAppView::addPopup().  An archived top-level 
//  VMenu is simply a place to keep the collection of popup menus to 
//  supply to VAppView::addMenu() or VAppView::removeMenu().
//
void VMenu::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VMenu::getFrom(a);                  
}

#endif //   ARCHIVER    

// --------------------------------------------------------------------------
boolean VMenu::realize(VObject *client)
{
    if (hMenu != 0) {
        return TRUE;
    }
    else if ((hMenu = CreateMenu()) == 0) {
        return FALSE;
    }
    DO (this->mitems(), VObject, menu_item)
        this->realizeMenuItem(menu_item);
    END
    if (client != 0) {
        this->initializeMenuClients(client);
    }
    return TRUE;
}

// --------------------------------------------------------------------------
void VMenu::realizeMenuItem(VObject *menu_item)
{
    HMENU hmenu;

    if ((menu_item == 0) || (hmenu = this->hmenu()) == 0) {
        return;
    }
    if (menu_item->isA(VMenuItemCls)) {
        VMenuItem *mi = (VMenuItem *)menu_item;
        mi->menu = this;
        const char *s = mi->label;
        if ((s == 0) || (s[0] == '\0')) {
            (void)AppendMenu(hmenu, MF_SEPARATOR, 0, (LPSTR)0);
        }
        else {
            int index = mi->menuId = notifier->getMenuId();
            (void)AppendMenu(hmenu, MF_STRING, 
                             (UINT)(HMENU)index, (LPCSTR)(mi->label.gets()));
        }
		mi->appendOrInsertMenuDone = 1;
        if (mi->wKey != K_NULL) {
            notifier->setAccel(mi->wKey, methodOf(VMenuItem, selected), mi);
        }
        mi->enable(mi->enabled());
        mi->check(mi->checked());
    }
    else if (menu_item->isA(VPopupMenuCls)) {
        VPopupMenu *pm = (VPopupMenu *)menu_item;
        pm->menu = this;
        int position = -1, npopups = GetMenuItemCount(hmenu);
        if ((npopups > 0) &&
            (SC_RESTORE == GetMenuItemID(hmenu, npopups - 1))) {
            position = npopups - 1;
        }
        if (position < 0) {
            (void)AppendMenu(hmenu, MF_POPUP, (UINT)(HMENU)(pm->hMenu),
                             (LPCSTR)pm->label.gets());
        }
        else {
            char buffer[22]; // This and the next statement support MDI
            if (GetMenuString
                (hmenu, 0, (LPSTR)buffer, 20, MF_BYPOSITION) <= 0) {
                position++;
            }
            (void)InsertMenu(hmenu, position, MF_BYPOSITION | MF_POPUP,
                             (UINT)(HMENU)(pm->hMenu),
                             (LPCSTR)pm->label.gets());
        }
        int i, count = unsigned(pm->items.count());
        for (i = 0 ; i < count ; i++) {
            VMenuItem *mi = (VMenuItem *)(pm->menuAt(i));
            if (mi->wKey != K_NULL) {
                notifier->setAccel
                    (mi->wKey, methodOf(VMenuItem, selected), mi);
            }
        }
        pm->enable(pm->isEnabled());
    }
}

// --------------------------------------------------------------------------
void VMenu::initializeMenuClients(VObject *client)
{
    DO (this->mitems(), VObject, menu_item)
        if (menu_item->isA(VMenuItemCls)) {
            VMenuItem *mi = (VMenuItem *)menu_item;
            if (mi->getClient() == 0) {
                mi->setClient(client);
            }
        }
        else if (menu_item->isA(VPopupMenuCls)) {
            VPopupMenu *pm = (VPopupMenu *)menu_item;
            if (pm->getClient() == 0) {
                pm->setClient(client);
            }
            pm->initializeMenuClients(client);
        }
    END
}

// --------------------------------------------------------------------------
    VPopupMenu  *   VMenu::getPopupFromId(int desiredId){
        VPopupMenu * desiredPopup=0; 
        DO(this->mitems() , VObject, anId)
            if (anId->isA(VPopupMenuCls)){
                int id = ((VPopupMenu *)anId)->getIdNo(); 
                if (id == desiredId){
                    desiredPopup =  (VPopupMenu *)anId;  
            
                    break;
                } 
            }  
            
            // recurse within this popup
            if (anId->isA(VPopupMenuCls)){
                VPopupMenu * popup = (VPopupMenu*) anId;
                desiredPopup =  popup->getPopupFromId (desiredId);    
            }  
            if (desiredPopup !=0) break; // found it
        END    
        return desiredPopup;
    }
// --------------------------------------------------------------------------
    VMenuItem   *   VMenu::getMenuItemFromId(int desiredId){
        VMenuItem * desiredItem=0; 
        DO(this->mitems() , VObject, anId)
            if (anId->isA(VMenuItemCls)) {
                int id = ((VMenuItem *)anId)->getIdNo(); 
                if (id == desiredId){
                    desiredItem =  (VMenuItem *)anId;  
                    break;
                } 
            }  else if (anId->isA(VMenuCls)){
                VMenu * popup = (VMenu*) anId;
                desiredItem = popup->getMenuItemFromId (desiredId);    
            }  
            if (desiredItem !=0) break; // found it
        END    
        return desiredItem;
    }
//----------------------------------------------------------------------------
    VMenuItem   *   VMenu::getMenuItemFromName(const VString& desiredName){
        VMenuItem * desiredItem=0; 
        DO(mitems() , VObject, anId)
            if (anId->isA(VMenuItemCls)) {
                VString N = ((VMenuItem *)anId)->getName(); 
                if (N == desiredName){
                    desiredItem =  (VMenuItem *)anId;  
            
                    break;
                } 
            }  else if (anId->isA(VMenuCls)){
                VMenu * popup = (VMenu*) anId;
                desiredItem =popup->getMenuItemFromName (desiredName);    
            }  
            if (desiredItem !=0) break; // found it
        END    
        return desiredItem;
    }
// --------------------------------------------------------------------------
    VPopupMenu  *   VMenu::getPopupFromName(const VString& desiredName){
        VPopupMenu * desiredPopup=0; 
        DO(this->mitems() , VObject, anId)
            if (anId->isA(VPopupMenuCls)){
                VString name = ((VPopupMenu *)anId)->getName(); 
                if (name == desiredName){
                    desiredPopup =  (VPopupMenu *)anId;  
                    break;
                } 
            }  
            // recurse for the popup within a popup
            if (anId->isA(VPopupMenuCls)){
                VPopupMenu * popup = (VPopupMenu*) anId;
                desiredPopup = popup->getPopupFromName (desiredName);    
            }  
            if (desiredPopup !=0) break; // found it
        END    
        return desiredPopup;
    }
// --------------------------------------------------------------------------
