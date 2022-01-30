//	TetrisPiece.java
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
import java.util.*;
import ColorScheme;
import TetrisView;

// --------------------------------------------------------------------------
//
public class TetrisPiece extends Object
{
	// ----------------------------------------------------------------------
	//
	public static final int	TypeTrivial					=   0;
	public static final int	TypeOne						=   1;
	public static final int	TypeTwo						=   2;
	public static final int	TypeThree					=   3;
	public static final int	TypeFour					=   4;
	public static final int	TypeFive					=   5;
	public static final int	TypeSix						=   6;
	public static final int	TypeSeven					=   7;
	public static final int	TypeExtendedOne				=   8;
	public static final int	TypeExtendedTwo				=   9;
	public static final int	TypeExtendedThree			=  10;
	public static final int	TypeExtendedFour			=  11;
	public static final int	TypeExtendedFive			=  12;
	public static final int	TypeExtendedSix				=  13;
	public static final int	TypeExtendedSeven			=  14;
	public static final int	TypeExtendedEight			=  15;
	public static final int	TypeRandom					= -1;

	// ----------------------------------------------------------------------
	//
	protected static final int	TotalNumberOfTypes		= 16;
	protected static final int	NumberOfStandardTypes	=  7;
	protected static final int	NumberOfExtendedTypes	= 15;
	protected static final int	DefaultBlockPixelSize	= 23;

	// ----------------------------------------------------------------------
	//
	public TetrisPiece(TetrisView tetrisview)
	{
		tetrisView = tetrisview;
		setType(TypeRandom);
		blockLocation.x = tetrisView.getWidth() / 2 - 1;
		blockLocation.y = -getHeight();
	}

	// ----------------------------------------------------------------------
	//
	public TetrisPiece(int type, TetrisView tetrisview)
	{
		tetrisView = tetrisview;
		setType(type);
		blockLocation.x = tetrisView.getWidth() / 2 - 1;
		blockLocation.y = -getHeight();
	}

	// ----------------------------------------------------------------------
	//
	public int getType()
	{
		return blockType;
	}

	// ----------------------------------------------------------------------
	//
	public Color getColor()
	{
		if (blockColor != null) {
			return blockColor;
		}
		else {
			return getDefaultColor(blockType);
		}
	}

	// ----------------------------------------------------------------------
	//
	public Color getShadowColor()
	{
		if (blockShadowColor != null) {
			return blockShadowColor;
		}
		else {
			return getDefaultShadowColor(blockType);
		}
	}

	// ----------------------------------------------------------------------
	//
	public Color getHighlightColor()
	{
		if (blockHighlightColor != null) {
			return blockHighlightColor;
		}
		else {
			return getDefaultHighlightColor(blockType);
		}
	}

	// ----------------------------------------------------------------------
	//
	public Color getDarkShadowColor()
	{
		if (blockDarkShadowColor != null) {
			return blockDarkShadowColor;
		}
		else {
			return getDefaultDarkShadowColor(blockType);
		}
	}

	// ----------------------------------------------------------------------
	//
	public Color getDarkHighlightColor()
	{
		if (blockDarkHighlightColor != null) {
			return blockDarkHighlightColor;
		}
		else {
			return getDefaultDarkHighlightColor(blockType);
		}
	}

	// ----------------------------------------------------------------------
	//
	public int getBlockPixelSize()
	{
		return blockPixelSize;
	}

	// ----------------------------------------------------------------------
	//
	public int getX()
	{
		return blockLocation.x;
	}

	// ----------------------------------------------------------------------
	//
	public int getY()
	{
		return blockLocation.y;
	}

	// ----------------------------------------------------------------------
	//
	public Point getLocation()
	{
		return new Point(getX(), getY());
	}

	// ----------------------------------------------------------------------
	//
	public int getWidth()
	{
		return blockData[blockRotation].width;
	}

	// ----------------------------------------------------------------------
	//
	public int getHeight()
	{
		return blockData[blockRotation].height;
	}

	// ----------------------------------------------------------------------
	//
	public Dimension getSize()
	{
		return new Dimension(getWidth(), getHeight());
	}

	// ----------------------------------------------------------------------
	//
	public Point[] getBlockLocations()
	{
		if (blockData == null) {
			return null;
		}
		return blockData[blockRotation].map;
	}

	// ----------------------------------------------------------------------
	//
	public static int getNumberOfTypes()
	{
		if (useExtendedPieceSet) {
			return NumberOfExtendedTypes;
		}
		else {
			return NumberOfStandardTypes;
		}
	}

