//	TetrisView.java
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

import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.util.*;
import java.net.*;
import Timer;
import TimerListener;
import TetrisPiece;
import TetrisViewListener;

// --------------------------------------------------------------------------
//
public class TetrisView extends Canvas
{
	// ----------------------------------------------------------------------
	//
	public TetrisView(Applet applet)
	{
		appletParent = applet;
		setBackground(SystemColor.controlShadow);

		createTimer();

		addKeyListener(new KeyAdapter()
		{
			public void keyPressed(KeyEvent e)
			{
				handleKeyEvent(e);
			}
		});

		addMouseListener(new MouseAdapter()
		{
			public void mouseClicked(KeyEvent e)
			{
				requestFocus();
			}
		});

		addFocusListener(new FocusAdapter()
		{
			public void focusGained(FocusEvent e)
			{
				setEnabled(true);
			}
			public void focusLost(FocusEvent e)
			{
				setEnabled(false);
			}
		});

		addComponentListener(new ComponentAdapter()
		{
			public void componentResized(ComponentEvent e)
			{
				doLayout();
			}
		});
	}

	// ----------------------------------------------------------------------
	//
	public int getWidth()
	{
		return unitWidth;
	}

	// ----------------------------------------------------------------------
	//
	public int getHeight()
	{
		return unitHeight;
	}

	// ----------------------------------------------------------------------
	//
	public void setUnitSize(int unit_w, int unit_h)
	{
		if ((unit_w == unitWidth) && (unit_h == unitHeight)) {
			return;
		}
		if (unit_w < 1) {
			unit_w = 1;
		}
		if (unit_h < 1) {
			unit_h = 1;
		}
		TetrisPiece	dropped_blocks[][] =
					new TetrisPiece[unit_w][unit_h];
		int high_blocks[] = new int[unit_w];
		for (int i = unitWidth <= unit_w ? unitWidth - 1 : unit_w - 1 ;
				 i >= 0 ; i--) {
			for (int j = unitHeight <= unit_h ? unitHeight - 1 : unit_h - 1 ;
					 j >= 0 ; j--) {
				dropped_blocks[i][j] = droppedBlocks[i][j];
			}
			high_blocks[i] = highBlocks[i];
		}
		highBlocks = high_blocks;
		droppedBlocks = dropped_blocks;
		unitWidth = unit_w;
		if (unit_h > unitHeight) {
			int index = unit_h - 1, count = unit_h - unitHeight;
			unitHeight = unit_h;
			moveDownLines(index, count);
		}
		else {
			unitHeight = unit_h;
		}
		if (currentPiece != null) {
			newPiece(currentPiece.getType());
		}
		reloadBackgroundImage();
	}

	// ----------------------------------------------------------------------
	//
	public int getClientAreaPixelX()
	{
		return marginThicknessL + borderThicknessX;
	}

	// ----------------------------------------------------------------------
	//
	public int getClientAreaPixelY()
	{
		return marginThicknessT + borderThicknessY;
	}

	// ----------------------------------------------------------------------
	//
	public int getClientAreaPixelWidth()
	{
		return unitWidth * blockPixelSize;
	}

	// ----------------------------------------------------------------------
	//
	public int getClientAreaPixelHeight()
	{
		return unitHeight * blockPixelSize;
	}

	// ----------------------------------------------------------------------
	//
	public void setExtended(boolean b)
	{
		TetrisPiece.setExtended(b);
	}

	// ----------------------------------------------------------------------
	//
	public boolean isExtended()
	{
		return TetrisPiece.isExtended();
	}

	// ----------------------------------------------------------------------
	//
	public boolean isOccupied(int x, int y)
	{
		if ((x < 0) || (x >= unitWidth) || (y < 0) || (y >= unitHeight)) {
			return false;
		}
		return droppedBlocks[x][y] != null;
	}

