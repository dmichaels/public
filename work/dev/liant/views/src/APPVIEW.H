// $Header:   Q:/views/win/vcs/appview.h_v   1.19   Mar 04 1997 16:31:48   gregm  $ 

//	appview.h
//
//	VAppView interface
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
//	04/09/93 pmcm	changed id to VObject *
//	05/14/93 pmcm	preened, added constructors that take name argument
//	05/20/93 jmd	added menuHighlighted()
//	07/31/93 jmd	modified menu prompt logic
//	08/02/93 pmcm	added addMenu(); removeMenu()
//	08/08/93 jmd	made VFrame const
//	08/09/93 jmd	made addMenu(); removeMenu() take pointers
//	08/09/93 jmd	removed free
//	08/16/93 jmd	made name const VString
//	08/18/93 jmd	added closeApp
//	12/29/93 jmd	added help support (first cut)
//	08/04/94 dgm	Added archiver support; straightened out styles;
//					added operator(); added initialize(); added setMenuBar().
//	08/31/94 dss	Added isTopLevel().
//	10/11/94 dss	Removed help functions.
//	11/04/94 dss	Differentiate appview creation in and out of start()..
//	03/16/95 dss	added destroyed().
//	09/01/95 dss	VStyle changes.
//	09/28/95 dgm	Added constrainMinimumSize() and resizeClientArea().
//	10/18/95 dgm	Removed constrainMinimumSize() and resizeClientArea().
//					Added getNonClientAreaSize().
//	11/02/95 dgm	Changed getNonClientAreaSize() to getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	01/09/96 dss	VComponent name change.
//	01/29/96 dgm	Changed isTopLevel() to be a const function.
//	02/15/96 glm	removed destroyed()
//	02/16/96 pkt	Changed 'setAppIcon' and 'rscMenu' to take a const char*.
//	04/19/96 glm	removed v1.1 code/preened
// ---------------------------------------------------------------------------

#ifndef VAPPVIEW_H
#define VAPPVIEW_H

#include "view.h"
#include "menu.h"
#include "popupmen.h"
#include "icon.h"

CLASS VNotifier;
CLASS VComponentAppView;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VAppViewCls;

//	Introduction
//
//	The VAppView class is a subclass of `VView` that is designed
//	to be the top level view object for an application.  Typically, an
//	application will have only one instance of a subclass of VAppView. 
//	VAppView is the only subclass of the `VWindow` class that supports a
//	'menu bar'.
 
CLASS VAppView : public VView {

public:

					VAppView(int do_realize = 1);
					VAppView(const VString& name);
					VAppView(const VFrame&, const VStyle& style = StyleDefault);
					VAppView(const VString&, const VFrame&, 
							 const VStyle& style = StyleDefault);

					~VAppView();

	VClass 		   *iam();

	void		operator()(const VFrame&, const VStyle&);
	void		operator()(const VFrame&, VWindow *, const VStyle&);
	void		operator()(const VFrame&, const VStyle&, const VString&);
	void		operator()(const VFrame&, VWindow *, const VStyle&, const VString&);
	static VStyle&		getClassDefaultStyle();
	virtual VStyle&	getDefaultStyle() const;

	//	Application Icon

	boolean 		setAppIcon(const char *s);
	boolean 		setAppIcon(VIcon *ic);
	VIcon 		   *getAppIcon();
	boolean 		isIconic();

	//	Menu Bar Operations

	void 			drawMenu();
	VMenu 		   *getMenu();
	virtual void 	addPopup(VObject *m);
	void 			insertPopup(VObject *m, int index);
	void 			removePopup(VObject *m);
	void 			replacePopup(VObject *m, int index);
	void 			refreshMenu(boolean b);

	void 			rscMenu(	const char *name, 
								int    	   *ids, 
								method 	   *mthd, 
								VObject	   *client);
	void			addMenu(VMenu *m);
	void			removeMenu(VMenu *m);
	void			setMenuBar(VMenu *);

	//	Application Termination 

	boolean 		closeApp(VMenuItem *m);		// callback
	boolean 		close();
	boolean 		quit();

	//	Window Visibility
	void 			show();
	void 			show(const VStyle&);

	//	Window Events

	virtual boolean menuSelected(VPopupMenu *m);
	virtual boolean iconized(int w, int h);
	boolean 		nextFocus();
	boolean 		prevFocus();

	virtual	boolean	displayPrompt(VString *msg);

	// Window Attributes
	boolean isTopLevel() const;

	// Modality
	void		modal();
	void		endModal();

	// Embedded Objects
	VComponentAppView		*getComponentThis();

	// Geometry management.

    virtual void	getSystemBorderSize(int *, int *) const;

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:
	VMenuItem	   *itemOf(unsigned n);
	VMenu	 	   *menuOf(HMENU m);
	void	 		getInfo(VWinInfo& appinfo);
    UINT		    getCtlColor();
	void			initialize();
	boolean			wasCreatedInStart();

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

	VIcon 	   *appIcon;		// application icon
	VMenu 	   *menu; 			// menubar
	boolean 	menuUpdate;		// update menu when popup is added/removed?

private:
	friend		VMenu;
	friend		VNotifier;

	boolean		inStartCreation;		// TRUE if created in VNotifier::start().

};

//---------------------------------------------------------------------------
//
inline VComponentAppView *VAppView::getComponentThis()
{
	return (VComponentAppView *) VView::getComponentThis();
}

// ---------------------------------------------------------------------------
//
inline boolean VAppView::wasCreatedInStart()
{
	return inStartCreation;
}

// ---------------------------------------------------------------------------
//
inline VIcon *VAppView::getAppIcon()
{
	return appIcon;
}

// ---------------------------------------------------------------------------
//
inline VMenu *VAppView::getMenu() 
{ 
	return menu; 
}

// ---------------------------------------------------------------------------
//
inline void VAppView::show()
{ 
	VWindow::show(); 
}

// ---------------------------------------------------------------------------
//	protected:
//
inline VMenuItem *VAppView::itemOf(unsigned n) 
{ 
	return ((menu != 0) ? menu->itemOf(n) : 0);
}

// ---------------------------------------------------------------------------
//	protected:
//
inline VMenu *VAppView::menuOf(HMENU m) 
{ 
	return ((menu != 0) ? menu->menuOf(m) : 0);
}

// ---------------------------------------------------------------------------

#endif	// VAPPVIEW_H
