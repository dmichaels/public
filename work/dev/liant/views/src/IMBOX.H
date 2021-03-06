//	imbox.h
//
//	VImageBox interface [Common]
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
//	08/24/96 dgm	Original.
//	01/13/97 djs	#ifdef'd ::getPalette for OS/2 - TEMP
// --------------------------------------------------------------------------

#ifndef VIMBOX_H
#define VIMBOX_H

// --------------------------------------------------------------------------
//
#include "control.h"
#include "image.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VControl;
CLASS	VFont;
CLASS	VImage;
CLASS	VObject;
CLASS	VPort;
CLASS	VString;
CLASS	VPathString;
CLASS	VWindow;

#ifndef CV_NOARCHIVER
CLASS	VArchiver;
CLASS	VRscArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VImageBox : public VControl {
public:

#	if defined(Above)
#		undef Above
#	endif
#	if defined(Below)
#		undef Below
#	endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	enum Placement {	Above				=  1,
						Left				=  2,
						Below				=  3,
						Right				=  4
	};

	enum Border {		NoBorder			=  1,
						RaisedThinBorder	=  2,
						RaisedThickBorder	=  3,
						SunkenThinBorder	=  4,
						SunkenThickBorder	=  5,
						EtchedInBorder		=  6,
						EtchedOutBorder		=  7,
						FlatBorder			=  8,
						FlatThickBorder		=  9,
						FlatDoubleBorder	= 10,
						RaisedBorder		= RaisedThinBorder,
						SunkenBorder		= SunkenThinBorder
	};
		
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