	// ----------------------------------------------------------------------
	//
	public void paintBackground(Graphics g, int ux, int uy, int uw, int uh)
	{
		if (droppedBlocks[ux][uy] != null) {
			return;
		}
		int x = ux * blockPixelSize + getClientAreaPixelX();
		int y = uy * blockPixelSize + getClientAreaPixelY();
		int w = uw * blockPixelSize;
		int h = uh * blockPixelSize;
		Image background_image = getBackgroundImage();
		if (background_image != null) {
			if (paintUpsideDown) {
				int height = getSize().height;
				g.drawImage(background_image,
							x,
							height - y - h,
							x + w,
							height - y,
							x - getClientAreaPixelX(),
							height - y - h - getClientAreaPixelY(),
							x + w - getClientAreaPixelX(),
							height - y - getClientAreaPixelY(), this);
				
			}
			else {
				g.drawImage(background_image,
							x,
							y,
							x + w,
							y + h,
							x - getClientAreaPixelX(),
							y - getClientAreaPixelY(),
							x + w - getClientAreaPixelX(),
							y + h - getClientAreaPixelY(), this);
			}
		}
		else {
			if (paintUpsideDown) {
				y = getSize().height - y - h;
			}
			/*
			g.setColor(SystemColor.controlHighlight);
			g.drawLine(x, y, x + w - 2, y);
			g.drawLine(x, y + 1, x, y + h - 1);
			g.setColor(SystemColor.controlDkShadow);
			g.drawLine(x + w - 1, y, x + w - 1, y + h - 1);
			g.drawLine(x, y + h - 1, x + w - 2, y + h - 1);
			g.setColor(SystemColor.controlShadow);
			g.fillRect(x + 1, y + 1, w - 2, h - 2);
			*/
			g.setColor(SystemColor.controlShadow);
			g.fillRect(x, y, w, h);
			/*
			g.setColor(SystemColor.controlDkShadow);
			if (ux < unitWidth - 1) {
				g.drawLine(x + w - 1, y, x + w - 1, y + h - 1); w--;
			}
			if (uy < unitHeight - 1) {
				g.drawLine(x, y + h - 1, x + w - 1, y + h - 1); h--;
			}
			g.setColor(SystemColor.controlShadow);
			g.fillRect(x, y, w, h);
			*/
		}
	}

	// ----------------------------------------------------------------------
	//
	public void paintBackground(int x, int y, int w, int h)
	{
		Graphics g = getGraphics();
		paintBackground(g, x, y, w, h);
		g.dispose();
	}

	// ----------------------------------------------------------------------
	//
	public void repaintView()
	{
		Graphics g = getGraphics();
		for (int j = 0 ; j < unitHeight ; j++) {
			for (int i = 0 ; i < unitWidth ; i++) {
				if (droppedBlocks[i][j] != null) {
					droppedBlocks[i][j].paint(g);
				}
				else {
					paintBackground(g, i, j, 1, 1);
				}
			}
		}
		if (currentPiece != null) {
			currentPiece.paint(g);
		}
		g.dispose();
	}

	// ----------------------------------------------------------------------
	//
	public Graphics getGraphicsObject()
	{
		return getGraphics();
	}

	// ----------------------------------------------------------------------
	//
	public int getBlockPixelSize()
	{
		return blockPixelSize;
	}

	// ----------------------------------------------------------------------
	//
	public int getHighBlockY(int x)
	{
		return highBlocks[x];
	}

	// ----------------------------------------------------------------------
	//
	public void start()
	{
		resume();
	}

	// ----------------------------------------------------------------------
	//
	public void pause()
	{
		setPause(true);
	}

	// ----------------------------------------------------------------------
	//
	public void resume()
	{
		setPause(false);
	}

	// ----------------------------------------------------------------------
	//
	public void stop()
	{
		pause();
	}

	// ----------------------------------------------------------------------
	//
	public void newGame()
	{
		clearView(false);
		newPiece();
		if (!debugMode) {
			startTimer();
		}
		if (listener != null) {
			listener.gameBegun();
		}
		requestFocus();
	}

