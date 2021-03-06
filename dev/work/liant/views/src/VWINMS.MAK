# $Header:   Q:/views/win/vcs/vwinms.mav   1.21   Mar 26 1997 18:31:42   gregm  $
#
# --------------------------------------------------------------------------
#	vwinms.mak
#
#	Microsoft Visual C++ 1.5/4.2 for Windows library makefile
#
#	Allegris Foundation 1.1.00 for Microsoft Windows
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
#	11/27/95 glm    Original from cvb.mak
#	11/29/95 glm    default: 32-bit DLL
#	12/05/95 glm	default: CV_DEBUG, CV_ASSERT
#	12/06/95 dss	change vole2ui.lib voleui32.lib. Added globals2.obj.
#					LINK_DEBUG_FLAGS = /CO for 16 bit
#	12/12/95 dss	LINK_DEBUG_FLAGS += -DEBUG for 32 bit.
#					Removed aliases	STATIC, DLL, WIN32,WIN16
#					to avoid possible conflicts.
#	01/10/96 dss	dll16, lib32, and other pseudo targets: command goes
#					on line AFTER target. It's not a dependency.
#	02/09/96 glm	16-bit static uses export far fn for winstart
#	02/14/96 glm	added CV_SRC, CV_OBJ, TARGET_NAME
#	09/14/96 glm	independent CV_DEBUG, CV_ASSERT, CV_SYMBOLS
#					preened, added Alpha support, CV_OUT, CV_USER, CV_OPT
#	03/20/97 glm	added Allegris Constructor settings
# --------------------------------------------------------------------------

# --------------------------------------------------------------------------
# Using this makefile, build libraries, or individual objects.
# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# To build individual object files use:
#
#	nmake [nmake_options] -f vwinms.mak [v_defs] [object_files...]
#
# V_DEFS:
#	CV_TARGET=16	   --> for 16-bit build    (Default: CV_TARGET=32)
#	CV_DLL=0		   --> for static build	   (Default: CV_DLL=1)
#	CV_SYMBOLS=0	   --> 0= no Debug symbols (Default: CV_DEBUG=1)
#	CV_DEBUG=0 		   --> 0= CV_Debug off	   (Default: CV_DEBUG=1)
#	CV_ASSERT=0		   --> 0= CV_Assert off	   (Default: CV_DEBUG=1)
#	CV_CTL3D=0 		   --> 0= no ctl3d 		   (Default: CV_CTL3D=1)
#	CV_NOOLE=0 		   --> 0= OLE on		   (Default: CV_NOOLE=1)
#	CV_NOOLEDRAGDROP=0 --> 0= Drag-and-Drop on (Default: CV_NOOLEDRAGDROP=0)
#	MAKEDEF=0 		   --> 0= no new .DEF file (Default: MAKEDEF=1)
#
# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# To build a library target:
#
#	nmake [nmake_options] -f vwinms.mak [v_defs] [targets]
#
# TARGETS:
#	dll32: vm32.dll vm32i.lib (Views Microsoft 32-bit DLL and import library)
#	dll16: vm16.dll vm16i.lib (Views Microsoft 16-bit DLL and import library)
#
#	lib32: vm32d.lib vm32g.lib (Views Microsoft 32-bit GUI and data libraries)
#	lib16: vm16d.lib vm16g.lib (Views Microsoft 16-bit GUI and data libraries)
#
#	data32:	vm32d.lib (Views Microsoft 32-bit data library)
#	gui32:	vm32g.lib (Views Microsoft 32-bit GUI library)
#	data16:	vm16d.lib (Views Microsoft 16-bit data library)
#	gui16:	vm16g.lib (Views Microsoft 16-bit GUI library)
#
#	If V_DEFS CV_TARGET or CV_DLL are specified, the appropriate 
#	library/dll will be constructed without specifying a target.
#	Specifying a target will force use of that target's CV_TARGET
#	and CV_DLL setting
#
# --------------------------------------------------------------------------

