// $Header:   Q:/views/win/vcs/help.cpv   1.8   Mar 04 1997 17:55:10   gregm  $

//	help.cpp
//
//	VHelp implementation
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
// --------------------------------------------------------------------------- 

#include "help.h"
#include "notifier.h"


defineClass(VHelp, VWindow)
defineCallback(VHelp, search)
defineCallback(VHelp, topicFromMouse)
defineCallback(VHelp, topicFromFocus)
defineCallback(VHelp, contents)


// --------------------------------------------------------------------------- 
VHelp::VHelp()
{
	;
}

// --------------------------------------------------------------------------- 
VHelp::VHelp( VWindow* pwin,
				const VString& fname,
				boolean askParent,
				boolean popupStyle )
	: VWindow( VFrame(0,0,0,0), pwin, StyleHidden )
{
	hide();
	disable();
	_file_name = fname;
	_had_focus = 0;
	_ask_parent = askParent;
	_pop_it_up = popupStyle;
}

// --------------------------------------------------------------------------- 
VHelp::~VHelp()
{
	WinHelp( parent->hwnd(),
			_file_name.gets(),
			HELP_QUIT,
			0 );
}

// --------------------------------------------------------------------------- 
void VHelp::destroyed()
{
	// First argument should be parent, but parent is destroyed before
	// this gets called. Can be corrected if/when children are destroyed
	// before the parents.
	WinHelp( hwnd(),
			_file_name.gets(),
			HELP_QUIT,
			0 );
}


// --------------------------------------------------------------------------- 
boolean VHelp::contents()
{
	WinHelp( parent->hwnd(),
			_file_name.gets(),
			HELP_CONTENTS,
			0 );

	return TRUE;
}

// --------------------------------------------------------------------------- 
boolean VHelp::index()
{
#ifdef CV_WIN32
	WinHelp( parent->hwnd(),
			_file_name.gets(),
			HELP_FINDER,
			0 );
#else
	WinHelp( parent->hwnd(),
			_file_name.gets(),
			HELP_CONTENTS,
			0 );
#endif

	return TRUE;
}

// --------------------------------------------------------------------------- 
boolean VHelp::topic(long topic_id)
{
	WinHelp( parent->hwnd(),
			_file_name.gets(),
			(_pop_it_up || topic_id & USECONTEXTPOPUP) ? HELP_CONTEXTPOPUP : HELP_CONTEXT,
			(topic_id & ~USECONTEXTPOPUP) );

	return TRUE;
}


// --------------------------------------------------------------------------- 
boolean VHelp::topic(long topic_id, boolean popup_style)
{
	WinHelp( parent->hwnd(),
			_file_name.gets(),
			(popup_style || topic_id & USECONTEXTPOPUP) ? HELP_CONTEXTPOPUP : HELP_CONTEXT,
			(topic_id & ~USECONTEXTPOPUP) );

	return TRUE;
}


// --------------------------------------------------------------------------- 
boolean VHelp::topicFromFocus()
{
	VWindow* curio = notifier->getFocus();

	if ( curio != 0 ) {
		long topic_id = curio->getHelpId(_ask_parent);
		WinHelp( parent->hwnd(),
				_file_name.gets(),
				(_pop_it_up || topic_id & USECONTEXTPOPUP) ? HELP_CONTEXTPOPUP : HELP_CONTEXT,
				(topic_id & ~USECONTEXTPOPUP) );
	}

	return TRUE;
}


// --------------------------------------------------------------------------- 
boolean VHelp::topicFromMouse()
{
	VWindow* curio = notifier->whereIsMouse();

	if ( curio != 0 ) {
		long topic_id = curio->getHelpId(_ask_parent);
		WinHelp( parent->hwnd(),
				_file_name.gets(),
				(_pop_it_up || topic_id & USECONTEXTPOPUP) ? HELP_CONTEXTPOPUP : HELP_CONTEXT,
				(topic_id & ~USECONTEXTPOPUP) );
	}

	return TRUE;
}


// --------------------------------------------------------------------------- 
boolean VHelp::search(const VString& key_word, boolean exact)
{
	if (exact) {
		WinHelp( parent->hwnd(),
				_file_name.gets(),
				HELP_KEY,
				(DWORD) key_word.gets() );
	}
	else {
		WinHelp( parent->hwnd(),
				_file_name.gets(),
				HELP_PARTIALKEY,
				(DWORD) key_word.gets() );
	}

	return TRUE;
}


// --------------------------------------------------------------------------- 
boolean VHelp::captureMouse()
{
	if ( _had_focus == 0 ) {
		_had_focus = notifier->getFocus();
	}

	// Enable to get the ESC key
	enable();
	notifier->accelKeysOff();

#ifdef CV_WINDOWS
    if (hWnd) {
        SetFocus(hWnd);
	}
	else {
#else
	if ( !notifier->setFocus(this) ) {
#endif
		notifier->accelKeysOn();
		disable();
		return FALSE;
	}

	notifier->captureMouseFor(this);
	notifier->setCursor( Help );

	return TRUE;
}


// --------------------------------------------------------------------------- 
boolean VHelp::mouseUp(int x,int y)
{
	mapToScreen( &x, &y );

	POINT pt;
	pt.x = x;
	pt.y = y;

	VWindow* curio = notifier->getThis(WindowFromPoint(pt));

	if ( _had_focus != 0) {
		// give back the keybooard focus
#ifdef CV_WINDOWS
	    if (_had_focus->hwnd()) {
	        SetFocus(_had_focus->hwnd());
		}
#else
		notifier->setFocus(_had_focus);
#endif
		_had_focus = 0;
	}
	disable();
	notifier->accelKeysOn();

	notifier->restoreCursor();
	notifier->releaseCapture();

	if ( curio != 0 ) {
		long topic_id = curio->getHelpId(_ask_parent);
		WinHelp( parent->hwnd(),
				_file_name.gets(),
				(_pop_it_up || topic_id & USECONTEXTPOPUP) ? HELP_CONTEXTPOPUP : HELP_CONTEXT,
				(topic_id & ~USECONTEXTPOPUP) );
	}

	return TRUE;
}


// --------------------------------------------------------------------------- 
boolean VHelp::key(int key, char ch)
{
	if (key == K_Escape) {
		// give back the keybooard focus
#ifdef CV_WINDOWS
	    if (_had_focus->hwnd()) {
	        SetFocus(_had_focus->hwnd());
		}
#else
		notifier->setFocus(_had_focus);
#endif
		_had_focus = 0;

		notifier->restoreCursor();
		notifier->releaseCapture();
	}

	return TRUE;
}


// --------------------------------------------------------------------------- 
boolean VHelp::close()
{
	WinHelp( parent->hwnd(),
			_file_name.gets(),
			HELP_QUIT,
			0 );

	return TRUE;
}