	// ----------------------------------------------------------------------
	//
	public void endGame()
	{
		stopTimer();
		clearView(true);
		if (listener != null) {
			listener.gameOver();
		}
			newGame();
	}

	// ----------------------------------------------------------------------
	//
	public void setPause(boolean b)
	{
		pauseMode = b;
		if (pauseMode) {
			stopTimer();
			if (listener != null) {
				listener.gamePaused();
			}
		}
		else {
			if (!debugMode) {
				if ((currentPiece == null) && (highBlock == unitHeight)) {
					newGame();
				}
				else {
					startTimer();
					if (listener != null) {
						listener.gameResumed();
					}
				}
			}
			requestFocus();
		}
	}

	// ----------------------------------------------------------------------
	//
	public boolean isPause()
	{
		return pauseMode;
	}

	// ----------------------------------------------------------------------
	//
	public void togglePause()
	{
		setPause(!isPause());
	}

	// ----------------------------------------------------------------------
	//
	public boolean isDebugMode()
	{
		return debugMode;
	}

	// ----------------------------------------------------------------------
	//
	public void setDebugMode(boolean b)
	{
		debugMode = b;
		if (debugMode) {
			stopTimer();
		}
		else {
			startTimer();
		}
	}

	// ----------------------------------------------------------------------
	//
	public void toggleDebugMode()
	{
		setDebugMode(!isDebugMode());
	}

	// ----------------------------------------------------------------------
	//
	public int getLinesClearedCount()
	{
		return totalLinesCleared;
	}

	// ----------------------------------------------------------------------
	//
	public int getTetrisCount()
	{
		return totalTetris;
	}

	// ----------------------------------------------------------------------
	//
	public int getPiecesDroppedCount()
	{
		return totalPiecesDropped;
	}

	// ----------------------------------------------------------------------
	//
	public TetrisPiece getNextPiece()
	{
		return nextPiece;
	}

	// ----------------------------------------------------------------------
	//
	public Dimension getPreferredSize()
	{
		return new Dimension(unitWidth *
							 blockPixelSize + borderThicknessX * 2,
							 unitHeight *
							 blockPixelSize + borderThicknessY * 2);
	}

	// ----------------------------------------------------------------------
	//
	public Dimension getMinimumSize()
	{
		return getPreferredSize();
	}

	// ----------------------------------------------------------------------
	//
	// public void update(Graphics g)
	// {
	// }

	// ----------------------------------------------------------------------
	//
	public void setPaintUpsideDown(boolean b)
	{
		if (b != paintUpsideDown) {
			paintUpsideDown = b;
			repaintView();
		}
	}

	// ----------------------------------------------------------------------
	//
	public boolean isPaintUpsideDown()
	{
		return paintUpsideDown;
	}

	// ----------------------------------------------------------------------
	//
	public void doLayout()
	{
		Dimension size = getSize();

		size.width  -= borderThicknessX * 2;
		size.height -= borderThicknessY * 2;

		int unit_w = size.width  / blockPixelSize, extra_w;
		int unit_h = size.height / blockPixelSize, extra_h;

		if (unit_w < 8) {
			unit_w = 8;
			extra_w = 0;
		}
		else {
			extra_w = size.width % blockPixelSize;
		}
		if (unit_h < 8) {
			unit_h = 8;
			extra_h = 0;
		}
		else {
			extra_h = size.height % blockPixelSize;
		}

		int client_w = unit_w * blockPixelSize;
		int client_h = unit_h * blockPixelSize;


		marginThicknessL = extra_w / 2;
		marginThicknessT = extra_h / 2;
		marginThicknessR = extra_w - marginThicknessL;
		marginThicknessB = extra_h - marginThicknessT;

		setUnitSize(unit_w, unit_h);
	}