# --------------------------------------------------------------------------
# Allegris Constructor specific settings
#
!if "$(ALLEGRIS_CONSTRUCTOR)"!=""
CV_SRC=..
CV_OBJ=dllobj
DLL_IMPLIB_SUFFIX=i
CV_USER=-Zp4
CV_USER_LINK=-section:.text,ERS
CV_NOOLE=0
!if "$(TARGET_NAME)"==""
TARGET_NAME=vm32m41
!endif
!if "$(ALLEGRIS_CONSTRUCTOR)"=="nodebug"
CV_OPT=-Oisgyb1 -Gf -Gs
#
# O1	optimize for size
# includes the following: 
#   /Ob<n> inline expansion (default n=0)
#   /Og enable global optimization
#   /Os favor code space
#   /Oy[-] enable frame pointer omission
#   /Gf enable string pooling
#   /Gs[num] disable stack checking calls
#   /Gy separate functions for linker
#
#   /Oi enable intrinsic functions
#
CV_SYMBOLS=0
!endif  ## nodebug
!endif  ## constructor
# --------------------------------------------------------------------------

# --------------------------------------------------------------------------
# default directory settings
#
!if "$(CV_SRC)"==""
CV_SRC=.
!endif

!if "$(CV_OBJ)"==""
CV_OBJ=.
!endif

!if "$(CV_OUT)"==""
CV_OUT=.
!endif

# --------------------------------------------------------------------------
# set default switches
#
!if "$(CV_TARGET)"!="16"
CV_TARGET               = 32
!endif
!if "$(CV_DLL)"!="0"
CV_DLL                  = 1
!endif
!if "$(CV_THREADSAFE)"!="0" && $(CV_TARGET)==32 && $(CV_DLL)
CV_THREADSAFE           = 1
!else
CV_THREADSAFE           = 0
!endif
!if "$(CV_SYMBOLS)"!="0"
CV_SYMBOLS               = 1
!endif
!if "$(CV_DEBUG)"!="0"
CV_DEBUG                = 1
!endif
!if "$(CV_ASSERT)"!="0"
CV_ASSERT               = 1
!endif
!if "$(CV_NOOLE)"!="0"
CV_NOOLE                = 1
!endif
!if "$(CV_NOOLEDRAGDROP)"!="0"
CV_NOOLEDRAGDROP		= 0
!endif
!if "$(CV_CTL3D)"!="0"
CV_CTL3D                = 1
!endif
!if "$(MAKEDEF)"!="0" && $(CV_DLL)
MAKEDEF                 = 1
!else
MAKEDEF                 = 0
!endif
!if "$(DLLENTRY)"=="" && $(CV_TARGET)==32 && $(CV_DLL)
DLLENTRY                = @12
!endif

# --------------------------------------------------------------------------
# Compiler macro definitions
# --------------------------------------------------------------------------
!if $(CV_TARGET)==16
SYSTEM_DEFINES          = -DCV_WIN16
!else
SYSTEM_DEFINES          = -DCV_WIN32 -DCV_IMOUSE
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# set flags for compile line defines
#
!if $(CV_DLL)
DLL_DEFINES             = -DIN_DLL
!endif
!if $(CV_THREADSAFE)
THREAD_FLAG             = -DCV_THREADSAFE
!endif
!if $(CV_DEBUG)
CV_DEBUG_FLAG			= -DCV_DEBUG
!endif
!if $(CV_ASSERT)
CV_ASSERT_FLAG			= -DCV_ASSERT
!endif

!if $(CV_CTL3D)
3D_FLAG                 = -DCV_CTL3D
!if $(CV_TARGET)==16
3D_LIB					= ctl3dv2.lib
!else
3D_LIB					= ctl3d32.lib
!endif
!else
3D_LIB					=
!endif

!if $(CV_NOOLE)
OLE_FLAG                = -DCV_NOOLE
!if "$(CV_NOOLEDRAGDROP)"=="0" && $(CV_TARGET)==32
OLE_LIBS                =\
						  ole32.lib\
						  oleaut32.lib\
						  oledlg.lib\
						  advapi32.lib\
						  uuid.lib\
						  shell32.lib\
						  voleui32.lib
