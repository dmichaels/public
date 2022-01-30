//	imbutton.cpp
//
//	VImageButton implementation [Common]
//
//  Allegris Foundation 1.1.00
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of this    |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	04/01/96 dgm	Original.
// --------------------------------------------------------------------------

#include "imbutton.h"
#include "imbox.h"
#include "image.h"
#include "rect.h"
#include "clrscm.h"
#include "color.h"
#include "brush.h"
#include "pen.h"
#include "port.h"
#include "str.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "cllbckls.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VImageButton, VImageBox)
defineArchiveRevision(VImageButton, 0)

// --------------------------------------------------------------------------
//
static unsigned long	objectCount			= 0;
static VBrush		   *lightStippleBrush	= 0;
static VBrush		   *darkStippleBrush	= 0;

// --------------------------------------------------------------------------
//
#define hitTest(x, y)			(((x) >= 0) && ((y) >= 0) && \
								 ((x) < getWidth()) && ((y) < getHeight()))

#define ucharRangeCheck(n)		{ if ((n) < 0) (n) = 0; \
								  else if ((n) > 255) { (n) = 255; } }

// --------------------------------------------------------------------------
//
VImageButton::VImageButton()
	: VImageBox()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VImageButton::VImageButton(VWindow *parentwin, const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
}

// --------------------------------------------------------------------------
//
VImageButton::VImageButton(const VFrame& frame, VWindow *parentwin,
						   const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(frame, parentwin, style);
}

// --------------------------------------------------------------------------
//
VImageButton::VImageButton(VWindow *parentwin, VImage *image, int index,
						   const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
	setImage(image, index);
}

