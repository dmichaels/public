// $Header:   Q:/views/win/vcs/about.h_v   1.6   Mar 04 1997 16:31:46   gregm  $ 

//	about.h
//
//	VAbout class interface
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
//	07/25/93 jmd	added const
//	10/11/94 dss	Added operator().
//	11/03/94 jmd	Added iam()
//	11/13/96 pkt	modified for Web support.
// ----------------------------------------------------------------------------

#ifndef ABOUT_H
#define ABOUT_H

#include "dialog.h"

CLASS VTextBox;
CLASS VPushButton;
CLASS VButton;

//	The VAbout class is a modal dialog designed for the display of a
//	single message to the user in the format of a standard VAbout 
//	dialog box.  The VAbout class presents the message to the user 
//	through an instance of the VTextBox class that uses centered 
//	alignment.  A single VPushButton object is used to terminate 
//	the dialog.  
//
//	Note, the special characters	`\\t` (TAB), `\\r` or `\\n` 
//	(return/line feed) can be used to format the prompt message.

CLASS VAbout : public VDialog {

public:
	//	constructors/destructor

			VAbout();
			VAbout(const char *title, const char *message, VWindow *parent);
			~VAbout();

	VClass *iam();

	//	static dialog methods

	static void dialog(const char *message, VWindow *parent);
	static void dialog(const char *title, VWindow *parent, const char *message...);

	void operator()(const char *title, const char *message, VWindow *parent);
	
protected:
#ifndef CV_REMOTE_GUI
	VTextBox 	   *prompt;	// message
	VPushButton    *okbttn;
#endif
};

#endif	//	ABOUT_H

