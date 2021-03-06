// $Header:   Q:/views/win/vcs/stdmdiap.h_v   1.9   Mar 04 1997 16:33:06   gregm  $ 

//	stdmdiap.h
//
//	VStdMdiAppView interface
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
//	03/30/93 jmd	created
//	07/20/93 jmd	integrated david's many changes
//	07/21/93 pat	changed class to CLASS in forward declarations.
//	08/08/93 jmd	made VFrame const, removed free
//	09/08/93 dgm	added setMenuBar().
//	09/28/93 dgm	added de/installMenuAccelerators().
//	01/08/94 jmd	added help menu
//	08/16/94 dgm	Added archiver support; added operator(); added
//					do_realize to VStdMdiAppView(); added initialize().
//	10/25/94 dgm	Added menuBarIsMine data-member.
//	08/03/95 jld	removed help support not needed with new VHelp class
//	09/01/95 dss	VStyle changes.
//	01/02/97 dgm	Befriend VNotifier for MDI child/menu crash fix/hack.
// ----------------------------------------------------------------------------

#ifndef V_STDMDIAP_H
#define V_STDMDIAP_H

#include "mdiappvw.h"
#include "statusbr.h"

CLASS VClass;
CLASS VFrame;
CLASS VMdiAppView;
CLASS VMenuItem;
CLASS VNotifier;
CLASS VObject;
CLASS VPopupMenu;
CLASS VStatusBar;
CLASS VStdMdiView;
CLASS VString;
CLASS VToolBar;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VStdMdiAppViewCls;

CLASS VStdMdiAppView : public VMdiAppView {
public:
						VStdMdiAppView(int do_realize = 0);
						VStdMdiAppView(const VFrame&,
									   int tool_bar_height = 28,
									   int status_bar_height = 24,
									   const VStyle& style = StyleDefault);
						VStdMdiAppView(int x, int y, int w, int h,
									   int tool_bar_height = 28,
									   int status_bar_height = 24,
									   const VStyle& style = StyleDefault);
						VStdMdiAppView(const VString&,
						 			   const VFrame&,
									   int tool_bar_height = 28,
									   int status_bar_height = 24,
									   const VStyle& style = StyleDefault);
					   ~VStdMdiAppView();
	VClass			   *iam();

	void				operator()(const VFrame&, const VStyle&);
	void				operator()(const VFrame&, const VStyle&,
								   int tool_bar_height,
								   int status_bar_height);
	void				operator()(const VFrame&, const VStyle&, const VString&);
	void				operator()(const VFrame&, const VStyle&,
								   int tool_bar_height,
								   int status_bar_height,
								   const VString&);

public:
	VStdMdiView		   *getCurrentChild() const;
	VStdMdiView		   *getCurrChild();

public:
	virtual VMenu	   *newMenuBar();
	VMenu			   *getMenuBar() const;
	void				setMenuBar(VMenu *);
	VPopupMenu		   *newWindowMenu();

	virtual VToolBar   *newToolBar();
	VToolBar		   *getToolBar() const;
	void				setToolBar(VToolBar *);

	virtual VStatusBar *newStatusBar();
	VStatusBar		   *getStatusBar() const;
	void				setStatusBar(VStatusBar *);

	void				showStatusBar(boolean);
	void				showToolBar(boolean);

	virtual	boolean		displayPrompt(VString *msg);

public:
	virtual void		childrenActive();
	virtual void		childrenInactive();

	virtual void		childCreated(VStdMdiView *child);
	virtual void		childClosed(VStdMdiView *child, boolean deleted);
	virtual void		childActive(VStdMdiView *child);

#ifndef CV_NOARCHIVER
	virtual void		putTo(VArchiver& a);
	virtual void		getFrom(VArchiver& a);
	virtual void		getFrom(VArchiver& a, VObject *pwin,
								boolean do_realize = TRUE);
	virtual void		realize(VWindow *pwin);
#endif

protected:
	void				initialize();
	void				createBars(int tool_bar_height, int status_bar_height);
	void				updateBars();
	void				resetMenuBar(VStdMdiView *prevChild);
	void				selectWindowMenu(VPopupMenu *);
	void				showOrHideToolBar();
	void				showOrHideStatusBar();
	boolean				erased();
	void				updateMenuBarClients(VMenu *, VStdMdiView *);
	void				updateToolBarClients(VToolBar *, VStdMdiView *);
	VPopupMenu		   *findWindowMenu(VMenu *);
	VStdMdiView		   *findMdiChildControllingMenuBar(VMenu *);
	VStdMdiView		   *findMdiChildControllingToolBar(VToolBar *);
	void				clearStatusBar(VStatusBar *);
	void				installMenuAccelerators(VMenu *);
	void				deinstallMenuAccelerators(VMenu *);

#ifndef CV_NOARCHIVER
	void				putTo(VArchiver& a, boolean most_derived);
#endif

private:
	VStdMdiView		   *currChild;		// the current child window
	VMenu			   *menuBar;
	VMenu			   *currentMenuBar;
	VPopupMenu		   *currentWindowMenu;
	VToolBar		   *toolBar;
	VToolBar		   *currentToolBar;
	VStatusBar		   *statusBar;
	VStatusBar		   *currentStatusBar;
	VOrdCollect		   *windowMenus;
	VClass			   *currChildStyle;		// which menus are displayed
	int					childCount;			// number of MDI children
	int					isClosed : 1;
	int					showingToolBar : 1;
	int					showingStatusBar : 1;
	int					menuBarIsMine : 1;

private:
	friend				VStdMdiView;
	friend				VNotifier;
};

// ---------------------------------------------------------------------------
inline VMenu *VStdMdiAppView::getMenuBar() const
{
	return menuBar;
}

// ---------------------------------------------------------------------------
inline VToolBar *VStdMdiAppView::getToolBar() const
{
	return toolBar;
}

// ---------------------------------------------------------------------------
inline VStdMdiView *VStdMdiAppView::getCurrChild()
{
	return currChild;
}

inline VStdMdiView *VStdMdiAppView::getCurrentChild() const
{
	return currChild;
}

inline VStatusBar *VStdMdiAppView::getStatusBar() const
{
	return statusBar;
}

#endif // V_STDMDIAP_H
