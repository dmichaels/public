// $Header:   Q:/views/win/vcs/scrollbr.h_v   1.14   Mar 04 1997 16:33:02   gregm  $ 

//	scrollbr.h
//
//	VScrollBar interface.
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
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
//	11/05/92 jmd	preened
//	04/09/93 pat	changed id to VObject *
//	06/21/93 pat	added support for VArchiver; preened.
//					added constructor that takes name argument.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pat	removed free(); added changeMthdIndex.
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	08/23/93 dgm	added get/setChangeMethodIndex().
//					added setVertical(), setHorizontal(),
//					isHorizontal(), and isVertical().
//	08/25/93 dgm	added isTracking().
//	09/01/93 dgm	M8 changes; cast return value of getChangeMethodIndex().
//	01/27/94 dgm	added additional getText() to avoid hiding warnings.
//	02/09/94 jmd	converted to use long values
//	03/01/94 pat	putText() now takes const VString&
//	08/31/94 dss	Added getCtlColor().
//	09/01/95 dss	VStyle changes.
//	02/09/96 dgm	Added getPreferredSize().
//	04/19/96 glm	removed v1.1 code/preened
//	04/23/96 dgm	Added setPageSize(), getPageSize(), and setScrollValues().
//	08/13/96 dgm	Added dragging for VNotifer flag.
// ----------------------------------------------------------------------------

#ifndef VSCROLLBAR_H
#define VSCROLLBAR_H

#include "control.h"

CLASS VNotifier;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VScrollBarCls;

//	Introduction
//
//	The VScrollBar class provides a mechanism for the user to visually
//	select a value from a range of possible values using a scroll-bar.
//	Visually a scroll bar is composed of two 'arrows' at each end, a
//	rectangular 'thumb' defining its current value and two rectangular
//	'bands' between the arrows and the thumb. Two types of scroll bars are
//	defined: vertical and horizontal. A scroll bar has an integer value
//	which may be varied by the user. Four integer member variables define
//	the effect of clicking parts of a VScrollBar object: 'minVal',
//	'maxVal', 'lineAmt' and 'pageAmt'. The extreme minimum and maximum
//	values of a scroll bar are 'minVal' and 'maxVal', respectively. When
//	its arrows are clicked, its value is incremented/decremented by
//	'lineAmt'. When its 'bands' are clicked, its value is
//	incremented/decremented by 'pageAmt'. The default values for 'minVal',
//	'lineAmt', 'pageAmt' and 'maxVal' are 0, 100, 1000 and 10000,
//	respectively. A VScrollBar has two operational modes: 'tracking' or
//	'non-tracking'. In tracking mode, scroll bar values are continously
//	fed back to the client object while the user holds down the mouse
//	button. In non-tracking mode, the scrollbar values are only sent to
//	the client object when the user releases the mouse button. The default
//	mode is non-tracking.

