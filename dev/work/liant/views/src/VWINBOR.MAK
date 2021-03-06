# $Header:   Q:/views/win/vcs/vwinbor.mav   1.38   Mar 26 1997 18:58:22   gregm  $
#
# --------------------------------------------------------------------------
#	vwinbor.mak
#
#	Borland C++ 5.0 for Windows library makefile
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
#	08/09/95 dgm	Original.
#	11/29/95 glm	changed name, default: 32-bit DLL
#	12/05/95 glm	default: CV_DEBUG, CV_ASSERT
#	12/06/95 dss	Added globals2.obj.
#	12/06/95 dss	change vole2ui.lib voleui32.lib.
#	12/12/95 glm	modified targets, MAKEDEF=0, no MT on static build
#					Removed aliases	STATIC, DLL, WIN32,WIN16
#					to avoid possible conflicts.
#	12/14/95 glm	removed check for OLE, CV_NOOLE always set for 
#					Beta-2
#	02/09/96 glm	16-bit fast-huge pointer flag
#					changed page sizes for debug, no-debug
#					build everything with DLL version of RTL
#					16-bit use smart callbacks
#	02/14/96 glm	added CV_SRC,CV_OBJ, TARGET_NAME
#	02/19/96 glm	added needed targets for 16-bit DLL
#	02/22/96 glm	added flag for 16-bit DLL
#	02/25/96 dss	changed OLE_LIBS to be compatible w/borland.
#					Turned on CV_NOOLE.
#	02/27/96 glm	create third 16-bit DLL, fixed page size for
#					non-debug OLE
#	04/03/96 glm	five dll 16-bit build, no rtldll for static lib
#	06/03/96 glm	updated for B5 ole, preened
#	09/14/96 glm	independent CV_DEBUG, CV_ASSERT, CV_SYMBOLS
#					preened, added CV_OUT, CV_USER
#	11/20/96 glm	link flags in temp file, -tWS for 16 static
#	03/20/97 glm	added Allegris Constructor settings
# --------------------------------------------------------------------------

# --------------------------------------------------------------------------
# Using this makefile, build libraries, or individual objects.
# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# To build individual object files use:
#
#	make [make_options] -f vwinbor.mak [v_defs] [object_files...]
#
# V_DEFS:
#	-DCV_TARGET=16		 --> for 16-bit build	 (Default: CV_TARGET=32)
#	-DCV_DLL=0			 --> for static build	 (Default: CV_DLL=1)
#	-DCV_SYMBOLS=0		 --> 0= no Debug symbols (Default: CV_DEBUG=1)
#	-DCV_DEBUG=0 		 --> 0= CV_Debug off	 (Default: CV_DEBUG=1)
#	-DCV_ASSERT=0		 --> 0= CV_Assert off	 (Default: CV_DEBUG=1)
#	-DCV_CTL3D=0 		 --> 0= no ctl3d	 	 (Default: CV_CTL3D=1)
#	-DCV_NOOLE=0 		 --> 0= OLE on			 (Default: CV_NOOLE=1)
#	-DCV_NOOLEDRAGDROP=0 --> 0= Drag-and-Drop on (Default: CV_NOOLEDRAGDROP=0)
#	-DMAKEDEF=0 		 --> 0= no new .DEF file (Default: MAKEDEF=1)
#
# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# To build a library target:
#
#	make [make_options] -f vwinbor.mak [v_defs] [target]
#
# TARGET:
#	dll32: vb32.dll vb32i.lib (Borland 32-bit DLL and import library)
#	dll16: vb16x.dll vb16xi.lib (Borland 16-bit DLL and import library)
#
#	lib32: vb32d.lib vb32g.lib (Borland 32-bit GUI and data libraries)
#	lib16: vb16d.lib vb16g.lib (Borland 16-bit GUI and data libraries)
#
#	data32:	vb32d.lib (Borland 32-bit data library)
#	gui32:	vb32g.lib (Borland 32-bit GUI library)
#	data16:	vb16d.lib (Borland 16-bit data library)
#	gui16:	vb16g.lib (Borland 16-bit GUI library)
#
#	If V_DEFS CV_TARGET or CV_DLL are specified, the appropriate 
#	library/dll will be constructed without specifying a target.
#	Specifying a target will force use of that target's CV_TARGET
#	and CV_DLL setting
#
# --------------------------------------------------------------------------

