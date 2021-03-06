//	image.h
//
//	VImage interface [Common]
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
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VIMAGE_H
#define VIMAGE_H

// --------------------------------------------------------------------------
//
#include "object.h"

// --------------------------------------------------------------------------
//
CLASS	VBitMap;
CLASS	VClass;
CLASS	VColor;
CLASS	VControl;
CLASS	VDisplay;
CLASS	VObject;
CLASS	VPathString;
CLASS	VPort;
CLASS	VString;

#ifndef CV_NOARCHIVER
CLASS	VArchiver;
CLASS	VRscArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VImage : public VObject {
public:
	enum DrawMode {	Mapped				= 0x0001,
					Unmapped			= 0x0002,
					Disabled			= 0x0004,
					Mask				= 0x0008,
					DisabledMask		= 0x0010,
					GrayScale			= 0x0020,

					Transparent			= 0x0100,
					Opaque				= 0x0200,

					Focused				= 0x0400,
					Selected			= 0x0800,
					BlendedLight		= 0x1000,
					BlendedDark			= 0x2000,

					MappedTransparent	= Mapped | Transparent,
					MappedOpaque		= Mapped | Opaque,
					UnmappedTransparent	= Unmapped | Transparent,
					UnmappedOpaque		= Unmapped | Opaque,

					Default				= UnmappedOpaque
	};

public:
					VImage(const VString&
						   native_resource_or_bitmap_file_name,
						   int major_count = 1, int minor_count = 1);
					VImage(const char *native_resource_or_bitmap_file_name,
						   int major_count = 1, int minor_count = 1);
					VImage(const VPathString& bitmap_file_name,
						   int major_count = 1, int minor_count = 1);
					VImage(const VString& vrf_bitmap_name,
						   const VPathString& vrf_file_name,
						   int major_count = 1, int minor_count = 1);
					VImage(const VString& vrf_bitmap_name,
						   const VString& vrf_file_name,
						   int major_count = 1, int minor_count = 1);
					VImage(const VString& vrf_bitmap_name,
						   const char *vrf_file_name,
						   int major_count = 1, int minor_count = 1);
					VImage(const char *vrf_bitmap_name,
						   const VPathString& vrf_file_name,
						   int major_count = 1, int minor_count = 1);
					VImage(const char *vrf_bitmap_name,
						   const VString& vrf_file_name,
						   int major_count = 1, int minor_count = 1);
					VImage(const char *vrf_bitmap_name,
						   const char *vrf_file_name,
						   int major_count = 1, int minor_count = 1);
					VImage(const VBitMap&,
						   int major_count = 1, int minor_count = 1);
					VImage(const VBitMap *,
						   int major_count = 1, int minor_count = 1);
					VImage(const VImage&);
					VImage();
				   ~VImage();
	VClass		   *iam();

	// Set operations.

	VImage&			operator=(const VImage&);

	boolean			setImage(const VString&
							 native_resource_or_bitmap_file_name,
						     int major_count = 1, int minor_count = 1,
							 boolean load_bitmap_now = TRUE);
	boolean			setImage(const char *native_resource_or_bitmap_file_name,
						     int major_count = 1, int minor_count = 1,
							 boolean load_bitmap_now = TRUE);
	boolean			setImage(const VPathString& bitmap_file_name,
						     int major_count = 1, int minor_count = 1,
							 boolean load_bitmap_now = TRUE);
	boolean			setImage(const VString& vrf_bitmap_name,
							 const VPathString& vrf_file_name,
						     int major_count = 1, int minor_count = 1,
							 boolean load_bitmap_now = TRUE);
	boolean			setImage(const VString& vrf_bitmap_name,
							 const VString& vrf_file_name,
						     int major_count = 1, int minor_count = 1,
							 boolean load_bitmap_now = TRUE);
	boolean			setImage(const VString& vrf_bitmap_name,
							 const char *vrf_file_name,
						     int major_count = 1, int minor_count = 1,
							 boolean load_bitmap_now = TRUE);
	boolean			setImage(const char *vrf_bitmap_name,
							 const VPathString& vrf_file_name,
						     int major_count = 1, int minor_count = 1,
							 boolean load_bitmap_now = TRUE);
	boolean			setImage(const char *vrf_bitmap_name,
							 const VString& vrf_file_name,
						     int major_count = 1, int minor_count = 1,
							 boolean load_bitmap_now = TRUE);
	boolean			setImage(const char *vrf_bitmap_name,
							 const char *vrf_file_name,
						     int major_count = 1, int minor_count = 1,
							 boolean load_bitmap_now = TRUE);
	boolean			setImage(const VBitMap&,
						     int major_count = 1, int minor_count = 1);
	boolean			setImage(const VBitMap *,
						     int major_count = 1, int minor_count = 1);

	void			setItemCount(int major_count, int minor_count = 1);

	void			setTransparentColor(const VColor&);
	void			setAdditionalDisabledMaskColor(const VColor&);
	void			setAdditionalDisabledMaskColor();

	void			readTransparentColorFromUpperLeft(boolean);
	void			mapColors(boolean = TRUE);
	void			mapShadedColors(boolean = TRUE);
	void			mapShadedBackgroundColor(boolean = TRUE);
	void			mapShadedHighlightColor(boolean = TRUE);
	void			mapShadedShadowColor(boolean = TRUE);
	void			mapShadedForegroundColor(boolean = TRUE);
	void			mapFlatBackgroundColor(boolean = TRUE);
	void			mapFlatBackgroundSelectColor(boolean = TRUE);
	void			mapColor(const VColor& from, const VColor& to);

	void			setDrawMode(int);

	// Actions.

	void			draw(VPort *, int x, int y,
						 int major_index = 0,
						 int minor_index = 0, int style = 0);
	void			draw(VDisplay *, int x, int y,
						 int major_index = 0,
						 int minor_index = 0, int style = 0);

	// Get operations.

	int				getItemWidth() const;
	int				getItemHeight() const;
	void			getItemSize(int& w, int& h) const;
	void			getItemSize(int *w, int *h) const;
	void			getItemGeometry(int& x, int& y, int& w, int& h,
									int major_index = 0,
									int minor_index = 0);
	void			getItemGeometry(int *x, int *y, int *w, int *h,
									int major_index = 0,
									int minor_index = 0);

	int				getItemCount() const;
	int				getItemMajorCount() const;
	int				getItemMinorCount() const;

	int				getWidth() const;
	int				getHeight() const;
	void			getSize(int& w, int& h) const;
	void			getSize(int *w, int *h) const;
	int				getDepth() const;

	const VColor&	getTransparentColor() const;
	const VColor&	getShadedBackgroundColor() const;
	const VColor&	getShadedHighlightColor() const;
	const VColor&	getShadedShadowColor() const;
	const VColor&	getShadedForegroundColor() const;
	const VColor&	getFlatBackgroundColor() const;
	const VColor&	getFlatBackgroundSelectColor() const;
	const VColor&	getUpperLeftPixelColor() const;

	boolean			readingTransparentColorFromUpperLeft() const;
	boolean			mappingShadedColors() const;
	boolean			mappingShadedBackgroundColor() const;
	boolean			mappingShadedHighlightColor() const;
	boolean			mappingShadedShadowColor() const;
	boolean			mappingShadedForegroundColor() const;
	boolean			mappingFlatBackgroundColor() const;
	boolean			mappingFlatBackgroundSelectColor() const;

	int				getDrawMode() const;

	VBitMap		   *getOriginalBitmap() const;
	VBitMap		   *getMappedBitmap() const;
	boolean			isValid() const;

	// Notifications.

	void			systemColorChanged();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
	void			initialize();
	void			copy(const VImage&);
	boolean			retrieveOriginalBitmap(VObject * = 0) const;
	boolean			constructAndMaskBitmap();
	boolean			constructDisabledAndMaskBitmap();
	boolean			constructMappedBitmap();
	boolean			constructMappedOrBitmap();
	boolean			constructUnmappedOrBitmap();
	boolean			constructGrayScaleBitmap();
	boolean			constructGrayScaleOrBitmap();
	void			destruct();
	void			destructColors();
	void			destructBitmaps();
	void			drawMappedTransparent
						(VDisplay *, int, int, int = 0, int = 0);
	void			drawMappedOpaque
						(VDisplay *, int, int, int = 0, int = 0);
	void			drawUnmappedTransparent
						(VDisplay *, int, int, int = 0, int = 0);
	void			drawUnmappedOpaque
						(VDisplay *, int, int, int = 0, int = 0);
	void			drawGrayScaleTransparent
						(VDisplay *, int, int, int = 0, int = 0);
	void			drawGrayScaleOpaque
						(VDisplay *, int, int, int = 0, int = 0);
	void			drawDisabled
						(VDisplay *, int, int, int = 0, int = 0);
	void			drawMask
						(VDisplay *, int, int, int = 0, int = 0);
	void			drawDisabledMask
						(VDisplay *, int, int, int = 0, int = 0);
	void			performColorMapping(VBitMap *, boolean = TRUE);
	VBitMap		   *copyOriginalBitmap() const;

protected:
	static void		constructStatic();
	static void		destructStatic();
	static VBitMap *copyBitmap(const VBitMap *);

public:

#ifndef CV_NOARCHIVER
	virtual void	putTo(VArchiver&);
	virtual void	getFrom(VArchiver&);
	virtual void	getFrom(VArchiver&, VObject *, boolean = TRUE);
	boolean			loadResources(VRscArchiver&);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section; please KEEP OUT!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	VBitMap		   *originalBitmap;
	VBitMap		   *mappedBitmap;
	VBitMap		   *mappedOrBitmap;
	VBitMap		   *unmappedOrBitmap;
	VBitMap		   *andMaskBitmap;
	VBitMap		   *disabledAndMaskBitmap;
	VBitMap		   *grayScaleOrBitmap;
	VBitMap		   *grayScaleBitmap;
	VColor		   *transparentColor;
	VColor		   *additionalDisabledMaskColor;
	VColor		   *upperLeftPixelColor;
	VString		   *originalBitmapName;
	VString		   *originalBitmapVrfName;
	short			imageMajorCount;
	short			imageMinorCount;
	short			imageMajorSize;
	short			imageMinorSize;
	short			drawMode;
	int				amMappingShadedBackgroundColor;
	int				amMappingShadedHighlightColor;
	int				amMappingShadedShadowColor;
	int				amMappingShadedForegroundColor;
	int				amMappingFlatBackgroundColor;
	int				amMappingFlatBackgroundSelectColor;
	int				amReadingTransparentColorFromUpperLeft : 1;
	int				transparentColorIsMine : 1;
	int				originalBitmapIsMine : 1;
	int				originalBitmapFromVrf : 1;
	int				originalBitmapFromBitmapFile : 1;
	int				originalBitmapFromNativeResource : 1;
};

