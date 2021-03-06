// $Header:   Q:/views/win/vcs/control.cpv   1.50   Mar 04 1997 17:54:50   gregm  $ 

//  control.cpp
//
//  VControl implementation
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
//  10/05/92 jmd    preened
//  04/29/93 pat    added constructor that takes name argument;
//                  added getFrom(), putTo(), realize(); 
//                  included archiver.h
//  05/13/93 pat    added VString *controlText for realize().
//  05/20/93 jmd    added fontSize()
//  05/24/93 jmd    preened, removed free(), added shadow (paint())
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  07/27/93 pat    added clientIndex;
//                  updated getFrom() and putTo() to use client name list.
//  08/08/93 jmd    added test for border in getInfo
//  08/08/93 jmd    renamed value to idNo (added to archiver)
//  08/08/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  09/01/93 pat    added VString placeholder for archive validation info;
//                  included fontinfo.h and added VFontInfo to archive.
//  09/02/93 dgm    fix in getFrom().
//  09/20/93 dgm    fix in erased() for bad VGroup background
//                  repainting when focused.
//  09/22/93 pat    added archive tag
//  10/12/93 pat    initialized controlText realization data 
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  02/26/94 jmd    added test for hWnd in setFont
//  03/01/94 pat    putText() now takes const VString&
//  03/27/94 pat    CV_WIN32: cast appropriately for SendMessage().
//  09/05/94 dss    getInfo(): exStyle initialization.
//  10/30/94 jmd    added init(), stubs for tooltip
//  11/11/94 dss    Archiver tags macros.
//  06/12/95 jld    added support for ControlHelp (i.e. tooltip)
//  07/03/95 dgm    Merged in Intersolv changes:
//                  - Style3d for CV_WIN95.
//  09/14/95 evc    Adding realization of archived font information
//  09/14/95 dss    VStyle changes.
//  10/04/95 evc    Moved font manipulation to VWindow
//  10/12/95 dss    add default style functionality.
//  10/23/95 evc    Added n
//  11/01/95 dgm    Added #ifndef CV_NOOLE's.
//  11/02/95 dgm    Added getNonClientArea().
//  11/10/95 dgm    Removed getNonClientArea(); added getSystemBorderSize().
//                  Fixed getInfo() to check StyleBorder before putting on
//                  a 3-D border.
//  11/16/95 evc    Fixed setFont so it does sends a 0 font handle 
//                  to the window via WM_SETFONT when the VFont * arg is 0.
//  12/04/95 evc    Small change to setFont
//  01/04/96 dgm    Added another version of setControlHelpMsg().
//  01/04/96 dgm    Overrode getPreferredSize().
//  01/09/96 dss    VComponent name change.
//  01/29/96 dgm    Preferred-size changes.
//  02/01/96 dgm    Pop down tool-tip when on mouse click.
//  02/19/96 evc    Made defaultStyle CV_NEARDATA
//  02/23/96 dgm    Fix in queryPreferredSize().
//  09/11/96 dgm    Updated queryPreferredSize() to use VFont::system().
//  09/18/96 dgm    Fixed memory leak (controlText).
//	09/30/96 dgm	Added hack useBrokenFontSize() as a temporary workaround
//					for VRFs which were created using a PRW which was built
//					without the 9/20 fontSize() fix.
//	02/04/97 pkt	Changed 'init' to handle default styles later, and to use
//					transient styles.
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
// ----------------------------------------------------------------------------

#include "control.h"
#include "notifier.h"
#include "str.h"
#include "set.h"
#include "memry.h"
#include "font.h"
#include "port.h"
#include "group.h"
#include "cntrlhlp.h"
#include "cvassert.h"
#include "fontinfo.h"
#include "font.h"

// Compound Document Objects
#ifndef CV_NOOLE
#include "cmpwin.h"
#endif

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "fontinfo.h"
#endif

extern "C"{
#   undef NULL
#   include  <string.h>
}

#ifndef max
#   define max(a,b) ((a)>(b)?(a):(b))
#endif

defineClass(VControl, VWindow)
defineArchiveRevision(VControl, 2)

//----------------------------------------------------------------------------
VStyle& VControl::getClassDefaultStyle(void)
{
    static VWinStyle CV_NEARDATA classDefaultStyle(VWindow::getClassDefaultStyle() +
                                       setTransient + Style3D);

    return classDefaultStyle;
}

//----------------------------------------------------------------------------
VStyle& VControl::getDefaultStyle(void) const
{
    return getClassDefaultStyle();
}