# --------------------------------------------------------------------------
# Allegris Constructor specif settings
#
!if "$(ALLEGRIS_CONSTRUCTOR)"!=""
CV_SRC=..
CV_OBJ=dllobj
DLL_IMPLIB_SUFFIX=i
CV_USER=-a4
CV_NOOLE=0
!if "$(TARGET_NAME)"==""
TARGET_NAME=vb32b50
!endif
!if "$(ALLEGRIS_CONSTRUCTOR)"=="nodebug"
CV_OPT=-O1
#
# O1	optimize for size
# includes the following: 
#   -O	Jump optimizations
#   -Ob	Dead code elimination
#   -Oc	Duplicate expressions
#   -Oe	Register allocation and live range analysis
#   -Ol	Loop optimizations
#   -OS	Instruction scheduling
#   -Z	Register load suppression
#
CV_SYMBOLS=0
!endif
!endif
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
!if !$(CV_TARGET)
CV_TARGET				= 32
!endif
!if "$(CV_DLL)"!="0"
CV_DLL					= 1
!endif
!if "$(CV_THREADSAFE)"!="0" && $(CV_TARGET)==32 && $(CV_DLL)
CV_THREADSAFE			= 1
!else
CV_THREADSAFE           = 0
!endif
!if "$(CV_DEBUG)"!="0"
CV_DEBUG				= 1
!endif
!if "$(CV_ASSERT)"!="0"
CV_ASSERT				= 1
!endif
!if "$(CV_SYMBOLS)"!="0"
CV_SYMBOLS				= 1
!endif
!if "$(CV_NOOLE)"!="0"
CV_NOOLE				= 1
!endif
!if "$(CV_NOOLEDRAGDROP)"!="0"
CV_NOOLEDRAGDROP		= 0
!endif
!if "$(CV_CTL3D)"!="0"
CV_CTL3D				= 1
!endif
!if "$(MAKEDEF)"!="0" && $(CV_DLL)
MAKEDEF					= 1
!else
MAKEDEF					= 0
!endif
!if !$(DLLENTRY) && $(CV_TARGET)==32 && $(CV_DLL)==1
DLLENTRY				= @12
!endif

# --------------------------------------------------------------------------
# Compiler macro definitions
# --------------------------------------------------------------------------
!if $(CV_TARGET)==16
SYSTEM_DEFINES			= -DCV_WIN16
!else
SYSTEM_DEFINES			= -DCV_WIN32 -DCV_IMOUSE
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# set flags for compile line defines
#
!if $(CV_DLL)
DLL_DEFINES				= -DIN_DLL
!endif
!if $(CV_THREADSAFE)
THREAD_FLAG 			= -DCV_THREADSAFE
!endif
!if $(CV_DEBUG)
CV_DEBUG_FLAG			= -DCV_DEBUG
!endif
!if $(CV_ASSERT)
CV_ASSERT_FLAG			= -DCV_ASSERT
!endif

!if $(CV_CTL3D)
3D_FLAG 				= -DCV_CTL3D
!if $(CV_TARGET)==16
3D_LIB					= ctl3dv2.lib
!else
3D_LIB					= ctl3d32.lib
!endif
!else
3D_LIB					=
!endif

!if $(CV_NOOLE)
OLE_FLAG				= -DCV_NOOLE
OLE_LIBS				=
!else
OLE_FLAG				= -VF
!if $(CV_TARGET)==16
OLE_LIBS				=\
						  ole2w16.lib\
						  boleui.lib
!else
OLE_LIBS				=\
						  ole2w32.lib\
						  boleui32.lib\
						  vbolui32.lib
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
root_name				= $(TARGET_NAME)
!else
!if $(CV_TARGET)==16
root_name				= vb16
!else
root_name				= vb32
!endif
!endif

DATA_LIBRARY			= $(root_name)d
DATA_LIBRARY_FILE		= $(CV_OUT)\$(DATA_LIBRARY).lib

GUI_LIBRARY				= $(root_name)g
GUI_LIBRARY_FILE		= $(CV_OUT)\$(GUI_LIBRARY).lib

!if $(CV_TARGET)==32
DATA_GUI_DLL            = $(root_name)
DATA_GUI_DLL_FILE       = $(CV_OUT)\$(DATA_GUI_DLL).dll
DLL_IMPLIB_FILE			= $(CV_OUT)\$(DATA_GUI_DLL)$(DLL_IMPLIB_SUFFIX).lib
DATA_GUI_DEF_FILE		= $(CV_OUT)\$(DATA_GUI_DLL).def

!else


DATA_GUI_DLL1			= $(root_name)1
DATA_GUI_DLL_FILE1		= $(CV_OUT)\$(DATA_GUI_DLL1).dll
DLL_IMPLIB_FILE1		= $(CV_OUT)\$(DATA_GUI_DLL1)$(DLL_IMPLIB_SUFFIX).lib
DATA_GUI_DEF_FILE1		= $(CV_OUT)\$(DATA_GUI_DLL1).def

DATA_GUI_DLL2			= $(root_name)2
DATA_GUI_DLL_FILE2		= $(CV_OUT)\$(DATA_GUI_DLL2).dll
DLL_IMPLIB_FILE2		= $(CV_OUT)\$(DATA_GUI_DLL2)$(DLL_IMPLIB_SUFFIX).lib
DATA_GUI_DEF_FILE2		= $(CV_OUT)\$(DATA_GUI_DLL2).def

DATA_GUI_DLL3			= $(root_name)3
DATA_GUI_DLL_FILE3		= $(CV_OUT)\$(DATA_GUI_DLL3).dll
DLL_IMPLIB_FILE3		= $(CV_OUT)\$(DATA_GUI_DLL3)$(DLL_IMPLIB_SUFFIX).lib
DATA_GUI_DEF_FILE3		= $(CV_OUT)\$(DATA_GUI_DLL3).def

