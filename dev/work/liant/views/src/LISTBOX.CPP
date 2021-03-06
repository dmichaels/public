// $Header:   Q:/views/win/vcs/listbox.cpv   1.32   Mar 04 1997 17:55:16   gregm  $ 

//  listbox.cpp
//
//  VListBox class implementation
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
//  04/07/93 pat    changed id to VObject *
//  05/13/93 pat    added constructor that takes name argument;
//                  added getFrom(), putTo(), realize();
//                  included archiver.h
//  05/14/93 pat    added destructor (virtual)
//  05/25/93 jmd    added NOINTEGRALHEIGHT style, preened, removed free()
//  06/29/93 pat    initialized returnThis member in init()
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 pat    added selectMthdIndex, and dblClkMthdIndex;
//                  updated findCallback() calls
//  08/09/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  08/19/93 dgm    fix (null point check) in putTo().
//  09/02/93 pat    added cllbckls.h; updated findCallback() calls
//  09/22/93 pat    added archive tag
//  10/11/93 pat    initialized listOStrings and iSelect realization data.
//  10/11/93 jmd    minor preening
//  01/12/94 jmd    made getStringAt public
//  01/27/94 dgm    changed return type for SendMessage() to LRESULT in
//                  getStringAt().
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    added ClientCallbackMismatch check
//  02/09/94 pat    updated defineArchiveTag use
//  03/26/94 pat    CV_WIN32: cast appropriately for SendMessage().
//  06/23/94 jmd    delete listOStrings in realize
//  08/31/94 dss    Added getCtlColor().
//  09/08/94 jld    Added replaceStringAt().
//  11/11/94 dss    Archiver tags macros.
//  04/19/95 jld    delete listOStrings in destructor
//  06/02/95 pkt    made 'count', 'selectedString', and 'selectedItem' const.
//  06/02/95 pkt    added const to some parameters.
//  06/24/95 jld    made some parameters const
//  06/24/95 jld    added findPrefix() and selectPrefix()
//  07/18/95 dgm    Miscellaneous const char * changes.
//  08/02/95 dss    Style3D for CV_WIN95.
//  08/10/95 dgm    Changed findPrefix() to findString().
//  09/01/95 dss    VStyle changes.
//  12/15/95 evc    Added horizontal scroll bar support
//  01/15/96 pkt    fixed getStringAt to sometimes return an empty string.
//  01/29/95 evc    fixed horizontal scrollbars so they do not rely on 
//                  string length when deciding to refresh horizontal extent.
//  02/01/95 dgm    Added getPreferredSize().
//  03/26/96 dgm    Added revision checks to putTo().
//  04/19/96 glm    removed v1.1 code/preened
//  04/19/96 dgm    Fixed calculateExtent().
// ----------------------------------------------------------------------------

#include "listbox.h"
#include "notifier.h"
#include "str.h"
#include "port.h"
#include "font.h"

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "ordcllct.h"
#   include "iterator.h"
#   include "cllbckls.h"
#endif

defineClass(VListBox, VControl)
defineArchiveRevision(VListBox, 1)

// ---------------------------------------------------------------------------
//  Destroy this VListBox.
//
VListBox::~VListBox()
{
#ifndef CV_NOARCHIVER
    if (listOStrings != 0) {
        listOStrings->freeContents();
        delete listOStrings;
    }
#endif
}

// ---------------------------------------------------------------------------
void VListBox::init(const char *title)
{
    returnThis = FALSE;
    dblClkReturnThis = FALSE;
    dblClkClient = 0;
    selectMthd = dblClkMthd = 0;
    if (title) {
        putText(title);
    }

#ifndef CV_NOARCHIVER
    selectMthdIndex = -1L;
    dblClkMthdIndex = -1L;
    dblClkClientIndex = -1L;
    listOStrings = 0;
    iSelect = -1;
#endif
}

