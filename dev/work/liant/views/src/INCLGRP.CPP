// $Header:   Q:/views/win/vcs/inclgrp.cpv   1.11   Mar 04 1997 17:55:14   gregm  $ 

//	inclgrp.cpp
//
//	VInclusiveGroup implementation
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
//	09/10/92 jmd	made MSC ifdefs use >= instead of ==
//	04/07/93 pat	changed id to VObject *
//	06/22/93 pat	added VArchiver support;
//					added defineCallback for VInclusiveGroup::clicked();
//					added constructor with name argument; preened.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pat	added changedMthdIndex;
//					removed free();
//					updated findCallback() calls
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	09/02/93 pat	included cllbckls.h, callback.h;
//					updated findCallback() calls to use VCallbackList
//	09/22/93 pat 	added archive tag
//	09/27/93 dgm 	initialized type to VCheckBoxCls in default constructor.
//	09/29/93 dgm	added addButtons() which take a VOrdCollect.
//	10/05/93 dgm	fixed getFrom() for proper realization.
//	10/07/93 dgm	minor fix in getFrom().
//	10/07/93 dgm	added:	setCurrent(int)
//							givenFocus()
//							nextChildFocus(VWindow *)
//							prevChildFocus(VWindow *)
//	10/08/93 dgm	fix in realize();
//	10/11/93 jmd	minor preening, fixed setCurrent problems,
//					added test for children = 0
//  02/02/94 pat	added archive tag check
//  02/09/94 pat 	added ClientCallbackMismatch check
//  02/09/94 pat	updated defineArchiveTag use
//  03/08/94 jmd	added casts for SC61 problems
//	11/11/94 dss	Archiver tags macros.
//	09/01/95 dss	VStyle changes.
//	04/19/96 glm	removed v1.1 code/preened
//	12/23/96 pkt	updated to compile on Web platform.
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
// ----------------------------------------------------------------------------

#include "inclgrp.h"
#include "checkbox.h"
#include "ordcllct.h"
#include "iterator.h"

#ifndef CV_NOARCHIVER
#	include "archiver.h"
#	include "cllbckls.h"
#	include "callback.h"
#endif

defineClass(VInclusiveGroup, VButtonGroup)
defineArchiveRevision(VInclusiveGroup, 0)

// --------------------------------------------------------------------------
//
#ifndef CV_NOARCHIVER
defineCallback(VInclusiveGroup, clicked)
#endif

// --------------------------------------------------------------------------
//	Construct an VInclusiveGroup with no presentation window.
//
VInclusiveGroup::VInclusiveGroup() 
#ifndef CV_NOARCHIVER
				:	changedMthdIndex(-1L)
#endif
{ 
	changedMthd = 0;
	current = 0;
	type = VCheckBoxCls;
}

// --------------------------------------------------------------------------
//	Construct an VInclusiveGroup with frame 'frame' in parent 'win' with
//	title 'title'. Buttons of class 'cls' are to be put into this VInclusiveGroup. 
//
VInclusiveGroup::VInclusiveGroup(const VFrame& frame, VWindow *win, 
									char* title, VClass *cls)
#ifndef CV_NOARCHIVER
				:	changedMthdIndex(-1L)
#endif
{
	if (!cls) {
		cls = VCheckBoxCls;
	}
	type = cls;
	(*this)(frame, win);
	if (title) {
		putText(title);
	}
}

// --------------------------------------------------------------------------
// Construct an namedVInclusiveGroup with frame 'frame' in parent 'win' with
// title 'title'. Buttons of class 'cls' are to be put into this 
// VInclusiveGroup. 
//
VInclusiveGroup::VInclusiveGroup(const VString& wname, const VFrame& frame, VWindow *win, 
									char* title, VClass *cls)
#ifndef CV_NOARCHIVER
				:	changedMthdIndex(-1L)
#endif
{
	if (!cls) {
		cls = VCheckBoxCls;
	}
	type = cls;
	(*this)(frame, win, StyleDefault, wname);
	if (title) {
		putText(title);
	}
}

// --------------------------------------------------------------------------
//	Construct a VInclusiveGroup from the resource control-id 'cntrl'
//	of the resource associated with 'pWin'. This is a non-portable GUI system
//	method.
//
VInclusiveGroup::VInclusiveGroup(unsigned cntrl, VWindow *pWin) 
#ifndef CV_NOARCHIVER
				:	changedMthdIndex(-1L)
#endif
{ 
#ifndef CV_REMOTE_GUI
	getRscWindow(cntrl, pWin);
#endif
}

// ---------------------------------------------------------------------------
//
//	Construct a VInclusiveGroup from the resource control-id 'cntrl'
//	of the resource associated with 'pWin'. Add to this group the controls with
//	id\'s from 'start' through 'start' + 'count' - 1. This is a non-portable
//	GUI system method.
//	
VInclusiveGroup::VInclusiveGroup(unsigned cntrl, unsigned start, unsigned count, 
					VWindow *pWin) 
				: 	VButtonGroup(cntrl, pWin)
#ifndef CV_NOARCHIVER
					, changedMthdIndex(-1L)
