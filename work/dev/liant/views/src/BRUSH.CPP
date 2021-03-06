// $Header:   Q:/views/win/vcs/brush.cpv   1.10   Mar 04 1997 17:16:42   gregm  $ 

//	brush.cpp
//
//	VBrush implementation
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
//	04/07/93 pat	changed id to VObject *
//	07/20/93 jmd	preened, removed free(), changed rgbColor to VColor
// 	10/09/93 jmd	made pattern not inline
//	10/10/93 jmd	SC6 changes (typdef'd enums).
// 	02/18/94 pat	changed LightGray, etc. to VColor::LightGray, etc.
//	03/04/94 pat	STRICT:
//					cast GetStockObject() return appropriately;
//					changed HANDLE to HBRUSH.
//	03/24/94 pat	zeroed handle after deleting gui object in destructor
//	07/14/94 jld	replaced NULL with 0
//	12/05/95 dgm	Added VColorScheme::checkSystemBrushDeletion() call to ~.
//	01/09/96 dgm	Added assertion for shared system brush deletion.
//	02/05/96 pkt	Changed to handle grey patterns as blends.
//	02/06/96 pkt	Fixed 'make' to check special case colors correctly.
//	09/11/96 dgm	Added StipplePat.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "brush.h"
#include "iterator.h"
#include "clrscm.h"
#include "cvassert.h"

defineClass(VBrush, VObject)

// ---------------------------------------------------------------------------
// Construct a VBrush with foreground color 'fore', background color
// 'back', and fill pattern 'ptrn'.
//
VBrush::VBrush(FillPattern ptrn, VColor fore, VColor back)
{
	hbrush = 0;
	fgColor = fore;
	stock = FALSE;
	bkColor = back;
	fillPat = ptrn;
	ports = 0;
	make();
}

// ---------------------------------------------------------------------------
// Construct a VBrush with default attributes (see reset()).
//
VBrush::VBrush()
{
	hbrush = 0;
	stock = FALSE;
	ports = 0;
	reset();
}

// ---------------------------------------------------------------------------
// Construct a solid VBrush with color 'color'.
//
VBrush::VBrush(VColor color)
{
	hbrush = 0;
	fgColor = bkColor = color;
	stock = FALSE;
	fillPat = BackgroundPat;
	ports = 0;
	make();
}

// ---------------------------------------------------------------------------
// Destruct this VBrush and decouple it from any VPort objects.
//
VBrush::~VBrush()
{
	if (hbrush && !stock) {
		DeleteObject((HGDIOBJ) hbrush);
		hbrush = 0;
	}

	if (ports != 0) {
		if (ports->count()) {
			VObject *p;
			VIterator i(ports);
			while (p = i++) {
				p->perform(pSet, (long)0);
			}
		}
		delete ports;
	}

	boolean deleting_non_shared_resource = !VColorScheme::
										    checkSystemBrushDeletion(this);
	cv_assert(deleting_non_shared_resource);
}

