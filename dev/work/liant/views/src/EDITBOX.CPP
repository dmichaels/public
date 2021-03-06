// $Header:   Q:/views/win/vcs/editbox.cpv   1.22   Mar 04 1997 17:55:00   gregm  $

//  editbox.cpp
//
//  VEditBox implementation
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1995-1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  09/15/92 jmd    added support for StyleReadOnly
//  04/07/93 pat    changed id to VObject *
//  05/14/93 pat    added destructor (virtual)
//  06/16/93 pat    added constructor that takes name argument;
//                  added getFrom(), putTo(), realize();
//                  included archiver.h;
//                  added private nCharsPerTab and textLimit for archiving;
//                  made MAXtabs static constant; preened a bit.
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  07/27/93 pat    removed free();
//                  added outOfSpaceMthdIndex, changedIndex;
//                  updated findCallback() calls
//  08/09/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  08/22/93 dgm    added setReadOnly() and isReadOnly().
//  09/02/93 pat    updated findCallback() calls
//  09/20/93 dgm    call setReadOnly() in realize().
//  09/22/93 pat    added archive tag
//  09/26/93 jmd    preened, added setReadOnly in constructors
//  09/26/93 jmd    added canUndo
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    added ClientCallbackMismatch check
//  02/09/94 pat    updated defineArchiveTag use
//  02/18/94 pat    initialized new hInstance member (used by notifier).
//  02/26/94 jmd    removed private hInstance member
//  03/07/94 pat    included wintypes.h after winclass.h to prevent
//                  re-declaration errors of windows.h stuff
//  03/26/94 pat    CV_WIN32: updated EM_SETSEL message handling
//  03/26/94 pat    CV_WIN32: cast appropriately for SendMessage().
//  08/31/94 dss    Added getCtlColor().
//  10/24/94 jld    Moved getSel and setSel here from VTextEditor
//  11/11/94 dss    Archiver tags macros.
//  01/31/95 evc    changed way getText fills up a VString to be MBCS safe
//  07/03/95 dgm    Merged in Intersolv changes:
//                  - Pass "this" to perform().
//                  - Style3D for CV_WIN95.
//  09/01/95 dss    VStyle changes.
//  02/09/96 dgm    Added getPreferredSize().
//  03/26/96 dgm    Added revision checks to putTo().
//	08/28/96 dgm	Override default behavior of no tab-stop in VTextBox base.
//	09/25/96 dgm	Respect StyleNoBorder.
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "wintypes.h"
#include "notifier.h"
#include "editbox.h"
#include "popupmen.h"

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "cllbckls.h"
#endif

// ---------------------------------------------------------------------------

static const int MAXtabs = 100;

char   *editNmes[] = {  "&Undo\tAlt+BkSp", 
                        "", 
                        "Cu&t\tShift+Del", 
                        "&Copy\tCtrl+Ins", 
                        "&Paste\tShift+Ins", 
                        "Delete\tDel", 
                        "", 
                        "SelectAll", 
                        0
};

method  editSels[] = {  defMethod(VEditBox, undo), 
                        NIL_METHOD, 
                        defMethod(VEditBox, cutSel), 
                        defMethod(VEditBox, copySel), 
                        defMethod(VEditBox, paste), 
                        defMethod(VEditBox, clearSel), 
                        NIL_METHOD, 
                        defMethod(VEditBox, selectAll), 
                        NIL_METHOD 
};

int     editKeys[] = {  K_Mod_Meta2 | K_Backspace, 
                        K_NULL, 
                        K_NULL, 
                        K_NULL, 
                        K_NULL, 
                        K_NULL, 
                        K_NULL, 
                        K_NULL, 
                        0 
};

// ---------------------------------------------------------------------------

defineClass(VEditBox, VTextBox)
defineArchiveRevision(VEditBox, 1)

// ---------------------------------------------------------------------------
//  Destroy this VEditBox.
//
VEditBox::~VEditBox()
{
    ;
}

