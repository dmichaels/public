// $Header:   Q:/views/win/vcs/textedtr.h_v   1.7   Mar 04 1997 16:33:10   gregm  $ 

//	textedtr.h
//
//	VTextEditor interface.
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
//	06/24/93 pat	added support for VArchiver; preened;
//					added constructor that takes name argument.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pat	removed free()
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	10/24/94 jld	moved setSel() and getSelBegin() to VEditBox
//	09/01/95 dss	VStyle changes.
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#ifndef VTEXTEDITOR_H
#define VTEXTEDITOR_H

#include "editbox.h"

extern VClass *VTextEditorCls;

CLASS VString;
CLASS VPopupMenu;
CLASS VMenuItem;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

//	Introduction 
//
//	The VTextEditor supports the editing of text
//	contained in objects of class VString or its subclasses.
//	VTextEditor objects automatically create both vertical and
//	horizontal scroll bars, and they can produce a ready-made
//	VPopupMenu menu object to support searching functions.

CLASS VTextEditor : public VEditBox {

public:
				VTextEditor();
				VTextEditor(	unsigned cntrlId, VWindow *pwin);
				VTextEditor(	const VFrame& frame, 
								VWindow *pwin, 
								const VStyle& style = 	StyleDefault);
				VTextEditor(	const VString& wname,	
								const VFrame& frame, 
								VWindow *pwin, 
								const VStyle& style = 	StyleDefault);
				~VTextEditor();

	VClass 	   *iam();

	//	Edited text 

	unsigned 	getTextSize();				//	get how much text
	void 		editText(VString *s);		//	set text to edit
	unsigned 	editedText(VString *s);		//	get text in editor

	//	Standard search menu and its callbacks

	VPopupMenu *stdSearchMenu();			//	make standard popup menu
	boolean 	find(VMenuItem *m);			//	does "Search for" dialog
	integer 	findNext(VMenuItem *m);		//	does repeat search
	boolean 	replaceAll(VMenuItem *m);	//	does "Search/replace" dialog

	//	Selecting, searching and replacing

	integer 	findText(char *s);			//	search for C string
	long 		lineIndex(int lineNo);		// 	get charOffset of a line
											//		(lines are 0-indexed)
	unsigned 	lineLength(int charOffset);	//	length of line at charOffset
	boolean 	replace(char *s);			//	replace marked text with s

	//	Setting text

	VTextEditor& operator=(char *s);

#ifndef CV_NOARCHIVER
	//	Archival and retrieval

	void	 	 	putTo(VArchiver& a);
	void		 	getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif


protected:

	VString    *txStream;	//	VString or VStream frm whch t/extract text
	VString    *srchStr;	//	search string
	VString    *replStr;	//	replace string
	HANDLE 		txtBuffer;	//	memory handle of current edit buffer

#ifndef CV_NOARCHIVER
	void		putTo(VArchiver& a, boolean most_derived);
#endif

private:

#ifndef CV_NOARCHIVER
	int 		selBegin;	//	(realization data)
	int 		selEnd;
#endif
};

// ---------------------------------------------------------------------------
inline VTextEditor& VTextEditor::operator=(char *s) 
{ 
	putText(s); 
	return *this; 
}
// ---------------------------------------------------------------------------

#endif	//	VTEXTEDITOR_H
