// $Header:   Q:/views/win/vcs/mdiclnt.h_v   1.5   Mar 04 1997 16:32:36   gregm  $ 

//	mdiclnt.h
//
//	VMdiClient interface
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
//	06/25/93 jmd	preened
// ----------------------------------------------------------------------------

#ifndef VMDICLNT_H
#define VMDICLNT_H
#include "window.h"

CLASS VMdiClient;
CLASS VMdiView;
CLASS VPopupMenu;
CLASS VNotifier;
CLASS VMenuItem;

extern VClass *VMdiClientCls;

//
// `VMdiClient` is a MS Windows specific class implementing the
// MDI Client window which controls and manages all of the MDI child windows
// in an application. It is not to be directly constructed or used by the user
// because it is managed directly by the `VMdiAppView` class.
// 

CLASS VMdiClient : public VWindow {
public:
				VMdiClient();
				~VMdiClient();

	VClass	   *iam();

	void		setMdiWinMenu(VPopupMenu *m);
	VPopupMenu *getMdiWinMenu();

	VMdiView   *activeChild();
	boolean		cascade(VMenuItem *m);
	boolean		arrange(VMenuItem *m);
	boolean		tile(VMenuItem *m);

	void			closeAll();
	boolean		quit();
	enum WinType winType();

protected:
	VPopupMenu *popup; 			// The MDI popup-menu.
	void		getInfo(VWinInfo& info);
	friend		VNotifier;
};

// ---------------------------------------------------------------------------
// Return the standard MDI 'VWindow' popup menu.
//
inline VPopupMenu *VMdiClient::getMdiWinMenu()
{
	return popup;
}

#endif // VMDICLNT_H
