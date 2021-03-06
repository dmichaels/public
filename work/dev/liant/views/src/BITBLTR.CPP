// $Header:   Q:/views/win/vcs/bitbltr.cpv   1.10   Mar 04 1997 17:16:38   gregm  $ 

//	bitbltr.cpp
//
//	VBitBlitr implementation
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
//  08/09/93 jmd	removed free
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	03/04/94 pat	preened.
//	03/04/94 pat	Win32 changes (portable to Win 3.x):
//					Set/GetWindow/ViewportOrg/ExtEx(), in bitbltr().
//	07/14/94 jld	replaced NULL with 0 in Set/GetWindow/ViewportOrg/ExtEx()
//	11/21/95 glm	allow bitbltr to process map modes
//	04/08/96 dgm	Added undocumented/non-portable version of bitbltr()
//					which implements StretchBlt functionality.
//	09/30/96 dgm	Fix for long-time bug painting bitmaps with wrong palette.
//	01/24/97 dgm	Moved VBitMap/VPrinter hacks into VBitMap.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "windowsx.h"
#include "bitbltr.h"
#include "bitmap.h"
#include "printer.h"
#include "notifier.h"

defineClass(VBitBltr, VObject)

// ---------------------------------------------------------------------------
//	Construct a VBitBltr object with a NIL source and destination object.
//
VBitBltr::VBitBltr()
{
	src = dest = 0;
}

// ---------------------------------------------------------------------------
//	Construct a VBitBltr with the destination and source
//	objects 'destObj' and 'srcObj' respectively.
//
VBitBltr::VBitBltr(VDisplay *srcObj, VDisplay *destObj)
{
	dest = destObj;
	src = srcObj;
	if ((destObj != 0) && (srcObj != 0)) {
		int w, h;
		src->sizeOfImage(&w, &h);	
		srcRect.set(CornerDim, 0, 0, w, h);
	}
}

// ---------------------------------------------------------------------------
//
static DWORD tROP(RasterRule rule)
{
	switch (rule) {
	case CopyRule:		return SRCCOPY;
	case OrRule:		return SRCPAINT;
	case XorRule:		return SRCINVERT;
	case NoCopyRule:	return NOTSRCCOPY;
	case NotOrRule:		return MERGEPAINT;
	case NotXorRule:	return 0x990066;
	case EraseRule:		return 0x7700E6;
	case AndRule:		return SRCAND;
	default:			return SRCCOPY;
	}
}

// ---------------------------------------------------------------------------
//	Copy the bits (bitblt) from the source to the destination object.  Copy
//	a rectangular area 'w' pixels wide and 'h' pixels high to the point 
//	('dx','dy') in the destination object from the point ('sx', 'sy') in 
//	the source object using the specified combination rule 'rop'.       
//
void VBitBltr::bitbltr(	int sx,	int sy, 
						int w, 	int h, 
						int dx, int dy, RasterRule rop)
{
	if ((dest == 0) || (src == 0)) {
		return;
	}

#if 1

	if (src->isA(VBitMapCls)) {
		((VBitMap *)src)->bitblt(dest, dx, dy, w, h, sx, sy, rop);
	}
	else {
		HDC dst_hdc = dest->getHndl();
		if (dst_hdc != 0) {
			HDC src_hdc = src->getHndl();
			if (src_hdc != 0) {
				BitBlt(dst_hdc, dx, dy, w, h, src_hdc, sx, sy, tROP(rop));   
				src->ungetHndl();
			}
			dest->ungetHndl();
		}
	}

#else

	HDC srcDC, dstDC;

	if ((dstDC = dest->getHndl()) == 0) {
		return;
	}
	if ((srcDC = src->getHndl()) == 0) {
		dest->ungetHndl();
		return;
	}
     
	if (src->isA(VBitMapCls)) {
		if (dest->isA(VPrinterCls)) {
			//
			// When bliting monochrome bitmaps to a printer one should
			// use StretchDIBits.  Bitmaps sent to a Postscript printer will
			// not print otherwise.  9/26/94 evc
			//
			HPBITMAPINFO hpDIB;
			BYTE CVP_HUGE *hpDibBits; 
			hpDIB = ((VBitMap *)src)->convertToDIB();
			hpDibBits = ((VBitMap *)src)->getAddressOfBits(hpDIB);
			StretchDIBits(dstDC, dx, dy,  w, h, sx, sy,   w, h, hpDibBits,
						  LPBITMAPINFO(hpDIB), DIB_RGB_COLORS, tROP(rop));  
			GlobalFreePtr(hpDIB);
		}
		else {
			HPALETTE hpalette = src->getPalette(), old_hpalette = 0;
			if (hpalette != 0) {
				boolean background = !notifier->paletteChangedJustReceived;
				notifier->paletteChangedJustReceived = 0;
				old_hpalette = SelectPalette(dstDC, hpalette, background);
				RealizePalette(dstDC);
			}
			BitBlt(dstDC, dx, dy, w, h, srcDC, sx, sy, tROP(rop)); 
			if (old_hpalette != 0) {
				SelectPalette(dstDC, old_hpalette, FALSE);
			}
		}
	}
	else {
		BitBlt(dstDC, dx, dy, w, h, srcDC, sx, sy, tROP(rop));   
	}
     

	src->ungetHndl();
	dest->ungetHndl();

#endif

}

