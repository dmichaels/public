// $Header:   Q:/views/win/vcs/winrect.h_v   1.6   11 Aug 1997 15:16:46   GREGM  $

//  winrect.h
//
//  VWinRect interface
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
//  Revision History:
//  -----------------
//  01/18/95 dss    Initial Release.
// ---------------------------------------------------------------------------

#if !defined( _VWINRECT_H_ )
#define _VWINRECT_H_

#include "object.h"
#include "opoint.h"
#include "rect.h"

CLASS VOlePosition : public VOlePoint
{
public:
                VOlePosition()                      : VOlePoint( 0, 0, Pixels) {;}
                VOlePosition(const VOlePosition& p) : VOlePoint(p) {;}
				VOlePosition(int x, int y, const PtUnits units): VOlePoint(x, y, units) {;}

                VOlePosition(const POINTL& p, const PtUnits u) : VOlePoint(p, u) {;}
                VOlePosition(const SIZEL& s, const PtUnits u)   : VOlePoint(s, u) {;}
                VOlePosition(const POINT& p, const PtUnits u)   : VOlePoint(p, u) {;}

#if defined(CV_WIN16)
                VOlePosition(const SIZE& s, const PtUnits u)    : VOlePoint(s, u) {;}
#endif

    VClass      *iam();

			int		left() const {return x();}
			int		top() const {return y();}

			void	left(const int l) {x(l);}
			void	top(const int t) {y(t);}

// Start of Sanjay's Code
#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif
// End of Sanjay's Code
};

CLASS VOleSize : public VOlePoint
{
public:
                VOleSize()                      : VOlePoint( 1, 1, Pixels) {;}
                VOleSize(const VOleSize& p)     : VOlePoint(p) {;}
                VOleSize(const int x, const int y, 
                         const PtUnits units): VOlePoint(x, y, units) {;}

                VOleSize(const POINTL& p, const PtUnits u)  : VOlePoint(p, u) {;}
                VOleSize(const SIZEL& s, const PtUnits u)   : VOlePoint(s, u) {;}
                VOleSize(const POINT& p, const PtUnits u)   : VOlePoint(p, u) {;}

#if defined(CV_WIN16)
                VOleSize(const SIZE& s, const PtUnits u)    : VOlePoint(s, u) {;}
#endif

    VClass         *iam();

    int     w() const {return x();}
    int     h() const {return y();}

    void    w(const int w) {x(w);}
    void    h(const int h) {y(h);}

// Start of Sanjay's Code
#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif
// End of Sanjay's Code
};

// ---------------------------------------------------------------------------
// A VLocation is an (x,y) pair of short integers used to represent
// a point or a dimension in the presentation space of VDisplay objects.
//
CLASS VWinRect : public VObject {
public:
    // Constructors
                    VWinRect();
                    VWinRect(const VWinRect& r);
					VWinRect(int x, int y,
                             int w, int h);
                    VWinRect(const VOlePosition& pos);
                    VWinRect(const VOleSize& size);
                    VWinRect(const VOlePosition& pos, const VOleSize& size);

                    VWinRect(const RECTL& rectl, VOlePoint::PtUnits units);

                    VWinRect(const RECT& rect, VOlePoint::PtUnits units);
                    VWinRect(const LPCRECT lpcRect, VOlePoint::PtUnits units);

    VClass         *iam();

    // Operations on VWinRect
    VWinRect&       operator+=(const VOlePosition& pos);
    VWinRect&       operator+=(const VOleSize& size);
    VWinRect&       operator+=(const VWinRect& p);

    VWinRect&       operator-=(const VOlePosition& pos);
    VWinRect&       operator-=(const VOleSize& size);
    VWinRect&       operator-=(const VWinRect& p);

    VWinRect&       operator=(const VOlePosition& pos);
    VWinRect&       operator=(const VOleSize& size);

    // Querying and Setting Values
    void            setBounds(const int x, const int y, 
                              const int w, const int h, const VOlePoint::PtUnits units);
    void            setBounds(const RECTL& rect, const VOlePoint::PtUnits units);
    void            setBounds(const RECT& rect, const VOlePoint::PtUnits units);

    void            getBounds(int& x, int& y, int &w, int &h, 
                              const VOlePoint::PtUnits units) const;
    RECTL&          getBounds(RECTL& rect, const VOlePoint::PtUnits units) const;
    RECT&           getBounds(RECT& rect, const VOlePoint::PtUnits units) const;

    VOlePosition&   getPosition() {return position;}
    VOleSize&       getSize() {return size;}

    // Conversions
#if 0
                    operator RECTL&();
#if defined(CV_WIN16)
                    operator RECT&();
                    operator LPCRECT();
                    operator LPRECT();
#endif
#endif
					operator VRectangle&();
                    operator VRectangle*();
                    operator VFrame&();

    boolean         pointIn(VOlePoint& p)
                    {
                        return VRectangle(*this).pointIn(p.x(), p.y());
                    }

#ifndef CV_NOOLEARCHIVER
    void            putTo(VArchiver& a);
    void            getFrom(VArchiver& a);
    virtual void    getFrom(VArchiver& a, VObject *data,
                            boolean do_realize = TRUE);
#endif

protected:
    VOlePosition    position;
    VOleSize        size;

#if 0
    RECT            rect;
    RECTL           rectl;
#endif
	VRectangle      vrect;
    VFrame          vframe;
};

