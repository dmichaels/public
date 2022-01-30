//	TetrisViewListener.java
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

public interface TetrisViewListener extends java.util.EventListener
{
	public void gameBegun();
	public void gameOver();
	public void gamePaused();
	public void gameResumed();
	public void linesCleared();
	public void tetrisCleared();
	public void pieceStarted();
	public void pieceFalling();
	public void pieceDropped();
}
