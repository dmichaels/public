// $Header:   Q:/views/win/vcs/listslct.cpv   1.7   Mar 04 1997 17:55:18   gregm  $ 

//	listslct.cpp
//
//	VListSelect implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
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
//	05/05/93 jmd	added attachments, preened
//	05/25/93 jmd	added color
//	08/30/93 jmd	fixed focus problems
//	10/05/93 jmd	fixed width
//	02/18/94 pat	changed LightGray to VColor::LightGray
//	09/05/94 dss	Let Ctl3d choose the background.
//	10/11/94 dss	Added operator(). Constructor calls operator().
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#include "listslct.h"
#include "ordcllct.h"
#include "iterator.h"
#include "listbox.h"
#include "pushbttn.h"
#include "str.h"
#include "notifier.h"

#include "brush.h"
#include "shdctrl.h"

extern "C" {
#	include <string.h>
}

defineClass(VListSelect, VDialog)

// ---------------------------------------------------------------------------
VListSelect::VListSelect()
{
	;
}

// ---------------------------------------------------------------------------
// Construct a  VListSelect dialog box. Called by the dialog() static
// function.
//
VListSelect::VListSelect(boolean select, char *title,
							VOrdCollect *lst, int *n, VWindow *pWin)
	: VDialog(VFrame(100, 100, 30*16, 15*16, VFrame::Font16), pWin)
{
	(*this)(select, title, lst, n, pWin);
}

// ---------------------------------------------------------------------------
//
void VListSelect::operator()(boolean select, char *title,
							VOrdCollect *lst, int *n, VWindow *pWin)
{
	int i, max = 0;

	// let ctl3d do this
#if !defined(CV_CTL3D)
	setBackground(new VBrush(VColor::LightGray));
#endif

	VPushButton *pLstOk, *pLstCancel;

	VListBox pLstList(VFrame(0, 0, 1, 1), this, StyleBorder + StyleShadow);
	if (title) {
		max = strlen(title);
		setTitle(title);
	}
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

	VShadeControl *pShade;
	pShade = new VShadeControl(VFrame(0, 0.5F, 1.0F, 2), this, VShadeControl::HDip);

	attachWin(pShade,	 VAttach::Bottom, 	VAttach::Sibling, 8, pLstOk);

	attachWin(&pLstList, VAttach::Top, 		VAttach::Parent, 8);
	attachWin(&pLstList, VAttach::Left, 	VAttach::Parent, 8);
	attachWin(&pLstList, VAttach::Right,	VAttach::Parent, 8);
	attachWin(&pLstList, VAttach::Bottom,	VAttach::Sibling, 8, pShade);

	// find width of widest string
	DO (lst, VString, s)
		const char *p;
		pLstList.appendString(p = s->gets());
		if ((i = strlen(p)) > max) {
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

	pLstList.selectItem(-1);
	pLstList.uponClick(this, NIL_METHOD, methodOf(VDialog, ok));

	// show first: hidden windows can't take focus
	centerWindow(this);
	show();

	if (!select) {
		pLstOk->takeFocus();
	}
	else {
		pLstList.takeFocus();
	}
	modal();
	if (result && select) {
		*n = pLstList.selectedItem();
	}
	else {
		*n = -1;
	}
}

// ---------------------------------------------------------------------------
VListSelect::~VListSelect()
{
	delete getBackground();
}

// ---------------------------------------------------------------------------
// Perform a list selection style dialog within window 'win', with
// title 'title' and a list given by the objects in VOrdCollect 'lst'. 
// If 'select' is TRUE, then put both 'OK' and 'Cancel' VPushButton objects at
// the bottom of the dialog, otherwise, put only an 'OK' VPushButton at the
// bottom.  The number of strings to list is given by the number of non-NIL
// entries in VOrdCollect 'lst'.  If 'select' is true, then return the index
// of the selected string, otherwise return -1 if the user cancelled the dialog.  
// If 'select' is FALSE, then simply return.
//
int VListSelect::dialog(boolean select, char *title,
									VString **lst, VWindow *win)
{
	int			sel;
	VOrdCollect s;

	while (*lst) {
		s.add((VObject *) *lst++);
	}
	VListSelect l(select, title, &s, &sel, win);
	return sel;
}

// ---------------------------------------------------------------------------
// Perform a list selection style dialog within window 'win', with
// title 'title', and a list given by the objects contained in VOrdCollect
// 'lst'.  If 'select' is TRUE, then put both 'OK' and 'Cancel' VPushButton
// objects at the bottom of the dialog, otherwise, put only an 'OK' VPushButton
// at the bottom.  Also, if 'select' is TRUE, then return the index of the
// selected string, otherwise, return -1 if the user cancelled the dialog.  If
// 'select' is FALSE, then simply return.
//
int VListSelect::dialog(boolean select, char *title,
									VOrdCollect *lst, VWindow *win)
{
	int sel;
	VListSelect l(select, title, lst, &sel, win);
	return sel;
}

void VListSelect::centerWindow(VWindow* pwin)
{
    int dw, dh, sw, sh;
	int px, py, ph, pw, pcx, pcy;

    pwin->getAbs(&dw, &dh);

	if (pwin->getParent()) {
	   pwin->getParent()->getAbs(&px, &py, &ph, &pw);
	   pcx = px + (ph / 2);
	   pcy = py + (pw / 2);
	}
	else {
	   notifier->getScreenSize(&sw, &sh);
	   pcx = sw / 2;
	   pcy = sh / 2;
	}   
	  
    pwin->setFrameUnits(VFrame::Pixels);

    VRectangle rect(CenterDim, pcx, pcy, dw, dh);
    pwin->move(rect, FALSE);
}


// ---------------------------------------------------------------------------