extern VClass *VImageCls;

// --------------------------------------------------------------------------
//
inline boolean VImage::readingTransparentColorFromUpperLeft() const
{
	return amReadingTransparentColorFromUpperLeft;
}

inline boolean VImage::mappingShadedColors() const
{
	return amMappingShadedBackgroundColor &&
		   amMappingShadedHighlightColor &&
		   amMappingShadedShadowColor;
}

inline boolean VImage::mappingShadedBackgroundColor() const
{
	return amMappingShadedBackgroundColor;
}

inline boolean VImage::mappingShadedHighlightColor() const
{
	return amMappingShadedHighlightColor;
}

inline boolean VImage::mappingShadedShadowColor() const
{
	return amMappingShadedShadowColor;
}

inline boolean VImage::mappingShadedForegroundColor() const
{
	return amMappingShadedForegroundColor;
}

inline boolean VImage::mappingFlatBackgroundColor() const
{
	return amMappingFlatBackgroundColor;
}

inline boolean VImage::mappingFlatBackgroundSelectColor() const
{
	return amMappingFlatBackgroundSelectColor;
}

inline int VImage::getItemCount() const
{
	return imageMajorCount;
}

inline int VImage::getItemMajorCount() const
{
	return imageMajorCount;
}

inline int VImage::getItemMinorCount() const
{
	return imageMinorCount;
}

