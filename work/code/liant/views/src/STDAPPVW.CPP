// $Header:   Q:/views/win/vcs/stdappvw.cpv   1.7   Mar 04 1997 17:55:50   gregm  $ 

//	stdappvw.cpp
//
//	VStdAppView implementation
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
//	07/28/94 dgm	Original
//	11/04/94 dgm	Miscellaneous archiving fixes.
//	11/11/94 dss	Archiver tags macros.
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#include "stdappvw.h"
#include "menu.h"
#include "popupmen.h"
#include "menuitem.h"
#include "menudata.h"
#include "toolbar.h"
#include "statusbr.h"
#include "ordcllct.h"
#include "iterator.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VStdAppView, VAppView)
defineArchiveRevision(VStdAppView, 0)

// --------------------------------------------------------------------------
//
VStdAppView::VStdAppView(const VFrame& frame,
						 int tool_bar_height,
						 int status_bar_height,
						 const VStyle& style)
	: VAppView(0)
{
	initialize();
	(*this)(frame, style, "Main");
	if (tool_bar_height != 0) {
		createToolBar(tool_bar_height);
	}
	if (status_bar_height != 0) {
		createStatusBar(status_bar_height);
	}
}

// --------------------------------------------------------------------------
//
VStdAppView::VStdAppView(int do_realize)
	: VAppView(0)
{
	initialize();
	if (do_realize) {
		(*this)(VFrame(0.9F, 0.9F), StyleDefault, "Main");
	}
}

// --------------------------------------------------------------------------
//
VStdAppView::VStdAppView(const VString& name,
						 const VFrame& frame,
						 int tool_bar_height,
						 int status_bar_height,
						 const VStyle& style)
	: VAppView(0)
{
	initialize();
	(*this)(frame, style, name);
	if (tool_bar_height != 0) {
		createToolBar(tool_bar_height);
	}
	if (status_bar_height != 0) {
		createStatusBar(status_bar_height);
	}
}

// --------------------------------------------------------------------------
//
VStdAppView::~VStdAppView()
{
}

// --------------------------------------------------------------------------
//
void VStdAppView::operator()(const VFrame& frame, const VStyle& style)
{
	VAppView::operator()(frame, style, "Main");
	construct();
}