// ---------------------------------------------------------------------------
//  Construct a VEditBox with no presentation space.
//
VEditBox::VEditBox() 
#ifndef CV_NOARCHIVER
        :
        changedMthdIndex(-1L),
        outOfSpaceMthdIndex(-1L),
        changedClientIndex(-1L)
#endif
{
	setTabStop(TRUE);
    changedMthd = outOfSpaceMthd = 0;
    changedClient = 0;
    useTabs = TRUE;
    nCharsPerTab = 0;
    textLimit = 0;
}

// ---------------------------------------------------------------------------
//  Construct an VEditBox with frame 'frame', parent 'win' and style 
//  'style'.  The 'style' parameter may be one of `StyleLeft` (the default), 
//  `StyleRight` or `StyleCenter` or\'ed with `StyleBorder`. In addition,
//  the styles `StyleVertical` and `StyleHorizontal` may be or\'ed into 'style'
//  to add vertical or horizontal scroll bars.
//
VEditBox::VEditBox(const VFrame &frame, VWindow *win, const VStyle& style)
#ifndef CV_NOARCHIVER
            :
            changedMthdIndex(-1L),
            outOfSpaceMthdIndex(-1L),
            changedClientIndex(-1L)
#endif
{
	setTabStop(TRUE);
    changedMthd = outOfSpaceMthd = 0;
    changedClient = 0;
    useTabs = TRUE;
    nCharsPerTab = 0;
    textLimit = 0;

    (*this)(frame, win, style);

    if (style.contains(StyleReadOnly)) {
        setReadOnly(TRUE);
    }
}

// ---------------------------------------------------------------------------
//  Construct an named VEditBox with frame 'frame', parent 'win' and style 
//  'style'.  The 'style' parameter may be one of `StyleLeft` (the default), 
//  `StyleRight` or `StyleCenter` or\'ed with `StyleBorder`. In addition,
//  the styles `StyleVertical` and `StyleHorizontal` may be or\'ed into 'style'
//  to add vertical or horizontal scroll bars.
//
VEditBox::VEditBox(const VString& wname, const VFrame &frame, VWindow *win, 
                   const VStyle& style)
#ifndef CV_NOARCHIVER
            :
            changedMthdIndex(-1L),
            outOfSpaceMthdIndex(-1L),
            changedClientIndex(-1L)
#endif
{
	setTabStop(TRUE);
    changedMthd = outOfSpaceMthd = 0;
    changedClient = 0;
    useTabs = TRUE;
    nCharsPerTab = 0;
    textLimit = 0;
    (*this)(frame, win, style, wname);

    if (style.contains(StyleReadOnly)) {
        setReadOnly(TRUE);
    }
}

// ---------------------------------------------------------------------------
//  Construct an VEditBox from the resource control-id 'cntrl' of the
//  resource associated with 'pWin'. This is a non-portable GUI system method.
//
VEditBox::VEditBox(unsigned cntrl, VWindow *pWin)
#ifndef CV_NOARCHIVER
            :
            changedMthdIndex(-1L),
            outOfSpaceMthdIndex(-1L),
            changedClientIndex(-1L)
#endif
{
	setTabStop(TRUE);
    changedMthd = outOfSpaceMthd = 0;
    changedClient = 0;
    useTabs = TRUE;
    nCharsPerTab = 0;
    textLimit = 0;
    getRscWindow(cntrl, pWin);
}

// ---------------------------------------------------------------------------
//
void VEditBox::getInfo(VWinInfo &editinfo)
{
    VControl::getInfo(editinfo);
    editinfo.wClass = "edit";
    editinfo.style |= ES_MULTILINE | ES_NOHIDESEL | WS_CLIPSIBLINGS;

    if (style.contains(StyleBorder)) {
        editinfo.style |= WS_BORDER;
    }
            
#if defined(CV_WIN32)
    if (has3dEffects(V_WIN3D_EFFECTS)) {
        // remove the native style but leave the Views Style.
        editinfo.style &= ~WS_BORDER;
    	if (!style.contains(StyleNoBorder)) {
    	    editinfo.exStyle |= WS_EX_CLIENTEDGE;
		}
    }   
#endif

    if (style.contains(StyleCenter)) {
        editinfo.style |= ES_CENTER;
    }
    else if (style.contains(StyleRight)) {
        editinfo.style |= ES_RIGHT;
    } 
    else { // default = StyleLeft
        editinfo.style |= ES_LEFT;
    }

    if (style.contains(StyleVertical)) {
        editinfo.style |= WS_VSCROLL | ES_AUTOVSCROLL;
    }
    if (style.contains(StyleHorizontal)) {
        editinfo.style |= WS_HSCROLL | ES_AUTOHSCROLL;
    }
    if (style.contains(StyleWordWrap)) {
        editinfo.style &= ~ES_AUTOHSCROLL;
    }
    if (style.contains(StyleReadOnly)) {
        editinfo.style |= ES_READONLY;
    }
}

