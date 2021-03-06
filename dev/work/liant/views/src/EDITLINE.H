// $Header:   Q:/views/win/vcs/editline.h_v   1.12   Mar 04 1997 16:32:20   gregm  $ 

//	editline.h
//
//	VEditLine interface.
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
//	05/14/93 pmcm	added destructor (virtual)
//	06/21/93 pmcm	added support for VArchiver, preened some.
//					added constructor with name arg.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pmcm	removed free()
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	10/06/93 dgm	added setHorizontalScroll() and hasHorizontalScroll().
//	07/18/95 dgm	Miscellaneous const char * changes.
//	09/01/95 dss	VStyle changes.
//	01/05/96 dgm	Overrode getPreferredSize().
//	01/29/96 dgm	Preferred-size changes.
//	04/19/96 glm	removed v1.1 code/preened
// ---------------------------------------------------------------------------

#ifndef VEDITLINE_H
#define VEDITLINE_H

#include "editbox.h"

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VEditLineCls;

//	Introduction
//
//	The VEditLine class is an `VEditBox` containing only one line of text
//	and hence, it does not support scrollbars.  The contents of an VEditLine
//	object can be aligned to the left, right or center.  VEditLine objects
//	support clipboard cutting and pasting as well as a single level undo.

CLASS VEditLine : public VEditBox {

public:
				VEditLine();
				VEditLine(	unsigned cntrlId, VWindow *pwin);
				VEditLine(	const VString& wname,
							const VFrame& frame, 
							VWindow *pwin, 
							const VStyle& style = StyleDefault);
				VEditLine(	const VFrame& frame, 
							VWindow *pwin, 
							const VStyle& style = StyleDefault);
				~VEditLine();

	VClass 		*iam();

	void		getMin(int *w, int *h); 
	boolean 	enterHit();
	boolean 	takeFocus(); 

	VEditLine&	operator= (const char *s);	//	Copy C string to editline.

	void		 setHorizontalScroll(boolean);
	boolean		 hasHorizontalScroll() const;

#ifndef CV_NOARCHIVER
	//	Archival and Retrieval

	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

    virtual void	getPreferredSize(int *w, int *h) const;
	void			getInfo(VWinInfo& lineinfo);
	void			getMinSize(short& w, short& h);

#ifndef CV_NOARCHIVER
	void		putTo(VArchiver& a, boolean most_derived);
#endif

};

// ---------------------------------------------------------------------------
inline VEditLine& VEditLine::operator= (const char *s) 
{ 
	putText(s); 
	return *this; 
}
// ---------------------------------------------------------------------------
inline boolean VEditLine::hasHorizontalScroll() const
{
	return style.contains(StyleHorizontal);
}
// ---------------------------------------------------------------------------

#endif	// VEDITLINE_H
