//	Timer.java
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
//	04/14/97 dgm	Original.
// --------------------------------------------------------------------------

import java.awt.*;

public class Timer extends Object
{
	// ------------------------------------------------------------------
	//
	public Timer()
	{
	}

	// ------------------------------------------------------------------
	//
	public Timer(int m)
	{
		milliseconds = m;
	}

	// ----------------------------------------------------------------------
	//
	public Timer(TimerListener l)
	{
		addTimerListener(l);
	}

	// ----------------------------------------------------------------------
	//
	public Timer(int m, TimerListener l)
	{
		milliseconds = m;
		addTimerListener(l);
	}

	// ----------------------------------------------------------------------
	//
	public Timer(TimerListener l, int m)
	{
		milliseconds = m;
		addTimerListener(l);
	}

	// ----------------------------------------------------------------------
	//
	public void start()
	{
		if (!timing) {
			if (thread == null) {
				thread = new TimerThread();
			}
			timing = true;
			if (paused) {
				paused = false;
				thread.resume();
			}
			else {
				thread.start();
			}
		}
	}

	// ----------------------------------------------------------------------
	//
	public void start(int m)
	{
		milliseconds = m;
		start();
	}

	// ----------------------------------------------------------------------
	//
	public void pause()
	{
		if (timing) {
			paused = true;
			timing = false;
			if (!callingListener) {
				thread.suspend();
			}
		}
	}

	// ----------------------------------------------------------------------
	//
	public void stop()
	{
		pause();
	}

	// ----------------------------------------------------------------------
	//
	public void resume()
	{
		if (paused) {
			paused = false;
			timing = true;
			thread.resume();
		}
	}

	// ----------------------------------------------------------------------
	//
	public boolean isTiming()
	{
		return timing;
	}

	// ----------------------------------------------------------------------
	//
	public boolean isPaused()
	{
		return paused;
	}

	// ----------------------------------------------------------------------
	//
	public boolean isStopped()
	{
		return isPaused();
	}

	// ----------------------------------------------------------------------
	//
	public synchronized void addTimerListener(TimerListener l)
	{
		if (listener == null) {
			listener = l;
		}
	}

	// ----------------------------------------------------------------------
	//
	public synchronized void removeTimerListener(TimerListener l)
	{
		if (listener == l) {
			listener = null;
		}
	}

	// ----------------------------------------------------------------------
	//
	public void setDelay(int m)
	{
		milliseconds = m;
	}

	// ----------------------------------------------------------------------
	//
	public void setTimeout(int m)
	{
		milliseconds = m;
	}

	// ----------------------------------------------------------------------
	//
	public int getTimeout()
	{
		return milliseconds;
	}

	// ----------------------------------------------------------------------
	//
	public boolean isCallingListener()
	{
		return callingListener;
	}

	// ------------------------------------------------------------------
	//
	public class TimerThread extends Thread
	{
		public void run()
		{
			while (timing) {
				try
				{
					sleep(milliseconds);
				}
				catch (InterruptedException e)
				{
				}
				catch (Throwable e)
				{
				}
				if (!callingListener && (listener != null)) {
					callingListener = true;
					listener.timeout(event);
					//
					// java.awt.Toolkit toolkit =
					// 				 java.awt.Toolkit.getDefaultToolkit();
					// java.awt.EventQueue event_queue =
					// 					toolkit.getSystemEventQueue();
					// event_queue.postEvent(event);
					//
					callingListener = false;
				}
			}
		}
	}
	// ----------------------------------------------------------------------
	//
	protected TimerListener	listener		= null;
	protected int			milliseconds	= 1000;
	protected boolean		timing			= false;
	protected boolean		paused			= false;
	protected Thread		thread			= null;
	protected boolean		callingListener	= false;
	protected TimerEvent	event			= new TimerEvent(this);
}