// --------------------------------------------------------------------------
//
VImageButton::VImageButton(const VString& name,
						   VWindow *parentwin, const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VImageButton::VImageButton(const VString& name,
						   const VFrame& frame, VWindow *parentwin,
						   const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VImageButton::VImageButton(const VString& name,
						   VWindow *parentwin, VImage *image, int index,
						   const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
	setImage(image, index);
}

// --------------------------------------------------------------------------
//
void VImageButton::operator()(VWindow *parentwin)
{
	(*this)(VFrame(), parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VImageButton::operator()(VWindow *parentwin, const VStyle& style)
{
	(*this)(VFrame(), parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VImageButton::operator()(VWindow *parentwin,
							  const VStyle& style, const VString& name)
{
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VImageButton::operator()(const VFrame& frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VImageButton::operator()(const VFrame& frame, VWindow *parentwin,
							  const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VImageButton::operator()(const VFrame& frame, VWindow *parentwin,
							  const VStyle& style, const VString& name)
{
	VImageBox::operator()(frame, parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VImageButton::~VImageButton()
{
	if (--objectCount == 0) {
		destructStatic();
	}
}

// --------------------------------------------------------------------------
//
void VImageButton::initialize()
{
	if (objectCount++ == 0) {
		constructStatic();
	}

	clickedMethod					= 0;
#ifndef CV_NOARCHIVER
	clickedMethodIndex				= 0;
#endif
	currentlyArmed					= 0;
	currentlyPressed				= 0;
	currentlyToggledIn				= 0;
	currentlyStuckIn				= 0;
	currentlyIndeterminate			= 0;
	currentlyHasFocus				= 0;
	currentlyHasHover				= 0;
	checkBoxButton					= 0;
	radioButton						= 0;
	stickyRadioButton				= 0;
	thinBorder						= 0;
	thickBorder						= 1;
	showPressedDuringClickCallback	= 1;
	clickCallbackDisabled			= 0;
	currentlyCallingClicked			= 0;
	currentlyCallingCallback		= 0;
	amUsingHoverMode				= 0;
	amUsingStateImages				= 0;
	bitmapButtonCompatibilityMode	= 0;
	bitmapButtonFrameCount			= 4;
	bitmapButtonSequenceLength		= 4;
}

// --------------------------------------------------------------------------
//
void VImageButton::construct()
{
	setImmediateUpdate(FALSE);

	if (getStyle().contains(StyleNoBorder)) {
		thickBorder = thinBorder = 0;
	}
	if (thickBorder) {
		setMargins(1);
		VImageBox::setBorderStyle(VImageBox::RaisedThickBorder);
	}
	else if (thinBorder) {
		setMargins(2);
		VImageBox::setBorderStyle(VImageBox::RaisedThinBorder);
	}
	else {
		setMargins(2);
		VImageBox::setBorderStyle(VImageBox::NoBorder);
	}

	if (amUsingHoverMode) {
		hideBorder();
	}

	setImageDrawMode(VImage::Mapped | VImage::Transparent);

	if (getStyle().contains(StyleAutoToggle)) {
		checkBoxButton = 1;
	}

	setImmediateUpdate(TRUE);
}

// --------------------------------------------------------------------------
//
void VImageButton::constructStatic()
{
}

// --------------------------------------------------------------------------
//
void VImageButton::destructStatic()
{
#	if defined(CV_WINDOWS)
		if (lightStippleBrush != 0) { delete lightStippleBrush; }
		if (darkStippleBrush  != 0) { delete darkStippleBrush;  }
#	endif
	lightStippleBrush = 0;
	darkStippleBrush  = 0;
}

// --------------------------------------------------------------------------
//
VBrush *VImageButton::getLightStippleBrush()
{
	if (lightStippleBrush == 0) {
#		if defined(CV_WINDOWS)
			lightStippleBrush =
					new VBrush(StipplePat,
						   VColorScheme::systemColor(VColorScheme::
													 ShadedHighlight),
						   VColorScheme::systemColor(VColorScheme::
													 ShadedDarkHighlight));
#		else
			lightStippleBrush =
				&VColorScheme::systemBrush(VColorScheme::ShadedHighlight);
#		endif
	}
	return lightStippleBrush;
}

// --------------------------------------------------------------------------
//
VBrush *VImageButton::getDarkStippleBrush()
{
	if (darkStippleBrush == 0) {
#		if defined(CV_WINDOWS)
			darkStippleBrush =
				new VBrush(StipplePat,
						   VColorScheme::systemColor(VColorScheme::
													 ShadedShadow),
						   VColorScheme::systemColor(VColorScheme::
													 ShadedBackground));
#		else
			darkStippleBrush =
				&VColorScheme::systemBrush(VColorScheme::ShadedShadow);
#		endif
	}
	return darkStippleBrush;
}

// --------------------------------------------------------------------------
//
void VImageButton::setPushButton()
{
	if (!radioButton && !checkBoxButton) {
		return;
	}
	if (isToggledIn()) {
		toggleOut();
	}
	checkBoxButton = 0;
	radioButton = 0;
	stickyRadioButton = 0;
}

// --------------------------------------------------------------------------
//
void VImageButton::setCheckBoxButton()
{
	if (checkBoxButton) {
		return;
	}
	radioButton = 0;
	checkBoxButton = 1;
}

// --------------------------------------------------------------------------
//
void VImageButton::setRadioButton()
{
	if (radioButton) {
		return;
	}
	radioButton = 1;
	checkBoxButton = 0;
}

// --------------------------------------------------------------------------
//
void VImageButton::setStickyRadioButton()
{
	if (stickyRadioButton) {
		return;
	}
	radioButton = 1;
	stickyRadioButton = 1;
	checkBoxButton = 0;
}

// --------------------------------------------------------------------------
//
void VImageButton::setBorderStyle(int style)
{
	switch (style) {
	case ThickBorder:
		if (thickBorder) { return; }
		thickBorder = 1;
		thinBorder = 0;
		setImmediateUpdate(FALSE);
		VImageBox::setBorderStyle(VImageBox::RaisedThickBorder);
		setMargins(1);
		setImmediateUpdate(TRUE);
		break;
	case ThinBorder:
		if (thinBorder) { return; }
		thickBorder = 0;
		thinBorder = 1;
		setImmediateUpdate(FALSE);
		VImageBox::setBorderStyle(VImageBox::RaisedThinBorder);
		setMargins(2);
		setImmediateUpdate(TRUE);
		break;
	case NoBorder:
		if (!thickBorder && !thinBorder) { return; }
		thickBorder = 0;
		thinBorder = 0;
		setImmediateUpdate(FALSE);
		VImageBox::setBorderStyle(VImageBox::NoBorder);
		setMargins(2);
		setImmediateUpdate(TRUE);
		break;
	default:
		return;
	}
	updateGeometry(-1);
	repaint();
}

// --------------------------------------------------------------------------
//
void VImageButton::disable(boolean b)
{
	if (b && currentlyHasHover) {
		if (!currentlyToggledIn) {
			hideBorder();
		}
		currentlyHasHover = 0;
	}
	VImageBox::disable(b);
}

// --------------------------------------------------------------------------
//
void VImageButton::setIndeterminate(boolean b)
{
	if (b) {
		if (currentlyIndeterminate) {
			return;
		}
		currentlyIndeterminate = 1;
	}
	else {
		if (!currentlyIndeterminate) {
			return;
		}
		currentlyIndeterminate = 0;
	}
	repaint();
}

// --------------------------------------------------------------------------
//
void VImageButton::useHoverMode(boolean b)
{
	amUsingHoverMode = b;
}

// --------------------------------------------------------------------------
// When state-images are enabledd, sub-images within below (higher indexed
// rows) the first/default image are used if they exist for the following
// states:
//
// Image row 0: normal (toggled-out, unpressed)
// Image row 1: toggled-in
// Image row 2: pressed
// Image row 3: hover (toggled-out, unpressed)
// Image row 4: hover toggled-in
// Image row 5: stuck-in
// Image row 6: hover stuck-in
//
// ToDo, use virtual methods to obtain the indices of each state image.
//
void VImageButton::useStateImages(boolean b)
{
	if (b) {
		if (amUsingStateImages) {
			return;
		}
		amUsingStateImages = 1;
		if (currentlyPressed) {
			setImageMinorIndex(2);
		}
		else if (currentlyHasHover) {
			if (currentlyToggledIn) {
				if (currentlyStuckIn) {
					setImageMinorIndex(6);
				}
				else {
					setImageMinorIndex(4);
				}
			}
			else {
				setImageMinorIndex(3);
			}
		}
		else if (currentlyToggledIn) {
			if (currentlyStuckIn) {
				setImageMinorIndex(3);
			}
			else {
				setImageMinorIndex(1);
			}
		}
		else {
			setImageMinorIndex(0);
		}
	}
	else {
		if (!amUsingStateImages) {
			return;
		}
		amUsingStateImages = 0;
		setImageMinorIndex(0);
	}
}

// --------------------------------------------------------------------------
//
void VImageButton::uponClick(VObject *o, method m)
{
	setClient(o);
	clickedMethod = (boolean (VObject::*)(VImageButton *))m;
}

// --------------------------------------------------------------------------
//
void VImageButton::toggleIn(boolean b, int callback)
{
	if (!radioButton && !checkBoxButton) {
		return;
	}

	int flags;

	if (b) {
		if (currentlyToggledIn) {
			return;
		}
		flags = 0;
	}
	else {
		if (!currentlyToggledIn) {
			return;
		}
		currentlyStuckIn = 0;
		flags = ActivateForceToggle;
	}

	if (callback == 0) {
		flags |= ActivateDisableCallback;
	}
	else if (flags > 0) {
		flags |= ActivateEnableCallback;
	}

	activate(flags);
}

// --------------------------------------------------------------------------
//
void VImageButton::stickIn(boolean b, int callback)
{
	if (!radioButton) {
		return;
	}

	if (b) {
		if (currentlyStuckIn) {
			return;
		}
	}
	else {
		if (!currentlyStuckIn) {
			return;
		}
	}

	int flags = ActivateForceSticky;

	if (callback == 0) {
		flags |= ActivateDisableCallback;
	}
	else if (flags > 0) {
		flags |= ActivateEnableCallback;
	}

	activate(flags);
}

// --------------------------------------------------------------------------
//
void VImageButton::click(int callback)
{
	if (callback < 0) {
		activate();
	}
	else if (callback == 0) {
		activate(ActivateDisableCallback);
	}
	else {
		activate(ActivateEnableCallback);
	}
}

// --------------------------------------------------------------------------
//
void VImageButton::arm(boolean do_press)
{
	if (!currentlyArmed) {
		notifier->captureMouseFor(this);
		notifier->mouseTracking(TRUE);
		currentlyArmed = 1;
		if (do_press) {
			press();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageButton::disarm(boolean do_unpress)
{
	if (currentlyArmed) {
		notifier->mouseTracking(FALSE);
		notifier->releaseCapture();
		currentlyArmed = 0;
		if (do_unpress) {
			unpress();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageButton::activate(int flags)
{
	if (checkBoxButton) {
		currentlyToggledIn = !currentlyToggledIn;
	}
	else if (radioButton) {
		if (flags & ActivateForceToggle) {
			currentlyToggledIn = !currentlyToggledIn;
		}
		else if (!currentlyToggledIn) {
			currentlyToggledIn = 1;
			if (flags & ActivateForceSticky) {
				currentlyStuckIn = 1;
			}
		}
		else if (stickyRadioButton) {
			currentlyStuckIn = !currentlyStuckIn;
		}
		else {
			disarm();
			return;
		}
	}

	if (amUsingHoverMode) {
		if (currentlyToggledIn) {
			setImmediateUpdate(FALSE);
			showBorder();
			setImmediateUpdate(TRUE);
		}
		else if (checkBoxButton || radioButton) {
			setImmediateUpdate(FALSE);
			if (thickBorder) {
				VImageBox::setBorderStyle(VImageBox::RaisedThickBorder);
			}
			else if (thinBorder) {
				VImageBox::setBorderStyle(VImageBox::RaisedThinBorder);
			}
			setImmediateUpdate(TRUE);
		}
	}

	int do_repaint;

	if (showPressedDuringClickCallback) {
		do_repaint = 0;
		disarm(FALSE);
		press();
	}
	else if (currentlyArmed) {
		do_repaint = 0;
		disarm();
	}
	else {
		do_repaint = 1;
	}

	if (!(flags & ActivateDisableCallback)) {
		if (flags & ActivateEnableCallback) {
			currentlyCallingClicked = 1;
			clicked();
			currentlyCallingClicked = 0;
		}
		else if (!clickCallbackDisabled) {
			currentlyCallingClicked = 1;
			clicked();
			currentlyCallingClicked = 0;
		}
	}

	if (showPressedDuringClickCallback) {
		unpress();
	}
	else if (do_repaint) {
		repaint();
	}
}

// --------------------------------------------------------------------------
//
boolean VImageButton::clicked()
{
	VObject *client = getClient();
	if ((client != 0) && (clickedMethod != 0) &&
		!currentlyCallingCallback) {
		currentlyCallingCallback = 1;
		boolean status = (client->*clickedMethod)(this);
		currentlyCallingCallback = 0;
		return status;
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VImageButton::mouseDn(int x, int y)
{
	if (!isDisabled()) {
		arm();
	}
	return VImageBox::mouseDn(x, y);
}

// --------------------------------------------------------------------------
//
boolean VImageButton::mouseUp(int x, int y)
{
	if (currentlyArmed) {
		if (hitTest(x, y)) {
			activate();
		}
		else {
			disarm();
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImageButton::mouseDbl(int, int)
{
	if (!isDisabled()) {
		arm();
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImageButton::mouseMv(int x, int y, int)
{
	if (currentlyArmed) {
		if (hitTest(x, y)) {
			press();
		}
		else {
			unpress();
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImageButton::mouseEnter(int b)
{
	if (currentlyArmed) {
		press();
		return TRUE;
	}
	else {
		if (amUsingHoverMode && !currentlyHasHover && !isDisabled()) {
			currentlyHasHover = 1;
			if (!currentlyToggledIn) {
				showBorder();
			}
		}
		return VImageBox::mouseEnter(b);
	}
}

// --------------------------------------------------------------------------
//
boolean VImageButton::mouseExit(int b)
{
	if (currentlyArmed) {
		unpress();
		return TRUE;
	}
	else {
		if (amUsingHoverMode && currentlyHasHover && !isDisabled()) {
			currentlyHasHover = 0;
			if (!currentlyToggledIn) {
				if (!currentlyCallingClicked ||
					!showPressedDuringClickCallback) {
					hideBorder();
				}
			}
		}
		return VImageBox::mouseExit(b);
	}
}

// --------------------------------------------------------------------------
//
boolean VImageButton::paint()
{
	setImmediateUpdate(FALSE);
	if (currentlyPressed) {
		if (thickBorder) {
			VImageBox::setBorderStyle(SunkenThickBorder);
		}
		else if (thinBorder) {
			VImageBox::setBorderStyle(SunkenThinBorder);
		}
		setTextImageDisplacement(1);
	}
	else if (currentlyHasHover) {
		if (currentlyToggledIn) {
			if (thickBorder) {
				VImageBox::setBorderStyle(SunkenThickBorder);
			}
			else if (thinBorder) {
				VImageBox::setBorderStyle(SunkenThinBorder);
			}
			setTextImageDisplacement(1);
		}
		else {
			if (thickBorder) {
				VImageBox::setBorderStyle(RaisedThickBorder);
			}
			else if (thinBorder) {
				VImageBox::setBorderStyle(RaisedThinBorder);
			}
			setTextImageDisplacement(0);
		}
	}
	else if (currentlyToggledIn) {
		if (thickBorder) {
			VImageBox::setBorderStyle(SunkenThickBorder);
		}
		else if (thinBorder) {
			VImageBox::setBorderStyle(SunkenThinBorder);
		}
		setTextImageDisplacement(1);
	}
	else {
		if (thickBorder) {
			VImageBox::setBorderStyle(RaisedThickBorder);
		}
		else if (thinBorder) {
			VImageBox::setBorderStyle(RaisedThinBorder);
		}
		if (amUsingHoverMode) {
			hideBorder();
		}
		setTextImageDisplacement(0);
	}
	if (amUsingStateImages) {
		if (currentlyPressed) {
			setImageMinorIndex(2);
		}
		else if (currentlyToggledIn) {
			setImageMinorIndex(1);
		}
		else {
			setImageMinorIndex(0);
		}
	}
	VImageBox::paint();
	setImmediateUpdate(TRUE);
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VImageButton::paintBackground(VPort& port)
{
	if (isBitMapButtonCompatibilityMode()) {
		int index = bitmapButtonStartSequence * bitmapButtonSequenceLength;
		if (isDisabled()) {
			if (bitmapButtonSequenceLength >= 3) {
				index += 2;
			}
		}
		else if (currentlyPressed) {
			if (bitmapButtonSequenceLength >= 2) {
				index += 1;
			}
		}
		else if (currentlyHasFocus) {
			if (bitmapButtonSequenceLength >= 4) {
				index += 3;
			}
		}
		if (index < bitmapButtonFrameCount) {
			if (getImage() != 0) {
				getImage()->draw(&port, 0, 0, getImageIndex(), index,
								  VImage::Unmapped | VImage::Opaque);
			}
		}
	}
	else if (currentlyPressed) {
		port.useBrush(&VColorScheme::
					   systemBrush(VColorScheme::ShadedBackground));
	}
	else if (currentlyToggledIn) {
		if (currentlyStuckIn) {
			port.useBrush(getDarkStippleBrush());
		}
		else {
			port.useBrush(getLightStippleBrush());
		}
	}
	else if (currentlyIndeterminate) {
		port.useBrush(getLightStippleBrush());
	}
	else {
		port.useBrush(&VColorScheme::
					   systemBrush(VColorScheme::ShadedBackground));
	}
	static VRectangle r;
	if (isBorderHidden()) {
		r.set(CornerDim, 0, 0, getWidth(), getHeight());
	}
	else {
		r.set(CornerDim, getClientAreaX(), getClientAreaY(),
						 getClientAreaWidth(), getClientAreaHeight());
	}
	port.fillRegion(&r, FALSE);
}

// --------------------------------------------------------------------------
//
boolean VImageButton::givenFocus()
{
	currentlyHasFocus = 1;
	return VImageBox::givenFocus();
}

// --------------------------------------------------------------------------
//
boolean VImageButton::clearFocus()
{
	currentlyHasFocus = 0;
	return VImageBox::clearFocus();
}

// --------------------------------------------------------------------------
//
void VImageButton::setBitMapButtonCompatibilityMode(boolean b,
													VBitMap *bitmap,
													int frame_count,
													int sequence_length,
													int start_sequence)
{
	ucharRangeCheck(frame_count);
	ucharRangeCheck(sequence_length);
	ucharRangeCheck(start_sequence);

	if (b) {
		boolean update = FALSE;
		if (!bitmapButtonCompatibilityMode) {
			bitmapButtonCompatibilityMode = 1;
			update = TRUE;
		}
		if ((bitmapButtonFrameCount >= 0) &&
			(bitmapButtonFrameCount != frame_count)) {
			bitmapButtonFrameCount = frame_count;
			update = TRUE;
		}
		if ((bitmapButtonSequenceLength >= 0) &&
			(bitmapButtonSequenceLength != sequence_length)) {
			bitmapButtonSequenceLength = sequence_length;
			update = TRUE;
		}
		if ((bitmapButtonStartSequence >= 0) &&
			(bitmapButtonStartSequence != start_sequence)) {
			bitmapButtonStartSequence = start_sequence;
			update = TRUE;
		}
		if (bitmap != 0) {
			setImage(bitmap);
			getImage()->setItemCount(1, bitmapButtonFrameCount);
			update = TRUE;
		}
		if (update) {
			updateGeometry(-1);
			repaint();
		}
	}
	else if (bitmapButtonCompatibilityMode) {
		bitmapButtonCompatibilityMode = 0;
		updateGeometry(-1);
		repaint();
	}
}

// --------------------------------------------------------------------------
//
void VImageButton::setBitMapButtonBitmap(VBitMap *bitmap,
										 int frame_count,
										 int sequence_length,
										 int start_sequence)
{
	bitmapButtonFrameCount = frame_count;
	bitmapButtonSequenceLength = sequence_length;
	bitmapButtonStartSequence = start_sequence;
	setImage(bitmap);
	getImage()->setItemCount(1, bitmapButtonFrameCount);
	updateGeometry(-1);
	repaint();
}

// --------------------------------------------------------------------------
//
void VImageButton::setBitMapButtonBitmap(const VString& bitmap_file_name,
										 int frame_count,
										 int sequence_length,
										 int start_sequence)
{
	bitmapButtonFrameCount = frame_count;
	bitmapButtonSequenceLength = sequence_length;
	bitmapButtonStartSequence = start_sequence;
	setImage(bitmap_file_name);
	getImage()->setItemCount(1, bitmapButtonFrameCount);
	updateGeometry(-1);
	repaint();
}

// --------------------------------------------------------------------------
//
void VImageButton::setBitMapButtonFrameCount(int frame_count)
{
	if (bitmapButtonFrameCount != frame_count) {
		bitmapButtonFrameCount = frame_count;
		repaint();
	}
}

// --------------------------------------------------------------------------
//
void VImageButton::setBitMapButtonSequenceLength(int sequence_length)
{
	if (bitmapButtonSequenceLength != sequence_length) {
		bitmapButtonSequenceLength = sequence_length;
		repaint();
	}
}

// --------------------------------------------------------------------------
//
void VImageButton::setBitMapButtonStartSequence(int start_sequence)
{
	if (bitmapButtonStartSequence != start_sequence) {
		bitmapButtonStartSequence = start_sequence;
		repaint();
	}
}

// --------------------------------------------------------------------------
//
void VImageButton::systemColorChanged()
{
	VImageBox::systemColorChanged();
	destructStatic();
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VImageButton::putTo(VArchiver& a)
{
	VImageButton::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VImageButton::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VImageBox::putTo(a, FALSE);

	// Write the object version tag.

	a << VImageButtonArchiveTag;

	// Write any object specific data.

    const char *clicked_callback_class_name  = 0;            
    const char *clicked_callback_method_name = 0;            

    VCallbackList::findCallback
		(method(clickedMethod), clicked_callback_class_name,
								clicked_callback_method_name,
								clickedMethodIndex);
    a << (char *)clicked_callback_class_name;
    a << (char *)clicked_callback_method_name;

	a << short(bitmapButtonFrameCount);
	a << short(bitmapButtonSequenceLength);
	a << short(bitmapButtonStartSequence);

	a << char(currentlyToggledIn);
	a << char(radioButton);
	a << char(checkBoxButton);
	a << char(thickBorder);
	a << char(thinBorder);
	a << char(showPressedDuringClickCallback);
	a << char(clickCallbackDisabled);
	a << char(bitmapButtonCompatibilityMode);
}

// ---------------------------------------------------------------------------
//
void VImageButton::getFrom(VArchiver& a)
{
	// Read the base object data.

	VImageBox::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VImageButtonArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VImageButtonArchiveTag));
		return;
	}

	// Read any object specific data.

    char	clicked_callback_method_name[256];
    char	clicked_callback_class_name[256];
    char   *clicked_callback_method_name_ptr = clicked_callback_method_name;
    char   *clicked_callback_class_name_ptr = clicked_callback_class_name;

    a >> clicked_callback_class_name_ptr;
    a >> clicked_callback_method_name_ptr;

    method m = VCallbackList::findCallback(clicked_callback_class_name_ptr,
										   clicked_callback_method_name_ptr,
										   &clickedMethodIndex);
    if ((getClient() != 0) && (m != 0)) {
        if (!getClient()->isA(VClass::of(clicked_callback_class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }

    uponClick(getClient(), m);

	short s;

	a >> s; bitmapButtonFrameCount = s;
	a >> s; bitmapButtonSequenceLength = s;
	a >> s; bitmapButtonStartSequence = s;

	char c;

	a >> c; currentlyToggledIn				= c == 0 ? 0 : 1;
	a >> c; radioButton						= c == 0 ? 0 : 1;
	a >> c; checkBoxButton					= c == 0 ? 0 : 1;
	a >> c; thickBorder						= c == 0 ? 0 : 1;
	a >> c; thinBorder						= c == 0 ? 0 : 1;
	a >> c; showPressedDuringClickCallback	= c == 0 ? 0 : 1;
	a >> c; clickCallbackDisabled			= c == 0 ? 0 : 1;
	a >> c; bitmapButtonCompatibilityMode	= c == 0 ? 0 : 1;
}

// ---------------------------------------------------------------------------
//
void VImageButton::getFrom(VArchiver& a,
						   VObject *parentwin, boolean do_realize)
{
	VImageButton::getFrom(a);

	if (do_realize) {
		VImageButton::realize((VWindow *)parentwin);
	}
}

// ---------------------------------------------------------------------------
//
void VImageButton::realize(VWindow *parentwin)
{
	VImageBox::realize(parentwin);
	construct();
}

#endif // !CV_NOARCHIVER