#endif
{
	unsigned i;
	for (i = 0; i < count; i++) {
		addControl(new VCheckBox(i + start, this));
	}
}

// ---------------------------------------------------------------------------
//
VInclusiveGroup::~VInclusiveGroup()
{
	current = 0;
}

// --------------------------------------------------------------------------
//	Add VControl 'cntrl' to this Inclusivegroup.
//
void VInclusiveGroup::addControl(VControl *cntrl)
{
	VGroup::addControl(cntrl);

	if (!cntrl->isA(VButtonCls)) {
		return;
	}

	VButton *but = (VButton *)cntrl;

#if _MSC_VER >= 700
	but->uponClick(this, methodOf(VInclusiveGroup, clicked));
#else
	boolean  (VInclusiveGroup::*pmf)(VButton *) = &VInclusiveGroup::clicked;
	but->uponClick(this, (method)pmf);
#endif

	if (but->isA(VCheckBoxCls)) {
		but->autoToggle(FALSE);
	}
}

// --------------------------------------------------------------------------
//	Add a group of Buttons to this VGroup as per VButtonGroup::addButtons.
//
void VInclusiveGroup::addButtons(	char **buttonLabels, 
									float x, float y, float w, float h, 
									float dX, float dY)
{
#if _MSC_VER >= 700 || defined(__SC__)
	method mthd = methodOf(VInclusiveGroup, clicked);
#else
	boolean  (VInclusiveGroup::*pmf)(VButton *) = &VInclusiveGroup::clicked;
	method mthd = (method)pmf;
#endif

	VButtonGroup::addButtons(buttonLabels, (VObject *)this, FALSE,
							 &mthd, x, y, w, h,	dX, dY);

	if (type->isa(VCheckBoxCls)) {
		DO (controls, VCheckBox, cb)
			cb->autoToggle(FALSE);
		END 
		//controls.doPerform(methodOf(VCheckBox,autoToggle),FALSE);
	}
}

// --------------------------------------------------------------------------
void VInclusiveGroup::addButtons(VOrdCollect& buttonLabels,
								 float x, float y, float w, float h, 
								 float dX, float dY)
{
	VButtonGroup::addButtons(buttonLabels, (VObject *)this, 
							 methodOf(VInclusiveGroup, clicked),
							 x, y, w, h, dX, dY);

	if (type->isa(VCheckBoxCls)) {
		DO (controls, VCheckBox, cb)
			cb->autoToggle(FALSE);
		END 
	}
}

// --------------------------------------------------------------------------
//	Add a group of Buttons to this VGroup as per VButtonGroup::addButtons.
//
void VInclusiveGroup::addButtons(	char **buttonLabels, 
									int x, int y, int w, int h, 
									int dX, int dY)
{
#if _MSC_VER >= 700
	method mthd = methodOf(VInclusiveGroup, clicked);
#else
	boolean  (VInclusiveGroup::*pmf)(VButton *) = &VInclusiveGroup::clicked;
	method mthd = (method)pmf;
#endif

	VButtonGroup::addButtons(buttonLabels, (VObject *)this, 
								FALSE, &mthd, x, y, w, h, dX, dY);

	if (type->isa(VCheckBoxCls)) {
		DO (controls, VCheckBox, cb)
			cb->autoToggle(FALSE);
		END 
		//controls.doPerform(methodOf(VCheckBox,autoToggle),FALSE);
	}
}

// --------------------------------------------------------------------------
void VInclusiveGroup::addButtons(VOrdCollect& buttonLabels, 
								 int x, int y, int w, int h, 
								 int dX, int dY)
{
	VButtonGroup::addButtons(buttonLabels, (VObject *) this, 
							 methodOf(VInclusiveGroup, clicked),
							 x, y, w, h, dX, dY);

	if (type->isa(VCheckBoxCls)) {
		DO (controls, VCheckBox, cb)
			cb->autoToggle(FALSE);
		END 
	}
}

// --------------------------------------------------------------------------
//	Add a group of Buttons to this VGroup as per VButtonGroup::addButtons.
//
void VInclusiveGroup::addButtons(	char **buttonLabels, 
									int x, int y, 
									int dX, int dY)
{
#if _MSC_VER >= 700
	method mthd = methodOf(VInclusiveGroup, clicked);
#else
	boolean  (VInclusiveGroup::*pmf)(VButton *) = &VInclusiveGroup::clicked;
	method mthd = (method)pmf;
#endif

	VButtonGroup::addButtons(buttonLabels, (VObject *)this, 
								FALSE, &mthd, x, y, dX, dY);

	if (type->isa(VCheckBoxCls)) {
		DO (controls, VCheckBox, cb)
			cb->autoToggle(FALSE);
		END 
		//controls.doPerform(methodOf(VCheckBox,autoToggle),FALSE);
	}
}

// --------------------------------------------------------------------------
void VInclusiveGroup::addButtons(VOrdCollect& buttonLabels, 
								 int x, int y, 
								 int dX, int dY)
{
	VButtonGroup::addButtons(buttonLabels, this, 
							 methodOf(VInclusiveGroup, clicked),
							 x, y, dX, dY);
	if (type->isa(VCheckBoxCls)) {
		DO (controls, VCheckBox, cb)
			cb->autoToggle(FALSE);
		END 
	}
}

