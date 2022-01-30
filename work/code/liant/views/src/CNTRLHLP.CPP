// $Header:   Q:/views/common/vcs/cntrlhlp.cpv   1.12   Mar 24 1997 16:14:36   scottt  $

//  cntrlhlp.cpp
//
//  VControlHelp implementation [Common]
//
//  Allegris Foundation 1.1.00
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  01/12/96 tlf    Added call to show for MOTIF in showMsg.
//  01/29/96 dgm    Use VColorScheme.
//  02/02/96 tlf    Conditionally ifdef'd out experimental code
//                  in showMsg.
//  03/21/96 dgm    Minor change.
//  05/16/96 dgm    Minor changes.
//  06/04/96 dgm    Minor changes.
//  08/23/96 tlf    Minor fix in showMsg to prevent help from being drawn
//                  off the bottom of the screen.
//  09/13/96 dgm    Fixed memory leak (BoundsChecker).
//	09/16/96 pkt	Updated for WebShop.
//	09/18/96 dgm	Use VFont::system().
//	02/24/97 tlf	Updated version number to 1.00.02 and copyright date to 1997.
//	02/14/97 djs	#ifdef'd VDummy destruction of default VControlHelp for
//                OS/2.  Not sure why this doesn't crash under windows...
//                Moved to VNotifier's destructor.
// --------------------------------------------------------------------------

#include "cntrlhlp.h"
#include "intassoc.h"
#include "notifier.h"
#include "port.h"
#include "font.h"
#include "clrscm.h"
#include "timer.h"
#include "notifier.h"
#include "iterator.h"

// --------------------------------------------------------------------------
//
defineClass(VControlHelp, VPopupWindow)

// --------------------------------------------------------------------------
//
static VControlHelp	   *defaultControlHelp	= 0;

// --------------------------------------------------------------------------
//
VControlHelp::VControlHelp()
{
}

VControlHelp::VControlHelp( VWindow* pwin, VDictionary* helpDict ) :
#if 0
    VPopupWindow( VFrame(0,0,0,0), pwin, StyleBorder + StyleHidden )
#else
    VPopupWindow( VFrame(0,0,0,0), pwin, StyleHidden )
#endif
{
    disable();
    init(helpDict);
    setActive(TRUE);
    setShowPause(500);
    setHidePause(200);

    setBackground(&VColorScheme::
                  systemBrush(VColorScheme::ToolTipBackground));
}

VControlHelp::~VControlHelp()
{
    delete _hideTimer;
    delete _showTimer;
    delete _port;

	if (_tipList != 0) {
		DO (*_tipList, VAssoc, assoc)
			if (assoc->value != 0) {
				delete assoc->value;
			}
		END
		if (_tipListIsMine) {
			delete _tipList;
		}
	}
}

// ---------------------------------------------------------------------------
//
void VControlHelp::init(VDictionary* helpDict)
{
    _client = 0;
    _tip = 0;
    _tipId = 0;
	if (helpDict == 0) {
		_tipListIsMine = 1;
    	_tipList = new VDictionary(VIntAssocCls);
	}
	else {
		_tipListIsMine = 0;
    	_tipList = helpDict;
	}
    _port = new VPort( this );
    _showTimer = new VTimer( this, methodOf(VControlHelp, showNow) );
    _hideTimer = new VTimer( this, methodOf(VControlHelp, hideNow) );
	_port->useFont(&VFont::system(VFont::Info));
}

// ---------------------------------------------------------------------------
//
VControlHelp* VControlHelp::getDefault()
{
    if( defaultControlHelp == 0 ) {
        defaultControlHelp = new VControlHelp( 0, 0 );
    }
    return defaultControlHelp; 
}


// ---------------------------------------------------------------------------
//
VFont* VControlHelp::useFont(VFont* font)
{
    VFont *oldfont = _port->currentFont();
    return _port->useFont(font) ? oldfont : (VFont *)0;
}

// ---------------------------------------------------------------------------
//
VPen* VControlHelp::usePen(VPen* pen)
{
    VPen* oldPen = _port->currentPen();
    return _port->usePen( pen ) ? oldPen : (VPen*)0;
}

// ---------------------------------------------------------------------------
//
long VControlHelp::storeMsg(VString* msg, long id)
{
    if ( msg == 0 ) {
        return id;
    }
    
    if ( id >= 0 && !_tipList->includesKey( (VObject*)id ) ) {
        _tipList->atKeyPut( (VObject*)id, new VString( msg->gets() ) );
        return id;
    }
    // If id already used or not specified, fins
    // an unused id and return it.
    for ( id = 0;; id++ )
        if ( !_tipList->includesKey( (VObject*)id ) ) {
            _tipList->atKeyPut( (VObject*)id, new VString( msg->gets() ) );
            return id;
        }
}

// ---------------------------------------------------------------------------
//
void VControlHelp::tipPlease(VControl* clnt, long id)
{
    if( !_active ) {
        return;
    }
    // get help string or quit
    VObject* temp = _tipList->getValueAtKey( (VObject*)id );
    if( temp == 0 ) {
        return;
    }

    // is a tip showing?
    // _tip != 0 is not a good test, use !isHidden()
    if( _hideTimer->timing() ) {
        _hideTimer->stop();
        if( _client == clnt ) {
            return;
        }
        else {
            _client = clnt;
            _tipId = id;
            _tip = (VString *) temp;
            showMsg();
        }
    }
    else {
        _client = clnt;
        _tipId = id;
        _tip = (VString *) temp;
        _showTimer->oneshot(_showPause);
    }
}

