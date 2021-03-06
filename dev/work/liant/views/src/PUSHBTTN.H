// $Header:   Q:/views/win/vcs/pushbttn.h_v   1.11   Mar 04 1997 16:32:54   gregm  $ 

//	pushbttn.h
//
//	VPushButton interface
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
//	04/28/93 pat	added VPushButton() with name argument;
//					added getFrom(), putTo(), realize();
//					added reference to VArchiver
//	05/14/93 pat	added destructor (virtual)
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	08/08/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	08/16/93 jmd	made name const VString
//	09/29/93 dgm	added isAutoDefault().
//	05/24/95 jld	autoToggle() and isAutoToggle() inherited from VButton,
//	09/01/95 dss	VStyle changes.
//	01/04/96 dgm	Overrode getSystemBorderSize().
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#ifndef VPUSHBTTN_H
#define VPUSHBTTN_H
#include "button.h"

extern VClass *VPushButtonCls;
CLASS VGroup;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

//	A `VPushButton` is a button that is displayed with a border
//  surrounding its text contents.  `VPushButton` objects can be hilited
//  or unhilited.  `VPushButton` objects can also have the automatic attribute
//  which provides for an automatic toggle between the hilited and unhilited
//  states. A `VPushButton` object can also have the 'default' style attribute.
//  A `VPushButton` object with this style is drawn with a thicker border to
//  indicate that it is the default button choice and hence, it can be selected
//  by simply pressing the ENTER key.  By default, `VPushButton` object take on
//  the default style when they receive keyboard focus.  However, this behavior
//  can be disabled by constructing a `VPushButton` with the `StyleNoAutoDefault`
//  style flag or by calling the member autoDefault(FALSE). 

CLASS VPushButton : public VButton {

public:

	VClass *iam();

			VPushButton();
			VPushButton(unsigned, VWindow *);
			VPushButton(VRatio, VRatio, char*, 
							VWindow *, 
							const VStyle& style = StyleDefault);
			VPushButton(const VFrame &, 
							VWindow *, 
							const VStyle& style = StyleDefault, 
							char *label = 0);
			VPushButton(const VString& wname, 
							const VFrame &frame, 
							VWindow *pwin, 
							const VStyle& style = StyleDefault, 
							char *label = 0);
			~VPushButton();

	// Setting and Interrogating Attributes
	void			autoDefault(boolean b);
	boolean			isAutoDefault() const;
	virtual void	setDefault(boolean);
	void			sizeOf(int *, int *);
	boolean			isDefault();
	virtual void	hilite(boolean);
	virtual boolean	isHilited();

	// Event Handling
	boolean 		clearFocus();
	boolean 		enterHit();
	boolean 		givenFocus();

	// Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			putTo(VArchiver& a, boolean most_derived);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

	boolean			isdefault;   
	void			getInfo(VWinInfo &);
    virtual void	getSystemBorderSize(int *, int *) const;
	friend VGroup;

};

// ---------------------------------------------------------------------------
inline boolean VPushButton::isAutoDefault() const
{
	return style.contains(StyleNoAutoDefault);
}

// ---------------------------------------------------------------------------
//	If 'b' is TRUE, disable the automatic setting of the default
//	status when this `VPushButton` receives focus, otherwise enable automatic
//	default status.  If a `VPushButton` object is created without the
//	`StyleNoAutoDefault` style, then it will automatically have the default
//	status when it receives the keyboard focus.
//
inline void VPushButton::autoDefault(boolean b) 
{
	if (b) {
		style += StyleNoAutoDefault;
	}
	else {
		style -= StyleNoAutoDefault;
	}
}

// ---------------------------------------------------------------------------
//	Return TRUE if this button has the default push button style,
//	otherwise return FALSE.
//
inline boolean VPushButton::isDefault()
{
	return(isdefault);
}
// ---------------------------------------------------------------------------

#endif // VPUSHBTTN_H
