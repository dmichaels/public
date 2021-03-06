//	ColorScheme.java
//
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
//	07/17/97 dgm	Original.
// --------------------------------------------------------------------------

import java.awt.*;

// --------------------------------------------------------------------------
//
class ColorScheme extends Object
{
	// ----------------------------------------------------------------------
	//
	static public Color getShadow(Color color)
	{
		final int   DarkShadeShadowFactor	= 30;
		final int   LightShadeShadowFactor	= 45;
		final int   HighShadeShadowFactor	= 35;
		final int   LowShadeShadowFactor	= 60;

		int red         = color.getRed();
		int green       = color.getGreen();
		int blue        = color.getBlue();
		int brightness  = getColorBrightness(color);
		
		if (brightness < DarkThreshold) {
			red   += DarkShadeShadowFactor * (MaxRGB - red)   / 100;
			green += DarkShadeShadowFactor * (MaxRGB - green) / 100;
			blue  += DarkShadeShadowFactor * (MaxRGB - blue)  / 100;
		}
		else if (brightness > LightThreshold) {
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
		return new Color(red, green, blue);
	}

	// ----------------------------------------------------------------------
	//
	static public Color getHighlight(Color color)
	{
		final int   DarkShadeHighlightFactor	= 50;
		final int   LightShadeHighlightFactor	= 20;
		final int   HighShadeHighlightFactor	= 70;
		final int   LowShadeHighlightFactor		= 40;

		int red			= color.getRed();
		int green		= color.getGreen();
		int blue		= color.getBlue();
		int brightness	= getColorBrightness(color);

		if (brightness < DarkThreshold) {
			red   += DarkShadeHighlightFactor * (MaxRGB - red)   / 100;
			green += DarkShadeHighlightFactor * (MaxRGB - green) / 100;
			blue  += DarkShadeHighlightFactor * (MaxRGB - blue)  / 100;
		}
		else if (brightness > LightThreshold) {
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
		return new Color(red, green, blue);
	}

	// ----------------------------------------------------------------------
	//
	static public Color getDarkShadow(Color color)
	{
		return getShadow(color).darker();
	}

	// ----------------------------------------------------------------------
	//
	static public Color getDarkHighlight(Color color)
	{
		return getHighlight(color).darker();
	}

	// ----------------------------------------------------------------------
	//
	static public Color getForeground(Color color)
	{
		int red			= color.getRed();
		int green		= color.getGreen();
		int blue		= color.getBlue();
		int brightness	= getColorBrightness(color);

		if (brightness < DarkThreshold) {
			red = green = blue = MaxRGB;
		}
		else if (brightness > LightThreshold) {
			red = green = blue = 0;
		}
		else if (brightness > ForegroundThreshold) {
			red = green = blue = 0;
		}
		else {
			red = green = blue = MaxRGB;
		}
		return new Color(red, green, blue);
	}

	// ----------------------------------------------------------------------
	//
	static protected final int    MaxRGB				= 255;
	static protected final float  LightThreshold		= 0.77F * MaxRGB;
	static protected final float  DarkThreshold			= 0.15F * MaxRGB;
	static protected final float  ForegroundThreshold	= 0.35F * MaxRGB;

	// ----------------------------------------------------------------------
	//
	static protected int getColorBrightness(Color color)
	{
		final float RedLuminosity		= 0.30F;
		final float GreenLuminosity		= 0.59F;
		final float BlueLuminosity		= 0.11F;
		final int   IntensityFactor		= 25;
		final int   LightnessFactor		= 0;
		final int   LuminosityFactor	= 75;

		int red			= color.getRed();
		int green		= color.getGreen();
		int blue		= color.getBlue();
		int intensity	= (red + green + blue) / 3;
		int luminosity	= (int)((RedLuminosity   * red) +
								(GreenLuminosity * green) +
								(BlueLuminosity  * blue));
		int max			= (red > green) ? ((red   > blue) ? red   : blue) :
										  ((green > blue) ? green : blue);
		int min			= (red < green) ? ((red   < blue) ? red   : blue) :
										  ((green < blue) ? green : blue);
		int lightness	= (min + max) / 2;
		int brightness	= ((intensity  * IntensityFactor) +
						   (lightness  * LightnessFactor) +
						   (luminosity * LuminosityFactor)) / 100;
		return brightness;
	}
}