!else
OLE_LIBS                =
!endif
!else
OLE_FLAG                = 
!if $(CV_TARGET)==16
OLE_LIBS                =\
						  ole2.lib\
						  ole2disp.lib\
						  typelib.lib\
						  compobj.lib\
						  storage.lib\
						  shell.lib\
						  voleui.lib
!else
OLE_LIBS                =\
						  ole32.lib\
						  oleaut32.lib\
						  oledlg.lib\
						  advapi32.lib\
						  uuid.lib\
						  shell32.lib\
						  voleui32.lib
!endif
!endif

!if $(CV_NOOLEDRAGDROP)
!else
OLE_FLAG				= $(OLE_FLAG) -DCV_OLE_DRAG_DROP
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
VIEWS_DEFINES           =\
                          $(CV_USER)\
                          $(CV_DEBUG_FLAG)\
                          $(CV_ASSERT_FLAG)\
						  $(3D_FLAG)\
                          $(THREAD_FLAG)\
                          $(OLE_FLAG)\
                          $(SYSTEM_DEFINES)
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DEFINES                 =\
						  $(VIEWS_DEFINES)\
                          $(DLL_DEFINES)

# --------------------------------------------------------------------------
# Library specific modules and definitions
# --------------------------------------------------------------------------

!if "$(TARGET_NAME)"!=""
root_name = $(TARGET_NAME)
!else
!if $(CV_TARGET)==16
root_name = vm16
!else
root_name = vm32
!endif
!endif

DATA_LIBRARY            = $(root_name)d
DATA_LIBRARY_FILE       = $(CV_OUT)\$(DATA_LIBRARY).lib

GUI_LIBRARY             = $(root_name)g
GUI_LIBRARY_FILE        = $(CV_OUT)\$(GUI_LIBRARY).lib

DATA_GUI_DLL            = $(root_name)
DATA_GUI_DLL_FILE       = $(CV_OUT)\$(DATA_GUI_DLL).dll
DLL_IMPLIB_NAME         = $(DATA_GUI_DLL)$(DLL_IMPLIB_SUFFIX)
DLL_IMPLIB_FILE         = $(CV_OUT)\$(DLL_IMPLIB_NAME).lib
DATA_GUI_DEF_FILE       = $(CV_OUT)\$(DATA_GUI_DLL).def

!if $(CV_DLL)
PDB_FILE                = $(CV_OUT)\$(root_name).pdb
!else
PDB_FILE                = $(CV_OUT)\$(root_name)s.pdb
!endif

# --------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------

INCLUDES                = -I"$(INCLUDE)"

# --------------------------------------------------------------------------
# Object files
# --------------------------------------------------------------------------

!include "vobjects.inc"

# --------------------------------------------------------------------------
# Rules
# --------------------------------------------------------------------------

.SUFFIXES : .o .obj .c .cxx .cpp .C
{$(CV_SRC)}.c{$(CV_OBJ)}.o:
	$(CC) $(CFLAGS) -Fo$@ $<
{$(CV_SRC)}.cxx{$(CV_OBJ)}.o:
	$(CC) $(CFLAGS) -Fo$@ $<
{$(CV_SRC)}.cpp{$(CV_OBJ)}.o:
	$(CC) $(CFLAGS) -Fo$@ $<
{$(CV_SRC)}.C{$(CV_OBJ)}.o:
	$(CC) $(CFLAGS) -Fo$@ $<
{$(CV_SRC)}.c{$(CV_OBJ)}.obj:
	$(CC) $(CFLAGS) -Fo$@ $<
{$(CV_SRC)}.cxx{$(CV_OBJ)}.obj:
	$(CC) $(CFLAGS) -Fo$@ $<
{$(CV_SRC)}.cpp{$(CV_OBJ)}.obj:
	$(CC) $(CFLAGS) -Fo$@ $<
{$(CV_SRC)}.C{$(CV_OBJ)}.obj:
	$(CC) $(CFLAGS) -Fo$@ $<