// --------------------------------------------------------------------------
//
void VStdAppView::operator()(const VFrame& frame, const VStyle& style,
							 const VString& name)
{
	VAppView::operator()(frame, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
void VStdAppView::initialize()
{
	menuBar = 0;
	toolBar = 0;
	statusBar = 0;
	clientArea = 0;
}

// --------------------------------------------------------------------------
//
void VStdAppView::construct()
{
	createMenuBar();
	createToolBar();
	createStatusBar();
	createClientArea();
	frameChildren();
}

// --------------------------------------------------------------------------
//
void VStdAppView::createMenuBar()
{
}

// --------------------------------------------------------------------------
//
void VStdAppView::createToolBar(int height)
{
	if ((toolBar != 0) || (height == 0)) {
		return;
	}
	if (height < 0) {
		height = 28;
	}
	toolBar = new VToolBar(VFrame(0, 0, 1.0F, height), this, FALSE);
	toolBar->setAutoPlace(TRUE);
	attachWin(toolBar, VAttach::Top, VAttach::Parent);
}

// --------------------------------------------------------------------------
//
void VStdAppView::createStatusBar(int height)
{
	if ((statusBar != 0) || (height == 0)) {
		return;
	}
	if (height < 0) {
		statusBar =
			new VStatusBar(VFrame(0, 0, 1.0F, 24, VFrame::Font16), this);
	}
	else {
		statusBar =
			new VStatusBar(VFrame(0, 0, 1.0F, height), this);
	}
	attachWin(statusBar, VAttach::Bottom, VAttach::Parent);
	statusBar->frameChildren();
	statusBar->putText("Ready");
}

// --------------------------------------------------------------------------
//
void VStdAppView::createClientArea()
{
	if (clientArea != 0) {
		return;
	}
	clientArea = new VWindow(VFrame(0, 0, 1.0F, 1.0F), this);
	if (toolBar != 0) {
		attachWin(clientArea, VAttach::Top, VAttach::Sibling, 0, toolBar);
	}
	if (statusBar != 0) {
		attachWin(clientArea, VAttach::Bottom, VAttach::Sibling, 0, statusBar);
	}
}

// --------------------------------------------------------------------------
//
boolean VStdAppView::isToolBarHidden() const
{
	if ((toolBar == 0) || toolBar->isHidden()) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VStdAppView::isStatusBarHidden() const
{
	if ((statusBar == 0) || statusBar->isHidden()) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
void VStdAppView::hideStatusBar(boolean b)
{
	if (statusBar == 0) {
		return;
	}
	if (b) {
		if (!statusBar->isHidden()) {
			if (clientArea != 0) {
				attachWin(clientArea, VAttach::Bottom, VAttach::Parent);
			}
			statusBar->hide();
			frameChildren();
		}
	}
	else if (statusBar->isHidden()) {
		if (clientArea != 0) {
			attachWin(clientArea, VAttach::Bottom, VAttach::Sibling, 0,
												   statusBar);
		}
		statusBar->show();
		frameChildren();
	}
}

// --------------------------------------------------------------------------
//
void VStdAppView::hideToolBar(boolean b)
{
	if (toolBar == 0) {
		return;
	}
	if (b) {
		if (!toolBar->isHidden()) {
			if (clientArea != 0) {
				attachWin(clientArea, VAttach::Top, VAttach::Parent);
			}
			toolBar->hide();
			frameChildren();
		}
	}
	else if (toolBar->isHidden()) {
		if (clientArea != 0) {
			attachWin(clientArea, VAttach::Top, VAttach::Sibling, 0,
												toolBar);
		}
		toolBar->show();
		frameChildren();
	}
}

// --------------------------------------------------------------------------
//
boolean	VStdAppView::displayPrompt(VString *message)
{
	if (statusBar == 0) {
		return FALSE;
	}
	if (message == 0) {
		statusBar->putText("Ready");
	}
	else {
		statusBar->putText(*message);
	}
	return TRUE;
}

#ifndef CV_NOARCHIVER

// --------------------------------------------------------------------------
//
void VStdAppView::putTo(VArchiver& a)
{
	VStdAppView::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//
void VStdAppView::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VAppView::putTo(a, FALSE);

	// Write the object version tag.

	a << VStdAppViewArchiveTag;

	// Write any object specific data.

	;

	// Write any children.
	
	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// --------------------------------------------------------------------------
//
void VStdAppView::getFrom(VArchiver& a)
{
	// Read the base object data.

	VAppView::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;

	if (!archiveTagOk(tag, VStdAppViewArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VStdAppViewArchiveTag));
		return;
	}

	// Read any object specific data.

	;
}

// --------------------------------------------------------------------------
//
void VStdAppView::getFrom(VArchiver& a, VObject *parent_window,
						  boolean do_realize)
{
	VStdAppView::getFrom(a);

	if (do_realize) {
		VStdAppView::realize((VWindow *)parent_window);
	}

	VWindow::getChildrenFrom(a, do_realize);

	if (children != 0) {
		DO (*children, VWindow, child)
			if ((statusBar == 0) && child->isA(VStatusBarCls)) {
				statusBar = (VStatusBar *)child;
				if ((toolBar != 0) && (clientArea != 0)) {
					break;
				}
			}
			else if ((toolBar == 0) && child->isA(VToolBarCls)) {
				toolBar = (VToolBar *)child;
				if ((statusBar != 0) && (clientArea != 0)) {
					break;
				}
			}
			else if (clientArea == 0) {
				clientArea = child;
				if ((statusBar != 0) && (toolBar != 0)) {
					break;
				}
			}
		END
	}
}

// --------------------------------------------------------------------------
//
void VStdAppView::realize(VWindow *parent_window)
{
	VAppView::realize(parent_window);
}

#endif // ARCHIVER
