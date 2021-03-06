// $Header:   Q:/views/win/vcs/textbox.cpv   1.13   Mar 04 1997 17:55:54   gregm  $ 

//	textbox.cpp
//
//	VTextBox implementation
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
//	05/14/93 pat	added destructor (virtual)
//	05/19/93 pat	added constructor that takes name argument;
//					added getFrom(), putTo(), realize();
//					included archiver.h;
//	06/15/93 pat	added public enum Alignment align() method.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 dgm	pass final argument to getChildrenFrom().
//	07/27/93 pat	removed free()
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	09/22/93 pat	added archive tag
//	10/12/93 pat	initialize alignmentFlag realization data.
//	02/02/94 pat	added archive tag check
//	02/09/94 pat	updated defineArchiveTag use
//	02/27/94 dgm	fix in align (set uStyle accordingly).
//	08/31/94 dss	Added getCtlColor().
//	11/11/94 dss	Archiver tags macros.
//	03/24/95 jld	added resized() method
//	07/18/95 dgm	Miscellaneous const char * changes.
//	09/01/95 dss	VStyle changes.
//	10/05/95 dss	Move 3d default from ctor to getInfo.
//	02/06/96 dgm	Added getPreferredSize().  Added Win95 support
//					for sunken border if StyleBorder + StyleShadow.
//	04/19/96 glm	removed v1.1 code/preened
//	08/28/96 dgm	Default to no tab-stop.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "textbox.h"

#ifndef CV_NOARCHIVER
#	include "archiver.h"
#endif

defineClass(VTextBox, VControl)
defineArchiveRevision(VTextBox, 0)

// ---------------------------------------------------------------------------
//	Construct a `VTextBox` with no presentation space.
//
VTextBox::VTextBox() 
#ifndef CV_NOARCHIVER
	:	alignmentFlag(LeftJustify)
#endif							  
{ 
	setTabStop(FALSE);
}

// ---------------------------------------------------------------------------
//	Construct a `VTextBox` from the resource control-id 'cntrl' of the
//	resource associated with 'pWin'. This is a non-portable GUI system method.
//
VTextBox::VTextBox(unsigned cntrlId, VWindow *pWin)
#ifndef CV_NOARCHIVER
	:	alignmentFlag(LeftJustify)
#endif	
{
	setTabStop(FALSE);
	getRscWindow(cntrlId, pWin);
}

// ---------------------------------------------------------------------------
//	Construct a `VTextBox` with frame 'frame', parent 'win', style 'style'
//	and content 'text'.  'Style' may be one of `StyleLeft` (the default), 
//	`StyleRight` or `StyleCenter` or\'ed with `StyleBorder`.
//
VTextBox::VTextBox(const VFrame& frame, VWindow *win, const VStyle& style,
				   const char *text)
#ifndef CV_NOARCHIVER
	:	alignmentFlag(LeftJustify)
#endif	
{
	setTabStop(FALSE);
	(*this)(frame, win, style);
	if (text) {
		putText(text);
	}
}

// ---------------------------------------------------------------------------
//
VTextBox::VTextBox(const VString& wname, const VFrame& frame, VWindow *pwin, 
					const VStyle& style, const char *text)
#ifndef CV_NOARCHIVER
	:	alignmentFlag(LeftJustify)
#endif	
{
	setTabStop(FALSE);
	(*this)(frame, pwin, style, wname);
	if (text) {
		putText(text);
	}
}

// ---------------------------------------------------------------------------
//	Destroy this VTextBox.
//
VTextBox::~VTextBox()
{
	;
}

// ---------------------------------------------------------------------------
//
void VTextBox::getInfo(VWinInfo &textinfo)
{
	VControl::getInfo(textinfo);
	textinfo.wClass = "static";

	if (style.contains(StyleBorder)) {
		textinfo.style |= WS_BORDER;
	}

	if (style.contains(StyleCenter)) {
		textinfo.style |= SS_CENTER;
	}
	else if (style.contains(StyleRight)) {
		textinfo.style |= SS_RIGHT;
	}
	else { // default = StyleLeft
		textinfo.style |= SS_LEFT;
	}

	//!!!dss 8/23/95 hack. Override the control default.
#if defined(CV_WIN32)
	//!!!dss 8/23/95 hack. Overriding the default.
	// textboxes don't have 3d effects. Let them draw flat. The 
	//	gray background is unaffected.
	if (has3dEffects(V_WIN3D_EFFECTS)) {
		if (style.isPersistent(StyleBorder) &&
			style.isPersistent(Style3D)) {
			textinfo.style &= ~WS_BORDER;
			textinfo.exStyle |= WS_EX_STATICEDGE;
		}
		else {
			this->style -= Style3D;
			textinfo.exStyle &= ~WS_EX_STATICEDGE;
		}
	}
	textinfo.exStyle &= ~(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE);
#endif
}

