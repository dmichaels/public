// $Header:   Q:/views/win/vcs/checkbox.h_v   1.15   Mar 04 1997 16:32:00   gregm  $ 

//	checkbox.h
//
//	VCheckBox class interface
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
//	05/13/93 pmcm	preened;
//					added constructor that takes name argument;
//					added getFrom(), putTo(), realize(), ref to VArchiver,
//					and boolean checkedFlag for realize().
//	05/14/93 pmcm	added destructor (virtual)
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	08/09/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	08/16/93 jmd	made name const VString
//	05/24/95 jld	inherit toggle() from VButton
//	09/01/95 dss	VStyle changes.
//  01/24/95 evc	Put toggle back in and overrode VButton::click
//	01/29/96 dgm	Overrode getPreferredSize() & getSystemBorderSize().
//	04/19/96 glm	removed v1.1 code/preened
// ---------------------------------------------------------------------------

#ifndef VCHECKBOX_H
#define VCHECKBOX_H

#include "button.h"

extern VClass *VCheckBoxCls;
CLASS VGroup;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

//	Introduction
//
//	The VCheckBox class is a subclass VButton that is displayed with a
//	square box to the left of the text content (label). The square box can
//	be empty (unchecked) or marked with an x (checked). If a VCheckBox
//	object has the automatic attribute, then it will automatically toggle
//	between the checked and unchecked state.
//

CLASS VCheckBox : public VButton {

public:
	//	Constructors

				VCheckBox();
				VCheckBox(unsigned cntrlid, VWindow *pwin);
				VCheckBox(	VRatio x, VRatio y, 
							const char *label, 
							VWindow *pwin, 
							const VStyle& style = StyleAutoToggle);
				VCheckBox(	const VFrame& frame, 
							VWindow *pwin, 
							const VStyle& style = StyleAutoToggle, 
							const char *label = 0);
				VCheckBox(	const VString& wname,
							const VFrame& frame, 
							VWindow *pwin, 
							const VStyle& style = StyleAutoToggle, 
							const char *label = 0);
				~VCheckBox();

	VClass 		   *iam();

	//	Interrogating and Setting State

	// added for specialized behavior of autocheckboxes
	boolean toggle();
	boolean clicked();

	virtual void 	check(boolean b);
	virtual boolean checked();
	void 			sizeOf(int *w, int *h);
	virtual boolean	givenFocus();
	virtual boolean	clearFocus();

	//	Geometry

    virtual void	getPreferredSize(int *w, int *h) const;
    virtual void	getSystemBorderSize(int *, int *) const;

	//	Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			putTo(VArchiver& a, boolean most_derived);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

	friend VGroup;

	boolean			notify(int);
	void 			getInfo(VWinInfo& chkinfo);
	UINT			getCtlColor();

private:

#ifndef CV_NOARCHIVER
	boolean			checkedFlag;	//	realization data
#endif

};

#endif	//	VCHECKBOX_H
