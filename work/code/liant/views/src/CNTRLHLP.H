// $Header:   Q:/views/common/vcs/cntrlhlp.h_v   1.5   Mar 24 1997 16:14:36   scottt  $

//	cntrlhlp.h
//
//	VControlHelp interface [Common]
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
//	12/08/95 dgm	Added this header.  Removed #include "timer.h".
// 09/13/96 dgm	Fixed memory leak (BoundsChecker).
// 02/07/97 djs	Added ::isTopLevel() for PM	
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef cntrlhlp_h
#define cntrlhlp_h

#include "poppwndw.h"
#include "dictinry.h"
#include "font.h"
#include "pen.h"
#include "str.h"


CLASS VTimer;

extern VClass *VControlHelpCls;



CLASS VControlHelp : public VPopupWindow {

	friend VControl;

public:
	VControlHelp();
	VControlHelp(VWindow* pwin, VDictionary* help = 0);
	~VControlHelp();
	VClass *iam();

#ifdef PM
	// Don't want VControlHelps taking the focus
	boolean 	isTopLevel() const { return FALSE; }
#endif

	static VControlHelp*	getDefault();

	VFont*			useFont(VFont* fnt);
	VPen*			usePen(VPen* pen);
	boolean			paint();
	boolean			mouseEnter(int b);
	boolean			mouseExit(int b);
	void			setHidePause(int p);
	void			setShowPause(int p);
	void			setActive(boolean b);
protected:
	void			tipPlease(VControl* client, long id);
	void			tipThankYou(VControl* client, long id);
	long			storeMsg(VString* msg, long id);
	void			init(VDictionary* d);
	boolean			showMsg();

	boolean			showNow();
	boolean			hideNow();

public:
	static void		deleteDefault();

private:
	VTimer*			_showTimer;
	int				_showPause;
	VTimer*			_hideTimer;
	int				_hidePause;
	VControl*		_client;
	VDictionary*	_tipList;
	VString*		_tip;
	VPort*			_port;
	long			_tipId;
	boolean			_active : 1;
	boolean			_tipListIsMine : 1;
};

// ---------------------------------------------------------------------------
//
inline void VControlHelp::setHidePause(int pause)
{
	_hidePause = pause;
}

// ---------------------------------------------------------------------------
//
inline void VControlHelp::setShowPause(int pause)
{
	_showPause = pause;
}

// ---------------------------------------------------------------------------
//
inline void VControlHelp::setActive(boolean b)
{
	_active = b;

	// remove tip when becoming inactive 
	if( !_active ) {
		hideNow();
	}	
}

#endif // cntrlhlp_h
