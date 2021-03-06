// $Header:   Q:/views/win/vcs/button.h_v   1.14   Mar 04 1997 16:31:58   gregm  $ 

//	button.h
//
//	VButton class interface
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
//	11/01/92 jmd	preened
//	04/09/93 pmcm	changed id to VObject *
//	04/28/93 pmcm	added VButton() with name argument;
//					added getFrom(), putTo(), realize();
//					added reference to VArchiver
//	05/14/93 pmcm	added destructor (virtual)
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pmcm	added clickMthdIndex; removed free()
//	08/08/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	08/18/93 dgm	added getClickMethodIndex().
//	09/01/93 dgm	M8 changes; cast return value of getClickMethodIndex().
//	08/24/94 jld	changed isauto mem var from unsigned short to boolean
//	08/31/94 dss	Added getCtlColor().
//	11/03/94 jmd	preened
//	03/06/95 jld	added enable() with default argument for 
//					consistency w/ Motif
//	05/23/95 jld	added virtual functions givenFocus() and clearFocus() to
//					update 'state' data member
//	05/24/95 jld	made isAutoToggle(), autoToggle() virtual
//	09/01/95 dss	VStyle changes.
//	01/05/96 dgm	Overrode getPreferredSize().
//	04/19/96 glm	removed v1.1 code/preened
//	05/20/96 dgm	Added enableBool().
//	02/04/97 pkt	Changed 'autoToggle' to set the StyleAutoToggle style.
// ---------------------------------------------------------------------------

#ifndef VBUTTON_H
#define VBUTTON_H
#include "control.h"

CLASS VOrdCollect;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VButtonCls;

#define BTTNdraw		0x01	// draw entire user button
#define BTTNenabled		0x02	// user button is enabled
#define BTTNselectDelta	0x04	// user button selection status changed
#define BTTNselect		0x010	// current selection status
#define BTTNfocusDelta	0x020	// user button focus status changed
#define BTTNfocus		0x040	// current focus status
#define BTTNskipClick	0x08	// skip the next BN_CLICKED message for this button

// The VButton class is a subclass of VControl
// that provides an active area of the screen that is used to elicit
// a response for a client object when it is clicked on by the user. The
// VButton class should be subclassed for each user-drawn button. 
// Only the 'paint' method needs to be overridden.
// The VButton class encapsulates the common behavior of all
// button styles.  Buttons may have several attributes: on/off, 
// checked/unchecked, hilite/unhilite, etc. 
// VButton objects may have an "auto" attribute which
// provides for the automatic "toggle" of their state when they
// are clicked on, otherwise, their state is set through a message. 
// Subclasses of the VButton class define their specialized
// behavior and states. 
// The text contents of a VButton is the button label and it is set/retrieved
// by calls to VControl::put/getText() respectively.

CLASS VButton : public VControl {
public:

			VButton();
			VButton(unsigned ctrl, VWindow *pWin);
			VButton(const VFrame &, VWindow *,
						const VStyle& style = StyleDefault);
			VButton(const VString& wname, const VFrame &frame, 
						VWindow *pwin, const VStyle& style = StyleDefault);
			~VButton();

			VClass *iam();

	virtual boolean		autoToggle(boolean b);
	virtual boolean		isAutoToggle();
			void		uponClick(VObject *, method);
	virtual boolean		enable(boolean = TRUE);
	virtual void		enableBool(boolean);
	virtual boolean		givenFocus();
	virtual boolean		clearFocus();
			void		getMin(int *, int *);
	enum WinType		winType();

	// Event Handling
	virtual boolean		toggle();
	virtual boolean		clicked();
	virtual boolean 	enterHit();
	virtual boolean 	paint();

	//	Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);

	integer			getClickMethodIndex() const;
	void			setClickMethodIndex(integer);
#endif

	method			clickMthd;	// Callback used when a button is clicked on
	boolean 		isauto;		// The auto-toggle state of a VButton
	short			state;		// state options for user draw VButton objects

protected:

	void			getInfo(VWinInfo &);
	UINT			getCtlColor();

	boolean 		notify(int);
	void			getMinSize(short&, short&);

    virtual void	getPreferredSize(int *w, int *h) const;

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

private:
	
#ifndef CV_NOARCHIVER
	integer 		clickMthdIndex;
	boolean			isAutoBack;
#endif

};

// ---------------------------------------------------------------------------
// If 'b' is TRUE, then toggle the state of this VButton when
// clicked() is called, otherwise leave its state alone.
//
inline boolean VButton::autoToggle(boolean b)
{
#ifdef CVS_DYNAMIC
	if (isauto == b) {
	}
	else if (b) {
		style += StyleAutoToggle;
	}
	else {
		style += StyleNoAutoToggle;
	}
#endif
	return (isauto = b);
}

// ---------------------------------------------------------------------------
// Return TRUE if this VButton toggles when clicked, otherwise return FALSE.
inline boolean VButton::isAutoToggle()
{
	return isauto;
}

//============================================================================
#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
inline integer VButton::getClickMethodIndex() const
{
	return clickMthdIndex;
}

// ---------------------------------------------------------------------------
inline void VButton::setClickMethodIndex(integer index)
{
	clickMthdIndex = index;
}

#endif
//============================================================================

#endif	//	VBUTTON_H
