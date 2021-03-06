// $Header:   Q:/views/win/vcs/multlist.h_v   1.8   Mar 04 1997 16:32:42   gregm  $ 

//	multlist.h
//
//	VMultiList interface
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
//	05/13/93 pmcm	added destructor (virtual)
//	06/21/93 pmcm	added support for VArchiver; 
//					added constructor with name argument;
//					added virtual VString *getString(int i) for realize();
//					added is, and nSelects for realize().
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pmcm	removed free()
//	08/09/93 jmd	made VFrame const
//	10/12/93 pmcm	ifdef'd realization data
//	01/12/94 jmd	made getStringAt public
//	09/08/94 jld	added two arg versions of select() and deselect()
//	07/18/95 dgm	Changed selectedXXX() method to const.
//	09/01/95 dss	VStyle changes.
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#ifndef VMULTLIST_H
#define VMULTLIST_H

#include "listbox.h"

CLASS VString;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VMultiListBoxCls;

//	Introduction
//
//	The MultiListbox maintains and displays a list of text strings.
//	The list can be scrolled up and down by the user via a vertical
//	scrollbar. MultiListbox objects have two callbacks: one for when the
//	user clicks on an item in the list; and a second for when the
//	user double clicks on an item in the list.  Both callback methods
//	pass the index of the selected item to the client object.
//	The difference between a MultListBox object and a VListBox object is
//	that a MultListBox object allows the user to select more than
//	one string.

CLASS VMultiListBox : public VListBox {

public:

				VMultiListBox();
				VMultiListBox(	unsigned cntrlId, VWindow *pwin);
				VMultiListBox(	const VFrame& frame, 
								VWindow *pwin, 
								const VStyle& style = StyleBorder);

				VMultiListBox(	char *name, 
								const VFrame& frame, 
								VWindow *pwin, 
								const VStyle&  style = StyleBorder);

				~VMultiListBox();

	VClass 	   *iam();

	//	Selection

	int 		selectedCount() const;
	int 		selectedItem() const;
	int 		selectedItem(int i) const;
	VString    *selectedString() const;
	VString	   *selectedString(int i) const;
	boolean 	selected(int i) const;
	void		selectItem(integer start,integer end);
	void 		selectItem(integer i);
	void 		deselectItem(integer i);
	void		deselectItem(integer start,integer end);

	virtual VString    *getStringAt(int i);		

#ifndef CV_NOARCHIVER
	//	Archival and Retrieval

	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

	void 		getInfo(VWinInfo& multinfo);

#ifndef CV_NOARCHIVER
	void				putTo(VArchiver& a, boolean most_derived);
#endif

private:

#ifndef CV_NOARCHIVER
	int		   *iSelects;	//	(realization data)
	int			nSelects;	//	(realization data)
#endif
};

#endif	//	VMULTLIST_H