// --------------------------------------------------------------------------
// 
UINT VEditBox::getCtlColor()
{

#if defined(CV_WIN32)
    return WM_CTLCOLOREDIT;
#else
    return CTLCOLOR_EDIT;
#endif

}

// ---------------------------------------------------------------------------
//
void VEditBox::getMinSize(short &w, short &h)
{
    w = GetSystemMetrics(SM_CXVSCROLL) *2 + 
    GetSystemMetrics(SM_CXHSCROLL);
    h = GetSystemMetrics(SM_CYVSCROLL) *2 + 
    GetSystemMetrics(SM_CYHSCROLL);
}

// ---------------------------------------------------------------------------
//  Clear the 'content-changed' flag of this VEditBox to indicate no
//  changes in its text contents. 
//
void VEditBox::clearChanged()
{
    setChanged(FALSE);
}

// ---------------------------------------------------------------------------
//  Set the text between 'begin' and 'end' as the current selection 
//  in this VEditBox text buffer, that is, mark the text.
//  
void VEditBox::setSel(int begin, int  end)
{
#if defined(CV_WIN32)
    SendMessage(hWnd, EM_SETSEL, (WPARAM)(INT)begin, (LPARAM)(INT)end);
    SendMessage(hWnd, EM_SCROLLCARET, 0, 0);
#else                                             
    SendMessage(hWnd, EM_SETSEL, 0, MAKELONG(begin, end)); 
#endif
}

// ---------------------------------------------------------------------------
//  Return the beginning and ending indices of the current selection in
//  the VEditBox edit buffer in '*begin' and '*end', respectively.
//  
void VEditBox::getSel(int *begin, int *end)
{
    long   wLong;

#if defined(CV_WIN32)
    DWORD dwBegin, dwEnd;
    wLong   = SendMessage(hWnd, EM_GETSEL, (WPARAM) &dwBegin, (LPARAM) &dwEnd);
    *begin  = int(dwBegin);
    *end    = int(dwEnd);
#else
    wLong = SendMessage(hWnd, EM_GETSEL, 0, 0L);
    *begin = LOWORD(wLong);
    *end = HIWORD(wLong);
#endif
}

// ---------------------------------------------------------------------------
//  Erase the currently selected range of text from this VEditBox.
//  This method is a callback for the popup menu returned by `stdEditMenu()`.
//  It may be called directly with a NIL argument.
//  
boolean VEditBox::clearSel(VMenuItem *m)
{
    m = NIL;
    SendMessage(hWnd, WM_CLEAR, (WPARAM) 0, (LPARAM) 0);
    return TRUE;
}

// ---------------------------------------------------------------------------
//  Copy the currently selected range of text to the clipboard from
//  this VEditBox.  This method is a callback for the popup menu returned by
//  `stdEditMenu()`.  It may be called directly with a NIL argument.
//
boolean VEditBox::copySel(VMenuItem *m)
{
    m = NIL;
    SendMessage(hWnd, WM_COPY, (WPARAM) 0, (LPARAM) 0);
    return TRUE;
}

// ---------------------------------------------------------------------------
//  Copy the currently selected range of text to the clipboard and
//  erase the currently selected range of text from this VEditBox. 
//  This method is a callback for the popup menu returned by `stdEditMenu()`.
//  It may be called directly with a NIL argument.
//  
boolean VEditBox::cutSel(VMenuItem *m)
{
    m = NIL;
    SendMessage(hWnd, WM_CUT, (WPARAM) 0, (LPARAM) 0);
    return TRUE;
}

