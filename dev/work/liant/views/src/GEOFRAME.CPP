// $Header:   Q:/views/common/vcs/GEOFRAME.CPV   1.20   Mar 24 1997 16:09:04   scottt  $

//	geoframe.cpp
//
//	VGeoFrame implementation [Common]
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
//	09/29/95 dgm	Original.
//	10/02/95 dgm	Updated.
//	10/19/95 dgm	Updated.
//	11/01/95 dgm	Updated.
//	11/10/95 dgm	Updated.
//	11/15/95 dgm	Updated.
//	11/20/95 dgm	Use getInternalBorderGeometry() in setPhysicalGeometry().
//	11/29/95 dgm	Added static getFromDone().
//	12/05/95 dgm	Changes for UNIX DEC/Alpha-C++ compiler.
//	12/05/95 dgm	Minor fix in getPhysicalClientSize().
//	12/06/95 dgm	Added dontArchive support.
//	12/12/95 dgm	Changed changeGeometryXXX() to setGeometryXXX().
//	01/17/96 dgm	Added VComboBox height hack.
//	01/29/96 dgm	Size-group changes.
//	02/08/96 dgm	VComboBox fix.
//	05/31/96 dgm	Initialize dontArchive.
//	08/26/96 dgm	Changed update() to take an int rather than a boolean.
//	09/06/96 dgm	Changed the setGeometryXXX() routines to look at the new
//					forceFrameChildren bit for VGeoManagers and to (re)frame
//					the children  of those VGeoManagers whose sizes are being
//					set but are not actually changing (i.e. set to the same
//					size that it was).  This bit is set via updateGeometry().
//	09/18/96 dgm	Fixed memory leak (theGeometry).
//	01/14/97 dgm	Added null frame checks in getGeometry().
// --------------------------------------------------------------------------

#include "geoframe.h"
#include "geomgr.h"
#include "geoform.h"
#include "window.h"
#include "frame.h"
#include "ratio.h"
#include "rect.h"
#include "ordcllct.h"
#include "iterator.h"
#include "combobox.h"
#include <stdio.h>

// --------------------------------------------------------------------------
//
#if !defined(CV_NOARCHIVER)
#include "archiver.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VGeoFrame, VObject)
defineArchiveRevision(VGeoFrame, 0)

// --------------------------------------------------------------------------
//
#define font16ToPixels(dimension, font_dimension) \
			(int((float(dimension * font_dimension) / 16.0F) + 0.5F))

// --------------------------------------------------------------------------
//
VGeoFrame::VGeoFrame(const VFrame& frame, VGeoManager *parent)
{
	initialize();
	theGeometry = new Geometry(frame);
	if (parent != 0) {
		parent->addChild(this);
	}
}

// --------------------------------------------------------------------------
//
VGeoFrame::VGeoFrame(int x, int y, int w, int h, VGeoManager *parent)
{
	initialize();
	theGeometry = new Geometry(x, y, w, h);
	if (parent != 0) {
		parent->addChild(this);
	}
}

// --------------------------------------------------------------------------
//
VGeoFrame::VGeoFrame(int w, int h, VGeoManager *parent)
{
	initialize();
	theGeometry = new Geometry(w, h);
	if (parent != 0) {
		parent->addChild(this);
	}
}

// --------------------------------------------------------------------------
//
VGeoFrame::VGeoFrame(VGeoManager *parent)
{
	initialize();
	if (parent != 0) {
		parent->addChild(this);
	}
}

// --------------------------------------------------------------------------
//
VGeoFrame::VGeoFrame(const VFrame& frame, VWindow *parent_window)
{
	initialize();
	theGeometry = new Geometry(frame);
	if (parent_window != 0) {
		if (verifyManager(parent_window)) {
			parent_window->geoManager->addChild(this);
		}
	}
}

// --------------------------------------------------------------------------
//
VGeoFrame::VGeoFrame(int x, int y, int w, int h, VWindow *parent_window)
{
	initialize();
	theGeometry = new Geometry(x, y, w, h);
	if (parent_window != 0) {
		if (verifyManager(parent_window)) {
			parent_window->geoManager->addChild(this);
		}
	}
}

// --------------------------------------------------------------------------
//
VGeoFrame::VGeoFrame(int w, int h, VWindow *parent_window)
{
	initialize();
	theGeometry = new Geometry(w, h);
	if (parent_window != 0) {
		if (verifyManager(parent_window)) {
			parent_window->geoManager->addChild(this);
		}
	}
}

// --------------------------------------------------------------------------
//
VGeoFrame::VGeoFrame(VWindow *parent_window)
{
	initialize();
	if (parent_window != 0) {
		if (verifyManager(parent_window)) {
			parent_window->geoManager->addChild(this);
		}
	}
}

// --------------------------------------------------------------------------
//
VGeoFrame::VGeoFrame()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoFrame::~VGeoFrame()
{
	if (theParent != 0) {
		theParent->removeChild(this);
	}
	if (usingTheWindow) {
		if ((theWindow != 0) && (theWindow->geoFrame == this)) {
			theWindow->geoFrame = 0;
		}
	}
	else if (!usingTheWindowIndex) {
		if (theGeometry != 0) {
			delete theGeometry;
		}
	}
	if (childData != 0) {
		delete childData;
	}
}

// --------------------------------------------------------------------------
//
void VGeoFrame::initialize()
{
	theParent				= 0;
	theWindow				= 0;	// N.B. united with theGeometry
	childData				= 0;
	usingTheWindow			= 0;
	usingTheWindowIndex		= 0;
	dontFrame				= 0;
	dontArchive				= 0;
	isGeoManager			= 0;
	isGeoForm				= 0;
	isGeoRowOrColumn		= 0;
	isGeoRow				= 0;
	isGeoColumn				= 0;
	isGeoRowColumn			= 0;
	isGeoPacker				= 0;
	isDummy					= 0;
	queryManagerWindowSize	= 0;
	forceFrameChildren		= 0;
}

// --------------------------------------------------------------------------
// Return the (zero-based) index of this VGeoFrame within its parent's
// list of children.  If this VGeoFrame has no parent, then return -1.
//
int VGeoFrame::getIndex() const
{
	if (theParent == 0) {
		return -1;
	}
	return theParent->getChildIndex(this);
}

// --------------------------------------------------------------------------
//
void VGeoFrame::setFrame(const VFrame& frame)
{
	if (usingTheWindow) {
		usingTheWindow = 0;
		theGeometry = new Geometry(frame);
	}
	else if (usingTheWindowIndex) {
		usingTheWindowIndex = 0;
		theGeometry = new Geometry(frame);
	}
	else if (theGeometry == 0) {
		theGeometry = new Geometry(frame);
	}
	else {
		theGeometry->setFrame(frame);
	}
}

