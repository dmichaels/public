// $Header:   Q:/views/common/vcs/shade.cpv   1.7   Mar 24 1997 16:14:40   scottt  $ 

//	shade.cpp
//
//	VShade implementation [Common]
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
//	10/31/92 jmd	modified panel/pressed, preened
//	04/12/93 jmd	preened, added fill flag to VShade::panel()
//	06/26/93 jmd	preened, added RaisedButton
//	07/20/93 jmd	changed rgbColor to VColor
// 	02/18/94 pat	changed LightGray to VColor::LightGray
//	02/24/94 pkt	fixed VShade::line() to appear centered on its position.
//	03/08/94 pat	STRICT:  included display.h to prevent redeclaration
//					errors with	windows.h.
//	03/29/94 dgm	added Motif specific (for now) setColors which takes
//					a background color and computes the appropriate
//					hightlight and shadow colors.
//	05/20/94 dgm	Added setDefaultColors().
//	06/02/94 dgm	Simplified ?: expressions in function calls to
//					work-around SPARCworks C++ compiler (4.0) bug.
//	05/17/95 pkt	preened and merged.
//	12/05/95 dgm	Moved setColors() to shade.cpp.
//	09/12/96 dgm	Default to system colors; support setColors().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "display.h"
#include "shade.h"
#include "port.h"
#include "rect.h"
#include "clrscm.h"

// ---------------------------------------------------------------------------
// Construct a VShade object with defualt bevel thickness of two
// pixels, a default surface color of LightGray, a default hilite color of
// WHITE, and a default shadow color of Gray.
//
VShade::VShade() :
	sHilite(VColorScheme::systemColor(VColorScheme::ShadedHighlight)),
	sShadow(VColorScheme::systemColor(VColorScheme::ShadedShadow)),
	sSurface(VColorScheme::systemColor(VColorScheme::ShadedBackground)),
	bevelThickness(2) 
{
}

// ---------------------------------------------------------------------------
//
VShade::~VShade() 
{
}

// ---------------------------------------------------------------------------
// Set the hilite, shadow and surface colors of this VShade object to
// 'hilite', 'shadow' and 'surface', respectively.
//
void VShade::setColors(VColor hilite, VColor shadow, VColor surface)
{
	sHilite = hilite;
	sShadow = shadow;
	sSurface = surface;
}

// ---------------------------------------------------------------------------
// Draw a horizontal shaded line with lineType 'type' from ('i1','i2')
// to ('i3','i2') if the line is of 'type' HBump or HDip, or a vertical
// line from ('i1','i2') to ('i1','i3') if the line is of 'type' VBump 
// or VDip, using VPort object 'port'.  Upon entry into this method,
// the VPort object 'port' is assumed to be opened on some VDisplay 
// object, and this VPort object will be left opened upon exit from 
// this method.
//
void VShade::line(enum lineType type, int i1, int i2, int i3, VPort &port) 
{
	VPen pn, *oldpn;
	int bt, x0, y0, x1, y1;
	VColor c1, c2;
	
	if (type == HBump || type == VBump) {
		c1 = sHilite, c2 = sShadow;
	}
	else {
		c1 = sShadow, c2 = sHilite;
	}
	oldpn = port.currentPen();
	if (type == HBump || type == HDip) {
		pn.color(c1);
		port.usePen(&pn);
		bt = bevelThickness;
		x0 = i1, x1 = i3;
		for (y0 = i2 - 1; bt > 0; y0-- , bt--) {
			port.moveTo(x0, y0);
			port.lineTo(x1, y0);
		}
		bt = bevelThickness;
		pn.color(c2);
		for (y0 = i2; bt > 0; y0++ , bt--) {
			port.moveTo(x0, y0);
			port.lineTo(x1, y0);
		}
	}
	else {
		pn.color(c1);
		port.usePen(&pn);
		bt = bevelThickness;
		y0 = i2, y1 = i3;
		for (x0 = i1 - 1; bt > 0; x0-- , bt--) {
			port.moveTo(x0, y0);
			port.lineTo(x0, y1);
		}
		bt = bevelThickness;
		pn.color(c2);
		for (x0 = i1; bt > 0; x0++ , bt--) {
			port.moveTo(x0, y0);
			port.lineTo(x0, y1);
		}
	}

	port.usePen(oldpn);
}

