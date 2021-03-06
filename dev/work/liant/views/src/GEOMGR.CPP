// $Header:   Q:/views/common/vcs/GEOMGR.CPV   1.20   Mar 24 1997 16:09:08   scottt  $

//	geomgr.cpp
//
//	VGeoManager implementation [Common]
//
//	Allegris Foundation 1.1.00
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	10/20/95 dgm	Original.
//	11/01/95 dgm	Updated.
//	11/10/95 dgm	Updated.
//	11/20/95 dgm	Added beginFrameChildren() and endFrameChildren().
//	11/28/95 dgm	Added subtractInternalBorderSize().
//	12/06/95 dgm	Added dontArchive support.
//	12/11/95 dgm	Added backward compatibility for pre-dontArchive support.
//	01/26/96 dgm	Added SizeGroup support.
//	01/29/96 dgm	Size-group changes.
//	02/06/96 dgm	Added frameChildrenOk().
//	02/16/96 dgm	Support to query VWindow associated with VGeoManager.
//	04/04/96 dgm	Added null checks in getFrom().
//	05/02/96 dgm	Minor size-group fix.
//	08/26/96 dgm	Changed update() to take an int rather than a boolean;
//					this is to implement a currently undocumented feature
//					which allows update to propagate up to the top-level
//					parent, but without automatically (re)framing.
//	09/10/96 dgm	Fixed memory leak in destructor (theChildren).
//	09/13/96 dgm	Fixed memory leak; wasn't going through child list
//					backwards while delete so as not to mess up iteration.
//	04/24/97 dgm	A couple queryManagerWindowSize fixes.
// ---------------------------------------------------------------------------

#include "geomgr.h"
#include "window.h"
#include "frame.h"
#include "ratio.h"
#include "rect.h"
#include "ordcllct.h"
#include "iterator.h"
#include "combobox.h"
#include "cvassert.h"
#include <stdio.h>

// --------------------------------------------------------------------------
//
#if !defined(CV_NOARCHIVER)
#include "archiver.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VGeoManager, VGeoFrame)
//
// Revision 0: Original.
// Revision 1: Added dont-archive flag and child-index.
//
defineArchiveRevision(VGeoManager, 1)

// --------------------------------------------------------------------------
//
VGeoManager::VGeoManager(const VFrame& frame, VGeoManager *parent)
	: VGeoFrame(frame, parent)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoManager::VGeoManager(int x, int y, int w, int h, VGeoManager *parent)
	: VGeoFrame(x, y, w, h, parent)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoManager::VGeoManager(int w, int h, VGeoManager *parent)
	: VGeoFrame(w, h, parent)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoManager::VGeoManager(VGeoManager *parent)
	: VGeoFrame(parent)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoManager::VGeoManager(const VFrame& frame, VWindow *parent_window)
	: VGeoFrame(frame, parent_window)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoManager::VGeoManager(int x, int y, int w, int h, VWindow *parent_window)
	: VGeoFrame(x, y, w, h, parent_window)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoManager::VGeoManager(int w, int h, VWindow *parent_window)
	: VGeoFrame(w, h, parent_window)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoManager::VGeoManager(VWindow *parent_window)
	: VGeoFrame(parent_window)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoManager::VGeoManager()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoManager::~VGeoManager()
{
	if (theChildren != 0) {
		REVERSEDO (*theChildren, VGeoFrame, child)
			if (child->getWindow() == 0) {
				delete child;
			}
			else {
				removeChild(child);
			}
		END
		delete theChildren;
	}
	if (sizeGroups != 0) {
		sizeGroups->freeContents();
		delete sizeGroups;
	}
}

// --------------------------------------------------------------------------
// Make the given VGeoFrame a child of this VGeoManager, i.e. place it on
// this VGeoManagers's children list and set up the parent and child specific
// data appropriately.  If the given VGeoFrame is already a child of another
// VGeoManager, then it will be removed from that VGeoManager.
//
VGeoFrame *VGeoManager::addChild(VGeoFrame *child)
{
	if (child == 0) {
		return 0;
	}
	else if (child->theParent == this) {
		return child;
	}
	if (child->theParent != 0) {
		child->theParent->removeChild(child);
	}
	if (theChildren == 0) {
		theChildren = new VOrdCollect;
	}
	theChildren->add(child);
	child->theParent = this;
	if (child->childData != 0) {
		delete child->childData;
	}
	child->childData = newChildData();
	update();
	return child;
}

