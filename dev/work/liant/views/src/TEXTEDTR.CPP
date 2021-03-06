// $Header:   Q:/views/win/vcs/textedtr.cpv   1.8   Mar 04 1997 17:55:56   gregm  $ 

//	textedtr.cpp
//
//	VTextEditor implementation.
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
//	06/24/93 pat	added support for VArchiver; preened a bit;
//					added constructor that takes name argument.
//	06/24/93 pat	deleted VString returned by dialog ('srch') in replace().
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 dgm	pass final argument to getChildrenFrom().
//	07/27/93 pat	removed free()
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	09/22/93 pat	added archive tag
//	09/26/93 jmd	preened, added setReadOnly in constructors
//	10/12/93 pat	initialize selBegin and selEnd realization data.
//  02/02/94 pat	added archive tag check
//  02/09/94 pat	updated defineArchiveTag use
//	03/26/94 pat	CV_WIN32: updated EM_GETSEL/EM_SETSEL message handling
//	03/26/94 pat	CV_WIN32: cast appropriately for SendMessage()
//	10/24/94 jld	moved setSel() and getSelBegin() to VEditBox
//	11/11/94 dss	Archiver tags macros.
//	09/01/95 dss	VStyle changes.
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

extern "C" {
#	include <string.h>
}

#include "winclass.h"
#include "textedtr.h"
#include "strm.h"
#include "popupmen.h"
#include "input.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

defineClass(VTextEditor, VEditBox)
defineArchiveRevision(VTextEditor, 0)

// ---------------------------------------------------------------------------

char *srchNmes[] = {	"&Find...",
						"Find &Next\tF6",
						"Replace All...",
						(char *)0
};

method srchSels[] = {	defMethod(VTextEditor, find),
						defMethod(VTextEditor, findNext),
						defMethod(VTextEditor, replaceAll),
						NIL_METHOD
};

int srchKeys[] = { 		K_NULL, 
						K_F6, 
						K_NULL, 
						0 
};

// ---------------------------------------------------------------------------
//	Construct a VTextEditor with no presentation space.
//
VTextEditor::VTextEditor() 
			: txStream(0), txtBuffer(0), srchStr(0), replStr(0)
#ifndef CV_NOARCHIVER
			, selBegin(-1), selEnd(-1)
#endif
{
	;
}

// ---------------------------------------------------------------------------
//	Construct a VTextEditor from the resource control-id 'cntrl' of the
//	resource associated with 'pWin'. This is a non-portable GUI system method.
//	*
VTextEditor::VTextEditor(unsigned cntrl, VWindow *pWin) 
			: txStream(0), txtBuffer(0), srchStr(0), replStr(0)
#ifndef CV_NOARCHIVER
			, selBegin(-1), selEnd(-1)
#endif
{
	getRscWindow(cntrl, pWin);
}

// ---------------------------------------------------------------------------
//	Construct a VTextEditor with frame 'frame', parent 'win' and 
//	style 'style'. Styles are the same as for VEditBox.
//	 
VTextEditor::VTextEditor(const VFrame& frame, VWindow *win, 
								const VStyle& style) 
			: txStream(0), txtBuffer(0), srchStr(0), replStr(0)
#ifndef CV_NOARCHIVER
			, selBegin(-1), selEnd(-1)
#endif
{
	(*this)(frame, win, style);

	if (style.contains(StyleReadOnly)) {
		setReadOnly(TRUE);
	}
}

// ---------------------------------------------------------------------------
//	Construct a VTextEditor with frame 'frame', parent 'win' and 
//	style 'style'. Styles are the same as for VEditBox.
//	 
VTextEditor::VTextEditor(const VString& wname, const VFrame& frame, 
								VWindow *win, const VStyle& style) 
			: txStream(0), txtBuffer(0), srchStr(0), replStr(0)
#ifndef CV_NOARCHIVER
			, selBegin(-1), selEnd(-1)
#endif
{
	(*this)(frame, win, style, wname);

	if (style.contains(StyleReadOnly)) {
		setReadOnly(TRUE);
	}
}

// ---------------------------------------------------------------------------
//
VTextEditor::~VTextEditor()
{
	;
}

