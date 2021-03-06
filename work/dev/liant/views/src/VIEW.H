// $Header:   Q:/views/win/vcs/view.h_v   1.23   Mar 04 1997 16:33:16   gregm  $ 

//	view.h
//
//	VView interface
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
//	03/25/93 pat	preened
//	04/28/93 pat	added VView() and operator() with name argument;
//					getFrom(), putTo(), realize(); reference to VArchiver.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/25/93 jmd	changed setTitle to const VString, VFrame to const VFrame
//					removed free()
//	08/16/93 jmd	made name const VString
//	10/06/93 dgm	moved model and setModel() up to VWindow. (jmd).
//	08/31/94 dss	added isTopLevel().
//	11/03/94 dss	Moved accelerator, def button, focus mnmgt from VDialog.
//	09/01/95 dss	VStyle changes. Removed CPPV_11 code.
//	09/29/95 dgm	Added constrainMinimumSize() and resizeClientArea().
//	10/18/95 dgm	Removed constrainMinimumSize() and resizeClientArea().
//					Added getNonClientAreaSize().
//  11/01/95		Chaned definition of setACcel so it takes a method
//					Also added unsetAccel and assignAccelSet().
//	11/01/95 dgm	Moved getComponentThis() from view.h to view.cpp.
//	11/02/95 dgm	Changed getNonClientAreaSize() to getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	01/09/96 dss	VComponent name change.
//	01/29/96 dgm	Changed isTopLevel() to be a const function.
//	05/01/96 glm	added getFromWithTag to pass archive tag back to 
//					VTreeListBox, so old archives can be read.
// ----------------------------------------------------------------------------

#ifndef VIEW_H
#define VIEW_H

#include "set.h"
#include "str.h"
#include "scrollbr.h"
#include "sysmenu.h"
#include "window.h"

//	The VView class is derived from the VWindow class.  It provides 
//	the basis for building application specific views and subviews within
//	the MVC (Model View Controller) program paradigm.  A VView has an 
//	associated model object which contains the data to be displayed and/or
//	modified through the VView.
//	
//	Views may be created in several styles:
//	
//		StyleBorder 	give the view a simple border
//		StyleTitle		give the view a title bar
//		StyleSizable  	make the view sizable
//		StyleCloseBox 	include a close box in the view title
//		StyleHorizontal give the view a horizontal scroll bar
//		StyleVertical 	give the view a vertical scroll bar
//		StyleHidden 	to have the view initially shown as hidden.
//	
//	These style attributes may be added together to form composite
//	VView object styles.  The StyleDefault style flag is defined for 
//	creating simple unframed VView objects.

extern VClass *VViewCls;

CLASS VNotifier;
CLASS VButton;
CLASS VPushButton;
CLASS VSet;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

CLASS VComponentView;

CLASS VView : public VWindow {

friend VNotifier;
friend VPushButton;

public:

					VView();
					VView(	const VFrame&	frame, 
							VWindow    *pwin, 
							const VStyle& style = StyleDefault);
					VView (	const VString& wname,
				       		const VFrame&	frame,
			       			VWindow    *pwin, 
							const VStyle& style = StyleDefault);

					~VView();

	VClass 		   *iam();
	enum WinType 	winType();

	//	building the presentation window

	void 			operator()(	const VFrame& frame, VWindow *pwin, const VStyle& style);
	void			operator()(	const VFrame& frame, VWindow *pwin, const VStyle& style, 
								const VString& wname);

	VSysMenu 	   *getSysMenu();	// VMenu associated with the close box

	//	event handling

	boolean				close();
	virtual boolean	activate(VView *w);
	virtual boolean	deactivate(VView *w);

	// styles

	static VStyle&		getClassDefaultStyle(void);
	virtual VStyle&	getDefaultStyle(void) const;

	//	retrieving scrollbars

	VScrollBar 	   *getHScroll();
	VScrollBar 	   *getVScroll();

	static void 	stdSize(const VStyle& style, int *width, int *height);
	// void			setModel(VObject *m);	// moved to up to VWindow.
	int 			getTitle(VString& s);
	int 			getTitle(char *s, int n);
	void 			setTitle(const VString& s);
	void 			setSizable();

	// Focus
	boolean			givenFocus();
	boolean			takeFocus();
	void			setChildFocus(VWindow *win);

	// Top Level Window behavior
	void			setDefButton(VPushButton *button);
	void			setEscButton(VPushButton *button);
	virtual void	setAccel(int key, VWindow *win , method=0);
    virtual void    unsetAccel(int key);

    void            accelKeysOff();
    void            accelKeysOn();

	VPushButton	   *getDefButton() const;
	VPushButton	   *getEscButton() const;

	// Change the accel set and return old 1
	VSet *	 assignAccelSet(VSet*);

	virtual boolean ok(VButton *);
	virtual boolean cancel(VButton *);
	boolean			okPressed(VButton *);
	boolean			cancelPressed(VButton *);
	boolean			isTopLevel() const;
	boolean			enterHit();
	boolean			escapeHit();
	boolean			key(int, char);

    void			enableBool(boolean);

	// Geometry management.

    virtual void	getSystemBorderSize(int *, int *) const;

	// Embedded Objects
	VComponentView			*getComponentThis();

	//	Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);

	void			getFromWithTag(VArchiver& a, long *viewTag);

	integer			iDefButton;		// 	realization data
	integer			iEscButton;		// 	realization data
#endif

protected:

	void 			clean();
	void 			getInfo(VWinInfo& viewinfo);
	virtual void 	getMinSize(short& width, short& heigth);
	void 			init();

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

	VScrollBar 	   *hScroll; 
	VScrollBar 	   *vScroll;
	VString 		sTitle;
	VSysMenu   	   *sysmenu;		//	menu of close box, if box is present
	// VObject 	   *model; 			//	application model this VView represents

	VPushButton 	*defButton;		//	focus when no others have focus
	VPushButton 	*escButton;
	VSet				*accels;		//	accelerator table associated w
										// the view. 
    boolean         dontKey;

};

//---------------------------------------------------------------------------
//
// inline 	void VView::setModel(VObject * m) 
// { 
// 	model = m; 
// }

//---------------------------------------------------------------------------
//	Return the horizontal VScrollBar object attached to this VView. 
//	This is the VScrollBar object that is automatically created when this
//	VView is constructed with the StyleHorizontal style.
//
inline VScrollBar *VView::getHScroll() 
{ 
	return hScroll; 
}

//---------------------------------------------------------------------------
//	Return the vertical VScrollBar object attached to this VView. 
//	This is the VScrollBar object that is automatically created when this
//	VView is constructed with the StyleVertical style.
//
inline VScrollBar *VView::getVScroll() 
{ 
	return vScroll; 
}

// ---------------------------------------------------------------------------
//
//
inline void VView::accelKeysOn()
{
    dontKey = FALSE;
}

// ---------------------------------------------------------------------------
//
//
inline void VView::accelKeysOff()
{
    dontKey = TRUE;
}


//--------------------------------------------------------------------------
//  assign a new set of accelerators, and get back the old one.

inline VSet *	 VView::assignAccelSet(VSet* newset){
	VSet * oldset = accels;
	accels = newset;
	return oldset;
}

// ----------------------------------------------------------------------------
//
inline VPushButton *VView::getDefButton() const
{
	return defButton;
}

// ----------------------------------------------------------------------------
//
inline VPushButton *VView::getEscButton() const
{
	return escButton;
}

#endif	//	!VIEW_H

