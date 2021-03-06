// $Header:   Q:/views/win/vcs/stdmdivw.h_v   1.9   Mar 04 1997 16:33:06   gregm  $ 

//	stdmdivw.h
//
//	VStdMdiView interface
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
//	07/21/93 pat	changed class to CLASS in forward declarations
//	07/26/93 dgm	removed realize().
//	08/08/93 jmd	made VFrame const
//  08/09/93 jmd	removed free
//  10/12/93 jmd	preened
//	01/08/94 jmd	added help menu
//	08/16/94 dgm	Added archiver support; added operator();
//					add do_realize to VMdiAppView(); added initialize().
//					Set mainWin to parent during construction.
//	08/03/95 jld	removed help support not needed with new VHelp class
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#ifndef VSTDMDIVW_H
#define VSTDMDIVW_H

#include "mdiview.h"
#include "stdmdiap.h"

CLASS VStatusBar;
CLASS VToolBar;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VStdMdiViewCls;

CLASS VStdMdiView : public VMdiView {

public:
						VStdMdiView();
						VStdMdiView(const VFrame& f, VStdMdiAppView *win,
								 const VStyle& style = StyleDefault);
						VStdMdiView(const VString&,
					 			 const VFrame& f, VStdMdiAppView *win,
								 const VStyle& style = StyleDefault);
						~VStdMdiView();
	VClass			*iam();

	void				operator()(const char *,
								const VFrame&,
								VStdMdiAppView *, const VStyle& style, 
								char *title);
	void				operator()(const char *,
								const VFrame&,
								VStdMdiAppView *, const VStyle& style, 
								char *title, const VString&);

	virtual	void	classActive();
	virtual	void	classInactive();
	virtual	void	instanceActive();
	virtual	void	instanceInactive();

	VStdMdiAppView	*getMainWin();
	VStdMdiAppView	*getAppView();

	VMenu				*newMenuBar();
	VMenu				*getMenuBar();
	void				setMenuBar(VMenu *);
	VPopupMenu		*newWindowMenu();

	VToolBar			*newToolBar();
	void				setToolBar(VToolBar *);
	VToolBar			*getToolBar();

	VStatusBar		*newStatusBar();
	void				setStatusBar(VStatusBar *);
	VStatusBar		*getStatusBar();

	boolean			deactivate(VView *m);
	boolean			activate(VView *m);

#ifndef CV_NOARCHIVER
	virtual void	putTo(VArchiver& a);
	virtual void	getFrom(VArchiver& a);
	virtual void	getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:
	boolean			close();

#ifndef CV_NOARCHIVER
	void				putTo(VArchiver& a, boolean most_derived);
#endif

private:
	void				initialize();

private:
	VStdMdiAppView	*mainWin;
	VPopupMenu		*availableA;
	VMenu				*menuBar;
	VToolBar			*toolBar;
	VStatusBar		*statusBar;
	boolean			isClosed;

private:
	friend			VStdMdiAppView;
};

// ---------------------------------------------------------------------------
inline VStdMdiAppView *VStdMdiView::getAppView()
{
	return mainWin ? (mainWin->isClosed ? 0 : mainWin) : 0;
}

// ---------------------------------------------------------------------------
inline VStdMdiAppView *VStdMdiView::getMainWin()
{
	return mainWin ? (mainWin->isClosed ? 0 : mainWin) : 0;
}

// ---------------------------------------------------------------------------
inline void VStdMdiView::setStatusBar(VStatusBar *status_bar)
{
	statusBar = status_bar;
	if (mainWin != 0) {
		mainWin->setStatusBar(statusBar);
	}
}

// ---------------------------------------------------------------------------
inline VStatusBar *VStdMdiView::getStatusBar()
{
	return statusBar;
}

// ---------------------------------------------------------------------------
inline VStatusBar *VStdMdiView::newStatusBar()
{
	if (mainWin != 0) {
		return statusBar = mainWin->newStatusBar();
	}
	else {
		return 0;
	}
}

// ---------------------------------------------------------------------------
inline void VStdMdiView::setMenuBar(VMenu *menu_bar)
{
	menuBar = menu_bar;
	if (mainWin != 0) {
		if (menuBar->hmenu() == 0) {
			menuBar->realize(this);
		}
		mainWin->setMenuBar(menuBar);
	}
}

// ---------------------------------------------------------------------------
inline VMenu *VStdMdiView::getMenuBar()
{
	return menuBar;
}

// ---------------------------------------------------------------------------
inline VMenu *VStdMdiView::newMenuBar()
{
	if (mainWin != 0) {
		return menuBar = mainWin->newMenuBar();
	}
	else {
		return 0;
	}
}

// ---------------------------------------------------------------------------
inline void VStdMdiView::setToolBar(VToolBar *tool_bar)
{
	toolBar = tool_bar;
	if (mainWin != 0) {
		mainWin->setToolBar(toolBar);
	}
}

// ---------------------------------------------------------------------------
inline VToolBar *VStdMdiView::getToolBar()
{
	return toolBar;
}

// ---------------------------------------------------------------------------
inline VToolBar *VStdMdiView::newToolBar()
{
	if (mainWin != 0) {
		return toolBar = mainWin->newToolBar();
	}
	else {
		return 0;
	}
}

// ---------------------------------------------------------------------------
inline VPopupMenu *VStdMdiView::newWindowMenu()
{
	if (mainWin != 0) {
		return mainWin->newWindowMenu();
	}
	else {
		return 0;
	}
}

#endif // VSTDMDIVW_H