// ---------------------------------------------------------------------------
// Construct a VListBox from the resource control-id 'cntrl' of the
// resource associated with 'pWin'. This is a non-portable GUI system method.
//
VListBox::VListBox(unsigned cntrl, VWindow *pWin)
{
    getRscWindow(cntrl, pWin);
    init(0);
}

// ---------------------------------------------------------------------------
// Given the desired size of this VListBox, '*w' and '*h', compute the
// nearest size in terms of an integral number of characters in the default
// system font.
//
void VListBox::sizeOf(int *w, int *h)
{
    int cw, ch;
    int n = *h;
    fontSize(&cw, &ch);
    *w *= cw;
    *h *= ch;
    *w += GetSystemMetrics(SM_CXVSCROLL);
    *w += GetSystemMetrics(SM_CXBORDER) *4;
    *h += GetSystemMetrics(SM_CYBORDER) *n +2;
}

// ---------------------------------------------------------------------------
// Construct a VListBox with frame 'frame', parent 'win' and style 
// 'style'. Styles supported are StyleBorder, StyleSorted and 
// StyleHorizontal.  If no 'style' is given, then the default in listbox.h
// is StyleBorder.
//
VListBox::VListBox(const VFrame& frame, VWindow *win, const VStyle& style) 
{
    (*this)(frame, win, style);
    init(0);
}

// --------------------------------------------------------------------------
//
VListBox::VListBox(const VString& wname, const VFrame& frame, VWindow *pwin, 
                   const VStyle& style) 
{
    (*this)(frame, pwin, style, wname);
    init(0);
}

// --------------------------------------------------------------------------
// Return MS Windows Information
//
void VListBox::getInfo(VWinInfo &listinfo)
{
    VControl::getInfo(listinfo);
    listinfo.wClass = "listbox";
//  listinfo.style |= LBS_NOTIFY | WS_VSCROLL | WS_CLIPSIBLINGS;
//!!!jmd
    listinfo.style |= LBS_NOTIFY | WS_VSCROLL | WS_CLIPSIBLINGS | LBS_NOINTEGRALHEIGHT;

    if (style.contains(StyleSorted)) {
        listinfo.style |= LBS_SORT;
    }
    if (style.contains(StyleHorizontal)) {
        listinfo.style |= WS_HSCROLL;
    }

    listinfo.style &= ~WS_CAPTION;
    
    if (style.contains(StyleBorder)) {
        listinfo.style |= WS_BORDER;
    }
            
#if defined(CV_WIN32)
    if (has3dEffects(V_WIN3D_EFFECTS)) {
        listinfo.exStyle |= WS_EX_CLIENTEDGE;

        // remove the native style but leave the Views Style.
        listinfo.style &= ~WS_BORDER;
    }   
#endif

}

// --------------------------------------------------------------------------
// 
UINT VListBox::getCtlColor()
{

#if defined(CV_WIN32)
    return WM_CTLCOLORLISTBOX;
#else
    return CTLCOLOR_LISTBOX;
#endif

}

// ---------------------------------------------------------------------------
//
void VListBox::getMinSize(short &w, short &h)
{
    w = GetSystemMetrics(SM_CXMIN)+ GetSystemMetrics(SM_CXVSCROLL);
    h = GetSystemMetrics(SM_CYVSCROLL) * 2;
}

// ---------------------------------------------------------------------------
// Append the array of C strings, 'strs', to the end of the list in
// this VListBox. The array 'strs' must be terminated with a NULL pointer.
//
void VListBox::appendList(const char **strs)
{
    refresh(FALSE);
    for (; *strs; strs++)  {
        appendString(*strs);           
    }
    refresh(TRUE); 
}

// ---------------------------------------------------------------------------
// Insert an array of C strings, 'strs', into the list of this
// VListBox in alphabetical order.  The array 'strs' must be terminated
// with a NULL pointer.
//
void VListBox::insertList(const char **strs)
{
    refresh(FALSE);
    for (; *strs; strs++) {
        insertString(*strs);
    }
    refresh(TRUE);
}