.c.o:
	$(CC) $(CFLAGS) -Fo$@ $<
.cxx.o:
	$(CC) $(CFLAGS) -Fo$@ $<
.cpp.o:
	$(CC) $(CFLAGS) -Fo$@ $<
.C.o:
	$(CC) $(CFLAGS) -Fo$@ $<
.c.obj:
	$(CC) $(CFLAGS) -Fo$@ $<
.cxx.obj:
	$(CC) $(CFLAGS) -Fo$@ $<
.cpp.obj:
	$(CC) $(CFLAGS) -Fo$@ $<
.C.obj:
	$(CC) $(CFLAGS) -Fo$@ $<

# --------------------------------------------------------------------------
# Library specific
# --------------------------------------------------------------------------
!if $(CV_DLL)
current_type:			dll
!else
current_type:			lib
!endif

all:					current_type

res:					cv32.res cv16.res

cv16.res:
						rc /r /dCV_RC_COMPILE /dCV_WIN16 cv.rc
						rename cv.res cv16.res
cv32.res:
						rc /dCV_RC_COMPILE /dCV_WIN32 cv.rc
						rename cv.res cv32.res

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

data_gui_dll:			$(DATA_GUI_DLL_FILE) vmdll.lib
dll:					data_gui_dll

dll32:					dll

dll16:
		$(MAKE) -$(MAKEFLAGS) -f vwinms.mak CV_TARGET=16 data_gui_dll

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

lib:					$(DATA_LIBRARY_FILE) $(GUI_LIBRARY_FILE)
data:					$(DATA_LIBRARY_FILE)
gui:					$(GUI_LIBRARY_FILE)

lib32:
		$(MAKE) -$(MAKEFLAGS) -f vwinms.mak CV_DLL=0 lib

data32:
		$(MAKE) -$(MAKEFLAGS) -f vwinms.mak CV_DLL=0 data

gui32:
		$(MAKE) -$(MAKEFLAGS) -f vwinms.mak CV_DLL=0 gui

lib16:
		$(MAKE) -$(MAKEFLAGS) -f vwinms.mak CV_DLL=0 CV_TARGET=16 lib

data16:
		$(MAKE) -$(MAKEFLAGS) -f vwinms.mak CV_DLL=0 CV_TARGET=16 data

gui16:
		$(MAKE) -$(MAKEFLAGS) -f vwinms.mak CV_DLL=0 CV_TARGET=16 gui

# --------------------------------------------------------------------------
# Compiler and options
# --------------------------------------------------------------------------

CC                      = cl

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
!if "$(CV_OPT)"==""
## -Od      disables all optimizations (for debugging)
CV_OPT                  = -Od
!endif

!if $(CV_SYMBOLS)
## -Zi      include debug info in .OBJ files using a pdb file. 
##          (cvpack will run out of memory if you use -Z7)
COMPILER_DEBUG_FLAGS    = -Zi -Fd$(PDB_FILE) $(CV_OPT)
!else
COMPILER_DEBUG_FLAGS    = $(CV_OPT)
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## -Zl      Omit default library name in .OBJ
## -W3      Warning level 3
COMMON_COMPILER_FLAGS   = -c -nologo -W3 -Zl

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

!if $(CV_TARGET)==16
!if $(CV_DLL)
## -Alfw    large memory model, far data pointers, SS!=DS
## -G2      286 instructions
## -GD      DLL protected-mode Win entry/exit code
## -GEf     create prolog/epilog code for all __far fnctions
COMPILER_FLAGS          = -Alfw -G2 -GD -GEf

!else
## -AL      large memory model
## -G2      286 instructions
## -GA      application protected-mode Win entry/exit code
## -GEf     create prolog/epilog code for all __far fnctions
COMPILER_FLAGS          = -AL -G2 -GA -GEf