DATA_GUI_DLL4			= $(root_name)4
DATA_GUI_DLL_FILE4		= $(CV_OUT)\$(DATA_GUI_DLL4).dll
DLL_IMPLIB_FILE4		= $(CV_OUT)\$(DATA_GUI_DLL4)$(DLL_IMPLIB_SUFFIX).lib
DATA_GUI_DEF_FILE4		= $(CV_OUT)\$(DATA_GUI_DLL4).def

DATA_GUI_DLL5			= $(root_name)5
DATA_GUI_DLL_FILE5		= $(CV_OUT)\$(DATA_GUI_DLL5).dll
DLL_IMPLIB_FILE5		= $(CV_OUT)\$(DATA_GUI_DLL5)$(DLL_IMPLIB_SUFFIX).lib
DATA_GUI_DEF_FILE5		= $(CV_OUT)\$(DATA_GUI_DLL5).def
!endif

# --------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------

INCLUDES				= -I$(INCLUDE)

# --------------------------------------------------------------------------
# Object files
# --------------------------------------------------------------------------

!include "vobjects.inc"

# --------------------------------------------------------------------------
# Rules
# --------------------------------------------------------------------------

.SUFFIXES : .o .obj .c .cxx .cpp .C
{$(CV_SRC)}.c{$(CV_OBJ)}.o:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
{$(CV_SRC)}.cxx{$(CV_OBJ)}.o:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
{$(CV_SRC)}.cpp{$(CV_OBJ)}.o:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
{$(CV_SRC)}.C{$(CV_OBJ)}.o:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
{$(CV_SRC)}.c{$(CV_OBJ)}.obj:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
{$(CV_SRC)}.cxx{$(CV_OBJ)}.obj:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
{$(CV_SRC)}.cpp{$(CV_OBJ)}.obj:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
{$(CV_SRC)}.C{$(CV_OBJ)}.obj:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!

.c.o:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
.cxx.o:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
.cpp.o:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
.C.o:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
.c.obj:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
.cxx.obj:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
.cpp.obj:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!
.C.obj:
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
-o$@ $<
!

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

cv32.res:
						brcc32 -dCV_RC_COMPILE -dCV_WIN32 cv.rc
						rename cv.res cv32.res
cv16.res:
						brcc -dCV_RC_COMPILE -dCV_WIN16 cv.rc
						rename cv.res cv16.res

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

!if $(CV_TARGET)==16
data_gui_dll:           $(DATA_GUI_DLL_FILE1)\
						$(DATA_GUI_DLL_FILE2)\
						$(DATA_GUI_DLL_FILE3)\
						$(DATA_GUI_DLL_FILE4)\
						$(DATA_GUI_DLL_FILE5)\
						clean vb16dll.lib
!else
data_gui_dll:           $(DATA_GUI_DLL_FILE) vb32dll.lib
!endif

dll:                    data_gui_dll

dll32:                  dll


dll16:
		$(MAKE) -$(MAKEFLAGS) -f vwinbor.mak -DCV_TARGET=16\
			-DCV_DEBUG=$(CV_DEBUG)\
			-DCV_ASSERT=$(CV_ASSERT)\
			-DCV_SYMBOLS=$(CV_SYMBOLS)\
			-DCV_CTL3D=$(CV_CTL3D)\
			-DCV_NOOLE=$(CV_NOOLE)\
			-DMAKEDEF=$(MAKEDEF)\
			 data_gui_dll

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

lib:					$(DATA_LIBRARY_FILE) $(GUI_LIBRARY_FILE)
data:					$(DATA_LIBRARY_FILE)
gui:					$(GUI_LIBRARY_FILE)

lib32:
		$(MAKE) -$(MAKEFLAGS) -f vwinbor.mak -DCV_DLL=0\
			-DCV_DEBUG=$(CV_DEBUG)\
			-DCV_ASSERT=$(CV_ASSERT)\
			-DCV_SYMBOLS=$(CV_SYMBOLS)\
			-DCV_CTL3D=$(CV_CTL3D)\
			-DCV_NOOLE=$(CV_NOOLE)\
			 lib

data32:
		$(MAKE) -$(MAKEFLAGS) -f vwinbor.mak -DCV_DLL=0\
			-DCV_DEBUG=$(CV_DEBUG)\
			-DCV_ASSERT=$(CV_ASSERT)\
			-DCV_SYMBOLS=$(CV_SYMBOLS)\
			-DCV_CTL3D=$(CV_CTL3D)\
			-DCV_NOOLE=$(CV_NOOLE)\
			 data

gui32:
		$(MAKE) -$(MAKEFLAGS) -f vwinbor.mak -DCV_DLL=0\
			-DCV_DEBUG=$(CV_DEBUG)\
			-DCV_ASSERT=$(CV_ASSERT)\
			-DCV_SYMBOLS=$(CV_SYMBOLS)\
			-DCV_CTL3D=$(CV_CTL3D)\
			-DCV_NOOLE=$(CV_NOOLE)\
			 gui

