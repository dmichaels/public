// $Header:   Q:/views/win/vcs/groupbox.cpv   1.14   25 Feb 1997 10:24:20   tomf  $
//
// groupbox.cpp
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
//	Revision History:
//	-----------------
//
//	06/27/94 gjl	created
//	02/20/96 dgm	Update for Motif.
//	04/08/96 dgm	Fixed takeFocus() hiding compiler warning.
//	05/14/96 dgm	Motif support.
//	05/16/96 dgm	New constructors and fix for constructor putText() call.
//	07/19096 tlf	Added getText(VString &) member function.
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Include Files
// --------------------------------------------------------------------------

#include "winclass.h"
#include "groupbox.h"
#include "str.h"
#include "notifier.h"
#include "brush.h"

#if defined(CV_MOTIF)
#include "motstr.h"
#include "brush.h"
#include "font.h"
#include "xsetdefs.h"
#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include <Xm/Frame.h>
#include <Xm/LabelG.h>
#include <Xm/Form.h>
#include "xclrdefs.h"
#endif

// ---------------------------------------------------------------------------
//
defineClass(VGroupBox, VControl) 

// ---------------------------------------------------------------------------
// 
VGroupBox::VGroupBox()
	: VControl()
{
}

// ---------------------------------------------------------------------------
//
VGroupBox::VGroupBox(VWindow *parentwin, const char *text)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, StyleDefault);
	if (text != 0) { VGroupBox::putText(text); }
}

// ---------------------------------------------------------------------------
//
VGroupBox::VGroupBox(const VFrame& frame,
					 VWindow *parentwin, const char *text)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, StyleDefault);
	if (text != 0) { VGroupBox::putText(text); }
}

// ---------------------------------------------------------------------------
//
VGroupBox::VGroupBox(VWindow *parentwin, const VStyle& style, const char *text)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
	if (text != 0) { VGroupBox::putText(text); }
}

// ---------------------------------------------------------------------------
//
VGroupBox::VGroupBox(const VFrame& frame, VWindow *parentwin,
					 const VStyle& style, const char *text)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, style);
	if (text != 0) { VGroupBox::putText(text); }
}

// ---------------------------------------------------------------------------
//
VGroupBox::VGroupBox(const VString& name,
					 const VFrame& frame, VWindow *parentwin,
					 const VStyle& style, const char *text)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, style, name);
	if (text != 0) { VGroupBox::putText(text); }
}

// ---------------------------------------------------------------------------
//
VGroupBox::~VGroupBox()
{
}

