// $Header:   Q:/views/win/vcs/addrmvls.h_v   1.7   Mar 04 1997 16:31:46   gregm  $ 

//	addrmvls.h
//
//	VAddRemoveList class interface
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
//	05/25/93 jmd	preened, added geometry and color
//	10/11/94 dss	Added operator().
//	03/05/96 pkt	changed 'char *' parameters to 'const char *'.
//	11/13/96 pkt	modified for Web support.
// ----------------------------------------------------------------------------

#ifndef VADDRMVLS_H
#define VADDRMVLS_H

#include "dialog.h"
#include "ordcllct.h"
#include "multlist.h"
#include "pushbttn.h"

extern VClass *VAddRemoveListCls;

CLASS VButton;

//
// `VAddRemoveList` is a dialog that allows the user to add or remove
// elements to or from a subset of a set.  The set and subsets are typically
// provided as `VOrdCollect` objects containing `VString` objects.  The
// `VAddRemoveList` dialog presents two `VMultiListBox` objects: one on the left
// containing the elements of the set `VOrdCollect` and one on the right
// containing the elements of the subset `VOrdCollect`. When the user selects items
// from one of these `VMultiListBox`s, a directional pushbutton is enabled so
// that the selected items can be copied to the other list.  If the OK button is
// then selected, the subset `VOrdCollect` can optionally be returned with the
// contents of the subset `VMultiListBox`.
//

CLASS VAddRemoveList : public VDialog {
public:
			VAddRemoveList();
			~VAddRemoveList();
			VAddRemoveList(VWindow *, boolean &, VCollection *,
							VCollection *, const char *, const char *, const char *);
	static boolean dialog(VWindow *, boolean, VCollection *,
							VCollection *, const char *, const char *, const char *...);
	VClass *iam();

	void operator()(VWindow *pWin, boolean &sub,
				  VCollection *set, VCollection *subset,
				  const char *setLabel, const char *subsetLabel,
				  const char *prompt);

protected:
	VOrdCollect		setColl;
	VOrdCollect		subSetColl;
#ifndef CV_REMOTE_GUI
	VMultiListBox	pSetList;
	VMultiListBox	pSubsetList;
	VPushButton		pAddSet;
	VPushButton		pRemoveSet;
#endif

	void	fill();
	boolean add(VButton *);
	boolean remove(VButton *);
	boolean listChange(VMultiListBox *);
};

#endif // VADDRMVLS_H
