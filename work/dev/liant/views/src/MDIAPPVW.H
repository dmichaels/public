// $Header:   Q:/views/win/vcs/mdiappvw.h_v   1.9   Mar 04 1997 16:32:34   gregm  $ 

//	mdiappvw.h
//
//	VMdiAppView interface
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
//	04/09/93 pat	changed id to VObject *
//	06/24/93 jmd	preened, removed free()
//	08/08/93 jmd	made VFrame const
//	09/14/93 dgm	added isClosing().
//	02/09/94 dgm	override showBool() to make sure all VPopupWindow
//					children get hidden when hiding the VMdiAppView.
//	08/09/94 dgm	Added archiver support; added operator(); added
//					do_realize to VMdiAppView(); added initialize().
//	09/01/95 dss	VStyle changes.
//	11/01/95 dgm	Changed activate() signature.
//	01/30/95 evc	Removed a constructor that had no implimentation:
//					VMdiAppView(const VString&, const VFrame& f)
// ----------------------------------------------------------------------------

#ifndef VMDIAPPVW_H
#define VMDIAPPVW_H

#include "appview.h"
#include "mdiclnt.h"

CLASS VMdiView;
CLASS VNotifier;
CLASS VPopupMenu;
CLASS VView;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VMdiAppViewCls;

//
// VMdiAppView is a MS Windows specific class that implements
// the MDI VFrame window in an MDI application. An VMdiAppView is
// an VAppViewwith added functionality for constructing and interfacing
// with the VMdiClient object and its contained VMdiView child windows. 
// 

CLASS VMdiAppView : public VAppView {
public:
				VMdiAppView(int do_realize = 1);
				VMdiAppView(const VFrame& f);
				VMdiAppView(const VFrame& f, const VFrame& clientframe,
							const VStyle& client_style = StyleHorizontal +
												StyleVertical);
				VMdiAppView(const VString&, const VFrame& f);
				VMdiAppView(const VString&,
							const VFrame& f, const VFrame& clientframe,
							const VStyle& client_style = StyleHorizontal +
												 StyleVertical);
			   ~VMdiAppView();
	VClass	   *iam();

	void			operator()(const VFrame&, const VStyle&);
	void			operator()(const VFrame&, const VFrame&, const VStyle&);
	void			operator()(const VFrame&, const VStyle&, const VString&);
	void			operator()(const VFrame&, const VFrame&,
						   const VStyle&, const VString&);

	void			addPopup(VObject *m);
	VPopupMenu *mdiPopup();
	boolean			activate(VView *child);
	VMdiView	   *activeChild();
	void			tile();
	void			arrange();
	void			cascade();

	boolean	  	nextChildFocus(VWindow *child);
	boolean		prevChildFocus(VWindow *child);
	void			setChildFocus(VWindow *win);
	VWindow		*getChildFocus();

	WinType		winType();

	VWindow	   *clientOf();

	boolean		close();
	boolean		quit();
	boolean		isClosing() const;

#ifndef CV_NOARCHIVER
	virtual void	putTo(VArchiver& a);
	virtual void	getFrom(VArchiver& a);
	virtual void	getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:
	void		getInfo(VWinInfo& info);
	void 		showBool(boolean, boolean update = TRUE);
	void		initialize();

#ifndef CV_NOARCHIVER
	void		putTo(VArchiver& a, boolean most_derived);
#endif

protected:
	VMdiClient	client;		// The MDI client window for this application

private:
	boolean		closing;

private:
	friend		VNotifier;
};

// ---------------------------------------------------------------------------
// Return the MDI VWindow VPopupMenu for this application.
//
inline VPopupMenu *VMdiAppView::mdiPopup()
{
	return client.getMdiWinMenu();
}

// ---------------------------------------------------------------------------
// Return the current active VMdiView child window.
//
inline VMdiView *VMdiAppView::activeChild()
{
	return client.activeChild();
}

// ---------------------------------------------------------------------------
// Tile the currently non-iconized VMdiView windows.
//
inline void VMdiAppView::tile()
{
	client.tile(0);
}

// ---------------------------------------------------------------------------
// Arrange the currently iconized VMdiView windows.
//
inline void VMdiAppView::arrange()
{
	client.arrange(0);
}

// ---------------------------------------------------------------------------
// Cascade the currently non-iconized VMdiView windows.
//
inline void VMdiAppView::cascade()
{
	client.cascade(0);
}

// ---------------------------------------------------------------------------
// Return the MDI Client window of this VMdiAppView object.
//
inline VWindow *VMdiAppView::clientOf()
{
	return &client;
}

// ---------------------------------------------------------------------------
// Return TRUE if we are in the process of closing down this application,
// otherwise FALSE.
//
inline boolean VMdiAppView::isClosing() const
{
	return closing;
}

#endif // VMDIAPPVW_H
