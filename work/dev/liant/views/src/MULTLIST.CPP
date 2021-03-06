// $Header:   Q:/views/win/vcs/multlist.cpv   1.11   Mar 04 1997 17:55:26   gregm  $ 

//	multlist.cpp
//
//	VMultiListBox implementation.
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
//	06/21/93 pat	added support for VArchiver; 
//					added constructor with name argument;
//					added virtual VString *getString(int i) for realize();
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 dgm	pass final argument to getChildrenFrom().
//	07/27/93 pat	removed free()
//	08/09/93 jmd	made VFrame const
//	09/22/93 pat	added archive tag
//	10/12/93 pat	initialized iSelects and nSelects realization data.
//  01/12/94 jmd	made getStringAt public
//  02/02/94 pat	added archive tag check
//  02/09/94 pat	updated defineArchiveTag use
//	03/26/94 pat	CV_WIN32: cast appropriately for SendMessage().
//	08/18/94 jld	extended selectItem() and deselectItem() to ranges
//	11/11/94 dss	Archiver tags macros.
//	07/18/95 dgm	Changed selectedXXX() method to const.
//	09/01/95 dss	VStyle changes.
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#include "multlist.h"
#include "notifier.h"
#include "str.h"
#include "archiver.h"

#ifndef CV_NOARCHIVER
#endif

defineClass(VMultiListBox, VListBox)
defineArchiveRevision(VMultiListBox, 0)

// ---------------------------------------------------------------------------
//	Destroy this VMultiListBox.
//
VMultiListBox::~VMultiListBox()
{
	;
}

// --------------------------------------------------------------------------
//	Construct a VMultiListBox from the resource control-id 'cntrl' of
//	the resource associated with 'pWin'. This is a non-portable 
//  GUI system method.
//
VMultiListBox::VMultiListBox(unsigned cntrl, VWindow *pWin) 
#ifndef CV_NOARCHIVER
	:	iSelects(0),
		nSelects(0)
#endif
{
	getRscWindow(cntrl, pWin);
}

// --------------------------------------------------------------------------
//	Construct a VMultiListBox with frame 'frame', parent 'win' and style
//	'style'. Styles are the same as for VListBox.
//
VMultiListBox::VMultiListBox(const VFrame& frame, VWindow *win, 
								const VStyle& style) 
#ifndef CV_NOARCHIVER
	:	iSelects(0),
		nSelects(0)
#endif
{
	(*this)(frame, win, style);
}

// --------------------------------------------------------------------------
//	Construct a VMultiListBox with frame 'frame', parent 'win' and style
//	'style'. Styles are the same as for VListBox.
//
VMultiListBox::VMultiListBox(char *name, const VFrame& frame, VWindow *win, 
								const VStyle& style) 
#ifndef CV_NOARCHIVER
	:	iSelects(0),
		nSelects(0)
#endif
{
	(*this)(frame, win, style, name);
}

// --------------------------------------------------------------------------
// Return MS Windows Information.
//
void VMultiListBox::getInfo(VWinInfo &listinfo)
{
	VListBox::getInfo(listinfo);
	listinfo.style |= LBS_EXTENDEDSEL;
}

// --------------------------------------------------------------------------
//	Return TRUE if selection 'n' is selected, otherwise return FALSE.
//
boolean VMultiListBox::selected(int n) const
{
	return(boolean)SendMessage(hWnd, LB_GETSEL, (WPARAM) n, (LPARAM) 0);
}

// --------------------------------------------------------------------------
//	Return the number of currently selected items of this VMultiListBox.
//
int VMultiListBox::selectedCount() const
{
	return(int)SendMessage(hWnd, LB_GETSELCOUNT, (WPARAM) 0, (LPARAM) 0);
}

// --------------------------------------------------------------------------
//	Return the index of the 'n'th currently selected item of this
//  VMultiListBox.
//
int VMultiListBox::selectedItem(int n) const
{
	int idx;
	int max = selectedCount();
	if (n >= max || n < 0) {
		return(-1);
	}
	int *ip = new int[max];
	if (SendMessage(hWnd, LB_GETSELITEMS, (WPARAM) max, (LPARAM) ip) > 0) {
		idx = ip[n];
	}
	else {
		idx = -1;
	}
	delete ip;
	return(idx);
}