// --------------------------------------------------------------------------
// Return a pointer to the VFrame of this VGeoFrame or its associated VWindow.
//
VFrame *VGeoFrame::getFrame() const
{
	if (usingTheWindow) {
		if (theWindow != 0) {
			return &theWindow->winFrame;
		}
	}
	else if (!usingTheWindowIndex) {
		if (theGeometry != 0) {
			return theGeometry->frame;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
boolean VGeoFrame::associateWindow(VWindow *window)
{
	if (window == 0) {
		if (usingTheWindow) {
			if (theWindow != 0) {
				theWindow->geoFrame = 0;
				theWindow = 0;
			}
			usingTheWindow = 0;
		}
		else if (usingTheWindowIndex) {
			usingTheWindowIndex = 0;
		}
	}
	else {
		if (usingTheWindow) {
			if (theWindow != 0) {
				theWindow->geoFrame = 0;
			}
		}
		else if (!usingTheWindowIndex) {
			if (theGeometry != 0) {
				delete theGeometry;
			}
			usingTheWindow = 1;
		}
		else {
			usingTheWindowIndex = 0;
			usingTheWindow = 1;
		}
		window->geoFrame = this;
		theWindow = window;
		windowAssociated(window);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VGeoFrame::windowAssociated(VWindow *)
{
}

// --------------------------------------------------------------------------
//
boolean VGeoFrame::verifyManager(VWindow *parent_window)
{
	if (parent_window == 0) {
		return FALSE;
	}
	else if ((parent_window->geoFrame == 0) ||
			 !parent_window->geoFrame->isGeoManager) {
		VGeoManager *manager = new VGeoForm;
		return manager->associateWindow(parent_window);
	}
	else {
		return TRUE;
	}
}

// --------------------------------------------------------------------------
// Set the size of this VGeoFrame to its preferred or minimum size
// depending on the given minimum flag.
//
void VGeoFrame::frame(boolean minimum)
{
	int w, h;
	getSize(&w, &h, minimum);
	setPhysicalSize(w, h);
}

// --------------------------------------------------------------------------
// Mark this VGeoFrame as having had some sort of change which would effect
// the geometry management of it, e.g. a change of font, text, other
// contents, its VFrame, or its opinion of its preferred or minimum size.
// This simply involves marking any cached data as being invalid.  If the
// given child is null or is not a child of this VGeoFrame then do nothing.
//
void VGeoFrame::update(int propagate_up)
{
	preferredWidthCache.setInvalid();
	preferredHeightCache.setInvalid();
	if (theParent != 0) {
		theParent->childUpdated(this);
		if (propagate_up != 0) {
			theParent->forceFrameChildren = 1;
			theParent->update(propagate_up);
		}
	}
}

// --------------------------------------------------------------------------
//
boolean VGeoFrame::hasChildren() const
{
	if (isGeoManager) {
		return ((VGeoManager *)this)->hasChildren();
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
// Return in (*w, *h) the preferred size for this VGeoFrame.
// This may be overidden by derived classes.
//
void VGeoFrame::getPreferredSize(int *w, int *h)
{
	if (usingTheWindow && (theWindow != 0)) {
		theWindow->getPreferredSize(w, h);
		addNonClientAreaSize(w, h);
	}
	else {
		if (w != 0) { *w = 16; }
		if (h != 0) { *h = 16; }
	}
}

// --------------------------------------------------------------------------
// Return in (*w, *h) the preferred size for this VGeoFrame.
// This may be overidden by derived classes.
//
boolean VGeoFrame::getConstrainedSize(int *w, int *h,
									  int constrained_w, int constrained_h)
{
	if (usingTheWindow && (theWindow != 0)) {
		if (theWindow->
			getConstrainedSize(w, h, constrained_w, constrained_h)) {
			addNonClientAreaSize(w, h);
			return TRUE;
		}
	}
	return FALSE;
}

// --------------------------------------------------------------------------
// Return in (*w, *h) the minimum desired size for this VGeoFrame.
// This may be overidden by derived classes.
//
void VGeoFrame::getMinimumSize(int *w, int *h)
{
	if (usingTheWindow && (theWindow != 0)) {
		theWindow->getMinimumSize(w, h);
		addNonClientAreaSize(w, h);
	}
	else {
		if (w != 0) { *w = 8; }
		if (h != 0) { *h = 8; }
	}
}

// --------------------------------------------------------------------------
// This is currently not used.
//
void VGeoFrame::getMaximumSize(int *w, int *h)
{
	if (w != 0) {
		*w = 32767;
	}
	if (h != 0) {
		*h = 32767;
	}
}

// --------------------------------------------------------------------------
//
void VGeoFrame::addNonClientAreaSize(int *w, int *h)
{
	VWindow *window = getWindow();

	if (window == 0) {
		return;
	}

	int width, height, left, right, top, bottom;

	if (w != 0) {
		if (h != 0) {
			window->getSystemBorderSize(&width, &height);
			if (isGeoManager) {
				((VGeoManager *)this)->
					getInternalBorderGeometry(&left, &right, &top, &bottom);
				*w += width + left + right;
				*h += height + top + bottom;
			}
			else {
				*w += width;
				*h += height;
			}
		}
		else {
			window->getSystemBorderSize(&width, 0);
			if (isGeoManager) {
				((VGeoManager *)this)->
					getInternalBorderGeometry(&left, &right, 0, 0);
				*w += width + left + right;
			}
			else {
				*w += width;
			}
		}
	}
	else if (h != 0) {
		window->getSystemBorderSize(0, &height);
		if (isGeoManager) {
			((VGeoManager *)this)->
				getInternalBorderGeometry(0, 0, &top, &bottom);
			*h += height + top + bottom;
		}
		else {
			*h += height;
		}
	}
}

// --------------------------------------------------------------------------
// Create a ChildData object which can be used to contain the child specific
// data for any child of this VGeoFrame.  This is meant to be overidden by
// derived VGeoFrame classes and to return new a new instance of and object
// derived from the ChildData class.
//
VGeoFrame::ChildData *VGeoFrame::newChildData() const
{
	return 0;
}

// --------------------------------------------------------------------------
//
void VGeoFrame::determinePreferredSize(int *w, int *h)
{
	getSize(w, h);
}

// --------------------------------------------------------------------------
//
void VGeoFrame::determineConstrainedSize(int *w, int *h,
										 int constrained_w,
										 int constrained_h)
{
	getSize(w, h, FALSE, constrained_w, constrained_h);
}

// --------------------------------------------------------------------------
//
void VGeoFrame::determineMinimumSize(int *w, int *h)
{
	getSize(w, h, TRUE);
}

// --------------------------------------------------------------------------
//
void VGeoFrame::determineMaximumSize(int *w, int *h)
{
	if (w != 0) {
		*w = 32767;
	}
	if (h != 0) {
		*h = 32767;
	}
}

// --------------------------------------------------------------------------
// Return in (*w, *h) the desired width and height of this VGeoFrame.
// The desired width and height is that which is contained within the
// associated VFrame if any and if not zero, otherwise, it is that which
// is obtained from a call to getPreferredSize() or getMinimumSize()
// depending on the given minimum flag.  If the width or height is
// dependent upon a ratio, it will be assumed to be zero (in which case
// getPreferredSize() or getMinimumSize() will be called to obtain the
// value of the dimension).  The intended width/height arguments are
// simply passed on to getpreferredSize() if it is called.
// If any of (w, h) are null, then they will be ignored.
//
void VGeoFrame::getSize(int *w, int *h,
					    boolean minimum,
					    int constrained_w, int constrained_h)
{
	int mode;

	if (minimum) {
		mode = GetMinimumPreferredSize | ForcePreferredSize |
			   GetFontSize | NoRatios;
	}
	else if ((constrained_w == 0) && (constrained_h == 0)) {
		mode = GetSimplePreferredSize | GetFontSize | NoRatios;
	}
	else {
		if (w != 0) {
			if (h != 0) {
				GeoComponent gcw, gch;
				getGeometry(0, 0, &gcw, &gch,
							NoGetPreferredSize | NoGetFontSize | NoRatios);
				if (gcw.isPreferredSizeNeeded()) {
					if (gch.isPreferredSizeNeeded()) {
						completeGeometry
							(0, 0, &gcw, &gch,
							 GetConstrainedPreferredSize | GetFontSize,
							 constrained_w, constrained_h);
						preferredHeightCache = gch;
					}
					else {
						completeGeometry
							(0, 0, &gcw, 0,
							 GetConstrainedPreferredSize | GetFontSize,
							 constrained_w, constrained_h);
						if (!preferredHeightCache.isComplete(FALSE)) {
							completeGeometry
								(0, 0, 0, &gch,
								 GetSimplePreferredSize | GetFontSize);
							preferredHeightCache = gch;
						}
					}
					preferredWidthCache = gcw;
				}
				else if (gch.isPreferredSizeNeeded()) {
					if (!preferredWidthCache.isComplete(FALSE)) {
						completeGeometry
							(0, 0, &gcw, 0,
							 GetSimplePreferredSize | GetFontSize);
						preferredWidthCache = gcw;
					}
					completeGeometry
						(0, 0, 0, &gch, 
						 GetConstrainedPreferredSize | GetFontSize,
						 constrained_w, constrained_h);
					preferredHeightCache = gch;
				}
				else {
					if (!preferredWidthCache.isComplete(FALSE)) {
						if (!preferredHeightCache.isComplete(FALSE)) {
							completeGeometry
								(0, 0, &gcw, &gch,
								 GetSimplePreferredSize | GetFontSize);
							preferredHeightCache = gch;
						}
						else {
							completeGeometry
								(0, 0, &gcw, 0,
								 GetSimplePreferredSize | GetFontSize);
						}
						preferredWidthCache = gcw;
					}
					else if (!preferredHeightCache.isComplete(FALSE)) {
						completeGeometry
							(0, 0, 0, &gch,
							 GetSimplePreferredSize | GetFontSize);
						preferredHeightCache = gch;
					}
				}
				*w = preferredWidthCache;
				*h = preferredHeightCache;
			}
			else {
				GeoComponent gcw;
				getGeometry(0, 0, &gcw, 0,
							NoGetPreferredSize | NoGetFontSize | NoRatios);
				if (gcw.isPreferredSizeNeeded()) {
					completeGeometry
						(0, 0, &gcw, 0,
						 GetConstrainedPreferredSize | GetFontSize,
						 constrained_w, constrained_h);
					preferredWidthCache = gcw;
				}
				else if (!preferredWidthCache.isComplete(FALSE)) {
					completeGeometry
						(0, 0, &gcw, 0,
						 GetSimplePreferredSize | GetFontSize);
					preferredWidthCache = gcw;
				}
				*w = preferredWidthCache;
			}
		}
		else if (h != 0) {
			GeoComponent gch;
			getGeometry(0, 0, 0, &gch,
						NoGetPreferredSize | NoGetFontSize | NoRatios);
			if (gch.isPreferredSizeNeeded()) {
				completeGeometry
					(0, 0, 0, &gch,
					 GetConstrainedPreferredSize | GetFontSize,
					 constrained_w, constrained_h);
				preferredHeightCache = gch;
			}
			else if (!preferredHeightCache.isComplete(FALSE)) {
				completeGeometry
					(0, 0, 0, &gch,
					 GetSimplePreferredSize | GetFontSize);
				preferredHeightCache = gch;
			}
			*h = preferredHeightCache;
		}
		return;
	}

#if 1
	if (w != 0) {
		if (h != 0) {
			if (!preferredWidthCache.isComplete(minimum)) {
				if (!preferredHeightCache.isComplete(minimum)) {
					GeoComponent gcw, gch;
					getGeometry(0, 0, &gcw, &gch, mode);
					preferredWidthCache = gcw;
					preferredHeightCache = gch;
				}
				else {
					GeoComponent gcw;
					getGeometry(0, 0, &gcw, 0, mode);
					preferredWidthCache = gcw;
				}
			}
			else if (!preferredHeightCache.isComplete(minimum)) {
				GeoComponent gch;
				getGeometry(0, 0, 0, &gch, mode);
				preferredHeightCache = gch;
			}
			*h = preferredHeightCache;
		}
		else if (!preferredWidthCache.isComplete(minimum)) {
			GeoComponent gcw;
			getGeometry(0, 0, &gcw, 0, mode);
			preferredWidthCache = gcw;
		}
		*w = preferredWidthCache;
	}
	else if (h != 0) {
		if (!preferredHeightCache.isComplete(minimum)) {
			GeoComponent gch;
			getGeometry(0, 0, 0, &gch, mode);
			preferredHeightCache = gch;
		}
		*h = preferredHeightCache;
	}

#else

	int preferred_width_cache_ok, preferred_height_cache_ok;

	if (minimum) {
		mode = GetMinimumPreferredSize | ForcePreferredSize |
			   NoRatios | GetFontSize;
		preferred_width_cache_ok = preferredWidthCache.isComplete(TRUE);
		preferred_height_cache_ok = preferredHeightCache.isComplete(TRUE);
	}
	else if ((constrained_w > 0) || (constrained_h > 0)) {
		mode = GetConstrainedPreferredSize | NoRatios | GetFontSize;
		preferred_width_cache_ok = 0;
		preferred_height_cache_ok = 0;
	}
	else {
		mode = GetSimplePreferredSize | NoRatios | GetFontSize;
		preferred_width_cache_ok = preferredWidthCache.isComplete(FALSE);
		preferred_height_cache_ok = preferredHeightCache.isComplete(FALSE);
	}

	if (w != 0) {
		if (h != 0) {
			if (!preferred_width_cache_ok) {
				if (!preferred_height_cache_ok) {
					GeoComponent gcw, gch;
					getGeometry(0, 0, &gcw, &gch,
							    mode, constrained_w, constrained_h);
					preferredWidthCache = gcw;
					preferredHeightCache = gch;
				}
				else {
					GeoComponent gcw;
					getGeometry(0, 0, &gcw, 0,
							    mode, constrained_w, constrained_h);
					preferredWidthCache = gcw;
				}
			}
			else if (!preferred_height_cache_ok) {
				GeoComponent gch;
				getGeometry(0, 0, 0, &gch,
						    mode, constrained_w, constrained_h);
				preferredHeightCache = gch;
			}
			*w = preferredWidthCache;
			*h = preferredHeightCache;
		}
		else {
			if (!preferred_width_cache_ok) {
				GeoComponent gcw;
				getGeometry(0, 0, &gcw, 0,
						    mode, constrained_w, constrained_h);
				preferredWidthCache = gcw;
			}
			*w = preferredWidthCache;
		}
	}
	else if (h != 0) {
		if (!preferred_height_cache_ok) {
			GeoComponent gch;
			getGeometry(0, 0, 0, &gch,
					    mode, constrained_w, constrained_h);
			preferredHeightCache = gch;
		}
		*h = preferredHeightCache;
	}
#endif
}

// --------------------------------------------------------------------------
// Return in (*x, *y, *w, *h) the requested geometry of this VGeoFrame.
// The requested geometry is that which is contained within this VGeoFrame's
// VFrame or associated VWindow's VFrame's.  If either or both the width
// and height specified in the VFrame are zero, then the virtual member-
// function VGeoFrame::getPreferredSize() will be called to obtain the
// preferred value for the dimension.  If any of (x, y, w, h) are null,
// then they will be ignored.  If the noratio flag is set, then ratios
// will not be allowed; in this case, if a position (x, y) is found to
// involve a ratio it is set to zero, and if a dimension (w, h) is found
// to involve a ratio its preferred dimension will be queried.
//
// If the given minimum flag is set, then, the VFrame will *not* be
// examined as described above to obtain the width and height, rather
// the virtual member-function VGeoFrame::getMinimumSize() will be called
// to obtain these values.
//
// Note that care is taken so that neither VGeoFrame::getPreferredSize()
// nor VGeoFrame::getMinimumSize() will be called more than once from
// this function.
//
// Note that VGeoFrame::getPreferredSize() or VGeoFrame::getMinimumSize()
// may be called to obtain a width/height even if the given w/h pointers
// are null, if for example, the VFrame specifies CenterDim in which
// case the position is dependent upon the dimension.
//
// If the width of this VGeoFrame involves a ratio and the height is null
// or if the height involves a ratio and the width is null, and the given
// noratio flag is not set, and the given minimum flag is not set, then the
// call to VGeoFrame::getPreferredSize() which will be made to obtain the
// desired value of the null dimension, will be accompanied by a third
// argument which represents the intended value of the dimension which
// involves the ratio.  And since the dimension involving ratio is not
// sought in this case, a null pointer will be passed to VGeoFrame::
// getPreferredSize() for that dimension.
//
// FYI, this kind of call to VGeoFrame::getPreferredSize() should be done
// even if the corresponding cache is invalid, although this function is
// not responsible for ensuring this; it should be done earlier.
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Fill in the given VGeoComponents from the geometry information contained
// within this VGeoFrame object.  The following values (only) may be ORed
// together and specified as the flags argument:
//
//		NoRatios
//		NoGetPreferredSize
//		NoGetFontSize
//		Ratios							--> default
//		GetSimplePreferredSize			--> default
//		GetConstrainedPreferredSize
//		GetMinimumPreferredSize
//		GetFontSize						--> default
//		ForcePreferredSize
//
void VGeoFrame::getGeometry(VGeoFrame::GeoComponent *x,
							VGeoFrame::GeoComponent *y,
						    VGeoFrame::GeoComponent *w,
							VGeoFrame::GeoComponent *h,
						    int flags,
						    int constrained_w, int constrained_h)
{
	VFrame *frame;
	VRatio xr, yr, wr, hr;
	int xi, yi, wi, hi;
	float f;
	RectOpts ro;

#if 0
	if ((frame = getFrame()) == 0) {
		return;
	}
	frame->get(xr, yr, wr, hr, ro);
#else
	if ((frame = getFrame()) == 0) {
		xr = yr = wr = hr = 0;
		ro = CornerDim;
	}
	else {
		frame->get(xr, yr, wr, hr, ro);
	}
#endif


	// Hack for brain-damaged VComboBox whose (VFrame) size is defined
	// to represent the *total* height, i.e. including the drop-down!

	if (h != 0) {
		VWindow *window = getWindow();
		if ((window != 0) && window->isA(VComboBoxCls)) {
			VComboBox *combobox = (VComboBox *)window;
			if (!combobox->getStyle().contains(StyleComboEdit)) {
				int combobox_edit_height = combobox->getEditHeight();
				if (combobox_edit_height > 0) {
					h->set(combobox_edit_height);
					h->setValid();
					h = 0;
				}
			}
		}
	}

	switch (ro) {

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//
	case CornerDim:
	{
		if (x != 0) {
			if (xr.isRatio()) {
				if (flags & NoRatios) {
					x->set(0);
				}
				else {
					xr.asFloat(f);
					x->set(f);
				}
			}
			else {
				xi = int(xr);
				x->set(xi);
				if ((frame != 0) && (frame->getUnits() == VFrame::Font16)) {
					x->setFontSizeNeeded();
				}
			}
			x->setValid();
		}
		if (w != 0) {
			if (flags & ForcePreferredSize) {
				w->setPreferredSizeNeeded();
			}
			else if (wr.isRatio()) {
				if (flags & NoRatios) {
					w->set(0);
					w->setPreferredSizeNeeded();
				}
				else {
					wr.asFloat(f);
					w->set(f);
				}
			}
			else {
				wi = int(wr);
				if (wi == 0) {
					w->setPreferredSizeNeeded();
				}
				else {
					w->set(wi);
					if ((frame != 0) &&
						(frame->getUnits() == VFrame::Font16)) {
						w->setFontSizeNeeded();
					}
				}
			}
			w->setValid();
		}
		if (y != 0) {
			if (yr.isRatio()) {
				if (flags & NoRatios) {
					y->set(0);
					y->setPreferredSizeNeeded();
				}
				else {
					yr.asFloat(f);
					y->set(f);
				}
			}
			else {
				yi = int(yr);
				y->set(yi);
				if ((frame != 0) && (frame->getUnits() == VFrame::Font16)) {
					y->setFontSizeNeeded();
				}
			}
			y->setValid();
		}
		if (h != 0) {
			if (flags & ForcePreferredSize) {
				h->setPreferredSizeNeeded();
			}
			else if (hr.isRatio()) {
				if (flags & NoRatios) {
					h->set(0);
					h->setPreferredSizeNeeded();
				}
				else {
					hr.asFloat(f);
					h->set(f);
				}
			}
			else {
				hi = int(hr);
				if (hi == 0) {
					h->setPreferredSizeNeeded();
				}
				else {
					h->set(hi);
					if ((frame != 0) &&
						(frame->getUnits() == VFrame::Font16)) {
						h->setFontSizeNeeded();
					}
				}
			}
			h->setValid();
		}
		break;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//
	case CenterDim:
	{
		GeoComponent gcw, gch;

		if ((x != 0) && (w == 0)) {
			w = &gcw;
		}
		if (w != 0) {
			if (flags & ForcePreferredSize) {
				w->setPreferredSizeNeeded();
			}
			else if (wr.isRatio()) {
				if (flags & NoRatios) {
					w->set(0);
					w->setPreferredSizeNeeded();
				}
				else {
					wr.asFloat(f);
					w->set(f);
				}
			}
			else {
				wi = int(wr);
				if (wi == 0) {
					w->setPreferredSizeNeeded();
				}
				else {
					w->set(wi);
					if ((frame != 0) &&
						(frame->getUnits() == VFrame::Font16)) {
						w->setFontSizeNeeded();
					}
				}
			}
			w->setValid();
		}
		if (x != 0) {
			if (xr.isRatio()) {
				if (flags & NoRatios) {
					x->set(0);
				}
				else {
					xr.asFloat(f);
					x->set(f);
				}
			}
			else {
				x->set(int(xr));
				if ((frame != 0) && (frame->getUnits() == VFrame::Font16)) {
					x->setFontSizeNeeded();
				}
			}
			if (w->isPreferredSizeNeeded()) {
				x->setPreferredSizeNeeded();
			}
			else {
				if (w->isFontSizeNeeded()) {
					x->setFontSizeNeeded();
				}
				x->setRatio(x->getRatio() - (w->getRatio() / 2));
				x->setOffset(x->getOffset() -
							 int(float(w->getOffset()) / 2.0F + 0.5F));
			}
			x->setValid();
		}
		if ((y != 0) && (h == 0)) {
			h = &gch;
		}
		if (h != 0) {
			if (flags & ForcePreferredSize) {
				h->setPreferredSizeNeeded();
			}
			else if (hr.isRatio()) {
				if (flags & NoRatios) {
					h->set(0);
					h->setPreferredSizeNeeded();
				}
				else {
					hr.asFloat(f);
					h->set(f);
				}
			}
			else {
				hi = int(hr);
				if (hi == 0) {
					h->setPreferredSizeNeeded();
				}
				else {
					h->set(hi);
					if ((frame != 0) &&
						(frame->getUnits() == VFrame::Font16)) {
						h->setFontSizeNeeded();
					}
				}
			}
			h->setValid();
		}
		if (y != 0) {
			if (yr.isRatio()) {
				if (flags & NoRatios) {
					y->set(0);
				}
				else {
					yr.asFloat(f);
					y->set(f);
				}
			}
			else {
				y->set(int(yr));
				if ((frame != 0) && (frame->getUnits() == VFrame::Font16)) {
					y->setFontSizeNeeded();
				}
			}
			if (h->isPreferredSizeNeeded()) {
				y->setPreferredSizeNeeded();
			}
			else {
				if (h->isFontSizeNeeded()) {
					y->setFontSizeNeeded();
				}
				y->setRatio(y->getRatio() - (h->getRatio() / 2));
				y->setOffset(y->getOffset() -
							 int(float(h->getOffset()) / 2.0F + 0.5F));
			}
			y->setValid();
		}
		break;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//
	case Corners:
	{
		GeoComponent gcx, gcy;

		if ((w != 0) && (x == 0)) {
			x = &gcx;
		}
		if (x != 0) {
			if (xr.isRatio()) {
				if (flags & NoRatios) {
					x->set(0);
				}
				else {
					xr.asFloat(f);
					x->set(f);
				}
			}
			else {
				x->set(int(xr));
				if ((frame != 0) && (frame->getUnits() == VFrame::Font16)) {
					x->setFontSizeNeeded();
				}
			}
			x->setValid();
		}
		if (w != 0) {
			if (flags & ForcePreferredSize) {
				w->setPreferredSizeNeeded();
			}
			else if (wr.isRatio()) {
				if (flags & NoRatios) {
					w->set(0);
					w->setPreferredSizeNeeded();
				}
				else {
					wr.asFloat(f);
					w->set(f);
				}
			}
			else {
				wi = int(wr);
				if (wi == 0) {
					w->setPreferredSizeNeeded();
				}
				else {
					w->set(wi);
					if ((frame != 0) &&
						(frame->getUnits() == VFrame::Font16)) {
						w->setFontSizeNeeded();
					}
				}
			}
			if (!w->isPreferredSizeNeeded()) {
				w->setOffset(w->getOffset() - x->getOffset());
				w->setRatio(w->getRatio() - x->getRatio());
			}
			w->setValid();
		}
		if ((h != 0) && (y == 0)) {
			y = &gcy;
		}
		if (y != 0) {
			if (yr.isRatio()) {
				if (flags & NoRatios) {
					y->set(0);
				}
				else {
					yr.asFloat(f);
					y->set(f);
				}
			}
			else {
				y->set(int(yr));
				if ((frame != 0) && (frame->getUnits() == VFrame::Font16)) {
					y->setFontSizeNeeded();
				}
			}
			y->setValid();
		}
		if (h != 0) {
			if (flags & ForcePreferredSize) {
				h->setPreferredSizeNeeded();
			}
			else if (hr.isRatio()) {
				if (flags & NoRatios) {
					h->set(0);
					h->setPreferredSizeNeeded();
				}
				else {
					hr.asFloat(f);
					h->set(f);
				}
			}
			else {
				hi = int(hr);
				if (hi == 0) {
					h->setPreferredSizeNeeded();
				}
				else {
					h->set(hi);
					if ((frame != 0) &&
						(frame->getUnits() == VFrame::Font16)) {
						h->setFontSizeNeeded();
					}
				}
			}
			if (!h->isPreferredSizeNeeded()) {
				h->setOffset(h->getOffset() - y->getOffset());
				h->setRatio(h->getRatio() - y->getRatio());
			}
			h->setValid();
		}
		break;
	}}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	// N.B. Default to GetSimplePreferredSize and GetFontSize.

	if (flags & NoGetPreferredSize) {
		if (!(flags & NoGetFontSize)) {
			completeGeometry(x, y, w, h, NoGetPreferredSize | GetFontSize);
		}
	}
	else {
		completeGeometry(x, y, w, h, flags, constrained_w, constrained_h);
	}
}

// --------------------------------------------------------------------------
// Complete the given VGeoComponents; this essentially means query for the
// preferred size (simple, constrained, or minimum) and the font-size if
// necessary.
//
// The flags parameter may contain the following values (only) ORed together:
//
//		GetFontSize					--> default
//		GetPreferredSize			--> default
//		GetSimplePreferredSize		--> default
//		GetConstrainedPreferredSize
//		NoGetPreferredSize
//		NoGetFontSize
//
void VGeoFrame::completeGeometry(VGeoFrame::GeoComponent *x,
								 VGeoFrame::GeoComponent *y,
								 VGeoFrame::GeoComponent *w,
								 VGeoFrame::GeoComponent *h,
								 int flags, 
								 int constrained_w, int constrained_h)
{

	int get_preferred_width, get_preferred_height;
	int get_preferred_width_for_x, get_preferred_height_for_y;
	int get_font_width, get_font_height;
	int get_font_width_for_x, get_font_height_for_y;
	int pw, ph, fh, fw, dummy;

	int preferred	= !(flags & NoGetPreferredSize);
	int constrained	=  (flags & GetConstrainedPreferredSize) ==
							    GetConstrainedPreferredSize;
	int minimum		=  (flags & GetMinimumPreferredSize) ==
							    GetMinimumPreferredSize;
	int font		= !(flags & NoGetFontSize);

	// First, see if, and for whom, we need to
	// obtain the preferred-size and/or font-size.

	if (w != 0) {
		get_preferred_width =
			preferred && w->isPreferredSizeNeeded() &&
			(constrained || (minimum && !w->isMinimumPreferredSizeDone()) ||
							(!minimum && !w->isSimplePreferredSizeDone()));
		get_font_width =
			font && !get_preferred_width &&
			w->isFontSizeNeeded() && !w->isFontSizeDone();
	}
	else {
		get_preferred_width = 0;
		get_font_width = 0;
	}
	if (x != 0) {
		//
		// N.B. The (x,y) position can be dependent on preferred-size
		// only in the case of CenterDim where the position depends on
		// the dimension, and when the dimension requires a preferred-size
		// query; the position in this case is value of the position minus
		// half the value of the dimension.
		//
		get_preferred_width_for_x =
			preferred && x->isPreferredSizeNeeded() &&
			(constrained || (minimum && !x->isMinimumPreferredSizeDone()) ||
							(!minimum && !x->isSimplePreferredSizeDone()));
		get_font_width_for_x =
			font && x->isFontSizeNeeded() && !x->isFontSizeDone();
	}
	else {
		get_preferred_width_for_x = 0;
		get_font_width_for_x = 0;
	}

	if (h != 0) {
		get_preferred_height =
			preferred && h->isPreferredSizeNeeded() &&
			(constrained || (minimum && !h->isMinimumPreferredSizeDone()) ||
							(!minimum && !h->isSimplePreferredSizeDone()));
		get_font_height =
			font && !get_preferred_height &&
			h->isFontSizeNeeded() && !h->isFontSizeDone();
	}
	else {
		get_preferred_height = 0;
		get_font_height = 0;
	}
	if (y != 0) {
		//
		// See N.B. for x above.
		//
		get_preferred_height_for_y =
			preferred && y->isPreferredSizeNeeded() &&
			(constrained || (minimum && !y->isMinimumPreferredSizeDone()) ||
							(!minimum && !y->isSimplePreferredSizeDone()));
		get_font_height_for_y =
			font && y->isFontSizeNeeded() && !y->isFontSizeDone();
	}
	else {
		get_preferred_height_for_y = 0;
		get_font_height_for_y = 0;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Note that code is a bit cumbersome because we want to minimize
	// calls to getPreferredSize(), getMinimumSize(), and/or fontSize().
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	// Obtain the preferred-size if necessary.

	if (get_preferred_width || get_preferred_width_for_x) {
		if (get_preferred_height || get_preferred_height_for_y) {
			if (constrained) {
				if (!getConstrainedSize
					(&pw, &ph, constrained_w, constrained_h)) {
					getPreferredSize(&pw, &ph);
				}
				if (get_preferred_width) {
					w->setConstrainedPreferredSizeDone();
					w->setOffset(pw);
				}
				if (get_preferred_width_for_x) {
					x->setOffset(x->getOffset() -
								 int(float(pw) / 2.0F + 0.5F));
					x->setConstrainedPreferredSizeDone();
				}
				if (get_preferred_height) {
					h->setConstrainedPreferredSizeDone();
					h->setOffset(ph);
				}
				if (get_preferred_height_for_y) {
					y->setOffset(y->getOffset() -
								 int(float(ph) / 2.0F + 0.5F));
					y->setConstrainedPreferredSizeDone();
				}
			}
			else if (minimum) {
				getMinimumSize(&pw, &ph);
				if (get_preferred_width) {
					w->setMinimumPreferredSizeDone();
					w->setOffset(pw);
				}
				if (get_preferred_height) {
					h->setMinimumPreferredSizeDone();
					h->setOffset(ph);
				}
				if (get_preferred_width_for_x) {
					x->setOffset(x->getOffset() -
								 int(float(pw) / 2.0F + 0.5F));
					x->setMinimumPreferredSizeDone();
				}
				if (get_preferred_height_for_y) {
					y->setOffset(y->getOffset() -
								 int(float(ph) / 2.0F + 0.5F));
					y->setMinimumPreferredSizeDone();
				}
			}
			else {
				getPreferredSize(&pw, &ph);
				if (get_preferred_width) {
					w->setSimplePreferredSizeDone();
					w->setOffset(pw);
				}
				if (get_preferred_height) {
					h->setSimplePreferredSizeDone();
					h->setOffset(ph);
				}
				if (get_preferred_width_for_x) {
					x->setOffset(x->getOffset() -
								 int(float(pw) / 2.0F + 0.5F));
					x->setSimplePreferredSizeDone();
				}
				if (get_preferred_height_for_y) {
					y->setOffset(y->getOffset() -
								 int(float(ph) / 2.0F + 0.5F));
					y->setSimplePreferredSizeDone();
				}
			}
		}
		else {
			if (constrained) {
				if (!getConstrainedSize
					(&pw, 0, constrained_w, constrained_h)) {
					getPreferredSize(&pw, 0);
				}
				if (get_preferred_width) {
					w->setConstrainedPreferredSizeDone();
					w->setOffset(pw);
				}
				if (get_preferred_width_for_x) {
					x->setOffset(x->getOffset() -
								 int(float(pw) / 2.0F + 0.5F));
					x->setConstrainedPreferredSizeDone();
				}
			}
			else if (minimum) {
				getMinimumSize(&pw, 0);
				if (get_preferred_width) {
					w->setMinimumPreferredSizeDone();
					w->setOffset(pw);
				}
				if (get_preferred_width_for_x) {
					x->setOffset(x->getOffset() -
								 int(float(pw) / 2.0F + 0.5F));
					x->setMinimumPreferredSizeDone();
				}
			}
			else {
				getPreferredSize(&pw, 0);
				if (get_preferred_width) {
					w->setSimplePreferredSizeDone();
					w->setOffset(pw);
				}
				if (get_preferred_width_for_x) {
					x->setOffset(x->getOffset() -
								 int(float(pw) / 2.0F + 0.5F));
					x->setSimplePreferredSizeDone();
				}
			}
		}
	}
	else if (get_preferred_height || get_preferred_height_for_y) {
		if (constrained) {
			if (!getConstrainedSize
				(0, &ph, constrained_w, constrained_h)) {
				getPreferredSize(0, &ph);
			}
			if (get_preferred_height) {
				h->setConstrainedPreferredSizeDone();
				h->setOffset(ph);
			}
			if (get_preferred_height_for_y) {
				y->setOffset(y->getOffset() -
							 int(float(ph) / 2.0F + 0.5F));
				y->setConstrainedPreferredSizeDone();
			}
		}
		else if (minimum) {
			getMinimumSize(0, &ph);
			if (get_preferred_height) {
				h->setMinimumPreferredSizeDone();
				h->setOffset(ph);
			}
			if (get_preferred_height_for_y) {
				y->setOffset(y->getOffset() -
							 int(float(ph) / 2.0F + 0.5F));
				y->setMinimumPreferredSizeDone();
			}
		}
		else {
			getPreferredSize(0, &ph);
			if (get_preferred_height) {
				h->setSimplePreferredSizeDone();
				h->setOffset(ph);
			}
			if (get_preferred_height_for_y) {
				y->setOffset(y->getOffset() -
							 int(float(ph) / 2.0F + 0.5F));
				y->setSimplePreferredSizeDone();
			}
		}
	}

	// Obtain the font-size if necessary.

	if (get_font_width || get_font_width_for_x) {
		if (get_font_height || get_font_height_for_y) {
			if (usingTheWindow && (theWindow != 0)) {
				theWindow->fontSize(&fw, &fh);
				if (get_font_width) {
					w->setOffset(font16ToPixels(w->getOffset(), fw));
					w->setFontSizeDone();
				}
				if (get_font_width_for_x) {
					x->setOffset(font16ToPixels(x->getOffset(), fw));
					x->setFontSizeDone();
				}
				if (get_font_height) {
					h->setOffset(font16ToPixels(h->getOffset(), fh));
					h->setFontSizeDone();
				}
				if (get_font_height_for_y) {
					y->setOffset(font16ToPixels(y->getOffset(), fh));
					y->setFontSizeDone();
				}
			}
		}
		else if (usingTheWindow && (theWindow != 0)) {
			theWindow->fontSize(&fw, &dummy);
			if (get_font_width) {
				w->setOffset(font16ToPixels(w->getOffset(), fw));
				w->setFontSizeDone();
			}
			if (get_font_width_for_x) {
				x->setOffset(font16ToPixels(x->getOffset(), fw));
				x->setFontSizeDone();
			}
		}
	}
	else if (get_font_height || get_font_height_for_y) {
		if (usingTheWindow && (theWindow != 0)) {
			theWindow->fontSize(&dummy, &fh);
			if (get_font_height) {
				h->setOffset(font16ToPixels(h->getOffset(), fh));
				h->setFontSizeDone();
			}
			if (get_font_height_for_y) {
				y->setOffset(font16ToPixels(y->getOffset(), fh));
				y->setFontSizeDone();
			}
		}
	}
}

// --------------------------------------------------------------------------
// Return a pointer to the VWindow of the nearest ancestor of this VGeoFrame
// which has an associated VWindow.  Do not count this VGeoFrame's associated
// VWindow, if any.
//
VWindow *VGeoFrame::getParentWindow() const
{
	for (VGeoManager *gm = theParent ; gm != 0 ; gm = gm->theParent) {
		if (gm->usingTheWindow && (gm->theWindow != 0)) {
			return gm->theWindow;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
// Translate the given (x, y) location to the equivalent location relative
// to the nearest ancestor of this VGeoFrame which has an associated VWindow.
//
void VGeoFrame::translateToWindowLocation(int& x, int& y)
{
	for (VGeoManager *gm = theParent ;
		 (gm != 0) && !gm->usingTheWindow ; gm = gm->theParent) {
		if (gm->theGeometry != 0) {
			x += gm->theGeometry->x;
			y += gm->theGeometry->y;
		}
	}
}

// --------------------------------------------------------------------------
//
void VGeoFrame::getPhysicalClientSize(int *w, int *h)
{
	if (usingTheWindow) {
		if (theWindow != 0) {
			int cw, ch;
			theWindow->sizeOfImage(&cw, &ch);
			if (w != 0) { *w = cw; }
			if (h != 0) { *h = ch; }
		}
		else {
			if (w != 0) { *w = 0; }
			if (h != 0) { *h = 0; }
		}
	}
	else if (!usingTheWindowIndex) {
		if (theGeometry == 0) {
			if (w != 0) { *w = 0; }
			if (h != 0) { *h = 0; }
		}
		else {
			if (w != 0) { *w = theGeometry->width; }
			if (h != 0) { *h = theGeometry->height; }
		}
	}
}

// --------------------------------------------------------------------------
//
void VGeoFrame::getPhysicalGeometry(int *x, int *y, int *w, int *h)
{
	if (usingTheWindow) {
		if (theWindow != 0) {
			int xx, yy, ww, hh;
			theWindow->getRel(&xx, &yy, &ww, &hh);
			if (x != 0) { *x = xx; }
			if (y != 0) { *y = yy; }
			if (w != 0) { *w = ww; }
			if (h != 0) { *h = hh; }
		}
		else {
			if (x != 0) { *x = 0; }
			if (y != 0) { *y = 0; }
			if (w != 0) { *w = 0; }
			if (h != 0) { *h = 0; }
		}
	}
	else if (usingTheWindowIndex) {
		if (x != 0) { *x = 0; }
		if (y != 0) { *y = 0; }
		if (w != 0) { *w = 0; }
		if (h != 0) { *h = 0; }
	}
	else if (theGeometry == 0) {
		if (x != 0) { *x = 0; }
		if (y != 0) { *y = 0; }
		if (w != 0) { *w = 0; }
		if (h != 0) { *h = 0; }
	}
	else {
		if (x != 0) { *x = theGeometry->x; }
		if (y != 0) { *y = theGeometry->y; }
		if (w != 0) { *w = theGeometry->width; }
		if (h != 0) { *h = theGeometry->height; }
	}
}

// --------------------------------------------------------------------------
//
void VGeoFrame::setPhysicalSize(int w, int h)
{
	if (usingTheWindow) {
		if (theWindow != 0) {
			if (theWindow->isA(VComboBoxCls)) {
				VComboBox *combobox = (VComboBox *)theWindow;
				if (!combobox->getStyle().contains(StyleComboEdit)) {
					h = combobox->getDropDownHeight();
				}
			}
			if (isGeoManager && ((VGeoManager *)this)->forceFrameChildren) {
				//
				// This will force the children of this VGeoManager
				// to be reframed via a call to frameChildren(), even
				// if the size of this window is not really being changed.
				//
				((VGeoManager *)this)->forceFrameChildren = 0;
				int current_w, current_h;
				theWindow->sizeOfWin(&current_w, &current_h);
				if ((w == current_w) && (h == current_h)) {
					int sbw, sbh;
					theWindow->getSystemBorderSize(&sbw, &sbh);
					((VGeoManager *)this)->resized(w - sbw, h - sbh);
				}
				else {
					theWindow->setGeometrySize(w, h);
				}
			}
			else {
				theWindow->setGeometrySize(w, h);
			}
		}
	}
	else if (!usingTheWindowIndex) {
		if (theGeometry == 0) {
			theGeometry = new Geometry(w, h);
			theGeometry->width = w;
			theGeometry->height = h;
			if (isGeoManager) {
				((VGeoManager *)this)->resized(w, h);
			}
		}
		else if (isGeoManager && ((VGeoManager *)this)->forceFrameChildren) {
			((VGeoManager *)this)->forceFrameChildren = 0;
			theGeometry->width = w;
			theGeometry->height = h;
			((VGeoManager *)this)->resized(w, h);
		}
		else if (theGeometry->width != w) {
			theGeometry->width = w;
			if (theGeometry->height != h) {
				theGeometry->height = h;
				((VGeoManager *)this)->resized(w, h);
			}
			else {
				((VGeoManager *)this)->resized(w, 0);
			}
		}
		else if (theGeometry->height != h) {
			theGeometry->height = h;
			((VGeoManager *)this)->resized(0, h);
		}
	}
}

// --------------------------------------------------------------------------
//
void VGeoFrame::setPhysicalGeometry(int x, int y, int w, int h)
{
	if (theParent != 0) {
		int left, top;
		theParent->getInternalBorderGeometry(&left, 0, &top, 0);
		x += left;
		y += top;
	}

	if (usingTheWindow) {
		if (theWindow != 0) {
			if ((theParent != 0) && (!theParent->usingTheWindow ||
									 (theParent->theWindow == 0))) {
				for (VGeoManager *gm = theParent ;
					 (gm != 0) && !gm->usingTheWindow ; gm = gm->theParent) {
					if (gm->theGeometry != 0) {
						x += gm->theGeometry->x;
						y += gm->theGeometry->y;
					}
				}
			}
			if (theWindow->isA(VComboBoxCls)) {
				VComboBox *combobox = (VComboBox *)theWindow;
				if (!combobox->getStyle().contains(StyleComboEdit)) {
					h = combobox->getDropDownHeight();
				}
			}
			if (isGeoManager && ((VGeoManager *)this)->forceFrameChildren) {
				//
				// This will force the children of this VGeoManager
				// to be reframed via a call to frameChildren(), even
				// if the size of this window is not really being changed.
				//
				((VGeoManager *)this)->forceFrameChildren = 0;
				int current_w, current_h;
				theWindow->getAbs(&current_w, &current_h);
				if ((w == current_w) && (h == current_h)) {
					int sbw, sbh;
					theWindow->getSystemBorderSize(&sbw, &sbh);
					((VGeoManager *)this)->resized(w - sbw, h - sbh);
				}
				else {
					theWindow->setGeometry(x, y, w, h);
				}
			}
			else {
				theWindow->setGeometry(x, y, w, h);
			}
		}
	}
	else if (!usingTheWindowIndex) {
		if (theGeometry == 0) {
			theGeometry = new Geometry;
		}
		if ((theGeometry->x != x) || (theGeometry->y != y)) {
			int xoffset = x - theGeometry->x;
			int yoffset = y - theGeometry->y;
			if (isGeoManager) {
				moveWindowChildren(xoffset, yoffset);
			}
			theGeometry->x = short(x);
			theGeometry->y = short(y);
		}
		if (isGeoManager && ((VGeoManager *)this)->forceFrameChildren) {
			((VGeoManager *)this)->forceFrameChildren = 0;
			theGeometry->width = w;
			theGeometry->height = h;
			((VGeoManager *)this)->resized(w, h);
		}
		else if (theGeometry->width != w) {
			theGeometry->width = w;
			if (theGeometry->height != h) {
				theGeometry->height = h;
				if (isGeoManager) {
					((VGeoManager *)this)->resized(w, h);
				}
			}
			else if (isGeoManager) {
				((VGeoManager *)this)->resized(w, 0);
			}
		}
		else if (theGeometry->height != h) {
			theGeometry->height = h;
			if (isGeoManager) {
				((VGeoManager *)this)->resized(0, h);
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VGeoFrame::moveWindowChildren(int xoffset, int yoffset)
{
	if (!isGeoManager || (((VGeoManager *)this)->theChildren == 0)) {
		return;
	}

	VGeoManager::ChildIterator children((VGeoManager *)this);
	VGeoFrame *child;

	while ((child = children.next()) != 0) {
		if (child->usingTheWindow && (child->theWindow != 0)) {
			child->theWindow->setGeometryOffset(xoffset, yoffset);
		}
		else if (child->isGeoManager) {
			child->moveWindowChildren(xoffset, yoffset);
		}
	}
}

// ==========================================================================

#if !defined(CV_NOARCHIVER)

// --------------------------------------------------------------------------
//
void VGeoFrame::putTo(VArchiver& a)
{
	putTo(a, TRUE);
	putToDone(a);
}

// --------------------------------------------------------------------------
//
void VGeoFrame::putTo(VArchiver& a, boolean)
{
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Archive format:
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//
	// TYPE		  WHAT				NOTES
	// ----		  ----				-----
	// long:	  revision-tag	  | VGeoFrameArchiveTag == 0
	// char:	  window-flag	  | 0 == not associated with a VWindow
	//							  | 1 == associated with a VWindow
	// short:	  window-index	  * iff use-window-flag == 1 then
	//							  |     >=0  == index (0-based) of
	//							  |             associated VWindow within
	//							  |             its parent VWindow's
	//							  |             children list
	//							  |     -1   == associated with a VWindow
	//							  |             which has no parent VWindow
	//							  |     <=-2 == error getting index
	// Geometry:  geometry-flag	  * iff use-window-flag == 0 then
	//							  |     0 == no Geometry object is present
	//							  |     1 == Geometry object is present
	// Geometry:  geometry		  * iff geometry-flag == 1 then
	//							  |     Geometry object
	// char:	  dont-frame-flag | 1 == dont-frame is on
	//							  | 0 == dont-frame is off
	// long:	  manager-type	  | 0 == not a manager
	//							  | 0 != manager type code
	// char:	  child-data-flag | 1 == ChildData object is present
	//							  | 0 == ChildData object is not present
	// ChildData: child-data	  * iff child-data-flag == 1 then
	//							  |     ChildData object
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	// revision-tag

	a << VGeoFrameArchiveTag;

	// use-window-flag & (window-index | (geometry-flag & geometry))

	if (usingTheWindow) {
		a << char(1);
		a << short(getWindowIndex(theWindow));
	}
	else {
		a << char(0);
		if (theGeometry != 0) {
			a << char(1);
			theGeometry->putTo(a);
		}
		else {
			a << char(0);
		}
	}

	// dont-frame-flag

	if (dontFrame) {
		a << char(1);
	}
	else {
		a << char(0);
	}

	// manager-type

	long type;
	if (isGeoManager) {
		type = 0x00000001;
		if (isGeoForm)			{ type |= 0x00000002; }
		if (isGeoRowOrColumn)	{ type |= 0x00000004; }
		if (isGeoRow)			{ type |= 0x00000008; }
		if (isGeoColumn)		{ type |= 0x00000010; }
		if (isGeoRowColumn)		{ type |= 0x00000020; }
		if (isGeoPacker)		{ type |= 0x00000040; }
	}
	else {
		type = 0x00000000;
	}
	a << type;
}

// --------------------------------------------------------------------------
//
void VGeoFrame::putToDone(VArchiver&)
{
}

// --------------------------------------------------------------------------
//
void VGeoFrame::getFrom(VArchiver& a, VObject *object, boolean)
{
	getFrom(a);
	getFromDone(a, object);
}

// --------------------------------------------------------------------------
// See putTo() for the archive format.
//
void VGeoFrame::getFrom(VArchiver& a)
{
	char flag;

	// revision-tag

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VGeoFrameArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VGeoFrameArchiveTag));
		return;
	}

	// use-window-flag & (window-index | (geometry-flag & geometry))

	a >> flag;
	if (flag != char(0)) {
		usingTheWindowIndex = 1;
		short index;
		a >> index;
		theWindowIndex = index;
	}
	else {
		a >> flag;
		if (flag != char(0)) {
			theGeometry = new Geometry;
			theGeometry->getFrom(a);
		}
	}

	// dont-frame-flag

	a >> flag;
	if (flag != char(0)) {
		dontFrame = 1;
	}

	// manager-type

	long type;
	a >> type;
	if (type & 0x00000001) { isGeoManager	  = 1; }
	if (type & 0x00000002) { isGeoForm		  = 1; }
	if (type & 0x00000004) { isGeoRowOrColumn = 1; }
	if (type & 0x00000008) { isGeoRow		  = 1; }
	if (type & 0x00000010) { isGeoColumn	  = 1; }
	if (type & 0x00000020) { isGeoRowColumn	  = 1; }
	if (type & 0x00000040) { isGeoPacker	  = 1; }
}

// --------------------------------------------------------------------------
//
void VGeoFrame::getFromDone(VArchiver& a, VObject *object)
{
	if ((object != 0) && object->isA(VWindowCls)) {
		//
		// The given VObject is a VWindow;
		// associate it with this VGeoFrame.
		//
		VGeoFrame::associateWindow((VWindow *)object);
	}
}

// --------------------------------------------------------------------------
// Called at the end of VWindow::getFrom() to traverse the VGeo hierarchy and
// purge any VGeoFrames/Managers which have unresolved VWindow associations.
//
void VGeoFrame::getFromDone(VWindow *window)
{
	if ((window == 0) ||
		(window->geoFrame == 0) ||
		!window->geoFrame->isGeoManager ||
		(window->geoManager->theChildren == 0)) {
		return;
	}
	VGeoManager::ChildIterator children(window->geoManager, TRUE);
	VGeoFrame *child;
	children.reverse();
	while ((child = children.next()) != 0) {
		if (child->usingTheWindowIndex) {
			delete child;
		}
	}
}

// --------------------------------------------------------------------------
// Return the (zero-based) index of the given VWindow within its parent's
// child list.  If this VWindow has no parent return -1.  Return -2 on error.
//
int VGeoFrame::getWindowIndex(VWindow *window)
{
	if (window == 0) {
		return -2;
	}

	if (window->isTopLevel()) {
		return -1;
	}

	VWindow *parent_window = window->getParent();

	if (parent_window == 0) {
		return -1;
	}

	VOrdCollect *children = parent_window->childrenOf();

	if (children == 0) {
		return -2;
	}

	int index = int(children->indexOfSame(window));

	if (index < 0) {
		return -2;
	}

	return index;
}

// --------------------------------------------------------------------------
//
void VGeoFrame::Geometry::putTo(VArchiver& a)
{
	if (frame != 0) {
		a << char(1);
		a.putObject(frame);
	}
	else {
		a << char(0);
	}
}

// --------------------------------------------------------------------------
//
void VGeoFrame::Geometry::getFrom(VArchiver& a)
{
	char flag;
	a >> flag;
	if (flag != char(0)) {
		frame = (VFrame *)a.getObject(0);
	}
}

#endif