// --------------------------------------------------------------------------
// 
UINT VTextBox::getCtlColor()
{

#if defined(CV_WIN32)
	return WM_CTLCOLORSTATIC;
#else
	return CTLCOLOR_STATIC;
#endif

}

// ---------------------------------------------------------------------------
//	Align the text contents of this textbox as specified.
//	Alignment is specified as one of the following:
//
//		LeftJustify, 
//		Center
//		RightJustify
//
void VTextBox::align(enum Alignment how)
{
	LONG l = GetWindowLong(hWnd, GWL_STYLE) & 0xFFFFFFF0L;

	switch (how) {
	case Center:
		l |= SS_CENTER;
		style -= StyleLeft + StyleRight;
		style +=  StyleCenter;
		break;
	case LeftJustify:
		l |= SS_LEFT;
		style -= StyleCenter + StyleRight;
		style += StyleLeft;
		break;
	case RightJustify:
		l |= SS_RIGHT;
		style -= StyleCenter + StyleLeft;
		style += StyleRight;
		break;
	}

	SetWindowLong(hWnd, GWL_STYLE, l);
	update();
}

// ---------------------------------------------------------------------------
//	Query and return the alignment setting for the text contents of this 
//	textbox as specified.  The return value is one of the following:
//
//		LeftJustify, 
//		Center
//		RightJustify
//
enum Alignment VTextBox::align()
{
	LONG l = GetWindowLong(hWnd, GWL_STYLE);
	enum Alignment how = LeftJustify;

	if (l & SS_CENTER) {
		how = Center;
	}
	else if (l & SS_LEFT) {
		how = LeftJustify;
	}
	else if (l & SS_RIGHT) {
		how = RightJustify;
	}

	return how;
}

// ---------------------------------------------------------------------------
//	Return the minimum width and height of this textbox in the short 
//	integer stores provided by the caller.
//
void VTextBox::getMinSize(short &w, short &h)
{
	int fw, fh;

	fontSize(&fw, &fh);
	w = fw *2;
	h = fh;
}

// ---------------------------------------------------------------------------
//	Return the minimum width and height in screen pixel units for
//	legibility of this `VTextBox` window in '*w' and '*h' respectively.
//
void VTextBox::getMin(int *w, int *h)
{
	fontSize(w, h);
	*w *= sizeOfText()+2;
	*h += *h /2;
}

// ---------------------------------------------------------------------------
//
void VTextBox::getPreferredSize(int *width, int *height) const
{
	queryPreferredSize(width, height, FALSE, FALSE, TRUE);
	if (style.contains(StyleBorder)) {
		if (width  != 0) { *width  += 4; }
		if (height != 0) { *height += 2; }
	}
}

// ---------------------------------------------------------------------------
//	The implementation of this method in `VTextBox` rejects
//	keyboard focus (see `VWindow::takeFocus()`).
//
boolean VTextBox::takeFocus()
{
	return(FALSE);
}

// --------------------------------------------------------------------------
//	The implementation of this method in VTextBox makes sure the entire
//	client area is painted when resize occurs.
//	
boolean VTextBox::resized(int w, int h)
{
	w = h = 0;
	update(0, FALSE);
	return FALSE;
}

// ---------------------------------------------------------------------------
//	Return the standard size of a `VTextBox` of width and height '*w' and
//	'*h', respectively, in '*w' and '*h'.
//
void VTextBox::sizeOf(int *w, int *h)
{
	int cw, ch;
	fontSize(&cw, &ch);
	*w *= cw;
	*h *= ch;
}

// ---------------------------------------------------------------------------
//
enum WinType VTextBox::winType()
{
	return(WinControl);
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

// --------------------------------------------------------------------------
// VTextBox::putTo
//
void VTextBox::putTo(VArchiver& a)
{
	VTextBox::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
// VTextBox::putTo
//
void VTextBox::putTo(VArchiver& a, boolean most_derived)
{
	VControl::putTo(a, FALSE); 					//	base object data

	a << VTextBoxArchiveTag;

	a << (int) align();							//	text alignment setting

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// --------------------------------------------------------------------------
// VTextBox::getFrom
//
void VTextBox::getFrom(VArchiver& a)
{
	VControl::getFrom(a);	   					//	base object data

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VTextBoxArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VTextBoxArchiveTag));
		return;
	}

	int af;
	a >> af;
	alignmentFlag = (enum Alignment) af;  		//	text alignment setting
}

// --------------------------------------------------------------------------
// VTextBox::getFrom
//
void VTextBox::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VTextBox::getFrom(a);				

	if (do_realize) {
		VTextBox::realize((VWindow *)data);
	}

	if (data != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// --------------------------------------------------------------------------
// VTextBox::realize
//
void VTextBox::realize(VWindow *pwin)
{
	VControl::realize(pwin);
	align(alignmentFlag);
}

// --------------------------------------------------------------------------

#endif 	//	CV_NOARCHIVER	
// ==========================================================================
