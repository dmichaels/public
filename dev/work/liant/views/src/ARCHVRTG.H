// $Header:   Q:/views/common/vcs/ARCHVRTG.H_V   1.28   Mar 24 1997 16:08:00   scottt  $

//  archvrtg.h
//
//  VArchiverTag interface [Common]
//
//  Allegris Foundation 1.1.00
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
//  09/22/93 pmcm   original.
//  09/23/93 pmcm   added VSashTag
//  01/13/94 dgm    merged and preened
//  02/02/94 pmcm   inlined get*() stuff; added archiveTagAbortCheck()
//  02/09/94 pmcm   removed class specific tag values in favor of magic number
//  08/27/94 pkt    updated for Metrowerks C++.
//  08/31/94 dgm    Added defineArchiveRevision.
//  10/26/94 dgm    Fixed defineArchiveRevision.
//  11/11/94 dss    Archiver tags macros.
//  03/06/95 jld    added #ifndef CV_NOARCHIVER wrapper
//  05/19/95 pkt    merged for common source.
//  09/26/95 dss    Replaced CV_ARCHTAG305 with CV_ARCHTAG. VWindow tag = 2.
//  10/04/95 evc    VWindow tag = 3 to allow archiving of new window font
//                  Also VControl tag change
//	10/25/95 evc	Added VMenuItem and VPopup and VMenu tag changes
//	11/10/95 dgm	VWindow tag = 4: VGeoFrame/Manager archiving support.
//	12/29/95 evc	Added CV_VPOPUPMENU_TAG_REV = 3 for bChecked
//	12/29/95 tlf	VFontInfo tag = 1
//	05/01/96 glm	CV_VVIEW_TAG_REV=2 and added VTreeListBox
//	05/10/96 dgm	VWindow tag = 6 for OLE/NOOLE VRF compatibility.
//	06/12/96 pkt	added #include "archiver.h".
//	11/27/96 tlf    updated tag for VWindow to 7.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VARCHVRTG_H
#define VARCHVRTG_H

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
// *****************
// *** ATTENTION ***
// *****************
//
// These CV_classname_TAG_REV definitions should be updated to coincide
// with the value specified in the most up-to-date version of the class
// implementation, but these definitions are currently only for documentation
// purposes, i.e. to find out what the latest class revision is.  The actual
// setting of the class revision tag (i.e. the one which will be written,
// read, and queried at run-time) is done using the defineArchiveRevision()
// macro within the the class implementation file for each class.
//
// N.B. Archives written for a particular version of the library can only be
// read by that version, or a later version, of the library.  However, newer
// versions of Views will always be able to read older archive versions.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define CV_VAccelerator_TAG_REV				0
#define CV_VAppView_TAG_REV					0
#define CV_VBitMap_TAG_REV					1
#define CV_VBrushInfo_TAG_REV				0
#define CV_VButtonGroup_TAG_REV				0
#define CV_VButton_TAG_REV					0
#define CV_VCheckBox_TAG_REV				0
#define CV_VComboBox_TAG_REV				2
#define CV_VControl_TAG_REV					2
#define CV_VDialog_TAG_REV					1
#define CV_VDisplay_TAG_REV					0
#define CV_VDockableToolBar_TAG_REV			1
#define CV_VDockableWindow_TAG_REV			1
#define CV_VDockSiteWindow_TAG_REV			1
#define CV_VEditBox_TAG_REV					1
#define CV_VEditLine_TAG_REV				0
#define CV_VExclusiveGroup_TAG_REV			0
#define CV_VFontInfo_TAG_REV				1
#define CV_VGeoColumn_TAG_REV				0
#define CV_VGeoFrame_TAG_REV				0
#define CV_VGeoManager_TAG_REV				1
#define CV_VGeoRow_TAG_REV					0
#define CV_VGeoRowOrColumn_TAG_REV			1
#define CV_VGroup_TAG_REV					0
#define CV_VIconButton_TAG_REV				0
#define CV_VImage_TAG_REV					0
#define CV_VImageBox_TAG_REV				0
#define CV_VImageButton_TAG_REV				0
#define CV_VInclusiveGroup_TAG_REV			0
#define CV_VInternationalTable_TAG_REV		0
#define CV_VInternationalComboTable_TAG_REV	0
#define CV_VListBox_TAG_REV					1
#define CV_VLocale_TAG_REV					1
#define CV_VMdiAppView_TAG_REV				0
#define CV_VMdiView_TAG_REV					0
#define CV_VMenuItem_TAG_REV				1
#define CV_VMenu_TAG_REV					1
#define CV_VMultiListBox_TAG_REV			0
#define CV_VMultiSet_TAG_REV				0
#define CV_VNoteBook_TAG_REV				0
#define CV_VNotePage_TAG_REV				0
#define CV_VPenInfo_TAG_REV					0
#define CV_VPopupMenu_TAG_REV				3
#define CV_VPopupWindow_TAG_REV				0
#define CV_VPushButton_TAG_REV				0
#define CV_VRadioButton_TAG_REV				1
#define CV_VSash_TAG_REV					0
#define CV_VScrollBar_TAG_REV				0
#define CV_VShadeControl_TAG_REV			1
#define CV_VStatusBar_TAG_REV				0
#define CV_VStdAppView_TAG_REV				0
#define CV_VStdMdiAppView_TAG_REV			0
#define CV_VStdMdiView_TAG_REV				0
#define CV_VStringTable_TAG_REV				1
#define CV_VTextBox_TAG_REV					0
#define CV_VTextEditor_TAG_REV				0
#define CV_VToolBar_TAG_REV					1
#define CV_VTreeListBox_TAG_REV				1
#define CV_VTriState_TAG_REV				0
#define CV_VUserControl_TAG_REV				1
#define CV_VUtilityWindow_TAG_REV			1
#define CV_VView_TAG_REV					2
#define CV_VVirtualListBox_TAG_REV			0
#define CV_VWindow_TAG_REV					7
#define CV_VWorkBook_TAG_REV				0
#define CV_VWorkPage_TAG_REV				0

