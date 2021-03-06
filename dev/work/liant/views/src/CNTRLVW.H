// $Header:   Q:/views/common/vcs/cntrlvw.h_v   1.6   May 30 1997 17:14:52   scottt  $ 

//	cntrlvw.h
//
//	VControlView class interface [Common]
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
//	08/09/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	05/16/95 pkt	preened
//	09/01/95 dss	VStyle changes. Remove CPPV_11 code.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef CNTRLVW_H
#define CNTRLVW_H
#include "view.h"

CLASS VControl;
extern VClass *VControlViewCls;

// The `VControlView` class is designed to provide a means for
// treating subclasses of the `VControl` class (that is, `VButton`, `VListBox`,
// etc.) as child windows that can take on the attributes of having a title,
// being sizable, being movable, or having the close box (system menu).
// `VControlView` objects are constructed by providing a referent to the
// `VClass` (Meta VClass) object of the subclass of `VControl` to be used, for
// example, VTextEditorCls for the `TextEdit` class. The `VControlView`
// constructor creates an instance of this class using framing ratios of 100%
// of width and height of the `VControlView` object.  Hence the control object
// is superimposed on the view.  A referent to the superimposed control can
// be obtained via the `getControl()` method.

CLASS VControlView : public VView {
public:
	VClass	   *iam();

	// Constructors
				VControlView();
				VControlView(const VFrame& f, VWindow *, VClass *,
								const VStyle& style = StyleDefault,
								const VStyle& cntrlStyle = StyleDefault);

				~VControlView();
	
	// Control Access
	VControl   *getControl();
	
	boolean		prevChildFocus(VWindow *);
	boolean		nextChildFocus(VWindow *);
	
protected:
	// An instance of the subclass of `VControl` imposed on this view.
	VControl   *control;
};

// ---------------------------------------------------------------------------
// Return a referent to the `VControl` object superimposed on this
// `VControlView`.
//
inline VControl *VControlView::getControl()
{
	return(control);
}

#endif /* CNTRLVW_H */