// --------------------------------------------------------------------------
// Make the given VWindow a child of this VGeoFrame; create an associated
// VGeoFrame for the given VWindow if it doesn't have already one, and place
// it on this VGeoManagers's children list and set up parent child specific
// data appropriately.  If the given VGeoFrame is already a child of another
// VGeoManager, then it will be removed from that VGeoManager.
//
VGeoFrame *VGeoManager::addChild(VWindow *child_window)
{
	if (child_window == 0) {
		return 0;
	}
	if (child_window->geoFrame == 0) {
		child_window->geoFrame = new VGeoFrame;
		child_window->geoFrame->associateWindow(child_window);
	}
	return addChild(child_window->geoFrame);
}

// --------------------------------------------------------------------------
// If the given VGeoFrame is a child of this VGeoManager then remove it from
// this VGeoManager.
//
boolean VGeoManager::removeChild(VGeoFrame *child)
{
	if ((child == 0) || (child->theParent != this)) {
		return FALSE;
	}
	if (child->childData != 0) {
		delete child->childData;
		child->childData = 0;
	}
	if (theChildren != 0) {
		theChildren->removeSame(child);
	}
	child->theParent = 0;
	childRemoved(child);
	return TRUE;
}

// --------------------------------------------------------------------------
// If the VGeoFrame associated with the given VWindow is a child of this
// VGeoManager then remove it from this VGeoManager.
//
boolean VGeoManager::removeChild(VWindow *child_window)
{
	if (child_window == 0) {
		return FALSE;
	}
	return removeChild(child_window->geoFrame);
}

// --------------------------------------------------------------------------
// Replace the given "old" VGeoFrame child with the given "new" VGeoFrame.
// The old VGeoFrame *must* be a child of this VGeoManager; it will be
// orphaned, but not deleted.  If the given new VGeoFrame is already a child
// of another VGeoManager, then it will be removed from that VGeoManager.
//
boolean VGeoManager::replaceChild(VGeoFrame *old_child, VGeoFrame *new_child)
{
	if ((new_child == 0) ||
		(old_child == 0) ||
		(theChildren == 0) ||
		(old_child->theParent != this)) {
		return FALSE;
	}
	if (old_child->isGeoManager && new_child->isGeoManager) {
		((VGeoManager *)old_child)->moveChildren((VGeoManager *)new_child);
	}
	if (new_child->theParent != 0) {
		new_child->theParent->removeChild(new_child);
	}
	theChildren->replaceSame(old_child, new_child);
	new_child->theParent = this;
	new_child->childData = old_child->childData;
	old_child->theParent = 0;
	old_child->childData = 0;
	childReplaced(old_child, new_child);
	return TRUE;
}

// --------------------------------------------------------------------------
// Move the children of this VGeoManager to the the given new VGeoManager.
//
boolean VGeoManager::moveChildren(VGeoManager *new_manager,
								  boolean preserve_child_data)
{
	if ((new_manager == 0) || (theChildren == 0)) {
		return FALSE;
	}
	else if (new_manager == this) {
		return TRUE;
	}

	if (preserve_child_data) {
		if (iam() != new_manager->iam()) {
			preserve_child_data = FALSE;
		}
	}

	if (new_manager->theChildren == 0) {
		new_manager->theChildren = theChildren;
		ChildIterator children(new_manager, TRUE);
		VGeoFrame *child;
		while ((child = children.next()) != 0) {
			if (!preserve_child_data) {
				if (child->childData != 0) {
					delete child->childData;
				}
				child->theParent = new_manager;
				child->childData = new_manager->newChildData();
			}
			else {
				child->theParent = new_manager;
			}
		}
	}
	else {
		ChildIterator children(this, TRUE);
		VGeoFrame *child;
		if (new_manager->theChildren == 0) {
			new_manager->theChildren = new VOrdCollect;
		}
		while ((child = children.next()) != 0) {
			new_manager->theChildren->add(child);
			if (!preserve_child_data) {
				if (child->childData != 0) {
					delete child->childData;
				}
				child->theParent = new_manager;
				child->childData = new_manager->newChildData();
			}
			else {
				child->theParent = new_manager;
			}
		}
		delete theChildren;
	}
	theChildren = 0;

	return TRUE;
}

