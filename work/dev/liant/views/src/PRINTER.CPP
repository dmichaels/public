// $Header:   Q:/views/win/vcs/printer.cpv   1.9   Mar 04 1997 17:55:40   gregm  $ 

//	printer.cpp
//
//	VPinrter impelmentation
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
//      -----------------
//	08/09/93 jmd    removed free
//	12/22/95 glm	modified Escape for CV_WIN32, support for Win95
//	01/26/96 pkt	added 'const' to 'char*' parameters.
//	05/08/96 glm	removed Escape, newPage to use EndPage/StartPage
//					and ungetHndl to use EndPage
//	06/13/96 glm	removed most error print dialogs
// ----------------------------------------------------------------------------

#include "printer.h"
#include "winclass.h"
#include "notifier.h"

extern "C" {
#       include "commdlg.h"
}

defineClass(VPrinter, VDisplay)

// ----------------------------------------------------------------------------
// Construct a VPrinter object and bind it to the default 
// printer device.
//
VPrinter::VPrinter() : hDevMode(0)
{
	docName.puts("CPPVdoc");
	init();
}

VPrinter::~VPrinter() 
{ 
	clean(); 
}

// ----------------------------------------------------------------------------
// Abort the printing of the current document.
//
void VPrinter::abort()
{
	if (hDC) {
		AbortDoc(hDC);
	}
}

// ----------------------------------------------------------------------------
// Delete the device context for this VPrinter.
//
void VPrinter::clean()
{
	if (hDevMode) {
		GlobalFree(hDevMode);
		hDevMode = 0;
	}
	if (hDC) {
		DeleteDC(hDC);
		hDC = 0;
	}
}

// ----------------------------------------------------------------------------
// Return the width and height, in pixels, of a page on this VPrinter
// into '*w' and '*h' respectively.  Return zero width and zero height if 
// there is no printer device bound to this VPrinter object.
//
void VPrinter::sizeOfImage(int *w, int *h)
{
	if (hDC) {
		*w = GetDeviceCaps(hDC, HORZRES);
		*h = GetDeviceCaps(hDC, VERTRES);
	}
	else  {
		*w = *h = 0;
	}
}

// ----------------------------------------------------------------------------
// Return the device handle for this VPrinter and issue the
// start commands to the printing device. Return NIL if there is not a
// valid device handle.
//
HDC VPrinter::getHndl()
{
	HDC h = 0;

	if (hDC) {
		if (refCount++) {
			h = hDC;
		}
		else {
			DOCINFO di;
			memset(&di, 0, sizeof(DOCINFO));
			di.cbSize = sizeof(DOCINFO);
			di.lpszDocName = (char *) docName.gets();
			di.lpszOutput = 0;
			printerror = StartDoc(hDC,&di);
			if (printerror){
				h = hDC;
				printerror = StartPage(hDC);
			} else {
				refCount = 0;
			}
		}
	}
	return(h);
}   

// ----------------------------------------------------------------------------
// Advance this VPrinter to the next page.
//
void VPrinter::newPage()
{
	if (hDC) {
		printerror = SaveDC(hDC);
		printerror = EndPage(hDC);
	    printerror = StartPage(hDC);
	    printerror = RestoreDC(hDC, -1);
	}
}
// ----------------------------------------------------------------------------
// Release the device context handle for this VPrinter and send end
// document commands to the printer.  Note: this method does NOT perform a
// newPage() upon release of the device handle.
//
void VPrinter::ungetHndl()
{
	if (hDC && !-- refCount) {
		EndPage(hDC);
	    EndDoc(hDC);
	}
} 

// ----------------------------------------------------------------------------
// Construct a VPrinter object with document name 'dName'and bind it
// to the default printer device.  If 'defer' is TRUE (defualt set to FALSE in
// printer.h), then defer the binding of this object to the printer device.
//
VPrinter::VPrinter(const char *dName, boolean defer) : hDevMode(0)
{
	if (dName) {
		docName.puts(dName);
	}
	else {
		docName.puts("CPPVdoc");
	}
	if (!defer) {
		init();
	}
}

// ----------------------------------------------------------------------------
// Bind this VPrinter object to the default installed printer 
// device.  Return TRUE if successful, otherwise return FALSE.
//
boolean VPrinter::init()
{
	char  pPrintInfo[80];
	LPSTR lpTemp;
	LPSTR lpPrintType;
	LPSTR lpPrintDriver;
	LPSTR lpPrintPort;

	if (!GetProfileString("windows", "Device", (LPSTR)"", pPrintInfo, 80)) {
		return(FALSE);
	}
	lpTemp = lpPrintType = pPrintInfo;
	lpPrintDriver = lpPrintPort = 0;
	while (*lpTemp) {
		if (*lpTemp == ',') {
			*lpTemp++ = 0;
			while (*lpTemp == ' ') {
				lpTemp = AnsiNext(lpTemp);
			}
			if (!lpPrintDriver) {
				lpPrintDriver = lpTemp;
			}
			else {
				lpPrintPort = lpTemp;
				break;
			}
		}
		else {
			lpTemp = AnsiNext(lpTemp);
		}
	}

#if defined(CV_WIN32)
	if (!(hDC = CreateDC(lpPrintDriver, lpPrintType, lpPrintPort, 
		(CONST DEVMODE *) 0)))
#else
	if (!(hDC = CreateDC(lpPrintDriver, lpPrintType, lpPrintPort, 
		(const void FAR*) 0))) 
#endif
	{
		print("Unable to open printer on doc %s. ", (char*)docName.gets());
		return(FALSE);
	}
    SetAbortProc(hDC,0);
	return(TRUE);
}

// ----------------------------------------------------------------------------
// Set the document name of this VPrinter object to 'DocName'.
//
void VPrinter::documentName(const char *DocName)
{
	docName.puts(DocName);
}

// ----------------------------------------------------------------------------
// Set the MS VWindow's device context and device mode handles for 
// this VPrinter.  This method is for MS Windows only.
//
void VPrinter::setDevice(HDC hdc, HANDLE devmode)
{
	if (hDC != hdc) {
		if (hDC) {
			DeleteDC(hDC);
		}
		hDC = hdc;
	}
	hDevMode = devmode;
}
