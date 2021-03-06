// $Header:   Q:/views/win/vcs/addrmvls.cpv   1.8   Mar 04 1997 17:16:32   gregm  $ 

//	addrmvls.cpp
//
//	VAddRemoveList implementation
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
//	04/16/93 pat	added matching va_end for va_start
//	05/25/93 jmd	preened, added geometry and color
//	08/09/93 jmd	removed free
//	10/05/93 jmd	fixed width
//	02/18/94 pat	changed LightGray to VColor::LightGray
//	09/05/94 dss	Let Ctl3d choose the background.
//	10/11/94 dss	Added operator(). Constructor calls operator().
//	09/01/95 dss	VStyle changes.
//	01/03/96 pkt	bulletproofed call to vsprintf.
//	03/05/96 pkt	changed paramters from 'char *' to 'const char *'.
// ----------------------------------------------------------------------------

extern "C" {
#	include <stdarg.h>
#	include <stdio.h>
}

#include "addrmvls.h"
#include "multlist.h"
#include "pushbttn.h"
#include "textbox.h"
#include "iterator.h"
#include "str.h"

#include "brush.h"
#include "shdctrl.h"

defineClass(VAddRemoveList, VDialog)

// ---------------------------------------------------------------------------
VAddRemoveList::VAddRemoveList()
{
	;
}

// ---------------------------------------------------------------------------
// Construct a `VAddRemoveList` dialog under window 'pWin' with set 'set'
// and subset 'subset'. These collections should contain string objects. The C
// string 'SetLabel' will be placed above the set list and the C string
// 'subsetLabel' will be placed above the subset list.  The title of the dialog
// will be created from the printf() style `prompt`.  If 'sub' is TRUE and the
// OK button is selected, then upon return the `subSet` `VCollection` will
// contain the contents of the "subset" `VMultiListBox` in the dialog.
//
VAddRemoveList::VAddRemoveList(VWindow *pWin, boolean &sub,
								VCollection *set, VCollection *subset,
								const char *setLabel, const char *subsetLabel,
								const char *prompt) :
	VDialog(VFrame(4*16, 5*16, 45*16, 15*16, VFrame::Font16), pWin)
{
	(*this)(pWin, sub, set, subset, setLabel, subsetLabel, prompt);
}

//---------------------------------------------------------------------------
//
void VAddRemoveList::operator()(VWindow *pWin, boolean &sub,
								VCollection *set, VCollection *subset,
								const char *setLabel, const char *subsetLabel,
								const char *prompt)
{
	if (prompt) {
		setTitle(prompt);
	}

	// let ctl3d do this
#if !defined(CV_CTL3D)
	VBrush *br;
	setBackground(br = new VBrush(VColor::LightGray));
#endif

	DO (set, VObject, obj)
		setColl.add(obj);
	END 

	DO (subset, VObject, obj)
		subSetColl.add(obj);
	END 

	result = sub;

	// create controls
	VPushButton pOk(VFrame(0, 0, 10*16, 28, VFrame::Font16), this,
								StyleDefaultButton, "OK");
	setDefButton(&pOk);
	VPushButton pCancel(VFrame(0, 0, 10*16, 28, VFrame::Font16), this,
								StyleDefault, "Cancel");

	VShadeControl pShade(VFrame(0, 0.5F, 1.0F, 2), this, VShadeControl::HDip);

	VTextBox pSetTitle(VFrame(0, 0, 0.40F, 16, VFrame::Font16),
								this, StyleCenter, setLabel);

	VTextBox pSubsetTitle(VFrame(0, 0, 0.40F, 16, VFrame::Font16),
								this, StyleCenter, subsetLabel);

	// let ctl3d do this
#if !defined(CV_CTL3D)
	pSetTitle.setBackground(br);
	pSubsetTitle.setBackground(br);
#endif

	pSetList(VFrame(0, 0, 0.3F, 0.6F), this, StyleBorder + StyleShadow);
	pSubsetList(VFrame(0, 0, 0.3F, 0.6F), this, StyleBorder + StyleShadow);

	pAddSet(VFrame(0.5F, 0, 6*16, 28, CenterDim, VFrame::Font16), this);
	pAddSet.putText("<<");

	pRemoveSet(VFrame(0.5F, 0, 6*16, 28, CenterDim, VFrame::Font16), this);
	pRemoveSet.putText(">>");

	// set attachments
	attachWin(&pOk,			VAttach::Left, 	 VAttach::Parent, 8);
	attachWin(&pOk,			VAttach::Bottom, VAttach::Parent, 8);

	attachWin(&pCancel,		VAttach::Right,	 VAttach::Parent, 8);
	attachWin(&pCancel,		VAttach::Bottom, VAttach::Parent, 8);

	attachWin(&pShade,		VAttach::Bottom, VAttach::Sibling, 8, &pOk);

	attachWin(&pSetTitle,	VAttach::Top, 	 VAttach::Parent, 8);
	attachWin(&pSetTitle,	VAttach::Left, 	 VAttach::Parent, 8);

	attachWin(&pSubsetTitle,VAttach::Top, 	 VAttach::Parent, 8);
	attachWin(&pSubsetTitle,VAttach::Right,  VAttach::Parent, 8);

	attachWin(&pRemoveSet,	VAttach::Top, 	 VAttach::Sibling, 8, &pSetTitle);

	attachWin(&pAddSet,		VAttach::Bottom, VAttach::Sibling, 8, &pShade);

	attachWin(&pSetList,	VAttach::Top, 	 VAttach::Sibling, 8, &pSetTitle);
	attachWin(&pSetList,	VAttach::Left, 	 VAttach::Parent,  8);
	attachWin(&pSetList,	VAttach::Right,  VAttach::Sibling, 8, &pAddSet);
	attachWin(&pSetList,	VAttach::Bottom, VAttach::Sibling, 8, &pShade);

	attachWin(&pSubsetList,	VAttach::Top, 	 VAttach::Sibling, 8, &pSubsetTitle);
	attachWin(&pSubsetList,	VAttach::Left,   VAttach::Sibling, 8, &pAddSet);
	attachWin(&pSubsetList,	VAttach::Right,	 VAttach::Parent,  8);
	attachWin(&pSubsetList,	VAttach::Bottom, VAttach::Sibling, 8, &pShade);

	frameChildren();

	// set callbacks
	pOk.uponClick(this, methodOf(VDialog, ok));
	pCancel.uponClick(this, methodOf(VDialog, cancel));
	pAddSet.uponClick(this, methodOf(VAddRemoveList, add));
	pRemoveSet.uponClick(this, methodOf(VAddRemoveList, remove));

	pAddSet.disable();
	pRemoveSet.disable();

	pSetList.uponClick(this, methodOf(VAddRemoveList, listChange), 
		NIL_METHOD, TRUE);
	pSubsetList.uponClick(this, methodOf(VAddRemoveList, listChange), 
		NIL_METHOD, TRUE);

	fill();
	modal();

	if (sub = result) {
		subset->reset();
		DO (subSetColl, VObject, obj)
			subset->add(obj);
		END 
	}
}