// ---------------------------------------------------------------------------
// Draw a shaded panel with panelType 'type' in VRectangle 'wR'
// framed with a border of thickness 'border' using VPort object 'port'. The
// 'type' parameter may be one of Raised, Indented, Flat or Pressed.
// Upon entry into this method, the VPort object 'port' is assumed to be
// opened on some VDisplay object. The VPort object will be left opened 
// upon exit from this method.
//
void VShade::panel(panelType type, VRectangle& rect, short border, VPort& port, 
					boolean fill)
{
	int		xp, yp, hy, wx, x, y, w, h; 
	VBrush	bkBrush(sSurface), *oldbrsh;
	VPen	pn, *oldpn;

	oldbrsh = port.currentBrush();
	oldpn = port.currentPen();
	port.useBrush(&bkBrush);


	if (type == RaisedButton || type == Pressed) {
		if (fill) {
			VRectangle t = rect;
			t.inset(1, 1);
			port.fillRegion(&t, FALSE);
		}

		rect.get(Corners, &x, &y, &w, &h);

		if (border >= 1) {
			// draw a "rounded" border
			port.moveTo(x + 1, y);
			port.lineTo(w - 1, y);

			port.moveTo(w - 1, y + 1);
			port.lineTo(w - 1, h - 1);

			port.moveTo(x + 1, h - 1);
			port.lineTo(w - 1, h - 1);

			port.moveTo(x, y + 1);
			port.lineTo(x, h - 1);
		}
	}
	else {
		if (fill) {
			if (border) {
				port.fillRegion(&rect, TRUE);
			}
			else {
				port.fillRegion(&rect, FALSE);
			}
		}
		rect.get(Corners, &x, &y, &w, &h);

		if (border >= 2) {
			xp = x + 1;
			yp = y + 1;
			wx = w - 2;
			hy = h - 2;
			port.moveTo(xp, yp);
			port.lineTo(wx, yp);
			port.lineTo(wx, hy);
			port.lineTo(xp, hy);
			port.lineTo(xp, yp);
		}
	}

	x += border;
	y += border;
	w -= (border + 1);
	h -= (border + 1);
	int bt = bevelThickness + x;
	port.usePen(&pn);

	// Draw top & left edges.
	boolean draw = TRUE;
	if (type == Raised || type == RaisedButton) {
		pn.color(sHilite);
	}
	else {
		if (type == Indented || type == Pressed) {
			pn.color(sShadow);
		}
		else {
			// don't draw top & left edges
			draw = FALSE;
		}
	}
	if (draw) {
		for (xp = x, yp = h, hy = y, wx = w; xp < bt; xp++, yp--, hy++, wx--) {
			port.moveTo(xp, yp);
			port.lineTo(xp, hy);
			port.lineTo(wx, hy);
		}
	}

	// Draw bottom and right edges.
	draw = TRUE;
	if (type == Raised || type == RaisedButton || type == Pressed) {
		pn.color(sShadow);
	}
	else {
		if (type == Indented) {
			pn.color(sHilite);
		}
		else {
			// don't draw bottom & right
			draw = FALSE;
		}
	}
	if (draw) {
		int thick = (type == RaisedButton) ? bt + 1 : bt;

		for (xp = x, yp = h, hy = y, wx = w; xp < thick; xp++, yp--, hy++, wx--) {
			port.moveTo(xp, yp);
			port.lineTo(wx, yp);
			port.lineTo(wx, hy - 1);	//!!!jmd
		}
	}

	port.useBrush(oldbrsh);
	port.usePen(oldpn);
}

// ---------------------------------------------------------------------------
//

#if defined(MOTIF)

#	include "notifier.h"

	extern "C" {
#		include "xsetdefs.h"
#		include <Xm/Xm.h>
#		include "xclrdefs.h"
	}

// ---------------------------------------------------------------------------
//
void VShade::setColors(const VColor& surface, VColor *foreground)
{
	Pixel	background_color = Pixel(long(surface));
	Pixel	highlight_color, shadow_color;
	Pixel	foreground_color, select_color;

	XmGetColors(notifier->defaultScreen(), notifier->defaultColorMap(),
				 background_color,
				&foreground_color,
				&highlight_color,
				&shadow_color,
				&select_color);

	sHilite = (unsigned long)highlight_color;
	sShadow = (unsigned long)shadow_color;
	sSurface = surface;

	if (foreground != 0) {
		*foreground = (unsigned long)foreground_color;
	}
}

#else

// ---------------------------------------------------------------------------
//
void VShade::setColors(const VColor& surface, VColor *foreground)
{
	VColorScheme colorscheme;
	colorscheme.setShadedBackground(surface);
	colorscheme.getColor(VColorScheme::ShadedHighlight, sHilite);
	colorscheme.getColor(VColorScheme::ShadedShadow, sShadow);
	colorscheme.getColor(VColorScheme::ShadedBackground, sSurface);
	if (foreground != 0) {
		colorscheme.getColor(VColorScheme::ShadedForeground, *foreground);
	}
}

#endif

// ---------------------------------------------------------------------------
//
void VShade::setDefaultColors()
{
	sHilite = VColor(VColorScheme::
					 systemColor(VColorScheme::ShadedHighlight));
	sShadow = VColor(VColorScheme::
					 systemColor(VColorScheme::ShadedShadow));
	sSurface = VColor(VColorScheme::
					  systemColor(VColorScheme::ShadedBackground));
}
