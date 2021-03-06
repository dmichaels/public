// $Header:   Q:/views/common/vcs/MULTSLCT.H_V   1.6   Mar 24 1997 16:12:42   scottt  $ 

//	multslct.h
//
//	VMultiSelect class interface [Common]
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
//	10/05/93 jmd	preened
//	10/11/94 dss	Added public operator() and protected realize().
//	05/16/95 pkt	preened.
//	11/01/95 dgm	Changed name realize() to construct() to avoid
//					virtual function hiding warnings.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VMULTSLCT_H
#define VMULTSLCT_H

#include "dialog.h"

CLASS VButton;
CLASS VMultiListBox;
CLASS VPushButton;
CLASS VOrdCollect;
CLASS VString;

extern VClass *VMultiSelectCls;

//
// The VMultiSelect class is a modal dialog used to obtain one or
// more selections from a list of strings or to simply view a list of strings.
//

CLASS VMultiSelect : public VDialog {
public:
							VMultiSelect();
							VMultiSelect(boolean, char *, VOrdCollect *,
										 VOrdCollect *&, VWindow *);
							~VMultiSelect();

	VClass				   *iam();

	static VOrdCollect	   *dialog(boolean, char *, VString **, VWindow *);
	static VOrdCollect	   *dialog(boolean, char *, VOrdCollect *, VWindow *);

	void					operator()(boolean select, char *title, VOrdCollect *lst, VOrdCollect *&sel, VWindow *pWin);

protected:
	VMultiListBox		   *pLstList;
	VPushButton			   *pLstOk;
	VPushButton			   *pLstCancel;

	void					construct(boolean select,
									  char *title,
									  VOrdCollect *lst,
									  VOrdCollect *&sel,
									  VWindow *pWin);
};

// ---------------------------------------------------------------------------
//
inline void VMultiSelect::operator()(boolean select,
									 char *title,
									 VOrdCollect *lst, 
									 VOrdCollect *&sel, 
									 VWindow *pWin)
{
	construct(select, title, lst, sel, pWin);
}

#endif // VMULTSLCT_H