// --------------------------------------------------------------------------
// Move the given child of this VGeoManager to the the given VGeoManager.
//
boolean VGeoManager::moveChild(VGeoFrame *child,
							   VGeoManager *new_manager,
							   boolean preserve_child_data)
{
	if ((child == 0) || (child->theParent != this) ||
		(new_manager == 0) || (theChildren == 0)) {
		return FALSE;
	}
	else if (new_manager == this) {
		return TRUE;
	}

	if (preserve_child_data) {
		if (iam() != new_manager->iam()) {
			preserve_child_data = FALSE;
		}
	}

	if (!preserve_child_data) {
		if (child->childData != 0) {
			delete child->childData;
		}
		child->theParent = new_manager;
		child->childData = new_manager->newChildData();
	}
	else {
		child->theParent = new_manager;
	}

	if (new_manager->theChildren == 0) {
		new_manager->theChildren = new VOrdCollect;
	}
	new_manager->theChildren->add(child);
	theChildren->removeSame(child);
	if (theChildren->count() <= 0) {
		delete theChildren;
		theChildren = 0;
	}

	return TRUE;
}

// --------------------------------------------------------------------------
//
int VGeoManager::findChild(VGeoFrame *child) const
{
	if ((theChildren == 0) || (child == 0) || (child->theParent != this)) {
		return -1;
	}
	return int(theChildren->indexOfSame(child));
}

// --------------------------------------------------------------------------
//
void VGeoManager::getPreferredSize(int *w, int *h)
{
	if (w != 0) { *w = 0; }
	if (h != 0) { *h = 0; }
	if (queryManagerWindowSize) {
		checkWindowPreferredSize(w, h);
		addNonClientAreaSize(w, h);
	}
}