lib16:
		$(MAKE) -$(MAKEFLAGS) -f vwinbor.mak -DCV_DLL=0 -DCV_TARGET=16\
			-DCV_DEBUG=$(CV_DEBUG)\
			-DCV_ASSERT=$(CV_ASSERT)\
			-DCV_SYMBOLS=$(CV_SYMBOLS)\
			-DCV_CTL3D=$(CV_CTL3D)\
			-DCV_NOOLE=$(CV_NOOLE)\
			 lib

data16:
		$(MAKE) -$(MAKEFLAGS) -f vwinbor.mak -DCV_DLL=0 -DCV_TARGET=16\
			-DCV_DEBUG=$(CV_DEBUG)\
			-DCV_ASSERT=$(CV_ASSERT)\
			-DCV_SYMBOLS=$(CV_SYMBOLS)\
			-DCV_CTL3D=$(CV_CTL3D)\
			-DCV_NOOLE=$(CV_NOOLE)\
			 data

gui16:
		$(MAKE) -$(MAKEFLAGS) -f vwinbor.mak -DCV_DLL=0 -DCV_TARGET=16\
			-DCV_DEBUG=$(CV_DEBUG)\
			-DCV_ASSERT=$(CV_ASSERT)\
			-DCV_SYMBOLS=$(CV_SYMBOLS)\
			-DCV_CTL3D=$(CV_CTL3D)\
			-DCV_NOOLE=$(CV_NOOLE)\
			 gui

# --------------------------------------------------------------------------
# Compiler and options
# --------------------------------------------------------------------------

!if $(CV_TARGET)==16
COMPILER				= bcc
!else
COMPILER				= bcc32
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
!if "$(CV_OPT)"==""
CV_OPT					= -Od
!endif

!if $(CV_SYMBOLS)
## -v		include debug info
## -N		check for stack overflow
COMPILER_DEBUG_FLAGS	= -v -N $(CV_OPT)
!else
COMPILER_DEBUG_FLAGS	= $(CV_OPT)
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
COMMON_COMPILER_FLAGS	= -c

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

!if $(CV_TARGET)==16

!if $(CV_DLL)
## -ml      large memory model
## -Vf      far C++ virtual tables
## -h       fast-huge pointers
## -tWDE	create GUI DLL with explicit functions exportable and exported
## -Ff=1	create far variables automatically, set threshold to 1.
## -w-mpd	turn off warning:
## -D_RTLDLL	use DLL version of Run-Time-Library
COMPILER_FLAGS          = -ml -Vf -h -tWDE -Ff=1 -w-mpd -D_RTLDLL

!else
## -ml      large memory model
## -Vf      far C++ virtual tables
## -h       fast-huge pointers
## -tWSE	Windows Smart Callbacks explicit functions exportable
COMPILER_FLAGS          = -ml -Vf -h -tWS
        
!endif
!else

!if $(CV_DLL)
## -tWDE		creates a GUI DLL with explicit
## 				functions exportable and exported
## -tWM			compile for multithreaded (consistent with cw32mti.lib)
## -D_RTLDLL	use DLL version of Run-Time-Library
COMPILER_FLAGS			= -tWDE -tWM -D_RTLDLL

!else
## -tW	Windows Smart Callbacks explicit functions exportable
COMPILER_FLAGS			= -tW

!endif
!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

COMPILE                 =\
						  $(COMPILER)\
						  $(COMMON_COMPILER_FLAGS)\
						  $(COMPILER_FLAGS)\
						  $(COMPILER_DEBUG_FLAGS)\
						  $(DEFINES)\
						  $(INCLUDES)

# --------------------------------------------------------------------------
# linker debug options
# --------------------------------------------------------------------------

!if $(CV_SYMBOLS)
## -v			full symbolic debug information
## -s			detailed map of segemnts
!if $(CV_TARGET)==16
## -M		map with mangled public names
LINK_DEBUG_FLAGS		= -v -s -M
!else
## -Gm		write mangled public names in map file
LINK_DEBUG_FLAGS		= -v -s -Gm
!endif
!else
LINK_DEBUG_FLAGS		=
!endif

# --------------------------------------------------------------------------
# Library archiver and options
# --------------------------------------------------------------------------

ARCHIVER				= tlib
!if $(CV_SYMBOLS)
## /Pnumber			set library page size to 'number'
## /0				purge comments record
ARCHIVE_FLAGS			= /P512
!else
ARCHIVE_FLAGS			= /P64/0
!endif
ARCHIVE					= $(ARCHIVER) $(ARCHIVE_FLAGS)

# --------------------------------------------------------------------------
# DLL options
# --------------------------------------------------------------------------
!if $(CV_TARGET)==16
## -n		ignore default libraries
## -Twd		target & image = Windows, DLL
## -C		case sensitive exports and imports
## -c		case sensitive symbols
DLL_LINK				= tlink $(DLL_LINK_FLAGS)
DLL_LINK_FLAGS			= -n\
						  -Twd\
						  -C\
						  -c\
						  -L$(LIB)\
						  $(LINK_DEBUG_FLAGS)
