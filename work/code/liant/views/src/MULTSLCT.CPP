// $Header:   Q:/views/common/vcs/MULTSLCT.CPV   1.9   Mar 24 1997 16:12:40   scottt  $ 

//	multslct.cpp
//
//	VMultiSelect class implementation [Common]
//
//	Allegris Foundation 1.1.00
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	04/07/93 pat	changed id to VObject *
//	10/05/93 jmd	added attachments, preened, added VListSelect size logic
//	03/25/94 pkt	enhanced with default button.
//	03/25/94 pkt	enhanced with default button.
//	09/05/94 dss	Let Ctl3d choose the background.
//	10/11/94 dss	Added realize(). Constructor calls realize().
//	05/16/95 pkt	preened and merged.
//	09/01/95 dss	VStyle changes.
//	11/01/95 dgm	Changed name realize() to construct() to avoid
//					virtual function hiding warnings
//	12/11/95 pkt	internationalized.
//	11/13/96 pkt	modified for Web support.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "iterator.h"
#include "ordcllct.h"
#include "str.h"
#include "multlist.h"
#include "pushbttn.h"
#include "multslct.h"

#include "brush.h"
#ifndef CV_REMOTE_GUI
# include "shdctrl.h"
#endif

extern "C" {
#	undef NULL
#	include <string.h>
}

defineClass(VMultiSelect, VDialog)

// ---------------------------------------------------------------------------
VMultiSelect::VMultiSelect()
{
	;
}

// ---------------------------------------------------------------------------
// Construct a VMultiSelect dialog box. Called by the dialog() static functions.
//
VMultiSelect::VMultiSelect(boolean select, char *title, VOrdCollect *lst, VOrdCollect *&sel, VWindow *pWin) :
	VDialog(VFrame(100, 100, 30*16, 15*16, VFrame::Font16), pWin)
{
	construct(select, title, lst, sel, pWin);
}

// ---------------------------------------------------------------------------
//
void VMultiSelect::construct(boolean select, char *title, VOrdCollect *lst,
							 VOrdCollect *&sel, VWindow *pWin)
{

#ifndef CV_REMOTE_GUI
	int n;
	pLstCancel = 0;

	// let ctl3d do this
#if !defined(CV_CTL3D)
	setBackground(new VBrush(VColor::LightGray));
#endif

	if (title != 0) {
		setTitle(title);
	}
	else {
		setTitle("");
	}
	pLstList = new VMultiListBox(VFrame(0, 0, 1, 1), this, 
									StyleSorted + StyleBorder + StyleShadow);


	if (select) {
		pLstOk = new VPushButton(VFrame(0, 0, 8 * 16, 28, VFrame::Font16),
									this, StyleDefaultButton, "OK");

		pLstCancel = new VPushButton(VFrame(0, 0, 8 * 16, 28, VFrame::Font16),
									this, StyleDefault, "Cancel");

		pLstCancel->uponClick(this, methodOf(VDialog, cancel));

		attachWin(pLstOk, VAttach::Left, 	VAttach::Parent, 8);
		attachWin(pLstOk, VAttach::Bottom, 	VAttach::Parent, 8);

		attachWin(pLstCancel, VAttach::Right, 	VAttach::Parent, 8);
		attachWin(pLstCancel, VAttach::Bottom, 	VAttach::Parent, 8);
	}
	else {
		pLstOk = new VPushButton(VFrame(0.5F, 0, 8 * 16, 28,
											CenterDim, VFrame::Font16),
									this, StyleDefault, "OK");

		pLstCancel = 0;

		attachWin(pLstOk, VAttach::Bottom, 	VAttach::Parent, 8);
	}

	pLstOk->uponClick(this, methodOf(VDialog, ok));
	setDefButton(pLstOk);

	VShadeControl *pShade;
	pShade = new VShadeControl(VFrame(0, 0.5F, 1.0F, 2), this, VShadeControl::HDip);

	attachWin(pShade,	 VAttach::Bottom, 	VAttach::Sibling, 8, pLstOk);

	attachWin(pLstList, VAttach::Top, 		VAttach::Parent, 8);
	attachWin(pLstList, VAttach::Left, 		VAttach::Parent, 8);
	attachWin(pLstList, VAttach::Right,		VAttach::Parent, 8);
	attachWin(pLstList, VAttach::Bottom,	VAttach::Sibling, 8, pShade);

	// find width of widest string
	int i, max = 0;
	DO (lst, VString, s)
		const char *p = s->gets();
		pLstList->appendString(p);
		if ((i = VWideCharAux::mbslen(p)) > max) {
			max = i;
		}
	END 

	int fw, fh;
	fontSize(&fw, &fh);

	int	x, y, w, h;
	getRel(&x, &y, &w, &h);

	if ((max * fw) > w) {
		move(VFrame(100, 100, max*16, 12*16, VFrame::Font16));
	}

	frameChildren();

	pLstList->deselectItem(-1);

	show();
	if (!select) {
		pLstOk->takeFocus();
	}
	else {
		pLstList->takeFocus();
	}

	modal();
	if (result && (n = pLstList->selectedCount()) > 0) {
		VOrdCollect *c = new VOrdCollect;
		int i;
		for (i = 0; i < n; i++) {
			int m = pLstList->selectedItem(i);
			c->add(lst->idAt(m));
			sel = c;
		}
	}
	else {
		sel = 0;
	}
#else
	// unimplemented
	sel = 0;
#endif
}

// ---------------------------------------------------------------------------
VMultiSelect::~VMultiSelect()
{
#ifndef CV_REMOTE_GUI
	delete getBackground();
#endif
}

// ---------------------------------------------------------------------------
// Perform a multiple selection style dialog within window 'win', with
// title 'title', and an array of VString objects given by 'lst'. If 'select'
// is TRUE, then put both 'OK' and 'Cancel' VPushButton objects at the bottom
// of the dialog, otherwise, put only an 'OK' VPushButton at the bottom.  The
// number of strings to list is given by the number of non-0 entries in 'lst'. 
// If 'select' is TRUE, then return an VOrdCollect of the selected VString
// objects. Otherwise, if the user cancelled the dialog, return 0.  Also, if
// 'select' is FALSE, then return 0.
//
VOrdCollect *VMultiSelect::dialog(boolean select, char *title, VString **lst, VWindow *win)
{
	VOrdCollect *sel = 0;
	VOrdCollect s;
	while (*lst) {
		s.add((VObject *) *lst++);
	}

	VMultiSelect l(select, title, &s, sel, win);
	return sel;
}

// ---------------------------------------------------------------------------
// Perform a multiple selection style dialog within window 'win', with
// title 'title', and an VOrdCollect of objects given by 'lst'.  If 'select'
// is TRUE, then put both 'OK' and 'Cancel' VPushButton objects at the bottom
// of the dialog, otherwise, put only an 'OK' VPushButton at the bottom.  If
// 'select' is TRUE, return an VOrdCollect of the selected VString objects. 
// Otherwise, if the user cancelled the dialog, return 0.  Also, if 'select'
// is FALSE, then return 0.
//
VOrdCollect *VMultiSelect::dialog(boolean select, char *title, VOrdCollect *lst, VWindow *win)
{
	VOrdCollect *sel = 0;
	VMultiSelect l(select, title, lst, sel, win);
	return sel;
}

// ---------------------------------------------------------------------------
