// $Header:   Q:/views/common/vcs/POPUPMEN.H_v   1.11   Mar 04 1997 14:13:34   gregm  $

//  popupmenu.h
//
//  VPopupMenu interface
//
//  Allegris Foundation 1.1.00
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
//  04/12/93 jmd    added prompts to addItems()
//  06/28/93 pat    added support for VArchiver.
//  06/28/93 dgm    added setLabelAt().
//  06/30/93 dgm    new VMenuData interface.
//  07/20/93 jmd    made setPrompt take a const VString&, removed free()
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//  07/27/93 pat    added client and method indices
//  08/06/93 jmd    added consts
//  08/07/93 jmd    added getClientIndex() and setClientIndex().
//  08/08/93 jmd    added idNo
//  08/08/93 jmd    added getMethodIndex() and setMethodIndex().
//  09/01/93 dgm    M8 changes; cast return value of getMethodIndex()
//                  and getClientIndex().
//  09/13/93 dgm    M8 changes; change VMenuData& parameter to VMenuData.
//  09/16/93 dgm    Added get/setClient().
//  03/08/94 pat    STRICT:  unsigned type used for handle argument
//                  in VPopupMenu() is now HMENU.
//  08/17/94 dgm    Added enable(), disable(), and isEnabled().
//  09/25/95 pkt    changed 'getLabel' not to return a modifiable string.
//  10/25/95 evc    Changes associated with adding names and helpIds.
//  10/26/95 evc    Added enableMenuItem & checkMenuItem by name
//  11/08/95 evc    Fixed up for use in common for OS/2 and windows
//  12/28/95 evc    Added enablePopupMenu, checkPopupMenu, enablePopupMenu
//                  and checkPopupMenu along with supporting routines.
//	01/09/96 dgm	Non-inlined getPopupMenu().
//	06/12/96 pkt	updated HMENU -> VHMENU and HWND -> VHWND.
// ---------------------------------------------------------------------------

#ifndef VPOPUPMEN_H
#define VPOPUPMEN_H

#include "menu.h"
#include "menudata.h"
#include "menuitem.h"
#include "str.h"
#include "iterator.h"

extern VClass *VPopupMenuCls;

CLASS VNotifier;
CLASS WinAttrs;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

//  Introduction
//
//  A VPopupMenu object contains an ordered collection of VMenuItem
//  or VPopupMenu objects that implement the automatic menu selection process. 
//  The VPopupMenu class implements methods for altering names and
//  states of the menu-items.  For example, menu-items can be enabled/disabled
//  and checked/unchecked. Cascading, or side-step, menus are obtained by 
//  using the inherited addItem() and insertItem() methods of VMenu to append 
//  or insert VPopupMenu objects onto a VPopupMenu object.

