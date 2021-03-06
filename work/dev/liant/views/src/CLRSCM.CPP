// $Header:   Q:/views/common/vcs/clrscm.cpv   1.14   02 Apr 1997 12:26:32   GREGM  $

//  clrscm.cpp
//
//  VColorScheme implementation [Common]
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
//  Revision History:
//  -----------------
//  03/14/95 dgm    Original.
//                  Required 3.0.5 library changes (Motif):
//                      o Added VNotifier::getTopWindow()
//                      o RGB value range conversion fixes in VColor.
//  10/18/95 dgm    Added TopXXX colors.
//  10/19/95 dgm    Added OS/2 stubs.
//  11/01/95 dgm    Changes to suppress double/float conversion warnings.
//  11/10/95 dgm    Added TopActiveFlatBackground/TopInactiveFlatBackground.
//  11/13/95 dgm    Added TopActiveFlatBorder/TopInactiveFlatBorder.
//  12/04/95 dgm    Added system(), systemColor(), systemBrush(), systemPen().
//  12/05/95 dgm    Added ToolTipBackground/Foreground.
//  12/05/95 dgm    Motif changes.
//  12/05/95 dss    MS 16 bit compiler exports VDummy with empty dtor which
//					Competes with name in toolbar.cpp. Added function call
//					seems to fix problem.
//	12/21/95 pkt	changed declaration of VDummyDtor to static to compile
//					on Dec.
//	01/10/96 dgm	Changed to not flag system resource deletions upon
//					destruction.
//	01/22/96 dgm	Minor Motif changes.
//  02/19/96 evc    decorated dummy with CV_NEARDATA
//  05/16/96 dgm    Added ToolTipHighlight/Shadow.
//  09/10/96 dgm    Fixed memory leak in system brush/pen destruction.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "clrscm.h"
#include "color.h"
#include "window.h"
#include "class.h"
#include "pen.h"
#include "brush.h"
#include "winclass.h"

// --------------------------------------------------------------------------
//
defineClass(VColorScheme, VObject)

// --------------------------------------------------------------------------
//
static VColorScheme    *system_colorscheme = 0;
static VBrush         **system_brushes = 0;
static VPen           **system_pens = 0;

// --------------------------------------------------------------------------
//
VColorScheme::VColorScheme()
{
    initialize();
}

// --------------------------------------------------------------------------
//
VColorScheme::VColorScheme(const VColor& flat_shaded_background)
{
    initialize();
    useGivenFlatBackground = 1;
    useGivenShadedBackground = 1;
    theColors[FlatBackground] = new VColor(flat_shaded_background);
    theColors[ShadedBackground] = new VColor(flat_shaded_background);
}

// --------------------------------------------------------------------------
//
VColorScheme::VColorScheme(VWindow *the_window)
{
    initialize();
    useWindow = ((theWindow = the_window) != 0);
}

// --------------------------------------------------------------------------
//
VColorScheme::VColorScheme(VClass *the_class)
{
    initialize();
    useClass = ((theClass = the_class) != 0);
}

// --------------------------------------------------------------------------
//
VColorScheme::VColorScheme(const VColorScheme& color_scheme)
{
    initialize(FALSE);
    *this = color_scheme;
}

// --------------------------------------------------------------------------
//
VColorScheme& VColorScheme::operator=(const VColorScheme& color_scheme)
{
    useNativeColorCalculationFunction =
        color_scheme.useNativeColorCalculationFunction;

    for (int i = 0 ; i < MaxColors ; i++) {
        if (color_scheme.theColors[i] == 0) {
            if (theColors[i] != 0) {
                delete theColors[i];
                theColors[i] = 0;
            }
        }
        else if (color_scheme.theColors[i] == 0) {
            theColors[i] = new VColor(*color_scheme.theColors[i]);
        }
        else {
            *theColors[i] = *color_scheme.theColors[i];
        }
    }

    useGivenFlatBackground = 0;
    useGivenShadedBackground = 0;
    useClass = 0;
    useWindow = 0;
    theBackgroundWindowAndClass = 0;

    if (color_scheme.useGivenFlatBackground &&
        (color_scheme.theColors != 0) &&
        (color_scheme.theColors[FlatBackground] != 0)) {
        useGivenFlatBackground = 1;
    }
    if (color_scheme.useGivenShadedBackground &&
        (color_scheme.theColors != 0) &&
        (color_scheme.theColors[ShadedBackground] != 0)) {
        useGivenShadedBackground = 1;
    }

    if (color_scheme.useWindow && (color_scheme.theWindow != 0)) {
        useWindow = 1;
        theWindow = color_scheme.theWindow;
    }
    else if (color_scheme.useClass && (color_scheme.theClass != 0)) {
        useClass = 1;
        theClass = color_scheme.theClass;
    }

    return *this;
}

// --------------------------------------------------------------------------
//
void VColorScheme::set()
{
    reinitializeColorArray();
    useGivenFlatBackground = 0;
    useGivenShadedBackground = 0;
    useClass = 0;
    useWindow = 0;
    theBackgroundWindowAndClass = 0;
}

// --------------------------------------------------------------------------
//
void VColorScheme::set(VWindow *the_window)
{
    reinitializeColorArray();
    useGivenFlatBackground = 0;
    useGivenShadedBackground = 0;
    useClass = 0;
    useWindow = ((theWindow = the_window) != 0);
}

// --------------------------------------------------------------------------
//
void VColorScheme::set(VClass *the_class)
{
    reinitializeColorArray();
    useGivenFlatBackground = 0;
    useGivenShadedBackground = 0;
    useWindow = 0;
    useClass = ((theClass = the_class) != 0);
}

// --------------------------------------------------------------------------
//
void VColorScheme::set(const VColor& flat_shaded_background)
{
    reinitializeColorArray();
    useWindow = 0;
    useClass = 0;
    useGivenFlatBackground = 1;
    useGivenShadedBackground = 1;
    theColors[FlatBackground] = new VColor(flat_shaded_background);
    theColors[ShadedBackground] = new VColor(flat_shaded_background);
}

// --------------------------------------------------------------------------
//
void VColorScheme::setFlatBackground()
{
    if (theColors != 0) {
        if (theColors[FlatBackground] != 0) {
            delete theColors[FlatBackground];
            theColors[FlatBackground] = 0;
        }
        if (theColors[FlatForeground] != 0) {
            delete theColors[FlatForeground];
            theColors[FlatForeground] = 0;
        }
        if (theColors[FlatBackgroundSelect] != 0) {
            delete theColors[FlatBackgroundSelect];
            theColors[FlatBackgroundSelect] = 0;
        }
        if (theColors[FlatForegroundSelect] != 0) {
            delete theColors[FlatForegroundSelect];
            theColors[FlatForegroundSelect] = 0;
        }
    }
    useGivenFlatBackground = 0;
}

// --------------------------------------------------------------------------
//
void VColorScheme::setShadedBackground()
{
    if (theColors != 0) {
        if (theColors[ShadedBackground] != 0) {
            delete theColors[ShadedBackground];
            theColors[ShadedBackground] = 0;
        }
        if (theColors[ShadedForeground] != 0) {
            delete theColors[ShadedForeground];
            theColors[ShadedForeground] = 0;
        }
        if (theColors[ShadedBackgroundSelect] != 0) {
            delete theColors[ShadedBackgroundSelect];
            theColors[ShadedBackgroundSelect] = 0;
        }
        if (theColors[ShadedForegroundSelect] != 0) {
            delete theColors[ShadedForegroundSelect];
            theColors[ShadedForegroundSelect] = 0;
        }
        if (theColors[ShadedHighlight] != 0) {
            delete theColors[ShadedHighlight];
            theColors[ShadedHighlight] = 0;
        }
        if (theColors[ShadedDarkHighlight] != 0) {
            delete theColors[ShadedDarkHighlight];
            theColors[ShadedDarkHighlight] = 0;
        }
        if (theColors[ShadedShadow] != 0) {
            delete theColors[ShadedShadow];
            theColors[ShadedShadow] = 0;
        }
        if (theColors[ShadedDarkShadow] != 0) {
            delete theColors[ShadedDarkShadow];
            theColors[ShadedDarkShadow] = 0;
        }
    }
    useGivenShadedBackground = 0;
}

