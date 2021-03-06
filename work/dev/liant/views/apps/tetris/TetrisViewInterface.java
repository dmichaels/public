//	TetrisViewInterface.java
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
public interface TetrisViewInterface
{
	public int		getWidth();
	public int		getHeight();
	public int		getPixelX();
	public int		getPixelY();
	public int		getPixelWidth();
	public int		getPixelHeight();
	public boolean	isOccupied(int x, int y);
	public void		paintBackground(int x, int y, int w, int h);
	public Graphics	getGraphicsObject();
}