CLASS VPopupMenu : public VMenu {

public:
                VPopupMenu();

#if defined (PM)
                VPopupMenu(VHWND , 
                const VString& ,  //label
                const VString& = VString(""),   //name
                int = -1,   //id
                int =0 );   //help Id
#elif defined (CV_WIN16) || defined (CV_WIN32)
                VPopupMenu(VHMENU, const VString& ,  //label
                const VString& = VString(""),   //name
                int = -1,   //id
                int =0 );   //help Id
#endif



                VPopupMenu(const VString& label,
                    const VString& = VString(""),
                    int = -1,
                    int  = 0 );

                VPopupMenu(const VMenuData &);
                ~VPopupMenu();

    VClass     *iam();

    void         addMenuItem(const VMenuData &, VObject * = 0);
    void         addMenuItems(const VMenuData *, VObject * = 0);
#if _MSC_VER >= 700
    void         addMenuItems(VMenuData, ...);
#else
    void         addMenuItems(const VMenuData&, ...);
#endif
    VMenuItem  *getMenuItem(int menu_item_id);
    VMenuItem  *getMenuItem(const VString&); // find it by name (new)

    // NEW
    VPopupMenu *getPopupMenu(int menu_item_id);
    VPopupMenu *getPopupMenu(const VString&); 
    // Note that getMenuItem OR getPopupMenu are NOT recursive
    // nor the routines that follow

    void        setMenuItemPrompt(int idno, const VString& p);
    void        setMenuItemLabel(int idno, const VString& label);
    void        setMenuItemClient(int idno, VObject *clnt);
    void        setMenuItemCallback(int, method, VObject * = 0);
    void        setMenuItemName(int idno, const VString& );
    void        setMenuItemHelpId(int idno, int );

    void        setMenuItemPrompt(const VString& , const VString& );
    void        setMenuItemLabel(const VString& , const VString& );
    void        setMenuItemClient(const VString& , VObject *);
    void        setMenuItemCallback(const VString&, method, VObject * = 0);
    void        setMenuItemId(const VString&, int);
    void        setMenuItemHelpId(const VString& , int );


    void        enableMenuItem(int menuno, boolean = TRUE);
    void        checkMenuItem(int menuno, boolean = TRUE);

    void        enableMenuItem(const VString& , boolean = TRUE);
    void        checkMenuItem(const VString&  , boolean = TRUE);

    // NEW
    void        enablePopupMenu(int menuno, boolean = TRUE);
    void        checkPopupMenu(int menuno, boolean = TRUE);

    void        enablePopupMenu(const VString& , boolean = TRUE);
    void        checkPopupMenu(const VString&  , boolean = TRUE);
    //-----


    const VString& getLabel();
    void        setLabel(const VString& label);
    void        setLabelAt(const VString& label, int n);

    void        checkAt(boolean tf, int n);
    void        checkAll(boolean tf);
    boolean     checkedAt(int n);
    void        enableAt(boolean tf, int n);
    void        enableAll(boolean tf);
    boolean     enabledAt(int n);

    void        addItems(char **names, method *sels, VObject *clnt,
                            char **prompts = 0);
    void        addItems(char **names, method *sels, int *keys, VObject *clnt,
                            char **prompts = 0);

    void        setAllClients(VObject *clnt);
    void        setClientMethod(VObject *clnt, method m, int i);
    void        uponSelect(VObject *clnt, method m);
    VObject    *getClient() const;
    void        setClient(VObject *);
    void        selected();

    void        popupAt(int x, int y);
    boolean     copyFrom(VObject *obj);
    void        setPrompt(const VString& p);
    VString&    getPrompt();

    void        setPromptAt(const VString& p, int i);


    void        disable(boolean = TRUE);
    void        enable(boolean = TRUE);

    // NEW
    void        check(boolean =TRUE);
    void        uncheck(boolean =TRUE);

    boolean     isChecked() const; // NEW

    boolean     isEnabled() const;
    // new
    int         getHelpId() const;
    void        setHelpId(int);

#ifndef CV_NOARCHIVER
    integer          getClientIndex() const;
    void             setClientIndex(integer);
    integer          getMethodIndex() const;
    void             setMethodIndex(integer);



    //  Archival and Retrieval
    void        putTo(VArchiver& a);
    void        getFrom(VArchiver& a);
    void        getFrom(VArchiver& a, VObject *data,
                        boolean do_realize);
#endif

protected:

    friend VMenu; 
    friend VNotifier;

    VMenu      *menu;       //  the VMenu this VPopupMenu is an item of
    VObject    *client;     //  the callback client
    method      mthd;       //  the callback method
    VString     label;      //  the VPopupMenu title (or label)
    // was called 'name'

    VString     prompt;     //  prompt string
    int         helpId;     //  Help!   
    boolean     bEnabled : 1;
    boolean     bChecked : 1;
	boolean   	appendOrInsertMenuDone : 1;
    boolean     unusedA;

#ifndef CV_NOARCHIVER
    void        putTo(VArchiver& a, boolean most_derived);
#endif

private:

#ifndef CV_NOARCHIVER
    integer     clientIndex;
    integer     methodIndex;
#endif

};

// ---------------------------------------------------------------------------
//
inline VString& VPopupMenu::getPrompt()
{
    return prompt;
}

