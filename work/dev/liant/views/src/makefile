#	makefile
#
#	Allegris Foundation 1.1.00 Library Makefile
#
#	Allegris Foundation 1.1.00
#	Copyright (c) 1997 by INTERSOLV, Inc.
#	+-----------------------------------------------------------------+
#	| This product is the property of INTERSOLV, Inc. and is licensed |
#	| pursuant to a written license agreement.  No portion of  this   |
#	| product may be reproduced without the written permission of     |
#	| INTERSOLV, Inc. except pursuant to the license agreement.       |
#	+-----------------------------------------------------------------+
#
#	Revision History:
#	-----------------
#	01/01/97 dgm	Original.
# --------------------------------------------------------------------------

default:	dll

dll:
	nmake -f vwinms.mak CV_TARGET=32 \
						CV_DLL=1 \
						CV_NOOLE=1 \
						CV_DEBUG=0 \
						CV_ASSERT=0 \
						CV_SYMBOLS=0

static:
	nmake -f vwinms.mak CV_TARGET=32 \
						CV_DLL=0 \
						CV_NOOLE=1 \
						CV_DEBUG=0 \
						CV_ASSERT=0 \
						CV_SYMBOLS=0 MAKDEF=1

vc-dll-debug:
	@call usevc42
	nmake -f vwinms.mak CV_TARGET=32 \
						CV_DLL=1 \
						CV_NOOLE=1 \
						CV_DEBUG=0 \
						CV_ASSERT=0 \
						CV_SYMBOLS=1 > ..\log\vc42\makev32d.log
	@if exist *.lib mv *.lib ..\lib
	@if exist *.dll mv *.dll ..\bin
#	@if exist *.obj mv *.obj ..\obj\vc32d
#	@if exist *.def mv *.def ..\obj\vc32d
#	@if exist *.map mv *.map ..\obj\vc32d

bc-all:	bc bc-dll bc-16 bc-16-dll

vc-all: vc vc-dll vc-16 vc-16-dll vc22 vc22-dll

# --------------------------------------------------------------------------

help:
		@echo Make Allegris Foundation 1.1.00 Libraries:
		@echo -- make bc
		@echo -- make bc-dll
		@echo -- make bc-16
		@echo -- make bc-16-dll
		@echo -- make vc
		@echo -- make vc-dll
		@echo -- make vc-16
		@echo -- make vc-16-dll
		@echo -- make vc22
		@echo -- make vc22-dll

# --------------------------------------------------------------------------
# Borland C++ 5.0 (32-bit, static)
# --------------------------------------------------------------------------

bc:
	@call usebc5
	make -f vwinbor.mak -DCV_TARGET=32 \
						-DCV_DLL=0 \
						-DCV_NOOLE=1 \
						-DCV_DEBUG=0 \
						-DCV_ASSERT=0 \
						-DCV_SYMBOLS=0 > ..\log\bc5\makeb32.log
	@if exist *.lib mv *.lib ..\lib
	@if exist *.dll mv *.dll ..\bin
	@if exist *.obj mv *.obj ..\obj\bc32
	@if exist *.def mv *.def ..\obj\bc32
	@if exist *.map mv *.map ..\obj\bc32

# --------------------------------------------------------------------------
# Borland C++ 5.0 (32-bit, DLL)
# --------------------------------------------------------------------------

bc-dll:
#	@call usebc5
	make -f vwinbor.mak -DCV_TARGET=32 \
						-DCV_DLL=1 \
						-DCV_NOOLE=1 \
						-DCV_DEBUG=0 \
						-DCV_ASSERT=0 \
						-DCV_SYMBOLS=0 > ..\log\bc5\makeb32d.log
	@if exist *.lib mv *.lib ..\lib
	@if exist *.dll mv *.dll ..\bin
	@if exist *.obj mv *.obj ..\obj\bc32d
	@if exist *.def mv *.def ..\obj\bc32d
	@if exist *.map mv *.map ..\obj\bc32d

# --------------------------------------------------------------------------
# Borland C++ 5.0 (16-bit, static)
# --------------------------------------------------------------------------

bc-16:
	@call usebc5
	make -f vwinbor.mak -DCV_TARGET=16 \
						-DCV_DLL=0 \
						-DCV_NOOLE=1 \
						-DCV_DEBUG=0 \
						-DCV_ASSERT=0 \
						-DCV_SYMBOLS=0 > ..\log\bc5\makeb16.log
	@if exist *.lib mv *.lib ..\lib
	@if exist *.dll mv *.dll ..\bin
	@if exist *.obj mv *.obj ..\obj\bc16
	@if exist *.def mv *.def ..\obj\bc16
	@if exist *.map mv *.map ..\obj\bc16

# --------------------------------------------------------------------------
# Borland C++ 5.0 (16-bit, DLL)
# --------------------------------------------------------------------------