	// ----------------------------------------------------------------------
	//
	public void paint(Graphics g)
	{
		Image background_image = getBackgroundImage();
		if (background_image != null) {
			g.drawImage(background_image, getClientAreaPixelX(),
										  getClientAreaPixelY(), this);
		}
		for (int j = unitHeight - 1 ; j >= 0 ; j--) {
			for (int i = 0 ; i < unitWidth ; i++) {
				if (droppedBlocks[i][j] != null) {
					droppedBlocks[i][j].paint(g);
				}
			}
		}
		if (currentPiece != null) {
			currentPiece.paint(g);
		}
		paintBorder(g);
	}

	// ----------------------------------------------------------------------
	//
	public void paintBorder(Graphics g)
	{
		int x = marginThicknessL;
		int y = marginThicknessT;
		int w = unitWidth  * blockPixelSize + borderThicknessX * 2;
		int h = unitHeight * blockPixelSize + borderThicknessY * 2;

		if ((marginThicknessL > 0) || (marginThicknessR > 0) ||
			(marginThicknessT > 0) || (marginThicknessB > 0)) {
			Dimension size = getSize();
			g.setColor(getParent().getBackground());
			if (marginThicknessL > 0) {
				g.fillRect(0, 0, marginThicknessL, size.height);
			}
			if (marginThicknessR > 0) {
				g.fillRect(size.width - marginThicknessR, 0,
						   marginThicknessR, size.height);
			}
			if (marginThicknessT > 0) {
				g.fillRect(0, 0, size.width, marginThicknessT);
			}
			if (marginThicknessB > 0) {
				g.fillRect(0, size.height - marginThicknessB,
						   size.width, marginThicknessB); 
			}
		}

		// Paint the top/left outer shadow.

		g.setColor(SystemColor.controlShadow);
		g.drawLine(x, y, x + w - 1, y);
		g.drawLine(x, y + 1, x, y + h - 1);

		// Paint the bottom/right outer shadow.

		g.setColor(SystemColor.controlLtHighlight);
		g.drawLine(x, y + h - 1, x + w - 1, y + h - 1);
		g.drawLine(x + w - 1, y, x + w - 1, y + h - 1);

		// Paint the top/left inner shadow.

		g.setColor(SystemColor.controlDkShadow);
		g.drawLine(x + 1, y + 1, x + w - 2, y + 1);
		g.drawLine(x + 1, y + 2, x + 1, y + h - 2);

		// Paint the bottom/right inner shadow.

		g.setColor(SystemColor.controlHighlight);
		g.drawLine(x + 1, y + h - 2, x + w - 2, y + h - 2);
		g.drawLine(x + w - 2, y + 1, x + w - 2, y + h - 2);
	}

	// ------------------------------------------------------------------------
	//
	protected void setBackgroundImage(String file)
	{
		backgroundImageFile = "file:/" + file.replace('\\', '/');
		reloadBackgroundImage();
		repaintView();
	}

	// ------------------------------------------------------------------------
	//
	protected void reloadBackgroundImage()
	{
		backgroundImage = null;
		backgroundImageDone	= false;
	}

	// ------------------------------------------------------------------------
	//
	protected Image getBackgroundImage()
	{
		if (backgroundImageDone) {
			return backgroundImage;
		}
		backgroundImageDone = true;
		URL url = appletParent.getDocumentBase();
		backgroundImage = appletParent.getImage(url, backgroundImageFile);
		if (backgroundImage != null) {
		 	MediaTracker tracker = new MediaTracker(this);
			tracker.addImage(backgroundImage, 0);
			try { tracker.waitForAll(); } catch (InterruptedException e) { }
			int image_w = backgroundImage.getWidth(null);
			int image_h = backgroundImage.getHeight(null);
			if ((image_w <= 0) || (image_h <= 0)) {
				return backgroundImage = null;
			}
			Dimension size = getSize();
			size.width = unitWidth * blockPixelSize;
			size.height = unitHeight * blockPixelSize;
			if ((size.width > image_w) || (size.height > image_h)) {
				Image image = createImage(size.width, size.height);
				if (image == null) {
					return backgroundImage = null;
				}
				Graphics g = image.getGraphics();
				if (g == null) {
					return backgroundImage = null;
				}
				for (int i = 0 ;
						 i <= size.width + image_w ; i += image_w) {
					for (int j = 0 ;
							 j <= size.height + image_h ; j += image_h) {
						g.drawImage(backgroundImage, i, j, null);
					}
				}
				g.dispose();
				backgroundImage = image;
			}
		}
		return backgroundImage;
	}

