// $Header:   Q:/views/win/vcs/font.cpv   1.27   Mar 04 1997 17:55:06   gregm  $ 

//  font.cpp
//
//  VFont implementaton.
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
//  10/20/92 jmd    (teb) added support for 8514oem and terminal typefaces
//  04/07/93 pat    changed id to VObject *
//  05/13/93 pat    preened a bit;
//                  added getFrom(), putTo(), realize();
//                  included archiver.h, str.h
//  07/20/93 jmd    minor preening, removed free()
//  07/20/93 jmd    made face use const VString
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//  09/01/93 pat    removed VArchiver support
//  09/01/93 dgm    M8 changes; != cast.
//  01/12/94 pat    fixed point size calculation by removing internal
//                  leading in select();
//                  set point size of stock fonts in select();
//                  reset stock and actualLog members under make(int);
//                  changing the attributes of stock fonts that are not
//                  bound to a port now creates a non-stock font;
//                  preened.
//  02/02/94 pat    added _height (positive, height of cell) distinct from
//                  LOGFONT.lfHeight (negative, height of glyph) to ease
//                  point size calculation.
//  03/05/94 pat    STRICT changes:
//                  hFont is now HFONT, not HANDLE.
//                  cast appropriately for DeleteObject() and SelectObject().
//  03/25/94 pat    cast max char width from int/long to short in select().
//  07/14/94 jld    replaced NULL with 0
//  11/07/94 dss    STRICT. MakeProcInstance() for EnumFontFamilies. Fixes
//                  Borland static build crash.
//  09/08/95 evc    1)  Font handle creation and deletion now tracks
//                  VControls that  are using the VFont, and updates
//                  them if the font object changes. Also fixed places where
//                  VFont usage by ports  needed to be tracked, but wasn't.
//                  Added method, updateControls(), to do this.
//                  2) added a fontinfo object to class data.
//                  3) added copy constructor
//                  4) added assignment operator
//                  lots more ....
//  09/14/95 dss    Iterating Platform type fix.
//  10/05/95evc     Added private ctor that builds a VFont from
//                  a stockobject ID
//  10/05/95 evc    Some fallout from making setFont virtual.
//  10/12/95 evc    removed assignment operator
//  11/02/95 evc    small fix to make ()
//  11/15/95 evc    added getNativeWindowFont(VWindow *)
//  11/29/95 pkt    fixed 'make()' to zero out hFont if it's deferring the make.
//  04/26/96 dgm    Added useOldDefaultFont().
//  09/09/96 dgm	Added system(); fixed some minor BoundsChecker problems.
//  09/18/96 dgm	Fixed memory leak in bind(FALSE).
// ----------------------------------------------------------------------------

#include "font.h"
#include "port.h"
#include "control.h"
#include "iterator.h"
#include "str.h"
#include "notifier.h"   //  for application instance handle in enum fonts

#include "fontinfo.h"

extern "C" {
#   include <string.h>
}

defineClass(VFont, VObject)

// ---------------------------------------------------------------------------
//
static boolean use_old_default_font = FALSE;

// ---------------------------------------------------------------------------
//
void VFont::useOldDefaultFont(boolean b)
{
    use_old_default_font = b;
}

// ---------------------------------------------------------------------------
//  Construct the default GUI system VFont object, which is a 
//  variable pitched, proportional width font.
//
VFont::VFont()
    : controls(0), ports(0)
{
    fontinfo = new VFontInfo;

    ptSize = 0;
    actualLog = FALSE;
    stock = FALSE;
	isSystem = 0;

    if (use_old_default_font) {
        if ((hFont = HFONT(GetStockObject(SYSTEM_FONT))) != 0) {
            GetObject(hFont, sizeof(LOGFONT), LPSTR(&lf)); 
            stock = SYSTEM_FONT;
        }
    }
    else if ((hFont = HFONT(GetStockObject(ANSI_VAR_FONT))) != 0) {
        GetObject(hFont, sizeof(LOGFONT), (LPSTR) &lf); 
        stock = ANSI_VAR_FONT;
        //
        // This sets up the lf structure and the VFontInfo so
        // we have characteristics that describe the Stock Font
        //
        getActual();
    }
}

