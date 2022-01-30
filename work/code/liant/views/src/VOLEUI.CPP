// $Header:   Q:/views/win/vcs/VOLEUI.CPv   1.5   Mar 04 1997 14:14:26   gregm  $

//	voleui.cpp
//
//  Views OLE2UI Definitions and Prototypes
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
//	Some definitions and functions did not migrate from WIN16 to
//	WIN32 so they are replicated here.
//
//	Revision History:
//	-----------------
//	05/31/95 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#include "pre.h"
#include "port.h"
#include "pen.h"

void DrawItemHandles(VComponentItem& item, DWORD dwFlags, UINT cSize, BOOL fDraw)
{
	VRectangle	rect = item.bounds();	
	VPort		port(item.getVThis());
	VPen		pen;
        
    // if erasing                   
	if (!fDraw) {
		// get this windows background color, if one is defined, else
		// use white.
		// TO DO: use the system background color.
		if (item.getVThis()->getBackground()) {
			pen.color(item.getVThis()->getBackground()->background());
		}
		else {
			pen.color(VColor::White);
		}
	}
	else {
		pen.color(VColor::Black);
	}
	
	if (item.isLinkedItem()) {
		pen.pattern(DashLine);
	}
	else {
		pen.pattern(SolidLine);
	}
	
//	port.rule(XorRule);
//	port.rule(draw ? CopyRule : XorRule);
	port.usePen(&pen);

	port.open();
	
	port.frameRegion(&rect);
	
	int	left, top, right, bottom;
	
	rect.get(Corners, &left, &top, &right, &bottom);

	VRectangle		handle;
	
	static POINT	near handlePos[8] = 
		{ 
			{left, top},											// top, left 
			{left + (right - left - cSize) / 2, top},				// top, center
			{right - cSize, top},									// top, right
			{right - cSize, top + (bottom - top - cSize) / 2},		// right, center
			{right - cSize, bottom - cSize},						// right, bottom
			{left + (right - left - cSize) / 2, bottom - cSize},	// bottom, center
			{left, bottom - cSize},									// bottom, left
			{left, top + (bottom - top - cSize) / 2}				// left, center
		};
        
//	if (fDraw) {
	{
    	for (int i = 0; i < 8; i++) {
			handle.set(CornerDim, handlePos[i].x, handlePos[i].y, cSize, cSize);
			port.fillRegion(&handle);
		}		
	}
	
	port.close();

} // DrawItemHandles
