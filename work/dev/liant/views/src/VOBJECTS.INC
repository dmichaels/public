# $Header:   Q:/views/common/vcs/vobjects.inv   1.32   Mar 25 1997 15:50:04   gregm  $
#
# --------------------------------------------------------------------------
#   vobjects.inc
#   
#   object module list for makefiles
#
#   Allegris Foundation 1.1.00
#   Copyright (c) 1997 by INTERSOLV, Inc.
#   +-----------------------------------------------------------------+
#   | This product is the property of INTERSOLV, Inc. and is licensed |
#   | pursuant to a written license agreement.  No portion of  this   |
#   | product may be reproduced without the written permission of     |
#   | INTERSOLV, Inc. except pursuant to the license agreement.       |
#   +-----------------------------------------------------------------+
#
#   Revision History:
#   -----------------
#   08/29/96 glm    Arranged for Allegris
# --------------------------------------------------------------------------

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
# GUI Objects
# --------------------------------------------------------------------------

!if $(CV_DLL)
winstart_or_null=
!else
winstart_or_null=$(CV_OBJ)\winstart.obj
!endif

!if "$(CV_TARGET)"=="16"
cvthread_or_null=
mutex_or_null=
semaphor_or_null=
!else
cvthread_or_null=$(CV_OBJ)\cvthread.obj
mutex_or_null=$(CV_OBJ)\mutex.obj
semaphor_or_null=$(CV_OBJ)\semaphor.obj
!endif

# --------------------------------------------------------------------------

GUI_OBJECTS1=\
	$(CV_OBJ)\about.obj\
	$(CV_OBJ)\accel.obj\
	$(CV_OBJ)\addrmvls.obj\
	$(CV_OBJ)\appview.obj\
	$(CV_OBJ)\arc.obj\
	$(CV_OBJ)\atom.obj\
	$(CV_OBJ)\attach.obj\
	$(CV_OBJ)\bitbltr.obj\
	$(CV_OBJ)\bitmap.obj\
	$(CV_OBJ)\bitmapvw.obj\
	$(CV_OBJ)\bmapbttn.obj\
	$(CV_OBJ)\brush.obj\
	$(CV_OBJ)\brushinf.obj\
	$(CV_OBJ)\bttngrp.obj\
	$(CV_OBJ)\button.obj\
	$(CV_OBJ)\callback.obj\
	$(CV_OBJ)\checkbox.obj\
	$(CV_OBJ)\clipbord.obj\
	$(CV_OBJ)\cllbckls.obj\
	$(CV_OBJ)\clrscm.obj\
	$(CV_OBJ)\cntrlhlp.obj\
	$(CV_OBJ)\cntrlvw.obj\
	$(CV_OBJ)\color.obj\
	$(CV_OBJ)\combobox.obj\
	$(CV_OBJ)\control.obj\
	$(CV_OBJ)\ddeclnt.obj\
	$(CV_OBJ)\ddecls.obj\
	$(CV_OBJ)\ddeserv.obj\
	$(CV_OBJ)\dialog.obj\
	$(CV_OBJ)\display.obj\
	$(CV_OBJ)\dndimp.obj\
	$(CV_OBJ)\dragsite.obj\
	$(CV_OBJ)\dropsite.obj\
	$(CV_OBJ)\docksite.obj\
	$(CV_OBJ)\docktool.obj\
	$(CV_OBJ)\dockwin.obj\
	$(CV_OBJ)\drag.obj\
	$(CV_OBJ)\dragmgr.obj\
	$(CV_OBJ)\editbox.obj\
	$(CV_OBJ)\editline.obj\
	$(CV_OBJ)\editnot.obj\
	$(CV_OBJ)\ellipse.obj\
	$(CV_OBJ)\event.obj\
	$(CV_OBJ)\eventmgr.obj\
	$(CV_OBJ)\exclgrp.obj

