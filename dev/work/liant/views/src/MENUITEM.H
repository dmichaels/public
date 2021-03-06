// $Header:   Q:/views/common/vcs/MENUITEM.H_v   1.9   Mar 04 1997 14:13:08   gregm  $ 

//	menuitem.h
//
//	VMenuItem interface.
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
//	04/10/93 jmd	preened, added prompt
//	05/13/93 jmd	added prompt getMenu()
//	06/25/93 pat	added support for VArchiver.
//	06/28/93 dgm	added setValue(), getValue(), and value.
//	06/30/93 dgm	updated for the new VMenuData interface.
//	07/20/93 jmd	made setPrompt take a const VString&, removed free
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//	07/27/93 pat	added client and method indices
//  08/01/93 jmd	made setValue public...
//  08/06/93 jmd	added consts
//	08/07/93 jmd	added getClientIndex() and setClientIndex().
//	08/08/93 jmd	added assignment operator
//	08/08/93 jmd	renamed value to idNo
//	08/08/93 jmd	added getMethodIndex() and setMethodIndex().
//	09/01/93 dgm	M8 changes; cast return value of getMethodIndex()
//					and getClientIndex().
//	09/16/93 dgm	Added getClient().
//	08/17/94 dgm	Split up setClientMethod() into two parts;
//					added setAccel().
//	09/28/94 jld	Added unsetAccel(), getAccel() and getMethod()
//  10/24/95 evc  	added name and help id.  Note what used to be 'name' is
//					now 'label'; name is an id to find a menuitem, label
//					is what you see in the pulldown.
//  01/10/96 evc    Added  flag 'let_ole_delete' to defer deletion of ole
// 					provided menuitems and popups to compound doc 
//					support.(windows only)
//	06/12/96 pkt	added #include "wintypes.h".
//	09/14/96 dgm	Fixed erroneous resource reference (BoundsChecker).
// ----------------------------------------------------------------------------

#ifndef VMENUITEM_H
#define VMENUITEM_H


#if defined (PM)
extern unsigned short availIds;
#endif


#include "display.h"
#include "str.h"
#include "wintypes.h"

extern VClass *VMenuItemCls;

CLASS VSysMenu;
CLASS VPopupMenu;
CLASS VMenu;
CLASS VMenuData;
CLASS VNotifier;
CLASS VStdMdiAppView;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

CLASS VMenuItem : public VObject {

public:
				VMenuItem();

				VMenuItem(	const VString& label, 
							VObject *clnt, method mthd, 
							int accelKey = K_NULL,
							int helpId=0,
							const VString& name=VString(""));

				VMenuItem(	unsigned idOfMenu, 
							const VString& label, 
							VObject *clnt, method mthd,
							int accelKey = K_NULL,
							int helpId=0,
							const VString& name=VString(""));

				VMenuItem(	const VMenuItem& m);

				~VMenuItem();

	VClass 	   *iam();

	VMenuItem& 	operator =  (const VMenuItem& m);

	boolean 	check(boolean);
	boolean 	checked();
	boolean 	enable(boolean);
	boolean 	enabled();
	boolean 	setClient(VObject *clnt);
	VObject    *getClient() const;
	void		setClientMethod(VObject *clnt, method mthd);
	void		setClientMethod(VObject *clnt, method mthd, int key );
	void		setAccel(int key);
	int			unsetAccel();
	int			getAccel() const;
	method		getMethod() const;
	void		setLabel(const VString& label);
	VString&	getLabel();
	boolean 	selected();

	void		setPrompt(const VString& p);
	VString& 	getPrompt();

	VMenu	   *getMenu() const;

	int			getIdNo() const;
	void		setIdNo(int v);

	// new 
	VString&	getName();
	void        setName(const VString&);

	int			getHelpId() const;
	void		setHelpId(int);

	// end new
#ifndef CV_NOARCHIVER
	integer			 getClientIndex() const;
	void			 setClientIndex(integer);

	integer			 getMethodIndex() const;
	void			 setMethodIndex(integer);

	//	Archival and retrieval

	void	   	putTo(VArchiver& a);
	void	   	getFrom(VArchiver& a);
	void	   	getFrom(VArchiver& a, VObject *data, boolean do_realize);
#endif

	inline void			oleDeletes(boolean);
	
protected:
	friend		VSysMenu;
	friend		VMenu;
	friend		VNotifier;
	friend		VMenuData;
	friend		VPopupMenu;
	friend		VStdMdiAppView;

	VMenu  	   *menu;		// the menu this `VMenuItem` is part of.
	VString	    label;		// the menu label to display on the popup menu.
	// used to be name

	VString 	prompt;		// prompt string for menu choice.
	VString     name  ;		// used as an ID (new)
	short		helpId;     // Help!
	short		menuId;     // also used for identification
	boolean	    bEnabled;
	boolean	    bChecked;
	VObject	   *client;		// callback client.
	method	   	selector;
	short		wKey;		// accelerator key associated w/this menu-item.
	int			idNo;		// menu item id number

#if defined (PM)
	void init(const VString &, id , method, int, int = 0); 
#else
	void init( VObject *, method , int , int = 0);
#endif

#ifndef CV_NOARCHIVER
	void	   	putTo(VArchiver& a, boolean most_derived);
#endif
#if defined(CV_WINDOWS)
	boolean   let_ole_delete : 1;
	boolean   appendOrInsertMenuDone : 1;
#endif

private:

#ifndef CV_NOARCHIVER
	integer		clientIndex;
	integer		methodIndex;
#endif

};

//------------------------------------------------------------------
inline VString&	VMenuItem::getName(){
	return name;
}
//------------------------------------------------------------------
inline	void        VMenuItem::setName(const VString& name){
	VMenuItem::name = name;
}
//------------------------------------------------------------------

inline	int			VMenuItem::getHelpId() const{
	return helpId;
}
//------------------------------------------------------------------

inline	void		VMenuItem::setHelpId(int helpId) {
	VMenuItem::helpId = helpId;
}
//------------------------------------------------------------------



// ---------------------------------------------------------------------------
inline VString& VMenuItem::getPrompt()
{
	return prompt;
}

// ---------------------------------------------------------------------------
inline void VMenuItem::setPrompt(const VString& p)
{
	prompt = p;
}

// ---------------------------------------------------------------------------
inline VMenu *VMenuItem::getMenu() const
{
	return menu;
}
// ---------------------------------------------------------------------------
inline void VMenuItem::setIdNo(int v)
{
	idNo = v;
}

// ---------------------------------------------------------------------------
inline int VMenuItem::getIdNo() const
{
	return idNo;
}

// ---------------------------------------------------------------------------
inline VObject *VMenuItem::getClient() const
{
	return client;
}

// ---------------------------------------------------------------------------
inline int VMenuItem::getAccel() const
{
	return wKey;
}

// ---------------------------------------------------------------------------
inline method VMenuItem::getMethod() const
{
	return selector;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
inline integer VMenuItem::getClientIndex() const
{
	return clientIndex;
}

// ---------------------------------------------------------------------------
inline void VMenuItem::setClientIndex(integer index)
{
	clientIndex = index;
}

// ---------------------------------------------------------------------------
inline integer VMenuItem::getMethodIndex() const
{
	return methodIndex;
}

// ---------------------------------------------------------------------------
inline void VMenuItem::setMethodIndex(integer index)
{
	methodIndex = index;
}
#endif

#if defined(CV_WINDOWS)
inline void			VMenuItem::oleDeletes(boolean f){
	let_ole_delete = f;
}
#endif
#endif // VMENUITEM_H