// ---------------------------------------------------------------------------
//	Copy a rectangle of ('x','y') bits (bitblt) from the source to 
//	the destination object using the rule 'rop'.
//	
void VBitBltr::bitbltr(int x, int y, RasterRule rop)
{
	int sx, sy, w, h;
	srcRect.get(CornerDim, &sx, &sy, &w, &h);
	bitbltr(sx, sy, w, h, x, y, rop);
}

// ---------------------------------------------------------------------------
//
void VBitBltr::bitbltr(	int sx,	int sy, 
						int sw, int sh, 
						int dx, int dy,
						int dw, int dh, RasterRule rop)
{
	if ((dest == 0) || (src == 0)) {
		return;
	}

#if 1

	if (src->isA(VBitMapCls)) {
		((VBitMap *)src)->bitblt(dest, dx, dy, dw, dh, sx, sy, sw, sh, rop);
	}
	else {
		HDC dst_hdc = dest->getHndl();
		if (dst_hdc != 0) {
			HDC src_hdc = src->getHndl();
			if (src_hdc != 0) {
				StretchBlt(dst_hdc, dx, dy, dw, dh,
						   src_hdc, sx, sy, sw, sh, tROP(rop));   
				src->ungetHndl();
			}
			dest->ungetHndl();
		}
	}

#else

	HDC srcDC, dstDC;

	if ((dstDC = dest->getHndl()) == 0) {
		return;
	}
	if ((srcDC = src->getHndl()) == 0) {
		dest->ungetHndl();
		return;
	}

	if (src->isA(VBitMapCls) && !dest->isA(VPrinterCls)) {
		HPALETTE hpalette = src->getPalette(), old_hpalette = 0;
		if (hpalette != 0) {
			boolean background = !notifier->paletteChangedJustReceived;
			notifier->paletteChangedJustReceived = 0;
			old_hpalette = SelectPalette(dstDC, hpalette, background);
			RealizePalette(dstDC);
		}
		StretchBlt(dstDC, dx, dy, dw, dh, srcDC, sx, sy, sw, sh, tROP(rop));   
		if (old_hpalette != 0) {
			SelectPalette(dstDC, old_hpalette, FALSE);
		}
	}
	else {
		StretchBlt(dstDC, dx, dy, dw, dh, srcDC, sx, sy, sw, sh, tROP(rop));   
	}

	src->ungetHndl();
	dest->ungetHndl();

#endif

}

// ---------------------------------------------------------------------------
//	Set the dimensions of the source rectangle (see VRectangle for
//	values of enum RectOpts).
//
void VBitBltr::source(RectOpts opt, int x, int y, int w, int h)
{
	srcRect.set(opt, x, y, w, h);
}

// ---------------------------------------------------------------------------
//	Set the dimensions of the source rectangle to 'rect'.   If 'rect'
//	is NIL, then set them to the dimensions of the source VDisplay object.
//
void VBitBltr::source(VRectangle *rect)
{
	if (rect == 0) {
		int w, h;
		src->sizeOfImage(&w, &h);
		srcRect.set(CornerDim, 0, 0, w, h);
	} 
	else {
		srcRect.copyFrom(rect);
	}
}
