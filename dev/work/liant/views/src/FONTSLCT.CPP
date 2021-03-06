// $Header:   Q:/views/win/vcs/fontslct.cpv   1.8   Mar 04 1997 17:55:08   gregm  $ 

//  fontslct.cpp
//
//  VFontSelect implementation
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
//  05/28/93 jmd    preened
//  07/10/93 jmd    added new members and features
//  07/20/93 jmd    changed rgbColor to VColor, 0 to "" for Font constructor
//  08/09/93 jmd    removed free
//  09/01/93 dgm    M8 changes.
//  03/25/94 pat    changed HANDLE to HINSTANCE in CHOOSEFONT initialization.
//  10/11/94 dss    Added operator(). Constructor calls operator().
// ----------------------------------------------------------------------------

#include "fontslct.h"
#include "font.h"
#include "printer.h"
#include "notifier.h"

extern "C" {
#   include "commdlg.h"
}

defineClass(VFontSelect, VDialog)

// ---------------------------------------------------------------------------
VFontSelect::VFontSelect()
{
    ;
}

// ---------------------------------------------------------------------------
VFontSelect::~VFontSelect()
{
    ;
}

// ---------------------------------------------------------------------------
// Construct a VFont Select dialog.
// font points to a pointer to a VFont object.
// if *font is not 0, display that font as the initial selection.
// pWin is the parent window for the dialog
// if dstyle == PrinterFonts show printer fonts as well as screen fonts
// using the specified printer is provided (print).
// upon return success will be TRUE if the user selected OK
// and *font will now point to the selected font
// color will be filled in if dstyle Effects is set.
// 
VFontSelect::VFontSelect(VFont **font,
                            VWindow *pWin,
                            boolean& success,
                            VColor& color,
                            VPrinter *print,
                            DlgStyle dstyle)
{
    success = (*this)(font, pWin, color, print, dstyle);
}

// ---------------------------------------------------------------------------
// Construct a VFont Select dialog.  If 'all' is TRUE, then show
// both printer and screen fonts in the dialog.  If 'all' is FALSE, then
// show only screen fonts in the dialog.  If `VPrinter` object 'print' is
// given, then the fonts for that printer will be shown in the dialog. 
// Otherwise, if 'all' is TRUE and 'print' is NIL, then the printer fonts
// for the default installed printer will be shown along with the screen fonts.
// If 'font' points to a NIL `VFont` object, then a new `VFont` object will be
// constructed with the user specified font characteristics.  Otherwise, the
// `VFont` object pointed to by 'font' will be given the user selected font
// specifications.  If the user cancels this dialog, then return a NIL pointer
// in *'font'. Construct this dialog as a child window of 'pWin'.
// THIS IS FOR V2.x COMPATABILITY
//
VFontSelect::VFontSelect(boolean all, VFont **font, VPrinter *print, VWindow *pWin)
{
    DlgStyle dstyle = (all) ? DlgStyle(PrinterFonts | ScreenFonts | Effects) :
                              DlgStyle(ScreenFonts | Effects);

    VColor color(BLACK);

    boolean success = (*this)(font, pWin, color, print, dstyle);

    if (!success) {
        *font = 0;
    }
}

