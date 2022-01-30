// $Header:   Q:/views/common/vcs/CLRSCM.H_V   1.5   Mar 24 1997 16:08:32   scottt  $

//	clrscm.h
//
//	VColorScheme interface [Common]
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
//	03/14/95 dgm	Original.
//	10/18/95 dgm	Added TopXXX colors.
//	11/10/95 dgm	Added TopActiveFlatBackground/TopInactiveFlatBackground.
//	11/13/95 dgm	Added TopActiveFlatBorder/TopInactiveFlatBorder.
//	12/04/95 dgm	Added system(), systemColor(), systemBrush(), systemPen().
//	12/05/95 dgm	Added ToolTipBackground/Foreground.
//	05/16/96 dgm	Added ToolTipHighlight/Shadow.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VCLRSCM_H
#define VCLRSCM_H

// --------------------------------------------------------------------------
//
#include "object.h"

// --------------------------------------------------------------------------
//
CLASS VBrush;
CLASS VClass;
CLASS VColor;
CLASS VObject;
CLASS VPen;
CLASS VString;
CLASS VWindow;

// --------------------------------------------------------------------------
//
CLASS VColorScheme : public VObject {

public:
	enum Color {
						// Non-shaded colors.

						FlatBackground					=  0,
						FlatForeground					=  1,
						FlatBackgroundSelect			=  2,
						FlatForegroundSelect			=  3,

						// Shaded colors.

						ShadedBackground				=  4,
						ShadedForeground				=  5,
						ShadedBackgroundSelect			=  6,
						ShadedForegroundSelect			=  7,
						ShadedHighlight					=  8,
						ShadedDarkHighlight				=  9,
						ShadedShadow					= 10,
						ShadedDarkShadow				= 11,

						// Tool-tip colors

						ToolTipBackground				= 12,
						ToolTipForeground				= 13,

						// Top-level window decoration colors.

						TopActiveFlatBackground			= 14,
						TopActiveFlatBorder				= 15,
						TopActiveShadedBackground		= 16,
						TopActiveShadedShadow			= 17,
						TopActiveShadedHighlight		= 18,
						TopActiveCaptionBackground		= 19,
						TopActiveCaptionForeground		= 20,

						TopInactiveFlatBackground		= 21,
						TopInactiveFlatBorder			= 22,
						TopInactiveShadedBackground		= 23,
						TopInactiveShadedShadow			= 24,
						TopInactiveShadedHighlight		= 25,
						TopInactiveCaptionBackground	= 26,
						TopInactiveCaptionForeground	= 27,

						ToolTipHighlight				= 28,
						ToolTipShadow					= 29
	};

public:
						VColorScheme();
						VColorScheme(VWindow *);
						VColorScheme(VClass *);
						VColorScheme(const VColor& flat_shaded_background);
						VColorScheme(const VColorScheme&);
					   ~VColorScheme();
	VClass			   *iam();

	VColorScheme&		operator=(const VColorScheme&);
	VColor			   *getColor(VColorScheme::Color);
	boolean				getColor(VColorScheme::Color, VColor&);
	boolean				getColor(VColorScheme::Color,
								 short& red, short& green, short& blue);
	void				setColor(VColorScheme::Color, VColor&);
	void				set();
	void				set(VWindow *);
	void				set(VClass *);
	void				set(const VColor& flat_shaded_background);
	void				setFlatBackground(const VColor&);
	void				setShadedBackground(const VColor&);
	void				setFlatBackground();
	void				setShadedBackground();

	void				useNativeColorCalculation(boolean);
	boolean				usingNativeColorCalculation() const;

	static VColorScheme&	system();
	static VColor&			systemColor(VColorScheme::Color);
	static VBrush&			systemBrush(VColorScheme::Color);
	static VPen&			systemPen(VColorScheme::Color);
	static boolean			isSystemColor(VColor *);
	static boolean			isSystemBrush(VBrush *);
	static boolean			isSystemPen(VPen *);
	static boolean			checkSystemColorDeletion(VColor *);
	static boolean			checkSystemBrushDeletion(VBrush *);
	static boolean			checkSystemPenDeletion(VPen *);
	static void				systemColorsChanged();

public:
	enum Constants {
						MaxColors	= 30
	};

protected:
	void				initialize(boolean = TRUE);
	void				reinitializeColorArray();
	boolean				getFlatBackgroundColor();
	boolean				getFlatForegroundColor();
	boolean				getFlatBackgroundSelectColor();
	boolean				getFlatForegroundSelectColor();
	boolean				getShadedBackgroundColor();
	boolean				getShadedForegroundColor();
	boolean				getShadedBackgroundSelectColor();
	boolean				getShadedForegroundSelectColor();
	boolean				getShadedHighlightColor();
	boolean				getShadedDarkHighlightColor();
	boolean				getShadedShadowColor();
	boolean				getShadedDarkShadowColor();
	boolean				getToolTipBackgroundColor();
	boolean				getToolTipForegroundColor();
	boolean				getToolTipHighlightColor();
	boolean				getToolTipShadowColor();
	boolean				getTopActiveCaptionBackgroundColor();
	boolean				getTopActiveCaptionForegroundColor();
	boolean				getTopActiveFlatBackgroundColor();
	boolean				getTopActiveFlatBorderColor();
	boolean				getTopActiveShadedBackgroundColor();
	boolean				getTopActiveShadedHighlightColor();
	boolean				getTopActiveShadedShadowColor();
	boolean				getTopInactiveCaptionBackgroundColor();
	boolean				getTopInactiveCaptionForegroundColor();
	boolean				getTopInactiveFlatBackgroundColor();
	boolean				getTopInactiveFlatBorderColor();
	boolean				getTopInactiveShadedBackgroundColor();
	boolean				getTopInactiveShadedHighlightColor();
	boolean				getTopInactiveShadedShadowColor();

private:
	VColor			  **theColors;
	union {
		VColor		   *theBackground;
		VWindow		   *theWindow;
		VClass		   *theClass;
		void		   *theBackgroundWindowAndClass;
	};
	int					useGivenFlatBackground : 1;
	int					useGivenShadedBackground : 1;
	int					useWindow : 1;
	int					useClass : 1;
	int					useNativeColorCalculationFunction : 1;
};

extern VClass *VColorSchemeCls;

#endif // VCLRSCM_H
