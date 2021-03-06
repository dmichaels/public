// $Header:   Q:/views/win/vcs/mdiview.h_v   1.9   Mar 24 1997 13:56:44   gregm  $ 

//	mdiview.h
//
//	VMdiView interface
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
//	06/24/93 jmd	preened
//	08/08/93 jmd	made VFrame const
//	08/16/93 jmd	renamed name to iconname, made it const
//	08/09/94 dgm	Added archiving support.
//	09/01/95 dss	VStyle changes.
//	02/04/97 pkt	Implemented 'getClassDefaultStyle'.
// ----------------------------------------------------------------------------

#ifndef VMDIVIEW_H
#define VMDIVIEW_H

#include "view.h"

CLASS VPopupMenu;
CLASS VMdiClient;
CLASS VNotifier;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

//
// VMdiView is a class that implements an MDI child window.
// Classes derived from it must always be 
// constructed as children of an VMdiAppView derived class.
// The styles defined for this window are StyleVirtical - add a
// vertical scroll-bar, StyleHorizontal - add a horizontal scroll-bar, or
// StyleDefault - default Mdi Child window with system menu, maximize-
// minimize controls and sizable border frame.  The resource name of the 
// VIcon to be used with this class is the name used as the first argument
// to the VMdiView constructor.  When VMdiView objects become active, they
// receive an activate event from the VNotifier through their activate()
// event response member function.  Similarily, when VMdiView objects become
// inactive, they receive a deactivate event from the VNotifier through their
// deactivate() event response member function. 
//

CLASS VMdiView : public VView {
public:
				VMdiView();
				VMdiView(const VStyle& style, const char *iconname,
						 VWindow *win, char *title = 0);
				VMdiView(const char *iconname, const VFrame& frame,
						 VWindow *win,
						 const VStyle& style = StyleDefault, char *title = 0);
				VMdiView(const VString& name,
						 const VStyle& style, const char *iconname,
						 VWindow *win, char *title = 0);
				VMdiView(const VString& name,
						 const char *iconname, const VFrame& frame,
						 VWindow *win,
						 const VStyle& style = StyleDefault, char *title = 0);

				~VMdiView();

	void		operator()(const char *iconname, const VFrame& frame,
						   VWindow *win, const VStyle& style, char *title);
	void		operator()(const char *iconname, const VFrame& frame,
						   VWindow *win, const VStyle& style, char *title,
						   const VString& name);

	VClass *iam();

	virtual boolean deactivate(VView *activated);
	virtual boolean activate(VView *deactivated);
	boolean close();
	void	maximize();
	void	minimize();

	boolean isIconic();
	boolean isMaximized();

	WinType winType();

#ifndef CV_NOARCHIVER
	virtual void	putTo(VArchiver& a);
	virtual void	getFrom(VArchiver& a);
	virtual void	getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

	virtual VStyle&	getDefaultStyle() const;
	static VStyle&	getClassDefaultStyle();

protected:
	void		getInfo(VWinInfo& info);

#ifndef CV_NOARCHIVER
	void		putTo(VArchiver& a, boolean most_derived);
#endif

protected:
	const char *pName;

private:
	friend		VNotifier;
};

extern VClass *VMdiViewCls;

#endif // VMDIVIEW_H