public:
					VImageBox(VWindow *, const VStyle& = StyleDefault);
					VImageBox(const VFrame&, VWindow *,
							  const VStyle& = StyleDefault);
					VImageBox(const VString&,
							  VWindow *, const VStyle& = StyleDefault);
					VImageBox(const VString&,
							  const VFrame&, VWindow *,
							  const VStyle& = StyleDefault);
					VImageBox(VWindow *, VImage *, int index = 0,
							  const VStyle& = StyleDefault);
					VImageBox(const VString&,
							  VWindow *, VImage *, int index = 0,
							  const VStyle& = StyleDefault);
					VImageBox();
				   ~VImageBox();
	VClass		   *iam();

	void			operator()(VWindow *);
	void			operator()(VWindow *, const VStyle&);
	void			operator()(VWindow *, const VStyle&, const VString&);
	void			operator()(const VFrame&, VWindow *);
	void			operator()(const VFrame&, VWindow *, const VStyle&);
	void			operator()(const VFrame&, VWindow *, const VStyle&,
							   const VString&);

	// Set operations.

	boolean			setImage(VImage *, int index = 0);
	boolean			setImage(const VString&
								   native_resource_or_bitmap_file_name,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const char *
								   native_resource_or_bitmap_file_name,
						     int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const VPathString& bitmap_file_name,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const VString& vrf_bitmap_name,
							 const VPathString& vrf_file_name,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const VString& vrf_bitmap_name,
							 const VString& vrf_file_name,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const VString& vrf_bitmap_name,
							 const char *vrf_file_name,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const char *vrf_bitmap_name,
							 const VPathString& vrf_file_name,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const char *vrf_bitmap_name,
							 const VString& vrf_file_name,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const char *vrf_bitmap_name,
							 const char *vrf_file_name,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const VBitMap&,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(const VBitMap *,
							 int major_count = 1, int minor_count = 1,
							 int index = 0);
	boolean			setImage(int = 0);

	void			setImageIndex(int);
	void			setImageIndex(int, int);
	void			setImageMajorIndex(int);
	void			setImageMinorIndex(int);

	void			putText(const VString&);
	void			setText(const VString&);
	void			setTextPlacement(Placement);
	void			setTextImageGap(int);
	void			setTextImageDisplacement(int);
	void			setTextImageDisplacement(int, int);

	void			displayText(boolean = TRUE);
	void			displayImage(boolean = TRUE);

	void			setBorderStyle(int);
	void			hideBorder(boolean = TRUE);
	void			showBorder(boolean = TRUE);

	void			setMargins(int);
	void			setMargins(int, int);
	void			setMargins(int, int, int, int);

	void			setImageDrawMode(int);
	void			useTheImageDrawMode(boolean);

	void			setImmediatePaintUpdate(boolean);
	void			setImmediateGeometryUpdate(boolean);
	void			setImmediateUpdate(boolean);

	virtual void	disable(boolean = TRUE);
	virtual void	enable(boolean = TRUE);

	// Get operations.

	VImage		   *getImage() const;
	int				getImageIndex() const;
	int				getImageMajorIndex() const;
	int				getImageMinorIndex() const;
	int				getImageCount() const;
	int				getImageMajorCount() const;
	int				getImageMinorCount() const;

	boolean			isEnabled() const;
	boolean			isDisabled() const;

	boolean			isDisplayingText() const;
	boolean			isDisplayingImage() const;

	int				getBorderStyle() const;
	int				getBorderThickness() const;
	boolean			isBorderHidden() const;
	boolean			isBorderShowing() const;

	Placement		getTextPlacement() const;
	int				getTextImageGap() const;
	void			getTextImageDisplacement(int&, int&) const;

	void			getMargins(int&, int&, int&, int&) const;
	int				getLeftMargin() const;
	int				getRightMargin() const;
	int				getTopMargin() const;
	int				getBottomMargin() const;
	int				getHorizontalMarginThickness() const;
	int				getVerticalMarginThickness() const;

	int				getImageDrawMode() const;
	boolean			usingTheImageDrawMode() const;

	boolean			isImmediatePaintUpdate() const;
	boolean			isImmediateGeometryUpdate() const;
	boolean			isImmediateUpdate() const;

	void			repaint();
	void			repaintBackgroundAndContents();
	void			repaintContents();
	void			repaintContents(boolean);
	void			repaintBorder();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
	void				initialize();
	void				construct();
	void				destruct();
	int					getImageWidth() const;
	int					getImageHeight() const;
	int					getTextWidth() const;
	int					getTextHeight() const;
	int					getWidth() const;
	int					getHeight() const;
	int					getClientAreaWidth() const;
	int					getClientAreaHeight() const;
	int					getClientAreaX() const;
	int					getClientAreaY() const;
	int					getNonClientAreaHorizontalThickness() const;
	int					getNonClientAreaVerticalThickness() const;

protected:
	virtual boolean		paint();
	virtual boolean		resized(int, int);
    virtual void		getPreferredSize(int *, int *) const;
    virtual void		getInternalBorderGeometry(int *, int *,
												  int *, int *) const;
	virtual boolean		erased();
    virtual void    	enableBool(boolean);
    virtual void		setFont(VFont *);
	virtual void		systemColorChanged();
	#ifndef PM
	virtual HPALETTE	getPalette();
	#endif

protected:
	virtual void		paintBorder(VPort&);
	virtual void		paintBackground(VPort&);
	virtual void		paintContents(VPort&);
	void				paintSunkenBorder(VPort&);
	void				paintSunkenThickBorder(VPort&);
	void				paintRaisedBorder(VPort&);
	void				paintRaisedThickBorder(VPort&);
	void				paintEtchedInBorder(VPort&);
	void				paintEtchedOutBorder(VPort&);
	void				paintFlatBorder(VPort&);
	void				paintFlatThickBorder(VPort&);
	void				paintFlatDoubleBorder(VPort&);
	void				paintNullBorder(VPort&);
	void				calculateTextSize();

protected:
	static void			constructStatic();
	static void			destructStatic();

#ifndef CV_NOARCHIVER
public:
	virtual void		putTo(VArchiver&);
	void				putTo(VArchiver&, boolean);
	virtual void		getFrom(VArchiver&);
	virtual void		getFrom(VArchiver&, VObject *, boolean = TRUE);
	virtual void		realize(VWindow *);
	virtual boolean		loadResources(VRscArchiver&);
protected:
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
						VImageBox(const VImageBox&) {}
	VImageBox&			operator=(const VImageBox&) { return *this; }

private:
	VImage			   *theImage;
	short				imageMajorIndex;
	short				imageMinorIndex;
	short				textWidth;
	short				textHeight;
	short				currentWidth;
	short				currentHeight;
	short				drawMode;
	unsigned char		borderStyle;
	unsigned char		borderThickness;
	unsigned char		leftMargin;
	unsigned char		rightMargin;
	unsigned char		topMargin;
	unsigned char		bottomMargin;
	unsigned char		textPlacement;
	unsigned char		textImageGap;
	unsigned char		textImageDisplacementX;
	unsigned char		textImageDisplacementY;
	unsigned char		currentlyDisabled : 1;
	unsigned char		currentlyToggledIn : 1;
	unsigned char		currentlyDisplayingImage : 1;
	unsigned char		currentlyDisplayingText : 1;
	unsigned char		currentlyShowingBorder : 1;
	unsigned char		imageIsMine : 1;
	unsigned char		amUsingTheImageDrawMode : 1;
	unsigned char		immediatePaintUpdate : 1;
	unsigned char		immediateGeometryUpdate : 1;
	unsigned char		receivedPaint : 1;
};

extern VClass *VImageBoxCls;

// --------------------------------------------------------------------------
//
inline void VImageBox::setImageIndex(int major_index)
{
	setImageIndex(major_index, imageMinorIndex);
}

inline void VImageBox::setImageMajorIndex(int major_index)
{
	setImageIndex(major_index, imageMinorIndex);
}