// ---------------------------------------------------------------------------
VAddRemoveList::~VAddRemoveList()
{
	delete getBackground();
}

// ---------------------------------------------------------------------------
// fill both lists
//
void VAddRemoveList::fill()
{
	pSubsetList.clearList();
	pSetList.clearList();
	subSetColl.sort(TRUE, methodOf(VString, compare));

	DO (subSetColl, VString, s)
		if (setColl.find(s)) {
			setColl.remove(s);
		}
		pSubsetList.appendString(s->gets());
	END 

	setColl.sort(TRUE, methodOf(VString, compare));
	DO (setColl, VString, s)
		pSetList.appendString(s->gets());
	END 

	pAddSet.disable();
	pRemoveSet.disable();
}

// ---------------------------------------------------------------------------
// Run a `VAddRemoveList` dialog under window 'win' with set 'set' and
// subset 'subset'. These collections should contain string objects. The C
// string 'SetLabel' will be placed above the set list and the C string
// 'subsetLabel' will be placed above the subset list.  The title of the dialog
// will be created from the printf() style `prompt`. If 'sub' is TRUE and the
// OK button is selected, then upon return the `subSet` `VCollection` will contain
// the contents of the "subset" `VMultiListBox` in the dialog.
//
boolean VAddRemoveList::dialog(VWindow *win, boolean sub, VCollection *set, VCollection *subset, const char*setLabel, const char*subsetLabel, const char *prompt...)
{
	char b[100];

	if (prompt) {
		VString sb;
		va_list arg;
		va_start(arg, prompt);
		char *bufp = VString::selectFormatBuffer(b, 100, &sb, prompt, arg);
		va_end(arg);
		if (bufp != 0) {
			va_start(arg, prompt);
			vsprintf(bufp, prompt, arg);
			va_end(arg);
			prompt = bufp;
		}
		else {
			prompt = 0;
		}
	}
	VAddRemoveList(win, sub, set, subset, setLabel, subsetLabel, prompt);

	return sub;
}

// ---------------------------------------------------------------------------
boolean VAddRemoveList::listChange(VMultiListBox *lst)
{
	if (lst == &pSubsetList) {
		if (lst->selectedCount()) {
			pAddSet.enable(TRUE);
		}
		else {
			pAddSet.enable(FALSE);
		}
	}
	else {
		if (lst->selectedCount()) {
			pRemoveSet.enable(TRUE);
		}
		else {
			pRemoveSet.enable(FALSE);
		}
	}			

	return TRUE;
}

// ---------------------------------------------------------------------------
// Add to subset button pressed.
//
boolean VAddRemoveList::add(VButton *)
{
	int		i, n;
	VString *s;

	if ((n = pSubsetList.selectedCount()) > 0) {
		for (i = 0; i < n; i++) {
			s = pSubsetList.selectedString(i);
			VObject *o = subSetColl.remove(s);
			delete s;  // jmd-8/09/93 (was free)
			setColl.add(o);
		}
		fill();
	} 

	return TRUE;
}

// ---------------------------------------------------------------------------
// 	Remove from subset button pressed.
//
boolean VAddRemoveList::remove(VButton *)
{
	int		i, n;
	VString *s;

	if ((n = pSetList.selectedCount()) > 0) {
		for (i = 0; i < n; i++) {
			s = pSetList.selectedString(i);
			VObject *o = setColl.remove(s);
			delete s;  // jmd-8/09/93 (was free)
			subSetColl.add(o);
		}
		fill();
	} 

	return TRUE;
}

// ---------------------------------------------------------------------------