// --------------------------------------------------------------------------
//
boolean VGeoManager::getConstrainedSize(int *w, int *h,
										int constrained_w, int constrained_h)
{
	if (w != 0) { *w = 0; }
	if (h != 0) { *h = 0; }
	if (queryManagerWindowSize) {
		checkWindowConstrainedSize(w, h, constrained_w, constrained_h);
		addNonClientAreaSize(w, h);
		if (((w != 0) && (*w > 0)) || ((h != 0) && (*h > 0))) {
			return TRUE;
		}
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
void VGeoManager::getMinimumSize(int *w, int *h)
{
	if (w != 0) { *w = 0; }
	if (h != 0) { *h = 0; }
	if (queryManagerWindowSize) {
		checkWindowMinimumSize(w, h);
		addNonClientAreaSize(w, h);
	}
}

// --------------------------------------------------------------------------
//
void VGeoManager::getMaximumSize(int *w, int *h)
{
	VGeoFrame::getMaximumSize(w, h);
	if (queryManagerWindowSize) {
		checkWindowMaximumSize(w, h);
		addNonClientAreaSize(w, h);
	}
}

// --------------------------------------------------------------------------
//
void VGeoManager::update(int propagate_up)
{
	VGeoFrame::update(propagate_up);
	if ((propagate_up > 0) && (theParent == 0)) {
		frame();
	}
}

// --------------------------------------------------------------------------
//
void VGeoManager::childUpdated(VGeoFrame *)
{
}

// --------------------------------------------------------------------------
//
void VGeoManager::childRemoved(VGeoFrame *)
{
}

// --------------------------------------------------------------------------
//
//
void VGeoManager::childReplaced(VGeoFrame *, VGeoFrame *)
{
}

// --------------------------------------------------------------------------
//
void VGeoManager::windowAssociated(VWindow *window)
{
	if (window != 0) {
		// int left, right, top, bottom;
		// left = right = top = bottom = 0;
		// window->getNonClientArea(&left, &right, &top, &bottom);
		// setNonClientArea(left, right, top, bottom);
	}
	else {
		// setNonClientArea(0, 0, 0, 0);
	}
}

// --------------------------------------------------------------------------
//
void VGeoManager::setInternalBorderGeometry(int left, int right,
											int top, int bottom)
{
	if      (left < 0)		{ internalLeftBorder   = 0;            }
	else if (left > 127)	{ internalLeftBorder   = char(127);    }
	else					{ internalLeftBorder   = char(left);   }
	if      (right < 0)		{ internalRightBorder  = 0;            }
	else if (right > 127)	{ internalRightBorder  = char(127);    }
	else					{ internalRightBorder  = char(right);  }
	if      (top < 0)		{ internalTopBorder    = 0;            }
	else if (top > 127)		{ internalTopBorder    = char(127);    }
	else					{ internalTopBorder    = char(top);    }
	if      (bottom < 0)	{ internalBottomBorder = 0;            }
	else if (bottom > 127)	{ internalBottomBorder = char(127);    }
	else					{ internalBottomBorder = char(bottom); }
}

// --------------------------------------------------------------------------
//
void VGeoManager::getInternalBorderGeometry(int *left, int *right,
										    int *top, int *bottom) const
{
	if (usingTheWindow) {
		if (theWindow != 0) {
			theWindow->getInternalBorderGeometry(left, right, top, bottom);
		}
		else {
			if (left   != 0) { *left   = 0; }
			if (right  != 0) { *right  = 0; }
			if (top    != 0) { *top    = 0; }
			if (bottom != 0) { *bottom = 0; }
		}
	}
	else {
		if (left   != 0) { *left   = internalLeftBorder;   }
		if (right  != 0) { *right  = internalRightBorder;  }
		if (top    != 0) { *top    = internalTopBorder;    }
		if (bottom != 0) { *bottom = internalBottomBorder; }
	}
}

// --------------------------------------------------------------------------
//
boolean VGeoManager::frameChildrenOk(int /* w */ , int /* h */ )
{
	return TRUE;
}

// --------------------------------------------------------------------------
// Frame any children of this VGeoFrame.
// This is intended to be overridden by derived classes.
//
void VGeoManager::frameChildren(int /* w */ , int /* h */ )
{
}

// --------------------------------------------------------------------------
// Resize notification; the given size (w, h) refers to the client area.
// ToDo: straighten out the strange handling of client area in native
// windows versus Views windows, e.g. consider the difference between a
// window with native shaded borders and VShadeControl ...
//
void VGeoManager::resized(int w, int h)
{
	VWindow::beginDeferGeometryChanges();
	frameChildren(w, h);
	VWindow::endDeferGeometryChanges();
}

// --------------------------------------------------------------------------
//
void VGeoManager::beginFrameChildren(int& w, int& h)
{
	if (w <= 0) {
		if (h <= 0) {
			getPhysicalClientSize(&w, &h);
		}
		else {
			getPhysicalClientSize(&w, 0);
		}
	}
	else if (h <= 0) {
		getPhysicalClientSize(0, &h);
	}

	// Subtract off any internal borders.

	subtractInternalBorderSize(&w, &h);

	VWindow::beginDeferGeometryChanges();

	// Handle size groups. 

	setupSizeGroups();
}

// --------------------------------------------------------------------------
//
void VGeoManager::setupSizeGroups()
{
	if (sizeGroups == 0) {
		return;
	}
	DO (*sizeGroups, SizeGroup, sizegroup)
		VOrdCollect *list = sizegroup->getList();
		if (list == 0) {
			continue;
		}
		int w, h, maxw = 0, maxh = 0;
		DO (*list, VGeoFrame, geoframe)
			geoframe->determinePreferredSize(&w, &h);
			if (w > maxw) { maxw = w; }
			if (h > maxh) { maxh = h; }
		END
		if (sizegroup->getType() & Square) {
			if (maxw > maxh) { maxh = maxw; }
			if (maxh > maxw) { maxw = maxh; }
		}
		DO (*list, VGeoFrame, geoframe)
			if (sizegroup->getType() & Width) {
				geoframe->preferredWidthCache = maxw;
				geoframe->preferredWidthCache.setValid();
			}
			if (sizegroup->getType() & Height) {
				geoframe->preferredHeightCache = maxh;
				geoframe->preferredHeightCache.setValid();
			}
		END
	END
}

// --------------------------------------------------------------------------
//
void VGeoManager::endFrameChildren()
{
	VWindow::endDeferGeometryChanges();
}

// --------------------------------------------------------------------------
//
void VGeoManager::subtractInternalBorderSize(int *w, int *h)
{
	int left, right, top, bottom;
	if (w != 0) {
		if (h != 0) {
			getInternalBorderGeometry(&left, &right, &top, &bottom);
			*w -= left + right;
			*h -= top + bottom;
		}
		else {
			getInternalBorderGeometry(&left, &right, 0, 0);
			*w -= left + right;
		}
	}
	else if (h != 0) {
		getInternalBorderGeometry(0, 0, &top, &bottom);
		*h -= top + bottom;
	}
}

// ---------------------------------------------------------------------------
//
void VGeoManager::checkWindowPreferredSize(int *w, int *h)
{
	if (queryManagerWindowSize && usingTheWindow && (theWindow != 0)) {
		int ww,  hh;
		if (w != 0) {
			if (h != 0) {
				theWindow->getPreferredSize(&ww, &hh);
				if (hh > *h) { *h = hh; }
			}
			else {
				theWindow->getPreferredSize(&ww, 0);
			}
			if (ww > *w) { *w = ww; }
		}
		else if (h != 0) {
			theWindow->getPreferredSize(0, &hh);
			if (hh > *h) { *h = hh; }
		}
	}
}

// ---------------------------------------------------------------------------
//
void VGeoManager::checkWindowConstrainedSize(int *w, int *h, int cw, int ch)
{
	if (queryManagerWindowSize && usingTheWindow && (theWindow != 0)) {
		int ww,  hh;
		if (w != 0) {
			if (h != 0) {
				if (!theWindow->getConstrainedSize(&ww, &hh, cw, ch)) {
					return;
				}
				if (hh > *h) { *h = hh; }
			}
			else {
				if (!theWindow->getConstrainedSize(&ww, 0, cw, ch)) {
					return;
				}
			}
			if (ww > *w) { *w = ww; }
		}
		else if (h != 0) {
			if (!theWindow->getConstrainedSize(0, &hh, cw, ch)) {
				return;
			}
			if (hh > *h) { *h = hh; }
		}
	}
}

// ---------------------------------------------------------------------------
//
void VGeoManager::checkWindowMinimumSize(int *w, int *h)
{
	if (queryManagerWindowSize && usingTheWindow && (theWindow != 0)) {
		int ww,  hh;
		if (w != 0) {
			if (h != 0) {
				theWindow->getMinimumSize(&ww, &hh);
				if (hh > *h) { *h = hh; }
			}
			else {
				theWindow->getMinimumSize(&ww, 0);
			}
			if (ww > *w) { *w = ww; }
		}
		else if (h != 0) {
			theWindow->getMinimumSize(0, &hh);
			if (hh > *h) { *h = hh; }
		}
	}
}

// ---------------------------------------------------------------------------
//
void VGeoManager::checkWindowMaximumSize(int *w, int *h)
{
	if (queryManagerWindowSize && usingTheWindow && (theWindow != 0)) {
		int ww,  hh;
		if (w != 0) {
			if (h != 0) {
				theWindow->getMaximumSize(&ww, &hh);
				if (hh < *h) { *h = hh; }
			}
			else {
				theWindow->getMaximumSize(&ww, 0);
			}
			if (ww < *w) { *w = ww; }
		}
		else if (h != 0) {
			theWindow->getMaximumSize(0, &hh);
			if (hh < *h) { *h = hh; }
		}
	}
}

// ---------------------------------------------------------------------------
// Associate the given VWindow with this VGeoManager.
//
boolean VGeoManager::associateWindow(VWindow *window)
{
	if (window == 0) {
		return FALSE;
	}
	if (this == window->geoManager) {
		//
		// This VGeoManager is already the
		// geometry manager for the given VWindow.
		//
		return TRUE;
	}
	if (theParent != 0) {
		//
		// This VGeoManager is already a
		// part of another VGeoFrame hierarchy.
		//
		return FALSE;
	}
	if (getWindow() != 0) {
		//
		// This VGeoManager is already
		// the geometry manager for a VWindow.
		//
		return FALSE;
	}
	if (window->geoFrame != 0) {
		//
		// The given VWindow already has a VGeoManager or VGeoFrame;
		// replace it with the given VGeoManager, and then delete it.
		//
		VGeoManager *parent_manager = window->geoFrame->getParent();
		if (parent_manager != 0) {
			parent_manager->replaceChild(window->geoFrame, this);
		}
		else if (window->geoFrame->isGeoManager) {
			window->geoManager->moveChildren(this);
		}
		delete window->geoFrame;
	}

	boolean status = VGeoFrame::associateWindow(window);

	if (usingTheWindow && (theWindow != 0)) {
		extern VClass *VGroupCls;
		if (theWindow->isA(VGroupCls)) {
			queryManagerWindowSize = 1;
		}
	}

	return status;
}

// --------------------------------------------------------------------------
//
void VGeoManager::ChildIterator::begin(const VGeoFrame *geoframe)
{
	const VGeoFrame *object;

	index = 0;

	if (backwards) {
		VIterator::end();
		if (geoframe != 0) {
			while (1) {
				object = (const VGeoFrame *)((*((VIterator *)this))--);
				if (object == geoframe) {
					(*this)++;
					break;
				}
				else if (object == 0) {
					VIterator::end();
					index = 0;
					break;
				}
				else {
					index++;
				}
			}
		}
	}
	else {
		VIterator::start();
		if (geoframe != 0) {
			while (1) {
				object = (const VGeoFrame *)((*((VIterator *)this))++);
				if (object == geoframe) {
					(*this)--;
					break;
				}
				else if (object == 0) {
					VIterator::start();
					index = 0;
					break;
				}
				else {
					index++;
				}
			}
		}
	}
}

// --------------------------------------------------------------------------
//
VGeoFrame *VGeoManager::ChildIterator::next()
{
	for (VGeoFrame *child ; 1 ; ) {
		if (backwards) {
			child = (VGeoFrame *)((*this)--);
		}
		else {
			child = (VGeoFrame *)((*this)++);
		}
		if (child == 0) {
			return 0;
		}
		index++;
		if (dontSkipAny || (!child->dontFrame &&
							!child->usingTheWindowIndex)) {
			return child;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
VGeoManager::SizeGroup::SizeGroup(int type)
{
	theType = type;
	theList = 0;
}

// --------------------------------------------------------------------------
//
VGeoManager::SizeGroup::~SizeGroup()
{
	if (theList != 0) {
		delete theList;
	}
}

// --------------------------------------------------------------------------
//
void VGeoManager::SizeGroup::setType(int type)
{
	theType = type;
}

// --------------------------------------------------------------------------
//
VGeoManager::SizeGroupType VGeoManager::SizeGroup::getType() const
{
	return SizeGroupType(theType);
}

// --------------------------------------------------------------------------
//
void VGeoManager::SizeGroup::add(VGeoFrame *geoframe)
{
	if (geoframe == 0) {
		return;
	}
	else if (theList == 0) {
		theList = new VOrdCollect;
	}
	else if (theList->findSame(geoframe)) {
		return;
	}
	theList->add(geoframe);
}

// --------------------------------------------------------------------------
//
void VGeoManager::SizeGroup::add(VWindow *window)
{
	if (window == 0) {
		return;
	}
	add(window->getGeoFrame());
}

// --------------------------------------------------------------------------
//
void VGeoManager::SizeGroup::remove(VGeoFrame *geoframe)
{
	if ((theList == 0) || (geoframe == 0)) {
		return;
	}
	theList->removeSame(geoframe);
}

// --------------------------------------------------------------------------
//
void VGeoManager::SizeGroup::remove(VWindow *window)
{
	if ((theList == 0) || (window == 0) || (window->getGeoFrame() == 0)) {
		return;
	}
	theList->removeSame(window->getGeoFrame());
}

// --------------------------------------------------------------------------
//
void VGeoManager::SizeGroup::removeAll()
{
	if (theList == 0) {
		return;
	}
	delete theList;
	theList = 0;
}

// --------------------------------------------------------------------------
//
VOrdCollect *VGeoManager::SizeGroup::getList() const
{
	return theList;
}

// --------------------------------------------------------------------------
//
VGeoManager::SizeGroup *VGeoManager::newSizeGroup(int type)
{
	SizeGroup *sizegroup = new SizeGroup(type);
	if (sizeGroups == 0) {
		sizeGroups = new VOrdCollect;
	}
	sizeGroups->add(sizegroup);
	return sizegroup;
}

// ==========================================================================

#if !defined(CV_NOARCHIVER)

static int archive_revision = 0;

// --------------------------------------------------------------------------
//
void VGeoManager::putTo(VArchiver& a)
{
	putTo(a, TRUE);
	putToDone(a);
}

// --------------------------------------------------------------------------
//
void VGeoManager::putTo(VArchiver& a, boolean)
{
	// Write the base class data.

	VGeoFrame::putTo(a, FALSE);

	// Write the revision tag.

	a << VGeoManagerArchiveTag;

	// Write the class specific data.

	a << internalLeftBorder;
	a << internalRightBorder;
	a << internalTopBorder;
	a << internalBottomBorder;
}

// --------------------------------------------------------------------------
//
void VGeoManager::putToDone(VArchiver& a)
{
	VGeoFrame::putToDone(a);

	long nchildren = getNumberOfChildren();

	a << nchildren;

	if (nchildren > 0) {
		ChildIterator children(this, TRUE);
		VGeoFrame *child;
		while ((child = children.next()) != 0) {
			if (child->dontArchive) {
				a << char(0);
			}
			else {
				a << char(1);
				a << short(child->getIndex());
				a.putObject(child);
				if (child->childData == 0) {
					a << char(0);
				}
				else {
					a << char(1);
					child->childData->putTo(a);
				}
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VGeoManager::getFrom(VArchiver& a, VObject *object, boolean)
{
	getFrom(a);
	getFromDone(a, object);
}

// --------------------------------------------------------------------------
//
void VGeoManager::getFrom(VArchiver& a)
{
	// Read the base class data.

	VGeoFrame::getFrom(a);

	// Read and verify the revision tag.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VGeoManagerArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VGeoManagerArchiveTag)); return;
	}

    archive_revision = getArchiveTagRev(tag);

	// Read the class specific data.

	a >> internalLeftBorder;
	a >> internalRightBorder;
	a >> internalTopBorder;
	a >> internalBottomBorder;
}

// --------------------------------------------------------------------------
//
void VGeoManager::getFromDone(VArchiver& a, VObject *object)
{
	VGeoFrame::getFromDone(a, object);

	long nchildren;
	a >> nchildren;

	if (nchildren > 0) {

		int ndummies = 0;

		if (theChildren == 0) {
			theChildren = new VOrdCollect;
		}

		for (int i = 0 ; i < nchildren ; i++) {
			if (archive_revision >= 1) {
				char flag;
				a >> flag;
				if (flag) {
					short index;
					a >> index;
					VGeoFrame *child = (VGeoFrame *)a.getObject(0, this);
					if (child == 0) {
						a.abort(VArchiver::ClassSpecific, 0); continue;
					}
					a >> flag;
					if (flag != char(0)) {
						child->childData = newChildData();
						if (child->childData != 0) {
							child->childData->getFrom(a);
						}
					}
					theChildren->add(child);
					child->theParent = this;
				}
				else {
					//
					// There was a child here when this VGeoManager
					// archived, but it was marked as dont-archive.
					// We create a dummy here so that anyone referencing
					// siblings via index (i.e. as would be the case when
					// archiving) will still be able to reliably do so.
					// These should be removed after we're done.
					//
					VGeoFrame *dummy_child = new VGeoFrame(this);
					dummy_child->setDontFrame(TRUE);
					dummy_child->isDummy = 1;
					ndummies++;
				}
			}
			else {
				//
				// Here, we are reading a revision 0 VGeoManager which
				// does not have a dont-archive flag nor a child-index.
				//
				VGeoFrame *child = (VGeoFrame *)a.getObject(0, this);
				if (child == 0) {
					a.abort(VArchiver::ClassSpecific, 0); continue;
				}
				char flag;
				a >> flag;
				if (flag != char(0)) {
					child->childData = newChildData();
					if (child->childData != 0) {
						child->childData->getFrom(a);
					}
				}
				theChildren->add(child);
				child->theParent = this;
			}
		}

		gotChildren();

		// Remove any dummys we created.
	
		if (ndummies > 0) {
			VGeoManager::ChildIterator children(this, TRUE);
			VGeoFrame *child;
			children.reverse();
			while ((child = children.next()) != 0) {
				if (child->isDummy) {
					delete child;
				}
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VGeoManager::gotChildren()
{
}

#endif