// --------------------------------------------------------------------------
VControl::VControl() :
    client(0),
#ifndef CV_NOARCHIVER
    clientIndex(-1L),
    controlText(0),
#endif
    hasShadow(0)
{
    init();
}

// --------------------------------------------------------------------------
// Construct a VControl from the GUI system resource with id 'cntrl'
// within the window 'pwin'. This method may not be portable.
//
VControl::VControl(unsigned cntrl, VWindow *pwin) :
    client(0),
#ifndef CV_NOARCHIVER
    clientIndex(-1L),
    controlText(0),
#endif
    hasShadow(0)
{
    init(); 
    getRscWindow(cntrl, pwin);
}

// --------------------------------------------------------------------------
// Construct a VControl with frame 'frame' parent 'win' and style 'style'.
//
VControl::VControl(const VFrame &frame, VWindow *win, const VStyle& style) :
    client(0),
#ifndef CV_NOARCHIVER
    clientIndex(-1L),
    controlText(0),
#endif
    hasShadow(0)
{
    init();
    (*this)(frame, win, style);
}

// ---------------------------------------------------------------------------
//
VControl::VControl(const VString& wname, const VFrame& frame, VWindow *pwin, const VStyle& style) :
    client(0),
#ifndef CV_NOARCHIVER
    clientIndex(-1L),
    controlText(0),
#endif
    hasShadow(0)
{
    init();

    (*this)(frame, pwin, style, wname);
}

// --------------------------------------------------------------------------
void VControl::init()
{
	amUsingBrokenFontSize = 1;

    _ctrlHelp = 0;
    _ctrlHelpId = 0;

#ifndef CVS_LATER_DEFAULT
    style = setTransient + getDefaultStyle();

    // Note: can't call notifier during construction of classDefaultStyle
    // so we enforce 3D here. Should be done at construction since 3d
    // availablity is application scope.
#if defined(CV_WIN32) || defined(CV_CTL3D)
    if (notifier->threeDEffectsAvailable()) {
# ifdef CVS_USE_TRANSIENT
        style += setTransient + Style3D;
# else
        style += Style3D;
# endif
    }
#else
# ifdef CVS_USE_TRANSIENT
	// This is not necessary, as 'init' is called only from a contstructor.
# else
    // no 3d allowed.
    style -= Style3D;
# endif
#endif
#endif
}

// --------------------------------------------------------------------------
VControl::~VControl()
{
    if (winFont != 0){
        winFont->bind(this, FALSE);
    }
#ifndef CV_NOARCHIVER
	if (controlText != 0) {
		delete controlText;
	}
#endif
}

// --------------------------------------------------------------------------
// Called by the VNotifier to obtain info on the underlying GUI system
// window represented by this VControl object.
//
void VControl::getInfo(VWinInfo &cntrlinfo)
{
    cntrlinfo.wClass = notifier->CTwindow();
    cntrlinfo.style = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    cntrlinfo.exStyle = 0;

    if (!(style.contains(StyleHidden))) {
        cntrlinfo.style |= WS_VISIBLE;
    }

#ifdef CVS_LATER_DEFAULT
# if defined(CV_WIN32) || defined(CV_CTL3D)
    if (notifier->threeDEffectsAvailable()) {
#  ifdef CVS_USE_TRANSIENT
        style += setTransient + Style3D;
#  else
        style += Style3D;
#  endif
    }
# endif
#endif

    if (style.contains(StyleBorder)) {
#if defined(CV_WIN32)
        if (has3dEffects(V_WIN3D_EFFECTS)) {
            cntrlinfo.exStyle |= WS_EX_CLIENTEDGE;
        }       
#else
        cntrlinfo.style |= WS_BORDER;
#endif
    }

    hasShadow = style.contains(StyleShadow) ? 1 : 0;
}

// --------------------------------------------------------------------------
// Return the minimum width and height for this window in 'w' and
// 'h' respectively. Overloaded in sub-classes to return different sizes.
//
void VControl::getMinSize(short &w, short &h)
{
    w = max(GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CXHSCROLL));
    h = max(GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CXHSCROLL));
}

// --------------------------------------------------------------------------
//
VString VControl::getText()
{
	VString s;
	getText(s);
	return s;
}