DLL_LINK_LIBRARIES  	= import.lib\
						  crtldll.lib\
						  $(3D_LIB)\
						  $(OLE_LIBS)
DLL_STARTUP_OBJECT		= c0dl.obj
DLL_LINK_OBJECTS		= $(CV_OBJ)\globals2.obj\
                          $(CV_OBJ)\globals.obj\
                          $(CV_OBJ)\dll.obj
DLL_IMPLIB				= implib -c

!else
## -n		no default libraries
## -Tpd		target & image = PE image, DLL
## -c		case significant link
DLL_LINK				= tlink32 $(DLL_LINK_FLAGS)
DLL_LINK_FLAGS			= -n\
						  -m\
						  -Tpd\
						  -c\
						  -L$(LIB)\
						  $(LINK_DEBUG_FLAGS)
!if "$(CV_NOOLE)"!="0" && "$(CV_NOOLEDRAGDROP)"=="0"
DLL_LINK_LIBRARIES		= import32.lib\
						  cw32mti.lib\
						  $(3D_LIB)\
						  ole2w32.lib
!else
DLL_LINK_LIBRARIES		= import32.lib\
						  cw32mti.lib\
						  $(3D_LIB)\
						  $(OLE_LIBS)
!endif
DLL_STARTUP_OBJECT		= c0d32.obj
DLL_LINK_OBJECTS		= $(CV_OBJ)\globals2.obj\
                          $(CV_OBJ)\globals.obj\
                          $(CV_OBJ)\dll.obj
DLL_IMPLIB				= implib -c

!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

$(DATA_LIBRARY_FILE):	$(ALL_DATA_OBJECTS)
		if exist $(DATA_LIBRARY_FILE) del $(DATA_LIBRARY_FILE)
	$(ARCHIVE) $(DATA_LIBRARY) @&&!
+$(ALL_DATA_OBJECTS: = &^
+)
!,,
		if exist $(DATA_LIBRARY).bak del $(DATA_LIBRARY).bak

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

$(GUI_LIBRARY_FILE):	$(ALL_GUI_OBJECTS)
		if exist $(GUI_LIBRARY_FILE) del $(GUI_LIBRARY_FILE)
	$(ARCHIVE) $(GUI_LIBRARY) @&&!
+$(ALL_GUI_OBJECTS: = &^
+)
!,,
		if exist $(GUI_LIBRARY).bak del $(GUI_LIBRARY).bak

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
!if $(CV_TARGET)==16
# Because tlink is unable to support the number of exports in the library,
# we split the classes into multiple dlls.  Each dll is dependant on
# information in the other dlls.  To work around interdependancies,
# we build the following:
#	temporary import libraries from def files
#	dll(1) linked with the temporary import libraries
#		then generate an import library(1)		
#	dll(2) linked with import library(1) and other temporary
#		import libraries, then regentrate import library(2)
#	repeat the process for the remaining dlls
#---------------
TEMP2_IMPLIB	= temp2i
TEMP3_IMPLIB	= temp3i
TEMP4_IMPLIB	= temp4i
TEMP5_IMPLIB	= temp5i

#---------------
# 16-bit build first DLL
$(DATA_GUI_DLL_FILE1):  $(DLL1_OBJECTS)\
                        $(DLL_LINK_OBJECTS)\
                        $(TEMP2_IMPLIB).lib\
                        $(TEMP3_IMPLIB).lib\
                        $(TEMP4_IMPLIB).lib\
                        $(TEMP5_IMPLIB).lib\
                        $(DATA_GUI_DEF_FILE1)
	$(DLL_LINK)	@&&!
$(DLL_STARTUP_OBJECT) $(DLL_LINK_OBJECTS) $(DLL1_OBJECTS)
$(DATA_GUI_DLL_FILE1)
$(DATA_GUI_DLL1).map
$(TEMP2_IMPLIB) $(TEMP3_IMPLIB) $(TEMP4_IMPLIB) $(TEMP5_IMPLIB) $(DLL_LINK_LIBRARIES)
$(DATA_GUI_DEF_FILE1)
!
                                             
    $(DLL_IMPLIB) $(DLL_IMPLIB_FILE1) $(DATA_GUI_DLL_FILE1)
    
#---------------
# 16-bit build second DLL
$(DATA_GUI_DLL_FILE2):   $(DLL2_OBJECTS)\
                        $(DLL_LINK_OBJECTS)\
                        $(DLL_IMPLIB_FILE1)\
                        $(TEMP3_IMPLIB).lib\
                        $(TEMP4_IMPLIB).lib\
                        $(TEMP5_IMPLIB).lib\
                        $(DATA_GUI_DEF_FILE2)
	$(DLL_LINK) @&&!