bc-16-dll:
	@call usebc5
	make -f vwinbor.mak -DCV_TARGET=16 \
						-DCV_DLL=1 \
						-DCV_NOOLE=1 \
						-DCV_DEBUG=0 \
						-DCV_ASSERT=0 \
						-DCV_SYMBOLS=0 > ..\log\bc5\makeb16d.log
	@if exist *.lib mv *.lib ..\lib
	@if exist *.dll mv *.dll ..\bin
	@if exist *.obj mv *.obj ..\obj\bc16d
	@if exist *.def mv *.def ..\obj\bc16d
	@if exist *.map mv *.map ..\obj\bc16d

# --------------------------------------------------------------------------
# Microsoft Visual C++ 4.2 (32-bit, static)
# --------------------------------------------------------------------------

vc:
	@call usevc42
	nmake -f vwinms.mak CV_TARGET=32 \
						CV_DLL=0 \
						CV_NOOLE=1 \
						CV_DEBUG=0 \
						CV_ASSERT=0 \
						CV_SYMBOLS=0 MAKDEF=1 > ..\log\vc42\makev32.log
	@if exist *.lib mv *.lib ..\lib
	@if exist *.dll mv *.dll ..\bin
	@if exist *.obj mv *.obj ..\obj\vc32
	@if exist *.def mv *.def ..\obj\vc32
	@if exist *.map mv *.map ..\obj\vc32

# --------------------------------------------------------------------------
# Microsoft Visual C++ 4.2 (32-bit, DLL)
# --------------------------------------------------------------------------

vc-dll:
#	@call usevc42
	nmake -f vwinms.mak CV_TARGET=32 \
						CV_DLL=1 \
						CV_NOOLE=1 \
						CV_DEBUG=0 \
						CV_ASSERT=0 \
						CV_SYMBOLS=0 > ..\log\vc42\makev32d.log
	@if exist *.lib mv *.lib ..\lib
	@if exist *.dll mv *.dll ..\bin
	@if exist *.obj mv *.obj ..\obj\vc32d
	@if exist *.def mv *.def ..\obj\vc32d
	@if exist *.map mv *.map ..\obj\vc32d

# --------------------------------------------------------------------------
# Microsoft Visual C++ 1.52c (16-bit, static)
# --------------------------------------------------------------------------

vc-16:
	@call usevc15
	nmake -f vwinms.mak CV_TARGET=16 \
						CV_DLL=0 \
						CV_NOOLE=1 \
						CV_DEBUG=0 \
						CV_ASSERT=0 \
						CV_SYMBOLS=0 > ..\log\vc15\makev16.log
	@if exist *.lib mv *.lib ..\lib
	@if exist *.dll mv *.dll ..\bin
	@if exist *.obj mv *.obj ..\obj\vc16
	@if exist *.def mv *.def ..\obj\vc16
	@if exist *.map mv *.map ..\obj\vc16

# --------------------------------------------------------------------------
# Microsoft Visual C++ 1.52c (16-bit, DLL)
# --------------------------------------------------------------------------

vc-16-dll:
	@call usevc15
	nmake -f vwinms.mak CV_TARGET=16 \
						CV_DLL=1 \
						CV_NOOLE=1 \
						CV_DEBUG=0 \
						CV_ASSERT=0 \
						CV_SYMBOLS=0 > ..\log\vc15\makev16d.log
	@if exist *.lib mv *.lib ..\lib
	@if exist *.dll mv *.dll ..\bin
	@if exist *.obj mv *.obj ..\obj\vc16d
	@if exist *.def mv *.def ..\obj\vc16d
	@if exist *.map mv *.map ..\obj\vc16d

# --------------------------------------------------------------------------
# Microsoft Visual C++ 2.2 (32-bit, static)
# --------------------------------------------------------------------------

vc22:
	@call usevc22
	nmake -f vwinms.mak CV_TARGET=32 \
						CV_DLL=0 \
						CV_NOOLE=1 \
						CV_DEBUG=0 \
						CV_ASSERT=0 \
						CV_SYMBOLS=0 > ..\log\vc22\makev32.log
	@if exist *.lib mv *.lib ..\lib\vc22
	@if exist *.dll mv *.dll ..\bin\vc22
	@if exist *.obj mv *.obj ..\obj\vc22
	@if exist *.def mv *.def ..\obj\vc22
	@if exist *.map mv *.map ..\obj\vc22

# --------------------------------------------------------------------------
# Microsoft Visual C++ 2.2 (32-bit, DLL)
# --------------------------------------------------------------------------

vc22-dll:
	@call usevc22
	nmake -f vwinms.mak CV_TARGET=32 \
						CV_DLL=1 \
						CV_NOOLE=1 \
						CV_DEBUG=0 \
						CV_ASSERT=0 \
						CV_SYMBOLS=0 > ..\log\vc22\makev32d.log
	@if exist *.lib mv *.lib ..\lib\vc22
	@if exist *.dll mv *.dll ..\bin\vc22
	@if exist *.obj mv *.obj ..\obj\vc22d
	@if exist *.def mv *.def ..\obj\vc22d
	@if exist *.map mv *.map ..\obj\vc22d
