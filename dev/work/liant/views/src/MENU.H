// $Header:   Q:/views/common/vcs/MENU.H_v   1.11   Mar 04 1997 14:13:06   gregm  $ 

//	menu.h
//
//	VMenu interface
//
//	Allegris Foundation 1.1.00
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
//	04/09/93 pat	changed id to VObject *
//	06/28/93 pat	added support for VArchiver; preened.
//	07/20/93 jmd	removed free
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//	08/02/93 pat	added default argument to default constructor
//	08/03/93 jmd	made two constructors
//	08/06/93 jmd	made one constructor (again), added consts
//	08/31/93 jmd	added setPopupClients
//	09/15/93 dgm	added realize().
//	09/22/93 dgm	added realizeMenuItem() (private).
//	09/26/93 jmd 	removed ARCHIVE condition for realize
//	03/08/94 pat	STRICT:  used HMENU, not HANDLE
//	10/25/95 evc	Added names, helpid, and routines to find menuitems and
//					popups.  Tinkered with the name of some class data
//  11/08/95 evc    Added changes so we could make this common for
//					os2 and windows.  Notee that its pvcs archive changed: It
//					used to be in thw windows archive.
//	11/08/95 dss	Added bindItem().
//	06/12/96 pkt	updated HWND -> VHWND and HMENU -> VHMENU.
// ----------------------------------------------------------------------------

#ifndef VMENU_H
#define VMENU_H

#include "wintypes.h"
#include "ordcllct.h"
#include "str.h"

extern VClass *VMenuCls;

CLASS VOrdCollect;
CLASS VMenuItem;
CLASS VNotifier;
CLASS VArchiver;
CLASS VPopupMenu;
CLASS VAppView;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif


#define MAXmenuLabel 20

//	Introduction 
//
//	The VMenu class implements the menu-bar for an application's top
//	level window (a subclass of VAppView). A VMenu object contains an
//	ordered collection of menu-items to be displayed on the menu-bar. The
//	menu-items contained in a VMenu object's collection are instances of
//	the VPopupMenu or VMenuItem classs. The VMenu class defines
//	methods for manipulation of the contents of the menu bar and for
//	attaching a VMenu object to a window.

CLASS VMenu;

CLASS VMenu : public VObject {

public:

	//	Constructors

#if defined (PM)
					VMenu(boolean build = TRUE,
#else
					VMenu(boolean build = FALSE,
#endif
						const VString& name=VString(), // logical name
						int idNo= 0);

					VMenu(const char *, // name in resource
						int *ids, //ids in resource
						method *mthds, 
						VObject *clnt,
						const VString& name=VString(), // logical name
						int idNo= 0);

					~VMenu();
		
	VClass	  	   *iam();

	//	Adding and Removing Submenus and MenuItems 
    
	void 			insertItem(VObject *item, int index);
	void 			addItem(VObject *item, int index);
	void 			addItem(VObject *item);
	boolean 		removeItem(VObject *item);
	void 			addSeparator();
	void			bindItem(VObject *item, int index = -1, int = -1);

	void		   	setPopupClients(VObject *client);

	//	Interrogating the Menu Bar

	VPopupMenu 	   *menuAt(int index); 
	VMenuItem  	   *itemOf(unsigned n);

#if defined (PM)
	VMenu 	   	   *menuOf(VHWND);
	HWND 			hmenu();
#elif defined (CV_WIN16) || defined (CV_WIN32)
	VMenu 	   	   *menuOf(VHMENU);
	HMENU 			hmenu();
#endif

	void 			setApp(VAppView *appvw);
	VOrdCollect&	mitems();

	//	Initialization 

	void 			freeAll();
	void 			freeContents();

#ifndef CV_NOARCHIVER
	//	Archival and retrieval

	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *data, boolean do_realize);
#endif

	void			realizeMenuItem(VObject *);
	void			initializeMenuClients(VObject *);
	boolean			realize(VObject * = 0);

	// this is a user assigned id for
	// retrival.  Dont confuse with the underlying gui ID
	int			getIdNo() const; 
	void		setIdNo(int v);

	// this is a user assigned VString that is the menu's name used for
	// retrival.  
	VString&	getName();
	void        setName(const VString&);

	// these routines search recursively thru the menu and all its submenus
	// untill a match is had, otherwise they return 0.
	VPopupMenu 	* 	getPopupFromId(int);
	VPopupMenu 	* 	getPopupFromName(const VString&);
	VMenuItem 	*   getMenuItemFromId(int);
	VMenuItem 	* 	getMenuItemFromName(const VString&);


	void			oleDeletes(boolean);

protected:

	friend VMenuItem;
	friend VNotifier;
	friend VAppView;
	friend VPopupMenu;

	VOrdCollect 	items;		//	VMenuItems and VPopupMenus
	VAppView 	   *app; 		//	associated application view 
	ushort 			menuId;		// 	GUI id

#if defined(CV_WINDOWS)
	boolean   let_ole_delete;
#endif

#if defined (PM)
	VHWND hwnd;
#elif defined (CV_WIN16) || defined (CV_WIN32)
	VHMENU 			hMenu;		// 	GUI handle
#endif

	VString		name;		// used for identification (new)
	int			idNo;		//	id number used for identification

#if defined (PM)
	void buildPopup(VHWND, int *,method *,id);
#elif defined (CV_WIN16) || defined (CV_WIN32)
	void buildPopup(VHMENU , int *, method *, VObject *);
#endif

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

};

// ---------------------------------------------------------------------------
//
inline VPopupMenu *VMenu::menuAt(int index) 
{ 
	return (VPopupMenu *) items.idAt(index); 
}

#if defined (PM)
inline VHWND VMenu::hmenu() 
{ 
	return hwnd;
}
#elif defined (CV_WIN16) || defined (CV_WIN32)
// ---------------------------------------------------------------------------
inline VHMENU VMenu::hmenu() 
{ 
	return hMenu; 
}
#endif
// ---------------------------------------------------------------------------
inline void VMenu::setApp(VAppView *appvw) 
{ 
	app = appvw; 
}
// ---------------------------------------------------------------------------
inline VOrdCollect& VMenu::mitems() 
{ 
	return items; 
}
// ---------------------------------------------------------------------------
//
inline void VMenu::freeAll() 
{ 
	freeContents(); 
	delete this; 
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//
inline void VMenu::setIdNo(int v)
{
	idNo = v;
}

// ---------------------------------------------------------------------------
inline int VMenu::getIdNo() const
{
	return idNo;
}
//------------------------------------------------------------------
inline VString&	VMenu::getName(){
	return name;
}
//------------------------------------------------------------------
inline	void        VMenu::setName(const VString& name){
	VMenu::name = name;
}
//------------------------------------------------------------------

#if defined(CV_WINDOWS)
inline void			VMenu::oleDeletes(boolean f){
	let_ole_delete = f;
}
#endif
#endif	//	!VMENU_H 
