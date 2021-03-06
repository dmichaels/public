# java.inc
#
# Java Applet Makefile Include File
#
# Copyright (c) 1997 by INTERSOLV, Inc.
# +-----------------------------------------------------------------+
# | This product is the property of INTERSOLV, Inc. and is licensed |
# | pursuant to a written license agreement.  No portion of this    |
# | product may be reproduced without the written permission of     |
# | INTERSOLV, Inc. except pursuant to the license agreement.       |
# +-----------------------------------------------------------------+
#
# Revision History:
# -----------------
# 04/01/97 dgm	Original.
# -------------------------------------------------------------------------

DEBUG						= 0
APPLET_WIDTH				= 345
APPLET_HEIGHT				= 480
HTML_FILE					= $(APPLET).html
JAR_FILE					= $(APPLET).jar
STANDALONE_APPLET			= StandaloneApplet
STANDALONE_CLASS			= $(STANDALONE_APPLET).class

# -------------------------------------------------------------------------

APPLET						= TetrisApplet
CLASSES						= $(APPLET).class \
							  TetrisView.class \
							  TetrisViewListener.class \
							  TetrisViewAdapter.class \
							  TetrisControlPanel.class \
							  TetrisPiece.class \
							  ColorScheme.class \
							  Timer.class \
							  TimerEvent.class \
							  TimerListener.class \
							  TimerAdapter.class
		
OTHER_FILES_FOR_JAR_FILE	= tetris.gif
