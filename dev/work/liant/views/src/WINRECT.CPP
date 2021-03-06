// $Header:   Q:/views/win/vcs/WINRECT.CPv   1.5   Mar 04 1997 14:14:30   gregm  $

//  winrect.cpp
//
//  VWinRect implementation
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
//
// ---------------------------------------------------------------------------

#include "pre.h"

#include "winrect.h"
#include "archiver.h"

defineClass(VWinRect, VObject)
defineClass(VOlePosition, VOlePoint)
defineClass(VOleSize, VOlePoint)

// Start of Sanjay's Code
#ifndef CV_NOOLEARCHIVER

// ---------------------------------------------------------------------------
// Store this VWinRect into the VArchiver a.
//
void VOlePosition::putTo(VArchiver& a)
{
	VOlePoint::putTo(a);
}

// ---------------------------------------------------------------------------
// Read this VWinRect from VArchiver a.
//
void VOlePosition::getFrom(VArchiver& a)
{
	VOlePoint::getFrom(a);
}

// ----------------------------------------------------------------------------
//
void VOlePosition::getFrom(VArchiver& a,
                        VObject * /* data */ , boolean /* do_realize */ )
{
    getFrom(a);
}

#endif // ARCHIVER

#ifndef CV_NOOLEARCHIVER

// ---------------------------------------------------------------------------
// Store this VWinRect into the VArchiver a.
//
void VOleSize::putTo(VArchiver& a)
{
	VOlePoint::putTo(a);
}

// ---------------------------------------------------------------------------
// Read this VWinRect from VArchiver a.
//
void VOleSize::getFrom(VArchiver& a)
{
	VOlePoint::getFrom(a);
}

// ----------------------------------------------------------------------------
//
void VOleSize::getFrom(VArchiver& a,
                        VObject * /* data */ , boolean /* do_realize */ )
{
    getFrom(a);
}

#endif // ARCHIVER
// End of Sanjay's Code

// ---------------------------------------------------------------------------
//
// Construct a VWinRect at the default VOlePosition and default
// VOleSize.
//
VWinRect::VWinRect()
{
}

// ---------------------------------------------------------------------------
//
// Construct a VWinRect from another VWinRect.
//
VWinRect::VWinRect(const VWinRect& r)
{
    position = r.position; 
    size = r.size; 
}

// ---------------------------------------------------------------------------
//
// Construct a VWinRect from four numbers.
//
VWinRect::VWinRect(int x, int y, int w, int h)
{
    position(x, y);
    size(w, h);
}

// ---------------------------------------------------------------------------
//
// Construct a VWinRect from a VOlePosition and a VOleSize.
//
VWinRect::VWinRect(const VOlePosition& pos, const VOleSize& size)
{
    this->position = pos;
    this->size = size;
}

// ---------------------------------------------------------------------------
//
// Construct a VWinRect from a VOlePosition.
//
VWinRect::VWinRect(const VOlePosition& pos)
{
    this->position = pos;
}

// ---------------------------------------------------------------------------
//
// Construct a VWinRect from a VOleSize.
//
VWinRect::VWinRect(const VOleSize& size)
{
    this->size = size;
}

// ---------------------------------------------------------------------------
//
// Construct a VWinRect from a RECT.
// Assumes RECT is in pixel units.
//
VWinRect::VWinRect(const RECT& rect, VOlePoint::PtUnits units)
{
    position.setPoint(rect.left, rect.top, units);
    size.setPoint(rect.right - rect.left, rect.bottom - rect.top, units);
}

// ---------------------------------------------------------------------------
//
// Construct a VWinRect from a LPCRECT.
// Assumes LPCRECT is in pixel units.
//
VWinRect::VWinRect(const LPCRECT lpcRect, VOlePoint::PtUnits units)
{
    position.setPoint(lpcRect->left, lpcRect->top, units);
    size.setPoint(lpcRect->right - lpcRect->left, lpcRect->bottom - lpcRect->top, units);
}

// ---------------------------------------------------------------------------
//
// Construct a VWinRect from a RECTL.
// Assumes RECTL is in Himetric units.
//
VWinRect::VWinRect(const RECTL& rect, VOlePoint::PtUnits units)
{
    // To DO: setPoint assumes int as Views lib does. RECTL has long.
    position.setPoint(int(rect.left), int(rect.top), units);
    size.setPoint(int(rect.right - rect.left), int(rect.bottom - rect.top), units);
}

// ---------------------------------------------------------------------------
//
VWinRect& VWinRect::operator+=(const VOlePosition& pos)
{
    this->position += pos; 

    return *this;
}

// ---------------------------------------------------------------------------
//
VWinRect& VWinRect::operator+=(const VOleSize& size) 
{
    this->size += size; 

    return *this;
}

// ---------------------------------------------------------------------------
//
VWinRect& VWinRect::operator+=(const VWinRect& p)
{
    position += p.position; 
    size += p.size;

    return *this;
}

// ---------------------------------------------------------------------------
//
VWinRect& VWinRect::operator-=(const VOlePosition& pos)
{
    position -= pos; 

    return *this;
}

// ---------------------------------------------------------------------------
//
VWinRect& VWinRect::operator-=(const VOleSize& size) 
{
    this->size -= size; 

    return *this;
}

// ---------------------------------------------------------------------------
//
VWinRect& VWinRect::operator-=(const VWinRect& p)
{
    position -= p.position; 
    size -= p.size;

    return *this;
}

// ---------------------------------------------------------------------------
//
VWinRect& VWinRect::operator=(const VOlePosition& pos) 
{
    this->position = pos; 
    
    return *this;
}

// ---------------------------------------------------------------------------
//
VWinRect& VWinRect::operator=(const VOleSize& size) 
{
    this->size = size; 
    
    return *this;
}

// ---------------------------------------------------------------------------
//
VWinRect::operator VRectangle&()
{
    vrect.set(CornerDim, position.x(), position.y(), size.w(), size.h());
    return vrect;
}

// ---------------------------------------------------------------------------
//
VWinRect::operator VRectangle*()
{
    vrect.set(CornerDim, position.x(), position.y(), size.w(), size.h());
    return &vrect;
}

// ---------------------------------------------------------------------------
//
VWinRect::operator VFrame&()
{
    vframe.set(position.x(), position.y(), size.w(), size.h());
    return vframe;
}

#ifndef CV_NOOLEARCHIVER

// ---------------------------------------------------------------------------
// Store this VWinRect into the VArchiver a.
//
void VWinRect::putTo(VArchiver& a)
{
	a.putObject(&position);
	a.putObject(&size);
//    position.putTo(a);
//    size.putTo(a);
}

// ---------------------------------------------------------------------------
// Read this VWinRect from VArchiver a.
//
void VWinRect::getFrom(VArchiver& a)
{
	a.getObject(&position);
	a.getObject(&size);
//    position.getFrom(a);
//    size.getFrom(a);
}

// ----------------------------------------------------------------------------
//
void VWinRect::getFrom(VArchiver& a,
                        VObject * /* data */ , boolean /* do_realize */ )
{
    getFrom(a);
}

#endif // ARCHIVER