GUI_OBJECTS2=\
	$(CV_OBJ)\fileslct.obj\
	$(CV_OBJ)\font.obj\
	$(CV_OBJ)\fontinfo.obj\
	$(CV_OBJ)\fontslct.obj\
	$(CV_OBJ)\frame.obj\
	$(CV_OBJ)\geocol.obj\
	$(CV_OBJ)\geoform.obj\
	$(CV_OBJ)\geoframe.obj\
	$(CV_OBJ)\geomgr.obj\
	$(CV_OBJ)\georoc.obj\
	$(CV_OBJ)\georow.obj\
	$(CV_OBJ)\geowin.obj\
	$(CV_OBJ)\group.obj\
	$(CV_OBJ)\groupbox.obj\
	$(CV_OBJ)\help.obj\
	$(CV_OBJ)\icon.obj\
	$(CV_OBJ)\iconbttn.obj\
	$(CV_OBJ)\image.obj\
	$(CV_OBJ)\imanim.obj\
	$(CV_OBJ)\imbox.obj\
	$(CV_OBJ)\imbutton.obj\
	$(CV_OBJ)\imouse.obj\
	$(CV_OBJ)\inclgrp.obj\
	$(CV_OBJ)\input.obj\
	$(CV_OBJ)\listbox.obj\
	$(CV_OBJ)\listslct.obj\
	$(CV_OBJ)\mdiappvw.obj\
	$(CV_OBJ)\mdiclnt.obj\
	$(CV_OBJ)\mdiview.obj\
	$(CV_OBJ)\menu.obj\
	$(CV_OBJ)\menuitem.obj\
	$(CV_OBJ)\mouscrsr.obj\
	$(CV_OBJ)\multlist.obj\
	$(CV_OBJ)\multslct.obj\
	$(CV_OBJ)\notebook.obj\
	$(CV_OBJ)\notepage.obj\
	$(CV_OBJ)\notifier.obj\
	$(CV_OBJ)\objcour.obj\
	$(CV_OBJ)\objtype.obj\
	$(CV_OBJ)\panner.obj\
	$(CV_OBJ)\pen.obj\
	$(CV_OBJ)\peninfo.obj\
	$(CV_OBJ)\picbttn.obj\
	$(CV_OBJ)\polygon.obj\
	$(CV_OBJ)\poppwndw.obj\
	$(CV_OBJ)\popupmen.obj\
	$(CV_OBJ)\port.obj\
	$(CV_OBJ)\printab.obj\
	$(CV_OBJ)\printdlg.obj\
	$(CV_OBJ)\printer.obj\
	$(CV_OBJ)\pushbttn.obj

GUI_OBJECTS3=\
	$(CV_OBJ)\radibttn.obj\
	$(CV_OBJ)\ratio.obj\
	$(CV_OBJ)\rcdef.obj\
	$(CV_OBJ)\rect.obj\
	$(CV_OBJ)\region.obj\
	$(CV_OBJ)\report.obj\
	$(CV_OBJ)\rndrect.obj\
	$(CV_OBJ)\rsc.obj\
	$(CV_OBJ)\sash.obj\
	$(CV_OBJ)\savedlg.obj\
	$(CV_OBJ)\scrollbr.obj\
	$(CV_OBJ)\scrollw.obj\
	$(CV_OBJ)\scrollwx.obj\
	$(CV_OBJ)\shade.obj\
	$(CV_OBJ)\shdctrl.obj\
	$(CV_OBJ)\statusbr.obj\
	$(CV_OBJ)\stdappvw.obj\
	$(CV_OBJ)\stdmdiap.obj\
	$(CV_OBJ)\stdmdivw.obj\
	$(CV_OBJ)\sysmenu.obj\
	$(CV_OBJ)\tablemdl.obj\
	$(CV_OBJ)\tablesub.obj\
	$(CV_OBJ)\tablevw.obj\
	$(CV_OBJ)\textbox.obj\
	$(CV_OBJ)\textedtr.obj\
	$(CV_OBJ)\timer.obj\
	$(CV_OBJ)\toolbar.obj\
	$(CV_OBJ)\toolbox.obj\
	$(CV_OBJ)\transwin.obj\
	$(CV_OBJ)\treenode.obj\
	$(CV_OBJ)\tristate.obj\
	$(CV_OBJ)\userctrl.obj\
	$(CV_OBJ)\utilwin.obj\
	$(CV_OBJ)\view.obj\
	$(CV_OBJ)\vlistbox.obj\
	$(CV_OBJ)\vtlistbx.obj\
	$(CV_OBJ)\vtree.obj\
	$(CV_OBJ)\window.obj\
	$(CV_OBJ)\wininfo.obj\
	$(winstart_or_null)\
	$(CV_OBJ)\winstyle.obj\
	$(CV_OBJ)\workbook.obj\
	$(CV_OBJ)\workpage.obj\
	$(CV_OBJ)\yesno.obj