// ---------------------------------------------------------------------------
//  Copy the text contents of this VEditBox into VString object 's' and
//  return the actual number of characters copied.
//
int VEditBox::getText(VString &s)
{
    unsigned n;

// MBCS safe - evc.  

    n= sizeOfText();
    char * tempbuf = new char[n+1];
    SendMessage(hWnd, WM_GETTEXT, (WPARAM)(n + 1), (LPARAM)tempbuf);
    s=tempbuf;
    delete [] tempbuf;
    return n;
}

// --------------------------------------------------------------------------
//  Return TRUE if the content of this VEditBox has been modified, 
//  otherwise return FALSE. 
//
boolean VEditBox::changed()
{
    return (boolean)SendMessage(hWnd, EM_GETMODIFY, (WPARAM) 0, (LPARAM) 0);
}


// --------------------------------------------------------------------------
//  Called from the VNotifier when out of memory or when the 
//  contents of this VEditBox have changed.
//
boolean VEditBox::notify(int code)
{
	method mthd;
	VObject *clnt;

	switch (code) {
	case EN_CHANGE:
		clnt = changedClient;
		mthd = changedMthd;
		break;
	case EN_ERRSPACE:
		clnt = client; 
		mthd = outOfSpaceMthd;
		break;
	case ACCELcode:
		if (!VWindow::checkLosingFocus()) {
			takeFocus();
			return TRUE;
		}
	default:
		return FALSE;
	}
	if ((clnt != 0) && (mthd != 0)) {
		clnt->perform(mthd, long(this));
		return TRUE; 
	}
	else {
		return FALSE; 
	}
}

// --------------------------------------------------------------------------
//  Paste the contents of the clipboard over the current selection
//  in this VEditBox. This method is a callback for the popup menu returned
//  by `stdEditMenu()`. It may be called directly with a NIL argument.
//
boolean VEditBox::paste(VMenuItem *m)
{
    m = NIL;
    SendMessage(hWnd, WM_PASTE, (WPARAM) 0, (LPARAM) 0);
    return TRUE;
}

// --------------------------------------------------------------------------
//  The implementation of this method in VEditBox does nothing
//  (see VWindow::resized()).
//  
boolean VEditBox::resized(int w, int h)
{
    w = h = 0;
    return FALSE;
}