// --------------------------------------------------------------------------
//
void VColorScheme::setFlatBackground(const VColor& flat_background)
{
    setFlatBackground();
    if (theColors != 0) {
        theColors[FlatBackground] = new VColor(flat_background);
    }
    useGivenFlatBackground = 1;
}

// --------------------------------------------------------------------------
//
void VColorScheme::setShadedBackground(const VColor& shaded_background)
{
    setShadedBackground();
    if (theColors != 0) {
        theColors[ShadedBackground] = new VColor(shaded_background);
    }
    useGivenShadedBackground = 1;
}

// --------------------------------------------------------------------------
//
void VColorScheme::initialize(boolean create_array)
{
    if (create_array) {
        theColors = new VColor *[MaxColors];
        for (int i = 0 ; i < MaxColors ; i++) {
            theColors[i] = 0;
        }
    }
    else {
        theColors = 0;
    }
    useGivenFlatBackground = 0;
    useGivenShadedBackground = 0;
    useClass = 0;
    useWindow = 0;
    theBackgroundWindowAndClass = 0;
#if defined(CV_MOTIF)
    useNativeColorCalculationFunction = 1;
#else
    useNativeColorCalculationFunction = 0;
#endif
}

// --------------------------------------------------------------------------
//
void VColorScheme::reinitializeColorArray()
{
    if (theColors != 0) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (theColors[i] != 0) {
                delete theColors[i];
                theColors[i] = 0;
            }
        }
    }
}

// --------------------------------------------------------------------------
//
VColorScheme::~VColorScheme()
{
    if (theColors != 0) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (theColors[i] != 0) {
                delete theColors[i];
            }
        }
        delete [] theColors;
    }
    if (this == system_colorscheme) {
        //
        // Note that this allows for careless users deleting the
        // VColorScheme object returned by VColorScheme::system().
        //
        system_colorscheme = 0;
    }
}

