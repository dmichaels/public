// $Header:   Q:/views/win/vcs/textbox.h_v   1.11   Mar 04 1997 16:33:10   gregm  $ 

//	textbox.h
//
//	VTextBox interface.
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
//	07/15/92 jmd	preened
//	05/14/93 pat	added destructor (virtual)
//	05/19/93 pat	added constructor that takes name argument;
//	              	added getFrom(), putTo(), realize();
//					added reference to VArchiver;
//					preened; added private alignmentFlag data member.
//	06/15/93 pat	added public enum Alignment align() method.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pat	removed free()
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	08/31/94 dss	Added getCtlColor().
//	03/24/95 jld	added resized() method
//	07/18/95 dgm	Miscellaneous const char * changes.
//	09/01/95 dss	VStyle changes.
//	02/09/96 dgm	Added getPreferredSize().
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#ifndef VTEXTBOX_H
#define VTEXTBOX_H

#include "control.h"

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VTextBoxCls;

enum Alignment { Center, LeftJustify, RightJustify };

//	Introduction
//
//	The `VTextBox` class displays a character string aligned
//	either left, right or centered within a window. 
//	VTextBox objects are static in the sense that they do not
//	respond to mouse or keyboard events.

CLASS VTextBox : public VControl {

public:

	//	Constructors/destructor

				VTextBox();
				VTextBox(	unsigned cntrlId, 
							VWindow *pwin);
				VTextBox(	const VFrame& 	frame, 
							VWindow    *pwin, 
							const VStyle& style = StyleDefault, 
							const char  *text = 0);
				VTextBox(	const VString& wname, 
							const VFrame& 	frame, 
							VWindow    *pwin, 
							const VStyle& style = StyleDefault, 
							const char *text = 0);

				~VTextBox();

	VClass 		   *iam();
	enum WinType 	winType();

	//	Setting Text 

	VTextBox&		operator= (const char *s);

	//	Alignment of Text 

	void			align(enum Alignment how);
	enum Alignment	align();

	//	Control Size 

    virtual void	getPreferredSize(int *, int *) const;
	void			sizeOf(int *w, int *h);
	void			getMin(int *w, int *h); 

	//	Event Handlers

	boolean 		takeFocus();
	boolean 		resized(int w, int h);

	//	Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

	void			getInfo(VWinInfo& textinfo);
	UINT			getCtlColor();
	void			getMinSize(short&, short&);

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

private:

#ifndef CV_NOARCHIVER
	enum Alignment 	alignmentFlag;	//	realization data
#endif

};

// ---------------------------------------------------------------------------
inline VTextBox& VTextBox::operator= (const char *s) 
{ 
	putText(s); 
	return(*this); 
}
// ---------------------------------------------------------------------------

#endif	//	VTEXTBOX_H
