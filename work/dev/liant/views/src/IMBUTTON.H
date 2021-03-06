//	imbutton.h
//
//	VImageButton interface [Common]
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
// --------------------------------------------------------------------------

#ifndef VIMBUTTON_H
#define VIMBUTTON_H

// --------------------------------------------------------------------------
//
#include "imbox.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VControl;
CLASS	VFont;
CLASS	VImage;
CLASS	VImageBox;
CLASS	VObject;
CLASS	VString;
CLASS	VPathString;
CLASS	VPort;
CLASS	VWindow;

#ifndef CV_NOARCHIVER
CLASS	VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VImageButton : public VImageBox {
public:

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	enum Type 	{	Push			= 0,
					CheckBox		= 1,
					Radio			= 2,
					StickyRadio		= 3 };


	enum Border {	NoBorder		=  1,
					ThinBorder		=  2,
					ThickBorder		=  3 };

	enum State {	ToggledOut		= 0x0001,
					ToggledIn		= 0x0002,
					StuckIn			= 0x0004,
					UnPressed		= 0x0008,
					Pressed			= 0x0010,
					Enabled			= 0x0020,
					Disabled		= 0x0040,
					Indeterminate	= 0x0080,
					UnArmed			= 0x0100,
					Armed			= 0x0200,
					Focus			= 0x0400,
					Hover			= 0x0800 };

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

public:
					VImageButton(VWindow *, const VStyle& = StyleDefault);
					VImageButton(const VFrame&, VWindow *,
								 const VStyle& = StyleDefault);
					VImageButton(const VString&,
								 VWindow *, const VStyle& = StyleDefault);
					VImageButton(const VString&,
								 const VFrame&, VWindow *,
								 const VStyle& = StyleDefault);
					VImageButton(VWindow *, VImage *, int index = 0,
								 const VStyle& = StyleDefault);
					VImageButton(const VString&,
								 VWindow *, VImage *, int index = 0,
								 const VStyle& = StyleDefault);
					VImageButton();
				   ~VImageButton();
	VClass		   *iam();

	void			operator()(VWindow *);
	void			operator()(VWindow *, const VStyle&);
	void			operator()(VWindow *, const VStyle&, const VString&);
	void			operator()(const VFrame&, VWindow *);
	void			operator()(const VFrame&, VWindow *, const VStyle&);
	void			operator()(const VFrame&, VWindow *, const VStyle&,
							   const VString&);

	// Set operations.

	void			uponClick(VObject *, method);

	void			setType(Type);
	void			setPushButton();
	void			setCheckBoxButton();
	void			setRadioButton();
	void			setStickyRadioButton();

	void			click(int callback = -1);
	void			toggle(int callback = -1);
	void			toggleIn(boolean = TRUE, int callback = -1);
	void			toggleOut(boolean = TRUE, int callback = -1);
	void			check(boolean = TRUE, int callback = -1);
	void			uncheck(boolean = TRUE, int callback = -1);
	void			stickIn(boolean = TRUE, int callback = -1);
	void			unstickIn(boolean = TRUE, int callback = -1);

	void			setIndeterminate(boolean);

	void			disableClickCallback(boolean = TRUE);
	void			enableClickCallback(boolean = TRUE);

	void			setShowPressedDuringClickCallback(boolean);
	void			useStateImages(boolean = TRUE);
	void			useHoverMode(boolean = TRUE);
	void			useGrayScaleHoverMode(boolean = TRUE);

	void			setBorderStyle(int);

	virtual void	disable(boolean = TRUE);

	// Get operations.

	Type			getType() const;
	boolean			isPushButton() const;
	boolean			isCheckBoxButton() const;
	boolean			isRadioButton() const;
	boolean			isStickyRadioButton() const;

	boolean			isToggledIn() const;
	boolean			isToggledOut() const;
	boolean			checked() const;
	boolean			unchecked() const;

	boolean			isStuckIn() const;

	boolean			isIndeterminate() const;

	boolean			hasFocus() const;
	boolean			hasHover() const;

	boolean			isClickCallbackEnabled() const;
	boolean			isClickCallbackDisabled() const;

	boolean			isShowPressedDuringClickCallback() const;
	boolean			usingStateImages() const;
	boolean			usingHoverMode() const;
	boolean			usingGrayScaleHoverMode() const;

	int				getBorderStyle() const;
	int				getState() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Obsolete!

	void			setAutoToggleButton();
	void			setToggleButton();
	boolean			isAutoToggleButton() const;
	boolean			isToggleButton() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// VBitMapButton compatibility mode -- undocumented.

	void			setBitMapButtonCompatibilityMode(boolean,
													 VBitMap * = 0,
													 int = -1,
													 int = -1, int = -1);
	boolean			isBitMapButtonCompatibilityMode() const;

	void			setBitMapButtonBitmap(VBitMap *,
										  int = -1, int = -1, int = -1);
	void			setBitMapButtonBitmap(const VString&,
										  int = -1, int = -1, int = -1);

	void			setBitMapButtonFrameCount(int);
	int				getBitMapButtonFrameCount() const;
	void			setBitMapButtonSequenceLength(int);
	int				getBitMapButtonSequenceLength() const;
	void			setBitMapButtonStartSequence(int);
	int				getBitMapButtonStartSequence() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
	virtual boolean		mouseDn(int, int);
	virtual boolean		mouseMv(int, int, int);
	virtual boolean		mouseUp(int, int);
	virtual boolean		mouseDbl(int, int);
	virtual boolean		mouseEnter(int);
	virtual boolean		mouseExit(int);
	virtual boolean		givenFocus();
	virtual boolean		clearFocus();
	virtual boolean		clicked();
	virtual void		systemColorChanged();
	virtual boolean		paint();
	virtual void		paintBackground(VPort&);

protected:
	enum ActivateFlags {	ActivateDisableCallback	= 0x0001,
							ActivateEnableCallback	= 0x0002,
							ActivateForceToggle		= 0x0004,
							ActivateForceSticky		= 0x0008 };

protected:
	void					initialize();
	void					construct();
	void					arm(boolean = TRUE);
	void					disarm(boolean = TRUE);
	void					press();
	void					unpress();
	void					activate(int = 0);
	boolean					isPressed() const;
	boolean					isArmed() const;

#ifndef CV_NOARCHIVER
public:
	virtual void			putTo(VArchiver&);
	void					putTo(VArchiver&, boolean);
	virtual void			getFrom(VArchiver&);
	virtual void			getFrom(VArchiver&, VObject *, boolean = TRUE);
	virtual void			realize(VWindow *);
protected:
#endif

protected:
	static void				constructStatic();
	static void				destructStatic();
	static VBrush		   *getLightStippleBrush();
	static VBrush		   *getDarkStippleBrush();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
							VImageButton(const VImageButton&) {}
	VImageButton&			operator=(const VImageButton&) { return *this; }

private:
	boolean		 (VObject::*clickedMethod)(VImageButton *);
#ifndef CV_NOARCHIVER
	integer 				clickedMethodIndex;
#endif
	unsigned char			bitmapButtonFrameCount;
	unsigned char			bitmapButtonSequenceLength;
	unsigned char			bitmapButtonStartSequence;
	unsigned short			checkBoxButton : 1;
	unsigned short			radioButton : 1;
	unsigned short			stickyRadioButton : 1;
	unsigned short			currentlyArmed : 1;
	unsigned short			currentlyPressed : 1;
	unsigned short			currentlyToggledIn : 1;
	unsigned short			currentlyStuckIn : 1;
	unsigned short			currentlyIndeterminate : 1;
	unsigned short			currentlyHasFocus : 1;
	unsigned short			currentlyHasHover : 1;
	unsigned short			thickBorder : 1;
	unsigned short			thinBorder : 1;
	unsigned short			showPressedDuringClickCallback : 1;
	unsigned short			clickCallbackDisabled : 1;
	unsigned short			currentlyCallingClicked : 1;
	unsigned short			currentlyCallingCallback : 1;
	unsigned short			bitmapButtonCompatibilityMode : 1;
	unsigned short			amUsingStateImages : 1;
	unsigned short			amUsingHoverMode : 1;
	unsigned short			amUsingGrayScaleHoverMode : 1;
};

