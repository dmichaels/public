//	TimerTestApplet.java
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
//	04/07/97 dgm	Original.
// --------------------------------------------------------------------------

import java.awt.*;
import java.awt.event.*;
import java.applet.*;
import com.sun.java.swing.*;

// --------------------------------------------------------------------------
//
public class TimerTestApplet extends Applet
{
	public TimerTestApplet()
	{
		applet = this;
		// startJTimer();
		setGreetingColor(Color.red);
		startTimer();
	}

	public void startTimer()
	{
		Timer timer = new Timer(500,
								new TimerAdapter()
								{
									public void timeout(TimerEvent e)
									{
										paintGreeting = !paintGreeting;
										repaint();
									}
								});
		timer.start();
	}

/*
	public void startJTimer()
	{
		JTimer timer = new JTimer(500,
								  new ActionListener()
								  {
									public void actionPerformed(ActionEvent e)
									{
										paintGreeting = !paintGreeting;
										repaint();
									}
								  });
		timer.start();
	}
*/

	public void init()
	{
		setSize(getPreferredSize());
	}

	public void paint(Graphics g)
	{
		if (paintGreeting) {
			g.drawString("Hello, world!", 40, 40);
		}
	}

	public Dimension getPreferredSize()
	{
		return new Dimension(500, 300);
	}

	public static void setGreetingColor(Color color)
	{
		greetingColor = color;
		applet.repaint();
	}

	public boolean	paintGreeting = true;
	public static Color greetingColor = null;
	public static TimerTestApplet applet = null;
}