!endif
!else
!if $(CV_DLL)
## -MD      to build with the DLL version of the C runtime library
##          and linking with MSVCRT.lib instead of LIBC.LIB.
##          Clients need to do the same
## -LD      Create .DLL
COMPILER_FLAGS          = -MD -LD

!else
COMPILER_FLAGS          =

!endif
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CFLAGS                 =\
						  $(COMMON_COMPILER_FLAGS)\
						  $(COMPILER_FLAGS)\
						  $(COMPILER_DEBUG_FLAGS)\
						  $(DEFINES)\
						  $(INCLUDES)

# --------------------------------------------------------------------------
# linker debug options
# --------------------------------------------------------------------------

!if $(CV_SYMBOLS)
!if $(CV_TARGET)==16
LINK_DEBUG_FLAGS        = /CO /MAP:FULL
!else
LINK_DEBUG_FLAGS        = -debug\
						  -debugtype:cv\
						  -pdb:$(PDB_FILE)\
						  -map:$(CV_OUT)\$(DATA_GUI_DLL).map
!endif
!else
LINK_DEBUG_FLAGS        =
!endif

# --------------------------------------------------------------------------
# Library archiver and options
# --------------------------------------------------------------------------

ARCHIVER                = lib
!if $(CV_TARGET)==16
ARCHIVE_FLAGS           = /NOI /PAGE:256
!endif
ARCHIVE                 = $(ARCHIVER)

# --------------------------------------------------------------------------
# DLL options
# --------------------------------------------------------------------------
!if $(CV_TARGET)==16
## /NOD			no default libraries
## /SEG:1024	segment alignment:1024
## /PACKF
## /PACKC
## /NOE
DLL_LINK                = link $(DLL_LINK_FLAGS)
DLL_LINK_FLAGS          = /NOD\
						  /SEG:1024\
						  /PACKF\
						  /PACKC\
						  /NOE\
						  $(CV_USER_LINK)\
                          $(LINK_DEBUG_FLAGS)
DLL_LINK_LIBRARIES      = ldllcew.lib\
						  libw.lib\
						  commdlg.lib\
						  oldnames.lib\
						  ole2nls.lib\
						  $(3D_LIB)\
						  $(OLE_LIBS)
DLL_STARTUP_OBJECT		= 
DLL_LINK_OBJECTS        = $(CV_OBJ)\dll.obj\
                          $(CV_OBJ)\globals2.obj\
                          $(CV_OBJ)\globals.obj
DLL_IMPLIB              = implib /noi /nol

#   Re: MSOBJECTS
#
#   The C++/Views library requires an sscanf() function.  
#   Note that Microsoft C/C++ 8.0 does not provide a sscanf() function 
#   for use in a DLL context.  In order to rebuild the C++/Views DLL under 
#   Microsoft C/C++ 8.0, you must first obtain these files from Microsoft:
# 
#       wsscanf.c 
#       wsscanf.h 
#       wstdarg.h
# 
#   If you have access to CompuServe, then you may download them from the 
#   Software Data Library (go MSL).  As of this writing, the queue number 
#   of the article is S13183.

MSOBJECTS = $(CV_OBJ)\wsscanf.obj

!else

!if "$(PROCESSOR_ARCHITECTURE)"!="Alpha"
MACHINE					= -machine:i386
!else
MACHINE					= -machine:alpha
!endif

## -DLL
## -SUBSYSTEM
## -NODEFAULTLIB
DLL_LINK                = link $(DLL_LINK_FLAGS)
DLL_LINK_FLAGS          = -dll\
						  -subsystem:windows\
						  -nodefaultlib\
						  $(MACHINE)\
                          $(LINK_DEBUG_FLAGS)
DLL_LINK_LIBRARIES      = msvcrt.lib\
						  oldnames.lib\
                          kernel32.lib\
						  user32.lib\
                          gdi32.lib\
						  comdlg32.lib\
                          $(3D_LIB)\
						  $(OLE_LIBS)