	// ----------------------------------------------------------------------
	//
	public void initializeSpeed()
	{
		setTimerSpeed(timerSpeed = 900);
	}

	// ----------------------------------------------------------------------
	//
	public void incrementSpeed()
	{
		setTimerSpeed(timerSpeed -= 75);
	}

	// ----------------------------------------------------------------------
	//
	protected void movePieceAllTheWayDownAndDrop()
	{
		if (currentPiece == null) {
			return;
		}
		while (currentPiece.moveDown()) {
			;
		}
		if (!debugMode) {
			dropPiece();
			newPiece();
		}
	}

	// ----------------------------------------------------------------------
	//
	protected void movePieceDown()
	{
		if (currentPiece == null) {
			return;
		}
		if (!currentPiece.moveDown()) {
			if (!debugMode) {
				dropPiece();
				newPiece();
			}
		}
	}

	// ----------------------------------------------------------------------
	//
	protected void movePieceUp()
	{
		if (currentPiece == null) {
			return;
		}
		if (debugMode) {
			currentPiece.moveUp();
		}
	}

	// ----------------------------------------------------------------------
	//
	protected void movePieceRight()
	{
		if (currentPiece == null) {
			return;
		}
		currentPiece.moveRight();
	}

	// ----------------------------------------------------------------------
	//
	protected void movePieceLeft()
	{
		if (currentPiece == null) {
			return;
		}
		currentPiece.moveLeft();
	}

	// ----------------------------------------------------------------------
	//
	protected void rotatePieceRight()
	{
		if (currentPiece == null) {
			return;
		}
		currentPiece.rotateRight();
	}

	// ----------------------------------------------------------------------
	//
	protected void rotatePieceLeft()
	{
		if (currentPiece == null) {
			return;
		}
		currentPiece.rotateLeft();
	}

	// ----------------------------------------------------------------------
	//
	protected void clearView(boolean fill_view_with_blocks_first)
	{
		currentPiece = null;
		nextPiece = null;
		for (int i = 0 ; i < unitWidth ; i++) {
			for (int j = 0 ; j < unitHeight ; j++) {
				droppedBlocks[i][j] = null;
			}
		}
		for (int i = 0 ; i < unitWidth ; i++) {
			highBlocks[i] = unitHeight;
		}
		highBlock = unitHeight;
		totalPiecesDropped = 0;
		totalLinesCleared = 0;
		totalTetris = 0;
		if (fill_view_with_blocks_first) {
			flashLines(0, unitHeight, 2);
		}
		paintBackground(0, 0, unitWidth, unitHeight);
	}

	// ----------------------------------------------------------------------
	//
	protected void newPiece()
	{
		newPiece(-1);
	}

	// ----------------------------------------------------------------------
	//
	protected void newPiece(int type)
	{
		if (currentPiece != null) {
			if (!debugMode) {
				return;
			}
			dropPiece();
		}
		if (type >= 0) {
			currentPiece = new TetrisPiece(type, this);
		}
		else if (nextPiece != null) {
			currentPiece = nextPiece;
		}
		else {
			currentPiece = new TetrisPiece(this);
		}
		nextPiece = new TetrisPiece(this);
		if (!currentPiece.moveDown()) {
			endGame();
			return;
		}
		currentPiece.paint();
		if (listener != null) {
			listener.pieceStarted();
		}
	}