// ---------------------------------------------------------------------------
// Make a new MS Windows brush
//
boolean VBrush::make()
{
	HBRUSH  h;
	boolean oldStock = stock;
	VObject *p;

	stock = FALSE;
	VColor bk = background();
	VColor fg = foreground();

	switch (pattern()) {
	case BackgroundPat: 
		if (bk == VColor(WHITE)) {
			h = (HBRUSH) GetStockObject(WHITE_BRUSH);
			stock = TRUE;
		}
		else if (bk == VColor(BLACK)) {
			h = (HBRUSH) GetStockObject(BLACK_BRUSH);
			stock = TRUE;
		}
		else {
			h = CreateSolidBrush(bk);
		}
		break;

	default: 
		pattern(BackgroundPat);
		// fall through
	case ForegroundPat: 
		if (fg == VColor(WHITE)) {
			h = (HBRUSH) GetStockObject(WHITE_BRUSH);
			stock = TRUE;
		}
		else if (fg == VColor(BLACK)) {
			h = (HBRUSH) GetStockObject(BLACK_BRUSH);
			stock = TRUE;
		} 
		else {
			h = CreateSolidBrush(fg);
		}
		break;

	case DarkGreyPat:
		if (fg == VColor(BLACK) && bk == VColor(WHITE)) { 
			h = (HBRUSH) GetStockObject(DKGRAY_BRUSH);
			stock = TRUE;
		}
		else {
			VColor blend((fg.getRed() * 3 + bk.getRed() + 2) / 4,
						 (fg.getGreen() * 3 + bk.getGreen() + 2) / 4,
						 (fg.getBlue() * 3 + bk.getBlue() + 2) / 4);
			h = CreateSolidBrush(blend);
		}
		break;

	case LightGreyPat: 
		if (fg == VColor(BLACK) && bk == VColor(WHITE)) { 
			h = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
			stock = TRUE;
		}
		else {
			VColor blend((fg.getRed() + bk.getRed() * 3 + 2) / 4,
						 (fg.getGreen() + bk.getGreen() * 3 + 2) / 4,
						 (fg.getBlue() + bk.getBlue() * 3 + 2) / 4);
			h = CreateSolidBrush(blend);
		}
		break;

	case GreyPat: 
		if (fg == VColor(BLACK) && bk == VColor(WHITE)) { 
			h = (HBRUSH) GetStockObject(GRAY_BRUSH);
			stock = TRUE;
		}
		else {
			VColor blend((fg.getRed() + bk.getRed() + 1) / 2,
						 (fg.getGreen() + bk.getGreen() + 1) / 2,
						 (fg.getBlue() + bk.getBlue() + 1) / 2);
			h = CreateSolidBrush(blend);
		}
		break;

	case HorizontalPat: 
		h = CreateHatchBrush(HS_HORIZONTAL, fg);
		break;

	case VerticalPat: 	
		h = CreateHatchBrush(HS_VERTICAL, fg);
		break;

	case ForeDiagPat: 
		h = CreateHatchBrush(HS_FDIAGONAL, fg);
		break;

	case BackDiagPat: 
		h = CreateHatchBrush(HS_BDIAGONAL, fg);
		break;

	case CrossHatchPat: 
		h = CreateHatchBrush(HS_CROSS, fg);
		break;

	case DiagHatchPat: 
		h = CreateHatchBrush(HS_DIAGCROSS, fg);
		break;

	case StipplePat:
	{
		static unsigned char stipple_bitmap_bits[] = { 0xAA, 0,
													   0x55, 0,
													   0xAA, 0,
													   0x55, 0,
													   0xAA, 0,
													   0x55, 0,
													   0xAA, 0,
													   0x55, 0 };
		HBITMAP hbitmap = CreateBitmap(8, 8, 1, 1, stipple_bitmap_bits);
		h = CreatePatternBrush(hbitmap);
		DeleteObject(hbitmap);
		break;
	}

	}

	if (h == (HANDLE) 0) {
		print("Invalid brush handle!");
		return FALSE;
	}
	else {
		if (ports == 0 || !ports->count()) {
			if (hbrush && !oldStock) {
				DeleteObject((HGDIOBJ) hbrush);
			}
			hbrush = h;
		}
		else {
			VIterator i(ports);
			while (p = i++) {
				p->perform(pReset);
			}
			if (hbrush && !oldStock)  {
				DeleteObject((HGDIOBJ) hbrush);
			}
			hbrush = h;
			i.start();
			while (p = i++) {
				p->perform(pSet, (long)this);
			}
		}
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// Reset the attributes to default values: foreground and background 
// to MS Windows or OS/2 PM default window background color, and pattern to
// ForegroundPat.
//
void VBrush::reset()
{
	fgColor = bkColor = GetSysColor(COLOR_WINDOW);
	fillPat = ForegroundPat;
	make();
}

// ---------------------------------------------------------------------------
// Set the foreground color to 'clr' for this VBrush .
//
void VBrush::foreground(const VColor& clr)
{ 
	if (fgColor != clr) {
		fgColor = clr;
		make();
	}
}

// ---------------------------------------------------------------------------
// Set the background color to 'clr' for this VBrush .
//
void VBrush::background(const VColor& clr)
{ 
	if (bkColor != clr) {
		bkColor = clr;
		make();
	}
}

// ---------------------------------------------------------------------------
// Set the fill pattern for this VBrush to 'ptrn'.
//
void VBrush::pattern(FillPattern ptrn)
{
	if (fillPat != ptrn) {
		fillPat = ptrn;
		make();
	}
}

// ---------------------------------------------------------------------------
// Return the fill pattern of this VBrush.
//
FillPattern VBrush::pattern()
{
	return fillPat;
}

// ---------------------------------------------------------------------------
// VBrush objects can be bound to more than one VPort object at a time. This 
// mechanism is designed to support the techniques used by MS Windows to 
// create its brush objects.  If an attribute of a brush changes, e.g., the 
// foreground color, then in MS Windows the existing brush object must be 
// destroyed and a new one created. Hence, because of this binding the VBrush 
// objects can then notify the VPort objects they are bound too when the
// MS Windows brush handle has changed.
//
void VBrush::bind(VObject *p, boolean b)
{
	if (ports == 0) {
		ports = new VOrdCollect();
	}
	if (b) {
		if (!(ports->findSame(p))) {
			ports->add(p);
		}
	}
	else {
		ports->removeSame(p);
	}
}

// ---------------------------------------------------------------------------