// ---------------------------------------------------------------------------
//  Construct a system VFont object with fixed pitch
//  (non-proportional spacing) if 'fixed' is TRUE, otherwise construct
//  a system VFont object with variable pitch (proportional spacing) if
//  fixed is FALSE. 
//
VFont::VFont(boolean fixed)
    : controls(0), ports(0)
{
    fontinfo = new VFontInfo;

    ptSize = 0;
    ports = 0;
    actualLog = FALSE;
    stock = FALSE;
	isSystem = 0;

    if (fixed < 0) {
        //
        // Undockumented hack to allow insistent
        // users to obtain the old default font.
        //
        if ((hFont = HFONT(GetStockObject(SYSTEM_FONT))) != 0) {
            GetObject(hFont, sizeof(LOGFONT), LPSTR(&lf)); 
            stock = SYSTEM_FONT;
        }
    }
    else if (use_old_default_font) {
        if ((hFont = HFONT(GetStockObject
                           (fixed ? SYSTEM_FIXED_FONT :
                                    SYSTEM_FONT))) != 0) {
            GetObject(hFont, sizeof(LOGFONT), LPSTR(&lf)); 
            stock = fixed ? SYSTEM_FIXED_FONT : SYSTEM_FONT;
        }
    }
    else if ((hFont = HFONT(GetStockObject
                            (fixed ? ANSI_FIXED_FONT :
                                     ANSI_VAR_FONT))) != 0) {
        GetObject(hFont, sizeof(LOGFONT), LPSTR(&lf)); 
        stock = fixed ? ANSI_FIXED_FONT : ANSI_VAR_FONT;
        getActual();
    } 
}

//-----------  Create a stock object font
VFont::VFont(int stock_id, int dummy):controls(0), ports(0)
{
    fontinfo = new VFontInfo();

    ptSize = 0;
    actualLog = FALSE;
    stock = FALSE;
	isSystem = 0;

    if ((hFont = (HFONT) GetStockObject(stock_id)) != 0) {
        GetObject(hFont, sizeof(LOGFONT), (LPSTR) &lf); 
        stock = stock_id;

        // This sets up the lf structure and the VFontInfo so
        // we have characteristics that describe the Stock Font
        getActual();
    }
}
//----------------------------------------------------------------------------
// Construct a VFont from a VFontInfo object.  These objects contain
// font descriptions for all platforms.  Hense, this is the only
// constructor that is crossplatformi (provioded the VFontInfo object is 
// correctly 'filled' out for each platform.
VFont::VFont(VFontInfo& fi): controls(0), ports(0)
{
	isSystem = 0;

    // If the stockobject member is not 0 then
    // we want a stock object, and we ignore all the rest of the
    // information
    if (fi.stockObject()){

    return ;
    }

    // first copy the fontinfo object into our private data
    fontinfo = new VFontInfo(fi);


    // now map cross platform logical fomt information to a platform
    // specific logical font information, in this case the LOGFONT structure.

    ptSize              = fi.pointSize();
    lf.lfWidth          = 0;
    lf.lfEscapement     = 0;
    lf.lfOrientation    = fi.orientation();
    lf.lfWeight         = fi.bold() ? FW_BOLD: FW_NORMAL;
    lf.lfItalic         = fi.italic();
    lf.lfUnderline      = fi.underline();
    lf.lfStrikeOut      = fi.strikeOut();
    lf.lfCharSet =          DEFAULT_CHARSET;
    lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
    lf.lfQuality        = DEFAULT_QUALITY;
    //lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    lf.lfPitchAndFamily = 0;
    copyFace(fi.face());

    hFont = 0;
    actualLog = FALSE;
    stock = FALSE;
//  hFont = getHndl();
}
//----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  Construct a VFont object with typeface 'face' and point size
//  'pointSize'. Set the weight of this VFont object to bold if 'bold'
//  is TRUE, set its style to italics if italics is TRUE, enable
//  underlining if 'undLin' is TRUE, and enable strikeout if 'strike'
//  is TRUE. 
//
VFont::VFont(   const VString& face, 
                int         points, 
                boolean     bold, 
                boolean     italics, 
                boolean     undLin, 
                boolean     strike      ) :controls(0) , ports(0)
{
    fontinfo = new VFontInfo();



    ptSize              = points;
    lf.lfWidth          = 0;
    lf.lfEscapement     = 0;
    lf.lfOrientation    = Normal;
    lf.lfWeight         = bold ? FW_BOLD: FW_NORMAL;
    lf.lfItalic         = italics;
    lf.lfUnderline      = undLin;
    lf.lfStrikeOut      = strike;
    //lf.lfCharSet      = ANSI_CHARSET;
    lf.lfCharSet = DEFAULT_CHARSET;

    lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
    lf.lfQuality        = DEFAULT_QUALITY;
    //lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    lf.lfPitchAndFamily = 0;
    copyFace(face);

    hFont = 0;
    actualLog = FALSE;
    stock = FALSE;
	isSystem = 0;

    // Keep the fontinfo structure. updated. Notice that we are 
    // initializing the fontinfo for the other platforms
    // to  even though we do not have a platform specific information.
    // Usually the face name, and perhaps the fontsize will be the main
    // problem areas.

    for (int i = (int) VFontInfo::first_platform; i<= 
        (int) VFontInfo::last_platform; i++)
    {
        VFontInfo::Platform     j = (VFontInfo::Platform) i;

        fontinfo->orientation(Normal,j);
        fontinfo->pointSize(points,j);
        fontinfo->bold(bold,j);
        fontinfo->italic(italics,j);
        fontinfo->underline(undLin,j);
        fontinfo->strikeOut(strike,j);
        fontinfo->face(face,j);
    }
    //hFont = getHndl();

}