	// ----------------------------------------------------------------------
	//
	public TetrisView getTetrisView()
	{
		return tetrisView;
	}

	// ----------------------------------------------------------------------
	//
	public boolean getUpdateTetrisView()
	{
		return updateTetrisView;
	}

	// ----------------------------------------------------------------------
	//
	public static Color getDefaultColor(int type)
	{
		if (type < 0) {
			type = 0;
		}
		else if (type >= TotalNumberOfTypes) {
			type = TotalNumberOfTypes - 1;
		}
		if (defaultBlockColor == null) {
			defaultBlockColor = new Color[TotalNumberOfTypes];
		}
		if (defaultBlockColor[type] != null) {
			return defaultBlockColor[type];
		}
		switch (type) {
			case TypeTrivial:
				defaultBlockColor[type] = new Color(125, 75, 125);
				break;
			case TypeOne:
				defaultBlockColor[type] = new Color(200, 20, 20);
				break;
			case TypeTwo:
				defaultBlockColor[type] = new Color(20, 200, 20);
				break;
			case TypeThree:
				defaultBlockColor[type] = new Color(80, 150, 100);
				break;
			case TypeFour:
				defaultBlockColor[type] = new Color(220, 220, 50);
				break;
			case TypeFive:
				defaultBlockColor[type] = new Color(255, 0, 255);
				break;
			case TypeSix:
				defaultBlockColor[type] = new Color(40, 220, 220);
				break;
			case TypeSeven:
				defaultBlockColor[type] = new Color(100, 100, 200);
				break;
			case TypeExtendedOne:
				defaultBlockColor[type] = new Color(60, 60, 130);
				break;
			case TypeExtendedTwo:
				defaultBlockColor[type] = new Color(150, 50, 100);
				break;
			case TypeExtendedThree:
				defaultBlockColor[type] = new Color(200, 100, 80);
				break;
			case TypeExtendedFour:
				defaultBlockColor[type] = new Color(233, 33, 83);
				break;
			case TypeExtendedFive:
				defaultBlockColor[type] = new Color(83, 83, 233);
				break;
			case TypeExtendedSix:
				defaultBlockColor[type] = new Color(93, 63, 33);
				break;
			case TypeExtendedSeven:
				defaultBlockColor[type] = new Color(193, 63, 133);
				break;
			case TypeExtendedEight:
				defaultBlockColor[type] = new Color(93, 163, 33);
				break;
		}
		return defaultBlockColor[type];
	}

	// ----------------------------------------------------------------------
	//
	public static Color getDefaultShadowColor(int type)
	{
		if (defaultBlockShadowColor == null) {
			defaultBlockShadowColor = new Color[TotalNumberOfTypes];
		}
		if (defaultBlockShadowColor[type] == null) {
			defaultBlockShadowColor[type] =
				ColorScheme.getShadow(getDefaultColor(type));
		}
		return defaultBlockShadowColor[type];
	}

	// ----------------------------------------------------------------------
	//
	public static Color getDefaultHighlightColor(int type)
	{
		if (defaultBlockHighlightColor == null) {
			defaultBlockHighlightColor = new Color[TotalNumberOfTypes];
		}
		if (defaultBlockHighlightColor[type] == null) {
			defaultBlockHighlightColor[type] =
				ColorScheme.getHighlight(getDefaultColor(type));
		}
		return defaultBlockHighlightColor[type];
	}

	// ----------------------------------------------------------------------
	//
	public static Color getDefaultDarkShadowColor(int type)
	{
		if (defaultBlockDarkShadowColor == null) {
			defaultBlockDarkShadowColor = new Color[TotalNumberOfTypes];
		}
		if (defaultBlockDarkShadowColor[type] == null) {
			defaultBlockDarkShadowColor[type] =
				ColorScheme.getDarkShadow(getDefaultColor(type));
		}
		return defaultBlockDarkShadowColor[type];
	}

	// ----------------------------------------------------------------------
	//
	public static Color getDefaultDarkHighlightColor(int type)
	{
		if (defaultBlockDarkHighlightColor == null) {
			defaultBlockDarkHighlightColor = new Color[TotalNumberOfTypes];
		}
		if (defaultBlockDarkHighlightColor[type] == null) {
			defaultBlockDarkHighlightColor[type] =
				ColorScheme.getDarkHighlight(getDefaultColor(type));
		}
		return defaultBlockDarkHighlightColor[type];
	}

	// ----------------------------------------------------------------------
	//
	public static int getDefaultBlockPixelSize()
	{
		return DefaultBlockPixelSize;
	}

