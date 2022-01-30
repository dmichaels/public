//	image.cpp
//
//	VImage implementation [Common]
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
//	11/26/96 dgm	16-bit fix.
// --------------------------------------------------------------------------

#include "image.h"
#include "rect.h"
#include "location.h"
#include "clrscm.h"
#include "color.h"
#include "brush.h"
#include "pen.h"
#include "bitmap.h"
#include "port.h"
#include "bitbltr.h"
#include "str.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "rscarch.h"
#endif

// --------------------------------------------------------------------------
// OSF/Motif specific includes.
//
#if defined(CV_MOTIF)
extern "C" {
#include "xsetdefs.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "xclrdefs.h"
}
#endif

// --------------------------------------------------------------------------
// Class identification.
//
defineClass(VImage, VObject)
defineArchiveRevision(VImage, 0)

// --------------------------------------------------------------------------
// Local static data.
//
unsigned long	objectCount							= 0;
VColor		   *defaultTransparentColor				= 0;
VColor		   *defaultShadedBackgroundColor		= 0;
VColor		   *defaultShadedHighlightColor			= 0;
VColor		   *defaultShadedShadowColor			= 0;
VColor		   *defaultShadedForegroundColor		= 0;
VColor		   *defaultFlatBackgroundColor			= 0;
VColor		   *defaultFlatBackgroundSelectColor	= 0;

// --------------------------------------------------------------------------
//
VImage::VImage()
	: VObject()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VImage::VImage(const VString& native_resource_or_bitmap_file_name,
			   int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(native_resource_or_bitmap_file_name,
			 major_count, minor_count, FALSE);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const char *native_resource_or_bitmap_file_name,
			   int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(native_resource_or_bitmap_file_name,
			 major_count, minor_count, FALSE);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const VPathString& bitmap_file_name,
			   int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(bitmap_file_name, major_count, minor_count, FALSE);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const VString& vrf_bitmap_name,
			   const VPathString& vrf_file_name,
			   int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(vrf_bitmap_name, vrf_file_name, major_count, minor_count, FALSE);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const VString& vrf_bitmap_name,
			   const VString& vrf_file_name,
			   int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(vrf_bitmap_name, vrf_file_name, major_count, minor_count, FALSE);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const VString& vrf_bitmap_name,
			   const char *vrf_file_name,
			   int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(vrf_bitmap_name, vrf_file_name, major_count, minor_count, FALSE);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const char *vrf_bitmap_name,
			   const VPathString& vrf_file_name,
			   int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(vrf_bitmap_name, vrf_file_name, major_count, minor_count, FALSE);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const char *vrf_bitmap_name,
			   const VString& vrf_file_name,
			   int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(vrf_bitmap_name, vrf_file_name, major_count, minor_count, FALSE);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const char *vrf_bitmap_name,
			   const char *vrf_file_name,
			   int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(vrf_bitmap_name, vrf_file_name, major_count, minor_count, FALSE);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const VBitMap& bitmap, int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(bitmap, major_count, minor_count);
}

// --------------------------------------------------------------------------
//
VImage::VImage(const VBitMap *bitmap, int major_count, int minor_count)
	: VObject()
{
	initialize();
	setImage(bitmap, major_count, minor_count);
}

// --------------------------------------------------------------------------
// Copy constructor.
//
VImage::VImage(const VImage& image)
{
	initialize();
	copy(image);
}

// --------------------------------------------------------------------------
// Assignment operator.
//
VImage& VImage::operator=(const VImage& image)
{
	destruct();
	copy(image);
	return *this;
}

// --------------------------------------------------------------------------
//
VImage::~VImage()
{
	destruct();
	if (--objectCount == 0) {
		destructStatic();
	}
}

// --------------------------------------------------------------------------
//
void VImage::initialize()
{
	if (objectCount++ == 0) {
		constructStatic();
	}

	originalBitmap							= 0;
	mappedBitmap							= 0;
	mappedOrBitmap							= 0;
	unmappedOrBitmap						= 0;
	andMaskBitmap							= 0;
	disabledAndMaskBitmap					= 0;

	originalBitmapName						= 0;
	originalBitmapVrfName					= 0;

	transparentColor						= 0;
	additionalDisabledMaskColor				= 0;
	upperLeftPixelColor						= 0;

	imageMajorCount							= 0;
	imageMinorCount							= 0;
	imageMajorSize							= 0;
	imageMinorSize							= 0;

	drawMode								= Default;

	amMappingShadedBackgroundColor			= 1;
	amMappingShadedShadowColor				= 1;
	amMappingShadedHighlightColor			= 1;
	amMappingShadedForegroundColor			= 0;
	amMappingFlatBackgroundColor			= 0;
	amMappingFlatBackgroundSelectColor		= 0;
	amReadingTransparentColorFromUpperLeft	= 1;

	transparentColorIsMine					= 0;
	originalBitmapIsMine					= 0;
	originalBitmapFromVrf					= 0;
	originalBitmapFromBitmapFile			= 0;
	originalBitmapFromNativeResource		= 0;
}

// --------------------------------------------------------------------------
//
void VImage::copy(const VImage& image)
{
	imageMajorCount	= image.getItemMajorCount();
	imageMinorCount	= image.getItemMinorCount();

	if (image.originalBitmapFromVrf) {
		originalBitmapFromVrf = image.originalBitmapFromVrf;
		if (image.originalBitmapName != 0) {
			originalBitmapName = new VString(*image.originalBitmapName);
		}
		if (image.originalBitmapVrfName != 0) {
			originalBitmapVrfName =
				new VString(*image.originalBitmapVrfName);
		}
	}
	else if (image.originalBitmapFromBitmapFile) {
		originalBitmapFromBitmapFile = image.originalBitmapFromBitmapFile;
		if (image.originalBitmapName != 0) {
			originalBitmapName = new VString(*image.originalBitmapName);
		}
	}
	else if (image.originalBitmapFromNativeResource) {
		originalBitmapFromNativeResource =
			image.originalBitmapFromNativeResource;
		if (image.originalBitmapName != 0) {
			originalBitmapName = new VString(*image.originalBitmapName);
		}
	}
	else {
		//
		// Make an actual copy of the bitmap; don't copy the pointer.
		// Is this the right thing to do?  I think so.
		//
		if ((originalBitmap = image.copyOriginalBitmap()) != 0) {
			originalBitmapIsMine = 1;
			imageMajorSize = image.getItemWidth();
			imageMinorSize = image.getItemHeight();
		}
	}

	if (image.amReadingTransparentColorFromUpperLeft) {
		amReadingTransparentColorFromUpperLeft = 1;
	}
	else {
		amReadingTransparentColorFromUpperLeft = 0;
		if (image.transparentColor != 0) {
			transparentColor = new VColor(*image.transparentColor);
			transparentColorIsMine = 1;
		}
	}

	if (image.additionalDisabledMaskColor != 0) {
		additionalDisabledMaskColor =
			new VColor(*image.additionalDisabledMaskColor);
	}

	amMappingShadedBackgroundColor =
		image.amMappingShadedBackgroundColor;
	amMappingShadedHighlightColor =
		image.amMappingShadedHighlightColor;
	amMappingShadedShadowColor =
		image.amMappingShadedShadowColor;
	amMappingShadedForegroundColor =
		image.amMappingShadedForegroundColor;
	amMappingFlatBackgroundColor =
		image.amMappingFlatBackgroundColor;
	amMappingFlatBackgroundSelectColor =
		image.amMappingFlatBackgroundSelectColor;
}

// --------------------------------------------------------------------------
//
void VImage::constructStatic()
{
}

// --------------------------------------------------------------------------
//
void VImage::destruct()
{
	destructColors();
	destructBitmaps();
}