extern VClass *VImageButtonCls;

// --------------------------------------------------------------------------
//
inline boolean VImageButton::isPushButton() const
{
	return !radioButton && !checkBoxButton;
}

inline boolean VImageButton::isCheckBoxButton() const
{
	return checkBoxButton;
}

inline boolean VImageButton::isRadioButton() const
{
	return radioButton;
}

inline boolean VImageButton::isStickyRadioButton() const
{
	return stickyRadioButton;
}

inline boolean VImageButton::isAutoToggleButton() const
{
	return checkBoxButton;
}

inline boolean VImageButton::isToggleButton() const
{
	return radioButton;
}

inline VImageButton::Type VImageButton::getType() const
{
	if (isCheckBoxButton()) {
		return CheckBox;
	}
	else if (isRadioButton()) {
		if (isStickyRadioButton()) {
			return StickyRadio;
		}
		else {
			return Radio;
		}
	}
	else {
		return Push;
	}
}

inline void VImageButton::setAutoToggleButton()
{
	setCheckBoxButton();
}

inline void VImageButton::setToggleButton()
{
	setRadioButton();
}

inline void VImageButton::setType(VImageButton::Type type)
{
	if (type == Push) {
		setPushButton();
	}
	else if (type == CheckBox) {
		setCheckBoxButton();
	}
	else if (type == Radio) {
		setRadioButton();
	}
	else if (type == StickyRadio) {
		setStickyRadioButton();
	}
}