// ---------------------------------------------------------------------------
//
VFont::VFont(LOGFONT& logical_font)
	: controls(0), ports(0)
{
	lf			= logical_font;
    fontinfo	= new VFontInfo;
    actualLog	= FALSE;
    stock		= FALSE;
	isSystem	= 0;

	HDC hdc = CreateCompatibleDC(0);    
	int vertical_resolution = GetDeviceCaps(hdc, LOGPIXELSY);
    hFont = CreateFontIndirect(&logical_font);
	HFONT save_hfont = HFONT(SelectObject(hdc, hFont));
	TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);           
	int internal_leading = tm.tmInternalLeading;
	SelectObject(hdc, save_hfont);
	DeleteDC(hdc);

	if (logical_font.lfHeight < 0) {
		//
		// Map the character height (em) to point-size.
		//
		int character_height = -logical_font.lfHeight;
		ptSize = (character_height * 72) / vertical_resolution;
	}
	else if (logical_font.lfHeight > 0) {
		//
		// Map the cell height to point-size.
		//
		int character_height = logical_font.lfHeight - internal_leading;
		ptSize = (character_height * 72) / vertical_resolution;
	}
	else {
		ptSize = 0;
	}
}

// ---------------------------------------------------------------------------
//
VFont::~VFont()
{
    clean();

	if (isSystem) {
		systemFontDeleted(this);
	}
}

// ---------------------------------------------------------------------------
//  Set the point size of this VFont object to 'ps'.
//
void VFont::pointSize(int ps)
{
    if (ptSize != ps) {
        ptSize = ps;
        make();
    }
    fontinfo->pointSize(ps);
}

// ---------------------------------------------------------------------------
//  Set the 'typeface' of this VFont object to the name 'f'.
//
void VFont::face(const VString& f)
{
    copyFace(f);
    make();
    fontinfo->face(f);
}

// ---------------------------------------------------------------------------
//  Set the angle of rotation of this VFont object
//  to 'o' where 'o' is specified as an angle (times ten) of rotation in
//  a counterclockwise direction, for example, the value 900 would
//  represent 90 degrees to the left. The parameter 'o' can also be
//  specified by one of the enum types Orient: Normal, Left90,
//  Right90, and UpsideDown. 
//
void VFont::orientation(int o)
{
    if (lf.lfEscapement != o) {
        lf.lfEscapement = o;
        make();
    }
}

// ---------------------------------------------------------------------------
//  Set the weight of this VFont object to bold if 'b' is TRUE,
//  otherwise set the weight to normal if 'b' is FALSE.
//
void VFont::bold(boolean b)
{
    if (((lf.lfWeight == FW_NORMAL) && b) || 
        ((lf.lfWeight != FW_NORMAL) && !b)) {

        lf.lfWeight = b ? FW_BOLD: FW_NORMAL;
        make();
    }
    fontinfo->bold(b);
}

// ---------------------------------------------------------------------------
//  Set the style of this VFont object to italics if 'b' is TRUE, 
//  otherwise set the style to normal if 'b' is FALSE.
//
void VFont::italic(boolean i)
{
    if (((lf.lfItalic == 0) && i) || ((lf.lfItalic != 0) && !i)) {
        lf.lfItalic = i ? 1 : 0;
        make();
    }
    fontinfo->italic(i);
}

// ---------------------------------------------------------------------------
//  Set the style of this VFont object to contain underlined characters
//  if 'b' is TRUE, otherwise set the style to normal if 'b' is FALSE.
//
void VFont::underline(boolean u)
{
    if (((lf.lfUnderline == 0) && u) || ((lf.lfUnderline != 0) && !u)) {
        lf.lfUnderline = u ? 1 : 0;
        make();
    }
    fontinfo->underline(u);
}

// ---------------------------------------------------------------------------
//  Set the style of this VFont object to contain the strikeout line
//  if 'b' is TRUE, otherwise set the style to normal if 'b' is FALSE.
//  
void VFont::strikeOut(boolean s)
{
    if (((lf.lfStrikeOut == 0) && s) || ((lf.lfStrikeOut != 0) && !s)) {
        lf.lfStrikeOut = s ? 1 : 0;
        make();
    }
    fontinfo->strikeOut(s);
}

// ---------------------------------------------------------------------------
//  Set the internal MS Windows LOGFONT structure to 'logfnt'. 
//
//  Note that this is a non-portable method for MS Windows only.
//
void VFont::logfont(LOGFONT &logfnt)
{
    lf = logfnt;
    make();
    // we copy the fontinfo information for this platform
    // to all the platforms.
    // We get the information from the private data access routines 
    // of the VFont we are in the middle of creating

    for (int i = (int) VFontInfo::first_platform; i<= 
        (int) VFontInfo::last_platform; i++)
    {
        VFontInfo::Platform j = (VFontInfo::Platform) i;

        fontinfo->orientation( (Orient)orientation(),j);
        fontinfo->pointSize(pointSize(),j);
        fontinfo->bold(bold(),j);
        fontinfo->italic(italic(),j);
        fontinfo->underline(underline(),j);
        fontinfo->strikeOut(strikeOut(),j);
        fontinfo->face(face(),j);
    }
}

