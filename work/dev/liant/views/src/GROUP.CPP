// $Header:   Q:/views/win/vcs/group.cpv   1.21   Mar 04 1997 17:55:08   gregm  $ 

//  group.cpp
//
//	VGroup implementation
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
//  Revision History:
//  -----------------
//  05/14/93 pat    added destructor (virtual)
//  05/25/93 jmd    preened, removed free
//  06/21/93 pat    added support for VArchiver; preened;
//                  added constructor with name argument.
//  07/15/93 pat    changed putTo() to use indices into archiveChildren
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  08/08/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  09/20/93 dgm    fix for bad background repainting of when we have focus.
//  09/22/93 pat    added archive tag
//  10/05/93 dgm    fix in getFrom().
//  10/12/93 pat    initialized currentControlIndex and controlChildIndices
//                  realization data members.
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  03/05/94 pat    STRICT:  
//                  changed HANDLE to HBRUSH in erased();
//                  cast to HGDIOBJ for DeleteObject().
//  03/04/94 jmd    added test for delete[] on 0
//  08/12/94 dss    Ctl3d support.
//  09/05/94 dss    getInfo(): exStyle initialization.
//  11/11/94 dss    Archiver tags macros.
//  09/01/95 dss    VStyle changes.
//	01/29/96 dgm	Overrode getInternalBorderGeometry().
//	02/17/96 dgm	Overrode getPreferredSize().
//	04/19/96 glm	removed v1.1 code/preened
//	08/28/96 dgm	Minor fix in givenFocus() and key().
//	12/23/96 pkt	updated to compile on Web platform.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "group.h"
#include "control.h"
#include "brush.h"
#include "iterator.h"
#include "font.h"

#ifdef CV_CTL3D
#include "ctl3d.h"
#endif

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

extern "C" {
#   include <string.h>
}

defineClass(VGroup, VControl)
defineArchiveRevision(VGroup, 0)

// ---------------------------------------------------------------------------
//  Destroy this VGroup.
//
VGroup::~VGroup()
{
    ;
}

// ---------------------------------------------------------------------------
// Return MS Windows information
//
void VGroup::getInfo(VWinInfo& grpinfo)
{
#ifndef CV_REMOTE_GUI
    grpinfo.wClass = "button";
    grpinfo.style = WS_CHILD | BS_GROUPBOX | WS_CLIPCHILDREN;
    grpinfo.exStyle = 0;

    if (!(style.contains(StyleHidden))) {
        grpinfo.style |= WS_VISIBLE;
    }
#else
	// Web
	VControl::getInfo(grpinfo);
	grpinfo.isChild = TRUE;
    grpinfo.isVisible = !style.contains(StyleHidden);
#endif
}

// ---------------------------------------------------------------------------
void VGroup::getMinSize(short& w, short& h)
{
    int wf, hf;

    fontSize(&wf, &hf);
    h = 7 * wf / 4;
    w = 2 * h;
}

// ---------------------------------------------------------------------------
// Construct a VGroup with no presentation space.
//
VGroup::VGroup()
#ifndef CV_NOARCHIVER
        :   currentControlIndex(-1L),
            controlChildIndices(0)
#endif
{
    current = 0;
}

// ---------------------------------------------------------------------------
// Construct a VGroup from the resource control-id 'cntrl' of the
// resource associated with 'pWin'. This is a non-portable GUI system method.
//
VGroup::VGroup(unsigned cntrl, VWindow *pWin) 
        :   current(0)
#ifndef CV_NOARCHIVER
        ,   currentControlIndex(-1L),
            controlChildIndices(0)
#endif
{ 
#ifndef CV_REMOTE_GUI
    getRscWindow(cntrl, pWin);
#endif
}

// ---------------------------------------------------------------------------
// Construct a VGroup with frame 'frame', parent 'win' and title 'title'.
//
VGroup::VGroup(const VFrame& frame, VWindow *pwin, char *title) 
        :   current(0)
#ifndef CV_NOARCHIVER
        ,   currentControlIndex(-1L),
            controlChildIndices(0)
#endif
{
    (*this)(frame, pwin);
    if (title) {
        putText(title);
    }
}

// ---------------------------------------------------------------------------
// Construct a named VGroup with frame 'frame', parent 'win' and title 'title'.
//
VGroup::VGroup(const VString& wname, const VFrame& frame, VWindow *pwin, 
                char *title) 
        :   current(0)
#ifndef CV_NOARCHIVER
        ,   currentControlIndex(-1L),
            controlChildIndices(0)
#endif
{
    (*this)(frame, pwin, StyleDefault, wname);
    if (title) {
        putText(title);
    }
}