inline boolean VImageButton::isToggledIn() const
{
	return currentlyToggledIn;
}

inline boolean VImageButton::isToggledOut() const
{
	return !currentlyToggledIn;
}

inline void VImageButton::toggleOut(boolean b, int callback)
{
	toggleIn(!b, callback);
}

inline void VImageButton::toggle(int callback)
{
	if (radioButton || checkBoxButton) {
		toggleIn(!currentlyToggledIn, callback);
	}
}

inline void VImageButton::check(boolean b, int callback)
{
	toggleIn(b, callback);
}

inline void VImageButton::uncheck(boolean b, int callback)
{
	toggleOut(b, callback);
}

inline void VImageButton::unstickIn(boolean b, int callback)
{
	stickIn(!b, callback);
}

inline boolean VImageButton::checked() const
{
	return isToggledIn();
}

inline boolean VImageButton::unchecked() const
{
	return isToggledOut();
}

inline boolean VImageButton::isIndeterminate() const
{
	return currentlyIndeterminate;
}

inline boolean VImageButton::hasFocus() const
{
	return currentlyHasFocus;
}

inline boolean VImageButton::hasHover() const
{
	return currentlyHasHover;
}

inline boolean VImageButton::isStuckIn() const
{
	return currentlyStuckIn;
}

inline void VImageButton::press()
{
	if (!currentlyPressed) {
		currentlyPressed = 1;
		repaint();
	}
}

inline void VImageButton::unpress()
{
	if (currentlyPressed) {
		currentlyPressed = 0;
		repaint();
	}
}

inline boolean VImageButton::isPressed() const
{
	return currentlyPressed;
}

inline boolean VImageButton::isArmed() const
{
	return currentlyArmed;
}

inline void VImageButton::disableClickCallback(boolean b)
{
	clickCallbackDisabled = b;
}

inline void VImageButton::enableClickCallback(boolean b)
{
	clickCallbackDisabled = !b;
}

inline boolean VImageButton::isClickCallbackEnabled() const
{
	return !clickCallbackDisabled;
}

inline boolean VImageButton::isClickCallbackDisabled() const
{
	return clickCallbackDisabled;
}

inline void VImageButton::setShowPressedDuringClickCallback(boolean b)
{
	showPressedDuringClickCallback = b;
}

inline boolean VImageButton::isShowPressedDuringClickCallback() const
{
	return showPressedDuringClickCallback;
}

inline boolean VImageButton::usingStateImages() const
{
	return amUsingStateImages;
}

inline boolean VImageButton::usingHoverMode() const
{
	return amUsingHoverMode;
}

inline boolean VImageButton::usingGrayScaleHoverMode() const
{
	return amUsingGrayScaleHoverMode;
}

inline int VImageButton::getBorderStyle() const
{
	if (thickBorder) {
		return ThickBorder;
	}
	else if (thinBorder) {
		return ThinBorder;
	}
	else {
		return NoBorder;
	}
}

inline boolean VImageButton::isBitMapButtonCompatibilityMode() const
{
	return bitmapButtonCompatibilityMode;
}

inline int VImageButton::getBitMapButtonFrameCount() const
{
	return bitmapButtonFrameCount;
}

inline int VImageButton::getBitMapButtonSequenceLength() const
{
	return bitmapButtonSequenceLength;
}

inline int VImageButton::getBitMapButtonStartSequence() const
{
	return bitmapButtonStartSequence;
}

inline int VImageButton::getState() const
{
	int state = 0;
	if (isToggledIn()) { state |= ToggledIn; } else { state |= ToggledOut; }
	if (isStuckIn())   { state |= StuckIn;   }
	if (isPressed())   { state |= Pressed;   } else { state |= UnPressed;  }
	if (isDisabled())  { state |= Disabled;  } else { state |= Enabled;    }
	if (isArmed())     { state |= Armed;     } else { state |= UnArmed;    }
	if (hasFocus())    { state |= Focus;     }
	if (hasHover())    { state |= Hover;     }
	return state;
}

#endif // VIMBUTTON_H
