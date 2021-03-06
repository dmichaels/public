// $Header:   Q:/views/common/vcs/exclgrp.cpv   1.15   Mar 24 1997 16:14:38   scottt  $

//	exclgrp.cpp
//
//	VExclusiveGroup class implementation [Common]
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
//	05/14/93 pat	added destructor (virtual)
//	06/21/93 pat	added support for VArchiver; preened a bit;
//					added constructor that takes name argument;
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 dgm	pass final argument to getChildrenFrom().
//	07/27/93 pat	removed free()
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	09/22/93 pat	added archive tag
//	09/27/93 dgm 	initialized type to VRadioButtonCls in default constructor.
//	10/05/93 dgm	fixed getFrom() for proper realization.
//	10/08/93 dgm	fix in realize().
//	10/20/93 dgm	3.0 changes.
//	01/31/94 jmd	minor preening, added setting of current in clicked
//	02/02/94 pat	added archive tag check
//	02/09/94 pat	updated defineArchiveTag use
//	06/02/94 dgm	Minor VString type-cast changes.
//	08/31/94 dgm	Added missing constructor definition.
//	11/11/94 dss	Archiver tags macros.
//	12/02/94 dgm	Fix for null type.
//	05/16/95 pkt	preened.
//	09/11/95 pkt	merged with Motif.
//	12/23/96 pkt	updated to compile on Web platform.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "wintypes.h"
#include "exclgrp.h"
#include "radibttn.h"

#ifdef MOTIF
#	include "ordcllct.h"
#	include "iterator.h"
#endif

#ifndef CV_NOARCHIVER
#	include "archiver.h"
#endif

// ---------------------------------------------------------------------------
//
defineClass(VExclusiveGroup, VInclusiveGroup)
defineArchiveRevision(VExclusiveGroup, 0)
defineCallback(VExclusiveGroup, clicked)

// ---------------------------------------------------------------------------
//
VExclusiveGroup::VExclusiveGroup() 
{ 
	type = VRadioButtonCls;
}

// ---------------------------------------------------------------------------
// Construct a VExclusiveGroup of buttons of class 'cls' with frame
// 'frame' and parent window 'win'.  If 'title' is non-null, then the group is
// created with that title.  If 'cls' is null, then VRadioButton buttons are
// assumed.
//
VExclusiveGroup::VExclusiveGroup(const VFrame& frame, VWindow *win, 
								 char *title, VClass *cls)
{
	if (cls == 0) {
		type = VRadioButtonCls;
	}
	else {
		type = cls;
	}
	(*this)(frame, win, StyleDefault);
	if (title != 0) {
		putText(VString(title));
	}
}

// ---------------------------------------------------------------------------
// Construct a named VExclusiveGroup of buttons of class 'cls' with frame
// 'frame' and parent window 'win'.  If 'title' is non-NIL, then the group is
// created with that title.  If 'cls' is NIL, then VRadioButton buttons are
// assumed.
//
VExclusiveGroup::VExclusiveGroup(const VString& wname, const VFrame& frame, 
								 VWindow *win, char *title, VClass *cls)
{
	if (cls == 0) {
		type = VRadioButtonCls;
	}
	else {
		type = cls;
	}
	(*this)(frame, win, StyleDefault, wname);
	if (title != 0) {
		putText(VString(title));
	}
}

#if (defined(CV_WINDOWS) || defined(PM)) && !defined(CV_REMOTE_GUI)
// ---------------------------------------------------------------------------
// Construct a VExclusiveGroup from the resource control-id 'cntrl'
// of the resource associated with 'pWin'. This is a non-portable GUI system
// method.
//
VExclusiveGroup::VExclusiveGroup(unsigned cntrl, VWindow *pWin) 
{
	getRscWindow(cntrl, pWin);
}

// ---------------------------------------------------------------------------
// Construct a VExclusiveGroup from the resource control-id 'cntrl'
// of the resource associated with 'pWin'. Add to this group the controls with
// id's from 'start' through 'start' + 'count' - 1. This is a non-portable
// GUI system method.
//
VExclusiveGroup::VExclusiveGroup(unsigned cntrl, unsigned start, unsigned count,
								 VWindow *pWin)
{
	getRscWindow(cntrl, pWin);
	unsigned i;
	for (i = 0; i < count; i++) {
		addControl(new VRadioButton(i + start, this));
	}
}
#endif

