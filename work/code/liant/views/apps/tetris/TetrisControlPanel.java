//	TetrisControlPanel.java
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
import java.awt.event.*;
import java.util.*;
import TetrisView;

// --------------------------------------------------------------------------
// N.B. IE 4.0 doesn't seem to deal with lightweight containers very well,
// so we derive Panel instead of Container here.  Also, IE 4.0 doesn't
// seems to do much better if we define getMinimumSize() to return
// getPreferredSize()...
//
public class TetrisControlPanel extends Panel // Container
{
	// ----------------------------------------------------------------------
	//
	public TetrisControlPanel(TetrisView tetris_view)
	{
		tetrisView = tetris_view;

		setBackground(SystemColor.control);
		setForeground(SystemColor.controlText);

		GridBagLayout gbl = new GridBagLayout();
		setLayout(gbl);

		Button button;
		GridBagConstraints gbc;

		gbc = new GridBagConstraints();
		gbc.gridx = 0;
		gbc.gridy = GridBagConstraints.RELATIVE;
		gbc.weighty = 0;
		gbc.anchor = GridBagConstraints.NORTH;
		gbc.fill = GridBagConstraints.HORIZONTAL;

		button = new Button("New Game");
		button.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				tetrisView.newGame();
			}
		});
		gbc.insets.bottom = 4;
		gbl.setConstraints(button, gbc);
		gbc.insets.bottom = 0;
		add(button);

		pauseButton = new Button("Pause");
		gbc.insets.bottom = 4;
		gbl.setConstraints(pauseButton, gbc);
		pauseButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				resumeButton.setEnabled(true);
				resumeButton.requestFocus();
				pauseButton.setEnabled(false);
			}
		});
		gbc.insets.bottom = 0;
		add(pauseButton);

		resumeButton = new Button("Resume");
		resumeButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				tetrisView.requestFocus();
			}
		});
		gbc.insets.bottom = 4;
		gbl.setConstraints(resumeButton, gbc);
		gbc.insets.bottom = 0;
		add(resumeButton);

		button = new Button("Extended");
		button.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				if (tetrisView.isExtended()) {
					tetrisView.setExtended(false);
					((Button)e.getSource()).setLabel("Extended");
				}
				else {
					tetrisView.setExtended(true);
					((Button)e.getSource()).setLabel("Standard");
				}
				tetrisView.requestFocus();
			}
		});
		gbc.insets.bottom = 4;
		gbl.setConstraints(button, gbc);
		gbc.insets.bottom = 0;
		add(button);

		button = new Button("Upside Down");
		button.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				if (tetrisView.isPaintUpsideDown()) {
					tetrisView.setPaintUpsideDown(false);
					((Button)e.getSource()).setLabel("Upside Down");
				}
				else {
					tetrisView.setPaintUpsideDown(true);
					((Button)e.getSource()).setLabel("Upside Right");
				}
				tetrisView.requestFocus();
			}
		});
		gbc.insets.bottom = 4;
		gbl.setConstraints(button, gbc);
		gbc.insets.bottom = 0;
		add(button);

		topLevelParent = getFrameParent();

		button = new Button("Background");
		button.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				if (topLevelParent != null) {
					FileDialog file_dialog = new FileDialog(topLevelParent);
					file_dialog.show();
					String file = file_dialog.getFile();
					if (file != null) {
						tetrisView.
							setBackgroundImage
								(file_dialog.getDirectory() + file);
					}
				}
				tetrisView.requestFocus();
			}
		});
		gbc.insets.bottom = 4;
		gbl.setConstraints(button, gbc);
		gbc.insets.bottom = 0;
		add(button);

		linesText = new Label("Lines:  ");
		gbl.setConstraints(linesText, gbc);
		gbc.insets.top = 0;
		add(linesText);

		tetrisText = new Label("Tetris:  ");
		gbl.setConstraints(tetrisText, gbc);
		add(tetrisText);

		blocksText = new Label("Blocks: ");
		gbl.setConstraints(blocksText, gbc);
		add(blocksText);

		timeText = new Label("Time: ");
		gbl.setConstraints(timeText, gbc);
		add(timeText);

		tetrisPiecePreview = new TetrisPiecePreview(tetrisView);
		gbc.anchor = GridBagConstraints.SOUTH;
		gbc.weighty = 1;
		gbc.insets.bottom = 4;
		gbl.setConstraints(tetrisPiecePreview, gbc);
		gbc.insets.bottom = 0;
		gbc.weighty = 0;
		add(tetrisPiecePreview);

		button = new Button("Hide Next");
		button.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				showingNext = !showingNext;
				if (showingNext) {
					tetrisPiecePreview.setVisible(true);
					((Button)e.getSource()).setLabel("Hide Next");
				}
				else {
					tetrisPiecePreview.setVisible(false);
					((Button)e.getSource()).setLabel("Show Next");
				}
				tetrisView.requestFocus();
			}
		});
		gbl.setConstraints(button, gbc);
		add(button);

		tetrisView.addTetrisViewListener(new TetrisViewAdapter()
		{
			public void gameBegun()
			{
				Date date = new Date();
				beginTime = date.getTime() / 1000;
				pauseButton.setEnabled(true);
				resumeButton.setEnabled(false);
				tetrisView.initializeSpeed();
			}

			public void gameOver()
			{
				tetrisPiecePreview.newPiece();
				pauseButton.setEnabled(false);
				resumeButton.setEnabled(false);
			}

			public void gamePaused()
			{
				gamePaused = true;
				pauseButton.setEnabled(false);
				resumeButton.setEnabled(true);
			}

			public void gameResumed()
			{
				gamePaused = false;
				pauseButton.setEnabled(true);
				resumeButton.setEnabled(false);
			}

			public void linesCleared()
			{
				linesText.setText("Lines: " +
								   (tetrisView.getLinesClearedCount()));
				if (tetrisView.getLinesClearedCount() % 10 == 0) {
					tetrisView.incrementSpeed();
				}
			}

			public void tetrisCleared()
			{
				tetrisText.setText("Tetri: " +
									(tetrisView.getTetrisCount()));
			}

			public void pieceStarted()
			{
				tetrisPiecePreview.newPiece();
			}

			public void pieceFalling()
			{
				Date date = new Date();
				long time = date.getTime() / 1000;
				long seconds = time - beginTime;
				long minutes = seconds / 60; seconds %= 60;
				long hours = minutes / 60; minutes %= 60;
				if (hours > 0) {
					timeText.setText("Time: " + hours + ":" +
												minutes + ":" + seconds);
				}
				else {
					timeText.setText("Time: " + minutes + ":" + seconds);
				}
			}

			public void pieceDropped()
			{
				blocksText.setText("Blocks: " +
									(tetrisView.getPiecesDroppedCount()));
			}
		});
	}

	// ----------------------------------------------------------------------
	//
	protected Frame getFrameParent()
	{
		for (Component w = this ; w != null ; w = w.getParent()) {
			if (w instanceof Frame) {
				return (Frame)w;
			}
		}
		return null;
	}

	// ----------------------------------------------------------------------
	//
	TetrisView			tetrisView;
	TetrisPiecePreview	tetrisPiecePreview;
	Button				pauseButton;
	Button				resumeButton;
	Label				linesText;
	Label				tetrisText;
	Label				blocksText;
	Label				timeText;
	long				beginTime;
	boolean				showingNext = true;
	boolean				gamePaused = false;
	Frame				topLevelParent = null;
}