// --------------------------------------------------------------------------
// Copy the text content of this VControl into 's' and the return the
// number of characters copied.
//
int VControl::getText(VString &s)
{
    unsigned n = sizeOfText();
    if (n == 0xffff) {      // strange MS Windows bug
        n = 256;
    }
    char* tempbuf = new char[n+1];
    tempbuf[0] = '\0';
    int bytes_copyed  = SendMessage(hWnd, WM_GETTEXT,
            (WPARAM)(n+1), (LPARAM)tempbuf);
    s = tempbuf;
    delete [] tempbuf;
    return bytes_copyed;
}

// --------------------------------------------------------------------------
// Return the number of characters in the text contents of this
// VControl. 
//
unsigned VControl::sizeOfText()
{
    return (unsigned) SendMessage(hWnd, WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0);
}

// --------------------------------------------------------------------------
// Set the text content of this VControl to 's'. 
//
void VControl::putText(const VString& s)
{
	//
	// This was apparently put in to allow strings containing tildes
	// to be treated on other systems as they are under OS/2 where they
	// are treated the way ampersands are treated everywhere else.  But,
	// this significantly slows things down, *and* makes it impossible
	// to have an actual tilde in a control string.
	//
    //	VString newString;
    //	newString.scanTilde(s.gets());
	//
    SendMessage(hWnd, WM_SETTEXT, (WPARAM) 0, (LPARAM)(LPCSTR)(s.gets()));
}

// --------------------------------------------------------------------------
// Called from the VNotifier to handle misc. GUI system events.
//
boolean VControl::notify(int code)
{
	if (code == ACCELcode) {
		if (!VWindow::checkLosingFocus()) {
			takeFocus();
			return TRUE;
		}
	}
	return FALSE;
}

// --------------------------------------------------------------------------
boolean VControl::getRscWindow(unsigned cntrl, VWindow *pwin)
{
    int x, y, w, h;

    if (!(hWnd = GetDlgItem(notifier->dlg(), (WORD)cntrl))) {
        print("cannot locate resource control %d", cntrl);
        return FALSE;
    }
    parent = pwin;
    fromRsc = 1;
    getRel(&x, &y, &w, &h);

    // Set VWindow VFrame from resource
    winFrame.set(x, y, w, h, CornerDim);

    return notifier->createWin(this, 0, 0, 0, 0);
}

// --------------------------------------------------------------------------
// To be implemented by subclasses to return the width and height in
// pixels of the control given control dependent width and height requirements.
// For example, to return pixel dimensions given the width and height of the
// characters in the control\'s label.
//
void VControl::sizeOf(int *w, int *h)
{
    ;
}

// --------------------------------------------------------------------------
// Set '*w' and '*h' to the width and height of the standard
// size of the control 'cls'.  These values are set in a class dependant
// fashion via a call to sizeOf().
//
void VControl::stdSize(VClass *cls, int *w, int *h)
{
    VControl *c = (VControl *)cls->construct();
    c->sizeOf(w, h);

    delete c;       // formerly c->free(), jmd
}

// --------------------------------------------------------------------------
// Set VFont object 'f' for use by this VControl in writing its text.  
// NOTE:  VControl objects do not free the VFont object.  The user of this
// VControl is responsible for the disposition of the VFont object after
// the VControl is freed.
//
void VControl::setFont(VFont *f) 
{
    VFont *oldFont = winFont;

    if (hWnd != 0) {
        if (f != 0){
            // make this VWindow a client of the VFont
            f->bind(this, TRUE);
            SendMessage(hWnd, WM_SETFONT, (WPARAM)(f->getHndl()), (LPARAM) 1);
        } else {
			SendMessage(hWnd, WM_SETFONT,
							  WPARAM(VFont::system().getHndl()), LPARAM(1));
        }
    } 

    // unbind the old font
    if ((oldFont != 0) && (oldFont != f))  {
        oldFont->bind(this, FALSE); // unbind the old one
    }
    winFont = f;
}

// --------------------------------------------------------------------------
// Copy upto 'len' characters into 'buffer' from the text content
// of this VControl and return the number of characters copied. The 'buffer' 
// must have room for 'len' + 1 characters.
//
int VControl::getText(char *buffer, unsigned len)
{
    VString s;
    unsigned n = (unsigned)getText(s);

    buffer[0] = '\0';
    if (n > len) {
        n = len;
    }

    n = (int)s.gets(buffer, (integer)n);
    return n;
}

// ---------------------------------------------------------------------------
// In general, don\'t take default action for background erasure events.
// Let each VControl subclass decide this event.
//
boolean VControl::erased()
{
    return FALSE;
}