// ---------------------------------------------------------------------------
//
void VControlHelp::tipThankYou(VControl* clnt, long id)
{
    if( !_active ) {
        return;
    }
    if (_showTimer->timing()) {
        _showTimer->stop();
        _tip = 0;
    }
    else {
        _hideTimer->oneshot(_hidePause);
    }
}

// ---------------------------------------------------------------------------
//
boolean VControlHelp::showNow()
{
    if( notifier->buttonStatus() ) {
        _showTimer->oneshot(_showPause);
        return TRUE;
    }

    if( _client != notifier->whereIsMouse() || !showMsg() ) {
        _client = 0;
        _tip = 0;
        _tipId = 0;
    }
    return TRUE;
}


// ---------------------------------------------------------------------------
//
boolean VControlHelp::showMsg()
{
    extern VClass *VUtilityWindowCls;

    if (parent != 0) {
        VWindow* innerFocus = notifier->getFocus();
        if (innerFocus == 0) {
            return TRUE;
        }
        VWindow* topLevelW = innerFocus->getTopLevelParent();
        if (topLevelW == 0) {
            return TRUE;
        }
        VWindow* topLevelP = parent->getTopLevelParent();
        if (topLevelP == 0) {
            return TRUE;
        }
        if ((topLevelW != topLevelP) &&
            (topLevelW != parent) && (innerFocus->isA(VUtilityWindowCls))) {
            return TRUE;
        }
    }

        int x, y, tw, th;
#if 1
        notifier->getCursorPos(&x, &y);
#else
    //  This looks better on the screen.
    //  The tip window is positioned at the bottom
    //  middle of the control, and not relative to the
    //  cursor position.  
        int vw, vh;
    VFrame vf(0, 0, 0, 0), *vfp;
    VWindow *vwp;

    vfp = &vf;
    vwp = (VWindow*)_client;
    vwp->getClientAbs(&x, &y, &vw, &vh);
#endif

        _port->open();
        if (!_tip)
          return FALSE;
        _port->textSize( _tip->gets(), &tw, &th );
        _port->close();

#if 1
        y += 24, tw += 8, th += 6;
        int sh, sw;
        notifier->getScreenSize(&sw, &sh);
        if (x + tw > sw) {
          x = sw - tw - 2;
        }
        if (y + th > sh) {
          y = sh - th - 2;
        }

        setGeometry(x, y, tw, th);
#else
    // Position the tip window at the bottom of
    // the control to which it applies, (y + vh + 4),
    // and at the middle. (x + vw/2).
        move(x + vw/2, y + vh + 4, tw + 8, th + 4);
#endif

//      enable();
// VWSPORT
#ifdef CV_WINDOWS
# ifdef CV_REMOTE_GUI
		show(TRUE);
# else
        ShowWindow(hwnd(), SW_SHOWNA);
# endif
#endif

#ifdef PM
//        boolean rc = WinShowWindow(hwnd(), TRUE);
       show(TRUE);
#endif
#ifdef MOTIF
       show(TRUE);
#endif
       update();
        return TRUE;
}


// ---------------------------------------------------------------------------
//
boolean VControlHelp::hideNow()
{
    _tip = 0;
    hide();
    disable();
    _client = 0;
    _tipId = 0;
    return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VControlHelp::paint()
{
    _port->open();

#if 0

    _port->wrtText(_tip, 1, 0);

#else

    static VRectangle CV_NEARDATA rectangle;
    int w, h;
    sizeOfImage(&w, &h);

    _port->usePen(&VColorScheme::systemPen(VColorScheme::ToolTipShadow));
    _port->moveTo(0, h - 1);
    _port->lineTo(w, h - 1);
    _port->moveTo(w - 1, 0);
    _port->lineTo(w - 1, h - 1);

    _port->usePen(&VColorScheme::systemPen(VColorScheme::ToolTipHighlight));
    _port->moveTo(0, 0);
    _port->lineTo(w - 1, 0);
    _port->moveTo(0, 1);
    _port->lineTo(0, h - 1);

    if (_tip != 0) {
        const char *s = _tip->gets();
        if (s != 0) {
            _port->usePen(&VColorScheme::
                           systemPen(VColorScheme::ToolTipForeground));
            rectangle.set(CornerDim, 0, 0, w, h);
            _port->wrtText(s, &rectangle, JustifyCenter |
                                          JustifyVcenter | SingleLine);
        }
    }

#endif

    _port->close();

    return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VControlHelp::mouseEnter(int b)
{
	if (!isEnabled()) {
		return FALSE;
	}
    if( _tip != 0 ) {
        _hideTimer->stop();
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VControlHelp::mouseExit(int b)
{
	if (!isEnabled()) {
		return FALSE;
	}
    if( _tip != 0 ) {
        _hideTimer->oneshot(_hidePause);
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
//
void VControlHelp::deleteDefault()
{
	if (defaultControlHelp != 0) {
		delete defaultControlHelp;
		defaultControlHelp = 0;
	}
}

#ifndef PM
// djs 02/14/97 - This is bad in general due to the special handling
// of global objects - especially those that will eventually reference
// the notifier.  For OS/2, the default deletion has been moved to the
// VNotifier's destructor - otherwise, the system crashes when the wrong
// VNotifier instance is returned - only happens with dynamically
// linked Foundation.  Not sure why this works under Windows...
// ---------------------------------------------------------------------------
//
struct VDummy { ~VDummy() { VControlHelp::deleteDefault(); } };
static VDummy CV_NEARDATA dummy;
#endif