// --------------------------------------------------------------------------
//  Set the selected text to the entire contents of this VEditBox. 
//  This method is a callback for the popup menu returned by `stdEditMenu()`. 
//  It may be called directly with a NIL argument.
//
boolean VEditBox::selectAll(VMenuItem *m)
{
    m = NIL;
#if defined(CV_WIN32)
    SendMessage(hWnd, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
#else
    SendMessage(hWnd, EM_SETSEL, 0, 0x7fff0000L);
#endif
    return TRUE;
}

// --------------------------------------------------------------------------
//  Set the 'contents-changed' flag of this VEditBox to 'flag'.
//
void VEditBox::setChanged(boolean flag)
{
    SendMessage(hWnd, EM_SETMODIFY, (WPARAM) flag, (LPARAM) 0);
}


// --------------------------------------------------------------------------
//  Return a standard editing VPopupMenu object for this VEditBox.
//
VPopupMenu *VEditBox::stdEditMenu()
{ 
    VPopupMenu *a;

    //  EDIT MENU

    a = new VPopupMenu("&Edit");
    a->addItems(editNmes, editSels, editKeys, this);
    return a;      
}


// --------------------------------------------------------------------------
//  Return the length (in bytes) of the text contents of this VEditBox.
//
int VEditBox::sizeOfText()
{
    return GetWindowTextLength(hWnd);
}

// --------------------------------------------------------------------------
//  Undo the last change to the contents of this VEditBox. It may be
//  called directly with NIL argument.
//
boolean VEditBox::undo(VMenuItem *m)
{
    m = NIL;
    if (SendMessage(hWnd, EM_CANUNDO, (WPARAM) 0, (LPARAM) 0)) {
        SendMessage(hWnd, EM_UNDO, (WPARAM) 0, (LPARAM) 0);
    }
    else {
        error("Warning: Cannot undo last change");
    }
    return TRUE;
}

// --------------------------------------------------------------------------
// Test if undo can be performed
//
boolean VEditBox::canUndo() const
{
    if (SendMessage(hWnd, EM_CANUNDO, (WPARAM) 0, (LPARAM) 0)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// --------------------------------------------------------------------------
//  Arrange to call the 'nospace' or 'change' methods of 'clnt' when
//  this VEditBox has run out of memory or the user has changed contents of
//  it, respectively.  The `NIL_METHOD` constant can be used for either
//  the 'nospace' or 'change' callback methods if they are not being used.
//
void VEditBox::uponEvent(VObject *clnt, method nospace, method change) 
{ 
    client = clnt; 
    if (change) {
      changedClient = clnt;
    }
    outOfSpaceMthd = nospace; 
    changedMthd = change; 
}

void VEditBox::uponChange(VObject *clnt, method change)
{
    changedClient = clnt;
    changedMthd = change;
}

void VEditBox::uponNoSpace(VObject *clnt, method nospace)
{
    client = clnt;
    outOfSpaceMthd = nospace;
}

// --------------------------------------------------------------------------
//  The implementation of this method in VEditBox does nothing
//  (see VWindow::enterHit()).
//
boolean VEditBox::enterHit() 
{ 
    return FALSE; 
}

// --------------------------------------------------------------------------
//  Sets the tab-stops for this VEditBox to 'n' characters.
//
void VEditBox::setTabs(int n)
{
    nCharsPerTab = n;       

    int i;
    long units = GetDialogBaseUnits();
    n = (n * LOWORD (units)) / 2;   // dialog units to 

    int tabs[MAXtabs];
    tabs[0] = n;
    for (i = 1; i < MAXtabs; i++) {
        tabs[i] = tabs[i - 1] + n;
    }
    SendMessage(hWnd, EM_SETTABSTOPS, (WPARAM) MAXtabs, (LPARAM) tabs);
}

// --------------------------------------------------------------------------
//  Set the maximum number of characters allowed for this VEditBox
//  to 'n' characters.
//
void VEditBox::setLimit(unsigned n)
{
    textLimit = n;
    SendMessage(hWnd, EM_LIMITTEXT, (WPARAM) n, (LPARAM) 0);
}

// --------------------------------------------------------------------------
//
boolean VEditBox::key(int virtual_key, char ascii_key)
{
	if (VWindow::key(virtual_key, ascii_key)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VEditBox::nextFocus()
{
	if (!useTabs || isReadOnly()) {
		return VControl::nextFocus();
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VEditBox::controlNextFocus()
{
	if (useTabs) {
		return VControl::nextFocus();
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VEditBox::controlPrevFocus()
{
	if (useTabs) {
		return VControl::prevFocus();
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VEditBox::takeFocus()
{
	return VControl::takeFocus();
}

// --------------------------------------------------------------------------
//  Copy upto 'len' characters into 'buffer' from the text content
//  of this VEditBox and return the number of characters copied. The text
//  copied into 'buffer' will be NULL terminated.  Therefore, 'buffer' must
//  have room for 'len' + 1 characters.
//
int VEditBox::getText(char *buffer, unsigned len)
{
    SendMessage(hWnd, WM_GETTEXT, (WPARAM) (len + 1), (LPARAM)(LPSTR) buffer);
    return(len < (unsigned)sizeOfText()) ? len : sizeOfText();
}

// ---------------------------------------------------------------------------
void VEditBox::addVScroll(boolean b)
{
    long s;
    if (b) {
        if (style.contains(StyleVertical)) {
            return;
        }
        style += StyleVertical;
        s = GetWindowLong(hWnd, GWL_STYLE) | (WS_VSCROLL | ES_AUTOVSCROLL);
    }
    else {
        if (!style.contains(StyleVertical)) {
            return;
        }
        style -= StyleVertical;
        s = GetWindowLong(hWnd, GWL_STYLE) & ~(WS_VSCROLL | ES_AUTOVSCROLL);
    }
    SetWindowLong(hWnd, GWL_STYLE, s);
}

// ---------------------------------------------------------------------------
void VEditBox::addHScroll(boolean b)
{
    long s = 7;
    if (b) {
        if (style.contains(StyleHorizontal)) {
            return;
        }
        style += StyleHorizontal;
        s = GetWindowLong(hWnd, GWL_STYLE);
        s |= (WS_HSCROLL | ES_AUTOHSCROLL);
    }
    else {
        if (!style.contains(StyleHorizontal)) {
            return;
        }
        style -= StyleHorizontal;
        s = GetWindowLong(hWnd, GWL_STYLE) & ~(WS_HSCROLL | ES_AUTOHSCROLL);
    }
    s = SetWindowLong(hWnd, GWL_STYLE, s);
}

// ---------------------------------------------------------------------------
boolean VEditBox::hasVScroll() const
{
    return style.contains(StyleVertical);
}

// ---------------------------------------------------------------------------
boolean VEditBox::hasHScroll() const
{
    return style.contains(StyleHorizontal);
}

// ---------------------------------------------------------------------------
void VEditBox::setReadOnly(boolean b)
{
    if (b) {
        style += StyleReadOnly;
    }
    else {
        style -= StyleReadOnly;
    }
    SendMessage(hWnd, EM_SETREADONLY, (WPARAM)(BOOL)(b), (LPARAM) 0);
}

// ---------------------------------------------------------------------------
boolean VEditBox::isReadOnly() const
{
    if (style.contains(StyleReadOnly)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// ---------------------------------------------------------------------------
//
void VEditBox::getPreferredSize(int *width, int *height) const
{
    queryPreferredSize(width, height, FALSE, FALSE);
    if (width  != 0) {
        *width += 4;
        if (hasVScroll()) {
            *width += 16;
        }
    }
    if (height != 0) {
        *height += 4;
        if (hasHScroll()) {
            *height += 16;
        }
    }
}

//---------------------------------------------------------------------------
// VEditBox::setFont(VFont *)
// fixes margin issue on win95

void VEditBox::setFont(VFont *font)
{
#if defined(CV_WIN32)
	DWORD dwMargins;
    dwMargins = SendMessage(  hWnd,
                              EM_GETMARGINS,
                              0,
                              0);
#endif

    VTextBox::setFont(font);

#if defined(CV_WIN32)
    SendMessage(  hWnd,
                  EM_SETMARGINS,
                  EC_LEFTMARGIN | EC_RIGHTMARGIN,
                  MAKELPARAM(LOWORD(dwMargins), HIWORD(dwMargins)));
#endif
}

// ----------------------------------------------------------------------------
//
void VEditBox::enableBool(boolean b)
{
	if (!b) {
#if defined(CV_WIN32)
		SendMessage(hWnd, EM_SETSEL, WPARAM(-1), LPARAM(0));
#else                                             
		SendMessage(hWnd, EM_SETSEL, WPARAM(1), MAKELONG(-1, 0)); 
#endif
	}
	VTextBox::enableBool(b);
}

// ===========================================================================
#ifndef CV_NOARCHIVER   

// --------------------------------------------------------------------------
// VEditBox::putTo
//
void VEditBox::putTo(VArchiver& a)
{
    VEditBox::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
// VEditBox::putTo
//
void VEditBox::putTo(VArchiver& a, boolean most_derived)
{
    VTextBox::putTo(a, FALSE);              //  base object data

    a << VEditBoxArchiveTag;

    a << useTabs;                           //  tabs f/edit or focus transfer?
    a << nCharsPerTab;                      //  characters per tab
    a << textLimit;                         //  maximum numbers of chars

    const char *changed_class_name  = 0;            
    const char *changed_method_name = 0;            

    VCallbackList::findCallback(changedMthd, changed_class_name,  
                                    changed_method_name, changedMthdIndex);

	a << (char *) changed_class_name;       //  changed method's class name
	a << (char *) changed_method_name;      //  changed method name

	const char *outOfSpace_class_name   = 0;            
	const char *outOfSpace_method_name  = 0;            

    VCallbackList::findCallback(outOfSpaceMthd, outOfSpace_class_name,  
                                outOfSpace_method_name, outOfSpaceMthdIndex);

    a << (char *) outOfSpace_class_name;	//  outOfSpace method's class name
    a << (char *) outOfSpace_method_name;	//  outOfSpace method name

	if (VEditBoxTagRev > 0) {
	    a.putObject(notifier->getClientName
					(changedClient, changedClientIndex), TRUE);
	}

    if (most_derived) {                     //  children
        VWindow::putChildrenTo(a);
    }
}

// --------------------------------------------------------------------------
// VEditBox::getFrom
//
void VEditBox::getFrom(VArchiver& a)
{
    VTextBox::getFrom(a);                       //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VEditBoxArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VEditBoxArchiveTag));
        return;
    }

    a >> useTabs;                           //  tabs f/edit or focus transfer?
    a >> nCharsPerTab;                      //  characters per tab
    a >> textLimit;                         //  maximum numbers of chars

    char        cmethod_name[256];
    char *      cmethod_name_ptr = cmethod_name;
    char        cclass_name[256];
    char *      cclass_name_ptr = cclass_name;
    char        omethod_name[256];
    char *      omethod_name_ptr = omethod_name;
    char        oclass_name[256];
    char *      oclass_name_ptr = oclass_name;


    a >> cclass_name_ptr;               //  changed method's class name
    a >> cmethod_name_ptr;              //  changed method name

    a >> oclass_name_ptr;               //  outOfSpace method's class name
    a >> omethod_name_ptr;              //  outOfSpace method name

    if (getArchiveTagRev(tag) > 0) {
      VString client_name;                            //  name of client
      a.getObject(&client_name);
      changedClient = notifier->getClient(client_name, &changedClientIndex);
    }
    else {
      changedClient = client;
    }

    method changedM = VCallbackList::findCallback(cclass_name_ptr, 
                                        cmethod_name_ptr, &changedMthdIndex);

    if (changedClient != 0 && changedM != 0) {
        if (!changedClient->isA(VClass::of(cclass_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }

    method outOfSpaceM = VCallbackList::findCallback(oclass_name_ptr, 
                                        omethod_name_ptr, &outOfSpaceMthdIndex);
    
    if (client != 0 && outOfSpaceM != 0) {
        if (!client->isA(VClass::of(oclass_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }

    uponChange(changedClient, changedM);
    uponNoSpace(client, outOfSpaceM);
}

// --------------------------------------------------------------------------
// VEditBox::getFrom
//
void VEditBox::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VEditBox::getFrom(a);

    if (do_realize) {
        VEditBox::realize((VWindow *)data);
    }

    if (data != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

// --------------------------------------------------------------------------
// VEditBox::realize
//
void VEditBox::realize(VWindow *pwin)
{
    VTextBox::realize(pwin);

    enableTabs(useTabs);
    if (nCharsPerTab != 0) {
        setTabs(nCharsPerTab);      //  !!! pat cant use 0, cant get defs from GUI
    }
    if (textLimit != 0) {
        setLimit(textLimit);
    }
    if (style.contains(StyleReadOnly)) {
        setReadOnly(TRUE);
    }

    // delayed client resolution
    VObject* changedClient = 0;
    VObject* outOfSpaceClient = 0;
    if (client == 0 && clientIndex == -1 && changedMthd != 0) {
        client = VCallbackList::findClientForCallback(changedMthd, this);
        changedClient = client;
    }
    if (client == 0 && clientIndex == -1 && outOfSpaceMthd != 0) {
        client = VCallbackList::findClientForCallback(outOfSpaceMthd, this);
        outOfSpaceClient = client;
    }

    if (changedMthd != 0 && outOfSpaceMthd != 0 &&
        changedClient != outOfSpaceClient) {
        // error; client mismatch; everyone loses
        client = 0;
    }
}

// --------------------------------------------------------------------------

#endif //   ARCHIVER    
// ===========================================================================