// ---------------------------------------------------------------------------
// Add VControl 'cntrl' to this VGroup.
//
void VGroup::addControl(VControl *cntrl)
{
    controls.add(cntrl);
}

// ---------------------------------------------------------------------------
//
VWindow *VGroup::getFocusCandidate(int mode) const
{
	return VControl::getFocusCandidate(mode);
}


// ---------------------------------------------------------------------------
//
void VGroup::setChildFocus(VWindow *child)
{
	VControl::setChildFocus(child);
	if (controls.findSame(child)) {
		current = (VControl *)child;
	}
	else {
		current = 0;
	}
}

// ---------------------------------------------------------------------------
// Find and return a pointer to the next child control eligible for focus
// after the given control.  If the given control is null then use the
// current control, if that's also null then start with first control.
// Wraparound to the end if necessary; null is returned only if there
// are no children or none of them are eligible to receive focus.
//
VWindow *VGroup::getNextChild(VWindow *current_child) const
{
	int count = int(controls.count()), index;
	VOrdCollect *children;
	
	if (count <= 0) {
		if ((children = ((VWindow *)this)->childrenOf()) == 0) {
			return 0;
		}
		count = children->count();
		if (count <= 0) {
			return 0;
		}
	}
	else {
		children = 0;
	}

	if ((current_child != 0) ||
		((current_child = children != 0 ? ((VWindow *)this)->
										   getChildFocus() : current) != 0)) {
		if (current_child->getParent() != (VWindow *)this) {
			index = 0;
		}
		else {
			if (children != 0) {
				index = children->indexOfSame(current_child);
			}
			else {
				index = controls.indexOfSame(current_child);
			}
			if ((index < 0) || (index >= count)) {
				index = 0;
			}
		}
	}
	else {
		index = 0;
	}

	for (int i = index + 1 ; 1 ; i++) {
		if (i >= count) {
			i = 0;
		}
		if (i == index) {
			break;
		}
		VWindow *child = children != 0 ? (VWindow *)children->idAt(i) :
										 (VWindow *)controls.idAt(i);
		if ((child != 0) &&
			!child->isTopLevel() && child->getFocusCandidate()) {
			return child;
		}
	}

	return 0;
}

// ---------------------------------------------------------------------------
// Find and return a pointer to the previous child control eligible for focus
// before the given control.  If the given control is null then use the
// current control, if that's also null then start with first control.
// Wraparound to the end if necessary; null is returned only if there
// are no children or none of them are eligible to receive focus.
//
VWindow *VGroup::getPrevChild(VWindow *current_child) const
{
	int count = int(controls.count()), index;
	VOrdCollect *children;
	
	if (count <= 0) {
		if ((children = ((VWindow *)this)->childrenOf()) == 0) {
			return 0;
		}
		count = children->count();
		if (count <= 0) {
			return 0;
		}
	}
	else {
		children = 0;
	}

	if ((current_child != 0) ||
		((current_child = children != 0 ? ((VWindow *)this)->
										   getChildFocus() : current) != 0)) {
		if (current_child->getParent() != (VWindow *)this) {
			index = count - 1;
		}
		else {
			if (children != 0) {
				index = children->indexOfSame(current_child);
			}
			else {
				index = controls.indexOfSame(current_child);
			}
			if ((index < 0) || (index >= count)) {
				index = count - 1;
			}
		}
	}
	else {
		index = count - 1;
	}

	for (int i = index - 1 ; 1 ; i--) {
		if (i < 0) {
			i = count - 1;
		}
		if (i == index) {
			break;
		}
		VWindow *child = children != 0 ? (VWindow *)children->idAt(i) :
										 (VWindow *)controls.idAt(i);
		if ((child != 0) &&
			!child->isTopLevel() && child->getFocusCandidate()) {
			return child;
		}
	}

	return 0;
}

// ---------------------------------------------------------------------------
//
VWindow *VGroup::getFirstChild() const
{
	int count = int(controls.count());
	if (count <= 0) {
		VOrdCollect *children = ((VWindow *)this)->childrenOf();
		if ((children == 0) || ((count = children->count()) <= 0)) {
			return 0;
		}
		return getNextChild((VWindow *)children->idAt(count - 1));
	}
	else {
		return getNextChild((VWindow *)controls.idAt(controls.count() - 1));
	}
}

// ---------------------------------------------------------------------------
//
VWindow *VGroup::getLastChild() const
{
	int count = int(controls.count());
	if (count <= 0) {
		VOrdCollect *children = ((VWindow *)this)->childrenOf();
		if (children == 0) {
			return 0;
		}
		return getPrevChild((VWindow *)children->idAt(0));
	}
	else {
		return getPrevChild((VWindow *)controls.idAt(0));
	}
}

