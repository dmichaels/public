------------------------------------------------------------------------------
 This directory contains a Java Tetris knockoff.  It currently only runs
 correctly as an application; as an applet (using IE 4.0 or appletviewer),
 the Tetris blocks don't automatically go down because the timer class I'm
 using (JTimer) from the JFC currently has a security bug which prevents it
 from working within applets.  Sun has acknowledged this bug and is working
 on a fix.  I did write my own Timer class, which works ok, but is not
 appropriate because timeouts occur asynchronously rather than synchronously
 with respect to the system GUI event queue; to make it work synchronously
 would require access to the system event queue, which is possible (Toolkit.
 getSystemEventQueue(), but which is not allowed from an applet -- thus the
 problem with JTimer.

 To run TetrisApplet as a standalone application, simply type:
 
 	nmake

 This assumes of course you have your Java environment setup correctly.
------------------------------------------------------------------------------