// ---------------------------------------------------------------------------
// Where it actually happens...
// 
boolean VFontSelect::operator()(VFont **font,
                            VWindow *pWin,
                            VColor& color,
                            VPrinter *print,
                            DlgStyle dstyle)
{
    boolean success = TRUE;

    CHOOSEFONT  chf;
    LOGFONT     logfnt;
    VPrinter   *prnt = 0;

    if (print != 0) {
        prnt = print;
    }
    else if (dstyle & PrinterFonts)  {
        prnt = new VPrinter();
    }

    chf.lStructSize = sizeof(CHOOSEFONT);
    chf.hwndOwner   = ((pWin == 0) ? 0 : pWin->hwnd());
    chf.Flags       = (dstyle & Effects) ? CF_EFFECTS : 0;

    boolean okPrinter = (prnt != 0);
    if (okPrinter) {
      chf.hDC = prnt->getDC();
      if (chf.hDC)
        okPrinter = TRUE;
      else
        okPrinter = FALSE;
    }

    if (okPrinter) {
        chf.Flags |= CF_PRINTERFONTS;
    }
    else if (dstyle & ScreenFonts) {
        chf.Flags |= CF_SCREENFONTS;
    }

    if (dstyle & PrinterFonts && chf.hDC && okPrinter) {
        chf.Flags |= CF_BOTH;
    }

    // set up default font info
    if (*font != 0) {
        chf.Flags |= CF_INITTOLOGFONTSTRUCT;
        if (!(*font)->actualLog) {
            (*font)->getActual();
        }

        logfnt = (*font)->lf;
    }

    chf.lpLogFont       = &logfnt;
    chf.rgbColors       = color;        //  RGB(0, 0, 255);
    chf.lCustData       = 0L;
    chf.lpTemplateName  = (LPCSTR) 0;
    chf.hInstance       = (HINSTANCE) 0;
    chf.lpszStyle       = (LPSTR) 0;
    chf.nFontType       = SCREEN_FONTTYPE;
    chf.nSizeMin        = 0;
    chf.nSizeMax        = 0;

    nativeDialog = TRUE;
    notifier->startModal( this);

    if (ChooseFont(&chf)) {
        success = TRUE;
        if (*font == 0) {
            *font = new VFont("", 10);
        }
        (*font)->logfont(logfnt);
        (*font)->pointSize(chf.iPointSize / 10);

        color = chf.rgbColors;
    }
    else {
        success = FALSE;
    }

    if (prnt != 0 && print == 0) {
        delete prnt;
    }

    // don't hide because the dialog was destroyed when OK or Cancel
    // was hit.
    notifier->endModal(FALSE);

    return success;
}

// ---------------------------------------------------------------------------
// Perform an VFont Select style dialog within window 'pWin'. If 'all'
// is TRUE, then display both screen and printer fonts in the dialog.  The
// printer fonts will be those available for the default installed printer.  
// Return a 'VFont' object constructed with the user selected font
// specifications.  If the "Cancel" button is selected, then return 0.
//
VFont *VFontSelect::dialog(boolean all, VWindow *pWin)
{
    VFont *fnt = 0;
    boolean success;
    VColor color(BLACK);
    DlgStyle dstyle = (all) ? DlgStyle(PrinterFonts | ScreenFonts) :
                              DlgStyle(ScreenFonts);

    VFontSelect dlg(&fnt, pWin, success, color, 0, dstyle);

    return success ? fnt : 0;
}

// ---------------------------------------------------------------------------
// Perform an VFont Select style dialog within window 'pWin'. If 'all'
// is TRUE, then display both screen and printer fonts in the dialog.  Otherwise, 
// if 'all' is FALSE, then show only the printer fonts for the printer specified
// with the `VPrinter` object 'printer'.  Return a 'VFont' object constructed with
// the user selected font specifications.  If the "Cancel" button is selected, 
// then return NIL.
//
VFont *VFontSelect::dialog(boolean all, VPrinter *printer, VWindow *pWin)
{
    VFont *fnt = 0;
    boolean success;
    VColor color(BLACK);
    DlgStyle dstyle = (all) ? DlgStyle(PrinterFonts | ScreenFonts) :
                              DlgStyle(PrinterFonts);

    VFontSelect dlg(&fnt, pWin, success, color, printer, dstyle);

    return success ? fnt : 0;
}

// ---------------------------------------------------------------------------
// Perform an VFont Select style dialog within window 'pWin'. If 'all'
// is TRUE, then display both screen and printer fonts in the dialog.  Otherwise, 
// if 'all' is FALSE, then show only the screen fonts.
// font is a pointer to a pointer to a VFont.
// if (*font) is non-zero upon entry use it for the initial dialog settings
// Upon return (*font) points to a new font constructed with
// the user selected font specifications.
// Returns FALSE if the user selects the "Cancel" button.
//
boolean VFontSelect::dialog(VFont **font, boolean all, VWindow *pWin)
{
    boolean success;
    VColor color(BLACK);
    DlgStyle dstyle = (all) ? DlgStyle(PrinterFonts | ScreenFonts) :
                              DlgStyle(ScreenFonts);

    VFontSelect dlg(font, pWin, success, color, 0, dstyle);

    return success;
}

// ---------------------------------------------------------------------------
