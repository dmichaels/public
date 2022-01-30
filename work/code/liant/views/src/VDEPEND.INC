# --------------------------------------------------------------------------
#	vdepend.inc
#                         
#	dependencies for all makefiles
#
#	Allegris Foundation 1.1.00
#	Copyright (c) 1995-1997 by INTERSOLV, Inc.
#	+-----------------------------------------------------------------+
#	| This product is the property of INTERSOLV, Inc. and is licensed |
#	| pursuant to a written license agreement.  No portion of  this   |
#	| product may be reproduced without the written permission of     |
#	| INTERSOLV, Inc. except pursuant to the license agreement.       |
#	+-----------------------------------------------------------------+
#
#	Revision History:
#	-----------------
#	10/28/96 glm	created from msvc
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

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\stylexpr.cpp
DEP_STYLE=\
	.\defs.h\
	.\object.h\
	.\archiver.h\
	.\cvassert.h\
	.\bitset.h\
	.\style.h\
	.\exclstyl.h\
	.\implstyl.h\
	.\stylexpr.h\
	.\class.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\winclass.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\vwchar.h

$(INTDIR)/stylexpr.obj :  $(SOURCE)  $(DEP_STYLE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpview.cpp
DEP_CMPVI=\
	.\pre.h\
	.\fileslct.h\
	.\yesno.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\dialog.h\
	.\strm.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\poppwndw.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/cmpview.obj :  $(SOURCE)  $(DEP_CMPVI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shdctrl.cpp
DEP_SHDCT=\
	.\shdctrl.h\
	.\port.h\
	.\archiver.h\
	.\control.h\
	.\shade.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\rect.h\
	.\font.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\window.h\
	.\str.h\
	.\cvassert.h\
	.\winclass.h\
	.\defs.h\
	.\wintypes.h\
	.\color.h\
	.\object.h\
	.\clrscm.h\
	.\region.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\vwchar.h\
	.\class.h\
	.\result.h\
	.\containr.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/shdctrl.obj :  $(SOURCE)  $(DEP_SHDCT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bool.cpp
DEP_BOOL_=\
	.\bool.h\
	.\archiver.h\
	.\str.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\vwchar.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\result.h\
	.\containr.h\
	.\winclass.h

$(INTDIR)/bool.obj :  $(SOURCE)  $(DEP_BOOL_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\result.cpp
DEP_RESUL=\
	.\str.h\
	.\result.h\
	.\cvassert.h\
	.\object.h\
	.\vwchar.h\
	.\defs.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/result.obj :  $(SOURCE)  $(DEP_RESUL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\itemwin.cpp
DEP_ITEMW=\
	.\pre.h\
	.\itemwin.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\cmpitem.h\
	.\voleui.h\
	.\str.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/itemwin.obj :  $(SOURCE)  $(DEP_ITEMW) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\radibttn.cpp
DEP_RADIB=\
	.\winclass.h\
	.\radibttn.h\
	.\iterator.h\
	.\archiver.h\
	.\defs.h\
	.\checkbox.h\
	.\object.h\
	.\collectn.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\button.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\objarray.h\
	.\control.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\window.h\
	.\cvassert.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/radibttn.obj :  $(SOURCE)  $(DEP_RADIB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\globals.cpp
DEP_GLOBA=\
	.\defs.h\
	.\notifier.h\
	.\eventmgr.h\
	.\classtbl.h\
	.\memry.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\memry16.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/globals.obj :  $(SOURCE)  $(DEP_GLOBA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fontslct.cpp
DEP_FONTS=\
	.\fontslct.h\
	.\font.h\
	.\printer.h\
	.\notifier.h\
	.\dialog.h\
	.\winclass.h\
	.\object.h\
	.\ordcllct.h\
	.\display.h\
	.\str.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\window.h\
	.\poppwndw.h\
	.\class.h\
	.\objarray.h\
	.\wintypes.h\
	.\vwchar.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\callback.h\
	.\color.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\clrscm.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/fontslct.obj :  $(SOURCE)  $(DEP_FONTS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\owindow.cpp
DEP_OWIND=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/owindow.obj :  $(SOURCE)  $(DEP_OWIND) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\multiset.cpp
DEP_MULTI=\
	.\multiset.h\
	.\archiver.h\
	.\archvrtg.h\
	.\object.h\
	.\assoc.h\
	.\dictinry.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\defs.h\
	.\class.h\
	.\set.h\
	.\strm.h\
	.\pathstr.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/multiset.obj :  $(SOURCE)  $(DEP_MULTI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\group.cpp
DEP_GROUP=\
	.\winclass.h\
	.\group.h\
	.\control.h\
	.\brush.h\
	.\iterator.h\
	.\font.h\
	.\archiver.h\
	.\defs.h\
	.\ordcllct.h\
	.\window.h\
	.\str.h\
	.\cvassert.h\
	.\object.h\
	.\wintypes.h\
	.\clrscm.h\
	.\collectn.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\vwchar.h\
	.\class.h\
	.\color.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\containr.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\result.h\
	.\region.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/group.obj :  $(SOURCE)  $(DEP_GROUP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\geomgr.cpp
DEP_GEOMG=\
	.\geomgr.h\
	.\window.h\
	.\frame.h\
	.\ratio.h\
	.\rect.h\
	.\ordcllct.h\
	.\iterator.h\
	.\combobox.h\
	.\cvassert.h\
	.\archiver.h\
	.\geoframe.h\
	.\attach.h\
	.\display.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\defs.h\
	.\region.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\listbox.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\winclass.h\
	.\wintypes.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\str.h\
	.\containr.h\
	.\class.h\
	.\control.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\color.h\
	.\style.h\
	.\vwchar.h\
	.\callback.h\
	.\result.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/geomgr.obj :  $(SOURCE)  $(DEP_GEOMG) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\docktool.cpp
DEP_DOCKT=\
	.\docktool.h\
	.\toolbar.h\
	.\georoc.h\
	.\archiver.h\
	.\dockwin.h\
	.\control.h\
	.\geomgr.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\window.h\
	.\utilwin.h\
	.\popupmen.h\
	.\str.h\
	.\cvassert.h\
	.\winclass.h\
	.\geoframe.h\
	.\iterator.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\poppwndw.h\
	.\rband.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\object.h\
	.\vwchar.h\
	.\defs.h\
	.\result.h\
	.\containr.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\view.h\
	.\class.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\callback.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/docktool.obj :  $(SOURCE)  $(DEP_DOCKT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dll.cpp
DEP_DLL_C=\
	.\classtbl.h\
	.\global.h\
	.\winclass.h\
	.\object.h\
	.\defs.h\
	.\class.h\
	.\callback.h

$(INTDIR)/dll.obj :  $(SOURCE)  $(DEP_DLL_C) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpstrg.cpp
DEP_CMPST=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/cmpstrg.obj :  $(SOURCE)  $(DEP_CMPST) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\button.cpp
DEP_BUTTO=\
	.\button.h\
	.\ordcllct.h\
	.\notifier.h\
	.\archiver.h\
	.\cllbckls.h\
	.\control.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\appview.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\cvassert.h\
	.\object.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\class.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\color.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/button.obj :  $(SOURCE)  $(DEP_BUTTO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\str.cpp
DEP_STR_C=\
	.\defs.h\
	.\winclass.h\
	.\wintypes.h\
	.\cvassert.h\
	.\notifier.h\
	.\archiver.h\
	.\str.h\
	.\vwchar.h\
	.\color.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\window.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\clrscm.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\class.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\callback.h\
	.\control.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/str.obj :  $(SOURCE)  $(DEP_STR_C) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tablesub.cpp
DEP_TABLE=\
	.\tablesub.h\
	.\port.h\
	.\brush.h\
	.\font.h\
	.\notifier.h\
	.\shade.h\
	.\pen.h\
	.\view.h\
	.\tablevw.h\
	.\drag.h\
	.\display.h\
	.\bitbltr.h\
	.\rect.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\winclass.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\color.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\tablemdl.h\
	.\archiver.h\
	.\region.h\
	.\class.h\
	.\objarray.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\control.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\cvassert.h\
	.\strm.h\
	.\pathstr.h\
	.\style.h\
	.\result.h\
	.\bitset.h

$(INTDIR)/tablesub.obj :  $(SOURCE)  $(DEP_TABLE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\geoframe.cpp
DEP_GEOFR=\
	.\geoframe.h\
	.\geomgr.h\
	.\geoform.h\
	.\window.h\
	.\frame.h\
	.\ratio.h\
	.\rect.h\
	.\ordcllct.h\
	.\iterator.h\
	.\combobox.h\
	.\archiver.h\
	.\object.h\
	.\attach.h\
	.\display.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\defs.h\
	.\region.h\
	.\collectn.h\
	.\objarray.h\
	.\listbox.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\class.h\
	.\winclass.h\
	.\wintypes.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\str.h\
	.\containr.h\
	.\control.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\callback.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\vwchar.h\
	.\result.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/geoframe.obj :  $(SOURCE)  $(DEP_GEOFR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\location.cpp
DEP_LOCAT=\
	.\location.h\
	.\archiver.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/location.obj :  $(SOURCE)  $(DEP_LOCAT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\datetime.cpp
DEP_DATET=\
	.\datetime.h\
	.\timezone.h\
	.\str.h\
	.\archiver.h\
	.\object.h\
	.\date.h\
	.\thyme.h\
	.\vwchar.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\result.h\
	.\containr.h\
	.\winclass.h

$(INTDIR)/datetime.obj :  $(SOURCE)  $(DEP_DATET) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cntrlhlp.cpp
DEP_CNTRL=\
	.\cntrlhlp.h\
	.\intassoc.h\
	.\notifier.h\
	.\port.h\
	.\font.h\
	.\clrscm.h\
	.\timer.h\
	.\poppwndw.h\
	.\dictinry.h\
	.\pen.h\
	.\str.h\
	.\assoc.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\winclass.h\
	.\window.h\
	.\display.h\
	.\bitbltr.h\
	.\brush.h\
	.\object.h\
	.\ordcllct.h\
	.\view.h\
	.\wintypes.h\
	.\vwchar.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\class.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\color.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/cntrlhlp.obj :  $(SOURCE)  $(DEP_CNTRL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clrscm.cpp
DEP_CLRSC=\
	.\clrscm.h\
	.\color.h\
	.\window.h\
	.\class.h\
	.\pen.h\
	.\brush.h\
	.\winclass.h\
	.\notifier.h\
	.\object.h\
	.\defs.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\wintypes.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\ratio.h\
	.\objarray.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\style.h\
	.\cvassert.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\bitset.h\
	.\control.h

$(INTDIR)/clrscm.obj :  $(SOURCE)  $(DEP_CLRSC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bitmapvw.cpp
DEP_BITMA=\
	.\bitmapvw.h\
	.\port.h\
	.\bitmap.h\
	.\scrollbr.h\
	.\archiver.h\
	.\view.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\rect.h\
	.\font.h\
	.\location.h\
	.\control.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\set.h\
	.\str.h\
	.\sysmenu.h\
	.\window.h\
	.\object.h\
	.\winclass.h\
	.\wintypes.h\
	.\clrscm.h\
	.\region.h\
	.\cvassert.h\
	.\strm.h\
	.\pathstr.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\vwchar.h\
	.\popupmen.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\defs.h\
	.\class.h\
	.\color.h\
	.\result.h\
	.\containr.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/bitmapvw.obj :  $(SOURCE)  $(DEP_BITMA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\unknown.cpp
DEP_UNKNO=\
	.\pre.h\
	.\object.h\
	.\iterator.h\
	.\dictinry.h\
	.\intassoc.h\
	.\ordcllct.h\
	.\unknown.h\
	.\oledefs.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\class.h\
	.\collectn.h\
	.\set.h\
	.\assoc.h\
	.\objarray.h\
	.\volestd.h\
	.\vwchar.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/unknown.obj :  $(SOURCE)  $(DEP_UNKNO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stack.cpp
DEP_STACK=\
	.\stack.h\
	.\ordcllct.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\containr.h\
	.\defs.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/stack.obj :  $(SOURCE)  $(DEP_STACK) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printab.cpp
DEP_PRINT=\
	.\defs.h\
	.\textbox.h\
	.\pushbttn.h\
	.\printer.h\
	.\printab.h\
	.\notifier.h\
	.\control.h\
	.\button.h\
	.\display.h\
	.\str.h\
	.\dialog.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\cvassert.h\
	.\object.h\
	.\wintypes.h\
	.\vwchar.h\
	.\poppwndw.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\class.h\
	.\color.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\clrscm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/printab.obj :  $(SOURCE)  $(DEP_PRINT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\oobject.cpp
DEP_OOBJE=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/oobject.obj :  $(SOURCE)  $(DEP_OOBJE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\oipappvw.cpp
DEP_OIPAP=\
	.\pre.h\
	.\statusbr.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\str.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\color.h\
	.\clrscm.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/oipappvw.obj :  $(SOURCE)  $(DEP_OIPAP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\vtlistbx.cpp
DEP_VTLIS=\
	.\port.h\
	.\vtlistbx.h\
	.\vtree.h\
	.\notifier.h\
	.\scrollbr.h\
	.\bitmap.h\
	.\ordcllct.h\
	.\iterator.h\
	.\popupmen.h\
	.\clrscm.h\
	.\color.h\
	.\cvassert.h\
	.\editline.h\
	.\editnot.h\
	.\image.h\
	.\cllbckls.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\rect.h\
	.\font.h\
	.\view.h\
	.\drag.h\
	.\treenode.h\
	.\archiver.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\control.h\
	.\location.h\
	.\objarray.h\
	.\object.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\editbox.h\
	.\callback.h\
	.\wintypes.h\
	.\region.h\
	.\sysmenu.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\icon.h\
	.\assoc.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\class.h\
	.\textbox.h\
	.\strm.h\
	.\pathstr.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/vtlistbx.obj :  $(SOURCE)  $(DEP_VTLIS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\droptarg.cpp
DEP_DROPT=\
	.\pre.h\
	.\cmpitem.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\voleui.h\
	.\rect.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\region.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/droptarg.obj :  $(SOURCE)  $(DEP_DROPT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\vlistbox.cpp
DEP_VLIST=\
	.\vlistbox.h\
	.\notifier.h\
	.\port.h\
	.\brush.h\
	.\pen.h\
	.\color.h\
	.\clrscm.h\
	.\font.h\
	.\bitmap.h\
	.\timer.h\
	.\archiver.h\
	.\cllbckls.h\
	.\view.h\
	.\str.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\winclass.h\
	.\window.h\
	.\display.h\
	.\bitbltr.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\location.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\vwchar.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\control.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\cvassert.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/vlistbox.obj :  $(SOURCE)  $(DEP_VLIST) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pathstr.cpp
DEP_PATHS=\
	.\defs.h\
	.\notifier.h\
	.\file.h\
	.\iterator.h\
	.\pathstr.h\
	\MSVC20\INCLUDE\sys\types.h\
	\MSVC20\INCLUDE\sys\stat.h\
	\MSVC20\INCLUDE\sys\utime.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\strm.h\
	.\object.h\
	.\ordcllct.h\
	.\result.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\class.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\control.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/pathstr.obj :  $(SOURCE)  $(DEP_PATHS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style.cpp
DEP_STYLE_=\
	.\defs.h\
	.\object.h\
	.\archiver.h\
	.\bitset.h\
	.\style.h\
	.\exclstyl.h\
	.\implstyl.h\
	.\stylexpr.h\
	.\class.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\cvassert.h\
	.\callback.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\winclass.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\vwchar.h

$(INTDIR)/style.obj :  $(SOURCE)  $(DEP_STYLE_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\help.cpp
DEP_HELP_=\
	.\help.h\
	.\notifier.h\
	.\window.h\
	.\str.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\object.h\
	.\vwchar.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\wintypes.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\class.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\callback.h\
	.\control.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/help.obj :  $(SOURCE)  $(DEP_HELP_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\objarray.cpp
DEP_OBJAR=\
	.\objarray.h\
	.\archiver.h\
	.\containr.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\defs.h\
	.\class.h\
	.\str.h\
	.\result.h\
	.\callback.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/objarray.obj :  $(SOURCE)  $(DEP_OBJAR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\set.cpp
DEP_SET_C=\
	.\set.h\
	.\iterator.h\
	.\archiver.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\containr.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\assoc.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/set.obj :  $(SOURCE)  $(DEP_SET_C) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rsc.cpp
DEP_RSC_C=\
	.\notifier.h\
	.\iterator.h\
	.\rscarch.h\
	.\menu.h\
	.\bitmap.h\
	.\bmapbttn.h\
	.\location.h\
	.\stdappvw.h\
	.\view.h\
	.\dialog.h\
	.\stdmdiap.h\
	.\stdmdivw.h\
	.\rsc.h\
	.\popupmen.h\
	.\pathstr.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\object.h\
	.\archiver.h\
	.\blockfil.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\display.h\
	.\pushbttn.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\poppwndw.h\
	.\mdiappvw.h\
	.\statusbr.h\
	.\mdiview.h\
	.\file.h\
	.\menudata.h\
	.\menuitem.h\
	.\result.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\class.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\bfdir.h\
	.\stack.h\
	.\color.h\
	.\button.h\
	.\control.h\
	.\mdiclnt.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\strm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\bfdiritm.h\
	.\clrscm.h\
	.\cvassert.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/rsc.obj :  $(SOURCE)  $(DEP_RSC_C) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mdiview.cpp
DEP_MDIVI=\
	.\winclass.h\
	.\mdiview.h\
	.\mdiappvw.h\
	.\notifier.h\
	.\archiver.h\
	.\cmpview.h\
	.\defs.h\
	.\view.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\oledefs.h\
	.\unknown.h\
	.\cmpwin.h\
	.\oipuiwin.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\volestd.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\itemwin.h\
	.\cmpobj.h\
	.\control.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\opoint.h\
	.\cmpstrg.h\
	.\cvassert.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\location.h\
	.\storag.h\
	.\yesno.h\
	.\clrscm.h\
	.\bitset.h\
	.\rstorag.h\
	.\dialog.h\
	.\olerslt.h\
	.\poppwndw.h

$(INTDIR)/mdiview.obj :  $(SOURCE)  $(DEP_MDIVI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dragmgr.cpp
DEP_DRAGM=\
	.\defs.h\
	.\drag.h\
	.\global.h\
	.\archiver.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/dragmgr.obj :  $(SOURCE)  $(DEP_DRAGM) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cntrlvw.cpp
DEP_CNTRLV=\
	.\cntrlvw.h\
	.\control.h\
	.\cvassert.h\
	.\view.h\
	.\window.h\
	.\str.h\
	.\winclass.h\
	.\defs.h\
	.\set.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\object.h\
	.\vwchar.h\
	.\collectn.h\
	.\objarray.h\
	.\popupmen.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\class.h\
	.\containr.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\callback.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/cntrlvw.obj :  $(SOURCE)  $(DEP_CNTRLV) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\arc.cpp
DEP_ARC_C=\
	.\winclass.h\
	.\arc.h\
	.\port.h\
	.\defs.h\
	.\ellipse.h\
	.\rect.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\region.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\class.h\
	.\color.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\containr.h

$(INTDIR)/arc.obj :  $(SOURCE)  $(DEP_ARC_C) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\olehelp.cpp

$(INTDIR)/olehelp.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\file.cpp
DEP_FILE_=\
	.\defs.h\
	.\notifier.h\
	.\winclass.h\
	.\file.h\
	\MSVC20\INCLUDE\sys\types.h\
	\MSVC20\INCLUDE\sys\stat.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\strm.h\
	.\pathstr.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\result.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/file.obj :  $(SOURCE)  $(DEP_FILE_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dialog.cpp
DEP_DIALO=\
	.\object.h\
	.\accel.h\
	.\class.h\
	.\control.h\
	.\dialog.h\
	.\notifier.h\
	.\sysmenu.h\
	.\pushbttn.h\
	.\iterator.h\
	.\archiver.h\
	.\rscarch.h\
	.\callback.h\
	.\defs.h\
	.\window.h\
	.\str.h\
	.\cvassert.h\
	.\winclass.h\
	.\poppwndw.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\popupmen.h\
	.\button.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\blockfil.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\vwchar.h\
	.\view.h\
	.\menu.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\menudata.h\
	.\menuitem.h\
	.\strm.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\wintypes.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\scrollbr.h\
	.\result.h\
	.\bfdiritm.h\
	.\color.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/dialog.obj :  $(SOURCE)  $(DEP_DIALO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\date.cpp
DEP_DATE_=\
	.\date.h\
	.\str.h\
	.\defs.h\
	.\object.h\
	.\vwchar.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/date.obj :  $(SOURCE)  $(DEP_DATE_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tablevw.cpp
DEP_TABLEV=\
	.\tablevw.h\
	.\tablesub.h\
	.\notifier.h\
	.\scrollbr.h\
	.\port.h\
	.\shade.h\
	.\cvassert.h\
	.\view.h\
	.\tablemdl.h\
	.\drag.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\control.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\wintypes.h\
	.\color.h\
	.\sysmenu.h\
	.\object.h\
	.\archiver.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\clrscm.h\
	.\class.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\strm.h\
	.\pathstr.h\
	.\style.h\
	.\result.h\
	.\bitset.h

$(INTDIR)/tablevw.obj :  $(SOURCE)  $(DEP_TABLEV) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\iconbttn.cpp
DEP_ICONB=\
	.\iconbttn.h\
	.\notifier.h\
	.\port.h\
	.\rect.h\
	.\shade.h\
	.\archiver.h\
	.\pushbttn.h\
	.\icon.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\region.h\
	.\wintypes.h\
	.\color.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\button.h\
	.\object.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\assoc.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\clrscm.h\
	.\strm.h\
	.\pathstr.h\
	.\control.h\
	.\class.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\cvassert.h\
	.\callback.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/iconbttn.obj :  $(SOURCE)  $(DEP_ICONB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpprog.cpp
DEP_CMPPR=\
	.\defs.h\
	.\cvassert.h\
	.\archiver.h\
	.\cmpprog.h\
	.\cmpcntl.h\
	.\cmpsite.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\str.h\
	.\vlocale.h\
	.\methdata.h\
	.\variable.h\
	.\status.h\
	.\cmpitem.h\
	.\iterator.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\class.h\
	.\vwchar.h\
	.\oledefs.h\
	.\rect.h\
	.\window.h\
	.\cmpwin.h\
	.\advsink.h\
	.\oclsite.h\
	.\oipsite.h\
	.\dropsrc.h\
	.\oobject.h\
	.\result.h\
	.\containr.h\
	.\callback.h\
	.\volestd.h\
	.\region.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\unknown.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\itemwin.h\
	.\cmpobj.h\
	.\winclass.h\
	.\wintypes.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\opoint.h\
	.\cmpstrg.h\
	.\color.h\
	.\style.h\
	.\location.h\
	.\storag.h\
	.\yesno.h\
	.\clrscm.h\
	.\bitset.h\
	.\rstorag.h\
	.\dialog.h\
	.\olerslt.h\
	.\poppwndw.h\
	.\view.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\control.h\
	.\popupmen.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h

$(INTDIR)/cmpprog.obj :  $(SOURCE)  $(DEP_CMPPR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fontinfo.cpp
DEP_FONTI=\
	.\font.h\
	.\fontinfo.h\
	.\archiver.h\
	.\winclass.h\
	.\object.h\
	.\ordcllct.h\
	.\str.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\collectn.h\
	.\objarray.h\
	.\vwchar.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\callback.h\
	.\containr.h\
	.\result.h

$(INTDIR)/fontinfo.obj :  $(SOURCE)  $(DEP_FONTI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\geoform.cpp
DEP_GEOFO=\
	.\geoform.h\
	.\window.h\
	.\ordcllct.h\
	.\iterator.h\
	.\cvassert.h\
	.\attach.h\
	.\archiver.h\
	.\geomgr.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\defs.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\geoframe.h\
	.\winclass.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\str.h\
	.\containr.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\vwchar.h\
	.\callback.h\
	.\result.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/geoform.obj :  $(SOURCE)  $(DEP_GEOFO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\thyme.cpp
DEP_THYME=\
	.\thyme.h\
	.\str.h\
	.\defs.h\
	.\object.h\
	.\vwchar.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/thyme.obj :  $(SOURCE)  $(DEP_THYME) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\strtable.cpp
DEP_STRTA=\
	.\strtable.h\
	.\iterator.h\
	.\str.h\
	.\strm.h\
	.\file.h\
	.\archiver.h\
	.\rscarch.h\
	.\archvrtg.h\
	.\intntltb.h\
	.\object.h\
	.\collectn.h\
	.\vwchar.h\
	.\pathstr.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\blockfil.h\
	.\assoc.h\
	.\multiset.h\
	.\vlocale.h\
	.\defs.h\
	.\class.h\
	.\result.h\
	.\set.h\
	.\objarray.h\
	.\bfdir.h\
	.\stack.h\
	.\callback.h\
	.\containr.h\
	.\bfdiritm.h\
	.\winclass.h

$(INTDIR)/strtable.obj :  $(SOURCE)  $(DEP_STRTA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdmdivw.cpp
DEP_STDMD=\
	.\stdmdivw.h\
	.\stdmdiap.h\
	.\archiver.h\
	.\cmpview.h\
	.\mdiview.h\
	.\mdiappvw.h\
	.\statusbr.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\oledefs.h\
	.\unknown.h\
	.\cmpwin.h\
	.\oipuiwin.h\
	.\view.h\
	.\appview.h\
	.\mdiclnt.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\str.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\volestd.h\
	.\object.h\
	.\vwchar.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\itemwin.h\
	.\cmpobj.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\window.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\cvassert.h\
	.\winclass.h\
	.\wintypes.h\
	.\color.h\
	.\clrscm.h\
	.\result.h\
	.\containr.h\
	.\class.h\
	.\opoint.h\
	.\rect.h\
	.\cmpstrg.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\callback.h\
	.\location.h\
	.\region.h\
	.\storag.h\
	.\yesno.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\rstorag.h\
	.\dialog.h\
	.\style.h\
	.\olerslt.h\
	.\poppwndw.h\
	.\bitset.h

$(INTDIR)/stdmdivw.obj :  $(SOURCE)  $(DEP_STDMD) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bttngrp.cpp
DEP_BTTNG=\
	.\bttngrp.h\
	.\button.h\
	.\checkbox.h\
	.\ordcllct.h\
	.\iterator.h\
	.\archiver.h\
	.\classtbl.h\
	.\str.h\
	.\group.h\
	.\control.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\global.h\
	.\winclass.h\
	.\vwchar.h\
	.\window.h\
	.\cvassert.h\
	.\containr.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\result.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/bttngrp.obj :  $(SOURCE)  $(DEP_BTTNG) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\memry32.cpp
DEP_MEMRY=\
	.\defs.h\
	.\memry32.h\
	.\winclass.h\
	.\intset.h\
	.\global.h\
	.\containr.h\
	.\object.h\
	.\class.h\
	.\callback.h

$(INTDIR)/memry32.obj :  $(SOURCE)  $(DEP_MEMRY) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\view.cpp
DEP_VIEW_=\
	.\accel.h\
	.\iterator.h\
	.\notifier.h\
	.\poppwndw.h\
	.\pushbttn.h\
	.\set.h\
	.\view.h\
	.\winclass.h\
	.\cvassert.h\
	.\archiver.h\
	.\rscarch.h\
	.\callback.h\
	.\cmpview.h\
	.\object.h\
	.\collectn.h\
	.\defs.h\
	.\appview.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\button.h\
	.\objarray.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\blockfil.h\
	.\oledefs.h\
	.\unknown.h\
	.\cmpwin.h\
	.\oipuiwin.h\
	.\class.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\control.h\
	.\strm.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\volestd.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\itemwin.h\
	.\cmpobj.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\bfdiritm.h\
	.\opoint.h\
	.\cmpstrg.h\
	.\color.h\
	.\style.h\
	.\location.h\
	.\storag.h\
	.\yesno.h\
	.\clrscm.h\
	.\bitset.h\
	.\rstorag.h\
	.\dialog.h\
	.\olerslt.h

$(INTDIR)/view.obj :  $(SOURCE)  $(DEP_VIEW_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pointrry.cpp
DEP_POINT=\
	.\pointrry.h\
	.\archiver.h\
	.\containr.h\
	.\location.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\class.h\
	.\str.h\
	.\result.h\
	.\callback.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/pointrry.obj :  $(SOURCE)  $(DEP_POINT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\font.cpp
DEP_FONT_=\
	.\font.h\
	.\port.h\
	.\control.h\
	.\iterator.h\
	.\str.h\
	.\notifier.h\
	.\fontinfo.h\
	.\winclass.h\
	.\object.h\
	.\ordcllct.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\rect.h\
	.\window.h\
	.\cvassert.h\
	.\collectn.h\
	.\vwchar.h\
	.\defs.h\
	.\appview.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\class.h\
	.\objarray.h\
	.\wintypes.h\
	.\clrscm.h\
	.\region.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\callback.h\
	.\color.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/font.obj :  $(SOURCE)  $(DEP_FONT_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wininfo.cpp
DEP_WININ=\
	.\wininfo.h\
	.\defs.h

$(INTDIR)/wininfo.obj :  $(SOURCE)  $(DEP_WININ) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\variable.cpp
DEP_VARIA=\
	.\defs.h\
	.\cvassert.h\
	.\datetime.h\
	.\str.h\
	.\variable.h\
	.\money.h\
	.\cmpprog.h\
	.\status.h\
	.\array.h\
	.\dosdttm.h\
	.\object.h\
	.\date.h\
	.\thyme.h\
	.\timezone.h\
	.\vwchar.h\
	.\oledefs.h\
	.\vlocale.h\
	.\methdata.h\
	.\class.h\
	.\volestd.h\
	.\ordcllct.h\
	.\callback.h\
	.\collectn.h\
	.\objarray.h\
	.\winclass.h\
	.\containr.h

$(INTDIR)/variable.obj :  $(SOURCE)  $(DEP_VARIA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\strm.cpp
DEP_STRM_=\
	.\defs.h\
	.\strm.h\
	.\archiver.h\
	.\cvassert.h\
	.\wsscanf.h\
	.\str.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\vwchar.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\class.h\
	.\result.h\
	.\containr.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/strm.obj :  $(SOURCE)  $(DEP_STRM_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printdlg.cpp
DEP_PRINTD=\
	.\printdlg.h\
	.\printer.h\
	.\notifier.h\
	.\dialog.h\
	.\display.h\
	.\str.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\poppwndw.h\
	.\object.h\
	.\wintypes.h\
	.\vwchar.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\class.h\
	.\color.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\clrscm.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/printdlg.obj :  $(SOURCE)  $(DEP_PRINTD) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pen.cpp
DEP_PEN_C=\
	.\winclass.h\
	.\pen.h\
	.\iterator.h\
	.\clrscm.h\
	.\cvassert.h\
	.\defs.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\collectn.h\
	.\class.h\
	.\color.h\
	.\objarray.h\
	.\callback.h\
	.\containr.h

$(INTDIR)/pen.obj :  $(SOURCE)  $(DEP_PEN_C) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\toolbar.cpp
DEP_TOOLB=\
	.\toolbar.h\
	.\geoframe.h\
	.\geomgr.h\
	.\georoc.h\
	.\georow.h\
	.\geocol.h\
	.\geoform.h\
	.\ordcllct.h\
	.\iterator.h\
	.\port.h\
	.\brush.h\
	.\pen.h\
	.\color.h\
	.\clrscm.h\
	.\rect.h\
	.\archiver.h\
	.\stdmdiap.h\
	.\control.h\
	.\object.h\
	.\window.h\
	.\frame.h\
	.\attach.h\
	.\collectn.h\
	.\objarray.h\
	.\display.h\
	.\bitbltr.h\
	.\font.h\
	.\wintypes.h\
	.\defs.h\
	.\region.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\mdiappvw.h\
	.\statusbr.h\
	.\str.h\
	.\cvassert.h\
	.\winclass.h\
	.\class.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\ratio.h\
	.\containr.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\appview.h\
	.\mdiclnt.h\
	.\shade.h\
	.\textbox.h\
	.\vwchar.h\
	.\callback.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\style.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\bitset.h

$(INTDIR)/toolbar.obj :  $(SOURCE)  $(DEP_TOOLB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ellipse.cpp
DEP_ELLIP=\
	.\winclass.h\
	.\ellipse.h\
	.\port.h\
	.\defs.h\
	.\rect.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\region.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\class.h\
	.\color.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\containr.h

$(INTDIR)/ellipse.obj :  $(SOURCE)  $(DEP_ELLIP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\port.cpp
DEP_PORT_=\
	.\bitmap.h\
	.\ellipse.h\
	.\pointrry.h\
	.\port.h\
	.\rndrect.h\
	.\str.h\
	.\winclass.h\
	.\window.h\
	.\icon.h\
	.\arc.h\
	.\display.h\
	.\archiver.h\
	.\location.h\
	.\rect.h\
	.\containr.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\object.h\
	.\vwchar.h\
	.\defs.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\wintypes.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\region.h\
	.\clrscm.h\
	.\class.h\
	.\ratio.h\
	.\collectn.h\
	.\objarray.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\result.h\
	.\bitset.h

$(INTDIR)/port.obj :  $(SOURCE)  $(DEP_PORT_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\report.cpp
DEP_REPOR=\
	.\winclass.h\
	.\report.h\
	.\notifier.h\
	.\defs.h\
	.\dialog.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\poppwndw.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/report.obj :  $(SOURCE)  $(DEP_REPOR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpappvw.cpp
DEP_CMPAP=\
	.\pre.h\
	.\stdappvw.h\
	.\mdiappvw.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\appview.h\
	.\mdiclnt.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/cmpappvw.obj :  $(SOURCE)  $(DEP_CMPAP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\accel.cpp
DEP_ACCEL=\
	.\accel.h\
	.\menuitem.h\
	.\control.h\
	.\archiver.h\
	.\notifier.h\
	.\cllbckls.h\
	.\object.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\window.h\
	.\cvassert.h\
	.\winclass.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\callback.h\
	.\class.h\
	.\vwchar.h\
	.\color.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\assoc.h\
	.\objarray.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\iterator.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/accel.obj :  $(SOURCE)  $(DEP_ACCEL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpsite.cpp
DEP_CMPSI=\
	.\defs.h\
	.\cvassert.h\
	.\archiver.h\
	.\cmpsite.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\cmpcntl.h\
	.\object.h\
	.\str.h\
	.\vlocale.h\
	.\methdata.h\
	.\variable.h\
	.\status.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\cmpitem.h\
	.\cmpprog.h\
	.\iterator.h\
	.\class.h\
	.\vwchar.h\
	.\oledefs.h\
	.\result.h\
	.\containr.h\
	.\rect.h\
	.\window.h\
	.\cmpwin.h\
	.\advsink.h\
	.\oclsite.h\
	.\oipsite.h\
	.\dropsrc.h\
	.\oobject.h\
	.\callback.h\
	.\volestd.h\
	.\region.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\unknown.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\itemwin.h\
	.\cmpobj.h\
	.\winclass.h\
	.\wintypes.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\opoint.h\
	.\cmpstrg.h\
	.\color.h\
	.\style.h\
	.\location.h\
	.\storag.h\
	.\yesno.h\
	.\clrscm.h\
	.\bitset.h\
	.\rstorag.h\
	.\dialog.h\
	.\olerslt.h\
	.\poppwndw.h\
	.\view.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\control.h\
	.\popupmen.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h

$(INTDIR)/cmpsite.obj :  $(SOURCE)  $(DEP_CMPSI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\storag.cpp
DEP_STORA=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/storag.obj :  $(SOURCE)  $(DEP_STORA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpitem.cpp
DEP_CMPIT=\
	.\pre.h\
	.\notifier.h\
	.\port.h\
	.\cmpitem.h\
	.\archbyte.h\
	.\cmpcntl.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\voleui.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\cmpprog.h\
	.\iterator.h\
	.\ordcllct.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\wintypes.h\
	.\mdiclnt.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\clrscm.h\
	.\vlocale.h\
	.\methdata.h\
	.\variable.h\
	.\status.h\
	.\callback.h\
	.\color.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/cmpitem.obj :  $(SOURCE)  $(DEP_CMPIT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\brush.cpp
DEP_BRUSH=\
	.\winclass.h\
	.\brush.h\
	.\iterator.h\
	.\clrscm.h\
	.\cvassert.h\
	.\defs.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\collectn.h\
	.\class.h\
	.\color.h\
	.\objarray.h\
	.\callback.h\
	.\containr.h

$(INTDIR)/brush.obj :  $(SOURCE)  $(DEP_BRUSH) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tzus.cpp
DEP_TZUS_=\
	.\tzus.h\
	.\datetime.h\
	.\str.h\
	.\timezone.h\
	.\object.h\
	.\date.h\
	.\thyme.h\
	.\vwchar.h\
	.\defs.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/tzus.obj :  $(SOURCE)  $(DEP_TZUS_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\serial.cpp
DEP_SERIA=\
	.\winclass.h\
	.\serial.h\
	.\defs.h\
	.\strm.h\
	.\control.h\
	.\timer.h\
	.\str.h\
	.\window.h\
	.\cvassert.h\
	.\object.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\class.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\collectn.h\
	.\objarray.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\color.h\
	.\region.h\
	.\containr.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/serial.obj :  $(SOURCE)  $(DEP_SERIA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\multlist.cpp
DEP_MULTL=\
	.\multlist.h\
	.\notifier.h\
	.\str.h\
	.\archiver.h\
	.\listbox.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\object.h\
	.\vwchar.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\control.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\cvassert.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\result.h\
	.\color.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/multlist.obj :  $(SOURCE)  $(DEP_MULTL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\intntltb.cpp
DEP_INTNT=\
	.\intntltb.h\
	.\iterator.h\
	.\str.h\
	.\file.h\
	.\archiver.h\
	.\rscarch.h\
	.\archvrtg.h\
	.\object.h\
	.\assoc.h\
	.\dictinry.h\
	.\multiset.h\
	.\vlocale.h\
	.\collectn.h\
	.\vwchar.h\
	.\strm.h\
	.\pathstr.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\blockfil.h\
	.\defs.h\
	.\class.h\
	.\set.h\
	.\result.h\
	.\objarray.h\
	.\bfdir.h\
	.\stack.h\
	.\callback.h\
	.\containr.h\
	.\bfdiritm.h\
	.\winclass.h

$(INTDIR)/intntltb.obj :  $(SOURCE)  $(DEP_INTNT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cvthread.cpp
DEP_CVTHR=\
	.\defs.h\
	.\cvthread.h

$(INTDIR)/cvthread.obj :  $(SOURCE)  $(DEP_CVTHR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\intassoc.cpp
DEP_INTAS=\
	.\intassoc.h\
	.\archiver.h\
	.\assoc.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\archvrtg.h\
	.\object.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\class.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\callback.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/intassoc.obj :  $(SOURCE)  $(DEP_INTAS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\checkbox.cpp
DEP_CHECK=\
	.\checkbox.h\
	.\notifier.h\
	.\archiver.h\
	.\button.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\control.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\cvassert.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/checkbox.obj :  $(SOURCE)  $(DEP_CHECK) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\styles.cpp
DEP_STYLES=\
	.\object.h\
	.\implstyl.h\
	.\defs.h\
	.\class.h\
	.\exclstyl.h\
	.\callback.h\
	.\style.h\
	.\winclass.h\
	.\bitset.h

$(INTDIR)/styles.obj :  $(SOURCE)  $(DEP_STYLES) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\containr.cpp
DEP_CONTA=\
	.\containr.h\
	.\archiver.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/containr.obj :  $(SOURCE)  $(DEP_CONTA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\oipframe.cpp
DEP_OIPFR=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/oipframe.obj :  $(SOURCE)  $(DEP_OIPFR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\imanim.cpp
DEP_IMANI=\
	.\imanim.h\
	.\image.h\
	.\timer.h\
	.\notifier.h\
	.\str.h\
	.\archiver.h\
	.\imbox.h\
	.\object.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\winclass.h\
	.\window.h\
	.\vwchar.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\control.h\
	.\class.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\cvassert.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\color.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/imanim.obj :  $(SOURCE)  $(DEP_IMANI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\attach.cpp
DEP_ATTAC=\
	.\attach.h\
	.\defs.h

$(INTDIR)/attach.obj :  $(SOURCE)  $(DEP_ATTAC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\groupbox.cpp
DEP_GROUPB=\
	.\winclass.h\
	.\groupbox.h\
	.\str.h\
	.\notifier.h\
	.\brush.h\
	.\font.h\
	.\defs.h\
	.\control.h\
	.\object.h\
	.\vwchar.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\window.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\cvassert.h\
	.\class.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\color.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/groupbox.obj :  $(SOURCE)  $(DEP_GROUPB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\georow.cpp
DEP_GEORO=\
	.\georow.h\
	.\archiver.h\
	.\georoc.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\geomgr.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\geoframe.h\
	.\iterator.h\
	.\str.h\
	.\result.h\
	.\object.h\
	.\containr.h\
	.\window.h\
	.\frame.h\
	.\vwchar.h\
	.\defs.h\
	.\class.h\
	.\attach.h\
	.\display.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\rect.h\
	.\ratio.h\
	.\callback.h\
	.\winclass.h\
	.\wintypes.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\region.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/georow.obj :  $(SOURCE)  $(DEP_GEORO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bfdir.cpp
DEP_BFDIR=\
	.\bfdir.h\
	.\bfdiritm.h\
	.\ordcllct.h\
	.\iterator.h\
	.\object.h\
	.\str.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\class.h\
	.\vwchar.h\
	.\containr.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/bfdir.obj :  $(SOURCE)  $(DEP_BFDIR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdmdiap.cpp
DEP_STDMDI=\
	.\stdmdiap.h\
	.\toolbar.h\
	.\statusbr.h\
	.\popupmen.h\
	.\menuitem.h\
	.\attach.h\
	.\textbox.h\
	.\ordcllct.h\
	.\iterator.h\
	.\stdmdivw.h\
	.\notifier.h\
	.\archiver.h\
	.\mdiappvw.h\
	.\control.h\
	.\shade.h\
	.\brush.h\
	.\str.h\
	.\menu.h\
	.\menudata.h\
	.\display.h\
	.\wintypes.h\
	.\defs.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\mdiview.h\
	.\appview.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\mdiclnt.h\
	.\cvassert.h\
	.\color.h\
	.\clrscm.h\
	.\vwchar.h\
	.\class.h\
	.\view.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/stdmdiap.obj :  $(SOURCE)  $(DEP_STDMDI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\notebook.cpp
DEP_NOTEB=\
	.\defs.h\
	.\brush.h\
	.\dialog.h\
	.\location.h\
	.\notebook.h\
	.\notepage.h\
	.\port.h\
	.\clrscm.h\
	.\iterator.h\
	.\rscarch.h\
	.\notifier.h\
	.\cllbckls.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\poppwndw.h\
	.\control.h\
	.\str.h\
	.\window.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\rect.h\
	.\font.h\
	.\collectn.h\
	.\archiver.h\
	.\blockfil.h\
	.\appview.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\callback.h\
	.\class.h\
	.\color.h\
	.\objarray.h\
	.\view.h\
	.\cvassert.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\region.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\strm.h\
	.\result.h\
	.\bfdiritm.h\
	.\menudata.h\
	.\menuitem.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/notebook.obj :  $(SOURCE)  $(DEP_NOTEB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ddecls.cpp
DEP_DDECL=\
	.\notifier.h\
	.\ddecls.h\
	.\atom.h\
	.\archiver.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\control.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/ddecls.obj :  $(SOURCE)  $(DEP_DDECL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\callback.cpp
DEP_CALLB=\
	.\callback.h\
	.\cllbckls.h\
	.\memry.h\
	.\winclass.h\
	.\object.h\
	.\defs.h\
	.\global.h\
	.\memry16.h\
	.\class.h

$(INTDIR)/callback.obj :  $(SOURCE)  $(DEP_CALLB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\workbook.cpp
DEP_WORKB=\
	.\brush.h\
	.\dialog.h\
	.\location.h\
	.\workbook.h\
	.\workpage.h\
	.\port.h\
	.\clrscm.h\
	.\iterator.h\
	.\pointrry.h\
	.\region.h\
	.\polygon.h\
	.\shade.h\
	.\notifier.h\
	.\rscarch.h\
	.\cllbckls.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\poppwndw.h\
	.\control.h\
	.\str.h\
	.\window.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\rect.h\
	.\font.h\
	.\collectn.h\
	.\containr.h\
	.\defs.h\
	.\color.h\
	.\appview.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\archiver.h\
	.\blockfil.h\
	.\callback.h\
	.\class.h\
	.\objarray.h\
	.\view.h\
	.\cvassert.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\menuitem.h\
	.\strm.h\
	.\result.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/workbook.obj :  $(SOURCE)  $(DEP_WORKB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\utilwin.cpp
DEP_UTILW=\
	.\utilwin.h\
	.\port.h\
	.\brush.h\
	.\pen.h\
	.\color.h\
	.\clrscm.h\
	.\font.h\
	.\mouscrsr.h\
	.\rband.h\
	.\str.h\
	.\rect.h\
	.\ordcllct.h\
	.\notifier.h\
	.\popupmen.h\
	.\menuitem.h\
	.\menudata.h\
	.\archiver.h\
	.\cllbckls.h\
	.\poppwndw.h\
	.\geomgr.h\
	.\display.h\
	.\bitbltr.h\
	.\object.h\
	.\wintypes.h\
	.\defs.h\
	.\winclass.h\
	.\vwchar.h\
	.\region.h\
	.\collectn.h\
	.\objarray.h\
	.\appview.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\window.h\
	.\menu.h\
	.\iterator.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\view.h\
	.\geoframe.h\
	.\class.h\
	.\icon.h\
	.\assoc.h\
	.\attach.h\
	.\frame.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/utilwin.obj :  $(SOURCE)  $(DEP_UTILW) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\toolbox.cpp
DEP_TOOLBO=\
	.\toolbox.h\
	.\georow.h\
	.\geocol.h\
	.\clrscm.h\
	.\utilwin.h\
	.\georoc.h\
	.\object.h\
	.\poppwndw.h\
	.\geomgr.h\
	.\popupmen.h\
	.\rband.h\
	.\defs.h\
	.\class.h\
	.\view.h\
	.\geoframe.h\
	.\ordcllct.h\
	.\iterator.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\str.h\
	.\callback.h\
	.\set.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\window.h\
	.\frame.h\
	.\collectn.h\
	.\objarray.h\
	.\wintypes.h\
	.\display.h\
	.\vwchar.h\
	.\winclass.h\
	.\control.h\
	.\attach.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\rect.h\
	.\ratio.h\
	.\containr.h\
	.\color.h\
	.\cvassert.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\region.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/toolbox.obj :  $(SOURCE)  $(DEP_TOOLBO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\sash.cpp
DEP_SASH_=\
	.\sash.h\
	.\notifier.h\
	.\appview.h\
	.\pushbttn.h\
	.\control.h\
	.\port.h\
	.\pen.h\
	.\brush.h\
	.\mouscrsr.h\
	.\attach.h\
	.\ordcllct.h\
	.\iterator.h\
	.\shade.h\
	.\archiver.h\
	.\defs.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\button.h\
	.\cvassert.h\
	.\display.h\
	.\bitbltr.h\
	.\font.h\
	.\object.h\
	.\wintypes.h\
	.\clrscm.h\
	.\objarray.h\
	.\color.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\frame.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\result.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/sash.obj :  $(SOURCE)  $(DEP_SASH_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\intset.cpp
DEP_INTSE=\
	.\intset.h\
	.\archiver.h\
	.\containr.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\class.h\
	.\str.h\
	.\result.h\
	.\callback.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/intset.obj :  $(SOURCE)  $(DEP_INTSE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tokens.cpp
DEP_TOKEN=\
	.\tokstrm.h\
	.\str.h\
	.\tokens.h\
	.\iterator.h\
	.\vwchar.h\
	.\tagstrm.h\
	.\object.h\
	.\dictinry.h\
	.\collectn.h\
	.\defs.h\
	.\tofmstrm.h\
	.\class.h\
	.\set.h\
	.\assoc.h\
	.\strm.h\
	.\callback.h\
	.\objarray.h\
	.\winclass.h\
	.\containr.h

$(INTDIR)/tokens.obj :  $(SOURCE)  $(DEP_TOKEN) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\menuitem.cpp
DEP_MENUI=\
	.\menuitem.h\
	.\menu.h\
	.\notifier.h\
	.\window.h\
	.\set.h\
	.\archiver.h\
	.\cllbckls.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\timer.h\
	.\winclass.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\objarray.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\object.h\
	.\vwchar.h\
	.\color.h\
	.\view.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\strm.h\
	.\pathstr.h\
	.\class.h\
	.\clrscm.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\iterator.h\
	.\style.h\
	.\cvassert.h\
	.\result.h\
	.\control.h\
	.\bitset.h

$(INTDIR)/menuitem.obj :  $(SOURCE)  $(DEP_MENUI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\appview.cpp
DEP_APPVI=\
	.\appview.h\
	.\stdmdiap.h\
	.\winclass.h\
	.\iterator.h\
	.\notifier.h\
	.\cmpappvw.h\
	.\archiver.h\
	.\callback.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\mdiappvw.h\
	.\statusbr.h\
	.\defs.h\
	.\object.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\stack.h\
	.\oledefs.h\
	.\unknown.h\
	.\oipappvw.h\
	.\cmpview.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\mdiclnt.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\class.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\volestd.h\
	.\oipframe.h\
	.\cmpwin.h\
	.\oipuiwin.h\
	.\strm.h\
	.\pathstr.h\
	.\color.h\
	.\cvassert.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\itemwin.h\
	.\cmpobj.h\
	.\result.h\
	.\style.h\
	.\opoint.h\
	.\cmpstrg.h\
	.\bitset.h\
	.\location.h\
	.\storag.h\
	.\yesno.h\
	.\rstorag.h\
	.\dialog.h\
	.\olerslt.h\
	.\poppwndw.h

$(INTDIR)/appview.obj :  $(SOURCE)  $(DEP_APPVI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\multslct.cpp
DEP_MULTS=\
	.\iterator.h\
	.\ordcllct.h\
	.\str.h\
	.\multlist.h\
	.\pushbttn.h\
	.\multslct.h\
	.\brush.h\
	.\shdctrl.h\
	.\object.h\
	.\collectn.h\
	.\objarray.h\
	.\vwchar.h\
	.\listbox.h\
	.\button.h\
	.\dialog.h\
	.\wintypes.h\
	.\clrscm.h\
	.\control.h\
	.\shade.h\
	.\defs.h\
	.\class.h\
	.\containr.h\
	.\poppwndw.h\
	.\color.h\
	.\window.h\
	.\cvassert.h\
	.\winclass.h\
	.\callback.h\
	.\view.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\set.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\popupmen.h\
	.\region.h\
	.\style.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\bitset.h

$(INTDIR)/multslct.obj :  $(SOURCE)  $(DEP_MULTS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\input.cpp
DEP_INPUT=\
	.\input.h\
	.\group.h\
	.\textbox.h\
	.\editline.h\
	.\pushbttn.h\
	.\georow.h\
	.\geocol.h\
	.\str.h\
	.\clrscm.h\
	.\notifier.h\
	.\dialog.h\
	.\control.h\
	.\ordcllct.h\
	.\editbox.h\
	.\button.h\
	.\georoc.h\
	.\object.h\
	.\vwchar.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\poppwndw.h\
	.\cvassert.h\
	.\objarray.h\
	.\geomgr.h\
	.\class.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\geoframe.h\
	.\iterator.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/input.obj :  $(SOURCE)  $(DEP_INPUT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\exclstyl.cpp
DEP_EXCLS=\
	.\defs.h\
	.\archiver.h\
	.\bitset.h\
	.\style.h\
	.\exclstyl.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\class.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\callback.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/exclstyl.obj :  $(SOURCE)  $(DEP_EXCLS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clipbord.cpp
DEP_CLIPB=\
	.\winclass.h\
	.\window.h\
	.\clipbord.h\
	.\archiver.h\
	.\notifier.h\
	.\defs.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\wintypes.h\
	.\ratio.h\
	.\objarray.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\assoc.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\vwchar.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\callback.h\
	.\result.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\clrscm.h\
	.\bitset.h\
	.\control.h

$(INTDIR)/clipbord.obj :  $(SOURCE)  $(DEP_CLIPB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\opoint.cpp
DEP_OPOIN=\
	.\pre.h\
	.\location.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/opoint.obj :  $(SOURCE)  $(DEP_OPOIN) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mdiclnt.cpp
DEP_MDICL=\
	.\mdiclnt.h\
	.\mdiview.h\
	.\notifier.h\
	.\popupmen.h\
	.\winclass.h\
	.\iterator.h\
	.\window.h\
	.\view.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\object.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\wintypes.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/mdiclnt.obj :  $(SOURCE)  $(DEP_MDICL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\brushinf.cpp
DEP_BRUSHI=\
	.\brush.h\
	.\brushinf.h\
	.\archiver.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\color.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\collectn.h\
	.\objarray.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\callback.h\
	.\containr.h\
	.\str.h\
	.\result.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/brushinf.obj :  $(SOURCE)  $(DEP_BRUSHI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\blockfil.cpp
DEP_BLOCK=\
	.\blockfil.h\
	.\iterator.h\
	.\ordcllct.h\
	.\str.h\
	.\wsscanf.h\
	.\object.h\
	.\file.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\collectn.h\
	.\objarray.h\
	.\vwchar.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\result.h\
	.\bfdiritm.h\
	.\containr.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/blockfil.obj :  $(SOURCE)  $(DEP_BLOCK) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mouscrsr.cpp
DEP_MOUSC=\
	.\mouscrsr.h\
	.\notifier.h\
	.\object.h\
	.\winclass.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\class.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/mouscrsr.obj :  $(SOURCE)  $(DEP_MOUSC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\listbox.cpp
DEP_LISTB=\
	.\listbox.h\
	.\notifier.h\
	.\str.h\
	.\port.h\
	.\font.h\
	.\archiver.h\
	.\ordcllct.h\
	.\iterator.h\
	.\cllbckls.h\
	.\control.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\object.h\
	.\vwchar.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\objarray.h\
	.\callback.h\
	.\cvassert.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\class.h\
	.\wintypes.h\
	.\clrscm.h\
	.\strm.h\
	.\pathstr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\result.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/listbox.obj :  $(SOURCE)  $(DEP_LISTB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\yesno.cpp
DEP_YESNO=\
	.\winclass.h\
	.\yesno.h\
	.\notifier.h\
	.\defs.h\
	.\dialog.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\poppwndw.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/yesno.obj :  $(SOURCE)  $(DEP_YESNO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ordcllct.cpp
DEP_ORDCL=\
	.\ordcllct.h\
	.\iterator.h\
	.\archiver.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\containr.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/ordcllct.obj :  $(SOURCE)  $(DEP_ORDCL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\memtest.cpp
DEP_MEMTE=\
	.\memtest.h\
	.\memry.h\
	.\memry16.h\
	.\global.h\
	.\object.h\
	.\defs.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/memtest.obj :  $(SOURCE)  $(DEP_MEMTE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bfdiritm.cpp
DEP_BFDIRI=\
	.\bfdiritm.h\
	.\object.h\
	.\str.h\
	.\defs.h\
	.\class.h\
	.\vwchar.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/bfdiritm.obj :  $(SOURCE)  $(DEP_BFDIRI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\winrect.cpp
DEP_WINRE=\
	.\pre.h\
	.\winrect.h\
	.\archiver.h\
	.\oledefs.h\
	.\object.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\rect.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\region.h\
	.\assoc.h\
	.\objarray.h\
	.\callback.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/winrect.obj :  $(SOURCE)  $(DEP_WINRE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\implstyl.cpp
DEP_IMPLS=\
	.\defs.h\
	.\archiver.h\
	.\cvassert.h\
	.\bitset.h\
	.\style.h\
	.\exclstyl.h\
	.\implstyl.h\
	.\stylexpr.h\
	.\styles.h\
	.\window.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\winclass.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\class.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\callback.h\
	.\color.h\
	.\region.h\
	.\vwchar.h\
	.\clrscm.h

$(INTDIR)/implstyl.obj :  $(SOURCE)  $(DEP_IMPLS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\class.cpp
DEP_CLASS=\
	.\object.h\
	.\class.h\
	.\classtbl.h\
	.\defs.h\
	.\callback.h\
	.\global.h\
	.\winclass.h

$(INTDIR)/class.obj :  $(SOURCE)  $(DEP_CLASS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\vtree.cpp
DEP_VTREE=\
	.\vtree.h\
	.\vtlistbx.h\
	.\treenode.h\
	.\ordcllct.h\
	.\archiver.h\
	.\view.h\
	.\drag.h\
	.\defs.h\
	.\str.h\
	.\color.h\
	.\collectn.h\
	.\objarray.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\set.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\window.h\
	.\global.h\
	.\object.h\
	.\vwchar.h\
	.\clrscm.h\
	.\containr.h\
	.\strm.h\
	.\pathstr.h\
	.\assoc.h\
	.\control.h\
	.\popupmen.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\class.h\
	.\result.h\
	.\cvassert.h\
	.\winclass.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\region.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/vtree.obj :  $(SOURCE)  $(DEP_VTREE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tagstrm.cpp
DEP_TAGST=\
	.\tagstrm.h\
	.\dictinry.h\
	.\archiver.h\
	.\tofmstrm.h\
	.\set.h\
	.\assoc.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\pathstr.h\
	.\str.h\
	.\containr.h\
	.\defs.h\
	.\class.h\
	.\result.h\
	.\vwchar.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/tagstrm.obj :  $(SOURCE)  $(DEP_TAGST) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\userctrl.cpp
DEP_USERC=\
	.\userctrl.h\
	.\ordcllct.h\
	.\port.h\
	.\notifier.h\
	.\archiver.h\
	.\cllbckls.h\
	.\control.h\
	.\str.h\
	.\collectn.h\
	.\objarray.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\rect.h\
	.\font.h\
	.\defs.h\
	.\appview.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\cvassert.h\
	.\object.h\
	.\vwchar.h\
	.\wintypes.h\
	.\clrscm.h\
	.\region.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\class.h\
	.\color.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/userctrl.obj :  $(SOURCE)  $(DEP_USERC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dictinry.cpp
DEP_DICTI=\
	.\dictinry.h\
	.\iterator.h\
	.\str.h\
	.\set.h\
	.\assoc.h\
	.\object.h\
	.\collectn.h\
	.\vwchar.h\
	.\objarray.h\
	.\defs.h\
	.\class.h\
	.\containr.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/dictinry.obj :  $(SOURCE)  $(DEP_DICTI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpcntl.cpp
DEP_CMPCN=\
	.\cmpcntl.h\
	.\cmpsite.h\
	.\cmpitem.h\
	.\cmpprog.h\
	.\iterator.h\
	.\ordcllct.h\
	.\object.h\
	.\str.h\
	.\vlocale.h\
	.\methdata.h\
	.\variable.h\
	.\status.h\
	.\archiver.h\
	.\rect.h\
	.\window.h\
	.\pathstr.h\
	.\cmpwin.h\
	.\advsink.h\
	.\oclsite.h\
	.\oipsite.h\
	.\dropsrc.h\
	.\oobject.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\class.h\
	.\vwchar.h\
	.\oledefs.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\region.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\result.h\
	.\unknown.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\itemwin.h\
	.\cmpobj.h\
	.\containr.h\
	.\callback.h\
	.\volestd.h\
	.\strm.h\
	.\set.h\
	.\assoc.h\
	.\wintypes.h\
	.\winclass.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\opoint.h\
	.\cmpstrg.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\location.h\
	.\storag.h\
	.\yesno.h\
	.\clrscm.h\
	.\bitset.h\
	.\rstorag.h\
	.\dialog.h\
	.\olerslt.h\
	.\poppwndw.h\
	.\view.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\control.h\
	.\popupmen.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h

$(INTDIR)/cmpcntl.obj :  $(SOURCE)  $(DEP_CMPCN) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\status.cpp
DEP_STATU=\
	.\defs.h\
	.\cvassert.h\
	.\archiver.h\
	.\status.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\oledefs.h\
	.\object.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/status.obj :  $(SOURCE)  $(DEP_STATU) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\imbox.cpp
DEP_IMBOX=\
	.\imbox.h\
	.\image.h\
	.\rect.h\
	.\clrscm.h\
	.\color.h\
	.\brush.h\
	.\pen.h\
	.\port.h\
	.\str.h\
	.\notifier.h\
	.\archiver.h\
	.\rscarch.h\
	.\control.h\
	.\object.h\
	.\region.h\
	.\defs.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\display.h\
	.\bitbltr.h\
	.\font.h\
	.\vwchar.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\blockfil.h\
	.\cvassert.h\
	.\class.h\
	.\objarray.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/imbox.obj :  $(SOURCE)  $(DEP_IMBOX) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\editline.cpp
DEP_EDITL=\
	.\winclass.h\
	.\editline.h\
	.\str.h\
	.\notifier.h\
	.\archiver.h\
	.\defs.h\
	.\editbox.h\
	.\object.h\
	.\vwchar.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\window.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\textbox.h\
	.\class.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\control.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\cvassert.h\
	.\color.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/editline.obj :  $(SOURCE)  $(DEP_EDITL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bitset.cpp
DEP_BITSE=\
	.\defs.h\
	.\archiver.h\
	.\object.h\
	.\cvassert.h\
	.\bitset.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/bitset.obj :  $(SOURCE)  $(DEP_BITSE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\methdata.cpp
DEP_METHD=\
	.\defs.h\
	.\cvassert.h\
	.\archiver.h\
	.\iterator.h\
	.\methdata.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\collectn.h\
	.\status.h\
	.\variable.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\objarray.h\
	.\class.h\
	.\oledefs.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\callback.h\
	.\volestd.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/methdata.obj :  $(SOURCE)  $(DEP_METHD) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mdiappvw.cpp
DEP_MDIAP=\
	.\winclass.h\
	.\mdiappvw.h\
	.\notifier.h\
	.\popupmen.h\
	.\mdiview.h\
	.\ordcllct.h\
	.\iterator.h\
	.\archiver.h\
	.\callback.h\
	.\cmpappvw.h\
	.\defs.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\view.h\
	.\objarray.h\
	.\object.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\stack.h\
	.\oledefs.h\
	.\unknown.h\
	.\oipappvw.h\
	.\cmpview.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\wintypes.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\volestd.h\
	.\oipframe.h\
	.\cmpwin.h\
	.\oipuiwin.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\control.h\
	.\result.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\itemwin.h\
	.\cmpobj.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\opoint.h\
	.\cmpstrg.h\
	.\bitset.h\
	.\location.h\
	.\storag.h\
	.\yesno.h\
	.\rstorag.h\
	.\dialog.h\
	.\olerslt.h\
	.\poppwndw.h

$(INTDIR)/mdiappvw.obj :  $(SOURCE)  $(DEP_MDIAP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\archbyte.cpp
DEP_ARCHB=\
	.\pre.h\
	.\archbyte.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/archbyte.obj :  $(SOURCE)  $(DEP_ARCHB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shade.cpp
DEP_SHADE=\
	.\display.h\
	.\shade.h\
	.\port.h\
	.\rect.h\
	.\clrscm.h\
	.\notifier.h\
	.\object.h\
	.\winclass.h\
	.\wintypes.h\
	.\defs.h\
	.\color.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\region.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\class.h\
	.\ordcllct.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/shade.obj :  $(SOURCE)  $(DEP_SHADE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rscarch.cpp
DEP_RSCAR=\
	.\rscarch.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\memry.h\
	.\archiver.h\
	.\blockfil.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\memry16.h\
	.\file.h\
	.\archvrtg.h\
	.\object.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\containr.h\
	.\global.h\
	.\strm.h\
	.\defs.h\
	.\class.h\
	.\str.h\
	.\result.h\
	.\bfdiritm.h\
	.\callback.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/rscarch.obj :  $(SOURCE)  $(DEP_RSCAR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printer.cpp
DEP_PRINTE=\
	.\printer.h\
	.\winclass.h\
	.\notifier.h\
	.\display.h\
	.\str.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\window.h\
	.\object.h\
	.\wintypes.h\
	.\vwchar.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\class.h\
	.\color.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\clrscm.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/printer.obj :  $(SOURCE)  $(DEP_PRINTE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\popupmen.cpp
DEP_POPUP=\
	.\notifier.h\
	.\popupmen.h\
	.\window.h\
	.\menuitem.h\
	.\set.h\
	.\iterator.h\
	.\archiver.h\
	.\cllbckls.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\menu.h\
	.\menudata.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\wintypes.h\
	.\objarray.h\
	.\object.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\view.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\result.h\
	.\control.h\
	.\bitset.h

$(INTDIR)/popupmen.obj :  $(SOURCE)  $(DEP_POPUP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\olerslt.cpp
DEP_OLERS=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/olerslt.obj :  $(SOURCE)  $(DEP_OLERS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cllbckls.cpp
DEP_CLLBC=\
	.\defs.h\
	.\cllbckls.h\
	.\callback.h\
	.\classtbl.h\
	.\memry.h\
	.\window.h\
	.\notifier.h\
	.\cvthread.h\
	.\global.h\
	.\winclass.h\
	.\object.h\
	.\memry16.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\class.h\
	.\wintypes.h\
	.\ratio.h\
	.\objarray.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\clrscm.h\
	.\bitset.h\
	.\control.h

$(INTDIR)/cllbckls.obj :  $(SOURCE)  $(DEP_CLLBC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rect.cpp
DEP_RECT_=\
	.\winclass.h\
	.\rect.h\
	.\port.h\
	.\pointrry.h\
	.\defs.h\
	.\region.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\containr.h\
	.\location.h\
	.\wintypes.h\
	.\object.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\color.h\
	.\class.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h

$(INTDIR)/rect.obj :  $(SOURCE)  $(DEP_RECT_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\atom.cpp
DEP_ATOM_=\
	.\winclass.h\
	.\memry.h\
	.\atom.h\
	.\defs.h\
	.\memry16.h\
	.\global.h\
	.\object.h\
	.\class.h\
	.\callback.h

$(INTDIR)/atom.obj :  $(SOURCE)  $(DEP_ATOM_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\docksite.cpp
DEP_DOCKS=\
	.\docksite.h\
	.\dockwin.h\
	.\brush.h\
	.\pen.h\
	.\color.h\
	.\clrscm.h\
	.\port.h\
	.\rect.h\
	.\georow.h\
	.\geocol.h\
	.\archiver.h\
	.\cllbckls.h\
	.\notifier.h\
	.\window.h\
	.\utilwin.h\
	.\popupmen.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\defs.h\
	.\display.h\
	.\bitbltr.h\
	.\font.h\
	.\region.h\
	.\georoc.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\global.h\
	.\winclass.h\
	.\callback.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\poppwndw.h\
	.\geomgr.h\
	.\rband.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\objarray.h\
	.\strm.h\
	.\pathstr.h\
	.\assoc.h\
	.\view.h\
	.\icon.h\
	.\vwchar.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\geoframe.h\
	.\result.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\style.h\
	.\cvassert.h\
	.\control.h\
	.\bitset.h

$(INTDIR)/docksite.obj :  $(SOURCE)  $(DEP_DOCKS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\listslct.cpp
DEP_LISTS=\
	.\listslct.h\
	.\ordcllct.h\
	.\iterator.h\
	.\listbox.h\
	.\pushbttn.h\
	.\str.h\
	.\notifier.h\
	.\brush.h\
	.\shdctrl.h\
	.\dialog.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\control.h\
	.\button.h\
	.\vwchar.h\
	.\defs.h\
	.\appview.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\wintypes.h\
	.\clrscm.h\
	.\shade.h\
	.\poppwndw.h\
	.\class.h\
	.\cvassert.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\color.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/listslct.obj :  $(SOURCE)  $(DEP_LISTS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\control.cpp
DEP_CONTR=\
	.\control.h\
	.\notifier.h\
	.\str.h\
	.\set.h\
	.\memry.h\
	.\font.h\
	.\port.h\
	.\group.h\
	.\cntrlhlp.h\
	.\cvassert.h\
	.\fontinfo.h\
	.\cmpwin.h\
	.\archiver.h\
	.\window.h\
	.\winclass.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\timer.h\
	.\object.h\
	.\vwchar.h\
	.\objarray.h\
	.\memry16.h\
	.\ordcllct.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\poppwndw.h\
	.\oledefs.h\
	.\unknown.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\itemwin.h\
	.\cmpobj.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\class.h\
	.\wintypes.h\
	.\clrscm.h\
	.\volestd.h\
	.\opoint.h\
	.\cmpstrg.h\
	.\strm.h\
	.\pathstr.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\callback.h\
	.\color.h\
	.\location.h\
	.\storag.h\
	.\yesno.h\
	.\result.h\
	.\style.h\
	.\rstorag.h\
	.\dialog.h\
	.\bitset.h\
	.\olerslt.h

$(INTDIR)/control.obj :  $(SOURCE)  $(DEP_CONTR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\voleui.cpp
DEP_VOLEU=\
	.\pre.h\
	.\port.h\
	.\pen.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\display.h\
	.\bitbltr.h\
	.\brush.h\
	.\rect.h\
	.\font.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\str.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\region.h\
	.\color.h\
	.\objarray.h\
	.\callback.h\
	.\assoc.h\
	.\popupmen.h\
	.\icon.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/voleui.obj :  $(SOURCE)  $(DEP_VOLEU) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpwin.cpp
DEP_CMPWI=\
	.\pre.h\
	.\iterator.h\
	.\cmpslct.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\collectn.h\
	.\dialog.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\poppwndw.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/cmpwin.obj :  $(SOURCE)  $(DEP_CMPWI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\advsink.cpp
DEP_ADVSI=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/advsink.obj :  $(SOURCE)  $(DEP_ADVSI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\globals2.cpp
DEP_GLOBAL=\
	.\defs.h\
	.\result.h\
	.\debug.h\
	.\olerslt.h\
	.\str.h\
	.\oledefs.h\
	.\cvassert.h\
	.\object.h\
	.\vwchar.h\
	.\volestd.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/globals2.obj :  $(SOURCE)  $(DEP_GLOBAL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\georoc.cpp
DEP_GEOROC=\
	.\georoc.h\
	.\ordcllct.h\
	.\iterator.h\
	.\archiver.h\
	.\geomgr.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\geoframe.h\
	.\containr.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\window.h\
	.\frame.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\attach.h\
	.\display.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\rect.h\
	.\ratio.h\
	.\winclass.h\
	.\vwchar.h\
	.\wintypes.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\region.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/georoc.obj :  $(SOURCE)  $(DEP_GEOROC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tokstrm.cpp
DEP_TOKST=\
	.\tokstrm.h\
	.\tokens.h\
	.\tagstrm.h\
	.\str.h\
	.\dictinry.h\
	.\tofmstrm.h\
	.\object.h\
	.\vwchar.h\
	.\set.h\
	.\assoc.h\
	.\strm.h\
	.\defs.h\
	.\class.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\containr.h\
	.\winclass.h

$(INTDIR)/tokstrm.obj :  $(SOURCE)  $(DEP_TOKST) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\eventmgr.cpp
DEP_EVENT=\
	.\eventmgr.h\
	.\dictinry.h\
	.\set.h\
	.\ordcllct.h\
	.\iterator.h\
	.\assoc.h\
	.\global.h\
	.\str.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\vwchar.h\
	.\containr.h\
	.\defs.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/eventmgr.obj :  $(SOURCE)  $(DEP_EVENT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cvassert.cpp
DEP_CVASS=\
	.\defs.h\
	.\object.h\
	.\notifier.h\
	.\class.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\callback.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/cvassert.obj :  $(SOURCE)  $(DEP_CVASS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tristate.cpp
DEP_TRIST=\
	.\winclass.h\
	.\tristate.h\
	.\archiver.h\
	.\defs.h\
	.\checkbox.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\button.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\control.h\
	.\str.h\
	.\result.h\
	.\object.h\
	.\containr.h\
	.\window.h\
	.\cvassert.h\
	.\vwchar.h\
	.\class.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/tristate.obj :  $(SOURCE)  $(DEP_TRIST) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dockwin.cpp
DEP_DOCKW=\
	.\dockwin.h\
	.\docksite.h\
	.\utilwin.h\
	.\geoform.h\
	.\rband.h\
	.\ordcllct.h\
	.\iterator.h\
	.\notifier.h\
	.\popupmen.h\
	.\menudata.h\
	.\archiver.h\
	.\cllbckls.h\
	.\window.h\
	.\poppwndw.h\
	.\geomgr.h\
	.\attach.h\
	.\object.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\appview.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\menu.h\
	.\menuitem.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\view.h\
	.\geoframe.h\
	.\class.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\wintypes.h\
	.\strm.h\
	.\pathstr.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\color.h\
	.\result.h\
	.\style.h\
	.\cvassert.h\
	.\control.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/dockwin.obj :  $(SOURCE)  $(DEP_DOCKW) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\color.cpp
DEP_COLOR=\
	.\winclass.h\
	.\wintypes.h\
	.\color.h\
	.\clrscm.h\
	.\cvassert.h\
	.\defs.h\
	.\object.h\
	.\class.h\
	.\callback.h

$(INTDIR)/color.obj :  $(SOURCE)  $(DEP_COLOR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bitmap.cpp
DEP_BITMAP=\
	.\winclass.h\
	.\notifier.h\
	.\bitmap.h\
	.\memry.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\display.h\
	.\archiver.h\
	.\location.h\
	.\memry16.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\wintypes.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\strm.h\
	.\pathstr.h\
	.\control.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\result.h\
	.\bitset.h

$(INTDIR)/bitmap.obj :  $(SOURCE)  $(DEP_BITMAP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\timezone.cpp
DEP_TIMEZ=\
	.\timezone.h\
	.\datetime.h\
	.\ordcllct.h\
	.\iterator.h\
	.\str.h\
	.\archiver.h\
	.\object.h\
	.\date.h\
	.\thyme.h\
	.\collectn.h\
	.\objarray.h\
	.\vwchar.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\containr.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\callback.h\
	.\result.h\
	.\winclass.h

$(INTDIR)/timezone.obj :  $(SOURCE)  $(DEP_TIMEZ) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\debug.cpp
DEP_DEBUG=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/debug.obj :  $(SOURCE)  $(DEP_DEBUG) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\classtbl.cpp
DEP_CLASST=\
	.\defs.h\
	.\cvthread.h\
	.\classtbl.h\
	.\memry.h\
	.\global.h\
	.\winclass.h\
	.\memry16.h\
	.\object.h\
	.\class.h\
	.\callback.h

$(INTDIR)/classtbl.obj :  $(SOURCE)  $(DEP_CLASST) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\array.cpp
DEP_ARRAY=\
	.\defs.h\
	.\cvassert.h\
	.\array.h\
	.\object.h\
	.\variable.h\
	.\class.h\
	.\oledefs.h\
	.\callback.h\
	.\volestd.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/array.obj :  $(SOURCE)  $(DEP_ARRAY) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\image.cpp
DEP_IMAGE=\
	.\image.h\
	.\rect.h\
	.\clrscm.h\
	.\color.h\
	.\brush.h\
	.\pen.h\
	.\bitmap.h\
	.\port.h\
	.\bitbltr.h\
	.\str.h\
	.\notifier.h\
	.\archiver.h\
	.\rscarch.h\
	.\object.h\
	.\region.h\
	.\defs.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\display.h\
	.\location.h\
	.\font.h\
	.\vwchar.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\blockfil.h\
	.\class.h\
	.\objarray.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\bfdiritm.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/image.obj :  $(SOURCE)  $(DEP_IMAGE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\event.cpp
DEP_EVENT_=\
	.\event.h\
	.\defs.h\
	.\winclass.h

$(INTDIR)/event.obj :  $(SOURCE)  $(DEP_EVENT_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdappvw.cpp
DEP_STDAP=\
	.\stdappvw.h\
	.\menu.h\
	.\popupmen.h\
	.\menuitem.h\
	.\menudata.h\
	.\toolbar.h\
	.\statusbr.h\
	.\ordcllct.h\
	.\iterator.h\
	.\archiver.h\
	.\appview.h\
	.\wintypes.h\
	.\str.h\
	.\display.h\
	.\object.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\collectn.h\
	.\objarray.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\view.h\
	.\icon.h\
	.\defs.h\
	.\color.h\
	.\vwchar.h\
	.\winclass.h\
	.\class.h\
	.\window.h\
	.\cvassert.h\
	.\clrscm.h\
	.\containr.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\callback.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\result.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\region.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/stdappvw.obj :  $(SOURCE)  $(DEP_STDAP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\peninfo.cpp
DEP_PENIN=\
	.\pen.h\
	.\peninfo.h\
	.\archiver.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\color.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\collectn.h\
	.\objarray.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\callback.h\
	.\containr.h\
	.\str.h\
	.\result.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/peninfo.obj :  $(SOURCE)  $(DEP_PENIN) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\substrg.cpp
DEP_SUBST=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/substrg.obj :  $(SOURCE)  $(DEP_SUBST) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rband.cpp
DEP_RBAND=\
	.\rband.h\
	.\rect.h\
	.\mouscrsr.h\
	.\wintypes.h\
	.\notifier.h\
	.\object.h\
	.\region.h\
	.\winclass.h\
	.\defs.h\
	.\color.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\class.h\
	.\clrscm.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/rband.obj :  $(SOURCE)  $(DEP_RBAND) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\archvrtg.cpp
DEP_ARCHV=\
	.\archiver.h\
	.\archvrtg.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\str.h\
	.\result.h\
	.\object.h\
	.\containr.h\
	.\vwchar.h\
	.\defs.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/archvrtg.obj :  $(SOURCE)  $(DEP_ARCHV) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\transwin.cpp
DEP_TRANS=\
	.\transwin.h\
	.\window.h\
	.\notifier.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\object.h\
	.\wintypes.h\
	.\ratio.h\
	.\objarray.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\class.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\callback.h\
	.\clrscm.h\
	.\bitset.h\
	.\control.h

$(INTDIR)/transwin.obj :  $(SOURCE)  $(DEP_TRANS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\iterator.cpp
DEP_ITERA=\
	.\object.h\
	.\iterator.h\
	.\archiver.h\
	.\defs.h\
	.\class.h\
	.\collectn.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\objarray.h\
	.\winclass.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\vwchar.h

$(INTDIR)/iterator.obj :  $(SOURCE)  $(DEP_ITERA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fileslct.cpp
DEP_FILES=\
	.\fileslct.h\
	.\notifier.h\
	.\pathstr.h\
	.\dialog.h\
	.\strm.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\ordcllct.h\
	.\result.h\
	.\poppwndw.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/fileslct.obj :  $(SOURCE)  $(DEP_FILES) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\about.cpp
DEP_ABOUT=\
	.\about.h\
	.\textbox.h\
	.\pushbttn.h\
	.\brush.h\
	.\shdctrl.h\
	.\dialog.h\
	.\control.h\
	.\button.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\shade.h\
	.\poppwndw.h\
	.\window.h\
	.\str.h\
	.\cvassert.h\
	.\winclass.h\
	.\defs.h\
	.\class.h\
	.\color.h\
	.\collectn.h\
	.\objarray.h\
	.\view.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\vwchar.h\
	.\callback.h\
	.\containr.h\
	.\set.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\popupmen.h\
	.\region.h\
	.\style.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\bitset.h

$(INTDIR)/about.obj :  $(SOURCE)  $(DEP_ABOUT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\textedtr.cpp
DEP_TEXTE=\
	.\winclass.h\
	.\textedtr.h\
	.\strm.h\
	.\popupmen.h\
	.\input.h\
	.\archiver.h\
	.\defs.h\
	.\editbox.h\
	.\str.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\dialog.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\textbox.h\
	.\object.h\
	.\vwchar.h\
	.\wintypes.h\
	.\display.h\
	.\collectn.h\
	.\poppwndw.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\objarray.h\
	.\control.h\
	.\class.h\
	.\color.h\
	.\view.h\
	.\result.h\
	.\containr.h\
	.\window.h\
	.\cvassert.h\
	.\callback.h\
	.\clrscm.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\region.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/textedtr.obj :  $(SOURCE)  $(DEP_TEXTE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\geocol.cpp
DEP_GEOCO=\
	.\geocol.h\
	.\archiver.h\
	.\georoc.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\geomgr.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\geoframe.h\
	.\iterator.h\
	.\str.h\
	.\result.h\
	.\object.h\
	.\containr.h\
	.\window.h\
	.\frame.h\
	.\vwchar.h\
	.\defs.h\
	.\class.h\
	.\attach.h\
	.\display.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\rect.h\
	.\ratio.h\
	.\callback.h\
	.\winclass.h\
	.\wintypes.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\region.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/geocol.obj :  $(SOURCE)  $(DEP_GEOCO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\textbox.cpp
DEP_TEXTB=\
	.\winclass.h\
	.\textbox.h\
	.\archiver.h\
	.\defs.h\
	.\control.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\window.h\
	.\str.h\
	.\cvassert.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\object.h\
	.\vwchar.h\
	.\result.h\
	.\containr.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\class.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\callback.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/textbox.obj :  $(SOURCE)  $(DEP_TEXTB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\statusbr.cpp
DEP_STATUS=\
	.\statusbr.h\
	.\control.h\
	.\port.h\
	.\brush.h\
	.\font.h\
	.\shade.h\
	.\attach.h\
	.\ordcllct.h\
	.\iterator.h\
	.\rect.h\
	.\stdmdiap.h\
	.\archiver.h\
	.\callback.h\
	.\textbox.h\
	.\str.h\
	.\window.h\
	.\cvassert.h\
	.\winclass.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\object.h\
	.\wintypes.h\
	.\clrscm.h\
	.\defs.h\
	.\color.h\
	.\collectn.h\
	.\objarray.h\
	.\region.h\
	.\mdiappvw.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\vwchar.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\class.h\
	.\containr.h\
	.\appview.h\
	.\mdiclnt.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\result.h\
	.\style.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\bitset.h

$(INTDIR)/statusbr.obj :  $(SOURCE)  $(DEP_STATUS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\oipuiwin.cpp
DEP_OIPUI=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/oipuiwin.obj :  $(SOURCE)  $(DEP_OIPUI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lockbyte.cpp
DEP_LOCKB=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/lockbyte.obj :  $(SOURCE)  $(DEP_LOCKB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ddeserv.cpp
DEP_DDESE=\
	.\winclass.h\
	.\ddeserv.h\
	.\atom.h\
	.\set.h\
	.\str.h\
	.\iterator.h\
	.\defs.h\
	.\ddecls.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\vwchar.h\
	.\window.h\
	.\containr.h\
	.\class.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/ddeserv.obj :  $(SOURCE)  $(DEP_DDESE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dropsrc.cpp
DEP_DROPS=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/dropsrc.obj :  $(SOURCE)  $(DEP_DROPS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bmapbttn.cpp
DEP_BMAPB=\
	.\bmapbttn.h\
	.\winclass.h\
	.\wintypes.h\
	.\notifier.h\
	.\port.h\
	.\appview.h\
	.\rscarch.h\
	.\bitmap.h\
	.\pushbttn.h\
	.\defs.h\
	.\color.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\archiver.h\
	.\blockfil.h\
	.\location.h\
	.\button.h\
	.\clrscm.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\control.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\strm.h\
	.\result.h\
	.\bfdiritm.h\
	.\cvassert.h\
	.\callback.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/bmapbttn.obj :  $(SOURCE)  $(DEP_BMAPB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\window.cpp
DEP_WINDO=\
	.\accel.h\
	.\brush.h\
	.\cvassert.h\
	.\event.h\
	.\geoframe.h\
	.\geomgr.h\
	.\geoform.h\
	.\georow.h\
	.\geocol.h\
	.\iterator.h\
	.\location.h\
	.\notifier.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\style.h\
	.\winclass.h\
	.\dialog.h\
	.\window.h\
	.\dictinry.h\
	.\intassoc.h\
	.\cmpwin.h\
	.\itemwin.h\
	.\archiver.h\
	.\brushinf.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\defs.h\
	.\frame.h\
	.\attach.h\
	.\georoc.h\
	.\collectn.h\
	.\appview.h\
	.\containr.h\
	.\global.h\
	.\timer.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\bitset.h\
	.\poppwndw.h\
	.\display.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\assoc.h\
	.\oledefs.h\
	.\unknown.h\
	.\droptarg.h\
	.\owindow.h\
	.\winrect.h\
	.\cmpobj.h\
	.\file.h\
	.\archvrtg.h\
	.\color.h\
	.\class.h\
	.\ratio.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\volestd.h\
	.\opoint.h\
	.\cmpstrg.h\
	.\strm.h\
	.\pathstr.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\storag.h\
	.\yesno.h\
	.\result.h\
	.\control.h\
	.\rstorag.h\
	.\olerslt.h

$(INTDIR)/window.obj :  $(SOURCE)  $(DEP_WINDO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rstorag.cpp
DEP_RSTOR=\
	.\pre.h\
	.\iterator.h\
	.\ordcllct.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/rstorag.obj :  $(SOURCE)  $(DEP_RSTOR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pushbttn.cpp
DEP_PUSHB=\
	.\pushbttn.h\
	.\notifier.h\
	.\archiver.h\
	.\button.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\control.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\cvassert.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\color.h\
	.\callback.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/pushbttn.obj :  $(SOURCE)  $(DEP_PUSHB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\imbutton.cpp
DEP_IMBUT=\
	.\imbutton.h\
	.\imbox.h\
	.\image.h\
	.\rect.h\
	.\clrscm.h\
	.\color.h\
	.\brush.h\
	.\pen.h\
	.\port.h\
	.\str.h\
	.\notifier.h\
	.\archiver.h\
	.\cllbckls.h\
	.\control.h\
	.\object.h\
	.\region.h\
	.\defs.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\display.h\
	.\bitbltr.h\
	.\font.h\
	.\vwchar.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\cvassert.h\
	.\class.h\
	.\objarray.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/imbutton.obj :  $(SOURCE)  $(DEP_IMBUT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\global.cpp
DEP_GLOBAL_=\
	.\global.h\
	.\defs.h\
	.\memry.h\
	.\set.h\
	.\winclass.h\
	.\wintypes.h\
	.\classtbl.h\
	.\cvassert.h\
	.\cvthread.h\
	.\object.h\
	.\memry16.h\
	.\collectn.h\
	.\objarray.h\
	.\color.h\
	.\class.h\
	.\containr.h\
	.\clrscm.h\
	.\callback.h

$(INTDIR)/global.obj :  $(SOURCE)  $(DEP_GLOBAL_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\editnot.cpp
DEP_EDITN=\
	.\editnot.h\
	.\str.h\
	.\notifier.h\
	.\editline.h\
	.\object.h\
	.\vwchar.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\editbox.h\
	.\class.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\textbox.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/editnot.obj :  $(SOURCE)  $(DEP_EDITN) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\inclgrp.cpp
DEP_INCLG=\
	.\inclgrp.h\
	.\checkbox.h\
	.\ordcllct.h\
	.\iterator.h\
	.\archiver.h\
	.\cllbckls.h\
	.\callback.h\
	.\bttngrp.h\
	.\button.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\global.h\
	.\winclass.h\
	.\defs.h\
	.\group.h\
	.\control.h\
	.\containr.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\window.h\
	.\str.h\
	.\cvassert.h\
	.\result.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\vwchar.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/inclgrp.obj :  $(SOURCE)  $(DEP_INCLG) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\picbttn.cpp
DEP_PICBT=\
	.\picbttn.h\
	.\winclass.h\
	.\wintypes.h\
	.\pushbttn.h\
	.\bitmap.h\
	.\defs.h\
	.\color.h\
	.\button.h\
	.\display.h\
	.\archiver.h\
	.\location.h\
	.\clrscm.h\
	.\control.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\window.h\
	.\str.h\
	.\cvassert.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\vwchar.h\
	.\callback.h\
	.\result.h\
	.\containr.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\region.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/picbttn.obj :  $(SOURCE)  $(DEP_PICBT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\exclgrp.cpp
DEP_EXCLG=\
	.\wintypes.h\
	.\exclgrp.h\
	.\radibttn.h\
	.\ordcllct.h\
	.\iterator.h\
	.\archiver.h\
	.\defs.h\
	.\color.h\
	.\inclgrp.h\
	.\checkbox.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\clrscm.h\
	.\bttngrp.h\
	.\button.h\
	.\containr.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\group.h\
	.\control.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\window.h\
	.\cvassert.h\
	.\winclass.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\region.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/exclgrp.obj :  $(SOURCE)  $(DEP_EXCLG) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bitbltr.cpp
DEP_BITBL=\
	.\winclass.h\
	.\bitbltr.h\
	.\bitmap.h\
	.\printer.h\
	.\notifier.h\
	.\defs.h\
	.\object.h\
	.\display.h\
	.\rect.h\
	.\archiver.h\
	.\location.h\
	.\str.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\window.h\
	.\class.h\
	.\wintypes.h\
	.\region.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\vwchar.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\objarray.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\color.h\
	.\strm.h\
	.\pathstr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\clrscm.h\
	.\result.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/bitbltr.obj :  $(SOURCE)  $(DEP_BITBL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\assoc.cpp
DEP_ASSOC=\
	.\assoc.h\
	.\archiver.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/assoc.obj :  $(SOURCE)  $(DEP_ASSOC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\object.cpp
DEP_OBJEC=\
	.\defs.h\
	.\winclass.h\
	.\object.h\
	.\archiver.h\
	.\str.h\
	.\notifier.h\
	.\class.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\vwchar.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\window.h\
	.\callback.h\
	.\strm.h\
	.\pathstr.h\
	.\assoc.h\
	.\objarray.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\result.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/object.obj :  $(SOURCE)  $(DEP_OBJEC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\notifier.cpp
DEP_NOTIF=\
	.\accel.h\
	.\appview.h\
	.\control.h\
	.\dialog.h\
	.\iterator.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\memry.h\
	.\menu.h\
	.\menuitem.h\
	.\popupmen.h\
	.\rect.h\
	.\scrollbr.h\
	.\editbox.h\
	.\stack.h\
	.\str.h\
	.\window.h\
	.\event.h\
	.\brush.h\
	.\mouscrsr.h\
	.\transwin.h\
	.\combobox.h\
	.\cvassert.h\
	.\notifier.h\
	.\pathstr.h\
	.\intassoc.h\
	.\shdctrl.h\
	.\utilwin.h\
	.\clrscm.h\
	.\object.h\
	.\view.h\
	.\icon.h\
	.\winclass.h\
	.\poppwndw.h\
	.\collectn.h\
	.\mdiclnt.h\
	.\memry16.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\display.h\
	.\menudata.h\
	.\region.h\
	.\textbox.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\defs.h\
	.\listbox.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\result.h\
	.\assoc.h\
	.\shade.h\
	.\geomgr.h\
	.\rband.h\
	.\class.h\
	.\sysmenu.h\
	.\color.h\
	.\objarray.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\geoframe.h\
	.\callback.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/notifier.obj :  $(SOURCE)  $(DEP_NOTIF) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\collectn.cpp
DEP_COLLE=\
	.\collectn.h\
	.\archiver.h\
	.\iterator.h\
	.\object.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\objarray.h\
	.\callback.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\winclass.h\
	.\vwchar.h

$(INTDIR)/collectn.obj :  $(SOURCE)  $(DEP_COLLE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\vwchar.cpp
DEP_VWCHA=\
	.\defs.h\
	.\vwchar.h\
	.\notifier.h\
	.\str.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\object.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\class.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\control.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/vwchar.obj :  $(SOURCE)  $(DEP_VWCHA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tablemdl.cpp
DEP_TABLEM=\
	.\tablemdl.h\
	.\tablevw.h\
	.\port.h\
	.\object.h\
	.\view.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\rect.h\
	.\font.h\
	.\defs.h\
	.\class.h\
	.\set.h\
	.\str.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\window.h\
	.\winclass.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\region.h\
	.\callback.h\
	.\collectn.h\
	.\objarray.h\
	.\vwchar.h\
	.\control.h\
	.\popupmen.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\color.h\
	.\containr.h\
	.\cvassert.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/tablemdl.obj :  $(SOURCE)  $(DEP_TABLEM) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\polygon.cpp
DEP_POLYG=\
	.\winclass.h\
	.\pointrry.h\
	.\polygon.h\
	.\defs.h\
	.\containr.h\
	.\location.h\
	.\region.h\
	.\object.h\
	.\wintypes.h\
	.\class.h\
	.\color.h\
	.\callback.h\
	.\clrscm.h

$(INTDIR)/polygon.obj :  $(SOURCE)  $(DEP_POLYG) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\frame.cpp
DEP_FRAME=\
	.\location.h\
	.\frame.h\
	.\archiver.h\
	.\window.h\
	.\object.h\
	.\rect.h\
	.\ratio.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\attach.h\
	.\display.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\defs.h\
	.\class.h\
	.\region.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\winclass.h\
	.\wintypes.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\str.h\
	.\callback.h\
	.\result.h\
	.\containr.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\vwchar.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/frame.obj :  $(SOURCE)  $(DEP_FRAME) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\xferagnt.cpp
DEP_XFERA=\
	.\pre.h\
	.\xferagnt.h\
	.\cvassert.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/xferagnt.obj :  $(SOURCE)  $(DEP_XFERA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\treenode.cpp
DEP_TREEN=\
	.\treenode.h\
	.\archiver.h\
	.\defs.h\
	.\str.h\
	.\color.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\vwchar.h\
	.\clrscm.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\class.h\
	.\result.h\
	.\containr.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/treenode.obj :  $(SOURCE)  $(DEP_TREEN) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\icon.cpp
DEP_ICON_=\
	.\icon.h\
	.\notifier.h\
	.\object.h\
	.\wintypes.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\class.h\
	.\color.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\clrscm.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/icon.obj :  $(SOURCE)  $(DEP_ICON_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tofmstrm.cpp
DEP_TOFMS=\
	.\tofmstrm.h\
	.\strm.h\
	.\str.h\
	.\object.h\
	.\vwchar.h\
	.\defs.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/tofmstrm.obj :  $(SOURCE)  $(DEP_TOFMS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\timer.cpp
DEP_TIMER=\
	.\winclass.h\
	.\timer.h\
	.\notifier.h\
	.\set.h\
	.\defs.h\
	.\object.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\str.h\
	.\window.h\
	.\objarray.h\
	.\class.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\color.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/timer.obj :  $(SOURCE)  $(DEP_TIMER) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\sysmenu.cpp
DEP_SYSME=\
	.\winclass.h\
	.\sysmenu.h\
	.\view.h\
	.\defs.h\
	.\popupmen.h\
	.\set.h\
	.\str.h\
	.\scrollbr.h\
	.\window.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\vwchar.h\
	.\control.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\wintypes.h\
	.\containr.h\
	.\class.h\
	.\cvassert.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\color.h\
	.\callback.h\
	.\region.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/sysmenu.obj :  $(SOURCE)  $(DEP_SYSME) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rootstrg.cpp
DEP_ROOTS=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/rootstrg.obj :  $(SOURCE)  $(DEP_ROOTS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\winstyle.cpp
DEP_WINST=\
	.\defs.h\
	.\archiver.h\
	.\bitset.h\
	.\style.h\
	.\exclstyl.h\
	.\implstyl.h\
	.\stylexpr.h\
	.\winstyle.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\cvassert.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\class.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\callback.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/winstyle.obj :  $(SOURCE)  $(DEP_WINST) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\vlocale.cpp
DEP_VLOCA=\
	.\vlocale.h\
	.\vwchar.h\
	.\str.h\
	.\dictinry.h\
	.\archiver.h\
	.\archvrtg.h\
	.\object.h\
	.\defs.h\
	.\set.h\
	.\assoc.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\class.h\
	.\collectn.h\
	.\objarray.h\
	.\strm.h\
	.\pathstr.h\
	.\callback.h\
	.\containr.h\
	.\result.h\
	.\winclass.h

$(INTDIR)/vlocale.obj :  $(SOURCE)  $(DEP_VLOCA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\oipsite.cpp
DEP_OIPSI=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/oipsite.obj :  $(SOURCE)  $(DEP_OIPSI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\intlcbtb.cpp
DEP_INTLC=\
	.\intlcbtb.h\
	.\iterator.h\
	.\str.h\
	.\file.h\
	.\archiver.h\
	.\rscarch.h\
	.\archvrtg.h\
	.\intntltb.h\
	.\object.h\
	.\collectn.h\
	.\vwchar.h\
	.\strm.h\
	.\pathstr.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\blockfil.h\
	.\assoc.h\
	.\multiset.h\
	.\vlocale.h\
	.\defs.h\
	.\class.h\
	.\result.h\
	.\set.h\
	.\objarray.h\
	.\bfdir.h\
	.\stack.h\
	.\callback.h\
	.\containr.h\
	.\bfdiritm.h\
	.\winclass.h

$(INTDIR)/intlcbtb.obj :  $(SOURCE)  $(DEP_INTLC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\addrmvls.cpp
DEP_ADDRM=\
	.\addrmvls.h\
	.\multlist.h\
	.\pushbttn.h\
	.\textbox.h\
	.\iterator.h\
	.\str.h\
	.\brush.h\
	.\shdctrl.h\
	.\dialog.h\
	.\ordcllct.h\
	.\listbox.h\
	.\button.h\
	.\control.h\
	.\object.h\
	.\collectn.h\
	.\vwchar.h\
	.\wintypes.h\
	.\clrscm.h\
	.\shade.h\
	.\poppwndw.h\
	.\objarray.h\
	.\window.h\
	.\cvassert.h\
	.\winclass.h\
	.\defs.h\
	.\class.h\
	.\color.h\
	.\view.h\
	.\containr.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\callback.h\
	.\set.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\popupmen.h\
	.\region.h\
	.\style.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\bitset.h

$(INTDIR)/addrmvls.obj :  $(SOURCE)  $(DEP_ADDRM) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\notepage.cpp
DEP_NOTEP=\
	.\notepage.h\
	.\archiver.h\
	.\object.h\
	.\str.h\
	.\window.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\winclass.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\containr.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/notepage.obj :  $(SOURCE)  $(DEP_NOTEP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\colrslct.cpp
DEP_COLRS=\
	.\colrslct.h\
	.\notifier.h\
	.\color.h\
	.\dialog.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\clrscm.h\
	.\poppwndw.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\control.h\
	.\callback.h\
	.\style.h\
	.\cvassert.h\
	.\bitset.h

$(INTDIR)/colrslct.obj :  $(SOURCE)  $(DEP_COLRS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\workpage.cpp
DEP_WORKP=\
	.\workpage.h\
	.\archiver.h\
	.\object.h\
	.\str.h\
	.\window.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\defs.h\
	.\class.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\winclass.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\containr.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\cvassert.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/workpage.obj :  $(SOURCE)  $(DEP_WORKP) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\oclsite.cpp
DEP_OCLSI=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/oclsite.obj :  $(SOURCE)  $(DEP_OCLSI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\editbox.cpp
DEP_EDITB=\
	.\winclass.h\
	.\wintypes.h\
	.\notifier.h\
	.\editbox.h\
	.\popupmen.h\
	.\archiver.h\
	.\cllbckls.h\
	.\defs.h\
	.\color.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\textbox.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\clrscm.h\
	.\view.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\control.h\
	.\strm.h\
	.\pathstr.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\cvassert.h\
	.\result.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/editbox.obj :  $(SOURCE)  $(DEP_EDITB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ratio.cpp
DEP_RATIO=\
	.\ratio.h\
	.\str.h\
	.\defs.h\
	.\object.h\
	.\vwchar.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/ratio.obj :  $(SOURCE)  $(DEP_RATIO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pre.cpp
DEP_PRE_C=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/pre.obj :  $(SOURCE)  $(DEP_PRE_C) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\poppwndw.cpp
DEP_POPPW=\
	.\winclass.h\
	.\poppwndw.h\
	.\archiver.h\
	.\defs.h\
	.\view.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\set.h\
	.\str.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\window.h\
	.\strm.h\
	.\pathstr.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\object.h\
	.\vwchar.h\
	.\control.h\
	.\popupmen.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\result.h\
	.\containr.h\
	.\class.h\
	.\cvassert.h\
	.\menu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\color.h\
	.\region.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/poppwndw.obj :  $(SOURCE)  $(DEP_POPPW) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpslct.cpp
DEP_CMPSL=\
	.\notifier.h\
	.\pre.h\
	.\cmpslct.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\dialog.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\volestd.h\
	.\class.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\wintypes.h\
	.\mdiclnt.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\location.h\
	.\poppwndw.h\
	.\menudata.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\color.h\
	.\bfdir.h\
	.\clrscm.h\
	.\style.h\
	.\bfdiritm.h\
	.\bitset.h

$(INTDIR)/cmpslct.obj :  $(SOURCE)  $(DEP_CMPSL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\archiver.cpp
DEP_ARCHI=\
	.\archiver.h\
	.\cvassert.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\memry.h\
	.\rscarch.h\
	.\blockfil.h\
	.\file.h\
	.\archvrtg.h\
	.\defs.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\memry16.h\
	.\object.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\strm.h\
	.\containr.h\
	.\global.h\
	.\class.h\
	.\str.h\
	.\result.h\
	.\bfdiritm.h\
	.\callback.h\
	.\vwchar.h\
	.\winclass.h

$(INTDIR)/archiver.obj :  $(SOURCE)  $(DEP_ARCHI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\savedlg.cpp
DEP_SAVED=\
	.\window.h\
	.\dialog.h\
	.\control.h\
	.\frame.h\
	.\rscarch.h\
	.\archiver.h\
	.\str.h\
	.\ordcllct.h\
	.\iterator.h\
	.\notifier.h\
	.\brush.h\
	.\geomgr.h\
	.\attach.h\
	.\display.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\poppwndw.h\
	.\cvassert.h\
	.\winclass.h\
	.\rect.h\
	.\ratio.h\
	.\blockfil.h\
	.\file.h\
	.\dictinry.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\object.h\
	.\vwchar.h\
	.\collectn.h\
	.\objarray.h\
	.\defs.h\
	.\appview.h\
	.\containr.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\wintypes.h\
	.\clrscm.h\
	.\geoframe.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\view.h\
	.\region.h\
	.\pathstr.h\
	.\bfdir.h\
	.\stack.h\
	.\strm.h\
	.\assoc.h\
	.\class.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\color.h\
	.\style.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\result.h\
	.\bfdiritm.h\
	.\callback.h\
	.\menudata.h\
	.\menuitem.h\
	.\bitset.h

$(INTDIR)/savedlg.obj :  $(SOURCE)  $(DEP_SAVED) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\menu.cpp
DEP_MENU_=\
	.\notifier.h\
	.\menu.h\
	.\popupmen.h\
	.\appview.h\
	.\ordcllct.h\
	.\menuitem.h\
	.\window.h\
	.\iterator.h\
	.\set.h\
	.\archiver.h\
	.\defs.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\wintypes.h\
	.\menudata.h\
	.\view.h\
	.\icon.h\
	.\objarray.h\
	.\display.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\object.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\assoc.h\
	.\region.h\
	.\vwchar.h\
	.\color.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\class.h\
	.\strm.h\
	.\pathstr.h\
	.\clrscm.h\
	.\control.h\
	.\style.h\
	.\cvassert.h\
	.\callback.h\
	.\result.h\
	.\bitset.h

$(INTDIR)/menu.obj :  $(SOURCE)  $(DEP_MENU_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmpobj.cpp
DEP_CMPOB=\
	.\pre.h\
	.\oledefs.h\
	.\object.h\
	.\archiver.h\
	.\file.h\
	.\cvassert.h\
	.\menuitem.h\
	.\mdiappvw.h\
	.\mdiview.h\
	.\notifier.h\
	.\pathstr.h\
	.\report.h\
	.\result.h\
	.\rscarch.h\
	.\statusbr.h\
	.\stack.h\
	.\stdmdiap.h\
	.\view.h\
	.\window.h\
	.\yesno.h\
	.\opoint.h\
	.\winrect.h\
	.\debug.h\
	.\olerslt.h\
	.\unknown.h\
	.\owindow.h\
	.\lockbyte.h\
	.\oipsite.h\
	.\oipuiwin.h\
	.\oipframe.h\
	.\oipappvw.h\
	.\oclsite.h\
	.\oobject.h\
	.\advsink.h\
	.\droptarg.h\
	.\dropsrc.h\
	.\xferagnt.h\
	.\rstorag.h\
	.\rootstrg.h\
	.\substrg.h\
	.\storag.h\
	.\cmpstrg.h\
	.\cmpobj.h\
	.\cmpwin.h\
	.\cmpview.h\
	.\cmpappvw.h\
	.\itemwin.h\
	.\cmpitem.h\
	.\voleui.h\
	.\defs.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\strm.h\
	.\display.h\
	.\str.h\
	.\wintypes.h\
	.\appview.h\
	.\mdiclnt.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\timer.h\
	.\winclass.h\
	.\dialog.h\
	.\blockfil.h\
	.\control.h\
	.\shade.h\
	.\textbox.h\
	.\brush.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\location.h\
	.\menu.h\
	.\callback.h\
	.\assoc.h\
	.\objarray.h\
	.\color.h\
	.\popupmen.h\
	.\icon.h\
	.\region.h\
	.\poppwndw.h\
	.\bfdir.h\
	.\clrscm.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\menudata.h\
	.\iterator.h\
	.\bfdiritm.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/cmpobj.obj :  $(SOURCE)  $(DEP_CMPOB) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rndrect.cpp
DEP_RNDRE=\
	.\winclass.h\
	.\rndrect.h\
	.\port.h\
	.\defs.h\
	.\rect.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\font.h\
	.\region.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\class.h\
	.\color.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\containr.h

$(INTDIR)/rndrect.obj :  $(SOURCE)  $(DEP_RNDRE) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\money.cpp
DEP_MONEY=\
	.\defs.h\
	.\cvassert.h\
	.\money.h\
	.\oledefs.h\
	.\object.h\
	.\volestd.h\
	.\vwchar.h\
	.\class.h\
	.\callback.h\
	.\winclass.h

$(INTDIR)/money.obj :  $(SOURCE)  $(DEP_MONEY) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\drag.cpp
DEP_DRAG_=\
	.\notifier.h\
	.\port.h\
	.\pen.h\
	.\rect.h\
	.\poppwndw.h\
	.\dictinry.h\
	.\iterator.h\
	.\mouscrsr.h\
	.\drag.h\
	.\defs.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\global.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\winclass.h\
	.\window.h\
	.\display.h\
	.\bitbltr.h\
	.\brush.h\
	.\font.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\region.h\
	.\view.h\
	.\assoc.h\
	.\archiver.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\frame.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\class.h\
	.\color.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\menudata.h\
	.\menuitem.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\callback.h\
	.\control.h\
	.\strm.h\
	.\pathstr.h\
	.\style.h\
	.\cvassert.h\
	.\result.h\
	.\bitset.h

$(INTDIR)/drag.obj :  $(SOURCE)  $(DEP_DRAG_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\display.cpp
DEP_DISPL=\
	.\winclass.h\
	.\display.h\
	.\archiver.h\
	.\defs.h\
	.\object.h\
	.\wintypes.h\
	.\file.h\
	.\dictinry.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\class.h\
	.\color.h\
	.\strm.h\
	.\pathstr.h\
	.\set.h\
	.\assoc.h\
	.\collectn.h\
	.\objarray.h\
	.\callback.h\
	.\clrscm.h\
	.\str.h\
	.\result.h\
	.\containr.h\
	.\vwchar.h

$(INTDIR)/display.obj :  $(SOURCE)  $(DEP_DISPL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\scrollbr.cpp
DEP_SCROL=\
	.\winclass.h\
	.\scrollbr.h\
	.\notifier.h\
	.\archiver.h\
	.\cllbckls.h\
	.\defs.h\
	.\control.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\rect.h\
	.\set.h\
	.\str.h\
	.\timer.h\
	.\window.h\
	.\file.h\
	.\ordcllct.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\callback.h\
	.\cvassert.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\object.h\
	.\assoc.h\
	.\region.h\
	.\objarray.h\
	.\vwchar.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\sysmenu.h\
	.\wintypes.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\class.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\color.h\
	.\style.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/scrollbr.obj :  $(SOURCE)  $(DEP_SCROL) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ddeclnt.cpp
DEP_DDECLN=\
	.\winclass.h\
	.\ddeclnt.h\
	.\atom.h\
	.\defs.h\
	.\ddecls.h\
	.\window.h\
	.\attach.h\
	.\display.h\
	.\frame.h\
	.\mouscrsr.h\
	.\ordcllct.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\font.h\
	.\fontinfo.h\
	.\object.h\
	.\wintypes.h\
	.\rect.h\
	.\ratio.h\
	.\collectn.h\
	.\objarray.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\str.h\
	.\class.h\
	.\color.h\
	.\region.h\
	.\containr.h\
	.\style.h\
	.\cvassert.h\
	.\vwchar.h\
	.\callback.h\
	.\clrscm.h\
	.\bitset.h

$(INTDIR)/ddeclnt.obj :  $(SOURCE)  $(DEP_DDECLN) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\combobox.cpp
DEP_COMBO=\
	.\winclass.h\
	.\combobox.h\
	.\brush.h\
	.\port.h\
	.\str.h\
	.\notifier.h\
	.\cllbckls.h\
	.\archiver.h\
	.\defs.h\
	.\listbox.h\
	.\object.h\
	.\wintypes.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\rect.h\
	.\font.h\
	.\vwchar.h\
	.\appview.h\
	.\collectn.h\
	.\containr.h\
	.\dictinry.h\
	.\global.h\
	.\set.h\
	.\timer.h\
	.\window.h\
	.\callback.h\
	.\file.h\
	.\intassoc.h\
	.\archvrtg.h\
	.\control.h\
	.\class.h\
	.\color.h\
	.\objarray.h\
	.\region.h\
	.\view.h\
	.\menu.h\
	.\popupmen.h\
	.\icon.h\
	.\assoc.h\
	.\attach.h\
	.\frame.h\
	.\mouscrsr.h\
	.\wininfo.h\
	.\implstyl.h\
	.\styles.h\
	.\winstyle.h\
	.\fontinfo.h\
	.\strm.h\
	.\pathstr.h\
	.\cvassert.h\
	.\scrollbr.h\
	.\sysmenu.h\
	.\menudata.h\
	.\menuitem.h\
	.\iterator.h\
	.\ratio.h\
	.\exclstyl.h\
	.\stylexpr.h\
	.\result.h\
	.\style.h\
	.\bitset.h

$(INTDIR)/combobox.obj :  $(SOURCE)  $(DEP_COMBO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\region.cpp
DEP_REGIO=\
	.\winclass.h\
	.\region.h\
	.\port.h\
	.\defs.h\
	.\wintypes.h\
	.\object.h\
	.\display.h\
	.\bitbltr.h\
	.\pen.h\
	.\brush.h\
	.\rect.h\
	.\font.h\
	.\color.h\
	.\class.h\
	.\ordcllct.h\
	.\clrscm.h\
	.\callback.h\
	.\collectn.h\
	.\objarray.h\
	.\containr.h

$(INTDIR)/region.obj :  $(SOURCE)  $(DEP_REGIO) $(INTDIR)

# End Source File
# End Group
################################################################################
