// $Header:   Q:/views/win/vcs/bttngrp.cpv   1.12   Mar 04 1997 17:16:44   gregm  $ 

//	bttngrp.cpp
//
//	VButtonGroup implementation
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
//	05/14/93 pat	added destructor (virtual)
//	06/21/93 pat	added VArchiver support, preened.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 dgm	pass final argument to getChildrenFrom().
//	08/09/93 jmd	made VFrame const
//  08/09/93 jmd	removed free
//	08/23/93 pat	removed reference to VGlobal classTable in call to
//					static VClass::of()
//	09/22/93 pat	added archive tag
//	09/29/93 dgm	added addButtons() which take a VOrdCollect.
//	10/05/93 dgm	fixed getFrom() for proper realization.
//	01/18/94 pat	included str.h when ARCHIVER is not defined
//  02/02/94 pat 	added archive tag check
//  02/09/94 pat 	updated defineArchiveTag use
//	11/11/94 dss	Archiver tags macros.
//	09/01/95 dss	VStyle changes.
//	01/29/96 dgm	Added missing constructor.
//	05/02/96 pkt	bulletproofed 'addButtons' against an ordered collection
//					of names that includes a null object pointer.
//	12/23/96 pkt	updated to compile on Web platform.
// ----------------------------------------------------------------------------

#include "bttngrp.h"
#include "button.h"
#include "checkbox.h"
#include "ordcllct.h"
#include "iterator.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "classtbl.h"
#else
#include "str.h"
#endif

extern "C" {
#include <string.h>
}

defineClass(VButtonGroup, VGroup)
defineArchiveRevision(VButtonGroup, 0)

// ---------------------------------------------------------------------------
//	Construct a VButtonGroup with no presentation window.
//
VButtonGroup::VButtonGroup() : type(0)
{ 
}

// --------------------------------------------------------------------------
//	Construct a VButtonGroup from the resource control-id 'cntrlId' of
//	the resource associated with 'pwin'. This is a non-portable GUI 
//	system method.
//
VButtonGroup::VButtonGroup(unsigned cntrlId, VWindow *pwin) : type(0)
{ 
#ifndef CV_REMOTE_GUI
	getRscWindow(cntrlId, pwin);
#endif
}

// --------------------------------------------------------------------------
//	Construct a VButtonGroup of buttons of class 'cls'.
//
VButtonGroup::VButtonGroup(VClass *cls) : type(cls)
{ 
}

// --------------------------------------------------------------------------
//	Construct a VButtonGroup with frame 'frame', parent 'win', label
//	'label', to hold buttons of class 'cls'.
//
VButtonGroup::VButtonGroup(const VFrame &frame, VWindow *win, 
							char *title, VClass *cls) : type(cls)
{
	(*this)(frame, win);
	if (title) {
		putText(title);
	}
}

// --------------------------------------------------------------------------
//
VButtonGroup::VButtonGroup(const VString& name,
						   const VFrame &frame, VWindow *win, 
						   char *title, VClass *cls) : type(cls)
{
	(*this)(frame, win, StyleDefault, name);
	if (title) {
		putText(title);
	}
}

// ---------------------------------------------------------------------------
//	Destroy this VButtonGroup.
//
VButtonGroup::~VButtonGroup()
{
	;
}

// -----------------------------------------------------------------------
// Mirror the enable state of the group with those of the buttons
//
void VButtonGroup::enableBool(boolean b)
{
   VWindow::enableBool(b);

   enableAll(b);

   return;
}

// --------------------------------------------------------------------------
//	Enable or disable all of the buttons in this VButtonGroup if
//	'b' is TRUE or FALSE respectively.
//
boolean VButtonGroup::enableAll(boolean b)
{
	controls.doPerform(methodOf(VButton, enable), b);
	return(TRUE);
}