// ---------------------------------------------------------------------------
// Associates a VControlHelp object with a control
//
void VControl::setControlHelp(VControlHelp* help)
{
    _ctrlHelp = help;
}

// ---------------------------------------------------------------------------
// 
//
long VControl::setControlHelpMsg(VString* msg, long id)
{
    if( 0 == _ctrlHelp ) {
        _ctrlHelp = VControlHelp::getDefault();
    }
    return _ctrlHelpId = ( id == 0 ) ? 0 : _ctrlHelp->storeMsg( msg, id );
}

// ---------------------------------------------------------------------------
//
long VControl::setControlHelpMsg(const VString& msg, long id)
{
    if (_ctrlHelp == 0) {
        _ctrlHelp = VControlHelp::getDefault();
    }
    return _ctrlHelpId = (id == 0) ? 0 :
                         _ctrlHelp->storeMsg((VString *)(&msg), id);
}

// ---------------------------------------------------------------------------
//
boolean VControl::mouseEnter(int button_down)
{
    if (button_down == 0) {
        if( _ctrlHelp != 0 ) {
            _ctrlHelp->tipPlease( this, _ctrlHelpId );
        }
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VControl::mouseExit(int)
{
    if( _ctrlHelp != 0 ) {
        _ctrlHelp->tipThankYou( this, _ctrlHelpId );
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VControl::mouseDn(int x, int y)
{
    if( _ctrlHelp != 0 ) {
        _ctrlHelp->hideNow();
    }
    return VWindow::mouseDn(x, y);
}

// ---------------------------------------------------------------------------
//
boolean VControl::rmouseDn(int x, int y)
{
    if( _ctrlHelp != 0 ) {
        _ctrlHelp->hideNow();
    }
    return VWindow::rmouseDn(x, y);
}

// ---------------------------------------------------------------------------
// if shadow is turned on, draw it on the parent window
//
boolean VControl::paint()
{
#ifndef CV_NOOLE
    if (getComponentThis()) {
        getComponentThis()->paint();
    }
#endif

    if (hasShadow) {

        // ToDo!! Drawing on the parent is totally bogus!

        VWindow *pwin = getParent();

        if (pwin != 0) {
        // draw shadow
        VPort port(pwin);

        port.open();

        int x, y, w, h;
        getPhysical().get(CornerDim, &x, &y, &w, &h);

        port.moveTo(x + 1, y + h);
        port.lineTo(x + w + 1, y + h);

        port.moveTo(x + w, y + 1);
        port.lineTo(x + w, y + h);

        port.close();
    }
    }

    return FALSE;
}

// ---------------------------------------------------------------------------
// Called when this VControl has been given the keyboard focus.  Return
// FALSE to specify system default behavior for 'VControl' objects.
//  
boolean VControl::givenFocus() 
{
    return VWindow::givenFocus(); 
}

// ---------------------------------------------------------------------------
// The implementation of this method in VControl passes the key to this 
// Controls\'s parent (see VWindow::key()).
//
boolean VControl::key(int virtual_key, char ascii_key)
{
	if (VWindow::key(virtual_key, ascii_key)) {
		return TRUE;
	}
	else {
		return propagateKey(virtual_key, ascii_key);
	}
}

// ---------------------------------------------------------------------------
//
void VControl::fontSize(int *w, int *h)
{
	if (amUsingBrokenFontSize) {
		VWindow::fontSize(w, h);
		return;
	}

    if (winFont == 0) {
		VFont& font = VFont::system();
		if (w != 0) { *w = font.width(); }
		if (h != 0) { *h = font.height(); }
    }
	else {
		VWindow::fontSize(w, h);
	}
}    

// ---------------------------------------------------------------------------
//
void VControl::queryPreferredSize(int *width, int *height,
                                  boolean extra_horizontal_space,
                                  boolean extra_vertical_space,
								  boolean multiline,
                                  VFont *font, const char *text) const
{
    int w, h;
    const int buffer_size = 2047;
    char buffer[buffer_size + 1];

	if (text == 0) {
		if (((VControl *)this)->getText(buffer, buffer_size) <= 0) {
			strcpy(buffer, "C++/Views");
		}
		text = buffer;
	}

    VPort port((VControl *)this);

    port.open();

    if ((font == 0) && ((font = getFont()) == 0)) {
		font = &VFont::system();
    }

    port.useFont(font);

	if (multiline) {
		VRectangle r; int dummy;
	    port.wrtText(text, &r, CalcRect);
		r.get(CornerDim, &dummy, &dummy, &w, &h);
	}
	else {
	    port.textSize(text, &w, &h);
	}

    port.close();

    if (extra_horizontal_space) {
        w = float(w) + (int(float(h) * 1.5) - h) * 2;
    }

    if (extra_vertical_space) {
        if (font != 0) {
            h = font->height();
        }
        h = int(float(h) * 1.5);
    }

    if (w < 4) { w = 4; }
    if (h < 4) { h = 4; }

    if (width  != 0) { *width  = w; }
    if (height != 0) { *height = h; }
}

// ---------------------------------------------------------------------------
//
void VControl::getPreferredSize(int *width, int *height) const
{
    queryPreferredSize(width, height);
}

// ---------------------------------------------------------------------------
//
void VControl::getSystemBorderSize(int *width, int *height) const
{
    VWindow::getSystemBorderSize(width, height);
}

// ----------------------------------------------------------------------------
//
void VControl::getInternalBorderGeometry(int *left, int *right,
                                         int *top, int *bottom) const
{
    VWindow::getInternalBorderGeometry(left, right, top, bottom);
}

// ----------------------------------------------------------------------------
//
void VControl::systemFontChanged()
{
	if (winFont == 0) {
		SendMessage(hWnd, WM_SETFONT,
						  WPARAM(VFont::system().getHndl()), LPARAM(1));
	}
	else {
		SendMessage(hWnd, WM_SETFONT,
						  WPARAM(winFont->getHndl()), LPARAM(1));
	}
	updateGeometry(-1);
}

// ===========================================================================

#ifndef CV_NOARCHIVER   

// --------------------------------------------------------------------------
// VControl::putTo
//
void VControl::putTo(VArchiver& a)
{
    VControl::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//  Archive this VControl object and, if most derived, its children.
//
//  Type:           What:
//                      VWindow base class object data
//      VString         text label
//      VFontInfo       fontInfo
//      int             idNo    (jmd)
//      VString         client's window name
//
void VControl::putTo(VArchiver& a, boolean most_derived)
{
    VWindow::putTo(a, FALSE);   //  base object data

    a << VControlArchiveTag;

    VString s;
    getText(s);
    a.putObject(&s, TRUE);      //  VString text label (TRUE means temp obj)

//  if ( VControlTagRev == 0){    // modern Views does this in VWindow
//  VFontInfo fontinfo(getFont());
//  a.putObject(&fontinfo, TRUE);
//  }

    a << idNo;                  //  data idNo

    //  name of client  
    a.putObject(notifier->getClientName(client, clientIndex), TRUE);

    //  placeholder for future validation information
    VString validation;
    a.putObject(&validation, TRUE);

    if (most_derived) {
        VWindow::putChildrenTo(a);
    }
}

// --------------------------------------------------------------------------
// VControl::getFrom
//
void VControl::getFrom(VArchiver& a)
{
    VWindow::getFrom(a);                            //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VControlArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VControlArchiveTag));
        return;
    }

    controlText = (VString *)   a.getObject(0);     //  text label VString


	if (getArchiveTagRev(tag) == 0) {
		VFontInfo finfo;
		a.getObject(&finfo);
		// if pointSize ==0, fontinfo was unitialized
		if (finfo.pointSize() != 0) {
			if (finfo.stockObject()) {
				winFont = new VFont(finfo.stockObject(), 1); 
				// create the STOCKOBJECT font 
			}
			else {
				winFont = new VFont(finfo); // create the font 
			}
		}
	}

	if (getArchiveTagRev(tag) < 2) {
		amUsingBrokenFontSize = 1;
	}

    a >> idNo;                                      //  data idNo

    VString client_name;                            //  name of client
    a.getObject(&client_name);
    client = notifier->getClient(client_name, &clientIndex);

    //  placeholder for future validation information
    VString validation;
    a.getObject(&validation);
}

// --------------------------------------------------------------------------
// VControl::getFrom
//
void VControl::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VControl::getFrom(a);               

    if (do_realize) {
        VControl::realize((VWindow *)data);
    }

    if (data != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

// --------------------------------------------------------------------------
// VControl::realize
//
void VControl::realize(VWindow *pwin)
{
    VWindow::realize(pwin);

    if (controlText != 0) {
        putText(*controlText);
        delete controlText;     
        controlText = 0;
    }

    setFont(winFont);
}

#endif // !CV_NOARCHIVER    
