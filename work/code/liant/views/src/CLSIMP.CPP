// $Header:   Q:/views/win/vcs/clsimp.cpv   1.19   Mar 04 1997 17:16:48   gregm  $ 

//  clsimp.cpp
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  11/03/92 jmd    added table, toolbar, and bitmap view classes
//
//  08/12/93 pat    added VMouseCursor; removed VNotifier; changed
//                  MultiList to VMultiListBox; alphabetized.
//  08/19/93 pat    added CPPV_GUI_DLL for new 2-DLLs scheme
//  08/25/93 pat    added importCallback
//  09/01/93 pat    added VTransparentWindow, VSash, VBitMapButton,
//                  VShadeControl, VStdMdiAppViewCls, VStdMdiView
//  09/15/93 pat    added VBrushInfo, VFontInfo, and VPenInfo
//                  VCallback, VCallbackList
//
//  11/31/93 pat    added VUserControl
//  02/02/94 pat    included "object.h" 'cuz "class.h" inlined class id macros.
//  07/16/94 jld    added VPopPrompt
//
//  03/03/95 jld    removed VPopPrompt, added VStatusBar
//  04/24/95 jld    added VArc, VAttach, VBlockFile, VBlockFileDir,
//					VBlockFileDirItem, VColor, VMenuData,
//                  VRscArchiver, VShade, VStdAppView
//  09/15/95 dss    VStyle additions.
//  11/06/95 pkt    added VInternationalTable, VStringTable, and
//                  VInternationalComboTable.
//  11/20/95 dss    Added OLE classes.
//  01/09/96 dss    VComponent name change.
//  01/26/96 dss    Added VComponentProgrammable classes.
//  01/25/97 dgm    Added new classes.
// ----------------------------------------------------------------------------

#include "defs.h"
#include "object.h"


// all the views classes
importClass(CUnknown)
importClass(VAbout)
importClass(VAccelerator)
importClass(VAddRemoveList)
importClass(VAppView)
importClass(VArc)
importClass(VArchiver)
importClass(VAssoc)
importClass(VBlockFileDirItem)
importClass(VBitBltr)
importClass(VBitMap)
importClass(VBitMapView)
importClass(VBitSet)
importClass(VBlockContext)
importClass(VBlockFile)
importClass(VBitMapButton)
importClass(VBool)
importClass(VBrush)
importClass(VBrushInfo)
importClass(VButtonGroup)
importClass(VButton)
importClass(VCheckBox)
importClass(VClassTable)
importClass(VClipBoard)
importClass(VCallbackList)
importClass(VCollection)
importClass(VControlHelp)
importClass(VControlView)
importClass(VComboBox)
importClass(VContainer)
importClass(VControl)
importClass(VDateTime)
importClass(VDdeClient)
importClass(VDde)
importClass(VDdeServer)
importClass(VDialog)
importClass(VDictionary)
importClass(VDisplay)
importClass(VDockableSiteWindow)
importClass(VDockableToolBar)
importClass(VDockableWindow)
importClass(VDropSite)
importClass(VDragSite)
importClass(VDragManager)
importClass(VDragManagerRep)
importClass(VDragManagerView)
importClass(VDST)
importClass(VGeoColumn)
importClass(VGeoForm)
importClass(VGeoFrame)
importClass(VGeoManager)
importClass(VGeoRowOrColumn)
importClass(VGeoRow)
importClass(VHandlerPair)
importClass(VEditBox)
importClass(VEditLine)
importClass(VEllipse)
importClass(VExclusiveGroup)
importClass(VExclusiveStyle)
importClass(VFile)
importClass(VFileSelect)
importClass(VFont)
importClass(VFontInfo)
importClass(VFontSelect)
importClass(VFrame)
importClass(VGlobal)
importClass(VGroup)
importClass(VGroupBox)
importClass(VIcon)
importClass(VIconButton)
importClass(VImplicitStyle)
importClass(VInclusiveGroup)
importClass(VInput)
importClass(VIntAssoc)
importClass(VIntegerSet)
importClass(VInternationalComboTable)
importClass(VInternationalTable)
importClass(VIterator)
importClass(VListBox)
importClass(VListSelect)
importClass(VLocation)
importClass(VMdiAppView)
importClass(VMdiClient)
importClass(VMdiView)
importClass(VMemory)
importClass(VMenu)
importClass(VMenuItem)
importClass(VMouseCursor)
importClass(VMultiListBox)
importClass(VMultiSelect)
importClass(VNoteBook)
importClass(VNotePage)
importClass(VNotifier)
importClass(VObject)
importClass(VObjectCourier)
importClass(VObjectType)
importClass(VObjArray)
importClass(VOrdCollect)
importClass(VPallette)
importClass(VPanner)
importClass(VPathString)
importClass(VPen)
importClass(VPenInfo)
importClass(VPicButton)
importClass(VPointArray)
importClass(VPolygon)
importClass(VPopPrompt)
importClass(VPopupWindow)
importClass(VPopupMenu)
importClass(VPort)
importClass(VPrintAbort)
importClass(VPrintDialog)
importClass(VPrinter)
importClass(VPushButton)
importClass(VRadioButton)
importClass(VRectangle)
importClass(VRegion)
importClass(VReport)
importClass(VRoundRect)
importClass(VRscArchiver)
importClass(VSash)
importClass(VScrollBar)
importClass(VScrollWindow)
importClass(VSeparator)
importClass(VSerial)
importClass(VSet)
importClass(VShadeControl)
importClass(VStack)
importClass(VStatusBar)
importClass(VStdAppView)
importClass(VStdMdiAppView)
importClass(VStdMdiView)
importClass(VString)
importClass(VStringTable)
importClass(VStream)
importClass(VStyle)
importClass(VSysMenu)
importClass(VTableModel)
importClass(VTableSubView)
importClass(VTableView)
importClass(VTagStream)
importClass(VTextBox)
importClass(VTextEditor)
importClass(VTimer)
importClass(VTimeZone)
importClass(VToFromStream)
importClass(VTokens)
importClass(VTokenStream)
importClass(VToolBar)
importClass(VTransparentWindow)
importClass(VTree)
importClass(VTreeNode)
importClass(VTreeListBox)
importClass(VTriState)
importClass(VUserControl)
importClass(VUtilityWindow)
importClass(VView)
importClass(VVirtualListBox)
importClass(VWindow)
importClass(VWinStyle)
importClass(VWorkBook)
importClass(VWorkPage)

#if !defined(CV_NOOLE)
importClass(VArray)
importClass(VComponentAppView)
importClass(VComponentItem)
importClass(VComponentObject)
importClass(VComponentProgrammable)
importClass(VComponentStorage)
importClass(VComponentView)
importClass(VComponentWindow)
importClass(VComponentControl)
importClass(VItemWindow)
importClass(VMethodData)
importClass(VMoney)
importClass(VOlePoint)
importClass(VOlePosition)
importClass(VOleSize)
importClass(VStatus)
importClass(VVariable)
importClass(VWinRect)
#endif

// ---------------------------------------------------------------------------
//  Import Allegris DLL callbacks
//
#ifndef IN_DLL
#ifdef USE_DLL

#include "callback.h"

importCallback(VAppView, closeApp)
importCallback(VDialog, okPressed)
importCallback(VDialog, cancelPressed)
importCallback(VInclusiveGroup, clicked)

#endif  
#endif  //  imports of DLL callbacks

// ---------------------------------------------------------------------------