DEV_GUI_OBJECTS=

!if "$(CV_TARGET)"=="PM"
PLATFORM_OBJECTS=\
	$(CV_OBJ)\ctwin.obj
!else
PLATFORM_OBJECTS=\
	$(CV_OBJ)\colrslct.obj

!endif


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
GUI_OBJECTS=\
	$(GUI_OBJECTS1)\
	$(GUI_OBJECTS2)\
	$(GUI_OBJECTS3)

ALL_GUI_OBJECTS=\
	$(GUI_OBJECTS1)\
	$(GUI_OBJECTS2)\
	$(GUI_OBJECTS3)\
	$(PLATFORM_OBJECTS)\
	$(COMPONENT_OBJECTS)\
	$(DEV_GUI_OBJECTS)

# --------------------------------------------------------------------------
# Data Objects
# --------------------------------------------------------------------------

!if $(CV_DLL)
globals_or_null=
!else
globals_or_null=\
	$(CV_OBJ)\globals2.obj\
	$(CV_OBJ)\globals.obj
!endif

# --------------------------------------------------------------------------

!if "$(CV_TARGET)"=="PM"
platform_memry=\
	$(CV_OBJ)\memry.obj
!endif
!if "$(CV_TARGET)"=="16"
platform_memry=\
	$(CV_OBJ)\memry16.obj
!endif
!if "$(CV_TARGET)"=="32"
platform_memry=\
	$(CV_OBJ)\memry32.obj
!endif

DATA_OBJECTS=\
	$(CV_OBJ)\archiver.obj\
	$(CV_OBJ)\archvrtg.obj\
	$(CV_OBJ)\assoc.obj\
	$(CV_OBJ)\bfdir.obj\
	$(CV_OBJ)\bfdiritm.obj\
	$(CV_OBJ)\bitset.obj\
	$(CV_OBJ)\blockfil.obj\
	$(CV_OBJ)\bool.obj\
	$(CV_OBJ)\class.obj\
	$(CV_OBJ)\classtbl.obj\
	$(CV_OBJ)\collectn.obj\
	$(CV_OBJ)\containr.obj\
	$(CV_OBJ)\cvassert.obj\
	$(cvthread_or_null)\
	$(CV_OBJ)\date.obj\
	$(CV_OBJ)\datetime.obj\
	$(CV_OBJ)\dictinry.obj\
	$(CV_OBJ)\exclstyl.obj\
	$(CV_OBJ)\file.obj\
	$(CV_OBJ)\global.obj\
	$(globals_or_null)\
	$(CV_OBJ)\implstyl.obj\
	$(CV_OBJ)\intassoc.obj\
	$(CV_OBJ)\intlcbtb.obj\
	$(CV_OBJ)\intntltb.obj\
	$(CV_OBJ)\intset.obj\
	$(CV_OBJ)\iterator.obj\
	$(CV_OBJ)\location.obj\
	$(platform_memry)\
	$(CV_OBJ)\multiset.obj\
	$(mutex_or_null)\
	$(CV_OBJ)\objarray.obj\
	$(CV_OBJ)\object.obj\
	$(CV_OBJ)\ordcllct.obj\
	$(CV_OBJ)\pathstr.obj\
	$(CV_OBJ)\pointrry.obj\
	$(CV_OBJ)\rband.obj\
	$(CV_OBJ)\result.obj\
	$(CV_OBJ)\rscarch.obj\
	$(semaphor_or_null)\
	$(CV_OBJ)\set.obj\
	$(CV_OBJ)\stack.obj\
	$(CV_OBJ)\str.obj\
	$(CV_OBJ)\strm.obj\
	$(CV_OBJ)\strtable.obj\
	$(CV_OBJ)\style.obj\
	$(CV_OBJ)\styles.obj\
	$(CV_OBJ)\stylexpr.obj\
	$(CV_OBJ)\tagstrm.obj\
	$(CV_OBJ)\thyme.obj\
	$(CV_OBJ)\timezone.obj\
	$(CV_OBJ)\tofmstrm.obj\
	$(CV_OBJ)\tokens.obj\
	$(CV_OBJ)\tokstrm.obj\
	$(CV_OBJ)\tzus.obj\
	$(CV_OBJ)\vlocale.obj\
	$(CV_OBJ)\vwchar.obj