// ---------------------------------------------------------------------------
// Destroy this VExclusiveGroup.
//
VExclusiveGroup::~VExclusiveGroup()
{
	;
}

// ---------------------------------------------------------------------------
// Called when button 'button' is clicked in this VExclusiveGroup.
//
boolean VExclusiveGroup::clicked(VButton *button)
{
	int i;
	if (button == 0) {
		return FALSE;
	}
#ifdef MOTIF
	boolean skip = FALSE;	// pkt 09/11/95: Why not skip?
#else
	boolean skip = FALSE;
#endif

	if ((i = int(controls.indexOf(button))) >= 0 && !skip) {

#ifndef MOTIF
		// jmd 01/31/94
		// pkt 09/11/95: This doesn't appear to be generally applicable.
		if (current == 0) {
			current = (VControl *)children->idAt(0);
		}
#endif

		if ((current != 0) && (current != button)) {
			((VCheckBox *)current)->check(FALSE);
		}
		current = button;
		((VCheckBox *)current)->check(TRUE);
		notify(i);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VExclusiveGroup::arrowKey(int index)
{
	VWindow *child = (VWindow *)controls.idAt(index);
	if ((child != 0) && child->isA(VRadioButtonCls)) {
		if (((VRadioButton *)child)->notify(ACCELcode)) {
			notify(index);
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else {
		return VInclusiveGroup::arrowKey(index);
	}
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

// ---------------------------------------------------------------------------
// Put this object to the archive.  This method is called from 
// VArchiver::putObject().
//
void VExclusiveGroup::putTo(VArchiver& a)
{
	VExclusiveGroup::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
// Put this object to the archive by archiving the base class layer, 
// then the object data of this class layer.
//
// If 'most_derived' is FALSE, then this method has been called from a derived
// class.  That is, this class is a base to some derived class and we should
// not archive our children, if any.
//
// If 'most_derived' is TRUE, then this method is being called directly from a 
// VArchiver.  That is, this class is the most derived class layer for this
// object and we archive our children, if any.
//
void VExclusiveGroup::putTo(VArchiver& a, boolean most_derived)
{
	VInclusiveGroup::putTo(a, FALSE); 			// base object data

	a << VExclusiveGroupArchiveTag;

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
// Get this object from the archive by retrieving the object 
// data of the base class and then the object data of this class.
//
// This method is invoked by the getFrom() of this most-derived class or by
// the getFrom() of a derived class.
//
void VExclusiveGroup::getFrom(VArchiver& a)
{
	VInclusiveGroup::getFrom(a);		 			// base object data

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VExclusiveGroupArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VExclusiveGroupArchiveTag));
		return;
	}
}

// ---------------------------------------------------------------------------
// Get this object from the archive.  The 'data' parameter is a VWindow *
// that is our parent window.  In response, retrieve our object data,
// realize the presentation space under the parent, and finally, retrieve
// our children.
//
// This method is invoked by VArchiver::getObject().
//
void VExclusiveGroup::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VExclusiveGroup::getFrom(a);				

	if (do_realize) {
		VGroup::realize((VWindow *)data);	// sic (not VInclusiveGroup::)
	}

	if (data != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
#if defined(MOTIF)
	if (do_realize) {
		VGroup::addControlsFromChildIndices();
		DO (controls, VControl, control)
			if ((control != 0) && control->isA(VRadioButtonCls)) {
				VRadioButton *radiobutton = (VRadioButton *)control;
				radiobutton->
					uponClick(this, methodOf(VExclusiveGroup, clicked));
			}
		END
	}
#else
	if (data != 0 && do_realize) {
		VGroup::addControlsFromChildIndices();
	}
#endif
}

// ---------------------------------------------------------------------------
// Realize the presentation space of this object.  Upon entry, the
// Views object exists, but no GUI object.  First, the realize() method of
// our base class is invoked to build the GUI object, then we set any 
// additional object data that affects the GUI.
//
// This method is invoked by the getFrom() method of this most-derived 
// class or by the getFrom() of a derived class.
//
void VExclusiveGroup::realize(VWindow *pwin)
{
	VButtonGroup::realize(pwin);	// sic (not VInclusiveGroup::)
#if defined(MOTIF)
	DO (controls, VControl, control)
		if ((control != 0) && control->isA(VRadioButtonCls)) {
			VRadioButton *radiobutton = (VRadioButton *)control;
			radiobutton->uponClick(this, methodOf(VExclusiveGroup, clicked));
		}
	END
#endif
}

#endif // ARCHIVER	