// --------------------------------------------------------------------------
//
class TetrisPiecePreview extends Canvas
{
	// ----------------------------------------------------------------------
	//
	public TetrisPiecePreview(TetrisView tetris_view)
	{
		tetrisView = tetris_view;
	}

	// ----------------------------------------------------------------------
	//
	public void newPiece()
	{
		Graphics g = getGraphics();
		paint(g);
		g.dispose();
	}

	// ----------------------------------------------------------------------
	//
	public void paint(Graphics g)
	{
		Dimension size = getSize();

		g.setColor(SystemColor.controlShadow);
		g.fillRect(2, 2, size.width - 4, size.height - 4);

		TetrisPiece piece = tetrisView.getNextPiece();
		if (piece != null) {
			int save_block_pixel_size = piece.getBlockPixelSize();
			piece.setBlockPixelSize(blockPixelSize);
			int x = (size.width - piece.getWidth() * blockPixelSize) / 2;
			int y = (size.height - piece.getHeight() * blockPixelSize) / 2;
			piece.paint(g, x, y);
			piece.setBlockPixelSize(save_block_pixel_size);
		}

		// Paint the border.

		int x = 0 , y = 0, w = size.width, h = size.height;

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

	// ----------------------------------------------------------------------
	//
	public Dimension getPreferredSize()
	{
		return new Dimension(blockPixelSize * 4 + 8,
							 blockPixelSize * 4 + 8);
	}

	// ----------------------------------------------------------------------
	//
	public Dimension getMinimumSize()
	{
		return getPreferredSize();
	}

	// ----------------------------------------------------------------------
	//
	TetrisView		tetrisView;
	int				blockPixelSize	= 12;
}
