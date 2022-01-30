// $Header:   Q:/views/win/vcs/bttngrp.h_v   1.6   Mar 04 1997 16:31:58   gregm  $ 

//	bttngrp.h
//
//	VButtonGroup class interface
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
//	04/09/93 pmcm	changed id to VObject *
//	05/14/93 pmcm	added destructor (virtual)
//	06/21/93 pmcm	added VArchiver support, preened.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	08/09/93 jmd	made VFrame const
//  08/09/93 jmd	removed free
//	08/16/93 jmd	made name const VString
//	09/29/93 dgm	added addButtons() which take a VOrdCollect.
// ---------------------------------------------------------------------------

#ifndef VBTTNGRP_H
#define VBTTNGRP_H

#include "group.h"

extern VClass *VButtonGroupCls;

CLASS VButton;
CLASS VOrdCollect;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

//	Introduction
//
//	A VButtonGroup is an abstract class representing an ordered 
//	collection of buttons.  It has no presentation window, nor does it 
//	enforce any protocol over the buttons in the collection beyond 
//	that of VGroup. These properties are implemented by classes 
//	derived from VButtonGroup.  All of the buttons in the group 
//	should be of the same type.

CLASS VButtonGroup : public VGroup {

public:

					VButtonGroup();
					VButtonGroup(	unsigned cntrlId, VWindow *pwin);
					VButtonGroup(	VClass *cls);

					VButtonGroup(	const VString& name,
									const VFrame& frame, VWindow *pwin, 
									char *title, VClass *cls);

					VButtonGroup(	const VFrame& frame, VWindow *pwin, 
									char *title, VClass *cls);

					~VButtonGroup();

	VClass 		   *iam();

	//	Adding Buttons

	void 			addButtons(	char **buttonLabels, 
								VObject *buttonClient, 
								boolean individualCallbacks, 
								method *callbackArray, 
								float x, float y, float w, float h, 
								float dx, float dy);

	void 			addButtons(	char **buttonLabels, 
								VObject *buttonClient, 
								boolean individualCallbacks, 
								method *callbackArray, 
								int x, int y, int w, int h, 
								int dx, int dy);

	void 			addButtons(	char **buttonLabels, 
								VObject *buttonClient, 
								boolean individualCallbacks, 
								method *callbackArray, 
								int x, int y, 
								int dx, int dy);

	void 			addButtons(	VOrdCollect& buttonLabels, 
								VObject *buttonClient, 
								method callback, 
								float x, float y, float w, float h, 
								float dx, float dy);

	void 			addButtons(	VOrdCollect& buttonLabels, 
								VObject *buttonClient, 
								method callback, 
								int x, int y, int w, int h, 
								int dx, int dy);

	void 			addButtons(	VOrdCollect& buttonLabels, 
								VObject *buttonClient, 
								method callback, 
								int x, int y, 
								int dx, int dy);

	//	Enabling Buttons 

    virtual void enableBool(boolean b);
	virtual boolean	enableAll(boolean tf);
	virtual boolean	enableAt(boolean tf, int i);


#ifndef CV_NOARCHIVER
	//	Archival and Retrieval

	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

	VClass *type;	//	class of buttons in the group

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

};

#endif // VBTTNGRP_H