// --------------------------------------------------------------------------
//	Called when the VButton object 'button' within this group was
//	clicked on.
//
boolean VInclusiveGroup::clicked(VButton *button)
{
	int i;

	if (button && (i = (int)controls.indexOf(button)) >= 0) {
		button->toggle();
		current = button;
		notify(i);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//	Called when the this VInclusiveGroup's client object is to be
//	notified of a button click.
//
boolean VInclusiveGroup::notify(int code)
{
	if (code == ACCELcode) {
		if (!VWindow::checkLosingFocus()) {
			if (children == 0) {
				return TRUE;
			}
			if (current == 0) {
				current = (VControl *)children->idAt(0);
			}
			current->takeFocus();
			return TRUE;
		}
	}
	if (code >= 0 && client != 0 && changedMthd != NIL_METHOD) {
		client->perform(changedMthd, (integer)code);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//	Arrange to call client 'clnt' with method 'mthd' upon a click
//	on a button in this VGroup. 
//	
void VInclusiveGroup::uponClick(VObject *clnt, method mthd) 
{ 
	if (clnt) {
		client = clnt;
	}
	if (mthd) {
		changedMthd = mthd; 
	}
	if (mthd == NIL_METHOD && !clnt) {
		client = 0;
		changedMthd = 0;
	}
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

// --------------------------------------------------------------------------
//	Put this object to the archive.  This method is called from 
//	VArchiver::putObject().
//
void VInclusiveGroup::putTo(VArchiver& a)
{
	VInclusiveGroup::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//	Put this object to the archive by archiving the base class layer, 
//	then the object data of this class layer.
//
//	If 'most_derived' is FALSE, then this method has been called from a derived
//	class.  That is, this class is a base to some derived class and we should
//	not archive our children, if any.
//
//	If 'most_derived' is TRUE, then this method is being called directly from a 
//	VArchiver.  That is, this class is the most derived class layer for this 
//	object and we archive our children, if any.
//
void VInclusiveGroup::putTo(VArchiver& a, boolean most_derived)
{
	VButtonGroup::putTo(a, FALSE); 			//	base object data

	a << VInclusiveGroupArchiveTag;

	//	callback for when any of the group's buttons change

	const char *changed_class_name 	= 0;			
	const char *changed_method_name	= 0;			

	VCallbackList::findCallback(changedMthd, changed_class_name, 
										changed_method_name,
										changedMthdIndex);

	a << (char *) changed_class_name;		//	changed method's class name
	a << (char *) changed_method_name;		//	changed method name

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// --------------------------------------------------------------------------
//	Get this object from the archive by retrieving the object 
//	data of the base class and then the object data of this class.
//
//	This method is invoked by the getFrom() of this most-derived class or by
//	the getFrom() of a derived class.
//
void VInclusiveGroup::getFrom(VArchiver& a)
{
	VButtonGroup::getFrom(a);		 	 	//	base object data
	
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VInclusiveGroupArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VInclusiveGroupArchiveTag));
		return;
	}

	//	callback for when any of the group's buttons change

	char		changed_method_name[256];
	char *		changed_method_name_ptr = changed_method_name;
	char		changed_class_name[256];
	char *		changed_class_name_ptr = changed_class_name;

	a >> changed_class_name_ptr;  			//	changed method's class name
	a >> changed_method_name_ptr; 			//	changed method name

	changedMthd = VCallbackList::findCallback(changed_class_name_ptr, 
		 				  			changed_method_name_ptr, &changedMthdIndex);

	if (client != 0 && changedMthd != 0) {
		if (!client->isA(VClass::of(changed_class_name_ptr))) {
			a.abort(VArchiver::ClientCallbackMismatch);
			return;
		}
	}
}

// --------------------------------------------------------------------------
//	Get this object from the archive.  The 'data' parameter is a VWindow *
//	that is our parent window.  In response, retrieve our object data,
//	realize the presentation space under the parent, and finally, retrieve
//	our children.
//
//	This method is invoked by VArchiver::getObject().
//
void VInclusiveGroup::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VInclusiveGroup::getFrom(a);

	if (do_realize) {
		VGroup::realize((VWindow *)data);	// sic
	}

	if (data != 0) {
		VWindow::getChildrenFrom(a, do_realize);
		if (do_realize) {
			VGroup::addControlsFromChildIndices();
		}
	}
}

// --------------------------------------------------------------------------
//	Realize the presentation space of this object.  Upon entry, the
//	Views object exists, but no GUI object.  First, the realize() method of 
//	our base class is invoked to build the GUI object, then we set any 
//	additional object data that affects the GUI.
//
//	This method is invoked by the getFrom() method of this most-derived 
//	class or by the getFrom() of a derived class.
//
void VInclusiveGroup::realize(VWindow *pwin)
{
	VButtonGroup::realize(pwin);
	DO (controls, VCheckBox, checkbox)
		checkbox->autoToggle(TRUE);
	END
}

#endif //	ARCHIVER	