// ---------------------------------------------------------------------------
//
void VGroupBox::operator()(const VFrame& frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// ---------------------------------------------------------------------------
//
void VGroupBox::operator()(const VFrame& frame, VWindow *parentwin,
						   const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// ---------------------------------------------------------------------------
//
void VGroupBox::operator()(const VFrame& frame, VWindow *parentwin,
						   const VStyle& style, const VString& name)
{
	VControl::operator()(frame, parentwin, style, name);
	construct();
}

// ---------------------------------------------------------------------------
//
void VGroupBox::initialize()
{
#if defined(CV_MOTIF)
	frameWidget = 0;
	labelWidget = 0;
#endif
}

// ---------------------------------------------------------------------------
//
void VGroupBox::construct()
{
}

// ---------------------------------------------------------------------------
//
void VGroupBox::putText(const VString& text)
{
#if defined(CV_MOTIF)
#	if (XmVERSION > 1) || ((XmVERSION == 1) && (XmREVISION >= 2))
	if (labelWidget != 0) {
		char *s = (char *)text.gets();
		XmString xms =  XmStringCreateLocalized(s);
		XtVaSetValues(labelWidget, XmNlabelString, xms, 0);
	}
#	endif
#else
	VControl::putText(text);
#endif
}

// ---------------------------------------------------------------------------
//
void VGroupBox::getInfo(VWinInfo &wi) 
{
#if defined(CV_WINDOWS)
	wi.wClass = "button";
	wi.style = WS_CHILD | WS_VISIBLE | BS_GROUPBOX |
			   WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	wi.exStyle = 0;
#elif defined(PM)
	VControl::getInfo(wi);
	wi.wClass = (CHAR *)(ULONG) WC_STATIC;
	wi.style |= SS_GROUPBOX | DT_MNEMONIC | DT_TOP;
	if (!(style.contains(StyleHidden))) {
		wi.style |= WS_VISIBLE;
	}
#elif defined(CV_MOTIF)
	wi.style = this->style;
	wi.sizingWid = frameWidget;
#endif
}

#if defined(CV_WINDOWS)

// --------------------------------------------------------------------------
// 
UINT VGroupBox::getCtlColor()
{
#if defined(CV_WIN32)
    return WM_CTLCOLORSTATIC;
#else
    return CTLCOLOR_STATIC;
#endif
}

// ---------------------------------------------------------------------------
//
boolean VGroupBox::has3dEffects() const
{
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VGroupBox::has3dEffects(V_3D_EFFECTS v) const
{
	return VControl::has3dEffects(v);
}

#endif

// ---------------------------------------------------------------------------
//
#if defined(CV_MOTIF)

// ---------------------------------------------------------------------------
//
void VGroupBox::getSystemBorderSize(int *w, int *h) const
{
	VControl::getSystemBorderSize(w, h);
	if (h != 0) {
		if (labelWidget != 0) {
			int lh;
			VWindow::queryPreferredSize(labelWidget, 0, &lh);
			*h += lh;
		}
	}
}

// ---------------------------------------------------------------------------
//
boolean VGroupBox::create(HWND parent_widget, VArgLst& args)
{
	char *window_name;

	if (winName != 0) {
		window_name = (char *)winName->gets();
	}
	else {
		window_name = "cvGroupBox";
	}

	args.add(XmNshadowType, XmSHADOW_ETCHED_IN);
	frameWidget = XmCreateFrame(parent_widget, window_name, args, args);

#if (XmVERSION > 1) || ((XmVERSION == 1) && (XmREVISION >= 2))
	//
	// Motif 1.2 or higher supports group-box text embedded
	// within the frame border.
	//
	labelWidget = XtVaCreateManagedWidget("cvGroupBox_Label",
										  xmLabelGadgetClass, frameWidget,
										  XmNchildType,
										  XmFRAME_TITLE_CHILD,
										  XmNchildVerticalAlignment,
										  XmALIGNMENT_CENTER, 0);
	XmString xms =  XmStringCreateLocalized("");
	XtVaSetValues(labelWidget, XmNlabelString, xms, 0);
#endif

	hWnd = XtVaCreateManagedWidget("cvGroupBox_Form",
								   xmFormWidgetClass, frameWidget,
								   XmNresizePolicy, int(XmRESIZE_NONE), 0);

	VControl::registerCallbacks();

	if (!(this->style.contains(StyleHidden))) {
		XtManageChild(frameWidget);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VGroupBox::ownsWidget(Widget widget, Widget *top_widget)
{
	if (widget == 0) {
		return FALSE;
	}
	else if ((widget == frameWidget) ||
			 (widget == hWnd) ||
			 (widget == hShell)) {
		if (top_widget != 0) {
			*top_widget = frameWidget;
		}
		return TRUE;
	}
	else if (XtParent(widget) == frameWidget) {
		if (top_widget != 0) {
			*top_widget = frameWidget;
		}
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// ---------------------------------------------------------------------------
//
Widget VGroupBox::getTopWidget() const
{
	return frameWidget;
}

// ---------------------------------------------------------------------------
//
#if defined(CV_MOTIF)

// ---------------------------------------------------------------------------
//
int VGroupBox::getText(VString& vms)
{
	if (labelWidget != 0) {
		XmString xms;
		XtVaGetValues(labelWidget, XmNlabelString, &xms, 0);
		if (xms == 0)
			return 0;
		VMotStr::getStr(xms, vms);
		XmStringFree(xms);
		return (int)vms.size();
	}
	else
		return 0;
}
#endif


// ---------------------------------------------------------------------------
//
void VGroupBox::showBool(boolean do_show, boolean) 
{ 
	if (frameWidget != 0) {
		if (do_show) {
			if (!XtIsManaged(frameWidget)){
				XtManageChild(frameWidget);
			}
		}
		else if (XtIsManaged(frameWidget)) {
			XtUnmanageChild(frameWidget);
		}
	}
}

// --------------------------------------------------------------------------
//
void VGroupBox::virtSetupBackground(VBrush *brush)
{
	VControl::virtSetupBackground(brush);
	if (backgroundIsPixel) {
		if (frameWidget != 0) {
			VWindow::setWidgetColor(frameWidget, backgroundPixel);
		}
		if (labelWidget != 0) {
			VWindow::setWidgetColor(labelWidget, backgroundPixel);
		}
	}
}
// --------------------------------------------------------------------------
//
void VGroupBox::virtSetupFont(VFont *font)
{
	if (labelWidget == 0) {
		return;
	}
	if (fList != 0) {
		XmFontListFree(fList);
		fList = 0;
	}
	if ((originalFontList == 0) && !labelWidget->core.being_destroyed) {
		XtVaGetValues(labelWidget, XmNfontList, &originalFontList, 0);
	}
	if (font != 0) {
		font->load(this);
		XmFontListEntry entry =
			XmFontListEntryCreate(XmFONTLIST_DEFAULT_TAG, XmFONT_IS_FONTSET,
								  font->getFontSet());
		fList = XmFontListAppendEntry(0, entry);
#if ((XmVERSION != 1) || (XmREVISION != 2) || (XmUPDATE_LEVEL != 1))
		XmFontListEntryFree(&entry);
#else
		XtFree(entry); // work around bug
#endif
	}
	else {
		fList = 0;
	}
	XmFontList xmfontlist = (fList == 0) ? originalFontList : fList;
	XtVaSetValues(labelWidget, XmNfontList, xmfontlist, 0);
}
 
#else

// ---------------------------------------------------------------------------
//
boolean VGroupBox::erased() 
{
	return FALSE;
}

// ---------------------------------------------------------------------------
//
enum WinType VGroupBox::winType()
{
	return WinControl;
}

#endif

// ---------------------------------------------------------------------------
//
boolean VGroupBox::key(int virtual_key, char ascii_key)
{
	VWindow *child;

	switch (virtual_key) {
	case K_Home:
		child = getFirstChild();
		break;
	case K_End:
		child = getLastChild();
		break;
	case K_Up:
	case K_Left:
		child = getPrevChild();
		break;
	case K_Down:
	case K_Right:
		child = getNextChild();
		break;
	default:
		child = 0;
		break;
	}

	if (child != 0) {
		if (!VWindow::checkLosingFocus()) {
			if (child->takeFocus()) {
				return TRUE;
			}
		}
	}

	return VControl::key(virtual_key, ascii_key);
}

// ---------------------------------------------------------------------------
//
VWindow *VGroupBox::getNextChild(VWindow *current_child) const
{
	VOrdCollect *children = ((VWindow *)this)->childrenOf();

	if (children == 0) {
		return 0;
	}

	int count = children->count(), index;
	
	if (count <= 0) {
		return 0;
	}

	if ((current_child != 0) ||
		((current_child = ((VWindow *)this)->getChildFocus()) != 0)) {
		if (current_child->getParent() != (VWindow *)this) {
			index = 0;
		}
		else {
			index = children->indexOfSame(current_child);
			if ((index < 0) || (index >= count)) {
				index = 0;
			}
		}
	}
	else {
		index = 0;
	}

	for (int i = index + 1 ; 1 ; i++) {
		if (i >= count) {
			i = 0;
		}
		if (i == index) {
			break;
		}
		VWindow *child = (VWindow *)children->idAt(i);
		if ((child != 0) &&
			!child->isTopLevel() && child->getFocusCandidate()) {
			return child;
		}
	}

	return 0;
}

// ---------------------------------------------------------------------------
//
VWindow *VGroupBox::getPrevChild(VWindow *current_child) const
{
	VOrdCollect *children = ((VWindow *)this)->childrenOf();

	if (children == 0) {
		return 0;
	}

	int count = children->count(), index;
	
	if (count <= 0) {
		return 0;
	}

	if ((current_child != 0) ||
		((current_child = ((VWindow *)this)->getChildFocus()) != 0)) {
		if (current_child->getParent() != (VWindow *)this) {
			index = 0;
		}
		else {
			index = children->indexOfSame(current_child);
			if ((index < 0) || (index >= count)) {
				index = 0;
			}
		}
	}
	else {
		index = 0;
	}

	for (int i = index - 1 ; 1 ; i--) {
		if (i < 0) {
			i = count - 1;
		}
		if (i == index) {
			break;
		}
		VWindow *child = (VWindow *)children->idAt(i);
		if ((child != 0) &&
			!child->isTopLevel() && child->getFocusCandidate()) {
			return child;
		}
	}

	return 0;
}

// ---------------------------------------------------------------------------
//
VWindow *VGroupBox::getFirstChild() const
{
	VOrdCollect *children = ((VWindow *)this)->childrenOf();

	if (children == 0) {
		return 0;
	}

	int count = int(children->count());

	if (count <= 0) {
		return 0;
	}

	return getNextChild((VWindow *)children->idAt(count - 1));
}

// ---------------------------------------------------------------------------
//
VWindow *VGroupBox::getLastChild() const
{
	VOrdCollect *children = ((VWindow *)this)->childrenOf();

	if (children == 0) {
		return 0;
	}

	int count = int(children->count());

	if (count <= 0) {
		return 0;
	}

	return getPrevChild((VWindow *)children->idAt(0));
}

// ----------------------------------------------------------------------------
//
void VGroupBox::getPreferredSize(int *w, int *h) const
{
	VControl::getPreferredSize(w, h);
	if (w != 0) { *w += 12; }
}

// ----------------------------------------------------------------------------
//
void VGroupBox::getInternalBorderGeometry(int *left, int *right,
										  int *top, int *bottom) const
{
	if (left   != 0) { *left   = 3; }
	if (right  != 0) { *right  = 3; }
	if (bottom != 0) { *bottom = 3; }

	if (top != 0) {
		int dummy;
		((VGroupBox *)this)->fontSize(&dummy, top);
		*top -= 1;
	}
}