// ---------------------------------------------------------------------------
//
int VGroup::getNextChildIndex(VWindow *current_child) const
{
	if (controls.count() <= 0) {
		if (((VWindow *)this)->childrenOf() != 0) {
			return ((VWindow *)this)->childrenOf()->
									  indexOfSame(getNextChild(current_child));
		}
		else {
			return -1;
		}
	}
	else {
		return controls.indexOfSame(getNextChild(current_child));
	}
}

// ---------------------------------------------------------------------------
//
int VGroup::getPrevChildIndex(VWindow *current_child) const
{
	if (controls.count() <= 0) {
		if (((VWindow *)this)->childrenOf() != 0) {
			return ((VWindow *)this)->childrenOf()->
									  indexOfSame(getPrevChild(current_child));
		}
		else {
			return -1;
		}
	}
	else {
		return controls.indexOfSame(getPrevChild(current_child));
	}
}

// ---------------------------------------------------------------------------
//
int VGroup::getFirstChildIndex() const
{
	if (controls.count() <= 0) {
		if (((VWindow *)this)->childrenOf() != 0) {
			return ((VWindow *)this)->childrenOf()->
									  indexOfSame(getFirstChild());
		}
		else {
			return -1;
		}
	}
	else {
		return controls.indexOfSame(getFirstChild());
	}
}

// ---------------------------------------------------------------------------
//
int VGroup::getLastChildIndex() const
{
	if (controls.count() <= 0) {
		if (((VWindow *)this)->childrenOf() != 0) {
			return ((VWindow *)this)->childrenOf()->
									  indexOfSame(getLastChild());
		}
		else {
			return -1;
		}
	}
	else {
		return controls.indexOfSame(getLastChild());
	}
}

// ---------------------------------------------------------------------------
// The implementation of this method implements focus transfer
// when the HOME, END, UP, DOWN, LEFT, and RIGHT keys are pressed
// (see VWindow::key()).
//
boolean VGroup::key(int virtual_key, char ascii_key)
{
	int index;

	switch (virtual_key) {
	case K_Home:
		index = getFirstChildIndex();
		if ((index >= 0) && arrowKey(index)) {
			return TRUE;
		}
		break;
	case K_End:
		index = getLastChildIndex();
		if ((index >= 0) && arrowKey(index)) {
			return TRUE;
		}
		break;
	case K_Up:
	case K_Left:
		index = getPrevChildIndex();
		if ((index >= 0) && arrowKey(index)) {
			return TRUE;
		}
		break;
	case K_Down:
	case K_Right:
		index = getNextChildIndex();
		if ((index >= 0) && arrowKey(index)) {
			return TRUE;
		}
		break;
#if 0
	//
	// For TAB-ing, if we have controls in our control-list then assume
	// that ARROW-ing is the preferred way of navigating the children of
	// the group and the group will act like a single control as far as
	// TAB-ing goes.
	//
    case K_Tab:
    case K_Tab | K_Mod_Control:
		if (controls.count() > 0) {
			return nextFocus();
		}
		break;
	case K_Tab | K_Mod_Shift:
	case K_Tab | K_Mod_Shift | K_Mod_Control:
		if (controls.count() > 0) {
			return prevFocus();
		}
		break;
#endif
	}
	return VControl::key(virtual_key, ascii_key);
}

// ---------------------------------------------------------------------------
// Called in response to an arrow key being pressed with the 'index'
// of the next control in this VGroup to become current.  The default 
// action for a VGroup object is to do nothing, but subclasses may implement
// this method to pass focus to the control at index 'index'.
//
boolean VGroup::arrowKey(int index)
{
	setCurrent(index);
	return TRUE;
}

// ---------------------------------------------------------------------------
// Return the index of the VControl with current focus in this
// VGroup.  The first VControl in the group has index zero.
//
int VGroup::getCurrent()
{
    return int(controls.indexOfSame(current));
}

// ---------------------------------------------------------------------------
// Set the current control and keyboard focus to 'i'th control
// within this VGroup.
// 
void VGroup::setCurrent(int index)
{
	if (index < 0) {
		index = 0;
	}

	int count = int(controls.count());
	VOrdCollect *children;

	if (count <= 0) {
		if ((children = childrenOf()) == 0) {
			return;
		}
		count = children->count();
	}
	else {
		children = 0;
	}

	if (index >= count) {
		index = count - 1;
	}

	VControl *control;
	
	if (children != 0) {
		control = (VControl *)children->idAt(index);
	}
	else {
		control = (VControl *)controls.idAt(index);
	}

	if (control != 0) {
		control->notify(ACCELcode);
		current = control;
	}
}

