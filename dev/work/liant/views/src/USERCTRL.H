// $Header:   Q:/views/common/vcs/userctrl.h_v   1.6   May 30 1997 17:17:10   scottt  $ 

//	userctrl.h
//
//	VUserControl class interface [Common]
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
//	10/05/92 jmd	created
//	05/25/95 pkt	preened.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VUSERCTRL_H
#define VUSERCTRL_H

#include "control.h"
#include "str.h"

#ifndef CV_NOARCHIVER
CLASS VArchiver;
CLASS VRscArchiver;
#endif

extern VClass *VUserControlCls;

CLASS VUserControl : public VControl {
public:
			VUserControl();
			~VUserControl();

	VClass *iam();

	boolean paint();

	//	Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
	boolean			loadResources(VRscArchiver& a);
#endif

	void		setUserClassName(const VString& uclass);
	VString&	getUserClassName();

protected:

#ifndef CV_NOARCHIVER
	void		putTo(VArchiver& a, boolean most_derived);
#endif

private:

	VString		userClassName;

};

// ---------------------------------------------------------------------------
//
inline void VUserControl::setUserClassName(const VString& uclass)
{
	userClassName = uclass;
}

// ---------------------------------------------------------------------------
//
inline VString& VUserControl::getUserClassName()
{
	return userClassName;
}

#endif	//	VUSERCTRL_H