// ---------------------------------------------------------------------------
// Insert an array of C strings, 'strs', into the list of this
// VListBox at 'idx'.  The array 'strs' must be terminated with a NULL
// pointer.
//
void VListBox::insertList(const char **strs, int idx)
{
    refresh(FALSE);
    for (; *strs; strs++) {
        insertString(*strs, idx++);
    }
    refresh(TRUE);
}

// ---------------------------------------------------------------------------
// Append C string 'str' at the end of the list of this VListBox.
//
boolean VListBox::appendString(const char *str)
{
    insertString(str, -1);
    return TRUE;
}

// ---------------------------------------------------------------------------
//
int VListBox::calculateExtent(const char * str)
{
#if 1
    VPort port(this);
    if (!port.open()) {
        return 0;
    }
    int extent, border, dummy;
    boolean delete_font;
    VFont *font = getFont();
    if (font == 0) {
        font = VFont::getNativeWindowFont(this);
        if (font != 0) {
            port.useFont(font);
            delete_font = TRUE;
        }
        else {
            delete_font = FALSE;
        }
    }
    else {
        port.useFont(font);
        delete_font = FALSE;
    }
    port.textSize(str, &extent, &dummy);
    port.close();
    getSystemBorderSize(&border, &dummy);
    if (delete_font) {
        delete font;
    }
    return extent + border;
#else
    HDC context;
    context = GetDC(hWnd);
    SIZE Size;
    GetTextExtentPoint(context, str, strlen(str) , &Size);
    TEXTMETRIC tm;
    GetTextMetrics(context, &tm);
    int extent = Size.cx + tm.tmAveCharWidth;
    ReleaseDC(hWnd, context);
    return extent;
#endif
}

// ---------------------------------------------------------------------------
// Insert C string 'str' in the list of this VListBox in
// alphabetical order.
//
void VListBox::insertString(const char *str)
{
    int extent = calculateExtent(str);

    int cur_extent = 
    getHorizontalExtent();

    if (extent >cur_extent){
        setHorizontalExtent(extent);
    }
    SendMessage(hWnd, (unsigned)LB_ADDSTRING, (WPARAM)0, 
                            (LPARAM)(LPSTR)str);
	updateGeometry();
}

// ---------------------------------------------------------------------------
// Insert C string 'str' into the the list of this VListBox at
// index 'idx'.  If the 'idx' is a -1, then append the string to the
// end of the list.
//
void VListBox::insertString(const char *str, int idx)
{
    int extent = calculateExtent(str);
    int cur_extent = getHorizontalExtent();
    if (extent >cur_extent){
        setHorizontalExtent(extent);
    }
    SendMessage(hWnd, (unsigned)LB_INSERTSTRING, (WPARAM)idx, 
                    (LPARAM)(LPSTR)str);
	updateGeometry();
}