// ---------------------------------------------------------------------------
// Return the type of this window as a WinControl.
//
enum WinType VGroup::winType()
{
    return WinControl;
}

// ---------------------------------------------------------------------------
// Return the ith 'VControl' object in this group.
//
VControl *VGroup::getControl(int i)
{
    return (VControl *)controls.idAt(i);
}

// ----------------------------------------------------------------------------
//
void VGroup::getPreferredSize(int *w, int *h) const
{
	VControl::getPreferredSize(w, h);
	if (w != 0) { *w += 12; }
}

// ----------------------------------------------------------------------------
//
void VGroup::getInternalBorderGeometry(int *left, int *right,
									   int *top, int *bottom) const
{
	if (left   != 0) { *left   = 3; }
	if (right  != 0) { *right  = 3; }
	if (bottom != 0) { *bottom = 3; }

	if (top != 0) {
		VFont *font = getFont();
	    if (font == 0) {
			font = &VFont::system();
		}
		*top = font->height();
	}
}

#ifndef CV_REMOTE_GUI
// --------------------------------------------------------------------------
// 
UINT VGroup::getCtlColor()
{
#if defined(CV_WIN32)
    return WM_CTLCOLORSTATIC;
#else
    return CTLCOLOR_STATIC;
#endif
}

// ---------------------------------------------------------------------------
//
boolean VGroup::has3dEffects() const
{
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VGroup::has3dEffects(V_3D_EFFECTS v) const
{
	return VControl::has3dEffects(v);
}
#endif

// ===========================================================================
#ifndef CV_NOARCHIVER   

// --------------------------------------------------------------------------
//  This method is invoked by getFrom() in some non-abstract, 
//  most-derived class after the children have been retrieve from an archive.
//  Its purpose is to add controls to the VGroup's ordered collection of
//  controls.  Controls are added from the indices of the controls in the 
//  VWindow child collection which were previously retrieved from the 
//  the archive by VGroup::getFrom().
//
void VGroup::addControlsFromChildIndices()
{
    if ((controlChildIndices != 0) && (childrenOf() != 0)) {
        integer n = controlChildIndices[0];
        for (integer i = 0; i < n; i++) {
            controls.add((VControl *)
                         childrenOf()->idAt(controlChildIndices[i + 1]));
        }
    }
    if (controlChildIndices != 0) {
        delete [] controlChildIndices;
        controlChildIndices = 0;
    }

    if (currentControlIndex > 0) {
        setCurrent(currentControlIndex);
    }
}

// --------------------------------------------------------------------------
//
void VGroup::putTo(VArchiver& a)
{
    VGroup::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//
void VGroup::putTo(VArchiver& a, boolean most_derived)
{
    VControl::putTo(a, FALSE);              //  base object data

    a << VGroupArchiveTag;

    //  indices of control collection items in child collecton
    
    integer n = controls.count();
    if (n <= 0) {
        n = 0;
        a << n;             // number of controls (zero);
        a << 0;             // index of current control in control collection
    }
    else {
        a << n;             // number of controls
        DO(controls, VControl, cntrl)
            a << archiveChildrenOf()->indexOfSame(cntrl);
        END     
        a << getCurrent();  // index of current control in control collection
    }

    if (most_derived) {
        //  (this is an abstract class, but what the heck)
        VWindow::putChildrenTo(a);
    }
}

// --------------------------------------------------------------------------
//
void VGroup::getFrom(VArchiver& a)
{
    VControl::getFrom(a);                   //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VGroupArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VGroupArchiveTag));
        return;
    }

    //  indices of control collection items in child collecton
    
    integer n;
    a >> n;
    
    if (n > 0) {
        controlChildIndices = new integer [n + 1];

        if (controlChildIndices != 0) {
            *controlChildIndices = n;
            for (integer i = 0; i < n; i++) {
                a >> controlChildIndices[i + 1];
            }
        }
    }
    else {
        controlChildIndices = 0;
    }

    //  index of current control in control collection

    a >> currentControlIndex;             
}

// --------------------------------------------------------------------------
//
void VGroup::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VGroup::getFrom(a);             

    if (do_realize) {
        VGroup::realize((VWindow *)data);
    }

    if (data != 0) {
        //  (this is an abstract class, but what the heck)
        VWindow::getChildrenFrom(a, do_realize);
    }
}

// --------------------------------------------------------------------------
//
void VGroup::realize(VWindow *pwin)
{
    VControl::realize(pwin);
}

#endif //   ARCHIVER    

