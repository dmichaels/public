// $Header:   Q:/views/win/vcs/inclgrp.h_v   1.7   Mar 04 1997 16:32:30   gregm  $ 

//	inclgrp.h
//
//	VInclusiveGroup interface
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
//	04/09/93 pat	changed id to VObject *
//	06/22/93 pat	added VArchiver support, preened;
//					added constructor with name argument.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pat	added changedMthdIndex; 
//					removed free().
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	09/29/93 dgm	added addButtons() which take a VOrdCollect.
//	10/07/93 dgm	added:	setCurrent(int)
//							givenFocus()
//							nextChildFocus(VWindow *)
//							prevChildFocus(VWindow *)
//							getClickedMethodIndex()
//							setClickedMethodIndex()
//	12/27/93 jmd	added ARCHIVER ifdef for inline functions
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#ifndef VINCLGRP_H
#define VINCLGRP_H

#include "bttngrp.h"

CLASS VOrdCollect;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VInclusiveGroupCls;
extern VClass *VCheckBoxCls;

//	Introduction
//
//	An VInclusiveGroup is a VGroup of VButtons arranged in a presentation
//	window. If the button is a subclass of VCheckBox, then more than one
//	button may be checked at a time. VInclusiveGroup objects receive all
//	click callbacks from their contained buttons and then respond to their
//	client with the index of the button that was clicked on.
//	VInclusiveGroup objects manage focus passing between buttons. The user
//	can use TAB and SHIFT-TAB to move the focus between buttons of the
//	VInclusiveGroup. By default, VCheckBox buttons are used in
//	VInclusiveGroups.

CLASS VInclusiveGroup : public VButtonGroup {

public:

	//	Constructors
				VInclusiveGroup();
				VInclusiveGroup(const VFrame& frame, VWindow *pwin, 
								char *title = 0, 
								VClass *cls = VCheckBoxCls);
				VInclusiveGroup(const VString& wname,
								const VFrame& frame, VWindow *pwin, 
								char *title = 0, 
								VClass *cls = VCheckBoxCls);
				VInclusiveGroup(unsigned cntrlId, 
								VWindow *pwin);
				VInclusiveGroup(unsigned cntrlId, 
								unsigned start, unsigned count, 
								VWindow *pwin);
				~VInclusiveGroup();

	VClass 		   *iam();

	//	Interrogating and Setting the Current VButton

	void 			uponClick(	VObject *obj, method m);

	//	Adding Buttons

	void 			addControl(	VControl *obj);

	void 			addButtons(	char **buttonLabels, 
								float x, float y, float w, float h, 
								float dX, float dY);

	void 			addButtons(	char **buttonLabels, 
								int x, int y, int w, int h, 
								int dX, int dY);

	void 			addButtons(	char **buttonLabels, 
								int x, int y, 
								int dX, int dY);

	void 			addButtons(	VOrdCollect& buttonLabels,
								float x, float y, float w, float h, 
								float dX, float dY);

	void 			addButtons(	VOrdCollect& buttonLabels,
								int x, int y, int w, int h, 
								int dX, int dY);

	void 			addButtons(	VOrdCollect& buttonLabels,
								int x, int y, 
								int dX, int dY);

	//	Event Handling

	virtual boolean clicked(VButton *obj);
	boolean clickedjunk(VButton *obj);

#ifndef CV_NOARCHIVER
	//	Archival and Retrieval

	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);

	integer			getClickedMethodIndex() const;
	void			setClickedMethodIndex(integer);
#endif

protected:

	method 			changedMthd;

	boolean 		notify(int code);
#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

private:

#ifndef CV_NOARCHIVER
	integer			changedMthdIndex;
#endif

};

// ==========================================================================
#ifndef CV_NOARCHIVER

//----------------------------------------------------------------------------
inline integer VInclusiveGroup::getClickedMethodIndex() const
{
	return changedMthdIndex;
}
//----------------------------------------------------------------------------
inline void VInclusiveGroup::setClickedMethodIndex(integer index)
{
	changedMthdIndex = index;
}
//----------------------------------------------------------------------------

#endif
// ==========================================================================

#endif // VINCLGRP_H