// --------------------------------------------------------------------------
//	Add additional buttons to this VButtonGroup, each with client
//	'client'. The 'ith' button will have label 'names[i]'. The group is
//	created starting at screen position ('x','y') and will be arranged
//	vertically if 'dX' is non-zero separated by 'dX' units. It will be
//	arranged horizontally if 'dY' is non-zero, separated by 'dY' units.
//	The buttons are of default size. 'Names' is an array of strings to be
//	used as labels for the buttons. The number of buttons is determined by
//	the number of strings in 'names'. If 'individualCallbacks' is FALSE,
//	make the click method of each of the buttons '*callbackArray',
//	otherwise 'callbackArray' is assumed to point to an array of methods
//	to be used as click methods. The type of button is determined by the
//	VClass object passed in to the constructor.
//
void VButtonGroup::addButtons(	char 	  **names, 
								VObject    *client, 
								boolean 	individualCallbacks, 
								method 	   *callbackArray, 
								int x, 		int y, 	
								int dX,		int dY)
{
	VButton *but;
	int i;
	int xPos, yPos;
	int w, h;

	xPos = x;
	yPos = y;
	for (i = 0; names[i]; i++) {
		but = (VButton *)type->construct();
		w = strlen(names[i]);
		h = 1;
		but->sizeOf(&w, &h);
		(*but)(VFrame(xPos, yPos, w, h), (VWindow *)this, StyleDefault);
		if (dX)  {
			xPos += w + dX;
		}
		if (dY) {
			yPos += h + dY;
		}
		but->putText(names[i]);
		but->uponClick(client, callbackArray[individualCallbacks ? i: 0]);
		controls.add(but);
	}  
}

void VButtonGroup::addButtons(VOrdCollect& names, 
							  VObject *client, 
							  method callback, 
							  int x, int y, int dX, int dY)
{
	int xPos = x;
	int yPos = y;
	DO (names, VString, name)
		if (name != 0) {
			VButton *but = (VButton *)type->construct();
			int w = strlen( name->gets() );
			int h = 1;
			but->sizeOf(&w, &h);
			(*but)(VFrame(xPos, yPos, w, h), (VWindow *)this, StyleDefault);
			if (dX)  {
				xPos += w + dX;
			}
			if (dY) {
				yPos += h + dY;
			}
			but->putText( name->gets() );
			but->uponClick(client, callback);
			controls.add(but);
		}
	END
}

// --------------------------------------------------------------------------
//	Add additonal VButtons to this VButtonGroup starting with position
//	'x', 'y' within 'parent' and of size 'w','h'. Step 'dX', 'dY' between
//	buttons. 'Names' is an array of strings to be used as labels for the
//	buttons. The number of buttons is determined by the number of strings
//	in 'names'. If 'individualCallbacks' is TRUE, make the click method of
//	each of the buttons '*callbackArray', otherwise 'callbackArray' is
//	assumed to point to an array of methods to be used as click methods.
//	The type of button is determined by the VClass object passed in to
//	the constructor.
//
void VButtonGroup::addButtons(	char 	  **names, 
								VObject    *client, 
								boolean 	individualCallbacks, 
								method 	   *callbackArray, 
								int x, 		int y, 
								int w, 		int h, 
								int dX, 	int dY)
{
	VButton *but;
	int i;
	int xPos, yPos;

	xPos = x;
	yPos = y;
	for (i = 0; names[i]; i++) {
		but = (VButton *)type->construct();
		(*but)(VFrame(xPos, yPos, w, h), (VWindow *)this, StyleDefault);
		if (dX) {
			xPos += w + dX;
		}
		if (dY) {
			yPos += h + dY;
		}
		but->putText(names[i]);
		but->uponClick(client, callbackArray[individualCallbacks ? i: 0]);
		controls.add(but);
	}  
}

void VButtonGroup::addButtons(VOrdCollect& names,
							  VObject *client, 
							  method callback, 
							  int x, int y, int w, int h, int dX, int dY)
{
	int xPos = x;
	int yPos = y;
	DO (names, VString, name)
		if (name != 0) {
			VButton *but = (VButton *)type->construct();
			(*but)(VFrame(xPos, yPos, w, h), (VWindow *)this, StyleDefault);
			if (dX) {
				xPos += w + dX;
			}
			if (dY) {
				yPos += h + dY;
			}
			but->putText( name->gets() );
			but->uponClick(client, callback);
			controls.add(but);
		}
	END
}