// --------------------------------------------------------------------------
//
VColor *VColorScheme::getColor(VColorScheme::Color color_scheme_color)
{
    switch (color_scheme_color) {

    case FlatBackground:
        if ((theColors[FlatBackground] == 0) &&
            !getFlatBackgroundColor()) {
            return 0;
        }
        return theColors[FlatBackground];

    case FlatForeground:
        if ((theColors[FlatForeground] == 0) &&
            !getFlatForegroundColor()) {
            return 0;
        }
        return theColors[FlatForeground];

    case FlatBackgroundSelect:
        if ((theColors[FlatBackgroundSelect] == 0) &&
            !getFlatBackgroundSelectColor()) {
            return 0;
        }
        return theColors[FlatBackgroundSelect];

    case FlatForegroundSelect:
        if ((theColors[FlatForegroundSelect] == 0) &&
            !getFlatForegroundSelectColor()) {
            return 0;
        }
        return theColors[FlatForegroundSelect];

    case ShadedBackground:
        if ((theColors[ShadedBackground] == 0) &&
            !getShadedBackgroundColor()) {
            return 0;
        }
        return theColors[ShadedBackground];

    case ShadedForeground:
        if ((theColors[ShadedForeground] == 0) &&
            !getShadedForegroundColor()) {
            return 0;
        }
        return theColors[ShadedForeground];

    case ShadedBackgroundSelect:
        if ((theColors[ShadedBackgroundSelect] == 0) &&
            !getShadedBackgroundSelectColor()) {
            return 0;
        }
        return theColors[ShadedBackgroundSelect];

    case ShadedForegroundSelect:
        if ((theColors[ShadedForegroundSelect] == 0) &&
            !getShadedForegroundSelectColor()) {
            return 0;
        }
        return theColors[ShadedForegroundSelect];

    case ShadedHighlight:
        if ((theColors[ShadedHighlight] == 0) &&
            !getShadedHighlightColor()) {
            return 0;
        }
        return theColors[ShadedHighlight];

    case ShadedDarkHighlight:
        if ((theColors[ShadedDarkHighlight] == 0) &&
            !getShadedDarkHighlightColor()) {
            return 0;
        }
        return theColors[ShadedDarkHighlight];

    case ShadedShadow:
        if ((theColors[ShadedShadow] == 0) &&
            !getShadedShadowColor()) {
            return 0;
        }
        return theColors[ShadedShadow];

    case ShadedDarkShadow:
        if ((theColors[ShadedDarkShadow] == 0) &&
            !getShadedDarkShadowColor()) {
            return 0;
        }
        return theColors[ShadedDarkShadow];

    case ToolTipBackground:
        if ((theColors[ToolTipBackground] == 0) &&
            !getToolTipBackgroundColor()) {
            return 0;
        }
        return theColors[ToolTipBackground];

    case ToolTipForeground:
        if ((theColors[ToolTipForeground] == 0) &&
            !getToolTipForegroundColor()) {
            return 0;
        }
        return theColors[ToolTipForeground];

    case ToolTipHighlight:
        if ((theColors[ToolTipHighlight] == 0) &&
            !getToolTipHighlightColor()) {
            return 0;
        }
        return theColors[ToolTipHighlight];

    case ToolTipShadow:
        if ((theColors[ToolTipShadow] == 0) &&
            !getToolTipShadowColor()) {
            return 0;
        }
        return theColors[ToolTipShadow];

    case TopActiveCaptionBackground:
        if ((theColors[TopActiveCaptionBackground] == 0) &&
            !getTopActiveCaptionBackgroundColor()) {
            return 0;
        }
        return theColors[TopActiveCaptionBackground];

    case TopActiveCaptionForeground:
        if ((theColors[TopActiveCaptionForeground] == 0) &&
            !getTopActiveCaptionForegroundColor()) {
            return 0;
        }
        return theColors[TopActiveCaptionForeground];

    case TopActiveFlatBackground:
        if ((theColors[TopActiveFlatBackground] == 0) &&
            !getTopActiveFlatBackgroundColor()) {
            return 0;
        }
        return theColors[TopActiveFlatBackground];

    case TopActiveFlatBorder:
        if ((theColors[TopActiveFlatBorder] == 0) &&
            !getTopActiveFlatBorderColor()) {
            return 0;
        }
        return theColors[TopActiveFlatBorder];

    case TopActiveShadedBackground:
        if ((theColors[TopActiveShadedBackground] == 0) &&
            !getTopActiveShadedBackgroundColor()) {
            return 0;
        }
        return theColors[TopActiveShadedBackground];

    case TopActiveShadedHighlight:
        if ((theColors[TopActiveShadedHighlight] == 0) &&
            !getTopActiveShadedHighlightColor()) {
            return 0;
        }
        return theColors[TopActiveShadedHighlight];

    case TopActiveShadedShadow:
        if ((theColors[TopActiveShadedShadow] == 0) &&
            !getTopActiveShadedShadowColor()) {
            return 0;
        }
        return theColors[TopActiveShadedShadow];

    case TopInactiveCaptionBackground:
        if ((theColors[TopInactiveCaptionBackground] == 0) &&
            !getTopInactiveCaptionBackgroundColor()) {
            return 0;
        }
        return theColors[TopInactiveCaptionBackground];

    case TopInactiveCaptionForeground:
        if ((theColors[TopInactiveCaptionForeground] == 0) &&
            !getTopInactiveCaptionForegroundColor()) {
            return 0;
        }
        return theColors[TopInactiveCaptionForeground];

    case TopInactiveFlatBackground:
        if ((theColors[TopInactiveFlatBackground] == 0) &&
            !getTopInactiveFlatBackgroundColor()) {
            return 0;
        }
        return theColors[TopInactiveFlatBackground];

    case TopInactiveFlatBorder:
        if ((theColors[TopInactiveFlatBorder] == 0) &&
            !getTopInactiveFlatBorderColor()) {
            return 0;
        }
        return theColors[TopInactiveFlatBorder];

    case TopInactiveShadedBackground:
        if ((theColors[TopInactiveShadedBackground] == 0) &&
            !getTopInactiveShadedBackgroundColor()) {
            return 0;
        }
        return theColors[TopInactiveShadedBackground];

    case TopInactiveShadedHighlight:
        if ((theColors[TopInactiveShadedHighlight] == 0) &&
            !getTopInactiveShadedHighlightColor()) {
            return 0;
        }
        return theColors[TopInactiveShadedHighlight];

    case TopInactiveShadedShadow:
        if ((theColors[TopInactiveShadedShadow] == 0) &&
            !getTopInactiveShadedShadowColor()) {
            return 0;
        }
        return theColors[TopInactiveShadedShadow];
    }

    return 0;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getColor(VColorScheme::Color color_scheme_color,
                               short& red, short& green, short& blue)
{
    VColor *c = getColor(color_scheme_color);

    if (c != 0) {
        red   = c->getRed();
        green = c->getGreen();
        blue  = c->getBlue();
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getColor(VColorScheme::Color color_scheme_color,
                               VColor& color)
{
    VColor *c = getColor(color_scheme_color);

    if (c != 0) {
        color = *c;
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// --------------------------------------------------------------------------
//
void VColorScheme::setColor(VColorScheme::Color color_scheme_color,
                            VColor& color)
{
    switch (color_scheme_color) {

    case FlatBackground:
        if (theColors[FlatBackground] != 0) {
            *theColors[FlatBackground] = color;
        }
        else {
            theColors[FlatBackground] = new VColor(color);
        }
        break;

    case FlatForeground:
        if (theColors[FlatForeground] != 0) {
            *theColors[FlatForeground] = color;
        }
        else {
            theColors[FlatForeground] = new VColor(color);
        }
        break;

    case FlatBackgroundSelect:
        if (theColors[FlatBackgroundSelect] != 0) {
            *theColors[FlatBackgroundSelect] = color;
        }
        else {
            theColors[FlatBackgroundSelect] = new VColor(color);
        }
        break;

    case FlatForegroundSelect:
        if (theColors[FlatForegroundSelect] != 0) {
            *theColors[FlatForegroundSelect] = color;
        }
        else {
            theColors[FlatForegroundSelect] = new VColor(color);
        }
        break;

    case ShadedBackground:
        if (theColors[ShadedBackground] != 0) {
            *theColors[ShadedBackground] = color;
        }
        else {
            theColors[ShadedBackground] = new VColor(color);
        }
        break;

    case ShadedForeground:
        if (theColors[ShadedForeground] != 0) {
            *theColors[ShadedForeground] = color;
        }
        else {
            theColors[ShadedForeground] = new VColor(color);
        }
        break;

    case ShadedBackgroundSelect:
        if (theColors[ShadedBackgroundSelect] != 0) {
            *theColors[ShadedBackgroundSelect] = color;
        }
        else {
            theColors[ShadedBackgroundSelect] = new VColor(color);
        }
        break;

    case ShadedForegroundSelect:
        if (theColors[ShadedForegroundSelect] != 0) {
            *theColors[ShadedForegroundSelect] = color;
        }
        else {
            theColors[ShadedForegroundSelect] = new VColor(color);
        }
        break;

    case ShadedHighlight:
        if (theColors[ShadedHighlight] != 0) {
            *theColors[ShadedHighlight] = color;
        }
        else {
            theColors[ShadedHighlight] = new VColor(color);
        }
        break;

    case ShadedShadow:
        if (theColors[ShadedShadow] != 0) {
            *theColors[ShadedShadow] = color;
        }
        else {
            theColors[ShadedShadow] = new VColor(color);
        }
        break;
    }
}

// --------------------------------------------------------------------------
//
void VColorScheme::useNativeColorCalculation(boolean b)
{

#if defined(CV_MOTIF)

    boolean reinitialize_color_array = FALSE;

    if (b) {
        if (!useNativeColorCalculationFunction) {
            useNativeColorCalculationFunction = 1;
            reinitialize_color_array = TRUE;
        }
    }
    else if (useNativeColorCalculationFunction) {
        useNativeColorCalculationFunction = 0;
        reinitialize_color_array = TRUE;
    }

    if (reinitialize_color_array) {
        VColor *flat_background = 0, *shaded_background = 0;
        if (useGivenFlatBackground && (theColors != 0) &&
                             (theColors[FlatBackground] != 0)) {
            flat_background = new VColor(*theColors[FlatBackground]);
        }
        if (useGivenShadedBackground && (theColors != 0) &&
                             (theColors[ShadedBackground] != 0)) {
            shaded_background = new VColor(*theColors[ShadedBackground]);
        }
        reinitializeColorArray();
        theColors[FlatBackground] = flat_background;
        theColors[ShadedBackground] = shaded_background;
    }

#endif

}

// --------------------------------------------------------------------------
//
boolean VColorScheme::usingNativeColorCalculation() const
{
    return useNativeColorCalculationFunction;
}

// --------------------------------------------------------------------------
//
static int  getColorBrightness(const VColor&);
static void calculateForegroundColor(const VColor&, VColor&);
static void calculateShadeHighlightColor(const VColor&, VColor&);
static void calculateShadeShadowColor(const VColor&, VColor&);
static void calculateFlatBackgroundColor(const VColor&, VColor&);

// --------------------------------------------------------------------------
//
static const int    MaxRGB              = 255;
static const float  LightThreshold      = 0.77F * MaxRGB;
static const float  DarkThreshold       = 0.15F * MaxRGB;
static const float  ForegroundThreshold = 0.35F * MaxRGB;

// --------------------------------------------------------------------------
//
static int getColorBrightness(const VColor& color)
{
    const float RedLuminosity       = 0.30F;
    const float GreenLuminosity     = 0.59F;
    const float BlueLuminosity      = 0.11F;
    const int   IntensityFactor     = 25;
    const int   LightnessFactor     = 0;
    const int   LuminosityFactor    = 75;

    int red         = color.getRed();
    int green       = color.getGreen();
    int blue        = color.getBlue();
    int intensity   = (red + green + blue) / 3;
    int luminosity  = int((RedLuminosity   * float(red)) +
                          (GreenLuminosity * float(green)) +
                          (BlueLuminosity  * float(blue)));
    int max         = (red > green) ? ((red   > blue) ? red   : blue) :
                                      ((green > blue) ? green : blue);
    int min         = (red < green) ? ((red   < blue) ? red   : blue) :
                                      ((green < blue) ? green : blue);
    int lightness   = (min + max) / 2;
    int brightness  = ((intensity  * IntensityFactor) +
                       (lightness  * LightnessFactor) +
                       (luminosity * LuminosityFactor)) / 100;
    return brightness;
}

// --------------------------------------------------------------------------
//
static void calculateForegroundColor(const VColor& color,
                                     VColor& result_color)
{
    int red         = color.getRed();
    int green       = color.getGreen();
    int blue        = color.getBlue();
    int brightness  = getColorBrightness(color);

    if (float(brightness) < DarkThreshold) {
        red = green = blue = MaxRGB;
    }
    else if (float(brightness) > LightThreshold) {
        red = green = blue = 0;
    }
    else if (float(brightness) > ForegroundThreshold) {
        red = green = blue = 0;
    }
    else {
        red = green = blue = MaxRGB;
    }
    result_color.setRGB(red, green, blue);
}

// --------------------------------------------------------------------------
//
static void calculateShadeHighlightColor(const VColor& color,
                                         VColor& result_color)
{
    const int   DarkShadeHighlightFactor    = 50;
    const int   LightShadeHighlightFactor   = 20;
    const int   HighShadeHighlightFactor    = 70;
    const int   LowShadeHighlightFactor     = 40;

    int red         = color.getRed();
    int green       = color.getGreen();
    int blue        = color.getBlue();
    int brightness  = getColorBrightness(color);

    if (float(brightness) < DarkThreshold) {
        red   += DarkShadeHighlightFactor * (MaxRGB - red)   / 100;
        green += DarkShadeHighlightFactor * (MaxRGB - green) / 100;
        blue  += DarkShadeHighlightFactor * (MaxRGB - blue)  / 100;
    }
    else if (float(brightness) > LightThreshold) {
        red   -= (LightShadeHighlightFactor * red)   / 100;
        green -= (LightShadeHighlightFactor * green) / 100;
        blue  -= (LightShadeHighlightFactor * blue)  / 100;
    }
    else {
        int f = LowShadeHighlightFactor +
                (brightness * (HighShadeHighlightFactor -
                               LowShadeHighlightFactor) / MaxRGB);
        red   += f * (MaxRGB - red)   / 100;
        green += f * (MaxRGB - green) / 100;
        blue  += f * (MaxRGB - blue)  / 100;
    }
    result_color.setRGB(red, green, blue);
}

// --------------------------------------------------------------------------
// Calculate the appropriate shadow colors from the given shade-background
// color.
//
static void calculateShadeShadowColor(const VColor& color,
                                      VColor& result_color)
{
    const int   DarkShadeShadowFactor       = 30;
    const int   LightShadeShadowFactor      = 45;
    const int   HighShadeShadowFactor       = 35;
    const int   LowShadeShadowFactor        = 60;

    int red         = color.getRed();
    int green       = color.getGreen();
    int blue        = color.getBlue();
    int brightness  = getColorBrightness(color);

    if (float(brightness) < DarkThreshold) {
        red   += DarkShadeShadowFactor * (MaxRGB - red)   / 100;
        green += DarkShadeShadowFactor * (MaxRGB - green) / 100;
        blue  += DarkShadeShadowFactor * (MaxRGB - blue)  / 100;
    }
    else if (float(brightness) > LightThreshold) {
        red   -= (LightShadeShadowFactor * red)   / 100;
        green -= (LightShadeShadowFactor * green) / 100;
        blue  -= (LightShadeShadowFactor * blue)  / 100;
    }
    else {
        int f = LowShadeShadowFactor -
                (brightness * (LowShadeShadowFactor -
                               HighShadeShadowFactor) / MaxRGB);
        red   -= (f * red)   / 100;
        green -= (f * green) / 100;
        blue  -= (f * blue)  / 100;
    }
    result_color.setRGB(red, green, blue);
}

// --------------------------------------------------------------------------
// Calculate the non-shade background color from the given shade-background
// color, under OSF/Motif this color is referred to as the "select" color
// and is used for "armed" push-buttons as well as for the "client" area
// certain controls like read-write edit-boxes, and list-boxes (sometimes);
// it should be roughly 15% darker the the shade-background.  This is nearly
// equivalent to the white background used for the client area of the same
// kinds of controls under MS/Windows (3.1 with ctrl3d, and 95).
//
static void calculateFlatBackgroundColor(const VColor& color,
                        	             VColor& result_color)
{
    const int   DarkBackgroundFactor        = 15;
    const int   LightBackgroundFactor       = 15;
    const int   HighBackgroundFactor        = 15;
    const int   LowBackgroundFactor         = 15;

    int red         = color.getRed();
    int green       = color.getGreen();
    int blue        = color.getBlue();
    int brightness  = getColorBrightness(color);

    if (float(brightness) < DarkThreshold) {
        red   += DarkBackgroundFactor * (MaxRGB - red)   / 100;
        green += DarkBackgroundFactor * (MaxRGB - green) / 100;
        blue  += DarkBackgroundFactor * (MaxRGB - blue)  / 100;
    }
    else if (float(brightness) > LightThreshold) {
        red   -= (LightBackgroundFactor * red)   / 100;
        green -= (LightBackgroundFactor * green) / 100;
        blue  -= (LightBackgroundFactor * blue)  / 100;
    }
    else {
        int f = LowBackgroundFactor +
                (brightness * (HighBackgroundFactor -
                               LowBackgroundFactor) / MaxRGB);
        red   -= (f * red)   / 100;
        green -= (f * green) / 100;
        blue  -= (f * blue)  / 100;
    }
    result_color.setRGB(red, green, blue);
}

// ==========================================================================
//
#if defined(CV_MOTIF)

// --------------------------------------------------------------------------
//
#include "notifier.h"

// --------------------------------------------------------------------------
//
static Pixel getBackgroundPixelFromWidget(Widget widget = 0);
static Pixel getForegroundPixelFromWidget(Widget widget = 0);
static Pixel getTopShadowPixelFromWidget(Widget widget = 0);
static Pixel getBottomShadowPixelFromWidget(Widget widget = 0);
static Pixel systemBackground, systemForeground;
static Pixel systemTopShadow, systemBottomShadow;

// --------------------------------------------------------------------------
//
static void getSystemColors()
{
	static int first_time = 1;

	if (!first_time) {
		return;
	}

	if (notifier->getTopLevel() == 0) {
		return;
	}

	first_time = 0;

	Widget top = XtVaCreatePopupShell("", overrideShellWidgetClass,
									  notifier->getTopLevel(),
									  XmNwidth, 1, XmNheight, 1,
									  XmNx, -1000, XmNy, -1000, 0);
	Widget primitive = XtVaCreateWidget("", xmPrimitiveWidgetClass,
									    top, XmNwidth, 1, XmNheight, 1, 0);
	XtVaGetValues(primitive, XmNbackground, &systemBackground,
							 XmNforeground, &systemForeground,
							 XmNtopShadowColor, &systemTopShadow,
							 XmNbottomShadowColor, &systemBottomShadow, 0);
}

// --------------------------------------------------------------------------
//
static Pixel getBackgroundPixelFromWidget(Widget widget)
{
    if (widget == 0) {
		getSystemColors();
        return systemBackground;
    }
    Pixel pixel;
    XtVaGetValues(widget, XmNbackground, &pixel, 0);
    return pixel;
}

// --------------------------------------------------------------------------
//
static Pixel getForegroundPixelFromWidget(Widget widget)
{
    if (widget == 0) {
		getSystemColors();
        return systemForeground;
    }
    Pixel pixel;
    XtVaGetValues(widget, XmNforeground, &pixel, 0);
    return pixel;
}

// --------------------------------------------------------------------------
//
static Pixel getTopShadowPixelFromWidget(Widget widget)
{
    if (widget == 0) {
		getSystemColors();
        return systemTopShadow;
    }
    Pixel pixel;
    XtVaGetValues(widget, XmNtopShadowColor, &pixel, 0);
    return pixel;
}

// --------------------------------------------------------------------------
//
static Pixel getBottomShadowPixelFromWidget(Widget widget)
{
    if (widget == 0) {
		getSystemColors();
        return systemBottomShadow;
    }
    Pixel pixel;
    XtVaGetValues(widget, XmNbottomShadowColor, &pixel, 0);
    return pixel;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatBackgroundColor()
{
    if (theColors[FlatBackground] != 0) {
        return TRUE;
    }
    if (useWindow && (theWindow != 0)) {
        theColors[FlatBackground] =
            new VColor(getBackgroundPixelFromWidget(theWindow->hwnd()));
    }
    else {
        theColors[FlatBackground] =
            new VColor(getBackgroundPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatForegroundColor()
{
    if (theColors[FlatForeground] != 0) {
        return TRUE;
    }
    if (useGivenFlatBackground && getFlatBackgroundColor()) {
        if (useNativeColorCalculationFunction) {
            Pixel foreground_pixel;
            XmGetColors(notifier->defaultScreen(),
                        notifier->defaultColorMap(),
                        Pixel(long(*theColors[FlatBackground])),
                        &foreground_pixel, 0, 0, 0);
            theColors[FlatForeground] = new VColor(foreground_pixel);
        }
        else {
            theColors[FlatForeground] = new VColor;
            calculateForegroundColor
                (*theColors[FlatBackground], *theColors[FlatForeground]);
        }
    }
    else if (useWindow && (theWindow != 0)) {
        theColors[FlatForeground] =
            new VColor(getForegroundPixelFromWidget(theWindow->hwnd()));
    }
    else {
        theColors[FlatForeground] =
            new VColor(getForegroundPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatBackgroundSelectColor()
{
    if (theColors[FlatBackgroundSelect] != 0) {
        return TRUE;
    }
    if (getFlatForegroundColor()) {
        theColors[FlatBackgroundSelect] =
            new VColor(*theColors[FlatForeground]);
    }
    else {
        theColors[FlatBackgroundSelect]=
            new VColor(getForegroundPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatForegroundSelectColor()
{
    if (theColors[FlatForegroundSelect] != 0) {
        return TRUE;
    }
    if (getFlatBackgroundColor()) {
        theColors[FlatForegroundSelect] =
            new VColor(*theColors[FlatBackground]);
    }
    else {
        theColors[FlatForegroundSelect]=
            new VColor(getBackgroundPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedBackgroundColor()
{
    if (theColors[ShadedBackground] != 0) {
        return TRUE;
    }
    if (useWindow && (theWindow != 0)) {
        theColors[ShadedBackground] =
            new VColor(getBackgroundPixelFromWidget(theWindow->hwnd()));
    }
    else {
        theColors[ShadedBackground] =
            new VColor(getBackgroundPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedForegroundColor()
{
    if (theColors[ShadedForeground] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        if (useNativeColorCalculationFunction) {
            Pixel shade_foreground_pixel;
            XmGetColors(notifier->defaultScreen(),
                        notifier->defaultColorMap(),
                        Pixel(long(*theColors[ShadedBackground])),
                        &shade_foreground_pixel, 0, 0, 0);
            theColors[ShadedForeground] = new VColor(shade_foreground_pixel);
        }
        else {
            theColors[ShadedForeground] = new VColor;
            calculateForegroundColor
                (*theColors[ShadedBackground], *theColors[ShadedForeground]);
        }
    }
    else if (useWindow && (theWindow != 0)) {
        theColors[ShadedForeground] =
            new VColor(getForegroundPixelFromWidget(theWindow->hwnd()));
    }
    else {
        theColors[ShadedForeground] =
                new VColor(getForegroundPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedBackgroundSelectColor()
{
    if (theColors[ShadedBackgroundSelect] != 0) {
        return TRUE;
    }
    if (getShadedForegroundColor()) {
        theColors[ShadedBackgroundSelect] =
            new VColor(*theColors[ShadedForeground]);
    }
    else {
        theColors[ShadedBackgroundSelect] =
                new VColor(getForegroundPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedForegroundSelectColor()
{
    if (theColors[ShadedForegroundSelect] != 0) {
        return TRUE;
    }
    if (getShadedBackgroundColor()) {
        theColors[ShadedForegroundSelect] =
            new VColor(*theColors[ShadedBackground]);
    }
    else {
        theColors[ShadedForegroundSelect] =
                new VColor(getBackgroundPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedHighlightColor()
{
    if (theColors[ShadedHighlight] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        if (useNativeColorCalculationFunction) {
            Pixel shade_highlight_pixel;
            XmGetColors(notifier->defaultScreen(),
                        notifier->defaultColorMap(),
                        Pixel(long(*theColors[ShadedBackground])),
                        0, &shade_highlight_pixel, 0, 0);
            theColors[ShadedHighlight] = new VColor(shade_highlight_pixel);
        }
        else {
            theColors[ShadedHighlight] = new VColor;
            calculateShadeHighlightColor
                (*theColors[ShadedBackground], *theColors[ShadedHighlight]);
        }
    }
    else if (useWindow && (theWindow != 0)) {
        theColors[ShadedHighlight] =
            new VColor(getTopShadowPixelFromWidget(theWindow->hwnd()));
    }
    else {
        theColors[ShadedHighlight] =
            new VColor(getTopShadowPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedDarkHighlightColor()
{
    if (theColors[ShadedDarkHighlight] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        if (useNativeColorCalculationFunction) {
            Pixel shade_highlight_pixel;
            XmGetColors(notifier->defaultScreen(),
                        notifier->defaultColorMap(),
                        Pixel(long(*theColors[ShadedBackground])),
                        0, &shade_highlight_pixel, 0, 0);
            theColors[ShadedDarkHighlight] =
                new VColor(shade_highlight_pixel);
        }
        else {
            theColors[ShadedDarkHighlight] = new VColor;
            calculateShadeHighlightColor
                (*theColors[ShadedBackground],
                 *theColors[ShadedDarkHighlight]);
        }
    }
    else if (useWindow && (theWindow != 0)) {
        theColors[ShadedDarkHighlight] =
            new VColor(getTopShadowPixelFromWidget(theWindow->hwnd()));
    }
    else {
        theColors[ShadedDarkHighlight] =
            new VColor(getTopShadowPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedShadowColor()
{
    if (theColors[ShadedShadow] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        if (useNativeColorCalculationFunction) {
            Pixel shade_shadow_pixel;
            XmGetColors(notifier->defaultScreen(),
                        notifier->defaultColorMap(),
                        Pixel(long(*theColors[ShadedBackground])),
                        0, 0, &shade_shadow_pixel, 0);
            theColors[ShadedShadow] = new VColor(shade_shadow_pixel);
        }
        else {
            theColors[ShadedShadow] = new VColor;
            calculateShadeShadowColor
                (*theColors[ShadedBackground], *theColors[ShadedShadow]);
        }
    }
    else if (useWindow && (theWindow != 0)) {
        theColors[ShadedShadow] =
            new VColor(getBottomShadowPixelFromWidget(theWindow->hwnd()));
    }
    else {
        theColors[ShadedShadow] =
            new VColor(getBottomShadowPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedDarkShadowColor()
{
    if (theColors[ShadedDarkShadow] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        if (useNativeColorCalculationFunction) {
            Pixel shade_shadow_pixel;
            XmGetColors(notifier->defaultScreen(),
                        notifier->defaultColorMap(),
                        Pixel(long(*theColors[ShadedBackground])),
                        0, 0, &shade_shadow_pixel, 0);
            theColors[ShadedDarkShadow] = new VColor(shade_shadow_pixel);
        }
        else {
            theColors[ShadedDarkShadow] = new VColor;
            calculateShadeShadowColor
                (*theColors[ShadedBackground], *theColors[ShadedDarkShadow]);
        }
    }
    else if (useWindow && (theWindow != 0)) {
        theColors[ShadedDarkShadow] =
            new VColor(getBottomShadowPixelFromWidget(theWindow->hwnd()));
    }
    else {
        theColors[ShadedDarkShadow] =
            new VColor(getBottomShadowPixelFromWidget());
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getToolTipBackgroundColor()
{
    if (theColors[ToolTipBackground] != 0) {
        return TRUE;
    }
    theColors[ToolTipBackground] = new VColor(VColor::Yellow);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getToolTipForegroundColor()
{
    if (theColors[ToolTipForeground] != 0) {
        return TRUE;
    }
    theColors[ToolTipForeground] = new VColor(VColor::Black);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveCaptionBackgroundColor()
{
	if (theColors[TopActiveCaptionBackground] != 0) {
		return TRUE;
	}
	theColors[TopActiveCaptionBackground] = new VColor(VColor::DarkBlue);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveCaptionForegroundColor()
{
	if (theColors[TopActiveCaptionForeground] != 0) {
		return TRUE;
	}
	theColors[TopActiveCaptionForeground] = new VColor(VColor::White);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveFlatBackgroundColor()
{
	if (theColors[TopActiveFlatBackground] != 0) {
		return TRUE;
	}
	theColors[TopActiveFlatBackground] = new VColor(VColor::DarkBlue);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveFlatBorderColor()
{
	if (theColors[TopActiveFlatBorder] != 0) {
		return TRUE;
	}
	theColors[TopActiveFlatBorder] = new VColor(VColor::Black);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveShadedBackgroundColor()
{
	if (theColors[TopActiveShadedBackground] != 0) {
		return TRUE;
	}
	theColors[TopActiveShadedBackground] = new VColor(VColor::Gray);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveShadedHighlightColor()
{
	if (theColors[TopActiveShadedHighlight] != 0) {
		return TRUE;
	}
	theColors[TopActiveShadedHighlight] = new VColor(VColor::LightGray);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveShadedShadowColor()
{
	if (theColors[TopActiveShadedShadow] != 0) {
		return TRUE;
	}
	theColors[TopActiveShadedShadow] = new VColor(VColor::DarkGray);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveCaptionBackgroundColor()
{
	if (theColors[TopInactiveCaptionBackground] != 0) {
		return TRUE;
	}
	theColors[TopInactiveCaptionBackground] = new VColor(VColor::DarkCyan);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveCaptionForegroundColor()
{
	if (theColors[TopInactiveCaptionForeground] != 0) {
		return TRUE;
	}
	theColors[TopInactiveCaptionForeground] = new VColor(VColor::LightGray);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveFlatBackgroundColor()
{
	if (theColors[TopInactiveFlatBackground] != 0) {
		return TRUE;
	}
	theColors[TopInactiveFlatBackground] = new VColor(VColor::DarkCyan);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveFlatBorderColor()
{
	if (theColors[TopInactiveFlatBorder] != 0) {
		return TRUE;
	}
	theColors[TopInactiveFlatBorder] = new VColor(VColor::Black);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveShadedBackgroundColor()
{
	if (theColors[TopInactiveShadedBackground] != 0) {
		return TRUE;
	}
	theColors[TopInactiveShadedBackground] = new VColor(VColor::Gray);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveShadedHighlightColor()
{
	if (theColors[TopInactiveShadedHighlight] != 0) {
		return TRUE;
	}
	theColors[TopInactiveShadedHighlight] = new VColor(VColor::LightGray);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveShadedShadowColor()
{
	if (theColors[TopInactiveShadedShadow] != 0) {
		return TRUE;
	}
	theColors[TopInactiveShadedShadow] = new VColor(VColor::DarkGray);
	return TRUE;
}

// ==========================================================================
//
#elif defined(CV_WINDOWS)

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatBackgroundColor()
{
    if (theColors[FlatBackground] != 0) {
        return TRUE;
    }
    theColors[FlatBackground] = new VColor(GetSysColor(COLOR_WINDOW));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatForegroundColor()
{
    if (theColors[FlatForeground] != 0) {
        return TRUE;
    }
    if (useGivenFlatBackground && getFlatBackgroundColor()) {
        theColors[FlatForeground] = new VColor;
        calculateForegroundColor
            (*theColors[FlatBackground], *theColors[FlatForeground]);
    }
    else {
        theColors[FlatForeground] =
            new VColor(GetSysColor(COLOR_WINDOWTEXT));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatBackgroundSelectColor()
{
    if (theColors[FlatBackgroundSelect] != 0) {
        return TRUE;
    }
    if (useGivenFlatBackground && getFlatForegroundColor()) {
        theColors[FlatBackgroundSelect] =
            new VColor(*theColors[FlatForeground]);
    }
    else {
        theColors[FlatBackgroundSelect] =
            new VColor(GetSysColor(COLOR_HIGHLIGHT));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatForegroundSelectColor()
{
    if (theColors[FlatForegroundSelect] != 0) {
        return TRUE;
    }
    if (useGivenFlatBackground && getFlatBackgroundColor()) {
        theColors[FlatForegroundSelect] =
            new VColor(*theColors[FlatBackground]);
    }
    else {
        theColors[FlatForegroundSelect] =
            new VColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedBackgroundColor()
{
    if (theColors[ShadedBackground] != 0) {
        return TRUE;
    }
    theColors[ShadedBackground] = new VColor(GetSysColor(COLOR_BTNFACE));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedForegroundColor()
{
    if (theColors[ShadedForeground] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedForeground] = new VColor;
        calculateForegroundColor
            (*theColors[ShadedBackground], *theColors[ShadedForeground]);
    }
    else {
        theColors[ShadedForeground] =
            new VColor(GetSysColor(COLOR_BTNTEXT));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedBackgroundSelectColor()
{
    if (theColors[ShadedBackgroundSelect] != 0) {
        return TRUE;
    }
    if (getShadedForegroundColor()) {
        theColors[ShadedBackgroundSelect] =
            new VColor(*theColors[ShadedForeground]);
    }
    else {
        theColors[ShadedBackgroundSelect] =
            new VColor(GetSysColor(COLOR_HIGHLIGHT));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedForegroundSelectColor()
{
    if (theColors[ShadedForegroundSelect] != 0) {
        return TRUE;
    }
    if (getShadedBackgroundColor()) {
        theColors[ShadedForegroundSelect] =
            new VColor(*theColors[ShadedBackground]);
    }
    else {
        theColors[ShadedBackgroundSelect] =
            new VColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedHighlightColor()
{
    if (theColors[ShadedHighlight] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedHighlight] = new VColor;
        calculateShadeHighlightColor
            (*theColors[ShadedBackground], *theColors[ShadedHighlight]);
    }
    else {
        theColors[ShadedHighlight] =
            new VColor(GetSysColor(COLOR_BTNHIGHLIGHT));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedDarkHighlightColor()
{
    if (theColors[ShadedDarkHighlight] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedDarkHighlight] = new VColor;
        calculateShadeHighlightColor
            (*theColors[ShadedBackground], *theColors[ShadedDarkHighlight]);
    }
    else {
        theColors[ShadedDarkHighlight] =
            new VColor(GetSysColor(COLOR_BTNFACE));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedShadowColor()
{
    if (theColors[ShadedShadow] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedShadow] = new VColor;
        calculateShadeShadowColor
            (*theColors[ShadedBackground], *theColors[ShadedShadow]);
    }
    else {
        theColors[ShadedShadow] = new VColor(GetSysColor(COLOR_BTNSHADOW));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedDarkShadowColor()
{
    if (theColors[ShadedDarkShadow] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedDarkShadow] = new VColor;
        calculateShadeShadowColor
            (*theColors[ShadedBackground], *theColors[ShadedDarkShadow]);
    }
    else {
        theColors[ShadedDarkShadow] =
            new VColor(GetSysColor(COLOR_WINDOWFRAME));
    }
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getToolTipBackgroundColor()
{
    if (theColors[ToolTipBackground] != 0) {
        return TRUE;
    }
#if defined(COLOR_INFOBK) && defined(COLOR_INFOTEXT)
	short major_version = LOBYTE(LOWORD(GetVersion()));
	if (major_version >= 4) {
	    theColors[ToolTipBackground] =
	        new VColor(GetSysColor(COLOR_INFOBK));
	}
	else {
    	theColors[ToolTipBackground] = new VColor(VColor::Yellow);
	}
#else
    theColors[ToolTipBackground] = new VColor(VColor::Yellow);
#endif
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getToolTipForegroundColor()
{
    if (theColors[ToolTipForeground] != 0) {
        return TRUE;
    }
#if defined(COLOR_INFOBK) && defined(COLOR_INFOTEXT)
	short major_version = LOBYTE(LOWORD(GetVersion()));
	if (major_version >= 4) {
	    theColors[ToolTipForeground] =
	        new VColor(GetSysColor(COLOR_INFOTEXT));
	}
	else {
    	theColors[ToolTipForeground] = new VColor(VColor::Black);
	}
#else
    theColors[ToolTipForeground] = new VColor(VColor::Black);
#endif
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveCaptionBackgroundColor()
{
    if (theColors[TopActiveCaptionBackground] != 0) {
        return TRUE;
    }
    theColors[TopActiveCaptionBackground] =
        new VColor(GetSysColor(COLOR_ACTIVECAPTION));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveCaptionForegroundColor()
{
    if (theColors[TopActiveCaptionForeground] != 0) {
        return TRUE;
    }
    theColors[TopActiveCaptionForeground] =
        new VColor(GetSysColor(COLOR_CAPTIONTEXT));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveFlatBackgroundColor()
{
    if (theColors[TopActiveFlatBackground] != 0) {
        return TRUE;
    }
    theColors[TopActiveFlatBackground] =
        new VColor(GetSysColor(COLOR_ACTIVEBORDER));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveFlatBorderColor()
{
    if (theColors[TopActiveFlatBorder] != 0) {
        return TRUE;
    }
    theColors[TopActiveFlatBorder] =
        new VColor(GetSysColor(COLOR_WINDOWFRAME));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveShadedBackgroundColor()
{
    if (theColors[TopActiveShadedBackground] != 0) {
        return TRUE;
    }
    if (!getShadedBackgroundColor()) {
        return FALSE;
    }
    theColors[TopActiveShadedBackground] =
        new VColor(*theColors[ShadedBackground]);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveShadedHighlightColor()
{
    if (theColors[TopActiveShadedHighlight] != 0) {
        return TRUE;
    }
    if (!getShadedHighlightColor()) {
        return FALSE;
    }
    theColors[TopActiveShadedHighlight] =
        new VColor(*theColors[ShadedHighlight]);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveShadedShadowColor()
{
    if (theColors[TopActiveShadedShadow] != 0) {
        return TRUE;
    }
    if (!getShadedHighlightColor()) {
        return FALSE;
    }
    theColors[TopActiveShadedShadow] =
        new VColor(*theColors[ShadedShadow]);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveCaptionBackgroundColor()
{
    if (theColors[TopInactiveCaptionBackground] != 0) {
        return TRUE;
    }
    theColors[TopInactiveCaptionBackground] =
        new VColor(GetSysColor(COLOR_INACTIVECAPTION));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveCaptionForegroundColor()
{
    if (theColors[TopInactiveCaptionForeground] != 0) {
        return TRUE;
    }
    theColors[TopInactiveCaptionForeground] =
        new VColor(GetSysColor(COLOR_INACTIVECAPTIONTEXT));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveFlatBackgroundColor()
{
    if (theColors[TopInactiveFlatBackground] != 0) {
        return TRUE;
    }
    theColors[TopInactiveFlatBackground] =
        new VColor(GetSysColor(COLOR_INACTIVEBORDER));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveFlatBorderColor()
{
    if (theColors[TopInactiveFlatBorder] != 0) {
        return TRUE;
    }
    theColors[TopInactiveFlatBorder] =
        new VColor(GetSysColor(COLOR_WINDOWFRAME));
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveShadedBackgroundColor()
{
    if (theColors[TopInactiveShadedBackground] != 0) {
        return TRUE;
    }
    if (!getShadedBackgroundColor()) {
        return FALSE;
    }
    theColors[TopInactiveShadedBackground] =
        new VColor(*theColors[ShadedBackground]);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveShadedHighlightColor()
{
    if (theColors[TopInactiveShadedHighlight] != 0) {
        return TRUE;
    }
    if (!getShadedHighlightColor()) {
        return FALSE;
    }
    theColors[TopInactiveShadedHighlight] =
        new VColor(*theColors[ShadedHighlight]);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveShadedShadowColor()
{
    if (theColors[TopInactiveShadedShadow] != 0) {
        return TRUE;
    }
    if (!getShadedHighlightColor()) {
        return FALSE;
    }
    theColors[TopInactiveShadedShadow] =
        new VColor(*theColors[ShadedShadow]);
    return TRUE;
}

// ==========================================================================
//
#elif defined(PM)

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatBackgroundColor()
{
    if (theColors[FlatBackground] != 0) {
        return TRUE;
    }
    theColors[FlatBackground] = new VColor(VColor::White);

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatForegroundColor()
{
    if (theColors[FlatForeground] != 0) {
        return TRUE;
    }
    if (useGivenFlatBackground && getFlatBackgroundColor()) {
        theColors[FlatForeground] = new VColor;
        calculateForegroundColor
            (*theColors[FlatBackground], *theColors[FlatForeground]);
    }
    else {
        theColors[FlatForeground] = new VColor(VColor::Black);
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatBackgroundSelectColor()
{
    if (theColors[FlatBackgroundSelect] != 0) {
        return TRUE;
    }
    if (useGivenFlatBackground && getFlatForegroundColor()) {
        theColors[FlatBackgroundSelect] =
            new VColor(*theColors[FlatForeground]);
    }
    else {
        theColors[FlatBackgroundSelect] = new VColor(VColor::Black); // ToDo!!!
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getFlatForegroundSelectColor()
{
    if (theColors[FlatForegroundSelect] != 0) {
        return TRUE;
    }
    if (useGivenFlatBackground && getFlatBackgroundColor()) {
        theColors[FlatForegroundSelect] =
            new VColor(*theColors[FlatBackground]);
    }
    else {
        theColors[FlatForegroundSelect] = new VColor(VColor::White);
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedBackgroundColor()
{
    if (theColors[ShadedBackground] != 0) {
        return TRUE;
    }
    theColors[ShadedBackground] = new VColor(VColor::LightGray); // ToDo!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedForegroundColor()
{
    if (theColors[ShadedForeground] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedForeground] = new VColor;
        calculateForegroundColor
            (*theColors[ShadedBackground], *theColors[ShadedForeground]);
    }
    else {
        theColors[ShadedForeground] = new VColor(VColor::Black);
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedBackgroundSelectColor()
{
    if (theColors[ShadedBackgroundSelect] != 0) {
        return TRUE;
    }
    if (getShadedForegroundColor()) {
        theColors[ShadedBackgroundSelect] =
            new VColor(*theColors[ShadedForeground]);
    }
    else {
        theColors[ShadedBackgroundSelect] = new VColor(VColor::Black); // ToDo!!!
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedForegroundSelectColor()
{
    if (theColors[ShadedForegroundSelect] != 0) {
        return TRUE;
    }
    if (getShadedBackgroundColor()) {
        theColors[ShadedForegroundSelect] =
            new VColor(*theColors[ShadedBackground]);
    }
    else {
        theColors[ShadedBackgroundSelect] = new VColor(VColor::Gray);
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedHighlightColor()
{
    if (theColors[ShadedHighlight] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedHighlight] = new VColor;
        calculateShadeHighlightColor
            (*theColors[ShadedBackground], *theColors[ShadedHighlight]);
    }
    else {
        theColors[ShadedHighlight] = new VColor(VColor::White); // ToDo!!!
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedDarkHighlightColor()
{
    if (theColors[ShadedDarkHighlight] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedDarkHighlight] = new VColor;
        calculateShadeHighlightColor
            (*theColors[ShadedBackground], *theColors[ShadedDarkHighlight]);
    }
    else {
        theColors[ShadedDarkHighlight] = new VColor(VColor::Gray);
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedShadowColor()
{
    if (theColors[ShadedShadow] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedShadow] = new VColor;
        calculateShadeShadowColor
            (*theColors[ShadedBackground], *theColors[ShadedShadow]);
    }
    else {
        theColors[ShadedShadow] = new VColor(VColor::DarkGray); // ToDo!!!
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getShadedDarkShadowColor()
{
    if (theColors[ShadedDarkShadow] != 0) {
        return TRUE;
    }
    if (useGivenShadedBackground && getShadedBackgroundColor()) {
        theColors[ShadedDarkShadow] = new VColor;
        calculateShadeShadowColor
            (*theColors[ShadedBackground], *theColors[ShadedDarkShadow]);
    }
    else {
        theColors[ShadedDarkShadow] = new VColor(VColor::Black); // ToDo!!!
    }
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getToolTipBackgroundColor()
{
    if (theColors[ToolTipBackground] != 0) {
        return TRUE;
    }
    theColors[ToolTipBackground] = new VColor(VColor::Yellow);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getToolTipForegroundColor()
{
    if (theColors[ToolTipForeground] != 0) {
        return TRUE;
    }
    theColors[ToolTipForeground] = new VColor(VColor::Black);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveCaptionBackgroundColor()
{
    if (theColors[TopActiveCaptionBackground] != 0) {
        return TRUE;
    }
    theColors[TopActiveCaptionBackground] = new VColor(VColor::DarkBlue); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveCaptionForegroundColor()
{
    if (theColors[TopActiveCaptionForeground] != 0) {
        return TRUE;
    }
    theColors[TopActiveCaptionForeground] = new VColor(VColor::White); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveFlatBackgroundColor()
{
    if (theColors[TopActiveFlatBackground] != 0) {
        return TRUE;
    }
    theColors[TopActiveFlatBackground] = new VColor(VColor::DarkBlue); // ToDo!!!
    
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveFlatBorderColor()
{
    if (theColors[TopActiveFlatBorder] != 0) {
        return TRUE;
    }
    theColors[TopActiveFlatBorder] = new VColor(VColor::Black); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveShadedBackgroundColor()
{
    if (theColors[TopActiveShadedBackground] != 0) {
        return TRUE;
    }
    if (!getShadedBackgroundColor()) {
        return FALSE;
    }
    theColors[TopActiveShadedBackground] = new VColor(VColor::Gray); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveShadedHighlightColor()
{
    if (theColors[TopActiveShadedHighlight] != 0) {
        return TRUE;
    }
    if (!getShadedHighlightColor()) {
        return FALSE;
    }
    theColors[TopActiveShadedHighlight] = new VColor(VColor::LightGray); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopActiveShadedShadowColor()
{
    if (theColors[TopActiveShadedShadow] != 0) {
        return TRUE;
    }
    if (!getShadedHighlightColor()) {
        return FALSE;
    }
    theColors[TopActiveShadedShadow] = new VColor(VColor::DarkGray); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveCaptionBackgroundColor()
{
    if (theColors[TopInactiveCaptionBackground] != 0) {
        return TRUE;
    }
    theColors[TopInactiveCaptionBackground] = new VColor(VColor::Gray); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveCaptionForegroundColor()
{
    if (theColors[TopInactiveCaptionForeground] != 0) {
        return TRUE;
    }
    theColors[TopInactiveCaptionForeground] = new VColor(VColor::DarkGray);

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveFlatBackgroundColor()
{
    if (theColors[TopInactiveFlatBackground] != 0) {
        return TRUE;
    }
    theColors[TopInactiveFlatBackground] = new VColor(VColor::Gray); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveFlatBorderColor()
{
    if (theColors[TopInactiveFlatBorder] != 0) {
        return TRUE;
    }
    theColors[TopInactiveFlatBorder] = new VColor(VColor::Black); // ToDo!!!
    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveShadedBackgroundColor()
{
    if (theColors[TopInactiveShadedBackground] != 0) {
        return TRUE;
    }
    if (!getShadedBackgroundColor()) {
        return FALSE;
    }
    theColors[TopInactiveShadedBackground] = new VColor(VColor::Gray); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveShadedHighlightColor()
{
    if (theColors[TopInactiveShadedHighlight] != 0) {
        return TRUE;
    }
    if (!getShadedHighlightColor()) {
        return FALSE;
    }
    theColors[TopInactiveShadedHighlight] = new VColor(VColor::LightGray); // ToDo!!!

    return TRUE; // ToDo!!!
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getTopInactiveShadedShadowColor()
{
    if (theColors[TopInactiveShadedShadow] != 0) {
        return TRUE;
    }
    if (!getShadedHighlightColor()) {
        return FALSE;
    }
    theColors[TopInactiveShadedShadow] = new VColor(VColor::DarkGray); // ToDo!!!

    return TRUE; // ToDo!!!
}

// ==========================================================================
//
#else

#   error "Time to port VColorScheme."

#endif

// --------------------------------------------------------------------------
//
boolean VColorScheme::getToolTipHighlightColor()
{
    if (theColors[ToolTipHighlight] != 0) {
        return TRUE;
    }
	if (!getToolTipBackgroundColor()) {
		return FALSE;
	}
	theColors[ToolTipHighlight] = new VColor;
	calculateShadeHighlightColor(*theColors[ToolTipBackground],
								 *theColors[ToolTipHighlight]);
    return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::getToolTipShadowColor()
{
    if (theColors[ToolTipShadow] != 0) {
        return TRUE;
    }
	if (!getShadedDarkShadowColor()) {
		return FALSE;
	}
	theColors[ToolTipShadow] = new VColor(*theColors[ShadedDarkShadow]);
    return TRUE;
}

// ==========================================================================


// --------------------------------------------------------------------------
// This apparent nonsense is to ensure that the static system VColorScheme
// and associated VBrushes and VPens, if created, are deleted on program
// termination.  The reason we don't use global objects is simply so to
// avoid the restriction that the user may not delete object returned by
// VColorScheme::system() and also so that these objects aren't created
// unless they're asked for.
//
static void VDummyDtor(void);
struct VDummy { ~VDummy() { VDummyDtor(); } };
static VDummy CV_NEARDATA dummy;
static int dummy_deletion_in_progress = 0;

static void VDummyDtor(void)
{
	dummy_deletion_in_progress = 1;
    if (system_colorscheme != 0) {
        delete system_colorscheme;
        system_colorscheme = 0;
    }
    if (system_brushes != 0) {
        for (int i = 0 ; i < VColorScheme::MaxColors ; i++) {
            if (system_brushes[i] != 0) {
                delete system_brushes[i];
            }
        }
        delete [] system_brushes;
        system_brushes = 0;
    }
    if (system_pens != 0) {
        for (int i = 0 ; i < VColorScheme::MaxColors ; i++) {
            if (system_pens[i] != 0) {
                delete system_pens[i];
            }
        }
        delete [] system_pens;
        system_pens = 0;
    }
	dummy_deletion_in_progress = 0;
}

// --------------------------------------------------------------------------
// Update our system colors.  Note that we take extra care to ensure that
// anyone who has previously obtained a system VColor, VBrush, or VPen
// pointer will still have a valid pointer.
//
void VColorScheme::systemColorsChanged()
{
    if ((system_colorscheme != 0) && (system_colorscheme->theColors != 0)) {
        VColorScheme new_system_colorscheme;
        for (int i = 0 ; i < MaxColors ; i++) {
            if (system_colorscheme->theColors[i] != 0) {
                *system_colorscheme->theColors[i] =
                    *new_system_colorscheme.getColor(Color(i));
            }
        }
    }
    if (system_brushes != 0) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (system_brushes[i] != 0) {
                system_brushes[i]->background(systemColor(Color(i)));
                system_brushes[i]->foreground(systemColor(Color(i)));
            }
        }
    }
    if (system_pens != 0) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (system_pens[i] != 0) {
                system_pens[i]->color(systemColor(Color(i)));
            }
        }
    }
}

// --------------------------------------------------------------------------
//
VColorScheme& VColorScheme::system()
{
    if (system_colorscheme == 0) {
        system_colorscheme = new VColorScheme;
    }
    return *system_colorscheme;
}

// --------------------------------------------------------------------------
//
VColor& VColorScheme::systemColor(VColorScheme::Color color)
{
    if (system_colorscheme == 0) {
        system_colorscheme = new VColorScheme;
    }
    return *system_colorscheme->getColor(color);
}

// --------------------------------------------------------------------------
//
VBrush& VColorScheme::systemBrush(VColorScheme::Color color)
{
    if (system_brushes == 0) {
        system_brushes = new VBrush *[MaxColors];
        for (int i = 0 ; i < MaxColors ; i++) {
            system_brushes[i] = 0;
        }
    }
    if (system_brushes[int(color)] == 0) {
        system_brushes[int(color)] = new VBrush(systemColor(color));
    }
    return *system_brushes[int(color)];
}

// --------------------------------------------------------------------------
//
VPen& VColorScheme::systemPen(VColorScheme::Color color)
{
    if (system_pens == 0) {
        system_pens = new VPen *[MaxColors];
        for (int i = 0 ; i < MaxColors ; i++) {
            system_pens[i] = 0;
        }
    }
    if (system_pens[int(color)] == 0) {
        system_pens[int(color)] = new VPen(systemColor(color));
    }
    return *system_pens[int(color)];
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::isSystemColor(VColor *color)
{
    if ((system_colorscheme != 0) && (system_colorscheme->theColors != 0)) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (system_colorscheme->theColors[i] == color) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::isSystemBrush(VBrush *brush)
{
    if (system_brushes != 0) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (system_brushes[i] == brush) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::isSystemPen(VPen *pen)
{
    if (system_pens != 0) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (system_pens[i] == pen) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::checkSystemColorDeletion(VColor *color)
{
    if ((system_colorscheme != 0) && (system_colorscheme->theColors != 0)) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (system_colorscheme->theColors[i] == color) {
                system_colorscheme->theColors[i] = 0;
                return dummy_deletion_in_progress ? FALSE : TRUE;
            }
        }
    }
    return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::checkSystemBrushDeletion(VBrush *brush)
{
    if (system_brushes != 0) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (system_brushes[i] == brush) {
                system_brushes[i] = 0;
                return dummy_deletion_in_progress ? FALSE : TRUE;
            }
        }
    }
    return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VColorScheme::checkSystemPenDeletion(VPen *pen)
{
    if (system_pens != 0) {
        for (int i = 0 ; i < MaxColors ; i++) {
            if (system_pens[i] == pen) {
                system_pens[i] = 0;
                return dummy_deletion_in_progress ? FALSE : TRUE;
            }
        }
    }
    return FALSE;
}