	// ----------------------------------------------------------------------
	//
	protected void dropPiece()
	{
		if (currentPiece == null) {
			return;
		}
		droppingPiece = true;
		Point[] points = currentPiece.getBlockLocations();
		TetrisPiece piece = currentPiece;
		currentPiece = null;
		int piecex = piece.getX();
		int piecey = piece.getY();
		int piecew = piece.getWidth();
		int pieceh = piece.getHeight();
		for (int i = points.length - 1 ; i >= 0 ; i--) {
			int x = piecex + points[i].x;
			int y = piecey + points[i].y;
			if ((x < 0) || (y < 0) ||
				(piecex + points[i].x >= unitWidth) ||
				(piecey + points[i].y >= unitHeight)) {
				continue;
			}
			TetrisPiece block =
						new TetrisPiece(TetrisPiece.TypeTrivial, this);
			block.setUpdateTetrisView(false);
			block.move(x, y);
			block.setColor(piece);
			block.setUpdateTetrisView(true);
			droppedBlocks[x][y] = block;
			if (y < highBlocks[x]) {
				highBlocks[x] = y;
				if (y < highBlock) {
					highBlock = y;
				}
			}
		}
		int lines = checkForFilledLines();
		totalLinesCleared += lines;
		if (lines >= 4) {
			totalTetris++;
		}
		totalPiecesDropped++;
		if (listener != null) {
			listener.pieceDropped();
			if (lines > 0) {
				listener.linesCleared();
				if (lines >= 4) {
					listener.tetrisCleared();
				}
			}
		}
		droppingPiece = false;
	}

	// ----------------------------------------------------------------------
	//
	protected int checkForFilledLines()
	{
		int linecount = 0, lineindex = 0;
		for (int j = unitHeight - 1 ; j >= 0 ; j--) {
			int i;
			for (i = 0 ; i < unitWidth ; i++) {
				if (droppedBlocks[i][j] == null) {
					break;
				}
			}
			if (i == unitWidth) {
				if (linecount == 0) {
					lineindex = j;
				}
				linecount++;
			}
			else if (linecount > 0) {
				break;
			}
			else {
				linecount = 0;
			}
		}
		if (linecount > 0) {
			removeFilledLines(lineindex, linecount);
			return linecount;
		}
		return 0;
	}

	// ----------------------------------------------------------------------
	//
	protected void removeFilledLines(int index, int count)
	{
		flashLines(index - count + 1, count, 2);
		moveDownLines(index, count);
	}

	// ----------------------------------------------------------------------
	//
	protected void moveDownLines(int index, int count)
	{
		for (int j = index - count + 1 ; j <= index ; j++) {
			for (int i = 0 ; i < unitWidth ; i++) {
				droppedBlocks[i][j] = null;
			}
		}

		int n = 0;
		for (int j = index - count ; j >= highBlock ; j--, n++) {
			for (int i = 0 ; i < unitWidth ; i++) {
				int newj = index - n;
				droppedBlocks[i][newj] = null;
				if (droppedBlocks[i][j] != null) {
					droppedBlocks[i][j].move(i, newj);
				}
				droppedBlocks[i][newj] = droppedBlocks[i][j];
			}
		}
		if (n > 0) {
			if (n < count) {
				for (int j = index - count + (index - count - highBlock) ;
						 j <= index - count + 1 ; j++) {
					for (int i = 0 ; i < unitWidth ; i++) {
						droppedBlocks[i][j] = null;
					}
				}
			}
			for (int j = index - n ; j >= highBlock ; j--) {
				for (int i = 0 ; i < unitWidth ; i++) {
					droppedBlocks[i][j] = null;
				}
			}
		}

		int previous_high_block = highBlock;

		highBlock = unitHeight;
		for (int i = 0 ; i < unitWidth ; i++) {
			int blocks = 0;
			for (int j = 0 ; j < unitHeight ; j++) {
				if (droppedBlocks[i][j] != null) {
					highBlocks[i] = j;
					if (j < highBlock) {
						highBlock = j;
					}
					blocks++;
					break;
				}
			}
			if (blocks == 0) {
				highBlocks[i] = unitHeight;
			}
		}

		// Repaint only what we need to.

		Graphics g = getGraphics();

		for (int j = index ; j >= previous_high_block ; j--) {
			for (int i = 0 ; i < unitWidth ; i++) {
				if (droppedBlocks[i][j] != null) {
					droppedBlocks[i][j].paint(g);
				}
				else {
					paintBackground(g, i, j, 1, 1);
				}
			}
		}

		g.dispose();
	}

