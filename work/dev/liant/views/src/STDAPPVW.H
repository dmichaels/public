// $Header:   Q:/views/win/vcs/stdappvw.h_v   1.7   Mar 04 1997 16:33:04   gregm  $ 

//	stdappvw.h
//
//	VStdAppView interface
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
//	07/28/94 dgm	Original
//	09/01/95 dss	VStyle changes.
//  02/26/96 evc    class -> CLASS
// ----------------------------------------------------------------------------

#ifndef STDAPPVW_H
#define STDAPPVW_H

// --------------------------------------------------------------------------
//
#include "appview.h"

// --------------------------------------------------------------------------
//
CLASS	VAppView;
CLASS	VClass;
CLASS	VFrame;
CLASS	VMenu;
CLASS	VString;
CLASS	VStatusBar;
CLASS	VToolBar;

// --------------------------------------------------------------------------
//
CLASS VStdAppView : public VAppView {

public:
					    VStdAppView(const VFrame&,
									int tool_bar_height = -1,
									int status_bar_height = -1,
									const VStyle& style = StyleDefault);
					    VStdAppView(int do_realize = 1);
					    VStdAppView(const VString&,
									const VFrame&,
									int tool_bar_height = -1,
									int status_bar_height = -1,
									const VStyle& style = StyleDefault);
					   ~VStdAppView();
	VClass			   *iam();

								 
	void				operator()(const VFrame& frame, const VStyle& style);
	void				operator()(const VFrame& frame, const VStyle& style,
								   const VString& name);

	void				hideToolBar(boolean = TRUE);
	void				hideStatusBar(boolean = TRUE);
	void				showToolBar(boolean = TRUE);
	void				showStatusBar(boolean = TRUE);
	boolean				isToolBarHidden() const;
	boolean				isStatusBarHidden() const;

	VWindow			   *getClientWindow() const;

	virtual	boolean		displayPrompt(VString *);

#ifndef CV_NOARCHIVER
	virtual void		putTo(VArchiver& a);
	virtual void		getFrom(VArchiver& a);
	virtual void		getFrom(VArchiver& a, VObject *pwin,
								boolean do_realize = TRUE);
	virtual void		realize(VWindow *pwin);
#endif

protected:
	void				initialize();
	void				construct();
	void				createMenuBar();
	void				createStatusBar(int height = -1);
	void				createToolBar(int height = -1);
	void				createClientArea();

#ifndef CV_NOARCHIVER
	void				putTo(VArchiver& a, boolean most_derived);
#endif

private:
	VMenu			   *menuBar;
	VToolBar		   *toolBar;
	VStatusBar		   *statusBar;
	VWindow			   *clientArea;
};

extern VClass *VStdAppViewCls;

// --------------------------------------------------------------------------
//
inline VWindow *VStdAppView::getClientWindow() const
{
	return clientArea;
}

#endif // STDAPPVW_H
