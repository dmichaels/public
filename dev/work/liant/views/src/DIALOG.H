// $Header:   Q:/views/win/vcs/dialog.h_v   1.19   Mar 04 1997 16:32:16   gregm  $ 

//	dialog.h
//
//	VDialog class interface
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
//	09/10/92 jmd	added optional style arg to
//					VDialog::VDialog(frame, win, title)
//	11/01/92 jmd	preened
//	07/15/93 pmcm	added VArchiver support; preened.
//	07/19/93 jmd	made VFrame const in constructor
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pmcm	removed free()
//	08/16/93 jmd	made name const VString
//	08/18/93 jmd	preened, added callbacks, Rsc constructors
//	08/20/93 jmd	removed loadResources()
//	08/23/93 pmcm	removed CLASS keyword from friend declaration
//	08/31/93 dgm	added getStyle().
//	10/05/93 jmd	added getResult().
//	10/05/93 jmd	removed loadFromResources
//	10/11/94 dss	Native dialog support.
//	11/03/94 dss	Moved accelerator, def button, focus mnmgt to view.
//	09/01/95 dss	VStyle changes.
//	09/29/95 dgm	Added constrainMinimumSize() and resizeClientArea().
//  10/08/95 evc	Added setFont.
//	10/12/95 dss	added default style functionality. Removed CPPV_11 code.
//	10/16/95 dss	added realizeInit();
//	10/18/95 dgm	Removed constrainMinimumSize() and resizeClientArea().
//					Added getNonClientAreaSize().
//	10/30/95 evc	Added a method as 3 arg to setAccel (with a default of 0).
//  11/01/95 evc    Got rid of setAccel (and put it in VView)
//	11/02/95 dgm	Changed getNonClientAreaSize() to getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	12/04/95 pkt	Changed parameters from char* to const char*.
// ---------------------------------------------------------------------------

#ifndef VDIALOG_H
#define VDIALOG_H

#include "poppwndw.h"

extern VClass *VDialogCls;

CLASS VButton;
CLASS VControl;
CLASS VSet;
CLASS VNotifier;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
CLASS VRscArchiver;
#endif

//	Introduction
//
//	The VDialog class is a subclass of the VPopupWindow class. It is
//	designed to embody the user interface elements common to all specific
//	forms of user dialog boxes. VDialog objects are popup windows so they
//	appear to lie on top of existing windows. They all must have a parent
//	window, but they are not clipped by their parent. VDialog objects, by
//	default, are distinguished from other windows by being framed with an
//	extra thick border. 
//	
//	VDialog objects can be displayed as either 'modal'
//	or 'modeless' dialogs. A modal dialog takes control from the
//	application as soon as it is displayed. Usually, some user action is
//	required to close the dialog and return to normal application control.
//	A modeless dialog does not take over control from the application, but
//	it allows other windows to function normally.

CLASS VDialog : public VPopupWindow {

public:

	friend VControl;
	friend VNotifier;

public:

	//	Constructors/destructor
	    
					VDialog();

					VDialog(const char *templateName, 
							VWindow *pwin);

					VDialog(const VString& wname,
							const VFrame& frame, 
							VWindow *pwin, 
							const char *title = 0, 
							const VStyle& style = StyleHidden + StyleCaption +
											StyleCloseBox + StyleModalBorder);

					VDialog(const VFrame& frame, 
							VWindow *pwin, 
							const char *title = 0, 
							const VStyle& style = StyleHidden + StyleCaption + 
											StyleCloseBox + StyleModalBorder);

					VDialog(int x, int y, int w, int h, 
							VWindow *pwin, 
							const VStyle& style = StyleHidden + StyleCaption + 
											StyleCloseBox + StyleModalBorder);

					~VDialog();

	VClass 		   *iam();
	static void 	stdSize(const VStyle& style, int *w, int *h);

	//	Modal Operation 
	    
	void			modal();
	void			endModal();

	// styles

	static VStyle& getClassDefaultStyle(void);
	virtual VStyle& getDefaultStyle(void) const;

	//	Event Handling 
	    
	boolean 		close();
	virtual boolean ok(VButton *button);
	virtual boolean cancel(VButton *button);
	void			setSysMenu();

	// callbacks
	boolean 		okPressed(VButton *button);
	boolean 		cancelPressed(VButton *button);

	boolean			getResult()	const;

	boolean			isNativeDialog() const;

	// Geometry management.

    virtual void	getSystemBorderSize(int *, int *) const;

	// Font management

	void			setFont(VFont *f);

#ifndef CV_NOARCHIVER
	//	Archival and Retrieval

	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
	void			loadChildResources(VArchiver& a);
#endif

    virtual UINT	getCtlColor();

protected:

	boolean 		result;			//	return value of dialog

	void			getInfo(VWinInfo& dialoginfo);
	void			init(void);
	void			realizeInit(const char *title);

	boolean		nativeDialog;

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

};

// ---------------------------------------------------------------------------
// Return the result flag for this VDialog:
// TRUE if the user selected OK, FALSE otherwise
//
inline boolean VDialog::getResult() const
{
	return result;
}

// ---------------------------------------------------------------------------
inline
boolean	VDialog::isNativeDialog() const
{
	return nativeDialog;
}

#endif	//	VDIALOG_H
