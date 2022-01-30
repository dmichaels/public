// $Header:   Q:/views/win/vcs/stdmdiap.cpv   1.13   Apr 02 1997 13:12:58   gregm  $ 

//	stdmdiap.cpp
//
//	VStdMdiAppView implementation
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
//	08/08/93 jmd	made VFrame const, removed free()
//	09/08/93 dgm	added setMenu().
//	09/22/93 dgm	cleaned up to use realize() correctly.
//	09/28/93 dgm	fixed installation/deinstallion of menu accelerators.
//	10/07/93 dgm	added menu mnemonics for "ToolBar" and "StatusBar".
//	10/15/93 dgm	fixed tile menu-item to call "tile" rather than "cascade".
//					added resetMenuBarClients() & resetToolBarClients().
//	10/16/93 dgm	fixed resetMenuBarClients() call in childClosed
//	01/08/94 dgm	added check in setMenuBar() for null currChild.
//	01/08/94 jmd	added help menu
//	01/31/94 dgm	fix in childClosed()
//	03/26/94 pat	CV_WIN32: updated WM_MDISETMENU handling
//	08/16/94 dgm	Added archiver support; added operator();
//					add do_realize to VMdiAppView(); added initialize().
//	08/25/94 dgm	Fix in realize().
//	09/29/94 jld	plugged mem leak by deleting menuBar in setMenuBar().
//	10/11/94 dss	help calls moved to notifier.
//	10/25/94 dgm	Fixes in getFrom().
//	11/11/94 dss	Archiver tags macros.
//	08/03/95 jld	removed help support not needed with new VHelp class
//	09/01/95 dss	VStyle changes.
//	01/02/97 dgm	Fix in childrenInactive() to get rid of the menu-bar if
//					we don't have one.
//	04/02/97 glm	reorder if/else in childrenInactive() to use menuBar
// ----------------------------------------------------------------------------

#include "stdmdiap.h"
#include "toolbar.h"
#include "statusbr.h"
#include "popupmen.h"
#include "menuitem.h"
#include "attach.h"
#include "textbox.h"
#include "ordcllct.h"
#include "iterator.h"
#include "stdmdivw.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

// ---------------------------------------------------------------------------
//
defineClass(VStdMdiAppView, VMdiAppView)
defineArchiveRevision(VStdMdiAppView, 0)

// ---------------------------------------------------------------------------
VStdMdiAppView::VStdMdiAppView(int do_realize)
	: VMdiAppView(0)
{
	initialize();
	if (do_realize) {
		(*this)(VFrame(0.05F, 0.05F, 0.85F, 0.85F),
				StyleDefault, 28, 24, "Main");
	}
}

// ---------------------------------------------------------------------------
VStdMdiAppView::VStdMdiAppView(const VFrame &frame,
							   int toolbar_height,
							   int statusbar_height,
							   const VStyle& style)
			  : VMdiAppView(frame,
							VFrame(0, 0, 1.0F, 0), style)
{
	initialize();
	createBars(toolbar_height, statusbar_height);
}

// ---------------------------------------------------------------------------
VStdMdiAppView::VStdMdiAppView(int x, int y, int w, int h,
							   int toolbar_height,
							   int statusbar_height,
							   const VStyle& style)
			  : VMdiAppView(VFrame(x, y, w, h),
							VFrame(0, 0, 1.0F, 0), style)
{
	initialize();
	createBars(toolbar_height, statusbar_height);
}

// ---------------------------------------------------------------------------
VStdMdiAppView::VStdMdiAppView(const VString& name,
							   const VFrame &frame,
							   int toolbar_height,
							   int statusbar_height,
							   const VStyle& style)
			  : VMdiAppView(name, frame, VFrame(0, 0, 1.0F, 0), style)
{
	initialize();
	createBars(toolbar_height, statusbar_height);
}

// ---------------------------------------------------------------------------
//
void VStdMdiAppView::operator()(const VFrame& frame, const VStyle& style)
{
	VMdiAppView::operator()(frame, VFrame(0, 0, 1.0F, 0), style);
	createBars(0, 0);
}