// --------------------------------------------------------------------------
//
void VImage::destructColors()
{
	if (transparentColorIsMine) {
		transparentColorIsMine = 0;
		if (transparentColor != 0) {
			delete transparentColor;
			transparentColor = 0;
		}
	}
	if (additionalDisabledMaskColor != 0) {
		delete additionalDisabledMaskColor;
		additionalDisabledMaskColor = 0;
	}
	if (upperLeftPixelColor != 0) {
		delete upperLeftPixelColor;
		upperLeftPixelColor = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::destructBitmaps()
{
	if (originalBitmap != 0) {
		if (originalBitmapIsMine) {
			originalBitmapIsMine = 0;
			delete originalBitmap;
		}
		originalBitmap = 0;
	}
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (unmappedOrBitmap != 0) {
		delete unmappedOrBitmap; unmappedOrBitmap = 0;
	}
	if (andMaskBitmap != 0) {
		delete andMaskBitmap; andMaskBitmap = 0;
	}
	if (mappedBitmap != 0) {
		delete mappedBitmap; mappedBitmap = 0;
	}
	if (disabledAndMaskBitmap != 0) {
		delete disabledAndMaskBitmap; disabledAndMaskBitmap = 0;
	}

	if (originalBitmapName != 0) {
		delete originalBitmapName;
		originalBitmapName = 0;
	}
	if (originalBitmapVrfName != 0) {
		delete originalBitmapVrfName;
		originalBitmapVrfName = 0;
	}

	originalBitmapIsMine				= 0;
	originalBitmapFromVrf				= 0;
	originalBitmapFromBitmapFile		= 0;
	originalBitmapFromNativeResource	= 0;
}

// --------------------------------------------------------------------------
//
void VImage::destructStatic()
{
	delete defaultTransparentColor;
	delete defaultShadedBackgroundColor;
	delete defaultShadedHighlightColor;
	delete defaultShadedShadowColor;
	delete defaultShadedForegroundColor;
	delete defaultFlatBackgroundColor;
	delete defaultFlatBackgroundSelectColor;

	defaultTransparentColor				= 0;
	defaultShadedBackgroundColor		= 0;
	defaultShadedHighlightColor			= 0;
	defaultShadedShadowColor			= 0;
	defaultShadedForegroundColor		= 0;
	defaultFlatBackgroundColor			= 0;
	defaultFlatBackgroundSelectColor	= 0;
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const VString& native_resource_or_bitmap_file_name,
						 int major_count, int minor_count,
						 boolean load_bitmap_now)
{
	destructBitmaps();
	originalBitmapName = new VString(native_resource_or_bitmap_file_name);
	originalBitmapFromNativeResource = 1;
	setItemCount(major_count, minor_count);
	return load_bitmap_now ? retrieveOriginalBitmap() : TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const char *native_resource_or_bitmap_file_name,
						 int major_count, int minor_count,
						 boolean load_bitmap_now)
{
	destructBitmaps();
	originalBitmapName = new VString(native_resource_or_bitmap_file_name);
	originalBitmapFromNativeResource = 1;
	setItemCount(major_count, minor_count);
	return load_bitmap_now ? retrieveOriginalBitmap() : TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const VPathString& bitmap_file_name,
						 int major_count, int minor_count,
						 boolean load_bitmap_now)
{
	destructBitmaps();
	originalBitmapName = new VPathString(bitmap_file_name);
	originalBitmapFromBitmapFile = 1;
	setItemCount(major_count, minor_count);
	return load_bitmap_now ? retrieveOriginalBitmap() : TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const VString& vrf_bitmap_name,
						 const VPathString& vrf_file_name,
						 int major_count, int minor_count,
						 boolean load_bitmap_now)
{
	destructBitmaps();
	originalBitmapName = new VString(vrf_bitmap_name);
	originalBitmapVrfName = new VPathString(vrf_file_name);
	originalBitmapFromVrf = 1;
	setItemCount(major_count, minor_count);
	return load_bitmap_now ? retrieveOriginalBitmap() : TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const VString& vrf_bitmap_name,
						 const VString& vrf_file_name,
						 int major_count, int minor_count,
						 boolean load_bitmap_now)
{
	return setImage(vrf_bitmap_name, VPathString(vrf_file_name),
					major_count, minor_count, load_bitmap_now);
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const VString& vrf_bitmap_name,
						 const char *vrf_file_name,
						 int major_count, int minor_count,
						 boolean load_bitmap_now)
{
	return setImage(vrf_bitmap_name, VPathString(vrf_file_name),
					major_count, minor_count, load_bitmap_now);
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const char *vrf_bitmap_name,
						 const VPathString& vrf_file_name,
						 int major_count, int minor_count,
						 boolean load_bitmap_now)
{
	return setImage(VString(vrf_bitmap_name), vrf_file_name,
					major_count, minor_count, load_bitmap_now);
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const char *vrf_bitmap_name,
						 const VString& vrf_file_name,
						 int major_count, int minor_count,
						 boolean load_bitmap_now)
{
	return setImage(VString(vrf_bitmap_name), VPathString(vrf_file_name),
					major_count, minor_count, load_bitmap_now);
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const char *vrf_bitmap_name,
						 const char *vrf_file_name,
						 int major_count, int minor_count,
						 boolean load_bitmap_now)
{
	return setImage(VString(vrf_bitmap_name), VPathString(vrf_file_name),
					major_count, minor_count, load_bitmap_now);
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const VBitMap& bitmap,
						 int major_count, int minor_count)
{
	if (!((VBitMap&)bitmap).isValid()) {
		return FALSE;
	}
	destructBitmaps();
	originalBitmap = copyBitmap(&bitmap);
	originalBitmapIsMine = 1;
	setItemCount(major_count, minor_count);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImage::setImage(const VBitMap *bitmap,
						 int major_count, int minor_count)
{
	if ((bitmap == 0) || !((VBitMap *)bitmap)->isValid()) {
		return FALSE;
	}
	destructBitmaps();
	originalBitmap = (VBitMap *)bitmap;
	setItemCount(major_count, minor_count);
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VImage::setItemCount(int major_count, int minor_count)
{
	if (major_count <= 0) { major_count = 1; }
	if (minor_count <= 0) { minor_count = 1; }
	imageMajorCount = major_count;
	imageMinorCount = minor_count;
	if (originalBitmap != 0) {
		int w, h;
		originalBitmap->sizeOfImage(&w, &h);
		imageMajorSize = w / imageMajorCount;
		imageMinorSize = h / imageMinorCount;
	}
}

// --------------------------------------------------------------------------
//
void VImage::getItemGeometry(int& x, int& y, int& w, int& h,
							 int major_index, int minor_index)
{
	if (major_index < 0) {
		major_index = 0;
	}
	else if (major_index >= imageMajorCount) {
		major_index = imageMajorCount - 1;
	}
	if (minor_index < 0) {
		minor_index = 0;
	}
	else if (minor_index >= imageMinorCount) {
		minor_index = imageMinorCount - 1;
	}
	if ((originalBitmap != 0) || retrieveOriginalBitmap()) {
		w = imageMajorSize;
		h = imageMinorSize;
		x = major_index * w;
		y = minor_index * h;
	}
	else {
		x = y = w = h = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::getItemGeometry(int *x, int *y, int *w, int *h,
							 int major_index, int minor_index)
{
	if (major_index < 0) {
		major_index = 0;
	}
	else if (major_index >= imageMajorCount) {
		major_index = imageMajorCount = 1;
	}
	if (minor_index < 0) {
		minor_index = 0;
	}
	else if (minor_index >= imageMinorCount) {
		minor_index = imageMinorCount = 1;
	}
	if ((originalBitmap != 0) || retrieveOriginalBitmap()) {
		if (x != 0) { *x = major_index * imageMajorSize; }
		if (y != 0) { *y = minor_index * imageMinorSize; }
		if (w != 0) { *w = imageMajorSize; }
		if (h != 0) { *h = imageMinorSize; }
	}
	else {
		if (x != 0) { *x = 0; }
		if (y != 0) { *y = 0; }
		if (w != 0) { *w = 0; }
		if (h != 0) { *h = 0; }
	}
}

// --------------------------------------------------------------------------
//
int VImage::getDepth() const
{
	if ((originalBitmap != 0) || retrieveOriginalBitmap()) {
		return originalBitmap->depthOfImage();
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::setTransparentColor(const VColor& color)
{
	if ((transparentColor == 0) ||
		(transparentColor == defaultTransparentColor)) {
		transparentColor = new VColor(color);
		transparentColorIsMine = 1;
	}
	else {
		*transparentColor = color;
	}
	if (andMaskBitmap != 0) {
		delete andMaskBitmap; andMaskBitmap = 0;
	}
	if (disabledAndMaskBitmap != 0) {
		delete disabledAndMaskBitmap; disabledAndMaskBitmap = 0;
	}
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (unmappedOrBitmap != 0) {
		delete unmappedOrBitmap; unmappedOrBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::setAdditionalDisabledMaskColor(const VColor& color)
{
	if (additionalDisabledMaskColor == 0) {
		additionalDisabledMaskColor = new VColor(color);
	}
	else {
		*additionalDisabledMaskColor = color;
	}
	if (disabledAndMaskBitmap != 0) {
		delete disabledAndMaskBitmap; disabledAndMaskBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::setAdditionalDisabledMaskColor()
{
	if (additionalDisabledMaskColor != 0) {
		delete additionalDisabledMaskColor; additionalDisabledMaskColor = 0;
		if (disabledAndMaskBitmap != 0) {
			delete disabledAndMaskBitmap; disabledAndMaskBitmap = 0;
		}
	}
}

// --------------------------------------------------------------------------
//
void VImage::readTransparentColorFromUpperLeft(boolean b)
{
	if (b) {
		if (amReadingTransparentColorFromUpperLeft) {
			return;
		}
		amReadingTransparentColorFromUpperLeft = 1;
	}
	else {
		if (!amReadingTransparentColorFromUpperLeft) {
			return;
		}
		amReadingTransparentColorFromUpperLeft = 0;
	}
	if (transparentColorIsMine) {
		transparentColorIsMine = 0;
		if (transparentColor != 0) {
			delete transparentColor;
			transparentColor = 0;
		}
	}
	if (andMaskBitmap != 0) {
		delete andMaskBitmap; andMaskBitmap = 0;
	}
	if (disabledAndMaskBitmap != 0) {
		delete disabledAndMaskBitmap; disabledAndMaskBitmap = 0;
	}
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (unmappedOrBitmap != 0) {
		delete unmappedOrBitmap; unmappedOrBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::mapColors(boolean b)
{
	mapShadedBackgroundColor(b);
	mapShadedHighlightColor(b);
	mapShadedShadowColor(b);
	mapShadedForegroundColor(b);
	mapFlatBackgroundColor(b);
	mapFlatBackgroundSelectColor(b);
}

// --------------------------------------------------------------------------
//
void VImage::mapShadedColors(boolean b)
{
	mapShadedBackgroundColor(b);
	mapShadedHighlightColor(b);
	mapShadedShadowColor(b);
}

// --------------------------------------------------------------------------
//
void VImage::mapShadedBackgroundColor(boolean b)
{
	if (b) {
		if (amMappingShadedBackgroundColor) {
			return;
		}
		amMappingShadedBackgroundColor = 1;
	}
	else {
		if (!amMappingShadedBackgroundColor) {
			return;
		}
		amMappingShadedBackgroundColor = 0;
	}
	if (disabledAndMaskBitmap != 0) {
		delete disabledAndMaskBitmap; disabledAndMaskBitmap = 0;
	}
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (mappedBitmap != 0) {
		delete mappedBitmap; mappedBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::mapShadedHighlightColor(boolean b)
{
	if (b) {
		if (amMappingShadedHighlightColor) {
			return;
		}
		amMappingShadedHighlightColor = 1;
	}
	else {
		if (!amMappingShadedHighlightColor) {
			return;
		}
		amMappingShadedHighlightColor = 0;
	}
	if (disabledAndMaskBitmap != 0) {
		delete disabledAndMaskBitmap; disabledAndMaskBitmap = 0;
	}
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (mappedBitmap != 0) {
		delete mappedBitmap; mappedBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::mapShadedShadowColor(boolean b)
{
	if (b) {
		if (amMappingShadedShadowColor) {
			return;
		}
		amMappingShadedShadowColor = 1;
	}
	else {
		if (!amMappingShadedShadowColor) {
			return;
		}
		amMappingShadedShadowColor = 0;
	}
	if (disabledAndMaskBitmap != 0) {
		delete disabledAndMaskBitmap; disabledAndMaskBitmap = 0;
	}
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (mappedBitmap != 0) {
		delete mappedBitmap; mappedBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::mapShadedForegroundColor(boolean b)
{
	if (b) {
		if (amMappingShadedForegroundColor) {
			return;
		}
		amMappingShadedForegroundColor = 1;
	}
	else {
		if (!amMappingShadedForegroundColor) {
			return;
		}
		amMappingShadedForegroundColor = 0;
	}
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (mappedBitmap != 0) {
		delete mappedBitmap; mappedBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::mapFlatBackgroundColor(boolean b)
{
	if (b) {
		if (amMappingFlatBackgroundColor) {
			return;
		}
		amMappingFlatBackgroundColor = 1;
	}
	else {
		if (!amMappingFlatBackgroundColor) {
			return;
		}
		amMappingFlatBackgroundColor = 0;
	}
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (mappedBitmap != 0) {
		delete mappedBitmap; mappedBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImage::mapFlatBackgroundSelectColor(boolean b)
{
	if (b) {
		if (amMappingFlatBackgroundSelectColor) {
			return;
		}
		amMappingFlatBackgroundSelectColor = 1;
	}
	else {
		if (!amMappingFlatBackgroundSelectColor) {
			return;
		}
		amMappingFlatBackgroundSelectColor = 0;
	}
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (mappedBitmap != 0) {
		delete mappedBitmap; mappedBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
const VColor& VImage::getTransparentColor() const
{
	if (amReadingTransparentColorFromUpperLeft) {
		return getUpperLeftPixelColor();
	}
	else if (transparentColor == 0) {
		if (defaultTransparentColor == 0) {
			defaultTransparentColor =
				new VColor(192, 192, 192);	// light-gray under MS/Windows
		}
		((VImage *)this)->transparentColor = defaultTransparentColor;
	}
	return *transparentColor;
}

// --------------------------------------------------------------------------
//
const VColor& VImage::getShadedBackgroundColor() const
{
	if (defaultShadedBackgroundColor == 0) {
		defaultShadedBackgroundColor =
			new VColor(192, 192, 192);		// light-gray under MS/Windows
	}
	return *defaultShadedBackgroundColor;
}

// --------------------------------------------------------------------------
//
const VColor& VImage::getShadedHighlightColor() const
{
	if (defaultShadedHighlightColor == 0) {
		defaultShadedHighlightColor =
			new VColor(255, 255, 255);		// white under MS/Windows
	}
	return *defaultShadedHighlightColor;
}

// --------------------------------------------------------------------------
//
const VColor& VImage::getShadedShadowColor() const
{
	if (defaultShadedShadowColor == 0) {
		defaultShadedShadowColor =
			new VColor(128, 128, 128);		// dark-gray under MS/Windows
	}
	return *defaultShadedShadowColor;
}

// --------------------------------------------------------------------------
//
const VColor& VImage::getShadedForegroundColor() const
{
	if (defaultShadedForegroundColor == 0) {
		defaultShadedForegroundColor =
			new VColor(0, 0, 0);			// black under MS/Windows
	}
	return *defaultShadedForegroundColor;
}

// --------------------------------------------------------------------------
//
const VColor& VImage::getFlatBackgroundColor() const
{
	if (defaultFlatBackgroundColor == 0) {
		defaultFlatBackgroundColor =
			new VColor(255, 0, 255);		// magenta under MS/Windows
	}
	return *defaultFlatBackgroundColor;
}

// --------------------------------------------------------------------------
//
const VColor& VImage::getFlatBackgroundSelectColor() const
{
	if (defaultFlatBackgroundSelectColor == 0) {
		defaultFlatBackgroundSelectColor =
			new VColor(0, 0, 255);		// blue under MS/Windows
	}
	return *defaultFlatBackgroundSelectColor;
}

// --------------------------------------------------------------------------
//
const VColor& VImage::getUpperLeftPixelColor() const
{
	if (upperLeftPixelColor == 0) {
		((VImage *)this)->upperLeftPixelColor = new VColor(192, 192, 192);
		if ((originalBitmap != 0) || retrieveOriginalBitmap()) {
			*upperLeftPixelColor =
				originalBitmap->readPixel(VLocation(0, 0));
		}
	}
	return *upperLeftPixelColor;
}

// --------------------------------------------------------------------------
//
boolean VImage::isValid() const
{
	return originalBitmap != 0 ?
		   originalBitmap->isValid() : retrieveOriginalBitmap();
}

// --------------------------------------------------------------------------
//
void VImage::draw(VPort *port, int x, int y,
				  int major_index, int minor_index, int style)
{
	draw(port->getImage(), x, y, major_index, minor_index, style);
}

// --------------------------------------------------------------------------
//
void VImage::draw(VDisplay *display, int x, int y,
				  int major_index, int minor_index, int style)
{
	if (style == 0) {
		style = drawMode;
	}

	if (style & Unmapped) {
		if (style & Transparent) {
			drawUnmappedTransparent(display, x, y, major_index, minor_index);
		}
		else {
			drawUnmappedOpaque(display, x, y, major_index, minor_index);
		}
	}
	else if (style & Mapped) {
		if (style & Transparent) {
			drawMappedTransparent(display, x, y, major_index, minor_index);
		}
		else {
			drawMappedOpaque(display, x, y, major_index, minor_index);
		}
	}
	else if (style & Disabled) {
		drawDisabled(display, x, y, major_index, minor_index);
	}
	else if (style & Mask) {
		drawMask(display, x, y, major_index, minor_index);
	}
	else if (style & DisabledMask) {
		drawDisabledMask(display, x, y, major_index, minor_index);
	}
	else {
		if (style & Transparent) {
			drawUnmappedTransparent(display, x, y, major_index, minor_index);
		}
		else {
			drawUnmappedOpaque(display, x, y, major_index, minor_index);
		}
	}
}

// --------------------------------------------------------------------------
// Draws the specified mapped image item within this VImage transparently at
// the specifed location within the given VDisplay.
//
void VImage::drawMappedTransparent(VDisplay *display, int x, int y,
								   int major_index, int minor_index)
{
	if (display == 0) {
		return;
	}
	if ((originalBitmap == 0) && !retrieveOriginalBitmap()) {
		return;
	}
	if (!constructMappedOrBitmap() || !constructAndMaskBitmap()) {
		return;
	}

	int ix, iy, iw, ih;
	getItemGeometry(ix, iy, iw, ih, major_index, minor_index);

#if defined(CV_WINDOWS)

	HDC src = mappedOrBitmap->getHndl();
	HDC mask = andMaskBitmap->getHndl();
	HDC dst = display->getHndl();
	HPALETTE hpalette = mappedOrBitmap->getPalette();

	SetBkColor(dst, RGB(255, 255, 255));
	SetTextColor(dst, RGB(0, 0, 0));
	BitBlt(dst, x, y, iw, ih, mask, ix, iy, SRCAND);
	if (hpalette != 0) {
		HPALETTE old_hpalette = SelectPalette(dst, hpalette, TRUE);
		RealizePalette(dst);
		BitBlt(dst, x, y, iw, ih, src, ix, iy, SRCPAINT);
		SelectPalette(dst, old_hpalette, TRUE);
	}
	else {
		BitBlt(dst, x, y, iw, ih, src, ix, iy, SRCPAINT);
	}

	mappedOrBitmap->ungetHndl();
	andMaskBitmap->ungetHndl();
	display->ungetHndl();

#elif defined(CV_MOTIF)

	HDC src = mappedOrBitmap->getHndl();
	HDC mask = andMaskBitmap->getHndl();
	HDC dst = display->getHndl();

	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XSetFunction(dst->display, gc, GXcopy);
	XSetClipMask(dst->display, gc, mask->drawable);
	XSetClipOrigin(dst->display, gc, -ix + x, -iy + y);
	VColor red(VColor::Blue);
	VColor green(VColor::Green);
	XSetForeground(dst->display, gc, long(red));
	XSetBackground(dst->display, gc, long(green));
	XCopyArea(dst->display, src->drawable, dst->drawable, gc,
			  ix, iy, iw, ih, x, y);
	XFreeGC(dst->display, gc);

	mappedOrBitmap->ungetHndl();
	andMaskBitmap->ungetHndl();
	display->ungetHndl();

#else

	VBitBltr bband(andMaskBitmap, display);
	bband.bitbltr(ix, iy, iw, ih, x, y, AndRule);
	VBitBltr bbor(mappedOrBitmap, display);
	bbor.bitbltr(ix, iy, iw, ih, x, y, OrRule);

#endif

}

// --------------------------------------------------------------------------
// Draws the specified mapped image item within this VImage at the specifed
// location within the given VDisplay.
//
void VImage::drawMappedOpaque(VDisplay *display, int x, int y,
							  int major_index, int minor_index)
{
	if (display == 0) {
		return;
	}
	if ((originalBitmap == 0) && !retrieveOriginalBitmap()) {
		return;
	}
	if (!constructMappedBitmap()) {
		return;
	}

	int ix, iy, iw, ih;
	getItemGeometry(ix, iy, iw, ih, major_index, minor_index);
	VBitBltr bb(mappedBitmap, display);
	bb.bitbltr(ix, iy, iw, ih, x, y, CopyRule);
}

// --------------------------------------------------------------------------
// Draws the specified unmapped image item within this VImage transparently
// at the specifed location within the given VDisplay.
//
void VImage::drawUnmappedTransparent(VDisplay *display, int x, int y,
									 int major_index, int minor_index)
{
	if (display == 0) {
		return;
	}
	if ((originalBitmap == 0) && !retrieveOriginalBitmap()) {
		return;
	}
	if (!constructUnmappedOrBitmap() || !constructAndMaskBitmap()) {
		return;
	}

	int ix, iy, iw, ih;
	getItemGeometry(ix, iy, iw, ih, major_index, minor_index);

#if defined(CV_WINDOWS)

	HDC src = unmappedOrBitmap->getHndl();
	HDC mask = andMaskBitmap->getHndl();
	HDC dst = display->getHndl();
	HPALETTE hpalette = unmappedOrBitmap->getPalette();

	SetBkColor(dst, RGB(255, 255, 255));
	SetTextColor(dst, RGB(0, 0, 0));
	BitBlt(dst, x, y, iw, ih, mask, ix, iy, SRCAND);
	if (hpalette != 0) {
		HPALETTE old_hpalette = SelectPalette(dst, hpalette, TRUE);
		RealizePalette(dst);
		BitBlt(dst, x, y, iw, ih, src, ix, iy, SRCPAINT);
		SelectPalette(dst, old_hpalette, TRUE);
	}
	else {
		BitBlt(dst, x, y, iw, ih, src, ix, iy, SRCPAINT);
	}

	unmappedOrBitmap->ungetHndl();
	andMaskBitmap->ungetHndl();
	display->ungetHndl();

#elif defined(CV_MOTIF)

	HDC src = unmappedOrBitmap->getHndl();
	HDC mask = andMaskBitmap->getHndl();
	HDC dst = display->getHndl();

	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XSetFunction(dst->display, gc, GXandInverted);
	XSetForeground(dst->display, gc, 0xFFFFFFFFul);
	XSetBackground(dst->display, gc, 0x00000000ul);
	XCopyPlane(dst->display, mask->drawable, dst->drawable, gc,
			   ix, iy, iw, ih, x, y, 1);
	XSetFunction(dst->display, gc, GXor);
	XCopyArea(dst->display, src->drawable, dst->drawable, gc,
			  ix, iy, iw, ih, x, y);
	XFreeGC(dst->display, gc);

	unmappedOrBitmap->ungetHndl();
	andMaskBitmap->ungetHndl();
	display->ungetHndl();

#else

	VBitBltr bb(originalBitmap, display);
	bb.bitbltr(ix, iy, iw, ih, x, y, CopyRule);

#endif

}

// --------------------------------------------------------------------------
// Draws the specified unmapped image item within this VImage at the
// specified location within the given VDisplay.
//
void VImage::drawUnmappedOpaque(VDisplay *display, int x, int y,
								int major_index, int minor_index)
{
	if (display == 0) {
		return;
	}
	if ((originalBitmap == 0) && !retrieveOriginalBitmap()) {
		return;
	}

	int ix, iy, iw, ih;
	getItemGeometry(ix, iy, iw, ih, major_index, minor_index);
	VBitBltr bb(originalBitmap, display);
	bb.bitbltr(ix, iy, iw, ih, x, y, CopyRule);
}

// --------------------------------------------------------------------------
// Draws the disabled image item within this VImage at the specified
// location within the given VDisplay.
//
void VImage::drawDisabled(VDisplay *display, int x, int y,
						  int major_index, int minor_index)
{
	if (display == 0) {
		return;
	}
	if ((originalBitmap == 0) && !retrieveOriginalBitmap()) {
		return;
	}
	if (!constructDisabledAndMaskBitmap()) {
		return;
	}

	int ix, iy, iw, ih;
	getItemGeometry(ix, iy, iw, ih, major_index, minor_index);

	long shaded_highlight_pixel =
		 VColorScheme::systemColor(VColorScheme::ShadedHighlight);
	long shaded_shadow_pixel =
		 VColorScheme::systemColor(VColorScheme::ShadedShadow);

#if defined(CV_WINDOWS)

	const unsigned long PSDPxax = 0x00B8074Aul;

	HDC mask = disabledAndMaskBitmap->getHndl();
	HDC dst = display->getHndl();

	SetBkColor(dst, RGB(255, 255, 255));
	SetTextColor(dst, RGB(0, 0, 0));
	HBRUSH highlight_brush = CreateSolidBrush(shaded_highlight_pixel);
	HBRUSH shadow_brush = CreateSolidBrush(shaded_shadow_pixel);
	HBRUSH old_brush = HBRUSH(SelectObject(dst, highlight_brush));
	BitBlt(dst, x + 1, y + 1, iw, ih, mask, ix, iy, PSDPxax);
	SelectObject(dst, shadow_brush);
	BitBlt(dst, x, y, iw, ih, mask, ix, iy, PSDPxax);
	SelectObject(dst, old_brush);
	DeleteObject(highlight_brush);
	DeleteObject(shadow_brush);

	disabledAndMaskBitmap->ungetHndl();
	display->ungetHndl();

#elif defined(CV_MOTIF)

	HDC mask = disabledAndMaskBitmap->getHndl();
	HDC dst = display->getHndl();

	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XSetFunction(dst->display, gc, GXcopy);
	XSetClipMask(dst->display, gc, mask->drawable);
	XSetClipOrigin(dst->display, gc, -ix + x + 1, -iy + y + 1);
	XSetForeground(dst->display, gc, shaded_highlight_pixel);
	XCopyPlane(dst->display, mask->drawable, dst->drawable, gc,
			   ix, iy, iw, ih, x + 1, y + 1, 1);
	XSetClipOrigin(dst->display, gc, -ix + x, -iy + y);
	XSetForeground(dst->display, gc, shaded_shadow_pixel);
	XCopyPlane(dst->display, mask->drawable, dst->drawable, gc,
			   ix, iy, iw, ih, x, y, 1);
	XFreeGC(dst->display, gc);

	disabledAndMaskBitmap->ungetHndl();
	display->ungetHndl();

#else

	VBitBltr bb(disabledAndMaskBitmap, display);
	bb.bitbltr(ix, iy, iw, ih, x, y, AndRule);

#endif

}

// --------------------------------------------------------------------------
// Draws the mask image item within this VImage transparently at the
// specified location within the given VDisplay.
//
void VImage::drawMask(VDisplay *display, int x, int y,
								 int major_index, int minor_index)
{
	if (display == 0) {
		return;
	}
	if ((originalBitmap == 0) && !retrieveOriginalBitmap()) {
		return;
	}
	if (!constructAndMaskBitmap()) {
		return;
	}

	int ix, iy, iw, ih;
	getItemGeometry(ix, iy, iw, ih, major_index, minor_index);

#if defined(CV_WINDOWS)

	HDC dst = display->getHndl();
	SetTextColor(dst, RGB(0, 0, 0));
	display->ungetHndl();

	VBitBltr bb(andMaskBitmap, display);
	bb.bitbltr(ix, iy, iw, ih, x, y, AndRule);

#elif defined(CV_MOTIF)

	HDC mask = andMaskBitmap->getHndl();
	HDC dst = display->getHndl();

	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XSetFunction(dst->display, gc, GXandInverted);
	XSetForeground(dst->display, gc, 0xFFFFFFFFul);
	XSetBackground(dst->display, gc, 0x00000000ul);
	XCopyPlane(dst->display, mask->drawable, dst->drawable, gc,
			   ix, iy, iw, ih, x, y, 1);
	XFreeGC(dst->display, gc);

	andMaskBitmap->ungetHndl();
	display->ungetHndl();

#else

	VBitBltr bb(andMaskBitmap, display);
	bb.bitbltr(ix, iy, iw, ih, x, y, AndRule);

#endif

}

// --------------------------------------------------------------------------
// Draws the disabled-mask image item within this VImage transparently at
// the specified location within the given VDisplay.
//
void VImage::drawDisabledMask(VDisplay *display, int x, int y,
										 int major_index, int minor_index)
{
	if (display == 0) {
		return;
	}
	if ((originalBitmap == 0) && !retrieveOriginalBitmap()) {
		return;
	}
	if (!constructDisabledAndMaskBitmap()) {
		return;
	}

	int ix, iy, iw, ih;
	getItemGeometry(ix, iy, iw, ih, major_index, minor_index);

#if defined(CV_WINDOWS)

	HDC dst = display->getHndl();
	SetTextColor(dst, RGB(0, 0, 0));
	display->ungetHndl();

	VBitBltr bb(disabledAndMaskBitmap, display);
	bb.bitbltr(ix, iy, iw, ih, x, y, AndRule);

#elif defined(CV_MOTIF)

	HDC mask = disabledAndMaskBitmap->getHndl();
	HDC dst = display->getHndl();

	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XSetFunction(dst->display, gc, GXandInverted);
	XSetForeground(dst->display, gc, 0xFFFFFFFFul);
	XSetBackground(dst->display, gc, 0x00000000ul);
	XCopyPlane(dst->display, mask->drawable, dst->drawable, gc,
			   ix, iy, iw, ih, x, y, 1);
	XFreeGC(dst->display, gc);

	disabledAndMaskBitmap->ungetHndl();
	display->ungetHndl();

#else

	VBitBltr bb(disabledAndMaskBitmap, display);
	bb.bitbltr(ix, iy, iw, ih, x, y, AndRule);

#endif

}

// --------------------------------------------------------------------------
//
void VImage::systemColorChanged()
{
	if (mappedOrBitmap != 0) {
		delete mappedOrBitmap; mappedOrBitmap = 0;
	}
	if (unmappedOrBitmap != 0) {
		delete unmappedOrBitmap; unmappedOrBitmap = 0;
	}
	if (mappedBitmap != 0) {
		delete mappedBitmap; mappedBitmap = 0;
	}
	if (disabledAndMaskBitmap != 0) {
		delete disabledAndMaskBitmap; disabledAndMaskBitmap = 0;
	}
}

// --------------------------------------------------------------------------
//
boolean VImage::retrieveOriginalBitmap(VObject *ra) const
{
	if (originalBitmap != 0) {
		return TRUE;
	}
	if ((originalBitmapName == 0) ||
		(originalBitmapName->size() <= 0)) {
		return FALSE;
	}

	if (originalBitmapFromVrf) {
		if (ra != 0) {
			boolean was_already_open = ((VRscArchiver *)ra)->isOpen();
			if (!was_already_open) {
				if (!((VRscArchiver *)ra)->open(VRscArchiver::ReadOnly)) {
					return FALSE;
				}
			}
			((VImage *)this)->originalBitmap =
				(VBitMap *)((VRscArchiver *)ra)->
					getObject(*originalBitmapName, 0, VBitMapCls, 0, TRUE);
			if (!was_already_open) {
				((VRscArchiver *)ra)->close();
			}
		}
		else if ((originalBitmapVrfName == 0) ||
				 (originalBitmapVrfName->size() <= 0)) {
			return FALSE;
		}
		else {
			VRscArchiver ra(*originalBitmapVrfName);
			if (!ra.open(VRscArchiver::ReadOnly)) {
				return FALSE;
			}
			((VImage *)this)->originalBitmap =
				(VBitMap *)ra.getObject(*originalBitmapName, 0,
										 VBitMapCls, 0, TRUE);
			ra.close();
		}
		if (originalBitmap == 0) {
			return FALSE;
		}
		if (!originalBitmap->isValid()) {
			delete ((VImage *)this)->originalBitmap;
			((VImage *)this)->originalBitmap = 0;
			return FALSE;
		}
		//
		// Note that originalBitmapIsMine is FALSE.
		//
	}
	else if (originalBitmapFromBitmapFile) {
		if ((((VImage *)this)->originalBitmap = new VBitMap) == 0) {
			return FALSE;
		}
		else if (!originalBitmap->loadFile(*originalBitmapName)) {
			delete ((VImage *)this)->originalBitmap;
			((VImage *)this)->originalBitmap = 0;
			return FALSE;
		}
		else {
			((VImage *)this)->originalBitmapIsMine = 1;
		}
	}
	else if (originalBitmapFromNativeResource) {
		if ((((VImage *)this)->originalBitmap =
							   new VBitMap((const char *)
							   			   *originalBitmapName)) == 0) {
			if ((((VImage *)this)->originalBitmap = new VBitMap) == 0) {
				return FALSE;
			}
			else if (!originalBitmap->loadFile(*originalBitmapName)) {
				delete ((VImage *)this)->originalBitmap;
				((VImage *)this)->originalBitmap = 0;
				return FALSE;
			}
			else {
				((VImage *)this)->originalBitmapFromNativeResource = 0;
				((VImage *)this)->originalBitmapFromBitmapFile = 1;
			}
		}
		else if (!originalBitmap->isValid()) {
			delete ((VImage *)this)->originalBitmap;
			if ((((VImage *)this)->originalBitmap = new VBitMap) == 0) {
				return FALSE;
			}
			else if (!originalBitmap->loadFile(*originalBitmapName)) {
				delete ((VImage *)this)->originalBitmap;
				((VImage *)this)->originalBitmap = 0;
				return FALSE;
			}
			else {
				((VImage *)this)->originalBitmapFromNativeResource = 0;
				((VImage *)this)->originalBitmapFromBitmapFile = 1;
			}
		}
		((VImage *)this)->originalBitmapIsMine = 1;
	}
	else {
		return FALSE;
	}

	int w, h;
	originalBitmap->sizeOfImage(&w, &h);
	((VImage *)this)->imageMajorSize = w / imageMajorCount;
	((VImage *)this)->imageMinorSize = h / imageMinorCount;
	return TRUE;
}

// --------------------------------------------------------------------------
// Construct the AND mask bitmap.  This is the original bitmap with all of
// the transparent pixels set to 1's (white on MS/Windows), and all of the
// rest of the pixels set to 0's (black on MS/Windows).
//
boolean VImage::constructAndMaskBitmap()
{
	if (andMaskBitmap != 0) {
		return TRUE;
	}
	if (!retrieveOriginalBitmap()) {
		return FALSE;
	}

	int w = getWidth();
	int h = getHeight();

	// Copy into the and-mask-image, the original image except
	// with each of the bitmap-transparent-color pixels set to
	// all 1's, and each of the other pixels set to all 0's.

#if defined(CV_WINDOWS)

	andMaskBitmap = new VBitMap(w, h, TRUE);

	HDC original = originalBitmap->getHndl();
	HDC dst = andMaskBitmap->getHndl();

	SetBkColor(original, long(getTransparentColor()));
	BitBlt(dst, 0, 0, w, h, original, 0, 0, SRCCOPY);

	originalBitmap->ungetHndl();
	andMaskBitmap->ungetHndl();

#elif defined(CV_MOTIF)

	andMaskBitmap = new VBitMap(w, h, TRUE);

	HDC original = originalBitmap->getHndl();

	XImage *original_image = XGetImage(original->display, original->drawable,
									   0, 0, w, h, AllPlanes, ZPixmap);
	if (original_image == 0) {
		originalBitmap->getHndl();
		return FALSE;
	}

	HDC dst = andMaskBitmap->getHndl();

	XImage *dst_image = XGetImage(dst->display, dst->drawable,
								  0, 0, w, h, AllPlanes, ZPixmap);
	if (dst_image == 0) {
		delete original_image;
		originalBitmap->ungetHndl();
		andMaskBitmap->ungetHndl();
		return FALSE;
	}

	long transparent_pixel = getTransparentColor();

	for (int x = 0 ; x < w ; x++) {
		for (int y = 0 ; y < h ; y++) {
			if (XGetPixel(original_image, x, y) == transparent_pixel) {
				XPutPixel(dst_image, x, y, 0);
			}
			else {
				XPutPixel(dst_image, x, y, 1);
			}
		}
	}
	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XPutImage(dst->display, dst->drawable, gc, dst_image, 0, 0, 0, 0, w, h);
	XFreeGC(dst->display, gc);
	XDestroyImage(dst_image);
	XDestroyImage(original_image);
	originalBitmap->ungetHndl();
	andMaskBitmap->ungetHndl();

#else

	andMaskBitmap = copyOriginalBitmap();

#endif

	return TRUE;
}

// --------------------------------------------------------------------------
// Construct the disabled AND mask bitmap.  This is the original bitmap with
// all of the transparent pixels and highlight pixels set to 1's (white on
// MS/Windows), and all of the rest of the pixels set to 0's (black on
// MS/Windows).  This bitmap is used to draw the disabled abled image by
// first drawing the 1 pixels in the system shaded-highlight color and then
// drawing the 1 pixels in the system shaded-shadow color offset up and to
// the left by 1.
//
boolean VImage::constructDisabledAndMaskBitmap()
{
	if (disabledAndMaskBitmap != 0) {
		return TRUE;
	}
	if (!retrieveOriginalBitmap()) {
		return FALSE;
	}

	int w = getWidth();
	int h = getHeight();

#if defined(CV_WINDOWS)

	disabledAndMaskBitmap = new VBitMap(w, h, TRUE);

	HDC original = originalBitmap->getHndl();
	HDC dst = disabledAndMaskBitmap->getHndl();

	// Copy into the monochrome and-mask-image, the original
	// image except with each of the bitmap-transparent-color
	// and bitmap-highlight-color pixels set to 1, and all
	// the other pixels set to 0.

	SetBkColor(original, long(getTransparentColor()));
	BitBlt(dst, 0, 0, w, h, original, 0, 0, SRCCOPY);

	SetBkColor(original, long(getShadedHighlightColor()));
	BitBlt(dst, 0, 0, w, h, original, 0, 0, SRCPAINT);

	if (additionalDisabledMaskColor != 0) {
		SetBkColor(original, long(*additionalDisabledMaskColor));
		BitBlt(dst, 0, 0, w, h, original, 0, 0, SRCPAINT);
	}

	originalBitmap->ungetHndl();
	disabledAndMaskBitmap->ungetHndl();

#elif defined(CV_MOTIF)

	disabledAndMaskBitmap = new VBitMap(w, h, TRUE);

	HDC original = originalBitmap->getHndl();

	XImage *original_image = XGetImage(original->display, original->drawable,
									   0, 0, w, h, AllPlanes, ZPixmap);
	if (original_image == 0) {
		originalBitmap->getHndl();
		return FALSE;
	}

	HDC dst = disabledAndMaskBitmap->getHndl();

	XImage *dst_image = XGetImage(dst->display, dst->drawable,
								  0, 0, w, h, AllPlanes, ZPixmap);
	if (dst_image == 0) {
		delete original_image;
		originalBitmap->ungetHndl();
		andMaskBitmap->ungetHndl();
		return FALSE;
	}

	long transparent_pixel = getTransparentColor();
	long shaded_highlight_pixel = getShadedHighlightColor();
	long additional_disabled_mask_pixel;
	
	if (additionalDisabledMaskColor != 0) {
		additional_disabled_mask_pixel = *additionalDisabledMaskColor;
	}

	for (int x = 0 ; x < w ; x++) {
		for (int y = 0 ; y < h ; y++) {
			long pixel = XGetPixel(original_image, x, y);
			if ((pixel == transparent_pixel) ||
				(pixel == shaded_highlight_pixel) ||
				((additionalDisabledMaskColor != 0) &&
				 (pixel == additional_disabled_mask_pixel))) {
				XPutPixel(dst_image, x, y, 0);
			}
			else {
				XPutPixel(dst_image, x, y, 1);
			}
		}
	}
	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XPutImage(dst->display, dst->drawable, gc, dst_image, 0, 0, 0, 0, w, h);
	XFreeGC(dst->display, gc);
	XDestroyImage(dst_image);
	XDestroyImage(original_image);
	originalBitmap->ungetHndl();
	disabledAndMaskBitmap->ungetHndl();

#else

	disabledAndMaskBitmap = copyOriginalBitmap();

#endif

	return TRUE;
}

// --------------------------------------------------------------------------
// Construct the mapped bitmap.  This is the original bitmap with the color
// mappings defined by performColorMapping().
//
boolean VImage::constructMappedBitmap()
{
	if (mappedBitmap != 0) {
		return TRUE;
	}
	if (!retrieveOriginalBitmap()) {
		return FALSE;
	}

	// Copy the original bitmap to this bitmap.

	mappedBitmap = copyOriginalBitmap();

	// Map pixels to the current system values if desired.

	performColorMapping(mappedBitmap, FALSE);

	return TRUE;
}

// --------------------------------------------------------------------------
// Construct the mapped or bitmap.  This is the original bitmap with each
// of the pixels corresponding to 1 bits in the and-mask bitmap (i.e. the
// transparent pixels) set to all 0's; the other pixels are left intact (*).
// However, in addition, the intact pixels may be mapped according to the
// transformations defeined in performColorMapping().
//
boolean VImage::constructMappedOrBitmap()
{
	if (mappedOrBitmap != 0) {
		return TRUE;
	}
	if (!retrieveOriginalBitmap() || !constructAndMaskBitmap()) {
		return FALSE;
	}

	// Copy into the original bitmap into this bitmap, except with each of
	// the pixels corresponding to 1 bits in the and-mask bitmap (i.e. the
	// transparent pixels) set to all 0's; the other pixels are left intact.

	mappedOrBitmap = copyOriginalBitmap();

#if defined(CV_WINDOWS)

	HDC mask = andMaskBitmap->getHndl();
	HDC dst = mappedOrBitmap->getHndl();

	SetBkColor(dst, RGB(0, 0, 0));
	SetTextColor(dst, RGB(255, 255, 255));
	BitBlt(dst, 0, 0, getWidth(), getHeight(), mask, 0, 0, SRCAND);

	mappedOrBitmap->ungetHndl();
	andMaskBitmap->ungetHndl();

#elif defined(CV_MOTIF)

	HDC mask = andMaskBitmap->getHndl();
	HDC dst = mappedOrBitmap->getHndl();

	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XSetFunction(dst->display, gc, GXand);
	XSetForeground(dst->display, gc, 0xFFFFFFFFul);
	XSetBackground(dst->display, gc, 0x00000000ul);
	XCopyPlane(dst->display, mask->drawable, dst->drawable, gc,
			   0, 0, getWidth(), getHeight(), 0, 0, 1);
	XFreeGC(dst->display, gc);

	andMaskBitmap->ungetHndl();
	mappedOrBitmap->ungetHndl();

#else

	VBitBltr bbor(andMaskBitmap, mappedOrBitmap);
	bbor.bitbltr(0, 0, getWidth(), getHeight(), 0, 0, EraseRule);

#endif

	// Map pixels to the current system values if desired.

	performColorMapping(mappedOrBitmap);

	return TRUE;
}

// --------------------------------------------------------------------------
// Construct the unmapped OR bitmap.  This is the original bitmap with all
// of the transparent pixels set to 1's (white on MS/Windows), and all of the
// remaining pixels intact.
//
boolean VImage::constructUnmappedOrBitmap()
{
	if (unmappedOrBitmap != 0) {
		return TRUE;
	}
	if (!retrieveOriginalBitmap() || !constructAndMaskBitmap()) {
		return FALSE;
	}

	// Copy the original bitmap into this bitmap.

	unmappedOrBitmap = copyOriginalBitmap();

#if defined(CV_WINDOWS)

	HDC mask = andMaskBitmap->getHndl();
	HDC dst = unmappedOrBitmap->getHndl();

	// Map each of the pixels in this bitmap which
	// correspond to 1 bits in the and-mask to all 1's;
	// leave the other pixels intact.

	SetBkColor(dst, RGB(0, 0, 0));
	SetTextColor(dst, RGB(255, 255, 255));
	BitBlt(dst, 0, 0, getWidth(), getHeight(), mask, 0, 0, SRCAND);

	unmappedOrBitmap->ungetHndl();
	andMaskBitmap->ungetHndl();
	originalBitmap->ungetHndl();

#else

	// ToDo!!!

#endif

	return TRUE;
}

// --------------------------------------------------------------------------
// Perform the following color mappings if desired:
//
//	192,192,192 [light-gray]  -->  VColorScheme::ShadedBackground
//	255,255,255 [white]       -->  VColorScheme::ShadedHighlight
//	128,128,128 [dark-gray]   -->  VColorScheme::ShadedShadow
//	  0,  0,  0 [black]       -->  VColorScheme::ShadedForeground
//	255,  0,255 [magenta]     -->  VColorScheme::FlatBackground
//	  0,  0,255 [blue]        -->  VColorScheme::FlatBackgroundSelect
//
void VImage::performColorMapping(VBitMap *bitmap,
								 boolean dont_map_transparent_pixel)
{
	if (!amMappingShadedBackgroundColor &&
		!amMappingShadedHighlightColor &&
		!amMappingShadedShadowColor &&
		!amMappingShadedForegroundColor &&
		!amMappingFlatBackgroundColor &&
		!amMappingFlatBackgroundSelectColor) {
		return;
	}

	long shaded_background_pixel		= getShadedBackgroundColor();
	long shaded_highlight_pixel			= getShadedHighlightColor();
	long shaded_shadow_pixel			= getShadedShadowColor();
	long shaded_foreground_pixel		= getShadedForegroundColor();
	long flat_background_pixel			= getFlatBackgroundColor();
	long flat_background_select_pixel	= getFlatBackgroundSelectColor();
	long transparent_pixel				= getTransparentColor();

	boolean map_shaded_background =
			amMappingShadedBackgroundColor &&
			(!dont_map_transparent_pixel ||
			 (shaded_background_pixel != transparent_pixel));
	boolean map_shaded_highlight =
			amMappingShadedHighlightColor &&
			(!dont_map_transparent_pixel ||
			 (shaded_highlight_pixel != transparent_pixel));
	boolean map_shaded_shadow =
			amMappingShadedShadowColor &&
			(!dont_map_transparent_pixel ||
			 (shaded_shadow_pixel != transparent_pixel));
	boolean map_shaded_foreground =
			amMappingShadedForegroundColor &&
			(!dont_map_transparent_pixel ||
			 (shaded_foreground_pixel != transparent_pixel));
	boolean map_flat_background =
			amMappingFlatBackgroundColor &&
			(!dont_map_transparent_pixel ||
			 (flat_background_pixel != transparent_pixel));
	boolean map_flat_background_select =
			amMappingFlatBackgroundSelectColor &&
			(!dont_map_transparent_pixel ||
			 (flat_background_select_pixel != transparent_pixel));

	if (!map_shaded_background &&
		!map_shaded_highlight &&
		!map_shaded_shadow &&
		!map_flat_background &&
		!map_flat_background_select) {
		return;
	}

	long shaded_background_mapped_pixel =
		 VColorScheme::systemColor(VColorScheme::ShadedBackground);
	long shaded_highlight_mapped_pixel =
		 VColorScheme::systemColor(VColorScheme::ShadedHighlight);
	long shaded_shadow_mapped_pixel =
		 VColorScheme::systemColor(VColorScheme::ShadedShadow);
	long shaded_foreground_mapped_pixel =
		 VColorScheme::systemColor(VColorScheme::ShadedForeground);
	long flat_background_mapped_pixel =
		 VColorScheme::systemColor(VColorScheme::FlatBackground);
	long flat_background_select_mapped_pixel =
		 VColorScheme::systemColor(VColorScheme::FlatBackgroundSelect);

	int	w = getWidth(), h = getHeight();

#if defined(CV_WINDOWS)

	const unsigned long PSDPxax = 0x00B8074Aul;  // P xor (S and (D xor P))

	VBitMap	   *mask_bitmap	= new VBitMap(w, h, TRUE);
	HDC			mask		= mask_bitmap->getHndl();
	HDC			original	= originalBitmap->getHndl();
	HDC			dst			= bitmap->getHndl();

	if (map_shaded_background) {
		SetBkColor(original, shaded_background_pixel);
		BitBlt(mask, 0, 0, w, h, original, 0, 0, SRCCOPY);
		SetBkColor(dst, RGB(0, 0, 0));
		SetTextColor(dst, RGB(255, 255, 255));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, SRCAND);
		HBRUSH brush = CreateSolidBrush(shaded_background_mapped_pixel);
		HBRUSH old_brush = HBRUSH(SelectObject(dst, brush));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, PSDPxax);
		SelectObject(dst, old_brush);
		DeleteObject(brush);
	}
	if (map_shaded_highlight) {
		SetBkColor(original, shaded_highlight_pixel);
		BitBlt(mask, 0, 0, w, h, original, 0, 0, SRCCOPY);
		SetBkColor(dst, RGB(0, 0, 0));
		SetTextColor(dst, RGB(255, 255, 255));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, SRCAND);
		HBRUSH brush = CreateSolidBrush(shaded_highlight_mapped_pixel);
		HBRUSH old_brush = HBRUSH(SelectObject(dst, brush));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, PSDPxax);
		SelectObject(dst, old_brush);
		DeleteObject(brush);
	}
	if (map_shaded_shadow) {
		SetBkColor(original, shaded_shadow_pixel);
		BitBlt(mask, 0, 0, w, h, original, 0, 0, SRCCOPY);
		SetBkColor(dst, RGB(0, 0, 0));
		SetTextColor(dst, RGB(255, 255, 255));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, SRCAND);
		HBRUSH brush = CreateSolidBrush(shaded_shadow_mapped_pixel);
		HBRUSH old_brush = HBRUSH(SelectObject(dst, brush));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, PSDPxax);
		SelectObject(dst, old_brush);
		DeleteObject(brush);
	}
	if (map_shaded_foreground) {
		SetBkColor(original, shaded_foreground_pixel);
		BitBlt(mask, 0, 0, w, h, original, 0, 0, SRCCOPY);
		SetBkColor(dst, RGB(0, 0, 0));
		SetTextColor(dst, RGB(255, 255, 255));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, SRCAND);
		HBRUSH brush = CreateSolidBrush(shaded_foreground_mapped_pixel);
		HBRUSH old_brush = HBRUSH(SelectObject(dst, brush));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, PSDPxax);
		SelectObject(dst, old_brush);
		DeleteObject(brush);
	}
	if (map_flat_background) {
		SetBkColor(original, flat_background_pixel);
		BitBlt(mask, 0, 0, w, h, original, 0, 0, SRCCOPY);
		SetBkColor(dst, RGB(0, 0, 0));
		SetTextColor(dst, RGB(255, 255, 255));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, SRCAND);
		HBRUSH brush = CreateSolidBrush(flat_background_mapped_pixel);
		HBRUSH old_brush = HBRUSH(SelectObject(dst, brush));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, PSDPxax);
		SelectObject(dst, old_brush);
		DeleteObject(brush);
	}
	if (map_flat_background_select) {
		SetBkColor(original, flat_background_select_pixel);
		BitBlt(mask, 0, 0, w, h, original, 0, 0, SRCCOPY);
		SetBkColor(dst, RGB(0, 0, 0));
		SetTextColor(dst, RGB(255, 255, 255));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, SRCAND);
		HBRUSH brush = CreateSolidBrush(flat_background_select_mapped_pixel);
		HBRUSH old_brush = HBRUSH(SelectObject(dst, brush));
		BitBlt(dst, 0, 0, w, h, mask, 0, 0, PSDPxax);
		SelectObject(dst, old_brush);
		DeleteObject(brush);
	}

	bitmap->ungetHndl();
	originalBitmap->ungetHndl();
	mask_bitmap->ungetHndl();
	delete mask_bitmap;

#elif defined(CV_MOTIF)

	HDC dst = bitmap->getHndl();
	XImage *image = XGetImage(dst->display, dst->drawable,
							  0, 0, w, h, AllPlanes, ZPixmap);
	if (image == 0) {
		bitmap->ungetHndl();
		return;
	}

	for (int x = 0 ; x < w ; x++) {
		for (int y = 0 ; y < h ; y++) {
			long pixel = XGetPixel(image, x, y);
			if (map_shaded_background &&
				(pixel == shaded_background_pixel)) {
				XPutPixel(image, x, y, shaded_background_mapped_pixel);
			}
			else if (map_shaded_highlight &&
					 (pixel == shaded_highlight_pixel)) {
				XPutPixel(image, x, y, shaded_highlight_mapped_pixel);
			}
			else if (map_shaded_shadow &&
					 (pixel == shaded_shadow_pixel)) {
				XPutPixel(image, x, y, shaded_shadow_mapped_pixel);
			}
			else if (map_shaded_foreground &&
					 (pixel == shaded_foreground_pixel)) {
				XPutPixel(image, x, y, shaded_foreground_mapped_pixel);
			}
			else if (map_flat_background &&
					 (pixel == flat_background_pixel)) {
				XPutPixel(image, x, y, flat_background_mapped_pixel);
			}
			else if (map_flat_background_select &&
					 (pixel == flat_background_select_pixel)) {
				XPutPixel(image, x, y, flat_background_select_mapped_pixel);
			}
		}
	}

	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XPutImage(dst->display, dst->drawable, gc, image, 0, 0, 0, 0, w, h);
	XFreeGC(dst->display, gc);
	XDestroyImage(image);
	bitmap->ungetHndl();

#endif

}

// ---------------------------------------------------------------------------
//
void VImage::mapColor(const VColor& from, const VColor& to)
{
	if (!constructMappedBitmap()) {
		return;
	}

	int	w = getWidth(), h = getHeight();
	long from_pixel	= long(from), to_pixel = long(to);

#if defined(CV_WINDOWS)

	const unsigned long PSDPxax = 0x00B8074A;  // P xor (S and (D xor P))

	VBitMap	   *mask_bitmap	= new VBitMap(w, h, TRUE);
	HDC			mask		= mask_bitmap->getHndl();
	HDC			original	= originalBitmap->getHndl();
	HDC			dst			= mappedBitmap->getHndl();

	SetBkColor(original, from_pixel);
	BitBlt(mask, 0, 0, w, h, original, 0, 0, SRCCOPY);
	SetBkColor(dst, RGB(0, 0, 0));
	SetTextColor(dst, RGB(255, 255, 255));
	BitBlt(dst, 0, 0, w, h, mask, 0, 0, SRCAND);
	HBRUSH brush = CreateSolidBrush(to_pixel);
	HBRUSH old_brush = HBRUSH(SelectObject(dst, brush));
	BitBlt(dst, 0, 0, w, h, mask, 0, 0, PSDPxax);
	SelectObject(dst, old_brush);
	DeleteObject(brush);

	mappedBitmap->ungetHndl();
	originalBitmap->ungetHndl();
	mask_bitmap->ungetHndl();
	delete mask_bitmap;

#elif defined(CV_MOTIF)

	HDC dst = mappedBitmap->getHndl();
	XImage *image = XGetImage(dst->display, dst->drawable,
							  0, 0, w, h, AllPlanes, ZPixmap);
	if (image == 0) {
		mappedBitmap->ungetHndl();
		return;
	}

	for (int x = 0 ; x < w ; x++) {
		for (int y = 0 ; y < h ; y++) {
			long pixel = XGetPixel(image, x, y);
			if (pixel == from_pixel) {
				XPutPixel(image, x, y, to_pixel);
			}
		}
	}

	GC gc = XCreateGC(dst->display, dst->drawable, 0, 0);
	XPutImage(dst->display, dst->drawable, gc, image, 0, 0, 0, 0, w, h);
	XFreeGC(dst->display, gc);
	XDestroyImage(image);
	mappedBitmap->ungetHndl();

#endif

}

// ---------------------------------------------------------------------------
//
VBitMap *VImage::copyOriginalBitmap() const
{
	if (!retrieveOriginalBitmap()) {
		return 0;
	}
	int w = getWidth();
	int h = getHeight();
	VBitMap *bitmap = new VBitMap(w, h, originalBitmap);
	if (bitmap == 0) {
		return 0;
	}
	VBitBltr bb(originalBitmap, bitmap);
	bb.bitbltr(0, 0, w, h, 0, 0, CopyRule);
	return bitmap;
}

// ---------------------------------------------------------------------------
//
VBitMap *VImage::copyBitmap(const VBitMap *bitmap)
{
	if ((bitmap == 0) || !((VBitMap *)bitmap)->isValid()) {
		return 0;
	}
	int w, h;
	((VBitMap *)bitmap)->sizeOfImage(&w, &h);
	VBitMap *new_bitmap = new VBitMap(w, h, (VBitMap *)bitmap);
	if (new_bitmap == 0) {
		return 0;
	}
	VBitBltr bb((VBitMap *)bitmap, new_bitmap);
	bb.bitbltr(0, 0, w, h, 0, 0, CopyRule);
	return new_bitmap;
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VImage::putTo(VArchiver& a)
{
	// Write the base object data.

	VObject::putTo(a);

	// Write the object version tag.

	a << VImageArchiveTag;

	// Write any object specific data.

	a << short(imageMajorCount);
	a << short(imageMinorCount);
	a << short(imageMajorSize);
	a << short(imageMinorSize);

	if (originalBitmapFromVrf) {
		a << char(1);
		a.putObject(originalBitmapName);
		a.putObject(originalBitmapVrfName);
	}
	else if (originalBitmapFromNativeResource) {
		a << char(2);
		a.putObject(originalBitmapName);
	}
	else if (originalBitmapFromBitmapFile) {
		a << char(3);
		a.putObject(originalBitmapName);
	}
	else if (originalBitmap != 0) {
		a << char(4);
		a.putObject(originalBitmap);
	}
	else {
		a << char(0);
	}

	if (transparentColor != 0) {
		a << char(1);
		a << short(transparentColor->getRed());
		a << short(transparentColor->getGreen());
		a << short(transparentColor->getBlue());
	}
	else {
		a << char(0);
	}

	if (additionalDisabledMaskColor != 0) {
		a << char(1);
		a << short(additionalDisabledMaskColor->getRed());
		a << short(additionalDisabledMaskColor->getGreen());
		a << short(additionalDisabledMaskColor->getBlue());
	}
	else {
		a << char(0);
	}

	a << char(amReadingTransparentColorFromUpperLeft);
	a << char(amMappingShadedBackgroundColor);
	a << char(amMappingShadedHighlightColor);
	a << char(amMappingShadedShadowColor);
	a << char(amMappingShadedForegroundColor);
	a << char(amMappingFlatBackgroundColor);
	a << char(amMappingFlatBackgroundSelectColor);
}

// ---------------------------------------------------------------------------
//
void VImage::getFrom(VArchiver& a)
{
	// Read the base object data.

	VObject::getFrom(a);

	// Read object version tag and verify.

	long tag; a >> tag;
	if (!archiveTagOk(tag, VImageArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VImageArchiveTag));
		return;
	}

	// Read any object specific data.

	a >> imageMajorCount;
	a >> imageMinorCount;
	a >> imageMajorSize;
	a >> imageMinorSize;

	char c;

	a >> c;

	if (c == 1) {
		originalBitmapFromVrf = 1;
		originalBitmapName = (VString *)a.getObject(0);
		originalBitmapVrfName = (VPathString *)a.getObject(0);
	}
	else if (c == 2) {
		originalBitmapFromNativeResource = 1;
		originalBitmapName = (VString *)a.getObject(0);
	}
	else if (c == 3) {
		originalBitmapFromBitmapFile = 1;
		originalBitmapName = (VString *)a.getObject(0);
	}
	else if (c == 4) {
		originalBitmap = (VBitMap *)a.getObject(0);
	}

	a >> c;
	if (c != 0) {
		short r, g, b;
		a >> r; a >> g; a >> b;
		transparentColor = new VColor(r, g, b);
		transparentColorIsMine = 1;
	}

	a >> c;
	if (c != 0) {
		short r, g, b;
		a >> r; a >> g; a >> b;
		additionalDisabledMaskColor = new VColor(r, g, b);
	}

	a >> c; amReadingTransparentColorFromUpperLeft = c;
	a >> c; amMappingShadedBackgroundColor = c;
	a >> c; amMappingShadedHighlightColor = c;
	a >> c; amMappingShadedShadowColor = c;
	a >> c; amMappingShadedForegroundColor = c;
	a >> c; amMappingFlatBackgroundColor = c;
	a >> c; amMappingFlatBackgroundSelectColor = c;
}

// ---------------------------------------------------------------------------
//
void VImage::getFrom(VArchiver& a, VObject *, boolean)
{
	VImage::getFrom(a);
}

// ---------------------------------------------------------------------------
//
boolean VImage::loadResources(VRscArchiver& ra)
{
	return retrieveOriginalBitmap(&ra);
}

#endif // !CV_NOARCHIVER