extern VClass *VWinRectCls;

// ---------------------------------------------------------------------------
//
// Return the bounds as position (x, y) and size (width and height).
//
inline void VWinRect::getBounds(int& x, int& y, int& w, int& h, const VOlePoint::PtUnits units) const
{
    ((VWinRect*)this)->getPosition().getPoint(x, y, units);
    ((VWinRect*)this)->getSize().getPoint(w, h, units);
}

// ---------------------------------------------------------------------------
//
// Return the bounds as a LPRECTL
//
inline RECTL& VWinRect::getBounds(RECTL& rect, const VOlePoint::PtUnits units) const
{
    int     x, y, w, h;
    
	((VWinRect*)this)->getPosition().getPoint(x, y, units);
    ((VWinRect*)this)->getSize().getPoint(w, h, units);
    
    rect.left = x;
    rect.top = y;
    rect.right = rect.left + w;
    rect.bottom = rect.top + h;

    return rect;
}

// ---------------------------------------------------------------------------
//
// Return the bounds as a LPRECT
//
inline RECT& VWinRect::getBounds(RECT& rect, const VOlePoint::PtUnits units) const
{
    int     x, y, w, h;

    ((VWinRect*)this)->getPosition().getPoint(x, y, units);
    ((VWinRect*)this)->getSize().getPoint(w, h, units);
    
    rect.left = x;
    rect.top = y;
	rect.right = rect.left + w;
    rect.bottom = rect.top + h;

    return rect;
}    

// ---------------------------------------------------------------------------
//
// Set the bounds from a position (x, y) and size (width and height).
//
inline void VWinRect::setBounds(const int x, const int y, 
                                const int w, const int h, 
                                const VOlePoint::PtUnits units)
{
    position.setPoint(x, y, units);
    size.setPoint(w, h, units);
}

// ---------------------------------------------------------------------------
//
// Set the bounds from a LPRECTL
//
inline void VWinRect::setBounds(const RECTL& rect, const VOlePoint::PtUnits units)
{
	// To DO: setPoint assumes int as Views lib does. RECTL has long.
    position.setPoint(int(rect.left), int(rect.top), units);
    size.setPoint(int(rect.right - rect.left), int(rect.bottom - rect.top), units);
}

// ---------------------------------------------------------------------------
//
// Set the bounds from a LPRECT
//
inline void VWinRect::setBounds(const RECT& rect, const VOlePoint::PtUnits units)
{
    position.setPoint(rect.left, rect.top, units);
    size.setPoint(rect.right - rect.left, rect.bottom - rect.top, units);
}    

#endif // _VWINRECT_H_