#define CV_VACCELERATOR_TAG_REV				CV_VAccelerator_TAG_REV
#define CV_VAPPVIEW_TAG_REV					CV_VAppView_TAG_REV
#define CV_VBITMAP_TAG_REV					CV_VBitMap_TAG_REV
#define CV_VBRUSHINFO_TAG_REV				CV_VBrushInfo_TAG_REV
#define CV_VBUTTONGROUP_TAG_REV				CV_VButtonGroup_TAG_REV
#define CV_VBUTTON_TAG_REV					CV_VButton_TAG_REV
#define CV_VCHECKBOX_TAG_REV				CV_VCheckBox_TAG_REV
#define CV_VCOMBOBOX_TAG_REV				CV_VComboBox_TAG_REV
#define CV_VCONTROL_TAG_REV					CV_VControl_TAG_REV
#define CV_VDIALOG_TAG_REV					CV_VDialog_TAG_REV
#define CV_VDISPLAY_TAG_REV					CV_VDisplay_TAG_REV
#define CV_VDOCKABLETOOLBAR_TAG_REV			CV_VDockableToolBar_TAG_REV
#define CV_VDOCKABLEWINDOW_TAG_REV			CV_VDockableWindow_TAG_REV
#define CV_VDOCKSITEWINDOW_TAG_REV			CV_VDockSiteWindow_TAG_REV
#define CV_VEDITBOX_TAG_REV					CV_VEditBox_TAG_REV
#define CV_VEDITLINE_TAG_REV				CV_VEditLine_TAG_REV
#define CV_VEXCLUSIVEGROUP_TAG_REV			CV_VExclusiveGroup_TAG_REV
#define CV_VFONTINFO_TAG_REV				CV_VFontInfo_TAG_REV
#define CV_VGEOCOLUMN_TAG_REV				CV_VGeoColumn_TAG_REV
#define CV_VGEOFRAME_TAG_REV				CV_VGeoFrame_TAG_REV
#define CV_VGEOMANAGER_TAG_REV				CV_VGeoManager_TAG_REV
#define CV_VGEOROW_TAG_REV					CV_VGeoRow_TAG_REV
#define CV_VGEOROWORCOLUMN_TAG_REV			CV_VGeoRowOrColumn_TAG_REV
#define CV_VGROUP_TAG_REV					CV_VGroup_TAG_REV
#define CV_VICONBUTTON_TAG_REV				CV_VIconButton_TAG_REV
#define CV_VIMAGE_TAG_REV					CV_VImage_TAG_REV
#define CV_VIMAGEBOX_TAG_REV				CV_VImageBox_TAG_REV
#define CV_VIMAGEBUTTON_TAG_REV				CV_VImageButton_TAG_REV
#define CV_VINCLUSIVEGROUP_TAG_REV			CV_VInclusiveGroup_TAG_REV
#define CV_VINTERNATIONALTABLE_TAG_REV		CV_VInternationalTable_TAG_REV
#define CV_VINTERNATIONALCOMBOTABLE_TAG_REV	CV_VInternationalComboTable_TAG_REV
#define CV_VLISTBOX_TAG_REV					CV_VListBox_TAG_REV
#define CV_VLOCALE_TAG_REV					CV_VLocale_TAG_REV
#define CV_VMDIAPPVIEW_TAG_REV				CV_VMdiAppView_TAG_REV
#define CV_VMDIVIEW_TAG_REV					CV_VMdiView_TAG_REV
#define CV_VMENUITEM_TAG_REV				CV_VMenuItem_TAG_REV
#define CV_VMENU_TAG_REV					CV_VMenu_TAG_REV
#define CV_VMULTILISTBOX_TAG_REV			CV_VMultiListBox_TAG_REV
#define CV_VMULTISET_TAG_REV				CV_VMultiSet_TAG_REV
#define CV_VNOTEBOOK_TAG_REV				CV_VNoteBook_TAG_REV
#define CV_VNOTEPAGE_TAG_REV				CV_VNotePage_TAG_REV
#define CV_VPENINFO_TAG_REV					CV_VPenInfo_TAG_REV
#define CV_VPOPUPMENU_TAG_REV				CV_VPopupMenu_TAG_REV
#define CV_VPOPUPWINDOW_TAG_REV				CV_VPopupWindow_TAG_REV
#define CV_VPUSHBUTTON_TAG_REV				CV_VPushButton_TAG_REV
#define CV_VRADIOBUTTON_TAG_REV				CV_VRadioButton_TAG_REV
#define CV_VSASH_TAG_REV					CV_VSash_TAG_REV
#define CV_VSCROLLBAR_TAG_REV				CV_VScrollBar_TAG_REV
#define CV_VSHADECONTROL_TAG_REV			CV_VShadeControl_TAG_REV
#define CV_VSTATUSBAR_TAG_REV				CV_VStatusBar_TAG_REV
#define CV_VSTDAPPVIEW_TAG_REV				CV_VStdAppView_TAG_REV
#define CV_VSTDMDIAPPVIEW_TAG_REV			CV_VStdMdiAppView_TAG_REV
#define CV_VSTDMDIVIEW_TAG_REV				CV_VStdMdiView_TAG_REV
#define CV_VSTRINGTABLE_TAG_REV				CV_VStringTable_TAG_REV
#define CV_VTEXTBOX_TAG_REV					CV_VTextBox_TAG_REV
#define CV_VTEXTEDITOR_TAG_REV				CV_VTextEditor_TAG_REV
#define CV_VTOOLBAR_TAG_REV					CV_VToolBar_TAG_REV
#define CV_VTREELISTBOX_TAG_REV				CV_VTreeListBox_TAG_REV
#define CV_VTRISTATE_TAG_REV				CV_VTriState_TAG_REV
#define CV_VUSERCONTROL_TAG_REV				CV_VUserControl_TAG_REV
#define CV_VUTILITYWINDOW_TAG_REV			CV_VUtilityWindow_TAG_REV
#define CV_VVIEW_TAG_REV					CV_VView_TAG_REV
#define CV_VVIRTUALLISTBOX_TAG_REV			CV_VVirtualListBox_TAG_REV
#define CV_VWINDOW_TAG_REV					CV_VWindow_TAG_REV
#define CV_VWORKBOOK_TAG_REV				CV_VWorkBook_TAG_REV
#define CV_VWORKPAGE_TAG_REV				CV_VWorkPage_TAG_REV