// ---------------------------------------------------------------------------
//
inline void VPopupMenu::setPrompt(const VString& p)
{
    prompt = p;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
//
inline integer VPopupMenu::getClientIndex() const
{
    return clientIndex;
}

// ---------------------------------------------------------------------------
//
inline void VPopupMenu::setClientIndex(integer index)
{
    clientIndex = index;
}

// ---------------------------------------------------------------------------
//
inline integer VPopupMenu::getMethodIndex() const
{
    return methodIndex;
}

// ---------------------------------------------------------------------------
//
inline void VPopupMenu::setMethodIndex(integer index)
{
    methodIndex = index;
}
#endif

// ---------------------------------------------------------------------------
//
inline VObject *VPopupMenu::getClient() const
{
    return client;
}

// ---------------------------------------------------------------------------
//
inline void VPopupMenu::setClient(VObject *c)
{
    client = c;
}

//------------------------------------------------------------------

inline  int         VPopupMenu::getHelpId() const{
    return helpId;
}
//------------------------------------------------------------------

inline  void        VPopupMenu::setHelpId(int helpId) {
    VPopupMenu::helpId = helpId;
}

inline void VPopupMenu::setMenuItemName(int menu_item_id, const VString &Name)
{
    VMenuItem *menu_item = this->getMenuItem(menu_item_id);
    
    if (menu_item != 0) {
        menu_item->setName(Name);
    }
}

inline void VPopupMenu::setMenuItemHelpId(int menu_item_id, int help_id)
{
    VMenuItem *menu_item = this->getMenuItem(menu_item_id);
    
    if (menu_item != 0) {
        menu_item->setHelpId(help_id);
    }
}

// the following identify the menuitem with strings

inline void VPopupMenu::setMenuItemPrompt(const VString & name, const VString &prompt)
{
    VMenuItem *menu_item = this->getMenuItem(name);
    if (menu_item != 0) {
        menu_item->setPrompt(prompt);
    }
}
inline void VPopupMenu::setMenuItemLabel(const VString & name, const VString &label)
{
    VMenuItem *menu_item = this->getMenuItem(name);
    if (menu_item != 0) {
        menu_item->setLabel(label);
    }
}

inline void     VPopupMenu::setMenuItemClient(const VString& name , VObject *client_object){
    VMenuItem *menu_item = this->getMenuItem(name);
    if (menu_item != 0) {
        menu_item->setClient(client_object);
    }
}

inline void     VPopupMenu::setMenuItemCallback(const VString& name, 
                                     method client_method,
                                     VObject *client_object)
{
    VMenuItem *menu_item = this->getMenuItem(name);
    
    if (menu_item != 0) {
        menu_item->setClientMethod(client_object, client_method);
    }
}
inline void     VPopupMenu::setMenuItemId(const VString& name, int new_id ){
    VMenuItem *menu_item = this->getMenuItem(name);
    if (menu_item != 0) {
        menu_item->setIdNo(new_id);
    }
}

inline void VPopupMenu::setMenuItemHelpId(const VString& name, int help_id)
{
    VMenuItem *menu_item = this->getMenuItem(name);
    if (menu_item != 0) {
        menu_item->setHelpId(help_id);
    }
}

// ---------------------------------------------------------------------------
//
inline void VPopupMenu::enableMenuItem(const VString& name , boolean b)
{
    VMenuItem *menu_item = this->getMenuItem(name);
    
    if (menu_item != 0) {
        menu_item->enable(b);
    }
}

// ---------------------------------------------------------------------------
//
inline void VPopupMenu::checkMenuItem(const VString& name, boolean b)
{
    VMenuItem *menu_item = this->getMenuItem(name);
    
    if (menu_item != 0) {
        menu_item->check(b);
    }
}

// ---------------------------------------------------------------------------
// If b is true, enable the pull-right VPopupMenu item designated by "menuno"
// within the given popup. If b is FALSE, disable it.
//
inline void     VPopupMenu::enablePopupMenu(int menuno, boolean b){
    VPopupMenu* pop=  this-> getPopupMenu(menuno);
    if (pop!= 0) {
        pop->enable(b);
    }
}

// ---------------------------------------------------------------------------
// If b is true, check the pull-right VPopupMenu item designated by "menuno"
// within the given popup. If b is FALSE, uncheck it.
//
inline void     VPopupMenu::checkPopupMenu(int menuno, boolean b){
    VPopupMenu* pop= this-> getPopupMenu(menuno);
    if (pop!= 0) {
        pop->check(b);
    }
}

// ---------------------------------------------------------------------------
// If b is true, enable the pull-right VPopupMenu item designated by "name"
// within the given popup. If b is FALSE, disable it.
//
inline void     VPopupMenu::enablePopupMenu(const VString& name,boolean b) {
    VPopupMenu* pop= this-> getPopupMenu(name);
    if (pop!= 0) {
        pop->enable(b);
    }

}

// ---------------------------------------------------------------------------
// If b is true, check the pull-right VPopupMenu item designated by name 
// within the given popup. If b is FALSE, uncheck it.
//
inline void     VPopupMenu::checkPopupMenu(const VString& name, boolean b) {
    VPopupMenu* pop= this-> getPopupMenu(name);
    if (pop!= 0) {
        pop->check(b);
    }
}

// ---------------------------------------------------------------------------
//------------------------------------------------------------------

#endif // VPOPUPMEN_H