// --------------------------------------------------------------------------
//	Return the 'n'th currently selected string of this VMultiListBox as
//  a newly created VString object.
//
VString *VMultiListBox::selectedString(int n) const
{
	char temp[100];
	VString *s = NIL;
	
	if (SendMessage(hWnd, LB_GETTEXT, (WPARAM) selectedItem(n), 
										(LPARAM)(LPSTR) temp) > 0) {

		s = new VString(temp);
	}
	return(s);
}

// --------------------------------------------------------------------------
//	Deselect the item at the index 'idx' of this VMultiListBox.
//
void VMultiListBox::deselectItem(integer idx)
{
	SendMessage(hWnd, LB_SETSEL, (WPARAM) 0, (LPARAM) idx);
}

// --------------------------------------------------------------------------
//	Select the item at index 'idx' of this VMultiListBox.
//
void VMultiListBox::selectItem(integer idx)
{
	SendMessage(hWnd, LB_SETSEL, (WPARAM) 1, (LPARAM) idx);
}

// --------------------------------------------------------------------------
//	Deselect the items with indices between idx1 and idx2 inclusive.
//
void VMultiListBox::deselectItem(integer idx1, integer idx2)
{
	if ( idx1 < 0 && idx2 < 0 ) return;
	idx1 = ( idx1 < 0 ) ? 0 : idx1;
	idx2 = ( idx2 < 0 ) ? 0 : idx2;
	SendMessage(hWnd, LB_SELITEMRANGE, (WPARAM) 0, (LPARAM) MAKELPARAM(idx1, idx2) );
}



// --------------------------------------------------------------------------
//	Select the items with indices between idx1 and idx2 inclusive.
//
void VMultiListBox::selectItem(integer idx1, integer idx2)
{
	if ( idx1 < 0 && idx2 < 0 ) return;
	idx1 = ( idx1 < 0 ) ? 0 : idx1;
	idx2 = ( idx2 < 0 ) ? 0 : idx2;
	SendMessage(hWnd, LB_SELITEMRANGE, (WPARAM) 1, (LPARAM) MAKELPARAM(idx1, idx2) );
}


// --------------------------------------------------------------------------
//	Construct a VMultiListBox with no presentation space.
//
VMultiListBox::VMultiListBox() 
#ifndef CV_NOARCHIVER
	:	iSelects(0),
		nSelects(0)
#endif
{ 
	; 
}


// --------------------------------------------------------------------------
//	Return the index of the first selected item of this VMultiListBox.
//
int VMultiListBox::selectedItem() const
{
	return(VMultiListBox::selectedItem(0));
}

// --------------------------------------------------------------------------
//	Return the first currently selected string of this VMultiListBox 
//	as a newly created VString object.
//
VString *VMultiListBox::selectedString() const
{
	return(VMultiListBox::selectedString(0));
}

// ---------------------------------------------------------------------------
//
VString *VMultiListBox::getStringAt(int i)
{
	return VListBox::getStringAt(i);
}

// ===========================================================================

#ifndef CV_NOARCHIVER	

// --------------------------------------------------------------------------
//
void VMultiListBox::putTo(VArchiver& a)
{
	VMultiListBox::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//
void VMultiListBox::putTo(VArchiver& a, boolean most_derived)
{
	VListBox::putTo(a, FALSE);			 	//	base object data

	a << VMultiListBoxArchiveTag;

	a << selectedCount();					//	selections
	for (int i = 0; i < selectedCount(); i++) {
		a << selectedItem(i);	 	
	}

	if (most_derived) {				   		//	children
		VWindow::putChildrenTo(a);
	}
}

// --------------------------------------------------------------------------
//
void VMultiListBox::getFrom(VArchiver& a)
{
	VListBox::getFrom(a);						//	base object data
	
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VMultiListBoxArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VMultiListBoxArchiveTag));
		return;
	}

	a >> nSelects;								//	selections

	iSelects = 0;
	if (nSelects > 0) {
	 	iSelects = new int [nSelects];
		if (iSelects != 0) {
				for (int i = 0; i < nSelects; i++) {
				 	a >> iSelects[i];
				}
		}
	}
}

// --------------------------------------------------------------------------
//
void VMultiListBox::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VMultiListBox::getFrom(a);				

	if (do_realize) {
		VMultiListBox::realize((VWindow *)data);
	}

	if (data != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// --------------------------------------------------------------------------
//
void VMultiListBox::realize(VWindow *pwin)
{
	VListBox::realize(pwin);

	refresh(FALSE);
	deselectItem(-1);
	if (iSelects != 0) {
		for (int i = 0; i < nSelects; i++) {	
			selectItem(iSelects[i]);
		}
	}
	refresh(TRUE);
}

#endif