$(DLL_STARTUP_OBJECT) $(DLL_LINK_OBJECTS) $(DLL2_OBJECTS)
$(DATA_GUI_DLL_FILE2)
$(DATA_GUI_DLL2).map
$(DLL_IMPLIB_FILE1) $(TEMP3_IMPLIB) $(TEMP4_IMPLIB) $(TEMP5_IMPLIB) $(DLL_LINK_LIBRARIES)
$(DATA_GUI_DEF_FILE2)
!

    $(DLL_IMPLIB) $(DLL_IMPLIB_FILE2) $(DATA_GUI_DLL_FILE2)

#---------------
# 16-bit build third DLL
$(DATA_GUI_DLL_FILE3):  $(DLL3_OBJECTS)\
                        $(DLL_LINK_OBJECTS)\
                        $(DLL_IMPLIB_FILE1)\
                        $(DLL_IMPLIB_FILE2)\
                        $(TEMP4_IMPLIB).lib\
                        $(TEMP5_IMPLIB).lib\
                        $(DATA_GUI_DEF_FILE3)
	$(DLL_LINK) @&&!
$(DLL_STARTUP_OBJECT) $(DLL_LINK_OBJECTS) $(DLL3_OBJECTS)
$(DATA_GUI_DLL_FILE3)
$(DATA_GUI_DLL3).map
$(DLL_IMPLIB_FILE1) $(DLL_IMPLIB_FILE2) $(TEMP4_IMPLIB) $(TEMP5_IMPLIB) $(DLL_LINK_LIBRARIES)
$(DATA_GUI_DEF_FILE3)
!

    $(DLL_IMPLIB) $(DLL_IMPLIB_FILE3) $(DATA_GUI_DLL_FILE3)

#---------------
# 16-bit build fourth DLL
$(DATA_GUI_DLL_FILE4):  $(DLL4_OBJECTS)\
                        $(DLL_LINK_OBJECTS)\
                        $(DLL_IMPLIB_FILE1)\
                        $(DLL_IMPLIB_FILE2)\
                        $(DLL_IMPLIB_FILE3)\
                        $(TEMP5_IMPLIB).lib\
                        $(DATA_GUI_DEF_FILE4)
	$(DLL_LINK) @&&!
$(DLL_STARTUP_OBJECT) $(DLL_LINK_OBJECTS) $(DLL4_OBJECTS)
$(DATA_GUI_DLL_FILE4)
$(DATA_GUI_DLL4).map
$(DLL_IMPLIB_FILE1) $(DLL_IMPLIB_FILE2) $(DLL_IMPLIB_FILE3) $(TEMP5_IMPLIB) $(DLL_LINK_LIBRARIES)
$(DATA_GUI_DEF_FILE4)
!

    $(DLL_IMPLIB) $(DLL_IMPLIB_FILE4) $(DATA_GUI_DLL_FILE4)

#---------------
# 16-bit build fifth DLL
$(DATA_GUI_DLL_FILE5):  $(DLL5_OBJECTS)\
                        $(DLL_LINK_OBJECTS)\
                        $(DLL_IMPLIB_FILE1)\
                        $(DLL_IMPLIB_FILE2)\
                        $(DLL_IMPLIB_FILE3)\
                        $(DLL_IMPLIB_FILE4)\
                        $(DATA_GUI_DEF_FILE5)
	$(DLL_LINK) @&&!
$(DLL_STARTUP_OBJECT) $(DLL_LINK_OBJECTS) $(DLL5_OBJECTS)
$(DATA_GUI_DLL_FILE5)
$(DATA_GUI_DLL5).map
$(DLL_IMPLIB_FILE1) $(DLL_IMPLIB_FILE2) $(DLL_IMPLIB_FILE3) $(DLL_IMPLIB_FILE4) $(DLL_LINK_LIBRARIES)
$(DATA_GUI_DEF_FILE5)
!

    $(DLL_IMPLIB) $(DLL_IMPLIB_FILE5) $(DATA_GUI_DLL_FILE5)

#---------------
# temporary import libraries from def file
#---------------
$(TEMP2_IMPLIB).lib: $(DATA_GUI_DEF_FILE2)
	$(DLL_IMPLIB) $(TEMP2_IMPLIB).lib $(DATA_GUI_DEF_FILE2)
	
$(TEMP3_IMPLIB).lib: $(DATA_GUI_DEF_FILE3)
	$(DLL_IMPLIB) $(TEMP3_IMPLIB).lib $(DATA_GUI_DEF_FILE3)
	
$(TEMP4_IMPLIB).lib: $(DATA_GUI_DEF_FILE4)
	$(DLL_IMPLIB) $(TEMP4_IMPLIB).lib $(DATA_GUI_DEF_FILE4)
	
$(TEMP5_IMPLIB).lib: $(DATA_GUI_DEF_FILE5)
	$(DLL_IMPLIB) $(TEMP5_IMPLIB).lib $(DATA_GUI_DEF_FILE5)
	

#---------------
clean:
	if exist $(TEMP2_IMPLIB).lib del $(TEMP2_IMPLIB).lib
	if exist $(TEMP3_IMPLIB).lib del $(TEMP3_IMPLIB).lib
	if exist $(TEMP4_IMPLIB).lib del $(TEMP4_IMPLIB).lib
	if exist $(TEMP5_IMPLIB).lib del $(TEMP5_IMPLIB).lib