#ifndef CV_NOARCHIVER

#include "archiver.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// It is useful for a class to include some identifier or tag value with the 
// object data it archives.  When unarchiving object data, a class can use 
// the tag value to confirm that the archiver is positioned at the 
// appropriate spot in the archive file.  It can also use the id or tag to
// specify a particular revision of the object data format.  That way,
// when you need to change the order or contents of the class object data 
// that you archive, you can still support the reading of older archives.
//  
// defineArchiveTag() is a convenience macro for making up an id tag for
// GUI objects to use when saving and retrieving from a VArchiver.
// Here, an archive tag is a long integer composed of two parts: 1) a
// magic number; and 2) a byte that indicates a revision number.
//
#define CV_ArchiveTagMagicNumber 0x0ABC00

#if !defined(CVP_OLD_CPP)

#define defineArchiveTag(c, r)                  \
    const long c ## ArchiveTag =                \
    ((long(CV_ArchiveTagMagicNumber)) |			\
	 (long(((unsigned char)(r)))));

#define defineArchiveRevision(c, r)             \
    const int c##TagRev = r;                    \
    const long c##ArchiveTag =                  \
    ((long(CV_ArchiveTagMagicNumber)) |         \
     (long(((unsigned char)(c##TagRev)))));

#else

#define defineArchiveTag(c, r)					\
    const long c/**/ArchiveTag =				\
    ((long(CV_ArchiveTagMagicNumber)) |			\
	 (long(((unsigned char)(r)))));

#define defineArchiveRevision(c, r)             \
    const int c/**/TagRev = r;                  \
    const long c/**/ArchiveTag =                \
    ((long(CV_ArchiveTagMagicNumber)) |         \
     (long(((unsigned char)(c/**/TagRev)))));

#endif

extern VArchiver::AbortReason archiveTagAbortCheck(const long archTag, 
                                                   const long objTag);

inline int getArchiveTagRev(const long tag)
{
    return (int)((unsigned char) (tag & 0x0FF));
}

inline boolean archiveTagOk(const long archTag, const long objTag)  
{
    return (archiveTagAbortCheck(archTag, objTag) == VArchiver::NoAbort);
}

#else

#ifdef defineArchiveTag
#undef defineArchiveTag
#endif
#ifdef defineArchiveRevision
#undef defineArchiveRevision
#endif

#define defineArchiveTag(c, r)
#define defineArchiveRevision(c, r)

#endif

#endif // VARCHVRTG_H