// ---------------------------------------------------------------------------
//  Return a pointer to this VFont object\'s 'typeface' name.
//
char *VFont::face()
{
    if (!actualLog) {
        getActual();
    }
    return (char *) lf.lfFaceName;
}

// ---------------------------------------------------------------------------
//  Return TRUE if this VFont object has the bold weight, otherwise
//  return FALSE.
//
boolean VFont::bold()
{
    if (!actualLog) {
        getActual();
    }
    if  (lf.lfWeight != FW_NORMAL) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// ---------------------------------------------------------------------------
//  Return TRUE if this VFont object\'s characters are italicized, 
//  otherwise return FALSE.
//
boolean VFont::italic()
{
    if (!actualLog) {
        getActual();
    }
    return (boolean) lf.lfItalic;
}

// ---------------------------------------------------------------------------
//  Return TRUE if this VFont object\'s characters are underlined, 
//  otherwise return FALSE.
//
boolean VFont::underline()
{
    if (!actualLog) {
        getActual();
    }
    return (boolean) lf.lfUnderline;
}

// ---------------------------------------------------------------------------
//  Return TRUE if this VFont object\'s characters have the
//  strikeout line, otherwise return FALSE.
//  
boolean VFont::strikeOut()
{
    if (!actualLog) {
        getActual();
    }
    return (boolean) lf.lfStrikeOut;
}


// ---------------------------------------------------------------------------
//  Return the height of this VFont object\'s character cell in pixels.
//
int VFont::height()
{
    if (!actualLog) {
        getActual();
    }
    return _height;
}

// ---------------------------------------------------------------------------
//  Return the width of this VFont object\'s character cell in pixels.
//
int VFont::width()
{
    if (!actualLog) {
        getActual();
    }
    return lf.lfWidth;
}

// ---------------------------------------------------------------------------
//  Return the maximum width of a character in this VFont object\'s
//  character cell in pixels.
//
short VFont::maxWidth()
{
    if (!actualLog) {
        getActual();
    }
    return mWidth;
}  



#if defined(CV_WIN32)
int CALLBACK EnumFontFamProc(ENUMLOGFONT FAR*  lpnlf, NEWTEXTMETRIC FAR* lpntm, 
            int FontType, LOGFONT FAR* lf){
    
    if (strcmp((char *)lf->lfFaceName, lpnlf->elfLogFont.lfFaceName)== 0) {  
    // if the requested facename is matched, record its character set
            lf->lfQuality = lpnlf->elfLogFont.lfQuality;
            lf->lfPitchAndFamily = lpnlf->elfLogFont.lfPitchAndFamily ;
            lf->lfCharSet = lpnlf->elfLogFont.lfCharSet;
            return 0;
    }
    return 1;
}   
#else
int CALLBACK EnumFontFamProc(LOGFONT FAR*  lpnlf, TEXTMETRIC FAR* lpntm, 
            int FontType, LOGFONT FAR* lf){
    
    if (strcmp((char *)lf->lfFaceName, lpnlf->lfFaceName)== 0) {  
    // if the requested facename is matched, record its character set
            lf->lfQuality = lpnlf->lfQuality;
            lf->lfPitchAndFamily = lpnlf->lfPitchAndFamily ;
            lf->lfCharSet = lpnlf->lfCharSet;
            return 0;
    }
    return 1;
}   
#endif


// ---------------------------------------------------------------------------
//  Copy VFont face name and set LogFont family and character set.
//
void VFont::copyFace(const VString& f)
{  
    
    if (f != (const char *)"") {
        FARPROC lpfnEnumProc;

        strncpy((char *)lf.lfFaceName, f.gets(), LF_FACESIZE - 1); 
        // LogFont family and character set are gotten within EnumFontFamProc
        HDC hdc = GetDC(0);

        lpfnEnumProc = 
        MakeProcInstance((FARPROC)EnumFontFamProc, notifier->inst());

        EnumFontFamilies(hdc, NULL,(FONTENUMPROC)EnumFontFamProc, 
                            LPARAM(&lf)); 

        FreeProcInstance(lpfnEnumProc);

        ReleaseDC(0,hdc);         
        
    }
}

// ---------------------------------------------------------------------------
//  VFont objects can be bound to more
//  than one VPort object at a time. This mechanism is designed to
//  support the techniques used by MS Windows to create its font objects.
//  If an attribute of a font changes, e.g., the point size, then in MS
//  Windows the existing font object must be destroyed and a new one
//  created. Hence, because of this binding, the VFont objects can then
//  notify the VPort objects they are bound too when the MS Windows font
//  handle has changed.
//
void VFont::bind(VObject *p, boolean b)
{
    if (b) {
        if (p->iam()->isa(VPortCls)){
		    if (ports == 0) {
		        ports = new VOrdCollect();
		    }
            if (!ports->findSame(p)) {
                ports->add(p);
            } 
        }
        if (p->iam()->isa(VWindowCls)){
	    	if (controls == 0) {
	    	    controls = new VOrdCollect();
	    	}
            if (!controls->findSame(p)) {
                controls->add(p);
            }
        }
    }
    else {
		if (ports != 0) {
	        if (p->iam()->isa(VPortCls)){
	            ports->removeSame(p);
	        } 
		}
		if (controls != 0) {
	        if (p->iam()->isa(VWindowCls)){
	            controls->removeSame(p);
	        }
		}
    }
}

// ---------------------------------------------------------------------------
//  Return the GUI system handle to this VFont object.
//
HFONT VFont::getHndl()
{
    if (hFont != 0) {
        return hFont;
    }

    //  Create a device context compatible with the system screen

    HDC hDC = CreateCompatibleDC(0);    

    //  Scale the physical point size of the font by the number of pixels 
    //  per logical vertical inch.
    //
    //  The number of pixels per physical inch on the video 
    //  display may insufficient to legibly render small fonts.  In addition,
    //  the viewing distance from the display screen is generally greater than 
    //  from the printed page.  Logical inches allow Windows to compensate 
    //  for this.  On the standard CGA, EGA, and CGA displays the logical inch
    //  is greater (has more pixels) than the physical inch.  In effect, 
    //  this magnifies the display so small fonts are plotted larger and are
    //  therefore legible.
    //
    //  We set the height argument to make() negative so that Windows 
    //  interprets this as the height of a glyph, not of a character cell.
    //
    make(-MulDiv(ptSize, GetDeviceCaps(hDC, LOGPIXELSY), 72));  

    DeleteDC(hDC);
    return hFont;
}

// ---------------------------------------------------------------------------
//  Create a new Windows font object.
//
//  Called for non-stock fonts when one of our attributes changes and we are 
//  bound to one or more ports.  Also called whenever the GUI handle has 
//  been requested and a underlying Windows font does not exist.
//
boolean VFont::make(int height)
{
    if (hFont && !stock) {
        DeleteObject((HGDIOBJ) hFont);
    }

    lf.lfHeight     = height;
    lf.lfWidth      = 0;
    copyFace(lf.lfFaceName);   //lf.lfPitchAndFamily may need to be corrected
    hFont           = CreateFontIndirect(&lf);
    stock           = FALSE;
    actualLog       = FALSE;

    updateControls();
    return TRUE;
}

// ---------------------------------------------------------------------------
//  Make a new font and notify the ports to which we are bound.
//  Called when a logical attribute changes.
//
boolean VFont::make()
{
    // If there are no ports or controls using this font we need
    // a height calculation.

    if (!((ports  && ports->count() ) || (controls && controls->count())) ){
        if (!stock) {
            if (hFont != 0) {
                DeleteObject((HGDIOBJ) hFont);
                hFont = 0;
            } 
            return FALSE;   
        }

        int h = lf.lfHeight;
        if (ptSize != 0) {
            //  point size overrides height spec, scale to logical coordinates
            HDC hDC = CreateCompatibleDC(0);    
    int xyzzy = GetDeviceCaps(hDC, LOGPIXELSY);
            h       = -MulDiv(ptSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);  
            DeleteDC(hDC);
        }

        boolean rv = make(h);
        //  create a non-stock font
        return rv;
    }


    if (ports){
        //  Reset this font from the ports to which we are bound.
        //  This has the effect of de-selecting this font from the 
        //  device contexts of those ports.

        VObject *p;
        VIterator i(ports);
        while (p = i++) {
            p->perform(pReset); //  (effectively, VPort::resetFont())
        }

        //  Delete the Windows font object.

        if (hFont && !stock) {
            DeleteObject((HGDIOBJ) hFont);
        }
        hFont       = 0;
        stock       = FALSE;
        actualLog   = FALSE;

        //  Select this font into the device contexts of the ports to
        //  which we are bound.  When selected, the Windows font object
        //  is rebuilt (see VFont::select()).

        i.start();
        while (p = i++) {
            p->perform(pSet, (long) this);  // (effectively, VPort::useFont())
        }
    }
    updateControls();

    return TRUE;
}

// ---------------------------------------------------------------------------
//
void VFont::getActual()
{
    HDC hDC = CreateDC("DISPLAY", 0, 0, 0);
    select(hDC, 0);
    DeleteDC(hDC);
}

// ---------------------------------------------------------------------------
//  Select the font into the given device context.  This maps a logical font
//  to a physical font.
//
//  The logical specification for a non-stock font is rebuilt according to 
//  the current point size request before mapping.  Stock fonts are not 
//  rebuilt since the Windows 3.1 font mapper does not scale these into 
//  logical space.  After mapping, we update logical font from the 
//  the physical font and recalculated the point size for stock fonts.
//
HFONT VFont::select(HDC context, VObject *client)
{
    HFONT       hOldFont;                                  
    TEXTMETRIC  tm;
    boolean     scaled = FALSE;


    //  Rebuild non-stock fonts according to the requested point size

    if (!stock && ptSize != 0 && !hFont) {

        //  Scale the physical point size of the font by the 
        //  number of pixels per logical vertical inch.
        //  
        //  The number of pixels per physical inch on the video 
        //  display may insufficient to legibly render small fonts.  
        //  In addition, the viewing distance from the display 
        //  screen is generally greater than from the printed page.
        //  Logical inches allow Windows to compensate for this.  
        //  On the standard CGA, EGA, and CGA displays the logical 
        //  inch is greater (has more pixels) than the physical 
        //  inch.  In effect, this magnifies the display so small 
        //  fonts are plotted larger and are therefore legible.
        //  
        //  We set the height argument to make() negative so 
        //  that Windows interprets this as the height of a 
        //  glyph, not of a character cell.

        int logPixHeight = -MulDiv(ptSize, 
                                    GetDeviceCaps(context, LOGPIXELSY), 72);

        if (client) {   //  scale through client viewing transformation
            int  x = 0;
            scaled = client->perform(scale, (long) &x, (long) &logPixHeight); 
        }

        make(logPixHeight);
    }

    //  Map the logical font into the device context
    //  and query the metrics of the physical font to which it maps

    hOldFont = (HFONT) SelectObject(context, (HGDIOBJ) hFont);
    GetTextMetrics(context, (TEXTMETRIC FAR *)(&tm));           

    _height             = tm.tmHeight;
    lf.lfHeight         = -(tm.tmHeight - tm.tmInternalLeading);
    lf.lfWidth          = tm.tmAveCharWidth;
    lf.lfWeight         = tm.tmWeight;
    lf.lfItalic         = tm.tmItalic;
    lf.lfUnderline      = tm.tmUnderlined;
    lf.lfStrikeOut      = tm.tmStruckOut;
    lf.lfCharSet        = tm.tmCharSet;
    lf.lfPitchAndFamily = tm.tmPitchAndFamily;

    GetTextFace(context, LF_FACESIZE - 1, (char *) lf.lfFaceName);

    //  pat--under Win16, we are casting an int to a short
    //  under Win32, we are casting a long to a short!!!
    mWidth = (short) tm.tmMaxCharWidth; 

    //  Initialize the point size 

    if (stock || ptSize == 0) {
        if (scaled) {
            ptSize = -lf.lfHeight / 20;
        }
        else {
            ptSize = -MulDiv(lf.lfHeight, 72, 
                            GetDeviceCaps(context, LOGPIXELSY));
        }
    }
    actualLog = TRUE;

    // Now update the fontinfo
    fontinfo->face(lf.lfFaceName);
    fontinfo->pointSize(ptSize);
    fontinfo->bold((lf.lfWeight == FW_NORMAL) ? 0 : 1);

    fontinfo->italic(lf.lfItalic);
    fontinfo->underline(lf.lfUnderline);
    fontinfo->strikeOut(lf.lfStrikeOut);
    fontinfo->stockObject(stock);

    return hOldFont; 
}
// ---------------------------------------------------------------------------

// updateControls is called when a font is used by a control (via a 
// call to VWindow::setFont, and then the font is re-made'.  This entails 
// deleteing the physical font handle, and then re-creating it, possibly
// with different characteristics.  If this happens, and the VFont had 
// been bound (via a call to VFont""bind in setControl) then the control 
// needs to have its font handle reselected. Otherwise the control will
// not have its text painted correctly because the fond that it used
// had been destroyed behind its back.

void VFont::updateControls(){
    if (controls){
        VObject *q;
        VIterator j(controls);
        while (q = j++) {
            ((VWindow*) q)->setFont(this);
        }
    }
}

 VFont *  VFont::getNativeWindowFont(VWindow * W){
// This routine creates a VFont from the information in the DC
// of a window.  It's to be used if you want a VFont object 
// derived from the native font that exists in the device context
// of the window.

    LOGFONT lf;
    memset(&lf,0, sizeof(lf));
    HGDIOBJ F=0,Orig = 0;
    HDC context; 
    if (W && W->isA(VControlCls)){
        F = (HGDIOBJ) SendMessage   (W->hwnd(),WM_GETFONT, 0, 0); 
		if (F != 0) {
	        context = GetDC(W->hwnd());
	        Orig = SelectObject(context, F);
		}
    } else {
        if (W){
            context = GetDC(W->hwnd());
        } else {
            context = GetDC(0);
        }
    }
    
       
    TEXTMETRIC  tm;
    GetTextFace(context, LF_FACESIZE - 1, (char *) lf.lfFaceName);
    GetTextMetrics(context, &tm);
    int _height             = tm.tmHeight;
    int pointsize    = -MulDiv( -(tm.tmHeight - tm.tmInternalLeading) ,
                            72, GetDeviceCaps(context, LOGPIXELSY));

    lf.lfWidth          = tm.tmAveCharWidth;
    lf.lfWeight         = tm.tmWeight;
    lf.lfItalic         = tm.tmItalic;
    lf.lfUnderline      = tm.tmUnderlined;
    lf.lfStrikeOut      = tm.tmStruckOut;
    lf.lfCharSet        = tm.tmCharSet;
    lf.lfPitchAndFamily = tm.tmPitchAndFamily;
      
    if (W){
        if ((Orig != 0) && W->isA(VControlCls)){
            SelectObject(context, Orig);   // put orig back
            }    
            ReleaseDC(W->hwnd(), context);
    }else{  
            ReleaseDC(0, context);
    }

    VFont* newOne = new VFont("", pointsize);
    newOne->logfont(lf);
    return newOne;
}

void VFont::clean()
{
    if (hFont && !stock) {
        DeleteObject((HGDIOBJ) hFont);
    }

    if (ports != 0) {
        if (ports->count()) {
            VObject *p;
            VIterator i(ports);
            i.end();
            while (p = i--) {
                p->perform(pSet, (long)0);
            }
            
        }
        delete ports;
        ports = 0;
    }
    if (controls != 0) {
        if (controls->count()) {
            VObject *p;
            VIterator i(controls);
            i.end();
            while (p = i--) {
                bind(p, FALSE); // take this guy out of the collection
            }
        }
        delete controls;
        controls = 0;
    }
    delete fontinfo;
    fontinfo=0;
}

// --------------------------------------------------------------------------
// systemFonts[bold][italics][underline][strikeout]
//
static VFont **systemFonts[2][2][2][2] = { 0 };

// --------------------------------------------------------------------------
// Return a reference to the specified system font.
//
VFont& VFont::system(int code)
{
	int b = code & Bold      ? 1 : 0;
	int	i = code & Italics   ? 1 : 0;
	int	u = code & Underline ? 1 : 0;
	int	s = code & Strikeout ? 1 : 0;

	code &= ~(Bold | Italics | Underline | Strikeout);

	if ((code < 0) || (code >= VFont::SystemFontCount)) {
		code = 0;
	}

	if (systemFonts[b][i][u][s] == 0) {
		systemFonts[b][i][u][s] = new VFont *[VFont::SystemFontCount];
		for (int n = 0 ; n < VFont::SystemFontCount ; n++) {
			systemFonts[b][i][u][s][n] = 0;
		}
	}
	else if (systemFonts[b][i][u][s][code] != 0) {
		return *systemFonts[b][i][u][s][code];
	}

	if (((b != 0) || (i != 0) || (u != 0) || (s != 0)) &&
		(systemFonts[0][0][0][0][code] != 0)) {
		//
		// ToDo!!!
		//
		systemFonts[b][i][u][s][code] = newSystemFont(code, b, i, u, s);
	}
	else {
		systemFonts[b][i][u][s][code] = newSystemFont(code, b, i, u, s);
	}

	return *systemFonts[b][i][u][s][code];
}

// --------------------------------------------------------------------------
// Set the specified system font to the given font.  The given font should
// *not* be deleted by the caller, in fact it should not even be referenced
// again through the given pointer as it may be deleted internally (after
// copying its value).  Instead VFont::system(int) should be used to obtain
// a reference to the font.
//
void VFont::system(int code, VFont *font)
{
	int b = code & Bold      ? 1 : 0;
	int	i = code & Italics   ? 1 : 0;
	int	u = code & Underline ? 1 : 0;
	int	s = code & Strikeout ? 1 : 0;

	code &= ~(Bold | Italics | Underline | Strikeout);

	if ((code < 0) || (code >= VFont::SystemFontCount)) {
		code = 0;
	}

	if (systemFonts[b][i][u][s] == 0) {
		systemFonts[b][i][u][s] = new VFont *[VFont::SystemFontCount];
		for (int n = 0 ; n < VFont::SystemFontCount ; n++) {
			systemFonts[b][i][u][s][n] = 0;
		}
		systemFonts[b][i][u][s][code] = font;
	}
	else if ((systemFonts[b][i][u][s][code] != font) &&
			 (systemFonts[b][i][u][s][code] != 0)) {
		systemFonts[b][i][u][s][code]->replaceFont(font);
	}
}

// --------------------------------------------------------------------------
//
VFont *VFont::newSystemFont(int code, int b, int i, int u, int s)
{
	VFont *font = 0;

	static int windows95 = LOBYTE(LOWORD(GetVersion())) >= 4;

	if (windows95) {

#		if defined(CV_WIN32) && defined(SPI_GETNONCLIENTMETRICS)
			NONCLIENTMETRICS ncm; ncm.cbSize = sizeof(ncm);
#		endif

		switch (code) {

		case Default:
		case Control:
		{
#			if defined(DEFAULT_GUI_FONT)
				font = new VFont(DEFAULT_GUI_FONT, 0);
#			endif
			break;
		}
		case Dialog:
		{
#			if defined(CV_WIN32) && defined(SPI_GETNONCLIENTMETRICS)
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
				font = new VFont(ncm.lfMessageFont);
#			endif
			break;
		}
		case Icon:
		{
#			if defined(CV_WIN32) && defined(SPI_GETNONCLIENTMETRICS)
				LOGFONT logfont;
				SystemParametersInfo(SPI_GETICONTITLELOGFONT,
									 sizeof(LOGFONT), &logfont, 0);
				font = new VFont(logfont);
#			endif
			break;
		}
		case Info:
		{
#			if defined(CV_WIN32) && defined(SPI_GETNONCLIENTMETRICS)
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
				font = new VFont(ncm.lfStatusFont);
#			endif
			break;
		}
		case Menu:
		{
#			if defined(CV_WIN32) && defined(SPI_GETNONCLIENTMETRICS)
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
				font = new VFont(ncm.lfMenuFont);
#			endif
			break;
		}
		case Caption:
		{
#			if defined(CV_WIN32) && defined(SPI_GETNONCLIENTMETRICS)
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
				font = new VFont(ncm.lfCaptionFont);
#			endif
			break;
		}
		case SmallCaption:
		{
#			if defined(CV_WIN32) && defined(SPI_GETNONCLIENTMETRICS)
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
				font = new VFont(ncm.lfSmCaptionFont);
#			endif
			break;
		}}
	}

	if (font == 0) {
		font = new VFont;
	}

	if (b) { font->bold(TRUE); }
	if (i) { font->italic(TRUE); }
	if (u) { font->underline(TRUE); }
	if (s) { font->strikeOut(TRUE); }

	font->isSystem = 1;

	return font;
}

// --------------------------------------------------------------------------
//
void VFont::deleteSystemFonts()
{
	int deleting_system_fonts = 0;

	if (deleting_system_fonts) {
		return;
	}

	deleting_system_fonts = 1;

	for (int b = 0 ; b < 2 ; b++) {
		for (int i = 0 ; i < 2 ; i++) {
			for (int u = 0 ; u < 2 ; u++) {
				for (int s = 0 ; s < 2 ; s++) {
					if (systemFonts[b][i][u][s] != 0) {
						for (int n = 0 ; n < VFont::SystemFontCount ; n++) {
							if (systemFonts[b][i][u][s][n] != 0) {
								delete systemFonts[b][i][u][s][n];
							}
						}
						delete [] systemFonts[b][i][u][s];
					}
				}
			}
		}
	}

	deleting_system_fonts = 0;
}

// --------------------------------------------------------------------------
// Replace the given value of this VFont with the value of the given
// new VFont and then delete the new VFont.  The point of this is to
// be able to update the value of a (system) VFont object in place, i.e.
// without so folks who've saved away pointers to the (system) VFont will
// still be valid.  This would be straight-forward an assignment-operators.
//
void VFont::replaceFont(VFont *new_font)
{
	if ((hFont != 0) && !stock) {
		DeleteObject(HGDIOBJ(hFont));
	}
    delete fontinfo;

	ptSize		= new_font->ptSize;
	mWidth		= new_font->mWidth;
	stock		= new_font->stock;
	isSystem	= new_font->isSystem;
	hFont		= new_font->hFont;
	lf			= new_font->lf;
	actualLog	= new_font->actualLog;
	_height		= new_font->_height;
	fontinfo	= new_font->fontinfo;

	new_font->hFont		= 0;
	new_font->fontinfo	= 0;
	new_font->isSystem	= 0;
	delete new_font;
}

// --------------------------------------------------------------------------
//
void VFont::systemFontDeleted(VFont *font)
{
	for (int b = 0 ; b < 2 ; b++) {
		for (int i = 0 ; i < 2 ; i++) {
			for (int u = 0 ; u < 2 ; u++) {
				for (int s = 0 ; s < 2 ; s++) {
					if (systemFonts[b][i][u][s] != 0) {
						for (int n = 0 ; n < VFont::SystemFontCount ; n++) {
							if (systemFonts[b][i][u][s][n] == font) {
								systemFonts[b][i][u][s][n] = 0;
								return;
							}
						}
					}
				}
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VFont::systemFontsChanged()
{
	int updating_system_fonts = 0;

	if (updating_system_fonts) {
		return;
	}

	updating_system_fonts = 1;

	for (int b = 0 ; b < 2 ; b++) {
		for (int i = 0 ; i < 2 ; i++) {
			for (int u = 0 ; u < 2 ; u++) {
				for (int s = 0 ; s < 2 ; s++) {
					if (systemFonts[b][i][u][s] != 0) {
						for (int n = 0 ; n < VFont::SystemFontCount ; n++) {
							if (systemFonts[b][i][u][s][n] != 0) {
								systemFonts[b][i][u][s][n]->
									replaceFont(newSystemFont(n, b, i, u, s));
							}
						}
					}
				}
			}
		}
	}

	updating_system_fonts = 0;
}

// --------------------------------------------------------------------------
//
struct VDummy { ~VDummy() { VFont::deleteSystemFonts(); } };
static VDummy CV_NEARDATA dummy;
