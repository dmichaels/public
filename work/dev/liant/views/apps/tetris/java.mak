# java.mak
#
# Java Applet Makefile
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

!include java.inc

# --------------------------------------------------------------------------

default:			run-standalone

# --------------------------------------------------------------------------

standalone:			run-standalone
applet:				run-applet

# --------------------------------------------------------------------------

run:				run-standalone
run-standalone:		build-standalone
					java $(STANDALONE_APPLET) $(APPLET)
run-applet:			build-applet
					appletviewer $(HTML_FILE)

# --------------------------------------------------------------------------

build:				build-standalone
build-standalone:	$(CLASSES) $(STANDALONE_CLASS)
build-applet:		$(CLASSES) $(HTML_FILE)

# --------------------------------------------------------------------------

jar:				$(JAR_FILE)
html:				$(HTML_FILE)

# --------------------------------------------------------------------------

!if "$(DEBUG)" == "1"
JAVA_DEBUG_FLAGS	= -g
!else
JAVA_DEBUG_FLAGS	=
!endif

JAVA_COMPILER		= javac
JAVA_COMPILER_FLAGS	= -deprecation $(JAVA_DEBUG_FLAGS)
JAVA_COMPILE		= $(JAVA_COMPILER) $(JAVA_COMPILER_FLAGS)

# --------------------------------------------------------------------------

$(HTML_FILE):		$(JAR_FILE)
					@echo <<$(HTML_FILE)
<title>$(APPLET)</title>
<center><h2><b><u>$(APPLET)</u></b></h2></center><br><hr><br><br>
<center><applet
	archive=$(JAR_FILE)
	code=$(APPLET).class
	width=$(APPLET_WIDTH)
	height=$(APPLET_HEIGHT)>
</applet></center>
<br><br><hr>
<<KEEP

# --------------------------------------------------------------------------

$(JAR_FILE):		$(JAVA_OBJECTS)
					jar cvf $(JAR_FILE) \
							$(CLASSES) $(OTHER_FILES_FOR_JAR_FILE)

# --------------------------------------------------------------------------

clean:
					@if exist *.cla del *.cla
					@if exist *.class del *.class
					@if exist *.jar del *.jar

# --------------------------------------------------------------------------

.SUFFIXES : .java .class
.java.class:
	$(JAVA_COMPILE) $<
