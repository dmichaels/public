//	TetrisApplet.java
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
import TetrisView;
import TetrisControlPanel;

// --------------------------------------------------------------------------
//
public class TetrisApplet extends Applet
{
	TetrisView			tetrisView			= null;
	TetrisControlPanel	tetrisControlPanel	= null;

	public TetrisApplet()
	{
	}

	public void init()
	{
		setBackground(SystemColor.control);
		setForeground(SystemColor.controlText);

		tetrisView = new TetrisView(this);
		tetrisControlPanel = new TetrisControlPanel(tetrisView);

		GridBagLayout gbl = new GridBagLayout();
		setLayout(gbl);

		GridBagConstraints gbc;

		gbc = new GridBagConstraints();
		gbc.insets = new Insets(6, 6, 6, 0);
		gbc.fill = GridBagConstraints.BOTH;
		gbc.weightx = 1;
		gbc.weighty = 1;
		gbl.setConstraints(tetrisView, gbc);
		add(tetrisView);

		gbc = new GridBagConstraints();
		gbc.insets = new Insets(8, 4, 8, 8);
		gbc.fill = GridBagConstraints.VERTICAL;
		gbl.setConstraints(tetrisControlPanel, gbc);
		add(tetrisControlPanel);

		tetrisView.requestFocus();

		Dimension size = getPreferredSize();
		setSize(getPreferredSize());
		System.err.println((getPreferredSize().width)+"x"+(getPreferredSize().height));
	}

	public void start()
	{
		tetrisView.start();
	}

	public void stop()
	{
		tetrisView.pause();
	}

	public Dimension getPreferredSize()
	{
		Dimension size = super.getPreferredSize();
		size.width += 4;
		size.height += 4;
		return size;
	}

	// ----------------------------------------------------------------------
	//
	public Dimension getMinimumSize()
	{
		return getPreferredSize();
	}

	public void paint(Graphics g)
	{
		Dimension size = getSize();
		int x = 0 , y = 0, w = size.width, h = size.height;

		// Paint the top/left outer shadow.

		g.setColor(SystemColor.controlHighlight);
		g.drawLine(x, y, x + w - 1, y);
		g.drawLine(x, y + 1, x, y + h - 1);

		// Paint the bottom/right outer shadow.

		g.setColor(SystemColor.controlDkShadow);
		g.drawLine(x, y + h - 1, x + w - 1, y + h - 1);
		g.drawLine(x + w - 1, y, x + w - 1, y + h - 1);

		// Paint the top/left inner shadow.

		g.setColor(SystemColor.controlLtHighlight);
		g.drawLine(x + 1, y + 1, x + w - 2, y + 1);
		g.drawLine(x + 1, y + 2, x + 1, y + h - 2);

		// Paint the bottom/right inner shadow.

		g.setColor(SystemColor.controlShadow);
		g.drawLine(x + 1, y + h - 2, x + w - 2, y + h - 2);
		g.drawLine(x + w - 2, y + 1, x + w - 2, y + h - 2);
	}
}