// ---------------------------------------------------------------------------
//
void VStdMdiAppView::operator()(const VFrame& frame, const VStyle& style,
								int tool_bar_height,
								int status_bar_height)
{
	VMdiAppView::operator()(frame, VFrame(0, 0, 1.0F, 0), style);
	createBars(tool_bar_height, status_bar_height);
}

// ---------------------------------------------------------------------------
//
void VStdMdiAppView::operator()(const VFrame& frame, const VStyle& style,
								const VString& name)
{
	VMdiAppView::operator()(frame, VFrame(0, 0, 1.0F, 0), style, name);
	createBars(0, 0);
}

// ---------------------------------------------------------------------------
//
void VStdMdiAppView::operator()(const VFrame& frame, const VStyle& style,
								int tool_bar_height,
								int status_bar_height,
								const VString& name)
{
	VMdiAppView::operator()(frame, VFrame(0, 0, 1.0F, 0), style, name);
	createBars(tool_bar_height, status_bar_height);
}

// ---------------------------------------------------------------------------
VStdMdiAppView::~VStdMdiAppView()
{
	if (menuBarIsMine) {
		this->menu = menuBar;
	}
	else {
		this->menu = 0;
	}
	delete windowMenus;
	isClosed = 1;
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::initialize()
{
	isClosed = 0;
	currChild = 0;
	currChildStyle = 0;
	childCount = 0;
	menuBar = 0;
	currentMenuBar = 0;
	toolBar = 0;
	currentToolBar = 0;
	statusBar = 0;
	currentStatusBar = 0;
	windowMenus = 0;
	currentWindowMenu = 0;
	showingToolBar = 1;
	showingStatusBar = 1;
	menuBarIsMine = 0;
}

// ---------------------------------------------------------------------------
// Create tool and status bars
//
void VStdMdiAppView::createBars(int toolbar_height, int statusbar_height)
{
	if (toolbar_height > 0) {
		toolBar = new VToolBar(VFrame(0, 0, 1.0F, toolbar_height),
							   this, FALSE);
		if (showingToolBar) {
			this->attachWin(&client, VAttach::Top,
							VAttach::Sibling, 0, toolBar);
		}
		else {
			toolBar->hide();
		}
		currentToolBar = toolBar;
	}
	else {
		this->attachWin(&client, VAttach::Top, VAttach::Parent);
	}

	if (statusbar_height > 0) {
		statusBar = new VStatusBar(VFrame(0, 0, 1.0F, statusbar_height),
									  this);
		this->attachWin(statusBar, VAttach::Bottom, VAttach::Parent);
		clearStatusBar(statusBar);
		currentStatusBar = statusBar;
		if (showingStatusBar) {
			this->attachWin(&client, VAttach::Bottom,
							VAttach::Sibling, 0, statusBar);
		}
		else {
			statusBar->hide();
		}
	}
	else {
		this->attachWin(&client, VAttach::Bottom, VAttach::Parent);
	}

	menuBar = new VMenu(TRUE);
	menuBarIsMine = 1;

	client.frameChildren();
	this->frameChildren();
}

// ---------------------------------------------------------------------------

VMenu *VStdMdiAppView::newMenuBar()
{
	return new VMenu(TRUE);
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::setMenuBar(VMenu *menu_bar)
{
	if (menu_bar == 0) {
		return;
	}
	if ((menu_bar->hmenu() == 0)) {
		//
		// Special case for menus which haven't been realized yet,
		// i.e. which were read from a Portable Views Resource File (VRF).
		//
		if (!menu_bar->realize()) {
			return;
		}
	}

	VMenu *app_menu = this->getMenu();

	if (menu_bar == app_menu) {
		if ((currChild != 0) && (menu_bar == currChild->getMenuBar())) {
			VStdMdiView *controlling_client =
				findMdiChildControllingMenuBar(menu_bar);
			if (controlling_client != 0) {
				updateMenuBarClients(menu_bar, controlling_client);
			}
		}
		return;
	}
	else if (app_menu == 0) {
		currentWindowMenu = findWindowMenu(menu_bar);
		this->menu = menu_bar;
		menu_bar->setApp(this);
		SetMenu(hWnd, menu_bar->hmenu());
		menuBar = this->getMenu();
		menuBarIsMine = 0;
	}
	else if (client.hwnd() != 0) {
		this->menu = menu_bar;	// this->setMenu(menu_bar);
		menu_bar->setApp(this);
		HMENU window_hmenu;
		if ((currentWindowMenu = findWindowMenu(menu_bar)) != 0) {
			window_hmenu = currentWindowMenu->hmenu();
		}
		else {
			window_hmenu = 0;
		}
		VStdMdiView *controlling_client =
			findMdiChildControllingMenuBar(menu_bar);
		if (controlling_client != 0) {
			updateMenuBarClients(menu_bar, controlling_client);
		}
		deinstallMenuAccelerators(currentMenuBar);
#if defined(CV_WIN32)
		SendMessage(client.hwnd(), WM_MDISETMENU, 
					(WPARAM) menu_bar->hmenu(), (LPARAM) window_hmenu);
#else
		SendMessage(client.hwnd(), WM_MDISETMENU, 0,
					MAKELONG(menu_bar->hmenu(), window_hmenu));
#endif
		installMenuAccelerators(menu_bar);
		DrawMenuBar(hWnd);
	}
	currentMenuBar = menu_bar;
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::installMenuAccelerators(VMenu *menu_bar)
{
	if (menu_bar == 0) {
		return;
	}
	DO (menu_bar->mitems(), VObject, menu_item)
		if (menu_item->isA(VMenuItemCls)) {
			VMenuItem *mi = (VMenuItem *)menu_item;
			if (mi->wKey != 0) {
				notifier->setAccel(mi->wKey, methodOf(VMenuItem, selected), mi);
			}
		}
		else if (menu_item->isA(VPopupMenuCls)) {
			VPopupMenu *pm = (VPopupMenu *)menu_item;
			installMenuAccelerators(pm);
		}
	END
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::deinstallMenuAccelerators(VMenu *menu_bar)
{
	if (menu_bar == 0) {
		return;
	}
	DO (menu_bar->mitems(), VObject, menu_item)
		if (menu_item->isA(VMenuItemCls)) {
			VMenuItem *mi = (VMenuItem *)menu_item;
			if (mi->wKey != 0) {
				notifier->unsetAccel(mi->wKey);
			}
		}
		else if (menu_item->isA(VPopupMenuCls)) {
			VPopupMenu *pm = (VPopupMenu *)menu_item;
			deinstallMenuAccelerators(pm);
		}
	END
}

// ---------------------------------------------------------------------------
VPopupMenu *VStdMdiAppView::findWindowMenu(VMenu *menu_bar)
{
	if ((menu_bar == 0) || (windowMenus == 0)) {
		return 0;
	}
	DO (menu_bar->mitems(), VObject, menu_item)
		if (!menu_item->isA(VPopupMenuCls)) {
			continue;
		}
		VPopupMenu *window_menu = (VPopupMenu *)
								  (windowMenus->findSame(menu_item));
		if (window_menu != 0) {
			return window_menu;
		}
	END
	return 0;
}

// ---------------------------------------------------------------------------
// Update the clients for each and every popup-menu and menu-item (recursively)
// in the entire menu-bar tree of the given VMenu to refer to the current MDI
// child (this->currChild), if necessary.
//
void VStdMdiAppView::updateMenuBarClients
	(VMenu *menu_bar, VStdMdiView *controlling_client)
{
	if ((menu_bar == 0) || (menu_bar == menuBar)) {
		return;
	}
	if (controlling_client == 0) {
		return;
	}
	DO (menu_bar->mitems(), VObject, menu_item)
		if (menu_item->isA(VMenuItemCls)) {
			VMenuItem *mi = (VMenuItem *)menu_item;
			if (mi->getClient() == controlling_client) {
				mi->setClient(currChild);
			}
		}
		else if (menu_item->isA(VPopupMenuCls)) {
			VPopupMenu *pm = (VPopupMenu *)menu_item;
			if (pm->getClient() == controlling_client) {
				pm->setClient(currChild);
			}
			updateMenuBarClients(pm, controlling_client);
		}
	END
}

// ---------------------------------------------------------------------------
static void resetMenuBarClients
	(VStdMdiAppView *thisthis,
	 VMenu *menu_bar, VStdMdiView *current_client, VStdMdiView *new_client)
{
	VClass *current_client_style;
	if ((menu_bar == 0) ||
		(current_client == 0) ||
		((current_client_style = current_client->iam()) == 0)) {
		return;
	}
	if (new_client == 0) {
		VIterator child_iterator(thisthis->clientOf()->childrenOf());
		for (VWindow *child ; (child = (VWindow *)child_iterator++) != 0 ; ) {
			if ((child != current_client) &&
				(child->iam() == current_client_style)) {
				new_client = (VStdMdiView *)child;
				break;
			}
		}
		if (new_client == 0) {
			return;
		}
	}
	DO (menu_bar->mitems(), VObject, menu_item)
		if (menu_item->isA(VMenuItemCls)) {
			VMenuItem *mi = (VMenuItem *)menu_item;
			if (mi->getClient() == current_client) {
				mi->setClient(new_client);
			}
		}
		else if (menu_item->isA(VPopupMenuCls)) {
			VPopupMenu *pm = (VPopupMenu *)menu_item;
			if (pm->getClient() == current_client) {
				pm->setClient(new_client);
			}
			resetMenuBarClients(thisthis, pm, current_client, new_client);
		}
	END
}

// ---------------------------------------------------------------------------
// Return a pointer to the MDI child which is currently controlling the
// menu-bar represented by the given VMenu pointer.  Controlling a menu-bar,
// for our purposes, means that an MDI child has the specified menu-bar as
// its menu-bar (i.e. was set via VStdMdiView::setMenuBar()), and there is
// at least one client somewhere within the menu-bar, i.e. within any of its
// popup-menus and menu-items (recursively), which is that MDI child itself.
// This is needed so that we can find out exactly which clients within a
// menu-bar to update when a new MDI child is activated.
//
VStdMdiView *VStdMdiAppView::findMdiChildControllingMenuBar(VMenu *menu_bar)
{
	if ((menu_bar == 0) ||
		(menu_bar->mitems().count() <= 0) ||
	    (client.childrenOf() == 0) ||
	    (client.childrenOf()->count() <= 1)) {
		return 0;
	}
	VIterator child_iterator(client.childrenOf());
	for (VWindow *child ; (child = (VWindow *)child_iterator++) != 0 ; ) {
		if (!child->isA(VStdMdiViewCls) ||
		    (((VStdMdiView *)child)->getMenuBar() != menu_bar)) {
			continue;
		}
		DO (menu_bar->mitems(), VObject, menu_item)
			if (menu_item->isA(VMenuItemCls)) {
				VMenuItem *mi = (VMenuItem *)menu_item;
				if (mi->getClient() == child) {
					return (VStdMdiView *)child;
				}
			}
			else if (menu_item->isA(VPopupMenuCls)) {
				VPopupMenu *pm = (VPopupMenu *)menu_item;
				VStdMdiView *c;
				if (pm->getClient() == child) {
					return (VStdMdiView *)child;
				}
				else if ((c = findMdiChildControllingMenuBar(pm)) != 0) {
					return c;
				}
			}
		END
	}
	return 0;
}

// ---------------------------------------------------------------------------
VPopupMenu *VStdMdiAppView::newWindowMenu()
{
	VPopupMenu *popup;

	popup = new VPopupMenu
			    (VMenuData("&Window",
			   			   "Select child window or configure main window",
						   methodOf(VStdMdiAppView, selectWindowMenu),
						   0));
	popup->addMenuItem
		(VMenuData("&Cascade\tShift+F5",
				   "Arrange child windows in a cascading fashion",
				   methodOf(VMdiClient, cascade),
				   &client,
				   1,
				   K_Mod_Shift | K_F5));
	popup->addMenuItem
		(VMenuData("&Tile\tShift+F4",
				   "Arrange child windows in a tiled fashion",
				   methodOf(VMdiClient, tile),
				   &client,
				   2,
				   K_Mod_Shift | K_F4));
	popup->addMenuItem
		(VMenuData("&Arrange Icons",
				   "Arrange iconized child windows",
				   methodOf(VMdiClient, arrange),
				   &client,
				   3,
				   0));
	popup->addMenuItem
		(VMenuData());
	popup->addMenuItem
		(VMenuData("Tool&Bar",
				   "Hide/show the tool bar",
				   methodOf(VStdMdiAppView, showOrHideToolBar),
				   this,
				   4,
				   0));
	popup->addMenuItem
		(VMenuData("&StatusBar",
				   "Hide/show the status bar",
				   methodOf(VStdMdiAppView, showOrHideStatusBar),
				   this,
				   5,
				   0));

	popup->uponSelect(this, methodOf(VStdMdiAppView, selectWindowMenu));

	if (windowMenus == 0) {
		windowMenus = new VOrdCollect;
	}
	windowMenus->add(popup);

	return popup;
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::selectWindowMenu(VPopupMenu *window_menu)
{
	if (currentWindowMenu != 0) {
		currentWindowMenu->
			checkMenuItem(4, (toolBar != 0) && showingToolBar);
		currentWindowMenu->
			checkMenuItem(5, (statusBar != 0) && showingStatusBar);
		currentWindowMenu->enableMenuItem(4, toolBar != 0);
		currentWindowMenu->enableMenuItem(5, statusBar != 0);
	}
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::showOrHideToolBar()
{
	showToolBar(!showingToolBar);
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::showOrHideStatusBar()
{
	showStatusBar(!showingStatusBar);
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::showToolBar(boolean b)
{
	if (b) {
		if (!showingToolBar) {
			if (currentToolBar != 0) {
				this->attachWin(&client, VAttach::Top,
								VAttach::Sibling, 0, currentToolBar);
				this->frameChildren();
				currentToolBar->bringToTop();
				currentToolBar->show();
			}
			showingToolBar = 1;
		}
	}
	else if (showingToolBar) {
		if (toolBar != 0) {
			this->attachWin(&client, VAttach::Top, VAttach::Parent);
			this->frameChildren();
			toolBar->hide();
			if (client.childrenOf() != 0) {
				VIterator child_iterator(client.childrenOf());
				VWindow *child;
				while ((child = (VWindow *)child_iterator++) != 0) {
					if (child->isA(VStdMdiViewCls)) {
						VToolBar *tool_bar = ((VStdMdiView *)child)->
											   getToolBar();
						if ((tool_bar != 0) && (tool_bar != toolBar)) {
							tool_bar->hide();
						}
					}
				}
			}
			client.update();
		}
		showingToolBar = 0;
	}
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::showStatusBar(boolean b)
{
	if (b) {
		if (!showingStatusBar) {
			if (currentStatusBar != 0) {
				this->attachWin(&client, VAttach::Bottom,
								VAttach::Sibling, 0, currentStatusBar);
				this->frameChildren();
				currentStatusBar->bringToTop();
				currentStatusBar->show();
			}
			showingStatusBar = 1;
		}
	}
	else if (showingStatusBar) {
		if (statusBar != 0) {
			this->attachWin(&client, VAttach::Bottom, VAttach::Parent);
			this->frameChildren();
			statusBar->hide();
			if (client.childrenOf() != 0) {
				VIterator child_iterator(client.childrenOf());
				VWindow *child;
				while ((child = (VWindow *)child_iterator++) != 0) {
					if (child->isA(VStdMdiViewCls)) {
						VStatusBar *status_bar = ((VStdMdiView *)child)->
												   getStatusBar();
						if ((status_bar != 0) && (status_bar != statusBar)) {
							status_bar->hide();
						}
					}
				}
			}
			client.update();
		}
		showingStatusBar = 0;
	}
}

// ---------------------------------------------------------------------------
VToolBar *VStdMdiAppView::newToolBar()
{
	if (toolBar == 0) {
		return 0;
	}
	int w, h;
	toolBar->sizeOfWin(&w, &h);
	VToolBar *tool_bar = new VToolBar(VFrame(0, 0, 1.0F, h), this);
	tool_bar->hide();
	return tool_bar;
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::setToolBar(VToolBar *tool_bar)
{
	if (tool_bar == 0) {
		if ((tool_bar = toolBar) == 0) {
			return;
		}
	}
	if (showingToolBar) {
		if ((tool_bar != currentToolBar) || currentToolBar->isHidden()) {
			this->attachWin(&client, VAttach::Top,
							VAttach::Sibling, 0, tool_bar);
			// ToDo: only do this if the tool-bar is a different height.
			this->frameChildren();
			tool_bar->bringToTop();
			tool_bar->show();
		}
		updateToolBarClients
			(tool_bar, findMdiChildControllingToolBar(tool_bar));
	}
	else {
		if ((tool_bar != currentToolBar) || !currentToolBar->isHidden()) {
			tool_bar->hide();
		}
	}
	currentToolBar = tool_bar;
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::updateToolBarClients
	(VToolBar *tool_bar, VStdMdiView *controlling_client)
{
	if ((tool_bar == 0) ||
		(tool_bar == toolBar) ||
		(tool_bar->childrenOf() == 0) ||
		(tool_bar->childrenOf()->count() <= 0) ||
		(controlling_client == 0)) {
		return;
	}
	DO (*(tool_bar->childrenOf()), VWindow, control_child)
		if (!control_child->isA(VControlCls)) {
			continue;
		}
		if (((VControl *)control_child)->getClient() == controlling_client) {
			((VControl *)control_child)->setClient(currChild);
		}
	END
}

// ---------------------------------------------------------------------------
static void resetToolBarClients
	(VStdMdiAppView *thisthis,
	 VToolBar *tool_bar, VStdMdiView *current_client, VStdMdiView *new_client)
{
	VClass *current_client_style;
	if ((tool_bar == 0) ||
		(tool_bar->childrenOf() == 0) ||
		(tool_bar->childrenOf()->count() <= 0) ||
		(current_client == 0) ||
		((current_client_style = current_client->iam()) == 0)) {
		return;
	}
	if (new_client == 0) {
		VIterator child_iterator(thisthis->clientOf()->childrenOf());
		for (VWindow *child ; (child = (VWindow *)child_iterator++) != 0 ; ) {
			if ((child != current_client) &&
				(child->iam() == current_client_style)) {
				new_client = (VStdMdiView *)child;
				break;
			}
		}
		if (new_client == 0) {
			return;
		}
	}
	DO (*(tool_bar->childrenOf()), VWindow, control_child)
		if (!control_child->isA(VControlCls)) {
			continue;
		}
		if (((VControl *)control_child)->getClient() == current_client) {
			((VControl *)control_child)->setClient(new_client);
		}
	END
}

// ---------------------------------------------------------------------------
// Return a pointer to the MDI child which is currently controlling the
// tool-bar represented by the given VToolBar pointer.  Controlling a
// tool-bar, for our purposes, means that an MDI child has the specified
// tool-bar as its tool-bar (i.e. was set via VStdMdiView::setToolBar()),
// and there is at least one control in the tool-bar which has as its client,
// that MDI child itself.  This is needed so that we can find out exactly
// which clients in a tool-bar to update when a new MDI child is activated.
//
VStdMdiView *VStdMdiAppView::findMdiChildControllingToolBar(VToolBar *tool_bar)
{
	if ((tool_bar == 0) ||
		(tool_bar->childrenOf() == 0) ||
		(tool_bar->childrenOf()->count() <= 0) ||
		(client.childrenOf() == 0) ||
		(client.childrenOf()->count() <= 1)) {
		return 0;
	}
	VIterator child_iterator(client.childrenOf());
	for (VWindow *child ; (child = (VWindow *)child_iterator++) != 0 ; ) {
		if (!child->isA(VStdMdiViewCls) ||
		    (((VStdMdiView *)child)->getToolBar() != tool_bar)) {
			continue;
		}
		DO (*(tool_bar->childrenOf()), VWindow, control_child)
			if (!control_child->isA(VControlCls)) {
				continue;
			}
			if (((VControl *)control_child)->getClient() == child) {
				return (VStdMdiView *)child;
			}
		END
	}
	return 0;
}

// ---------------------------------------------------------------------------
VStatusBar *VStdMdiAppView::newStatusBar()
{
	if (statusBar == 0) {
		return 0;
	}
	int w, h;
	statusBar->sizeOfWin(&w, &h);
	VStatusBar *status_bar = new VStatusBar(VFrame(0, 0, 1.0F, h), this);
	status_bar->hide();
	this->attachWin(status_bar, VAttach::Bottom, VAttach::Parent);
	return status_bar;
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::setStatusBar(VStatusBar *status_bar)
{
	if (status_bar == 0) {
		if ((status_bar = statusBar) == 0) {
			return;
		}
	}
	if (showingStatusBar) {
		if ((status_bar != currentStatusBar) ||
			 currentStatusBar->isHidden()) {
			this->attachWin(&client, VAttach::Bottom,
							VAttach::Sibling, 0, status_bar);
			// ToDo: only do this if the status-bar is a different height.
			this->frameChildren();
			status_bar->bringToTop();
			status_bar->show();
		}
	}
	else {
		if ((status_bar != currentStatusBar) ||
			 !currentStatusBar->isHidden()) {
			status_bar->hide();
		}
	}
	currentStatusBar = status_bar;
}

// ---------------------------------------------------------------------------
// Update the menu bar for the current child
//
void VStdMdiAppView::updateBars()
{
	if (isClosing()) {
		return;
	}

	if (currChild == 0) {
		if (currChildStyle != 0) {
			resetMenuBar(0);
		}
	}
	else {
		// Show the status-bar associated with this MDI
		// child; if it is just the default one, i.e.
		// the MDI child did not create its own, then
		// initialize its contents to the string "Ready".

		// notify currChild
		if (currChild->iam() != currChildStyle) {
			currChildStyle = currChild->iam();
			currChild->classActive();
		}

		setStatusBar(currChild->getStatusBar());
		if (currentStatusBar == statusBar) {
			clearStatusBar(statusBar);
		}
		setToolBar(currChild->getToolBar());
		setMenuBar(currChild->getMenuBar());

		// notify child window
		currChild->instanceActive();
	}
}

// ---------------------------------------------------------------------------
// reset the menu to basic state, prevChild is most recent currChild
//
void VStdMdiAppView::resetMenuBar(VStdMdiView *prevChild)
{
	if (prevChild) {
		prevChild->classInactive();
	}
	currChildStyle = 0;
}

// ---------------------------------------------------------------------------
// called by child StdMDI windows when they are created
//
void VStdMdiAppView::childCreated(VStdMdiView *child)
{
	if (childCount == 0) {
		childrenActive();
	}
	childCount++;
}

// ---------------------------------------------------------------------------
// called by child StdMDI windows when they are closed
// deleted TRUE means that child win destructor has been called
//
void VStdMdiAppView::childClosed(VStdMdiView *child, boolean deleted)
{
	// test if we're closed to prevent calling virtual functions from
	// destructor
	if (!isClosing()) {
		if ((child != 0) && (childCount > 1)) {
			resetMenuBarClients(this, child->getMenuBar(), child, 0);
			resetToolBarClients(this, child->getToolBar(), child, 0);
		}

		if (child == currChild && (childCount <= 1 || deleted)) {

			// assumes that a new window becomes
			// active BEFORE this one is deleted (??)

			if (currChild != 0) {
				currChild->instanceInactive();
			}
			resetMenuBar(currChild);
			currChild = 0;
		}

		childCount--;

		if (childCount == 0) {
			childrenInactive();
		}
	}
}

// ---------------------------------------------------------------------------
// called by child StdMDI windows when they become active
//
void VStdMdiAppView::childActive(VStdMdiView *b)
{
	if (currChild != 0) {
	  currChild->instanceInactive();
	  if (currChild->iam() != b->iam()) {
	    currChild->classInactive();
		resetMenuBar(currChild);
	  }
	}

	currChild = b;
	updateBars();
}

boolean VStdMdiAppView::erased()
{
	return TRUE;
}

// ---------------------------------------------------------------------------
// Set the status bar prompt to 'msg'
// if msg == 0, then restore original prompt
//
boolean VStdMdiAppView::displayPrompt(VString *msg)
{
	if ((statusBar == 0) || !showingStatusBar) {
		return TRUE;
	}

	if (msg == 0) {
		if (currChild != 0) {
			if (showingStatusBar) {
				setStatusBar(currChild->getStatusBar());
			}
			if (currentStatusBar == statusBar) {
				clearStatusBar(statusBar);
			}
		}
		else {
			clearStatusBar(statusBar);
		}
	}
	else {
		if (statusBar != currentStatusBar) {
			setStatusBar(statusBar);
		}
		statusBar->putText(*msg);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
void VStdMdiAppView::clearStatusBar(VStatusBar *status_bar)
{
	if (status_bar == 0) {
		return;
	}
	status_bar->putText(" Ready");
}

//
//
// ---------------------------------------------------------------------------
// called when the first child StdMDI window becomes active
//
//
void VStdMdiAppView::childrenActive()
{
}

// ---------------------------------------------------------------------------
// called when the last child StdMDI window becomes inactive
//
void VStdMdiAppView::childrenInactive()
{
	if (!isClosing()) {
		if ((menuBar != 0) &&
				 (menuBar == currentMenuBar) && (menuBar == menu)) {
			if (hWnd != 0) {
				SetMenu(hWnd, 0);
			}
			menuBar = 0;
			menuBarIsMine = 0;
			currentMenuBar = 0;
			menu = 0;
		}
		else {
			this->setMenuBar(menuBar);
		}
		this->setStatusBar(statusBar);
		this->setToolBar(toolBar);
	}
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VStdMdiAppView::putTo(VArchiver& a)
{
	VStdMdiAppView::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VStdMdiAppView::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VMdiAppView::putTo(a, FALSE);

	// Write the object version tag.

	a << VStdMdiAppViewArchiveTag;

	// Write any object specific data.
	
	;

	// Write any children.

	if (most_derived) {
		if (children != 0) {
			DO (*children, VWindow, child)
				if ((child->isA(VToolBarCls) && (child != toolBar)) ||
					(child->isA(VStatusBarCls) && (child != statusBar))) {
					child->setArchiveEnable(FALSE);
				}
			END
		}
		VWindow::putChildrenTo(a);
		if (children != 0) {
			DO (*children, VWindow, child)
				if ((child->isA(VToolBarCls) && (child != toolBar)) ||
					(child->isA(VStatusBarCls) && (child != statusBar))) {
					child->setArchiveEnable(TRUE);
				}
			END
		}
	}
}

// ---------------------------------------------------------------------------
//
void VStdMdiAppView::getFrom(VArchiver& a)
{
	// Read the base object data.

	VMdiAppView::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VStdMdiAppViewArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VStdMdiAppViewArchiveTag));
		return;
	}

	// Read any object specific data.

	;
}

// ---------------------------------------------------------------------------
//
void VStdMdiAppView::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
	VStdMdiAppView::getFrom(a);

#if 1

	VWindow::getChildrenFrom(a, FALSE);

	if (do_realize) {
		VStdMdiAppView::realize((VWindow *)parent);
//		VWindow::realizeChildren();
	}

	if (children != 0) {
		DO (*children, VWindow, child)
			if ((toolBar == 0) && child->isA(VToolBarCls)) {
				toolBar = (VToolBar *)child;
				currentToolBar = toolBar;
				if (statusBar != 0) {
					break;
				}
			}
			if ((statusBar == 0) && child->isA(VStatusBarCls)) {
				statusBar = (VStatusBar *)child;
				currentStatusBar = statusBar;
				if (toolBar != 0) {
					break;
				}
			}
		END
	}

	if (toolBar != 0) {
		attachWin(&client, VAttach::Top, VAttach::Sibling, 0, toolBar);
	}
	else {
		attachWin(&client, VAttach::Top, VAttach::Parent);
	}

	if (statusBar != 0) {
		attachWin(&client, VAttach::Bottom, VAttach::Sibling, 0, statusBar);
	}
	else {
		attachWin(&client, VAttach::Bottom, VAttach::Parent);
	}

	frameChildren();

#else
	if (do_realize) {
		VStdMdiAppView::realize((VWindow *)parent);
	}
	VWindow::getChildrenFrom(a, do_realize);
#endif

}

// ---------------------------------------------------------------------------
//
void VStdMdiAppView::realize(VWindow *parentw)
{
	VMdiAppView::realize(parentw);

	this->attachWin(&client, VAttach::Left, VAttach::Parent);
	this->attachWin(&client, VAttach::Right, VAttach::Parent);
	this->attachWin(&client, VAttach::Top, VAttach::Parent);
	this->attachWin(&client, VAttach::Bottom, VAttach::Parent);
}

#endif // ARCHIVER
