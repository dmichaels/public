// $Header:   Q:/views/win/vcs/input.cpv   1.14   Mar 04 1997 17:55:14   gregm  $ 

//  input.cpp
//
//  VInput implementation
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1995-1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  04/16/93 pat    added matching va_end for va_start
//  05/24/93 jmd    preened, added geometry and color
//  10/05/93 jmd    fixed width
//  02/18/94 pat    changed LightGray to VColor::LightGray
//  09/05/94 dss    Let Ctl3d choose the background.
//  10/11/94 dss    Added operator(). Constructor calls operator().
//  09/01/95 dss    VStyle changes.
//  01/03/96 pkt    bulletproofed call to vsprintf.
//  08/27/96 dgm    Reworked layout.
// ----------------------------------------------------------------------------

#include "input.h"
#include "group.h"
#include "textbox.h"
#include "editline.h"
#include "pushbttn.h"
#include "georow.h"
#include "geocol.h"
#include "str.h"
#include "clrscm.h"
#include "notifier.h"

extern "C" {

#include <stdarg.h>
#include <stdio.h>

}

// ----------------------------------------------------------------------------
//
defineClass(VInput, VDialog)

// ----------------------------------------------------------------------------
//
VInput::VInput()
	: VDialog()
{
}

// ----------------------------------------------------------------------------
// Construct a data entry dialog box.  If 'prompt' is given then display
// it.  If '*sp' points to a `VString`, then display that string as the
// string to be edited.  When this constructor exits, '*sp' is set to NIL
// if the user cancelled the dialog, otherwise, '*sp' is set to a new
// `VString` object that contains the entered text.
// 
VInput::VInput(const char *title, const char *prompt,
			   VString **sp, VWindow *parentwin)
	: VDialog(VFrame(), parentwin, title,
			  StyleHidden + StyleCaption + StyleCloseBox + StyleSizable)
{
    (*this)(title, prompt, sp, parentwin);
}

// ----------------------------------------------------------------------------
//
void VInput::operator()(const char *title, const char *prompt,
						VString **sp, VWindow *parentwin)
{
	if (title != 0) {
		setTitle(title);
	}

	setBackground(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));

	VGeoColumn *mgr = setGeoColumn();
	mgr->setSpacingStyle(VGeoColumn::NoSpacing);
	mgr->setPreferredSpacing(5);
	mgr->setDefaultChildJustifyStyle(VGeoColumn::FilledJustify);
	mgr->setSpaceStretchability(0);
	mgr->setMargins(5);

	VGroup *group = new VGroup(VFrame(), this, "");
	VGeoColumn *groupmgr = group->setGeoColumn();
	groupmgr->setMargins(5);
	groupmgr->setPreferredSpacing(5);
	groupmgr->setDefaultChildJustifyStyle(VGeoColumn::FilledJustify);
	mgr->setChildStretchability(groupmgr->getIndex(), 1);

    VTextBox *textbox = new VTextBox(VFrame(), group);

    editLine = new VEditLine(VFrame(0, 0, 8*16, 0, VFrame::Font16),
			   				 group, StyleHorizontal);

	VGeoRow *buttonmgr = new VGeoRow(mgr);
	buttonmgr->setSpacingStyle(VGeoRow::NoSpacingOpposite);
	buttonmgr->setPreferredSpacing(5);
	buttonmgr->setEqualSize(TRUE);
	buttonmgr->setSpaceElasticity(0.0F);
	mgr->setChildMargins(buttonmgr, 10, 0, 0, 0);

	VGeoFrame *space = new VGeoManager(buttonmgr);
	buttonmgr->setChildElasticity(space, 1.0F);

    VPushButton *ok =
				 new VPushButton(VFrame(), this, StyleDefaultButton, "OK");
	buttonmgr->addChild(ok);
    ok->uponClick(this, methodOf(VDialog, okPressed));   
    setDefButton(ok);

    VPushButton *cancel =
				 new VPushButton(VFrame(), this, StyleDefault, "Cancel");
	buttonmgr->addChild(cancel);
    cancel->uponClick(this, methodOf(VDialog, cancelPressed));

    if (sp[0] != 0) {
        editLine->putText(sp[0]->gets());
    }
    if (prompt != 0) {
        textbox->putText(prompt);
    }

	setFrame(VFrame());
	constrainMinimumSize();
	int preferred_h;
	determinePreferredSize(0, &preferred_h);
	constrainMaximumSize(0, preferred_h);
	frameWindow();
	centerWindow(parentwin);
    show();

    editLine->selectAll(0);
    editLine->takeFocus();
    modal();

    if (!result) {
        *sp = 0;
    }
    else {
        *sp = new VString;
        editLine->getText(**sp);
    }
}

// ----------------------------------------------------------------------------
//
VInput::~VInput()
{
}

// ----------------------------------------------------------------------------
// Perform an input style dialog within window 'parentwin' with prompt
// 'prompt', and initial text 'input'.  Return a new `VString` object with the
// result, or return NIL if the user cancelled the dialog.
//
VString *VInput::dialog(const char *prompt, VString *input, VWindow *parentwin)
{
    VInput i(0, prompt, &input, parentwin);
    return input;
}

// ----------------------------------------------------------------------------
// Perform an input style dialog within window 'parentwin' with prompt
// 'prompt', in 'printf' style format and initial text 'input'.  If 'title'
// is non-NIL, then make it the caption.  Return a new `VString` object
// with the result, or return a NIL pointer if the user cancelled the dialog.
//
VString *VInput::dialog(VString *input, const char *title,
                        VWindow *parentwin, const char *prompt ...)
{
    char    b[100];
    VString sb;
    va_list arg;

    va_start(arg, prompt);
    char *bufp = VString::selectFormatBuffer(b, 100, &sb, prompt, arg);
    va_end(arg);

    if (bufp != 0) {
        va_start(arg, prompt);
        vsprintf(bufp, prompt, arg);
        va_end(arg);
        VInput i(title, bufp, &input, parentwin);
        return input;
    }
    return 0;
}

// ----------------------------------------------------------------------------
//
boolean VInput::givenFocus()
{
	editLine->takeFocus();
	return TRUE;
}