CLASS VScrollBar : public VControl {

public:
				VScrollBar();
				VScrollBar(	unsigned cntrlId, VWindow *pwin);
				VScrollBar(	boolean vertical, VWindow *pwin);
				VScrollBar(	const VFrame& frame, 
							VWindow *pwin, 
							const VStyle& style = StyleDefault);
				VScrollBar(	const VString& wname, 
							const VFrame& frame, 
							VWindow *pwin, 
							const VStyle& style = StyleDefault);
				~VScrollBar();

	VClass 		   *iam();
	enum WinType 	winType();

	void			showBool(boolean tf, boolean update);
	virtual boolean isHidden();
	void			putText(const VString& s);
	int				getText(char *buf, unsigned len);
	int				getText(VString& s);
    VString			getText();
	  
	//	Tracking 
	    
	void			tracking(boolean enable);
	virtual void 	track(long val);				
	boolean			isTracking() const;

	//	Notification of Value Change 
	    
	void			setChangeMthd(method mthd);
	void			uponChange(VObject *obj, method mthd);
	void			uponEndChange(VObject *, method);
	void			uponLineClick(VObject *, method);
	void			uponPageClick(VObject *, method);

	//	Interrogating and Setting Range, Line and Page Amounts

	long			getMax();
	long			getMin();
	long			getPageAmt();
	long			getLineAmt();

	void			setValues(long max, long min, 
								long lineAmout, long pageAmout);

	void			setScrollAmts(long lineAmount, long pageAmount);
	void			setRange(long max, long min);

	void			setPageSize(long);
	long			getPageSize() const;

	void			setScrollValues(long min, long max, long page);
	void			setScrollValues(long min, long max, long page, long pos);
	void			setScrollValues(long min, long max, long page, long pos,
									long line_amount, long page_amount);

	void			setVertical(boolean = TRUE);
	void			setHorizontal(boolean = TRUE);
	boolean			isVertical() const;
	boolean			isHorizontal() const;

	//	Interrogating and Setting Value 
	    
	void			lineDown();
	void			lineUp();
	void			pageDown();
	void			pageUp();
	void			setPos(long pos);
	long			getPos();

	VScrollBar& 	operator =(long pos);
					operator long();
	virtual void 	endScroll(long val);

	long			scaleValue(int val);

	boolean			isPartOfView() const;

	// Geometry

    virtual void	getPreferredSize(int *w, int *h) const;

#ifndef CV_NOARCHIVER
	//	Archival and Retrieval

	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
	integer			getChangeMethodIndex() const;
	void			setChangeMethodIndex(integer);
#endif

protected:
	void	init(unsigned n);
	void	getInfo(VWinInfo& scrollinfo);
	UINT	getCtlColor();
	void	sizeOf(int *w, int *h);
	void	getMinSize(short& w, short& h);

#ifndef CV_NOARCHIVER
	void	putTo(VArchiver& a, boolean most_derived);
#endif

private:

#ifndef CV_NOARCHIVER
	integer	changeMthdIndex;	//	(realization data)
	integer	endChangeMthdIndex;	//	(realization data)
	integer	lineClickMthdIndex;	//	(realization data)
	integer	pageClickMthdIndex;	//	(realization data)
#endif

protected:
	long	minVal;			//	minimum value (value at top or left)
	long	maxVal;			//	maximum value (value at bottom or right)
	long	pageAmt;		//	amount to scroll for page up or page down
	long	lineAmt;		//	amount to scroll for line up or line down
	long	pageSize;
	float	scalingFactor;	// scaling from long to int...
	long	currentPos;		// realization data
	method	changeMthd;		// callback method when scrollbar value changes
	boolean	(VObject::*endChangeMthd)();
	boolean (VObject::*lineClickMthd)(int);
	boolean (VObject::*pageClickMthd)(int);
	char	dotrack : 1;	// TRUE, tracking enabled; FALSE, otherwise.
	char	hidden : 1;
	char	control : 1;
	char	vertical : 1;
	char	dragging : 1;

private:
	friend	VNotifier;
};


// ===========================================================================
#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
inline integer VScrollBar::getChangeMethodIndex() const
{
	return changeMthdIndex;
}

// ---------------------------------------------------------------------------
inline void VScrollBar::setChangeMethodIndex(integer index)
{
	changeMthdIndex = index;
}
// ---------------------------------------------------------------------------

#endif
// ===========================================================================

// ---------------------------------------------------------------------------
//
inline void VScrollBar::tracking(boolean enable) 
{ 
	dotrack = enable; 
}

// ---------------------------------------------------------------------------
//
inline void VScrollBar::setChangeMthd(method mthd) 
{ 
	changeMthd = mthd; 
}

// ---------------------------------------------------------------------------
//
inline long	VScrollBar::getMax() 
{ 
	return maxVal; 
}

// ---------------------------------------------------------------------------
//
inline long	VScrollBar::getMin() 
{ 
	return minVal; 
}

// ---------------------------------------------------------------------------
//
inline long	VScrollBar::getPageAmt() 
{ 
	return pageAmt; 
}

// ---------------------------------------------------------------------------
//
inline long	VScrollBar::getLineAmt() 
{ 
	return lineAmt; 
}

// ---------------------------------------------------------------------------
//
inline VScrollBar& VScrollBar::operator =(long pos) 
{ 
	setPos(pos); 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VScrollBar::operator long() 
{ 
	return getPos(); 
}

// ---------------------------------------------------------------------------
inline boolean VScrollBar::isTracking() const
{ 
	return dotrack;
}

// ---------------------------------------------------------------------------
// convert int to long using scalingFactor
// private to Windows version
//
inline long VScrollBar::scaleValue(int val)
{
	return long(scalingFactor * val);
}

// ---------------------------------------------------------------------------
//
inline boolean VScrollBar::isPartOfView() const
{
	return !control;
}

// ---------------------------------------------------------------------------
//
inline long VScrollBar::getPageSize() const
{
	return pageSize;
}

// ---------------------------------------------------------------------------
//
inline VString VScrollBar::getText()
{
	return VControl::getText();
}

#endif // VSCROLLBAR_H