// ---------------------------------------------------------------------------
// Clear the list of this VListBox.
//
void VListBox::clearList()
{
    SendMessage(hWnd, LB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Return the number of items in the list of this VListBox.
//
int VListBox::count() const
{
    return (int) SendMessage(hWnd, LB_GETCOUNT, (WPARAM) 0, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Remove the 'idx'th list item from this VListBox.
//
void VListBox::deleteStringAt(int idx)
{
    LRESULT len = SendMessage(hWnd, LB_GETTEXTLEN, (WPARAM) idx, (LPARAM)0);
    char * pChar = new char[len + 1];

    SendMessage(hWnd,LB_GETTEXT, (WPARAM) idx,(LPARAM) pChar);

    int extent_being_removed = calculateExtent(pChar);

    int cur_extent = getHorizontalExtent();

    SendMessage(hWnd, LB_DELETESTRING, (WPARAM) idx, (LPARAM) 0);
    if (extent_being_removed == cur_extent){
        refreshHorizontalScrollBars();
    }
    delete [] pChar;
}

// ---------------------------------------------------------------------------
// Replace the 'idx'th list item from this VListBox with the string 'nustr'.
//
void VListBox::replaceStringAt(int idx, const char *nustr)
{
    if( idx < 0 ) return;
    refresh(FALSE);
    deleteStringAt( idx );
    insertString( nustr, idx);
    refresh(TRUE);
}

// ---------------------------------------------------------------------------
//
int VListBox::findString(const char* str, int start)
{
    return findPrefix(str, start);
}

// ---------------------------------------------------------------------------
// Return the first index of the first item in this VListBox matching the given
// prefix. Matching begins with the item whose index is 'start'. If no match is
// found, -1 is returned.
//
int VListBox::findPrefix(const char* str, int start)
{
    LRESULT idx = SendMessage( hWnd, LB_FINDSTRING, (WPARAM) start, (LPARAM) str );
    return (idx == LB_ERR) ? -1 : (int) idx;
}

// ---------------------------------------------------------------------------
// Selects the first item in this VListBox matching the given prefix. Matching
// begins with the item whose index is 'start'. The index of the item matched 
// is returned or -1 if no matchwas found.
//
int VListBox::selectPrefix(const char* str, int start)
{
    int idx = findPrefix( str, start );
    if( idx > 0 ) {
        selectItem( idx );
    }
    return idx;
}

// ---------------------------------------------------------------------------
// Fill this VListBox from the current directory with the file names,
// that match the VStringobject 'str'.  To obtain file names, subdirectories
// and/or drives use one or more of the listAll, listFiles (uses the filter
// string 'str'), listDrives, or listSubDirectories types as the parameter
// e.  These can be or\'d together to select combinations (listAll is
// equivalent to listFiles|listDrives|listSubDirectories).  Return the
// number of items placed in this VListBox.  Note: Passing a NIL for 'str'
// will result in the string '*.*' being used as the filter.
//
int VListBox::dir(VString *str, unsigned e)
{
    int      i = 0;
    unsigned a = 0;
    char     filter[40];

    if (!str) {
        lstrcpy(filter, "*.*");
    }
    else {
        str->gets(filter, 39);
    }
    clearList();
    if (e & listFiles) {
        i = (int)SendMessage(hWnd, LB_DIR, (WPARAM)0, (LPARAM)(LPSTR)filter);
    }
    a |= ExclusiveList;
    if (e & listDrives) {
        a |= DriveList;
    }
    if (e & listSubDirectories) {
        a |= SubDirectoryList;
    }

    if ((e & listDrives) || (e & listSubDirectories)) {
        i += (int)SendMessage(hWnd, LB_DIR, (WPARAM)a, (LPARAM)"*.*");
    }
    return i;
}

// ---------------------------------------------------------------------------
// PRIVATE: VControl notification event code: notification code
//
boolean VListBox::notify(int code)
{
    method mthd = 0; 
    VObject *clnt = 0;
    boolean ret = FALSE;
    switch (code) {
    case LBN_ERRSPACE:
        error("Out of memory for listbox!!");
        break;
    case LBN_SELCHANGE:
        mthd = selectMthd;
        clnt = client;
        ret = returnThis;
        break;
    case LBN_DBLCLK:
        mthd = dblClkMthd;
        clnt = dblClkClient;
        ret = dblClkReturnThis;
        break;
    case ACCELcode:
        takeFocus();
        return TRUE;
    }
    if (clnt && mthd != NIL_METHOD) {
        if (ret) {
            clnt->perform(mthd, (long)this);
        }
        else {
            clnt->perform(mthd, (long)selectedItem());
        }
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// ---------------------------------------------------------------------------
// If 'b' is FALSE, then disable the refresh of this VListBox during
// list item update, for example, adding strings, otherwise enable display
// refresh.  If refresh is enabled, the display of list items will flicker
// with a large number of updates.
//
void VListBox::refresh(boolean b)
{
    SendMessage(hWnd, WM_SETREDRAW, (WPARAM)b, (LPARAM) 0); 
    if (b) {
        update();
    }
}

// ---------------------------------------------------------------------------
// The implementation of this method in VListBox does nothing.
//
boolean VListBox::resized(int w, int h)
{
    w = h = 0;

    // Let windows handle the adjusting of the listbox's size
    return FALSE;
}

// ---------------------------------------------------------------------------
// Return the index of the currently selected item of this VListBox.
//
int VListBox::selectedItem() const
{
    return (int)SendMessage(hWnd, LB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Return the currently selected string of this VListBox as a
// newly created VString object.
//
VString *VListBox::selectedString() const
{
    char temp[150];
    VString *s = 0;
    if (SendMessage(hWnd, LB_GETTEXT, (WPARAM)selectedItem(), (LPARAM)(LPSTR)temp) > 0) {
        s = new VString(temp);
    }

    return s;
}

// ---------------------------------------------------------------------------
// Select the item at the index 'idx' of this VListBox.
//
void VListBox::selectItem(integer idx)
{
    SendMessage(hWnd, LB_SETCURSEL, (WPARAM)idx, (LPARAM)0);
}

// ---------------------------------------------------------------------------
// Construct a VListBox with no presentation space.
//
VListBox::VListBox() 
{ 
    init(0);
}

// ---------------------------------------------------------------------------
// Arrange to call method 'mthd' and 'dblmthd' of object
// 'clnt' when the user clicks or double-clicks on an entry in this VListBox. 
// The call back methods 'mthd' and 'dblmthd' will take a single integer
// argument that will contain the index of the list item selected.
// If either 'mthd' or 'dblmthd' is not used then the constant NIL_METHOD
// should be passed. The 'returnList' parameter defaults to FALSE, however, if
// it is set to TRUE, it indicates that a pointer to 'this' object be passed
// to the click methods instead of the list index of the selected item.
//
void VListBox::uponClick(VObject *clnt, method mthd, 
                            method dblmthd, boolean returnList) 
{
    returnThis = returnList; 
    if (clnt) {
        client = clnt; 
    }
    if (clnt && dblmthd) {
        dblClkClient = clnt; 
    }
    if (dblmthd) {
        dblClkReturnThis = returnList;
    }
    if (mthd) {
        selectMthd = mthd; 
    }
    if (dblmthd) {
        dblClkMthd = dblmthd; 
    }
    if (!clnt && mthd == NIL_METHOD && dblmthd == NIL_METHOD) {
        client = 0;
        dblClkClient = 0;
        selectMthd = dblClkMthd = 0;
    }
}

// ---------------------------------------------------------------------------
void VListBox::uponClick(VObject *clnt, method mthd, boolean returnList) 
{
    returnThis = returnList; 
    if (clnt) {
        client = clnt; 
    }
    if (mthd) {
        selectMthd = mthd; 
    }
    if (!clnt && mthd == NIL_METHOD) {
        client = 0;
        selectMthd = 0;
    }
}

// ---------------------------------------------------------------------------
void VListBox::uponDblClick(VObject *clnt, method mthd, boolean returnList) 
{
    dblClkReturnThis = returnList; 
    if (clnt) {
        dblClkClient = clnt; 
    }
    if (mthd) {
        dblClkMthd = mthd; 
    }
    if (!clnt && mthd == NIL_METHOD) {
        dblClkClient = 0;
        dblClkMthd = 0;
    }
}


// ---------------------------------------------------------------------------
enum WinType VListBox::winType()
{
    return WinControl;
}

// ---------------------------------------------------------------------------
//
void VListBox::getPreferredSize(int *width, int *height) const
{
    if (width != 0) {
        *width = ((VListBox*)this)->getHorizontalExtent() + GetSystemMetrics(SM_CXHSCROLL);
    }
    if (height != 0) {
        queryPreferredSize(0, height);
        int n = count();
        if (n > 12) { n = 12; }
        *height *= n;
    }
}

// ---------------------------------------------------------------------------
//
VString *VListBox::getStringAt(int i)
{
    if (i < count()) {
        LRESULT len = SendMessage(hWnd, LB_GETTEXTLEN, (WPARAM) i, (LPARAM)0);
        if (len == LB_ERR) {
        }
        else if (len > 0) {
            char *buf = new char [(int) len + 1];

            if (buf != 0) {
                *buf = '\0';
                SendMessage(hWnd, LB_GETTEXT, (WPARAM) i, (LPARAM)(LPCSTR)buf);

                VString *s = new VString(buf);
                delete [] buf;
                return s;
            }
        }
        else if (len == 0) {
            return new VString;
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sets the horizontal extent in pixels
//
void VListBox::setHorizontalExtent(int extent)
{
    SendMessage(hWnd, LB_SETHORIZONTALEXTENT, (WPARAM)extent,0);
}

// ---------------------------------------------------------------------------
// returns the horizontal extent in pixels the listbox is using
//
int VListBox::getHorizontalExtent( )
{
    return SendMessage(hWnd, LB_GETHORIZONTALEXTENT, (WPARAM)0, 0);
}

//----------------------------------------------------------
void VListBox::setFont(VFont *f)
{
    // Nothing different fron VControl::setFont except we 
    // need to update the horizontal scrollbar extent because 
    // the font may have changed.

    if (f == winFont) {
        VControl::setFont(f);
    }
    else {
        VControl::setFont(f);
        refreshHorizontalScrollBars();
    }
}

//-----------------------------------------------------------------
void VListBox::refreshHorizontalScrollBars()
{
    int current_extent=0;
    int last_extent = 0;
    int max_len;

    int count = SendMessage(hWnd, LB_GETCOUNT, 0,0);
    for (int i=0 ; i<count; i++){
        max_len = 
        SendMessage(hWnd,LB_GETTEXTLEN, (WPARAM) i,(LPARAM) 0);

        char * pChar = new char[max_len + 1];
        SendMessage(hWnd,LB_GETTEXT, (WPARAM) i,(LPARAM) pChar);
        current_extent = 
                calculateExtent(pChar);

        if (current_extent > last_extent){
            last_extent = current_extent;
        }

        delete [] pChar;
    }
    setHorizontalExtent(current_extent);
}

// ===========================================================================
#ifndef CV_NOARCHIVER   

// ---------------------------------------------------------------------------
//
VOrdCollect *VListBox::getList()
{
    VOrdCollect *oc = 0;

    if (count() > 0) {
        oc = new VOrdCollect(count());
    
        for (int i = 0; i < count(); i++) {
            (void) oc->add(getStringAt(i));
        }
    }

    return oc;
}

// --------------------------------------------------------------------------
// VListBox::putTo
//
void VListBox::putTo(VArchiver& a)
{
    VListBox::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
// VListBox::putTo
//
void VListBox::putTo(VArchiver& a, boolean most_derived)
{
    //  base object data

    VControl::putTo(a, FALSE); 

    a << VListBoxArchiveTag;

    //  contents of list as an ordered collection of strings

    if ((listOStrings = getList()) != 0) {
        DO(*listOStrings, VString, s)
            s->isTemporaryObject(TRUE);
        END
    }
    a.putObject(listOStrings, TRUE);
    if (listOStrings != 0) {    
        listOStrings->freeContents();
        delete listOStrings;
        listOStrings = 0;
    }
    a << selectedItem();

    //  name of select callback and the class to which it belongs

    const char *method_name = 0;            
    const char *class_name  = 0;            

    VCallbackList::findCallback(selectMthd, class_name, method_name, 
                                    selectMthdIndex);

    a << (char *) class_name; 
    a << (char *) method_name;

    //  name of double click callback and the class to which it belongs

    method_name = 0;            
    class_name  = 0;            

    VCallbackList::findCallback(dblClkMthd, class_name, method_name, 
                                    dblClkMthdIndex);

    a << (char *) class_name; 
    a << (char *) method_name;

    //  return list or index?

    a << returnThis;

    //  children

    if (VListBoxTagRev > 0) {
        // new for Workshop 1.1
        a << dblClkReturnThis;
        a.putObject(notifier->getClientName
                    (dblClkClient, dblClkClientIndex), TRUE);
    }

    //  children

    if (most_derived) {
        VWindow::putChildrenTo(a);
    }

}

// --------------------------------------------------------------------------
// VListBox::getFrom
//
void VListBox::getFrom(VArchiver& a)
{
    //  base object data
    
    VControl::getFrom(a);

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VListBoxArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VListBoxArchiveTag));
        return;
    }

    //  contents of list as an ordered collection of strings

    listOStrings = (VOrdCollect *) a.getObject(0);
    a >> iSelect;

    //  select method

    char        method_name[256];
    char *      method_name_ptr = method_name;
    char        class_name[256];
    char *      class_name_ptr  = class_name;

    a >> class_name_ptr;    
    a >> method_name_ptr;   

    method sm = VCallbackList::findCallback(class_name_ptr, method_name_ptr, 
                                                            &selectMthdIndex);

    if (client != 0 && sm != 0) {
        if (!client->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }

    //  double click method

    a >> class_name_ptr;    
    a >> method_name_ptr;   

    // the calculation for the double click used to be here, but we will 
    // delay till we get the dblClickClient

    //  return list or index?

    boolean b, b2;
    a >> b;


    if (getArchiveTagRev(tag) > 0) {
      a >> b2;
      VString client_name;                            //  name of client
      a.getObject(&client_name);
      dblClkClient = notifier->getClient(client_name, &dblClkClientIndex);
    }
    else {
      dblClkClient = client;
      b2 = b;
    }

    method dcm = VCallbackList::findCallback(class_name_ptr, method_name_ptr, 
                                                            &dblClkMthdIndex);

    if (dblClkClient != 0 && dcm != 0) {
        if (!dblClkClient->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }

    
    uponClick(client, sm, b);
    uponDblClick(dblClkClient, dcm, b2);
}


// --------------------------------------------------------------------------
// VListBox::getFrom
//
void VListBox::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VListBox::getFrom(a);

    if (do_realize) {
        VListBox::realize((VWindow *)data);
    }

    if (data != 0) {
        VWindow::getChildrenFrom(a);
    }
}

// --------------------------------------------------------------------------
// VListBox::realize
//
void VListBox::realize(VWindow *pwin)
{
    boolean sorted;
    sorted = (style.contains(StyleSorted));

    VControl::realize(pwin);
    
    if (listOStrings != 0) {
        refresh(FALSE);
        DO(*listOStrings, VString, s)   
            if (sorted) {
              insertString(s->gets());
            }
            else {
              appendString(s->gets());
            }
        END
        selectItem((integer) iSelect);
        refresh(TRUE);

        listOStrings->freeContents();
        delete listOStrings;
        listOStrings = 0;
    }

    // delayed client resolution
    VObject* selectClient = 0;
    VObject* dblClkClient = 0;
    if (client == 0 && clientIndex == -1 && selectMthd != 0) {
        client = VCallbackList::findClientForCallback(selectMthd, this);
        selectClient = client;
    }
    if (client == 0 && clientIndex == -1 && dblClkMthd != 0) {
        client = VCallbackList::findClientForCallback(dblClkMthd, this);
        dblClkClient = client;
    }

    if (selectMthd != 0 && dblClkMthd != 0 &&
        selectClient != dblClkClient) {
        // error; client mismatch; everyone loses
        client = 0;
    }

}

#endif //   CV_NOARCHIVER    