inline int VImage::getItemWidth() const
{
	if ((originalBitmap != 0) || retrieveOriginalBitmap()) {
		return imageMajorSize;
	}
	else {
		return 0;
	}
}

inline int VImage::getItemHeight() const
{
	if ((originalBitmap != 0) || retrieveOriginalBitmap()) {
		return imageMinorSize;
	}
	else {
		return 0;
	}
}

inline void VImage::getItemSize(int& w, int& h) const
{
	if ((originalBitmap != 0) || retrieveOriginalBitmap()) {
		w = imageMajorSize;
		h = imageMinorSize;
	}
	else {
		w = h = 0;
	}
}

inline void VImage::getItemSize(int *w, int *h) const
{
	if ((originalBitmap != 0) || retrieveOriginalBitmap()) {
		if (w != 0) { *w = imageMajorSize; }
		if (h != 0) { *h = imageMinorSize; }
	}
	else {
		if (w != 0) { *w = 0; }
		if (h != 0) { *h = 0; }
	}
}

inline int VImage::getWidth() const
{
	return getItemWidth() * getItemMajorCount();
}

inline int VImage::getHeight() const
{
	return getItemHeight() * getItemMinorCount();
}

inline void VImage::getSize(int& w, int& h) const
{
	w = getItemWidth() * getItemMajorCount();
	h = getItemHeight() * getItemMinorCount();
}

inline void VImage::getSize(int *w, int *h) const
{
	if (w != 0) { *w = getItemWidth() * getItemMajorCount(); }
	if (h != 0) { *h = getItemHeight() * getItemMinorCount(); }
}

inline void VImage::setDrawMode(int mode)
{
	drawMode = mode;
}

inline int VImage::getDrawMode() const
{
	return drawMode;
}

inline VBitMap *VImage::getOriginalBitmap() const
{
	if (originalBitmap == 0) {
		retrieveOriginalBitmap();
	}
	return originalBitmap;
}

inline VBitMap *VImage::getMappedBitmap() const
{
	if (mappedBitmap == 0) {
		((VImage *)this)->constructMappedBitmap();
	}
	return mappedBitmap;
}

#endif // VIMAGE_H
