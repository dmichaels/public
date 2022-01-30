//	TimerEvent.java
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

public class TimerEvent extends java.awt.AWTEvent
{
	public TimerEvent(Timer timer)
	{
		super(timer, 1);
	}
}