DLL_STARTUP_OBJECT		= 
DLL_LINK_OBJECTS        = $(CV_OBJ)\dll.obj\
                          $(CV_OBJ)\globals2.obj\
                          $(CV_OBJ)\globals.obj
DLL_IMPLIB              = lib $(MACHINE)

!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

$(DATA_LIBRARY_FILE):   $(ALL_DATA_OBJECTS)
		if exist $(DATA_LIBRARY_FILE) del $(DATA_LIBRARY_FILE)
!if $(CV_TARGET)==16
	$(ARCHIVE) $(DATA_LIBRARY_FILE) $(ARCHIVE_FLAGS) @<<
$(ALL_DATA_OBJECTS: = &^
)
;
<<

!else

	$(ARCHIVE) @<<
/out:$(DATA_LIBRARY_FILE) $(ALL_DATA_OBJECTS)
<<
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

$(GUI_LIBRARY_FILE):    $(ALL_GUI_OBJECTS)
		if exist $(GUI_LIBRARY_FILE) del $(GUI_LIBRARY_FILE)
!if $(CV_TARGET)==16
	$(ARCHIVE) $(GUI_LIBRARY_FILE) $(ARCHIVE_FLAGS) @<<
$(ALL_GUI_OBJECTS: = &^
)
;
<<

!else

	$(ARCHIVE) @<<
/out:$(GUI_LIBRARY_FILE) $(ALL_GUI_OBJECTS)
<<
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

!if $(CV_DLL)

!if $(CV_TARGET)==16
$(DATA_GUI_DLL_FILE):   $(OBJECTS)\
						$(MSOBJECTS)\
						$(DLL_LINK_OBJECTS)\
						$(DATA_GUI_DEF_FILE)
	$(DLL_LINK) @<<
$(MSOBJECTS: = +^
)+
$(OBJECTS: = +^
)+
$(DLL_LINK_OBJECTS: = +^
)
$(DATA_GUI_DLL_FILE)
$(CV_OUT)\$(DATA_GUI_DLL).map
$(DLL_LINK_LIBRARIES)
$(DATA_GUI_DEF_FILE)
<<

    $(DLL_IMPLIB) $(DLL_IMPLIB_FILE) $(DATA_GUI_DEF_FILE)
!else

$(DATA_GUI_DLL_FILE):   $(OBJECTS)\
						$(DLL_LINK_OBJECTS)\
						$(CV_OUT)\$(DLL_IMPLIB_NAME).exp\
						$(DATA_GUI_DEF_FILE)
	$(DLL_LINK) @<<$(DATA_GUI_DLL).CRF
$(CV_OUT)\$(DLL_IMPLIB_NAME).exp
$(DLL_LINK_OBJECTS) $(OBJECTS)
-out:$(DATA_GUI_DLL_FILE)
$(DLL_LINK_LIBRARIES)
<<

$(CV_OUT)\$(DLL_IMPLIB_NAME).exp $(DLL_IMPLIB_FILE) : $(DATA_GUI_DEF_FILE)
	$(DLL_IMPLIB) -out:$(DLL_IMPLIB_FILE) -def:$(DATA_GUI_DEF_FILE)

!endif

!if $(CV_TARGET)==16
$(DATA_GUI_DEF_FILE):	$(OBJECTS)
!if $(MAKEDEF)
	 @if exist $@ echo deleting existing module definition file $@...
	 @if exist $@ del $@
	 @echo creating module definition file $@...
	 @echo LIBRARY	$(DATA_GUI_DLL) >> $@
	 @echo DESCRIPTION	'Allegris Foundation 1.0.00 for Microsoft Windows DLL for Win16 and Microsoft-C++ 1.5' >> $@
	 @echo EXETYPE	WINDOWS >> $@
	 @echo CODE	PRELOAD MOVEABLE DISCARDABLE >> $@
	 @echo DATA	MOVABLE SINGLE >> $@
	 @echo HEAPSIZE	8192 >> $@
	 @echo SEGMENTS	'WEP_TEXT' FIXED PRELOAD >> $@
	 @echo EXPORTS	>> $@
	 @echo WEP	>> $@
	 @echo CVPubFun is adding export definitions ...
	 @CVPubFun $@ /v /a /o /s /N @<<