// ---------------------------------------------------------------------------
//	Create and return a standard search VPopupMenu object.
//	
VPopupMenu *VTextEditor::stdSearchMenu()
{ 
	VPopupMenu *pop;

	pop = new VPopupMenu("&Search");
	pop->addItems(srchNmes, srchSels, srchKeys, this);
	return pop;      
}

// ---------------------------------------------------------------------------
//	Request a search string pattern from the user, then find and mark
//	the text.  This method is the callback for the "Find" function of
//	the "Search" VPopupMenu object. It may be called directly with a NIL
//	argument.
//	
boolean VTextEditor::find(VMenuItem *m)
{
	VString *result;
	char   buf[80];
	m = NIL;
	if (result = VInput::dialog(" Search For:", srchStr, this)) {
		result->gets(buf, 79);
		if (*buf) {
			findText(buf);
		}
		if (srchStr && srchStr != result) {
			delete srchStr;		// jmd (was free)
		}
		srchStr = result;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Find the next occurence of the search string and return its offset.
//	If the search string is empty, return a -1 for the offset. This method is
//	the callback for the "Find" function of the "Search" VPopupMenu object. 
//	It may be called directly with a NIL argument.
//	
integer VTextEditor::findNext(VMenuItem *m)
{
	char   buf[80];
	integer offset = -1;

	m = NIL;
	if (srchStr) {
		srchStr->gets(buf, 79);
		if (*buf != 0x00) {
			offset = findText(buf);
		}
	}
	return offset;
}

// ---------------------------------------------------------------------------
//	Replace all occurences in this VTextEditor edit buffer with
//	user supplied search and replace strings.  This method is used as a
//	callback for the "Replace All" function of the search VPopupMenu object. 
//	It may be called directly with a NIL argument.
//	
boolean VTextEditor::replaceAll(VMenuItem *m)
{
	VString *srch, *rplc;
	char   aStr[80], bStr[80];
	m = NIL;
	if (!(srch = VInput::dialog("Search For", srchStr, this))) {
		return FALSE;
	}
	srch->gets(aStr, 79);

	if (!*aStr) {
		delete srch;
		return FALSE;
	}
	if (!(rplc = VInput::dialog(" Replace With:", replStr, this))) {
		delete srch;
		return FALSE;
	}
	rplc->gets(bStr, 79);
	while (findText(aStr) != -1) {
		replace(bStr);
	}

	/* Save search & replace strings */
	if (srchStr && srchStr != srch) {
		delete srchStr;		// jmd (was free)
	}
	srchStr = srch;
	if (replStr && replStr != rplc) {
		delete replStr;		// jmd (was free)
	}
	replStr = rplc;
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Edit the text contained in the VString (or subclass of VString)
//	object 's'.
//
void VTextEditor::editText(VString *s)
{
	const char     *p;

	txStream = s; 
	if (s  && s->isA(VStreamCls)) {
		((VStream *)s)->at(0);
	}
	if (s) {
		p = s->gets();
		putText(p);
	}
	else {
		putText("");
	}
	clearChanged();
}

// ---------------------------------------------------------------------------
//	Copy the contents of this VTextEditor into 's' and
//	return the length of the contents.
//
unsigned VTextEditor::editedText(VString *s)
{
	unsigned size = 0;

	if (s) {
		size = getText(*s);
		if (s->isA(VStreamCls)) {
			((VStream *)s)->at(0);
		}
	}
	return size;
}

// ---------------------------------------------------------------------------
//	Return the length of the text in this VTextEditor edit buffer.
//
unsigned VTextEditor::getTextSize()
{
	return (unsigned) SendMessage(hWnd, WM_GETTEXTLENGTH, 
													(WPARAM) 0, (LPARAM) 0); 
}

// ---------------------------------------------------------------------------
//	Find and mark the string 's' in this VTextEditor and return
//	its offset.  If the string is not found, return a -1 for the offset.
//	
integer VTextEditor::findText(char *s)
{ 
	const char	*c;
	int	i, b, e, n, offset = -1;

	if (!strlen(s)) {
		return offset;
	}
	VString tmp;
	getSelBegin(&b, &e);

	i = strlen(s);
	n = getText(tmp);
	c = tmp.gets() + e;
	for (offset = e; offset < n; c++ , offset++) {
		if (lstrncmp((LPSTR)c, (LPSTR)s, i) == 0)  {
			break;
		}
	}

	if (offset == n)  {
		offset = -1;
	}
	else  {
		setSel(offset, offset + i); 
	}
	return offset;  
}

// ---------------------------------------------------------------------------
//	Replace the selected (marked) text in this VTextEditor edit buffer
//	with 's'. 
//
boolean VTextEditor::replace(char *s)
{
	SendMessage(hWnd, EM_REPLACESEL, (WPARAM)0, (LPARAM)(LPSTR)s); 
	return TRUE;  
}

// ---------------------------------------------------------------------------
//	Return the length of the line in this VTextEditor at character
//	offset 'off' in the edit buffer. 
//	
unsigned VTextEditor::lineLength(int off)
{
	return (unsigned)SendMessage(hWnd, EM_LINELENGTH, (WPARAM)off, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
//	Return the number of character positions that occur preceeding the
//	first character of line 'line'.  The number of the first line is zero.
//
long VTextEditor::lineIndex(int line)
{
	return (long) SendMessage(hWnd, EM_LINEINDEX, 	(WPARAM)(line - 1), 
													(LPARAM) 0);
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

// --------------------------------------------------------------------------
//	Put this object to the archive.  This method is called from 
//	VArchiver::putObject().
//
void VTextEditor::putTo(VArchiver& a)
{
	VTextEditor::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//	Put this object to the archive by archiving the base class layer, 
//	then the object data of this class layer.
//
//	If 'most_derived' is FALSE, then this method has been called from a derived
//	class.  That is, this class is a base to some derived class and we should
//	not archive our children, if any.
//
//	If 'most_derived' is TRUE, then this method is being called directly from a 
//	VArchiver.  That is, this class is the most derived class layer for this 
//	object and we archive our children, if any.
//
void VTextEditor::putTo(VArchiver& a, boolean most_derived)
{
	VEditBox::putTo(a, FALSE); 			//	base object data

	a << VTextEditorArchiveTag;
	
	a.putObject(srchStr);		 		//	search string
	a.putObject(replStr);				//	replace string

	selBegin = selEnd = -1;
	getSelBegin(&selBegin, &selEnd);
	a << selBegin;
	a << selEnd;

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// --------------------------------------------------------------------------
//	Get this object from the archive by retrieving the object 
//	data of the base class and then the object data of this class.
//
//	This method is invoked by the getFrom() of this most-derived class or by
//	the getFrom() of a derived class.
//
void VTextEditor::getFrom(VArchiver& a)
{
	VEditBox::getFrom(a);		 			//	base object data
	
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VTextEditorArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VTextEditorArchiveTag));
		return;
	}

	srchStr  = (VString *) a.getObject(0);	//	search string
	replStr  = (VString *) a.getObject(0);	//	replace string

	a >> selBegin;
	a >> selEnd;
}

// --------------------------------------------------------------------------
//	Get this object from the archive.  The 'data' parameter is a VWindow *
//	that is our parent window.  In response, retrieve our object data,
//	realize the presentation space under the parent, and finally, retrieve
//	our children.
//
//	This method is invoked by VArchiver::getObject().
//
void VTextEditor::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VTextEditor::getFrom(a);

	if (do_realize) {
		VTextEditor::realize((VWindow *)data);
	}

	if (data != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// --------------------------------------------------------------------------
//	Realize the presentation space of this object.  Upon entry, the
//	Views object exists, but no GUI object.  First, the realize() method of 
//	our base class is invoked to build the GUI object, then we set any 
//	additional object data that affects the GUI.
//
//	This method is invoked by the getFrom() method of this most-derived 
//	class or by the getFrom() of a derived class.
//
void VTextEditor::realize(VWindow *pwin)
{
	VEditBox::realize(pwin);

	setSel(selBegin, selEnd);
}

// --------------------------------------------------------------------------

#endif //	CV_NOARCHIVER	
// ===========================================================================