#---------------

!else
# 32-bit build DLL
$(DATA_GUI_DLL_FILE):   $(OBJECTS)\
                        $(DLL_LINK_OBJECTS)\
                        $(DATA_GUI_DEF_FILE)
	$(DLL_LINK) @&&!
$(DLL_STARTUP_OBJECT) $(DLL_LINK_OBJECTS) $(OBJECTS)
$(DATA_GUI_DLL_FILE)
$(DATA_GUI_DLL).map
$(DLL_LINK_LIBRARIES)
$(DATA_GUI_DEF_FILE)
!
    $(DLL_IMPLIB) $(DLL_IMPLIB_FILE) $(DATA_GUI_DLL_FILE)

!endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
!if $(CV_TARGET)==16
$(DATA_GUI_DEF_FILE1):    $(DLL1_OBJECTS) $(DLL_LINK_OBJECTS)
!if $(MAKEDEF)
    @if exist $@ echo deleting existing module definition file $@...
    @if exist $@ del $@
    @echo creating module definition file $@...
    @echo LIBRARY     $(DATA_GUI_DLL1) >> $@
    @echo DESCRIPTION 'DLL1 Allegris Workshop for Win16 and Borland-C++ 5.0' >> $@
    @echo EXETYPE     WINDOWS >> $@
    @echo CODE        PRELOAD MOVEABLE DISCARDABLE >> $@
    @echo DATA        MOVABLE SINGLE >> $@
    @echo HEAPSIZE    1000 >> $@
    @echo ; Export section created by CVPubFun. >> $@
    @echo EXPORTS >> $@
    @echo CVPubFun is adding export definitions ...
    @CVPubFun $@ /v /a /o /s /ed @&&!
$(DLL1_OBJECTS)
!
    @CVPubFun $@ /v /a /o6000 /ed $(CV_OBJ)\globals2.obj
    @echo CVPubFun is done.

!else
    @echo ***
    @echo This makefile requires a module definition file $@.
    @echo Restart with -DMAKEDEF on the MAKE command line.
    @echo ***
!endif

$(DATA_GUI_DEF_FILE2):    $(DLL2_OBJECTS)
!if $(MAKEDEF)
    @if exist $@ echo deleting existing module definition file $@...
    @if exist $@ del $@
    @echo creating module definition file $@...
    @echo LIBRARY     $(DATA_GUI_DLL2) >> $@
    @echo DESCRIPTION 'DLL2 Allegris Workshop for Win16 and Borland-C++ 5.0' >> $@
    @echo EXETYPE     WINDOWS >> $@
    @echo CODE        PRELOAD MOVEABLE DISCARDABLE >> $@
    @echo DATA        MOVABLE SINGLE >> $@
    @echo HEAPSIZE    1000 >> $@
    @echo ; Export section created by CVPubFun. >> $@
    @echo EXPORTS >> $@
    @echo CVPubFun is adding export definitions ...
    @CVPubFun $@ /v /a /o /s /ed @&&!
$(DLL2_OBJECTS)
!
    @echo CVPubFun is done.

!else
    @echo ***
    @echo This makefile requires a module definition file $@.
    @echo Restart with -DMAKEDEF on the MAKE command line.
    @echo ***
!endif

$(DATA_GUI_DEF_FILE3):    $(DLL3_OBJECTS)
!if $(MAKEDEF)
    @if exist $@ echo deleting existing module definition file $@...
    @if exist $@ del $@
    @echo creating module definition file $@...
    @echo LIBRARY     $(DATA_GUI_DLL3) >> $@
    @echo DESCRIPTION 'OLE DLL Allegris Workshop for Win16 and Borland-C++ 5.0' >> $@
    @echo EXETYPE     WINDOWS >> $@
    @echo CODE        PRELOAD MOVEABLE DISCARDABLE >> $@
    @echo DATA        MOVABLE SINGLE >> $@
    @echo HEAPSIZE    1000 >> $@
    @echo ; Export section created by CVPubFun. >> $@
    @echo EXPORTS >> $@
    @echo CVPubFun is adding export definitions ...
    @CVPubFun $@ /v /a /o /s /ed @&&!
$(DLL3_OBJECTS)
!
    @echo CVPubFun is done.

!else
    @echo ***
    @echo This makefile requires a module definition file $@.
    @echo Restart with -DMAKEDEF on the MAKE command line.
    @echo ***
!endif

$(DATA_GUI_DEF_FILE4):    $(DLL4_OBJECTS)
!if $(MAKEDEF)
    @if exist $@ echo deleting existing module definition file $@...
    @if exist $@ del $@
    @echo creating module definition file $@...
    @echo LIBRARY     $(DATA_GUI_DLL4) >> $@
    @echo DESCRIPTION 'DLL4 Allegris Workshop for Win16 and Borland-C++ 5.0' >> $@
    @echo EXETYPE     WINDOWS >> $@
    @echo CODE        PRELOAD MOVEABLE DISCARDABLE >> $@
    @echo DATA        MOVABLE SINGLE >> $@
    @echo HEAPSIZE    1000 >> $@
    @echo ; Export section created by CVPubFun. >> $@
    @echo EXPORTS >> $@
    @echo CVPubFun is adding export definitions ...
    @CVPubFun $@ /v /a /o /s /ed @&&!