// --------------------------------------------------------------------------
//	Create an array of VButton objects starting with relative position
//	'x', 'y' within 'parent'. Make the buttons size 'w','h' relative to
//	their parent. Between buttons, step 'dX', 'dY'. 'Names' is an array of
//	strings to be used as labels for the buttons. The number of buttons is
//	determined by the number of strings in 'names'. If
//	'individualCallbacks' is TRUE, make the click method of each of the
//	buttons '*callbackArray', otherwise 'callbackArray' is assumed to
//	point to an array of methods to be used as click methods. The type of
//	button is determined by the VClass object passed in to the
//	constructor.
//	
void VButtonGroup::addButtons(	char 	  **names, 
								VObject    *client, 
								boolean 	individualCallbacks, 	
								method 	   *callbackArray, 
								float x, 	float y, 
								float w, 	float h, 
								float dX, 	float dY)
{
	VButton *but;
	int i;
	float xPos, yPos;

	xPos = x;
	yPos = y;
	for (i = 0; names[i]; i++) {
		but = (VButton *)type->construct();
		(*but)(VFrame(xPos, yPos, w, h), (VWindow *)this, StyleDefault);
		if (dY) {
			yPos += h + dY;
		}
		if (dX) {
			xPos += w + dX;
		}
		but->putText(names[i]);
		but->uponClick(client, callbackArray[individualCallbacks ? i: 0]);
		controls.add(but);
	}
}
void VButtonGroup::addButtons(VOrdCollect& names,
							  VObject *client, 
							  method callback, 
							  float x, float y, float w, float h, 
							  float dX, float dY)
{
	float xPos = x;
	float yPos = y;
	DO (names, VString, name)
		if (name != 0) {
			VButton *but = (VButton *)type->construct();
			(*but)(VFrame(xPos, yPos, w, h), (VWindow *)this, StyleDefault);
			if (dY) {
				yPos += h + dY;
			}
			if (dX) {
				xPos += w + dX;
			}
			but->putText( name->gets() );
			but->uponClick(client, callback);
			controls.add(but);
		}
	END
}

// --------------------------------------------------------------------------
//	Enable or disable the 'ith' button in this VButtonGroup if
//	'b' is TRUE or FALSE respectively.
//
boolean VButtonGroup::enableAt(boolean b, int i)
{
	VButton *but = (VButton *)controls.idAt(i);
	if (but) {
		but->enable(b);
	}
	return(TRUE);
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

// --------------------------------------------------------------------------
//
void VButtonGroup::putTo(VArchiver& a)
{
	VButtonGroup::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//
void VButtonGroup::putTo(VArchiver& a, boolean most_derived)
{
	VGroup::putTo(a, FALSE); 				//	base object data
	
	a << VButtonGroupArchiveTag;

	const char * classNameOfMembers = (type == 0) ? 0 : type->nameOf();
 	a << classNameOfMembers;
}

// --------------------------------------------------------------------------
//
void VButtonGroup::getFrom(VArchiver& a)
{
	VGroup::getFrom(a);						//	base object data

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VButtonGroupArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VButtonGroupArchiveTag));
		return;
	}

	char classNameOfMembers[100];
	char *classNameOfMembersp = classNameOfMembers;
	a >> classNameOfMembersp;
	type = VClass::of(classNameOfMembersp);
}

// --------------------------------------------------------------------------
//
void VButtonGroup::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VButtonGroup::getFrom(a);				

	if (do_realize) {
		VGroup::realize((VWindow *)data);	// sic
	}

	if (data != 0) {
		//	(this is an abstract class, but what the heck)
		VWindow::getChildrenFrom(a, do_realize);
		if (do_realize) {
			VGroup::addControlsFromChildIndices();
		}
	}
}

// --------------------------------------------------------------------------
//
void VButtonGroup::realize(VWindow *pwin)
{
	VGroup::realize(pwin);
	VGroup::addControlsFromChildIndices();
}

// --------------------------------------------------------------------------

#endif //	ARCHIVER	
// ===========================================================================
