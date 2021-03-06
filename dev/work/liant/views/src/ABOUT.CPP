// $Header:   Q:/views/win/vcs/about.cpv   1.6   Mar 04 1997 17:16:30   gregm  $

//	about.cpp
//
//	VAbout implementation
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
//	04/06/93 pmcm	preened
//	04/06/93 pmcm	deleted prompt and okbttn in destructor
//	04/16/93 pmcm	added matching va_end for va_start
//	05/24/93 jmd	changed Ok to OK to end tony's complaining
//	05/25/93 jmd	added geometry and color
//	07/25/93 jmd	added const
//	10/05/93 jmd	fixed width
//	02/18/94 pmcm	changed LightGray to VColor::LightGray
//	09/05/94 dss	Let Ctl3d choose the background.
//	10/11/94 dss	Added operator(). Constructor calls operator().
//	11/03/94 jmd	Added defineClass
//	01/03/96 pkt	bulletproofed calls to vsprintf.
// ----------------------------------------------------------------------------

extern "C" {
#	include <stdarg.h>
#	include <stdio.h>
}

#include "about.h"
#include "textbox.h"
#include "pushbttn.h"
#include "brush.h"
#include "shdctrl.h"

defineClass(VAbout, VDialog)

//----------------------------------------------------------------------------
//	Default constructor
//
VAbout::VAbout() 
{
	prompt = 0;
	okbttn = 0;
}

//----------------------------------------------------------------------------
//	Construct a titled VAbout dialog as a child of the given parent window
//	to present a message to the user.

VAbout::VAbout(const char *title, const char *message, VWindow *win) :
	VDialog(VFrame(4*16, 5*16, 36*16, 14*16, VFrame::Font16), win)
{
	(*this)(title, message, parent);
}

//----------------------------------------------------------------------------
//

void VAbout::operator()(const char *title, const char *message, VWindow *win)
{
	if (title != 0) {
		setTitle(title);
	}

	prompt = new VTextBox(VFrame(0.0F, 0.0F, 1.0F, 0.72F), this);
	prompt->align(Center);

	prompt->putText((char *)message);	//!!!jmd-make putText const

	okbttn = new VPushButton(VFrame(0.5F, 0, 10*16, 28, CenterDim, VFrame::Font16), this,
								StyleDefaultButton, "OK");
	setDefButton(okbttn);

	VShadeControl *pShade;
	pShade = new VShadeControl(VFrame(0, 0.5F, 1.0F, 2), this, VShadeControl::HDip);

	// set attachments
	attachWin(okbttn,	VAttach::Bottom, VAttach::Parent, 8);

	attachWin(pShade,	VAttach::Bottom, VAttach::Sibling, 8, okbttn);

	attachWin(prompt,	VAttach::Top,	 VAttach::Parent, 8);
	attachWin(prompt,	VAttach::Left,	 VAttach::Parent, 8);
	attachWin(prompt,	VAttach::Right,	 VAttach::Parent, 8);
	attachWin(prompt,	VAttach::Bottom, VAttach::Sibling, 8, pShade);
	
	frameChildren();

	// set callbacks
	okbttn->uponClick(this, methodOf(VDialog, ok));

	// let ctl3d do this
#if !defined(CV_CTL3D)
	setBackground(new VBrush(VColor::LightGray), TRUE);
#endif

	show();
	modal();
}

//----------------------------------------------------------------------------
//	Destroy this about dialog
//
VAbout::~VAbout()
{
	delete getBackground();
}

//----------------------------------------------------------------------------
//	Perform an about dialog with given message within the given window.
//
void VAbout::dialog(const char *message, VWindow *parent)
{
	VAbout a((char *) 0, message, parent);
}

//----------------------------------------------------------------------------
//	Perform an about dialog with the given message in printf() format
//	within the given window.  If a non-NULL title is specified, then 
//	it is used as the dialog title.
//
void VAbout::dialog(const char *title, VWindow *parent, const char *message...)
{
	char b[100];
	VString sb;
	va_list arg;
	va_start(arg, message);
	char *bufp = VString::selectFormatBuffer(b, 100, &sb, message, arg);
	va_end(arg);
	if (bufp != 0) {
		va_start(arg, message);
		vsprintf(bufp, (char *)message, arg);
		va_end(arg);
		VAbout a(title, bufp, parent);
	}
}

//----------------------------------------------------------------------------