	// ----------------------------------------------------------------------
	//
	protected void flashLines(int index, int count, int times)
	{
		for (int n = 0 ; n < times ; n++) {
			for (int j = index ; j < index + count ; j++) {
				for (int i = 0 ; i < unitWidth ; i++) {
					TetrisPiece piece = new TetrisPiece
											(TetrisPiece.TypeTrivial, this);
					piece.setRandomColor();
					droppedBlocks[i][j] = null;
					piece.move(i, j);
					droppedBlocks[i][j] = piece;
				}
			}
		}
		for (int j = index ; j < index + count ; j++) {
			for (int i = 0 ; i < unitWidth ; i++) {
				droppedBlocks[i][j] = null;
			}
		}
	}

	// ----------------------------------------------------------------------
	//
	protected int getPieceType()
	{
		if (currentPiece == null) {
			return TetrisPiece.TypeTrivial;
		}
		return currentPiece.getType();
	}

	// ----------------------------------------------------------------------
	//
	protected void setPieceType(int type)
	{
		if (currentPiece == null) {
			return;
		}
		if (debugMode) {
			currentPiece.setType(type);
			repaintView();
		}
	}

	// ----------------------------------------------------------------------
	//
	public void setEnabled(boolean b)
	{
		setPause(!b);
	}

	// ----------------------------------------------------------------------
	//
	public boolean isFocusTraversable()
	{
		return true;
	}

	// ----------------------------------------------------------------------
	//
	public void processEvent(AWTEvent e)
	{
		processingEvent = true;
		super.processEvent(e);
		processingEvent = false;
	}

	// ----------------------------------------------------------------------
	//
	protected void handleKeyEvent(KeyEvent e)
	{
		if (processingTimeout || droppingPiece) {
			return;
		}
		if (pauseMode) {
			switch (e.getKeyCode()) {
				case KeyEvent.VK_R:				// Resume
					setPause(false);
					return;
				case KeyEvent.VK_P:				// Pause
					togglePause();
					return;
			}
			return;
		}
		switch (e.getKeyCode()) {
			case KeyEvent.VK_SPACE:				// Drop
			case KeyEvent.VK_ENTER:
				movePieceAllTheWayDownAndDrop();
				break;
			case KeyEvent.VK_LEFT:				// Move Left
			case KeyEvent.VK_J:
				movePieceLeft();
				break;
			case KeyEvent.VK_RIGHT:				// Move Right
			case KeyEvent.VK_L:
				movePieceRight();
				break;
			case KeyEvent.VK_DOWN:				// Move Down
				movePieceDown();
				break;
			case KeyEvent.VK_UP:				// Rotate Left
			case KeyEvent.VK_K:
				rotatePieceLeft();
				break;
			case KeyEvent.VK_END:				// Rotate Right
			case KeyEvent.VK_I:
				rotatePieceRight();
				break;
			case KeyEvent.VK_P:					// Pause
				setPause(true);
				break;
			case KeyEvent.VK_D:					// Debug Mode
				if (e.isControlDown()) {
					toggleDebugMode();
				}
				break;
			case KeyEvent.VK_PAGE_UP:			// Debug Mode: Move Up
				if (debugMode) {
					movePieceUp();
				}
				break;
			case KeyEvent.VK_X:					// Debug Mode: Drop Piece
				if (debugMode) {
					dropPiece();
				}
				break;
			case KeyEvent.VK_N:					// Debug Mode: New Piece
				if (debugMode) {
					newPiece();
				}
				break;
			case KeyEvent.VK_1:					// Debug Mode: Piece 1
				if (debugMode) {
					setPieceType(TetrisPiece.TypeOne);
				}
				break;
			case KeyEvent.VK_2:					// Debug Mode: Piece 2
				if (debugMode) {
					setPieceType(TetrisPiece.TypeTwo);
				}
				break;
			case KeyEvent.VK_3:					// Debug Mode: Piece 3
				if (debugMode) {
					setPieceType(TetrisPiece.TypeThree);
				}
				break;
			case KeyEvent.VK_4:					// Debug Mode: Piece 4
				if (debugMode) {
					setPieceType(TetrisPiece.TypeFour);
				}
				break;
			case KeyEvent.VK_5:					// Debug Mode: Piece 5
				if (debugMode) {
					setPieceType(TetrisPiece.TypeFive);
				}
				break;
			case KeyEvent.VK_6:					// Debug Mode: Piece 6
				if (debugMode) {
					setPieceType(TetrisPiece.TypeSix);
				}
				break;
			case KeyEvent.VK_7:					// Debug Mode: Piece 7
				if (debugMode) {
					setPieceType(TetrisPiece.TypeSeven);
				}
				break;
			case KeyEvent.VK_0:					// Debug Mode: Piece 0
				if (debugMode) {
					setPieceType(TetrisPiece.TypeTrivial);
				}
				else {
					movePieceAllTheWayDownAndDrop();
				}
				break;
		}
	}