	// ----------------------------------------------------------------------
	//
	public void setType(int type)
	{
		switch (type) {
			default:
			{
				return;
			}
			case TypeRandom:
			{
				int random = randomNumber.nextInt();
				if (random < 0) { random = -random; }
				type = (random % getNumberOfTypes()) + 1;
				setType(type);
				return;
			}
			case TypeTrivial:
			{
				blockData = BlockData.create(4);
				blockData[0].setBlock(0, 0, 0);			// X
				blockData[0].set(0, 0);
				blockData[1].copyBlocks(blockData[0]);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[0]);
				break;
			}
			case TypeOne:
			{
				blockData = BlockData.create(4);
				blockData[0].setBlock(0, 0, 0);			// OXOO
				blockData[0].setBlock(1, 0, 1);			// OXOO
				blockData[0].setBlock(2, 0, 2);			// OXOO
				blockData[0].setBlock(3, 0, 3);			// OXOO
				blockData[0].set(1, 0);
				blockData[1].setBlock(0, 0, 0);			// OOOO
				blockData[1].setBlock(1, 1, 0);			// XXXX
				blockData[1].setBlock(2, 2, 0);			// OOOO
				blockData[1].setBlock(3, 3, 0);			// OOOO
				blockData[1].set(0, 1);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[1]);
				break;
			}
			case TypeTwo:
			{
				blockData = BlockData.create(4);
				blockData[0].setBlock(0, 0, 0);			// OXOO
				blockData[0].setBlock(1, 0, 1);			// OXOO
				blockData[0].setBlock(2, 1, 2);			// OXXO
				blockData[0].setBlock(3, 0, 2);			// OOOO
				blockData[0].set(1, 0);
				blockData[1].setBlock(0, 0, 0);			// XXXO
				blockData[1].setBlock(1, 1, 0);			// XOOO
				blockData[1].setBlock(2, 2, 0);			// OOOO
				blockData[1].setBlock(3, 0, 1);			// OOOO
				blockData[1].set(0, 0);
				blockData[2].setBlock(0, 0, 0);			// OXXO
				blockData[2].setBlock(1, 1, 0);			// OOXO
				blockData[2].setBlock(2, 1, 1);			// OOXO
				blockData[2].setBlock(3, 1, 2);			// OOOO
				blockData[2].set(1, 0);
				blockData[3].setBlock(0, 2, 0);			// OOXO
				blockData[3].setBlock(1, 0, 1);			// XXXO
				blockData[3].setBlock(2, 1, 1);			// OOOO
				blockData[3].setBlock(3, 2, 1);			// OOOO
				blockData[3].set(0, 0);
				break;
			}
			case TypeThree:
			{
				blockData = BlockData.create(4);
				blockData[0].setBlock(0, 1, 0);			// OOXO
				blockData[0].setBlock(1, 1, 1);			// OOXO
				blockData[0].setBlock(2, 0, 2);			// OXXO
				blockData[0].setBlock(3, 1, 2);			// OOOO
				blockData[0].set(1, 0);
				blockData[1].setBlock(0, 0, 0);			// XOOO
				blockData[1].setBlock(1, 0, 1);			// XXXO
				blockData[1].setBlock(2, 1, 1);			// OOOO
				blockData[1].setBlock(3, 2, 1);			// OOOO
				blockData[1].set(0, 0);
				blockData[2].setBlock(0, 0, 0);			// OXXO
				blockData[2].setBlock(1, 1, 0);			// OXOO
				blockData[2].setBlock(2, 0, 1);			// OXOO
				blockData[2].setBlock(3, 0, 2);			// OOOO
				blockData[2].set(1, 0);
				blockData[3].setBlock(0, 0, 0);			// XXXO
				blockData[3].setBlock(1, 1, 0);			// OOXO
				blockData[3].setBlock(2, 2, 0);			// OOOO
				blockData[3].setBlock(3, 2, 1);			// OOOO
				blockData[3].set(0, 0);
				break;
			}
			case TypeFour:
			{
				blockData = BlockData.create(4);
				blockData[0].setBlock(0, 0, 0);			// OXOO
				blockData[0].setBlock(1, 0, 1);			// OXXO
				blockData[0].setBlock(2, 1, 1);			// OXOO
				blockData[0].setBlock(3, 0, 2);			// OOOO
				blockData[0].set(1, 0);
				blockData[1].setBlock(0, 0, 0);			// OOOO
				blockData[1].setBlock(1, 1, 0);			// XXXO
				blockData[1].setBlock(2, 2, 0);			// OXOO
				blockData[1].setBlock(3, 1, 1);			// OOOO
				blockData[1].set(0, 1);
				blockData[2].setBlock(0, 1, 0);			// OXOO
				blockData[2].setBlock(1, 0, 1);			// XXOO
				blockData[2].setBlock(2, 1, 1);			// OXOO
				blockData[2].setBlock(3, 1, 2);			// OOOO
				blockData[2].set(0, 0);
				blockData[3].setBlock(0, 1, 0);			// OXOO
				blockData[3].setBlock(1, 0, 1);			// XXXO
				blockData[3].setBlock(2, 1, 1);			// OOOO
				blockData[3].setBlock(3, 2, 1);			// OOOO
				blockData[3].set(0, 0);
				break;
			}
			case TypeFive:
			{
				blockData = BlockData.create(4);
				blockData[0].setBlock(0, 0, 0);			// OXOO
				blockData[0].setBlock(1, 0, 1);			// OXXO
				blockData[0].setBlock(2, 1, 1);			// OOXO
				blockData[0].setBlock(3, 1, 2);			// OOOO
				blockData[0].set(1, 0);
				blockData[1].setBlock(0, 1, 0);			// OXXO
				blockData[1].setBlock(1, 2, 0);			// XXOO
				blockData[1].setBlock(2, 0, 1);			// OOOO
				blockData[1].setBlock(3, 1, 1);			// OOOO
				blockData[1].set(0, 0);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[1]);
				break;
			}
			case TypeSix:
			{
				blockData = BlockData.create(4);
				blockData[0].setBlock(0, 1, 0);			// OOXO
				blockData[0].setBlock(1, 0, 1);			// OXXO
				blockData[0].setBlock(2, 1, 1);			// OXOO
				blockData[0].setBlock(3, 0, 2);			// OOOO
				blockData[0].set(1, 0);
				blockData[1].setBlock(0, 0, 0);			// XXOO
				blockData[1].setBlock(1, 1, 0);			// OXXO
				blockData[1].setBlock(2, 1, 1);			// OOOO
				blockData[1].setBlock(3, 2, 1);			// OOOO
				blockData[1].set(0, 0);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[1]);
				break;
			}
			case TypeSeven:
			{
				blockData = BlockData.create(4);
				blockData[0].setBlock(0, 0, 0);			// OOOO
				blockData[0].setBlock(1, 1, 0);			// OXXO
				blockData[0].setBlock(2, 0, 1);			// OXXO
				blockData[0].setBlock(3, 1, 1);			// OOOO
				blockData[0].set(1, 1);
				blockData[1].copyBlocks(blockData[0]);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[0]);
				break;
			}
			case TypeExtendedOne:
			{
				blockData = BlockData.create(1);
				blockData[0].setBlock(0, 0, 0);			// X
				blockData[0].set(0, 0);
				blockData[1].copyBlocks(blockData[0]);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[0]);
				break;
			}
			case TypeExtendedTwo:
			{
				blockData = BlockData.create(2);
				blockData[0].setBlock(0, 0, 0);			// XO
				blockData[0].setBlock(1, 0, 1);			// XO
				blockData[0].set(0, 0);
				blockData[1].setBlock(0, 0, 0);			// XX
				blockData[1].setBlock(1, 1, 0);			// OO
				blockData[1].set(0, 0);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[1]);
				break;
			}
			case TypeExtendedThree:
			{
				blockData = BlockData.create(2);
				blockData[0].setBlock(0, 0, 0);			// XO
				blockData[0].setBlock(1, 1, 1);			// OX
				blockData[0].set(0, 0);
				blockData[1].setBlock(0, 1, 0);			// OX
				blockData[1].setBlock(1, 0, 1);			// XO
				blockData[1].set(0, 0);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[1]);
				break;
			}
			case TypeExtendedFour:
			{
				blockData = BlockData.create(3);
				blockData[0].setBlock(0, 0, 0);			// OXO
				blockData[0].setBlock(1, 0, 1);			// OXO
				blockData[0].setBlock(2, 0, 2);			// OXO
				blockData[0].set(1, 0);
				blockData[1].setBlock(0, 0, 0);			// OOO
				blockData[1].setBlock(1, 1, 0);			// XXX
				blockData[1].setBlock(2, 2, 0);			// OOO
				blockData[1].set(0, 1);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[1]);
				break;
			}
			case TypeExtendedFive:
			{
				blockData = BlockData.create(3);
				blockData[0].setBlock(0, 0, 0);			// OXO
				blockData[0].setBlock(1, 0, 1);			// OXX
				blockData[0].setBlock(2, 1, 1);			// OOO
				blockData[0].set(1, 0);
				blockData[1].setBlock(0, 1, 0);			// OXO
				blockData[1].setBlock(1, 0, 1);			// XXO
				blockData[1].setBlock(2, 1, 1);			// OOO
				blockData[1].set(0, 0);
				blockData[2].setBlock(0, 0, 0);			// XXO
				blockData[2].setBlock(1, 1, 0);			// OXO
				blockData[2].setBlock(2, 1, 1);			// OOO
				blockData[2].set(0, 0);
				blockData[3].setBlock(0, 0, 0);			// XXO
				blockData[3].setBlock(1, 1, 0);			// XOO
				blockData[3].setBlock(2, 0, 1);			// OOO
				blockData[3].set(0, 0);
				break;
			}
			case TypeExtendedSix:
			{
				blockData = BlockData.create(3);
				blockData[0].setBlock(0, 0, 0);			// XOO
				blockData[0].setBlock(1, 1, 1);			// OXO
				blockData[0].setBlock(2, 2, 2);			// OOX
				blockData[0].set(0, 0);
				blockData[1].setBlock(0, 2, 0);			// OOX
				blockData[1].setBlock(1, 1, 1);			// OXO
				blockData[1].setBlock(2, 0, 2);			// XOO
				blockData[1].set(0, 0);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[1]);
				break;
			}
			case TypeExtendedSeven:
			{
				blockData = BlockData.create(3);
				blockData[0].setBlock(0, 0, 0);			// XOX
				blockData[0].setBlock(1, 2, 0);			// OXO
				blockData[0].setBlock(2, 1, 1);			// OOO
				blockData[0].set(0, 0);
				blockData[1].setBlock(0, 2, 0);			// OOX
				blockData[1].setBlock(1, 1, 1);			// OXO
				blockData[1].setBlock(2, 2, 2);			// OOX
				blockData[1].set(0, 0);
				blockData[2].setBlock(0, 1, 1);			// OOO
				blockData[2].setBlock(1, 0, 2);			// OXO
				blockData[2].setBlock(2, 2, 2);			// XOX
				blockData[2].set(0, 0);
				blockData[3].setBlock(0, 0, 0);			// XOO
				blockData[3].setBlock(1, 1, 1);			// OXO
				blockData[3].setBlock(2, 0, 2);			// XOO
				blockData[3].set(0, 0);
				break;
			}
			case TypeExtendedEight:
			{
				blockData = BlockData.create(4);
				blockData[0].setBlock(0, 1, 0);			// OXO
				blockData[0].setBlock(1, 0, 1);			// XOX
				blockData[0].setBlock(2, 2, 1);			// OXO
				blockData[0].setBlock(3, 1, 2);
				blockData[0].set(0, 0);
				blockData[1].copyBlocks(blockData[0]);
				blockData[2].copyBlocks(blockData[0]);
				blockData[3].copyBlocks(blockData[0]);
				break;
			}
		}
		blockType = type;
	}

	// ----------------------------------------------------------------------
	//
	public void setColor(Color color)
	{
		blockColor = color;
	}

	// ----------------------------------------------------------------------
	//
	public void setShadowColor(Color color)
	{
		blockShadowColor = color;
	}

	// ----------------------------------------------------------------------
	//
	public void setHighlightColor(Color color)
	{
		blockHighlightColor = color;
	}

	// ----------------------------------------------------------------------
	//
	public void setDarkShadowColor(Color color)
	{
		blockDarkShadowColor = color;
	}

	// ----------------------------------------------------------------------
	//
	public void setDarkHighlightColor(Color color)
	{
		blockDarkHighlightColor = color;
	}

	// ----------------------------------------------------------------------
	//
	public void setColor(TetrisPiece piece)
	{
		if (piece != null) {
			blockColor				= piece.getColor();
			blockShadowColor		= piece.getShadowColor();
			blockHighlightColor		= piece.getHighlightColor();
			blockDarkShadowColor	= piece.getDarkShadowColor();
			blockDarkHighlightColor	= piece.getDarkHighlightColor();
		}
	}

	// ----------------------------------------------------------------------
	//
	public void setRandomColor()
	{
		int random;
		random = randomNumber.nextInt();
		if (random < 0) { random = -random; }
		int r = (random % 256);
		random = randomNumber.nextInt();
		if (random < 0) { random = -random; }
		int g = (random % 256);
		random = randomNumber.nextInt();
		if (random < 0) { random = -random; }
		int b = (random % 256);
		setColor(new Color(r, g, b));
	}

	// ----------------------------------------------------------------------
	//
	public static void setDefaultColor(int type, Color color)
	{
		if ((type < 0) || (type >= TotalNumberOfTypes)) {
			return;
		}
		if (defaultBlockColor == null) {
			defaultBlockColor = new Color[TotalNumberOfTypes];
		}
		defaultBlockColor[type] = color;
	}

	// ----------------------------------------------------------------------
	//
	public static void setDefaultShadowColor(int type, Color color)
	{
		if ((type < 0) || (type >= TotalNumberOfTypes)) {
			return;
		}
		if (defaultBlockShadowColor == null) {
			defaultBlockShadowColor = new Color[TotalNumberOfTypes];
		}
		defaultBlockShadowColor[type] = color;
	}

	// ----------------------------------------------------------------------
	//
	public static void setDefaultHighlightColor(int type, Color color)
	{
		if ((type < 0) || (type >= TotalNumberOfTypes)) {
			return;
		}
		if (defaultBlockHighlightColor == null) {
			defaultBlockHighlightColor = new Color[TotalNumberOfTypes];
		}
		defaultBlockHighlightColor[type] = color;
	}

	// ----------------------------------------------------------------------
	//
	public static void setDefaultDarkShadowColor(int type, Color color)
	{
		if ((type < 0) || (type >= TotalNumberOfTypes)) {
			return;
		}
		if (defaultBlockDarkShadowColor == null) {
			defaultBlockDarkShadowColor = new Color[TotalNumberOfTypes];
		}
		defaultBlockDarkShadowColor[type] = color;
	}

	// ----------------------------------------------------------------------
	//
	public static void setDefaultDarkHighlightColor(int type, Color color)
	{
		if ((type < 0) || (type >= TotalNumberOfTypes)) {
			return;
		}
		if (defaultBlockDarkHighlightColor == null) {
			defaultBlockDarkHighlightColor = new Color[TotalNumberOfTypes];
		}
		defaultBlockDarkHighlightColor[type] = color;
	}

	// ----------------------------------------------------------------------
	//
	public void setBlockPixelSize(int size)
	{
		if (size <= 0) {
			blockPixelSize = DefaultBlockPixelSize;
		}
		else {
			blockPixelSize = size;
		}
	}

	// ----------------------------------------------------------------------
	//
	public void setTetrisView(TetrisView tetrisview)
	{
		tetrisView = tetrisview;
	}

	// ----------------------------------------------------------------------
	//
	public void setUpdateTetrisView(boolean b)
	{
		updateTetrisView = b;
	}

	// ----------------------------------------------------------------------
	//
	protected boolean canMove(int r, int x, int y)
	{
		if (y < 0) {
			if (y + blockData[r].height <= 0) {
				return false;
			}
		}
		if (x < 0) {
			return false;
		}
		if (x + blockData[r].width > tetrisView.getWidth()) {
			return false;
		}
		if (y + blockData[r].height > tetrisView.getHeight()) {
			return false;
		}
		for (int i = 0 ; i < blockData[r].map.length ; i++) {
			if (tetrisView.isOccupied(x + blockData[r].map[i].x,
									  y + blockData[r].map[i].y)) {
				return false;
			}
		}
		return true;
	}

	// ----------------------------------------------------------------------
	//
	protected boolean move(int r, int x, int y)
	{
		if ((r < 0) || (r >= 4) || !canMove(r, x, y)) {
			return false;
		}
		if (updateTetrisView) {
			for (int i = 0 ; i < blockData[blockRotation].map.length ; i++) {
				int thisx = blockLocation.x +
							blockData[blockRotation].map[i].x;
				int thisy = blockLocation.y +
							blockData[blockRotation].map[i].y;
				for (int j = 0 ; j < blockData[r].map.length ; j++) {
					if ((thisx == (x + blockData[r].map[j].x)) &&
					    (thisy == (y + blockData[r].map[j].y))) {
						thisx = -1;
						break;
					}
				}
				if ((thisx >= 0) && (thisy >= 0)) {
					tetrisView.paintBackground(thisx, thisy, 1, 1);
				}
			}
		}
		blockRotation	= r;
		blockLocation.x	= x;
		blockLocation.y	= y;
		if (updateTetrisView) {
			paint();
		}
		return true;
	}

	// ----------------------------------------------------------------------
	//
	public boolean move(int x, int y)
	{
		return move(blockRotation, x, y);
	}
	// ----------------------------------------------------------------------
	//
	public boolean moveRight()
	{
		return move(blockRotation, blockLocation.x + 1, blockLocation.y);
	}

	// ----------------------------------------------------------------------
	//
	public boolean moveRight(int n)
	{
		return move(blockRotation, blockLocation.x + n, blockLocation.y);
	}

	// ----------------------------------------------------------------------
	//
	public boolean moveLeft()
	{
		return move(blockRotation, blockLocation.x - 1, blockLocation.y);
	}

	// ----------------------------------------------------------------------
	//
	public boolean moveLeft(int n)
	{
		return move(blockRotation, blockLocation.x - n, blockLocation.y);
	}

	// ----------------------------------------------------------------------
	//
	public boolean moveDown()
	{
		return move(blockRotation, blockLocation.x, blockLocation.y + 1);
	}

	// ----------------------------------------------------------------------
	//
	public boolean moveDown(int n)
	{
		return move(blockRotation, blockLocation.x, blockLocation.y + n);
	}

	// ----------------------------------------------------------------------
	//
	public boolean moveUp()
	{
		return move(blockRotation, blockLocation.x, blockLocation.y - 1);
	}

	// ----------------------------------------------------------------------
	//
	public boolean moveUp(int n)
	{
		return move(blockRotation, blockLocation.x, blockLocation.y - n);
	}

	// ----------------------------------------------------------------------
	//
	public boolean rotate(int r)
	{
		if ((r %= 4) < 0) {
			r = 4 + r;
		}
		if ((r += blockRotation) >= 4) {
			r %= 4;
		}
		return move(r,
					blockLocation.x + (blockData[r].xoffset -
									   blockData[blockRotation].xoffset),
				    blockLocation.y + (blockData[r].yoffset -
									   blockData[blockRotation].yoffset));
	}

	// ----------------------------------------------------------------------
	//
	public boolean rotateRight(int r)
	{
		return rotate(r);
	}

	// ----------------------------------------------------------------------
	//
	public boolean rotateRight()
	{
		return rotate(1);
	}

	// ----------------------------------------------------------------------
	//
	public boolean rotateLeft(int r)
	{
		return rotate(-r);
	}

	// ----------------------------------------------------------------------
	//
	public boolean rotateLeft()
	{
		return rotate(-1);
	}

	// ----------------------------------------------------------------------
	//
	public static void setExtended(boolean b)
	{
		useExtendedPieceSet = b;
	}

	// ----------------------------------------------------------------------
	//
	public static boolean isExtended()
	{
		return useExtendedPieceSet;
	}

	// ----------------------------------------------------------------------
	//
	public void paint()
	{
		Graphics g = tetrisView.getGraphicsObject();
		paint(g);
		g.dispose();
	}

	// ----------------------------------------------------------------------
	//
	public void paint(Graphics g)
	{
		for (int i = 0 ; i < blockData[blockRotation].map.length ; i++) {
			int x = (blockLocation.x +
					 blockData[blockRotation].map[i].x) * blockPixelSize +
					tetrisView.getClientAreaPixelX();
			int y = (blockLocation.y +
					 blockData[blockRotation].map[i].y) * blockPixelSize +
					tetrisView.getClientAreaPixelY();
			if ((x >= tetrisView.getClientAreaPixelX()) &&
				(y >= tetrisView.getClientAreaPixelY())) {
				paintBlock(g, x, y, blockPixelSize, blockPixelSize);
			}
		}
	}

	// ----------------------------------------------------------------------
	//
	public void paint(Graphics g, int pixel_x, int pixel_y)
	{
		ignorePaintUpsideDown = true;
		for (int i = 0 ; i < blockData[blockRotation].map.length ; i++) {
			int x = (blockData[blockRotation].map[i].x) * blockPixelSize +
					pixel_x;
			int y = (blockData[blockRotation].map[i].y) * blockPixelSize +
					pixel_y;
			paintBlock(g, x, y, blockPixelSize, blockPixelSize);
		}
		ignorePaintUpsideDown = false;
	}

	// ----------------------------------------------------------------------
	//
	protected void paintBlock(int pixel_x, int pixel_y,
							  int pixel_w, int pixel_h)
	{
		Graphics g = tetrisView.getGraphicsObject();
		paintBlock(g, pixel_x, pixel_y, pixel_w, pixel_h);
		g.dispose();
	}

	// ----------------------------------------------------------------------
	//
	protected void paintBlock(Graphics g, int pixel_x, int pixel_y,
										  int pixel_w, int pixel_h)
	{
		if (!ignorePaintUpsideDown && tetrisView.isPaintUpsideDown()) {
			int height = tetrisView.getSize().height;
			pixel_y = height - pixel_y - pixel_h;
		}

		// Paint the background.

		g.setColor(getColor());
		g.fillRect(pixel_x + 2, pixel_y + 2, pixel_w - 4, pixel_h - 4);

		// Paint the top/left outer shadow.

		g.setColor(getDarkHighlightColor());
		g.drawLine(pixel_x, pixel_y, pixel_x + pixel_w - 1, pixel_y);
		g.drawLine(pixel_x, pixel_y + 1, pixel_x, pixel_y + pixel_h - 1);

				// Paint the bottom/right outer shadow.

		g.setColor(getDarkShadowColor());
		g.drawLine(pixel_x, pixel_y + pixel_h - 1,
				   pixel_x + pixel_w - 1, pixel_y + pixel_h - 1);
		g.drawLine(pixel_x + pixel_w - 1, pixel_y,
				   pixel_x + pixel_w - 1, pixel_y + pixel_h - 1);

		// Paint the top/left inner shadow.

		g.setColor(getHighlightColor());
		g.drawLine(pixel_x + 1, pixel_y + 1,
				   pixel_x + pixel_w - 2, pixel_y + 1);
		g.drawLine(pixel_x + 1, pixel_y + 2,
				   pixel_x + 1, pixel_y + pixel_h - 2);

		// Paint the bottom/right inner shadow.

		g.setColor(getShadowColor());
		g.drawLine(pixel_x + 1, pixel_y + pixel_h - 2,
				   pixel_x + pixel_w - 2, pixel_y + pixel_h - 2);
		g.drawLine(pixel_x + pixel_w - 2, pixel_y + 1,
				   pixel_x + pixel_w - 2, pixel_y + pixel_h - 2);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
	static class BlockData
	{
		public static BlockData[] create(int count)
		{
			BlockData[] blockdata = new BlockData[4];
			for (int i = 0 ; i < 4 ; i++) {
				blockdata[i] = new BlockData(count);
			}
			return blockdata;
		}
		public void setBlock(int n, int x, int y)
		{
			map[n].x = x;
			map[n].y = y;
		}
		public void set(int xo, int yo)
		{
			xoffset = xo;
			yoffset = yo;
			int maxx = 0, maxy = 0;
			for (int i = 0 ; i < map.length ; i++) {
				if (map[i].x > maxx) { maxx = map[i].x; }
				if (map[i].y > maxy) { maxy = map[i].y; }
			}
			width	= maxx + 1;
			height	= maxy + 1;
		}
		public boolean contains(Point point)
		{
			for (int i = 0 ; i < map.length ; i++) {
				if ((map[i].x == point.x) && (map[i].y == point.y)) {
					return true;
				}
			}
			return false;
		}
		public void copyBlocks(BlockData blockdata)
		{
			if (blockdata == null) {
				return;
			}
			if (blockdata.map != null) {
				map = new Point[blockdata.map.length];
				for (int i = 0 ; i < map.length ; i++) {
					map[i] = new Point(blockdata.map[i].x,
									   blockdata.map[i].y);
				}
			}
			xoffset	= blockdata.xoffset;
			yoffset	= blockdata.yoffset;
			width	= blockdata.width;
			height	= blockdata.height;
		}
		protected BlockData(int count)
		{
			map = new Point[count];
			for (int i = 0 ; i < count ; i++) {
				map[i] = new Point();
			}
		}
		public Point	map[]	= null;
		public int		xoffset	= 0;
		public int		yoffset	= 0;
		public int		width	= 0;
		public int		height	= 0;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
	int				blockType				= TypeTrivial;
	BlockData		blockData[]				= null;
	int				blockRotation			= 0;
	Point			blockLocation			= new Point();
	int				blockPixelSize			= DefaultBlockPixelSize;
	Color			blockColor				= null;
	Color			blockShadowColor		= null;
	Color			blockHighlightColor		= null;
	Color			blockDarkShadowColor	= null;
	Color			blockDarkHighlightColor	= null;
	boolean			updateTetrisView		= true;
	TetrisView		tetrisView				= null;
	boolean			ignorePaintUpsideDown	= false;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
	static Color	defaultBlockColor[]					= null;
	static Color	defaultBlockShadowColor[]			= null;
	static Color	defaultBlockHighlightColor[]		= null;
	static Color	defaultBlockDarkShadowColor[]		= null;
	static Color	defaultBlockDarkHighlightColor[]	= null;
	static Random	randomNumber						=
					new Random((int)((new Date()).getTime()));
	static boolean	useExtendedPieceSet					= false;
}
