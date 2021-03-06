//	panner.h -- [UNDER DEVELOPMENT]
//
//	VPanner interface
//
//  Allegris Foundation 1.1.00
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
//	04/01/96 dgm	Original.
// --------------------------------------------------------------------------

#ifndef PANNER_H
#define PANNER_H

// --------------------------------------------------------------------------
//
#include "control.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VColor;
CLASS	VControl;
CLASS	VObject;
CLASS	VString;
CLASS	VStyle;
CLASS	VWindow;

#ifndef CV_NOARCHIVER
CLASS	VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VPanner : public VControl {
public:
						VPanner(const VFrame&, VWindow *,
								const VStyle& = StyleDefault);
						VPanner(VWindow *,
								const VStyle& = StyleDefault);
						VPanner(const VString&,
								const VFrame&, VWindow *,
								const VStyle& = StyleDefault);
						VPanner(const VString&,
								VWindow *,
								const VStyle& = StyleDefault);
						VPanner();
					   ~VPanner();
	VClass			   *iam();

	void				operator()(const VFrame&, VWindow *);
	void				operator()(const VFrame&, VWindow *, const VStyle&);
	void				operator()(const VFrame&, VWindow *, const VStyle&,
								   const VString&);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	void				setMinimum(long, long);
	void				setMaximum(long, long);
	void				setPosition(long, long);
	void				setPageSize(long, long);
	void				setPageSizeRatio(float, float);
	void				setRange(long, long, long, long);
	void				setValues(long, long, long, long,
								  long, long, long, long);

	void				setHorizontalMinimum(long);
	void				setHorizontalMaximum(long);
	void				setHorizontalPosition(long);
	void				setHorizontalPageSize(long);
	void				setHorizontalPageSizeRatio(float);
	void				setHorizontalRange(long, long);
	void				setHorizontalValues(long, long, long, long);

	void				setVerticalMinimum(long);
	void				setVerticalMaximum(long);
	void				setVerticalPosition(long);
	void				setVerticalPageSize(long);
	void				setVerticalPageSizeRatio(float);
	void				setVerticalRange(long, long);
	void				setVerticalValues(long, long, long, long);

	void				setSubtractPageFromRange(boolean);
	void				setTracking(boolean);
	void				uponBeginChange(VObject *, method);
	void				uponChange(VObject *, method);
	void				uponEndChange(VObject *, method);

	void				setMargins(int, int, int, int);
	void				setThumbBorder(boolean);
	void				setBackgroundColor(const VColor&);
	void				setForegroundColor(const VColor&);
	void				setDefaultColors();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	void				getMinimum(long&, long&) const;
	void				getMaximum(long&, long&) const;
	void				getPosition(long&, long&) const;
	void				getPositionRatio(float&, float&) const;
	void				getPageSize(long&, long&) const;
	void				getPageSizeRatio(float&, float&) const;
	boolean				usingPageSizeRatio(boolean&, boolean&) const;
	void				getRange(long&, long&, long&, long&) const;
	void				getValues(long&, long&, long&, long&,
								  long&, long&, long&, long&) const;

	long				getHorizontalMinimum() const;
	long				getHorizontalMaximum() const;
	long				getHorizontalPosition() const;
	float				getHorizontalPositionRatio() const;
	long				getHorizontalPageSize() const;
	float				getHorizontalPageSizeRatio() const;
	boolean				usingHorizontalPageSizeRatio() const;
	void				getHorizontalRange(long&, long&) const;
	void				getHorizontalValues(long&, long&,
											long&, long&) const;

	long				getVerticalMinimum() const;
	long				getVerticalMaximum() const;
	long				getVerticalPosition() const;
	float				getVerticalPositionRatio() const;
	long				getVerticalPageSize() const;
	float				getVerticalPageSizeRatio() const;
	boolean				usingVerticalPageSizeRatio() const;
	void				getVerticalRange(long&, long&) const;
	void				getVerticalValues(long&, long&,
										  long&, long&) const;

	boolean				isSubtractPageFromRange() const;
	boolean				isTracking() const;
	boolean				isPanning() const;

	void				getMargins(int&, int&, int&, int&);
	boolean				hasThumbBorder() const;
	VColor&				getBackgroundColor() const;
	VColor&				getForegroundColor() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	void				updateThumb();
	void				beginDragging(int, int);
	void				drag(int, int, int = -1);
	void				endDragging(int, int);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

public:

	CLASS Thumb : public VWindow {
	public:
							Thumb(VPanner *);
	protected:
		virtual void		getPreferredSize(int *, int *) const;
		virtual void		getMinimumSize(int *, int *) const;
		virtual boolean		resized(int, int);
		virtual boolean		paint();
		virtual boolean		mouseDn(int, int);
    	virtual boolean		mouseUp(int, int);
	private:
		VPanner			   *thePanner;
		short				thisWidth;
		short				thisHeight;
	};

	class ScrollData {
	public:
							ScrollData();

		boolean				setMinimum(int, long);
		boolean				setMaximum(int, long);
		boolean				setPosition(int, long);
		boolean				setPositions(long, long);
		boolean				setPositions(float, float);
		boolean				setPageSize(int, long);
		boolean				setPageSizeRatio(int, float);
		boolean				setRange(int, long, long);
		boolean				setValues(int, long, long, long, long);
		boolean				setSubtractPageFromRange(boolean);

		long				getMinimum(int) const;
		long				getMaximum(int) const;
		long				getPosition(int) const;
		float				getPositionRatio(int) const;
		long				getPageSize(int) const;
		float				getPageSizeRatio(int) const;
		boolean				usingPageSizeRatio(int) const;
		void				getRange(int, long&, long&) const;
		boolean				isSubtractPageFromRange() const;

	protected:
		long				_min[2];
		long				_max[2];
		long				_pos[2];
		union {
			long			 value;
			float			 ratio;
		}					_page[2];
		unsigned char		_usePageRatio[2];
		unsigned char		_subtractPageFromRange;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

public:

#ifndef CV_NOARCHIVER
	virtual void		putTo(VArchiver&);
	virtual void		getFrom(VArchiver&);
	virtual void		getFrom(VArchiver&, VObject *, boolean = TRUE);
	virtual void		realize(VWindow *);
#endif

protected:
	virtual void		beginChange();
	virtual boolean		change(long, long);
	virtual void		endChange();
	virtual VWindow	   *newThumb(VPanner *);

protected:
    virtual boolean		resized(int, int);
    virtual boolean		mouseMv(int, int, int);
    virtual boolean		mouseUp(int, int);
	virtual boolean		mouseDn(int, int);
    virtual void    	getInternalBorderGeometry(int *, int *,
												  int *, int *) const;
#ifndef CV_NOARCHIVER
	void				putTo(VArchiver&, boolean);
#endif

protected:
	void				initialize();
	void				construct();
	void				createThumb();
	void				getThumbGeometry(int[2], int[2]) const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	ScrollData			scrollData;
	VWindow			   *theThumb;
	VObject			   *clientObject;
	void	 (VObject::*endChangeMethod)();
	boolean	 (VObject::*changeMethod)(long, long);
	void	 (VObject::*beginChangeMethod)();
	unsigned char		ltMargin[2];
	unsigned char		rbMargin[2];
	unsigned char		scrollTracking : 1;
	unsigned char		currentlyPanning : 1;
	unsigned char		customBackground : 1;
	unsigned char		scrollBarBackground : 1;
	unsigned char		thumbCustomColors : 1;
	unsigned char		thumbScrollBarColors : 1;
	unsigned char		thumbBorder : 1;

private:
	friend				Thumb;
};

extern VClass *VPannerCls;

// --------------------------------------------------------------------------
//
inline boolean VPanner::ScrollData::setMinimum(int d, long min)
{
	return setRange(d, min, _max[d]);
}

inline boolean VPanner::ScrollData::setMaximum(int d, long max)
{
	return setRange(d, _min[d], max);
}

inline boolean VPanner::ScrollData::setPositions(long x, long y)
{
	boolean xchanged = setPosition(0, x);
	boolean ychanged = setPosition(1, y);
	return xchanged || ychanged;
}

inline long VPanner::ScrollData::getMinimum(int d) const
{
	return _min[d];
}

inline long VPanner::ScrollData::getMaximum(int d) const
{
	return _max[d];
}

inline long VPanner::ScrollData::getPosition(int d) const
{
	return _pos[d];
}

inline long VPanner::ScrollData::getPageSize(int d) const
{
	if (_usePageRatio[d]) {
		return long(_page[d].ratio * (_max[d] - _min[d]));
	}
	else {
		return _page[d].value;
	}
}

inline boolean VPanner::ScrollData::usingPageSizeRatio(int d) const
{
	return _usePageRatio[d];
}

inline void VPanner::ScrollData::getRange(int d, long& min, long& max) const
{
	min = _min[d];
	max = _max[d];
}

inline boolean VPanner::ScrollData::isSubtractPageFromRange() const
{
	return _subtractPageFromRange;
}

// --------------------------------------------------------------------------
//
inline void VPanner::setValues(long minx, long maxx, long posx, long pagex,
							   long miny, long maxy, long posy, long pagey)
{
	boolean a = scrollData.setValues(0, minx, maxx, posx, pagex);
	boolean b = scrollData.setValues(1, miny, maxy, posy, pagey);
	if (a || b) {
		updateThumb();
	}
}

inline void VPanner::setRange(long minx, long maxx, long miny, long maxy)
{
	boolean a = scrollData.setRange(0, minx, maxx);
	boolean b = scrollData.setRange(1, miny, maxy);
	if (a || b) {
		updateThumb();
	}
}

inline void VPanner::setMinimum(long minx, long miny)
{
	boolean a = scrollData.setMinimum(0, minx);
	boolean b = scrollData.setMinimum(1, miny);
	if (a || b) {
		updateThumb();
	}
}

inline void VPanner::setMaximum(long maxx, long maxy)
{
	boolean a = scrollData.setMaximum(0, maxx);
	boolean b = scrollData.setMaximum(1, maxy);
	if (a || b) {
		updateThumb();
	}
}

inline void VPanner::setPosition(long posx, long posy)
{
	boolean a = scrollData.setPosition(0, posx);
	boolean b = scrollData.setPosition(1, posy);
	if (a || b) {
		updateThumb();
	}
}

inline void VPanner::setPageSize(long pagex, long pagey)
{
	boolean a = scrollData.setPageSize(0, pagex);
	boolean b = scrollData.setPageSize(1, pagey);
	if (a || b) {
		updateThumb();
	}
}

inline void VPanner::setPageSizeRatio(float pagex, float pagey)
{
	boolean a = scrollData.setPageSizeRatio(0, pagex);
	boolean b = scrollData.setPageSizeRatio(1, pagey);
	if (a || b) {
		updateThumb();
	}
}

inline void VPanner::setHorizontalMinimum(long min)
{
	if (scrollData.setMinimum(0, min)) {
		updateThumb();
	}
}

inline void VPanner::setHorizontalMaximum(long max)
{
	if (scrollData.setMaximum(0, max)) {
		updateThumb();
	}
}

inline void VPanner::setHorizontalPosition(long pos)
{
	if (scrollData.setPosition(0, pos)) {
		updateThumb();
	}
}

inline void VPanner::setHorizontalPageSize(long page)
{
	if (scrollData.setPageSize(0, page)) {
		updateThumb();
	}
}

inline void VPanner::setHorizontalPageSizeRatio(float page)
{
	if (scrollData.setPageSizeRatio(0, page)) {
		updateThumb();
	}
}

inline void VPanner::setHorizontalRange(long min, long max)
{
	if (scrollData.setRange(0, min, max)) {
		updateThumb();
	}
}

inline void VPanner::setHorizontalValues(long min, long max,
										 long pos, long page)
{
	boolean a = scrollData.setRange(0, min, max);
	boolean b = scrollData.setPosition(0, pos);
	boolean c = scrollData.setPageSize(0, page);
	if (a || b || c) {
		updateThumb();
	}
}

inline void VPanner::setVerticalMinimum(long min)
{
	if (scrollData.setMinimum(1, min)) {
		updateThumb();
	}
}

inline void VPanner::setVerticalMaximum(long max)
{
	if (scrollData.setMaximum(1, max)) {
		updateThumb();
	}
}

inline void VPanner::setVerticalPosition(long pos)
{
	if (scrollData.setPosition(1, pos)) {
		updateThumb();
	}
}

inline void VPanner::setVerticalPageSize(long page)
{
	if (scrollData.setPageSize(1, page)) {
		updateThumb();
	}
}

inline void VPanner::setVerticalPageSizeRatio(float page)
{
	if (scrollData.setPageSizeRatio(1, page)) {
		updateThumb();
	}
}

inline void VPanner::setVerticalRange(long min, long max)
{
	if (scrollData.setRange(1, min, max)) {
		updateThumb();
	}
}

inline void VPanner::setVerticalValues(long min, long max,
									   long pos, long page)
{
	boolean a = scrollData.setRange(1, min, max);
	boolean b = scrollData.setPosition(1, pos);
	boolean c = scrollData.setPageSize(1, page);
	if (a || b || c) {
		updateThumb();
	}
}

inline void VPanner::setSubtractPageFromRange(boolean b)
{
	scrollData.setSubtractPageFromRange(b);
}

inline void VPanner::setTracking(boolean b)
{
	scrollTracking = b;
}

// --------------------------------------------------------------------------
//
inline void VPanner::getValues(long& minx, long& maxx,
							   long& posx, long& pagex,
							   long& miny, long& maxy,
							   long& posy, long& pagey) const
{
	scrollData.getRange(0, minx, maxx);
	posx = scrollData.getPosition(0);
	pagex = scrollData.getPageSize(0);
	scrollData.getRange(1, miny, maxy);
	posy = scrollData.getPosition(1);
	pagey = scrollData.getPageSize(1);
}

inline void VPanner::getRange(long& minx, long& maxx,
							  long& miny, long& maxy) const
{
	scrollData.getRange(0, minx, maxx);
	scrollData.getRange(1, miny, maxy);
}

inline void VPanner::getMinimum(long& minx, long& miny) const
{
	minx = scrollData.getMinimum(0);
	miny = scrollData.getMinimum(1);
}

inline void VPanner::getMaximum(long& maxx, long& maxy) const
{
	maxx = scrollData.getMaximum(0);
	maxy = scrollData.getMaximum(1);
}

inline void VPanner::getPosition(long& posx, long& posy) const
{
	posx = scrollData.getPosition(0);
	posy = scrollData.getPosition(1);
}

inline void VPanner::getPositionRatio(float& posx, float& posy) const
{
	posx = scrollData.getPositionRatio(0);
	posy = scrollData.getPositionRatio(1);
}

inline void VPanner::getPageSize(long& pagex, long& pagey) const
{
	pagex = scrollData.getPageSize(0);
	pagey = scrollData.getPageSize(1);
}

inline boolean VPanner::usingPageSizeRatio(boolean& prx, boolean& pry) const
{
	prx = scrollData.usingPageSizeRatio(0);
	pry = scrollData.usingPageSizeRatio(1);
	return prx || pry;
}

inline void VPanner::getHorizontalValues(long& min, long& max,
										 long& pos, long& page) const
{
	scrollData.getRange(0, min, max);
	pos = scrollData.getPosition(0);
	page = scrollData.getPageSize(0);
}

inline void VPanner::getHorizontalRange(long& min, long& max) const
{
	scrollData.getRange(0, min, max);
}

inline long VPanner::getHorizontalMinimum() const
{
	return scrollData.getMinimum(0);
}

inline long VPanner::getHorizontalMaximum() const
{
	return scrollData.getMaximum(0);
}

inline long VPanner::getHorizontalPosition() const
{
	return scrollData.getPosition(0);
}

inline float VPanner::getHorizontalPositionRatio() const
{
	return scrollData.getPositionRatio(0);
}

inline long VPanner::getHorizontalPageSize() const
{
	return scrollData.getPageSize(0);
}

inline boolean VPanner::usingHorizontalPageSizeRatio() const
{
	return scrollData.usingPageSizeRatio(0);
}

inline void VPanner::getVerticalValues(long& min, long& max,
									   long& pos, long& page) const
{
	scrollData.getRange(1, min, max);
	pos = scrollData.getPosition(1);
	page = scrollData.getPageSize(1);
}

inline void VPanner::getVerticalRange(long& min, long& max) const
{
	scrollData.getRange(1, min, max);
}

inline long VPanner::getVerticalMinimum() const
{
	return scrollData.getMinimum(1);
}

inline long VPanner::getVerticalMaximum() const
{
	return scrollData.getMaximum(1);
}

inline long VPanner::getVerticalPosition() const
{
	return scrollData.getPosition(1);
}

inline float VPanner::getVerticalPositionRatio() const
{
	return scrollData.getPositionRatio(1);
}

inline long VPanner::getVerticalPageSize() const
{
	return scrollData.getPageSize(1);
}

inline boolean VPanner::usingVerticalPageSizeRatio() const
{
	return scrollData.usingPageSizeRatio(1);
}

inline boolean VPanner::isSubtractPageFromRange() const
{
	return scrollData.isSubtractPageFromRange();
}

inline boolean VPanner::isTracking() const
{
	return scrollTracking;
}

inline boolean VPanner::isPanning() const
{
	return currentlyPanning;
}

inline boolean VPanner::hasThumbBorder() const
{
	return thumbBorder;
}

#endif // PANNER_H