	// ----------------------------------------------------------------------
	//
	protected void createTimer()
	{
		if (timer != null) {
			return;
		}
		timer = new Timer(timerSpeed,
						  new TimerAdapter()
						  {
							  	public void timeout(TimerEvent e)
								{
									handleTimeout();
								}
						  });
	}

	// ----------------------------------------------------------------------
	//
	protected void handleTimeout()
	{
		if (processingEvent || droppingPiece) {
			return;
		}
		processingTimeout = true;
		movePieceDown();
		if (listener != null) {
			listener.pieceFalling();
		}
		processingTimeout = false;
	}

	// ----------------------------------------------------------------------
	//
	protected void stopTimer()
	{
		if (timer != null) {
			timer.stop();
		}
	}

	// ----------------------------------------------------------------------
	//
	protected void startTimer()
	{
		if (timer != null) {
			timer.start();
		}
	}

	// ----------------------------------------------------------------------
	//
	protected void setTimerSpeed(int ms)
	{
		if (timer != null) {
			timer.setDelay(ms);
			timer.start();
		}
	}

	// ----------------------------------------------------------------------
	//
	public synchronized void addTetrisViewListener(TetrisViewListener l)
	{
		listener = l;
	}

	// ----------------------------------------------------------------------
	//
	public synchronized void removeTetrisViewListener(TetrisViewListener l)
	{
		if (listener == l) {
			listener = null;
		}
	}

	// ----------------------------------------------------------------------
	//
	int			unitWidth			= 10;
	int			unitHeight			= 20;
	int			borderThicknessX	= 2;
	int			borderThicknessY	= 2;
	int			marginThicknessT	= 0;
	int			marginThicknessB	= 0;
	int			marginThicknessL	= 0;
	int			marginThicknessR	= 0;
	int			blockPixelSize		= TetrisPiece.getDefaultBlockPixelSize();
	TetrisPiece	currentPiece		= null;
	TetrisPiece	nextPiece			= null;
	TetrisPiece	droppedBlocks[][]	= new TetrisPiece[unitWidth][unitHeight];
	int			highBlocks[]		= new int[unitWidth];
	int			highBlock			= unitHeight;
	boolean		droppingPiece		= false;
	int			timerSpeed			= 800;
	Timer		timer				= null;
	Image		backgroundImage		= null;
	String		backgroundImageFile	= "tetris.gif";
	boolean		backgroundImageDone	= false;
	Applet		appletParent		= null;
	boolean		pauseMode			= false;
	boolean		debugMode			= false;
	int			totalPiecesDropped	= 0;
	int			totalLinesCleared	= 0;
	int			totalTetris			= 0;
	TetrisViewListener	listener	= null;
	boolean		paintUpsideDown		= false;
	boolean		processingEvent		= false;
	boolean		processingTimeout	= false;
}
