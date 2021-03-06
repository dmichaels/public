// $Header:   Q:/views/win/vcs/radibttn.cpv   1.23   Mar 04 1997 17:55:42   gregm  $ 

//  radibttn.cpp
//
//	VRadioButton implementation
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1995-1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  05/13/93 pat    added constructor that takes name argument;
//                  added getFrom(), putTo(), realize() and
//                  included archiver.h;
//  05/14/93 pat    added destructor (virtual)
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  08/09/93 jmd    made VFrame const
//  08/09/93 jmd    removed free
//  08/16/93 jmd    made name const VString
//  09/22/93 pat    added archive tag
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  11/11/94 dss    Archiver tags macros.
//  06/08/95 jld    added givenFocus() and clearFocus() methods to override
//					VPushButton members
//  06/08/95 jld    added BTTNskipclick to ignore first clicked()
//					call after getting focus
//  09/01/95 dss    VStyle changes.
//  11/20/95 dgm    Fixed typo (qualified toggle).
//	01/24/95 evc    Overrode VButton::clicked()
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "radibttn.h"
#include "iterator.h"
#include "exclgrp.h"

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#endif

extern "C" {
#   include <assert.h>
#   include <string.h>
}

defineClass(VRadioButton, VCheckBox)
defineArchiveRevision(VRadioButton, 1)

// ---------------------------------------------------------------------------
//  Destroy this VRadioButton.
//
VRadioButton::~VRadioButton()
{
    ;
}

// ---------------------------------------------------------------------------
//
//  Construct a VRadioButton button with no presentation space.
//
VRadioButton::VRadioButton() {

	m_groupNo = 0;

}


// ---------------------------------------------------------------------------
//
//  Construct a VRadioButton from the resource control-id 'cntrl' of
//  the resource associated with 'pWin'. This is a non-portable GUI system method.
//
VRadioButton::VRadioButton(unsigned cntrl, VWindow *pWin) 
{
    m_groupNo = 0;
    getRscWindow(cntrl, pWin);
}

// ---------------------------------------------------------------------------
//
//  Construct a VRadioButton button of default size with label 'label'
//  at position 'x', 'y' in screen units within 'win'. The default style for
//  VRadioButton buttons is StyleAutoToggle.
//
VRadioButton::VRadioButton(VRatio x, VRatio y, char *label, VWindow *win, 
                           const VStyle& style)
{
    int w, h = 1;
    if (!label) {
        label = "<unknown>";
    }
    w = strlen(label);
    sizeOf(&w, &h);
    (*this)(VFrame(x, y, w, h), win, style);
    putText(label);
    m_groupNo = 0;
}

//---------------------------------------------------------------------------
// 
//  Construct a VRadioButton with frame 'frame' parent 'win', style 
//  'style'and optional label 'label'.  The default style for VRadioButton
//  buttons is StyleAutoToggle.
//
VRadioButton::VRadioButton(const VFrame& frame, VWindow *win, const VStyle& style, 
                           char *label) 
{
    (*this)(frame, win, style);
    if (label) {
        putText(label);
    }
    m_groupNo = 0;
}

// ---------------------------------------------------------------------------
//
VRadioButton::VRadioButton(const VString& wname, const VFrame& frame, 
                            VWindow *win, const VStyle& style, char *label) 
{
    (*this)(frame, win, style, wname);
    if (label) {
        putText(label);
    }
    m_groupNo = 0;
}

// ----------------------------------------------------------------------------
//
boolean VRadioButton::notify(int code)
{
	if (code == ACCELcode) {
		if (hWnd != 0) {
			if (!VWindow::checkLosingFocus()) {
				takeFocus();
				long s = GetWindowLong(hWnd, GWL_STYLE);
				if ((s & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON) {
					check(TRUE);
					return TRUE;
				}
			}
		}
	}
	return VCheckBox::notify(code);
}

// ----------------------------------------------------------------------------
//
void VRadioButton::check(boolean b)
{
	if (b) {
		//
		// Find other VRadioButtons with the same parent
		// and group number and turn them off.
		//
		VWindow *parent = getParent();
		if (parent != 0) {
			DO (parent->childrenOf(), VRadioButton, child)
				if ((child != this) && child->isA(VRadioButtonCls) &&
					(child->groupNo() == groupNo()) && child->checked()) {
					child->check(FALSE);
				}
			END
		}
	}

	VCheckBox::check(b);

}

// ----------------------------------------------------------------------------
//
boolean VRadioButton::clicked()
{
	if (checked()) {
		//
		// Find other VRadioButtons with the same
		// parent and group number and turn them off.
		//
		VWindow *parent = getParent();
		if (parent != 0) {
			DO (parent->childrenOf(), VRadioButton, child)
				if ((child != this) && child->isA(VRadioButtonCls) &&
					(child->groupNo() == groupNo()) && child->checked()) {
					child->check(FALSE);
				}
			END
		}
	}

	if ((client != 0) && (clickMthd != 0)) {
		client->perform(clickMthd, long(this));
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
//
boolean VRadioButton::toggle()
{
    if (!checked()) {
        check(TRUE);
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
//
//  Called by the VNotifier to obtain info on the underlying GUI system
//  window represented by this VRadioButton object.
//
void VRadioButton::getInfo(VWinInfo &wi)
{
	VButton::getInfo(wi);
	wi.style &= ~BS_OWNERDRAW;
	if (style.contains(StyleAutoToggle) || (style == getDefaultStyle())) {
		wi.style |= BS_AUTORADIOBUTTON;
	}
	else {
		wi.style |= BS_RADIOBUTTON; 
	}
	wi.style |= WS_GROUP;
}

// ---------------------------------------------------------------------------
//
//
boolean VRadioButton::givenFocus()
{
	state |= BTTNskipClick;
    return VButton::givenFocus();
}

// ---------------------------------------------------------------------------
//
//
boolean VRadioButton::clearFocus()
{
    return VButton::clearFocus();
}

// ===========================================================================
#ifndef CV_NOARCHIVER

//---------------------------------------------------------------------------
// 
void VRadioButton::putTo(VArchiver& a)
{
    VRadioButton::putTo(a, TRUE);
}

//---------------------------------------------------------------------------
// 
void VRadioButton::putTo(VArchiver& a, boolean most_derived)
{
    VCheckBox::putTo(a, FALSE);             //  base object data

    a << VRadioButtonArchiveTag;

    a << m_groupNo;

    if (most_derived) {
        VWindow::putChildrenTo(a);          //  children
    }
}

//---------------------------------------------------------------------------
// 
void VRadioButton::getFrom(VArchiver& a)
{
    VCheckBox::getFrom(a);                  //  base object data

    long tag;
    a >> tag;

    if (!archiveTagOk(tag, VRadioButtonArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VRadioButtonArchiveTag));
        return;
    }

    if (getArchiveTagRev(tag) > 0) {
       a >> m_groupNo;
    }
    
}

//---------------------------------------------------------------------------
// 
void VRadioButton::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VRadioButton::getFrom(a);

    if (do_realize) {
        VRadioButton::realize((VWindow *) parent);
    }

    if (parent != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

//---------------------------------------------------------------------------
//
void VRadioButton::realize(VWindow *parent)
{
    VCheckBox::realize(parent);
}

#endif  //  ARCHIVER

