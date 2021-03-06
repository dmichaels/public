// $Header:   Q:/views/win/vcs/tristate.h_v   1.7   Mar 04 1997 16:33:14   gregm  $ 

//	tristate.h
//
//	VTriState interface.
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
//	05/13/93 pat	preened;
//					added constructor that takes name argument;
//					added getFrom(), putTo(), realize() and ref to VArchiver
//	05/14/93 pat	added destructor (virtual)
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	08/09/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	08/16/93 jmd	made name const VString
//	09/01/95 dss	VStyle changes.
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#ifndef VTRISTATE_H
#define VTRISTATE_H

#include "checkbox.h"

extern VClass *VTriStateCls;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

//	Introduction
//	
//	A VTriState is a button that is displayed with a square box
//	to the left of its text content (label).  The square box can be
//	empty (unchecked), marked with an x (checked), or be "grayed" and
//	unselectable (VTriState).  If a VTriState object has the 
//	automatic attribute, then it will 
//	automatically toggle between the checked and unchecked state.   

CLASS VTriState : public VCheckBox {

public:

	//	Constructors

				VTriState();

				VTriState(	unsigned cntrlid, VWindow *pwin);

				VTriState(	VRatio x, VRatio y, 
							char *label, 
							VWindow *, 
							const VStyle& style = StyleDefault);
				VTriState(	const VFrame& frame, 
							VWindow *, 
							const VStyle& style = StyleDefault, 
							char *label = 0);

				VTriState(	const VString& wname, 
							const VFrame& frame, 
							VWindow *, 
							const VStyle& style = StyleDefault, 
							char *label = 0);

				~VTriState();

	VClass 	   *iam();

		//	Setting and Interrogating State

	boolean 	enabled(); 
	boolean 	enable(boolean b);
	boolean 	checked();

	//	Event Handling

	boolean 	toggle();

	//	Archival and Retrieval

#ifndef CV_NOARCHIVER
	void  			putTo(VArchiver& a);
	void  			getFrom(VArchiver& a);
	void  			putTo(VArchiver& a, boolean most_derived);
	void  			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

	void 		getInfo(VWinInfo& triinfo); 

};

#endif	//	VTRISTATE_H