DEV_DATA_OBJECTS=

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ALL_DATA_OBJECTS=\
	$(DATA_OBJECTS)\
	$(DEV_DATA_OBJECTS)

# --------------------------------------------------------------------------
# Compound Document Objects
# --------------------------------------------------------------------------
!if "$(CV_NOOLE)"!="0"
COMPONENT_OBJECTS=
!else
COMPONENT_OBJECTS=\
	$(CV_OBJ)\advsink.obj\
	$(CV_OBJ)\archbyte.obj\
	$(CV_OBJ)\array.obj\
	$(CV_OBJ)\cmpappvw.obj\
	$(CV_OBJ)\cmpcntl.obj\
	$(CV_OBJ)\cmpitem.obj\
	$(CV_OBJ)\cmpobj.obj\
	$(CV_OBJ)\cmpprog.obj\
	$(CV_OBJ)\cmpsite.obj\
	$(CV_OBJ)\cmpslct.obj\
	$(CV_OBJ)\cmpstrg.obj\
	$(CV_OBJ)\cmpview.obj\
	$(CV_OBJ)\cmpwin.obj\
	$(CV_OBJ)\debug.obj\
	$(CV_OBJ)\dropsrc.obj\
	$(CV_OBJ)\droptarg.obj\
	$(CV_OBJ)\itemwin.obj\
	$(CV_OBJ)\lockbyte.obj\
	$(CV_OBJ)\methdata.obj\
	$(CV_OBJ)\money.obj\
	$(CV_OBJ)\oclsite.obj\
	$(CV_OBJ)\oipappvw.obj\
	$(CV_OBJ)\oipframe.obj\
	$(CV_OBJ)\oipsite.obj\
	$(CV_OBJ)\oipuiwin.obj\
	$(CV_OBJ)\olehelp.obj\
	$(CV_OBJ)\olerslt.obj\
	$(CV_OBJ)\oobject.obj\
	$(CV_OBJ)\opoint.obj\
	$(CV_OBJ)\owindow.obj\
	$(CV_OBJ)\rootstrg.obj\
	$(CV_OBJ)\rstorag.obj\
	$(CV_OBJ)\status.obj\
	$(CV_OBJ)\storag.obj\
	$(CV_OBJ)\substrg.obj\
	$(CV_OBJ)\unknown.obj\
	$(CV_OBJ)\variable.obj\
	$(CV_OBJ)\voleui.obj\
	$(CV_OBJ)\winrect.obj\
	$(CV_OBJ)\xferagnt.obj
!endif

# --------------------------------------------------------------------------
# ALL Objects
# --------------------------------------------------------------------------

ALL_OBJECTS=\
	$(ALL_DATA_OBJECTS)\
	$(ALL_GUI_OBJECTS)

OBJECTS=\
	$(ALL_OBJECTS)

# --------------------------------------------------------------------------
# For Borland 16 bit DLLs
# --------------------------------------------------------------------------

DLL1_OBJECTS=\
	$(GUI_OBJECTS1)

DLL2_OBJECTS=\
	$(GUI_OBJECTS2)

DLL3_OBJECTS=\
	$(GUI_OBJECTS3) $(PLATFORM_OBJECTS)

DLL4_OBJECTS=\
	$(ALL_DATA_OBJECTS)

DLL5_OBJECTS=\
	$(COMPONENT_OBJECTS)

# --------------------------------------------------------------------------