$(OBJECTS: = ^
)
<<
	 @CVPubFun $@ /v /a /o6000 /s /N /ed $(CV_OBJ)\styles.obj $(CV_OBJ)\rcdef.obj $(CV_OBJ)\globals2.obj $(MSOBJECTS)
	 @echo CVPubFun is done.

!else
!	if !exist ($(DATA_GUI_DEF_FILE))
!		MESSAGE *****
!		MESSAGE This makefile requires a module definition file $(DATA_GUI_DEF_FILE)
!		MESSAGE Restart with MAKEDEF=1 on the NMAKE command line.
!		MESSAGE *****
!		ERROR Please restart build!
!	endif
!endif
!else

$(DATA_GUI_DEF_FILE):	$(OBJECTS)
!if $(MAKEDEF)
	 @if exist $@ echo deleting existing module definition file $@...
	 @if exist $@ del $@
	 @echo creating module definition file $@...
	 @echo LIBRARY $(DATA_GUI_DLL) >> $@
	 @echo DESCRIPTION 'Allegris Foundation 1.0.00 for Microsoft Windows DLL for Win32 and Microsoft-C++ 4.2' >> $@
	 @echo EXPORTS >> $@
	 @echo CVCoff is adding export definitions ...
	 @CVCoff $@ /v /a /o /N @<<
$(OBJECTS: = ^
)
<<
	 @CVCoff $@ /v /a /o6000 /N /ed $(CV_OBJ)\styles.obj $(CV_OBJ)\rcdef.obj $(CV_OBJ)\globals2.obj
	 @echo CVCoff is done.

!else
!	if !exist ($(DATA_GUI_DEF_FILE))
!		MESSAGE *****
!		MESSAGE This makefile requires a module definition file $(DATA_GUI_DEF_FILE)
!		MESSAGE Restart with MAKEDEF=1 on the NMAKE command line.
!		MESSAGE *****
!		ERROR Please restart build!
!	endif
!endif
!endif

!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

!if $(CV_DLL)

!if $(CV_TARGET)==16
vmdll.lib:		vm16dll.lib
!else
vmdll.lib:		vm32dll.lib
!endif

vm16dll.lib:	winstart.obj globals.obj clsimp.obj
				cl -c -DUSE_DLL $(DEFINES) $(COMPILER_FLAGS) winstart.cpp
				cl -c -DUSE_DLL $(DEFINES) $(COMPILER_FLAGS) globals.cpp
				cl -c -DUSE_DLL $(DEFINES) $(COMPILER_FLAGS) clsimp.cpp
				$(ARCHIVE) vm16dll.lib $(ARCHIVER_FLAGS) \
					winstart.obj globals.obj clsimp.obj ;
vm32dll.lib:	winstart.obj globals.obj clsimp.obj
				cl -c -DUSE_DLL $(DEFINES) $(COMPILER_FLAGS) winstart.cpp
				cl -c -DUSE_DLL $(DEFINES) $(COMPILER_FLAGS) globals.cpp
				cl -c -DUSE_DLL $(DEFINES) $(COMPILER_FLAGS) clsimp.cpp
				$(ARCHIVER) /OUT:vm32dll.lib $(ARCHIVER_FLAGS) \
					winstart.obj globals.obj clsimp.obj
!endif

# --------------------------------------------------------------------------
# Special build for cmpitem.cpp
# needs to be compiled with exception handling turned on for Allegris
#
!if "$(ALLEGRIS_CONSTRUCTOR)"!=""
$(CV_OBJ)\cmpitem.obj : $(CV_SRC)\cmpitem.cpp
	$(CC) $(CFLAGS) -GX /DALLEGRIS_CONSTRUCTOR -Fo$@ $(CV_SRC)\cmpitem.cpp
!endif
# --------------------------------------------------------------------------

# --------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------
!include "vdepend.inc"
