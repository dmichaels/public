// $Header:   Q:/views/win/vcs/ITEMWIN.H_v   1.5   Mar 04 1997 14:12:50   gregm  $

//	itemwin.h
//
//  Compound Document Item VWindow
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
//	02/13/95 dss	Initial release.
//	---------------------------------------------------------------------------

#if !defined( _ITEMWINDOW_H_ )
#define _ITEMWINDOW_H_

#include "window.h"
#include "str.h"

#ifndef CV_NOARCHIVER
CLASS VArchiver;
CLASS VRscArchiver;
#endif

extern VClass *VItemWindowCls;

CLASS VItemWindow : public VWindow
{
public:
	VItemWindow();
	VClass	*iam();
	~VItemWindow();
	
	VItemWindow(const VFrame&	frame,
					 VWindow	*pwin,
					 const VStyle& style = StyleDefault);

	void			getInfo(VWinInfo &info);

	void			setUserClassName(const VString& uclass);
	VString&		getUserClassName();

#ifndef CV_NOARCHIVER
	//	Archival and Retrieval
	void			putTo(VArchiver& a);
	void			putTo(VArchiver& a, boolean most_derived);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

private:

	VString			userClassName;

};

// ---------------------------------------------------------------------------
//
inline void VItemWindow::setUserClassName(const VString& uclass)
{
	userClassName = uclass;
}

// ---------------------------------------------------------------------------
//
inline VString& VItemWindow::getUserClassName()
{
	return userClassName;
}

#endif // _ITEMWINDOW_H_


