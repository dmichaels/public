// $Header:   Q:/views/common/vcs/exclgrp.h_v   1.10   Mar 24 1997 16:14:38   scottt  $ 

//	exclgrp.h
//
//	VExclusiveGroup class interface [Common]
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
//	06/22/93 pat	added VArchiver support, preened;
//					added constructor with name argument.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pat	removed free()
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	10/20/93 dgm	3.0 changes.
//	05/16/95 pkt	preened.
//	09/11/95 pkt	merged with Motif.
//	12/23/96 pkt	updated to compile on Web platform.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VEXCLGRP_H
#define VEXCLGRP_H

// --------------------------------------------------------------------------
//
#include "inclgrp.h"

CLASS VButton;
CLASS VFrame;
CLASS VString;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VExclusiveGroupCls;
extern VClass *VRadioButtonCls;

// --------------------------------------------------------------------------
// A VExclusiveGroup is a VGroup of VButtons distributed in
// a presentation window. If the button type is a subclass of VCheckBox,
// then only one button may be checked at a time.  VExclusiveGroup objects
// receive all of the click callbacks from their contained buttons,
// and then, respond to their client with the index of the button that was
// clicked on. VExclusiveGroup objects manage focus passing between buttons,
// and the user can use up and down arrows to move the focus between the
// buttons. By default, VRadioButton buttons are used by this group.
//
CLASS VExclusiveGroup : public VInclusiveGroup {

public:

	// Constructors

					VExclusiveGroup();

					VExclusiveGroup(const VFrame& frame, VWindow *parent, 
									char *title = 0, 
									VClass *cls = VRadioButtonCls);
									
					VExclusiveGroup(const VString& wname,
									const VFrame& frame, VWindow *parent, 
									char *title = 0, 
									VClass *cls = VRadioButtonCls);
#if (defined(CV_WINDOWS) || defined(PM)) && !defined (CV_REMOTE_GUI)
					VExclusiveGroup(unsigned cntrlId, 
									VWindow *parent);

					VExclusiveGroup(unsigned cntrlId, 
									unsigned start, unsigned count, 
									VWindow *parent);
#endif
					~VExclusiveGroup();

	VClass 		   *iam();

	// Event Handling

	boolean 		clicked(VButton *);
	boolean			arrowKey(int);

#ifndef CV_NOARCHIVER
	// Archival and Retrieval

	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

};

#endif 	// !VEXCLGRP_H