inline void VImageBox::setImageMinorIndex(int minor_index)
{
	setImageIndex(imageMajorIndex, minor_index);
}

inline void VImageBox::putText(const VString& s)
{
	setText(s);
}

inline VImage *VImageBox::getImage() const
{
	return theImage;
}

inline int VImageBox::getImageIndex() const
{
	return imageMajorIndex;
}

inline int VImageBox::getImageMajorIndex() const
{
	return imageMajorIndex;
}

inline int VImageBox::getImageMinorIndex() const
{
	return imageMinorIndex;
}

inline int VImageBox::getImageCount() const
{
	return getImageMajorCount();
}

inline boolean VImageBox::isDisabled() const
{
	return currentlyDisabled;
}

inline boolean VImageBox::isEnabled() const
{
	return !currentlyDisabled;
}

inline boolean VImageBox::isDisplayingImage() const
{
	return currentlyDisplayingImage;
}

inline boolean VImageBox::isDisplayingText() const
{
	return currentlyDisplayingText;
}

inline VImageBox::Placement VImageBox::getTextPlacement() const
{
	return Placement(textPlacement);
}

inline int VImageBox::getImageWidth() const
{
	return theImage != 0 ? theImage->getItemWidth() : 0;
}

inline int VImageBox::getImageHeight() const
{
	return theImage != 0 ? theImage->getItemHeight() : 0;
}

inline int VImageBox::getTextWidth() const
{
	return textWidth;
}

inline int VImageBox::getTextHeight() const
{
	return textHeight;
}

inline int VImageBox::getWidth() const
{
	return currentWidth;
}

inline int VImageBox::getHeight() const
{
	return currentHeight;
}

inline int VImageBox::getClientAreaWidth() const
{
	return currentWidth - borderThickness - borderThickness;
}

inline int VImageBox::getClientAreaHeight() const
{
	return currentHeight - borderThickness - borderThickness;
}

inline int VImageBox::getClientAreaX() const
{
	return borderThickness;
}

inline int VImageBox::getClientAreaY() const
{
	return borderThickness;
}

inline int VImageBox::getNonClientAreaHorizontalThickness() const
{
	return borderThickness + borderThickness;
}

inline int VImageBox::getNonClientAreaVerticalThickness() const
{
	return borderThickness + borderThickness;
}

inline int VImageBox::getTextImageGap() const
{
	return textImageGap;
}

inline void VImageBox::getTextImageDisplacement(int& dx, int& dy) const
{
	dx = textImageDisplacementX;
	dy = textImageDisplacementY;
}

inline int VImageBox::getBorderStyle() const
{
	return borderStyle;
}

inline int VImageBox::getBorderThickness() const
{
	return borderThickness;
}

inline void VImageBox::hideBorder(boolean b)
{
	showBorder(!b);
}

inline boolean VImageBox::isBorderHidden() const
{
	return !currentlyShowingBorder;
}

inline boolean VImageBox::isBorderShowing() const
{
	return currentlyShowingBorder;
}

inline void VImageBox::getMargins(int& l, int& r, int& t, int& b) const
{
	l = leftMargin;
	r = rightMargin;
	t = topMargin;
	b = bottomMargin;
}

inline int VImageBox::getLeftMargin() const
{
	return leftMargin;
}

inline int VImageBox::getRightMargin() const
{
	return rightMargin;
}

inline int VImageBox::getTopMargin() const
{
	return topMargin;
}

inline int VImageBox::getBottomMargin() const
{
	return bottomMargin;
}

inline int VImageBox::getHorizontalMarginThickness() const
{
	return leftMargin + rightMargin;
}

inline int VImageBox::getVerticalMarginThickness() const
{
	return topMargin + bottomMargin;
}

inline void VImageBox::setImmediatePaintUpdate(boolean b)
{
	immediatePaintUpdate = b;
}

inline void VImageBox::setImmediateGeometryUpdate(boolean b)
{
	immediateGeometryUpdate = b;
}

inline void VImageBox::setImmediateUpdate(boolean b)
{
	immediatePaintUpdate = b;
	immediateGeometryUpdate = b;
}

inline void VImageBox::enable(boolean b)
{
	disable(!b);
}

inline int VImageBox::getImageDrawMode() const
{
	return drawMode;
}

inline boolean VImageBox::isImmediatePaintUpdate() const
{
	return immediatePaintUpdate;
}

inline boolean VImageBox::isImmediateGeometryUpdate() const
{
	return immediateGeometryUpdate;
}

inline boolean VImageBox::isImmediateUpdate() const
{
	return immediatePaintUpdate && immediateGeometryUpdate;
}

inline boolean VImageBox::usingTheImageDrawMode() const
{
	return amUsingTheImageDrawMode;
}

#endif // VIMBOX_H