$(DLL4_OBJECTS)
!
    @echo CVPubFun is done.

!else
    @echo ***
    @echo This makefile requires a module definition file $@.
    @echo Restart with -DMAKEDEF on the MAKE command line.
    @echo ***
!endif

$(DATA_GUI_DEF_FILE5):    $(DLL5_OBJECTS)
!if $(MAKEDEF)
    @if exist $@ echo deleting existing module definition file $@...
    @if exist $@ del $@
    @echo creating module definition file $@...
    @echo LIBRARY     $(DATA_GUI_DLL5) >> $@
    @echo DESCRIPTION 'DLL5 Allegris Workshop for Win16 and Borland-C++ 5.0' >> $@
    @echo EXETYPE     WINDOWS >> $@
    @echo CODE        PRELOAD MOVEABLE DISCARDABLE >> $@
    @echo DATA        MOVABLE SINGLE >> $@
    @echo HEAPSIZE    1000 >> $@
    @echo ; Export section created by CVPubFun. >> $@
    @echo EXPORTS >> $@
    @echo CVPubFun is adding export definitions ...
!if "$(CV_NOOLE)"=="0"
    @CVPubFun $@ /v /a /o /s /ed @&&!
$(DLL5_OBJECTS)
!
!endif
    @echo CVPubFun is done.

!else
    @echo ***
    @echo This makefile requires a module definition file $@.
    @echo Restart with -DMAKEDEF on the MAKE command line.
    @echo ***
!endif


!else

$(DATA_GUI_DEF_FILE):    $(OBJECTS)
!if $(MAKEDEF)
    @if exist $@ echo deleting existing module definition file $@...
    @if exist $@ del $@
    @echo creating module definition file $@...
    @echo LIBRARY     $(DATA_GUI_DLL) >> $@
    @echo DESCRIPTION 'Allegris Workshop for Win32 and Borland-C++ 5.0' >> $@
    @echo EXETYPE     WINDOWS >> $@
    @echo CODE        PRELOAD DISCARDABLE >> $@
    @echo DATA        MULTIPLE >> $@
    @echo ; Export section created by CVPubFun. >> $@
    @echo EXPORTS >> $@
    @echo CVPubFun is adding export definitions ...
    @CVPubFun $@ /v /a /o @&&!
$(OBJECTS)
!
    @CVPubFun $@ /v /a /o6000 /ed $(CV_OBJ)\styles.obj $(CV_OBJ)\rcdef.obj $(CV_OBJ)\globals2.obj
    @echo CVPubFun is done.

!else
    @echo ***
    @echo This makefile requires a module definition file $@.
    @echo Restart with -DMAKEDEF on the MAKE command line.
    @echo ***
!endif
!endif

!if $(CV_DLL)

vb16dll.lib:	winstart.obj globals.obj clsimp.obj
				bcc -DUSE_DLL $(DEFINES) $(COMPILER_FLAGS) winstart.cpp
				bcc -DUSE_DLL $(DEFINES) $(COMPILER_FLAGS) globals.cpp
				bcc -DUSE_DLL $(DEFINES) $(COMPILER_FLAGS) clsimp.cpp
				tlib vb16dll.lib +winstart +globals +clsimp

vb32dll.lib:	winstart.obj globals.obj clsimp.obj
				bcc32 -DUSE_DLL $(DEFINES) winstart.cpp
				bcc32 -DUSE_DLL $(DEFINES) globals.cpp
				bcc32 -DUSE_DLL $(DEFINES) clsimp.cpp
				tlib vb32dll.lib +winstart +globals +clsimp
!endif


# --------------------------------------------------------------------------
# Special build for cmpitem.cpp
# needs to be compiled with exception handling turned on for Allegris
#
!if "$(ALLEGRIS_CONSTRUCTOR)"!=""
$(CV_OBJ)\cmpitem.obj : $(CV_SRC)\cmpitem.cpp
	rem $(COMPILER)\
		$(COMMON_COMPILER_FLAGS)\
		$(COMPILER_FLAGS)\
		$(COMPILER_DEBUG_FLAGS)\
		/x /DALLEGRIS_CONSTRUCTOR
	rem $(DEFINES)
	$(COMPILER) @&&!
$(COMMON_COMPILER_FLAGS)
$(COMPILER_FLAGS)
$(COMPILER_DEBUG_FLAGS)
$(DEFINES)
$(INCLUDES)
/x /DALLEGRIS_CONSTRUCTOR -o$@ $(CV_SRC)\cmpitem.cpp
!

!endif
# --------------------------------------------------------------------------

# --------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------
!include "vdepend.inc"
