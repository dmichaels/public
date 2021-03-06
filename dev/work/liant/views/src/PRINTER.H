// $Header:   Q:/views/win/vcs/printer.h_v   1.9   Mar 04 1997 16:32:54   gregm  $ 

//	printer.h
//
//	VPrinter interface
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
//  08/09/93 jmd	removed free
//	03/05/94 pmcm	preened.
//	01/26/96 pkt	added 'const' to 'char*' parameters.
// ----------------------------------------------------------------------------

#ifndef VPRINTER_H
#define VPRINTER_H

#include "display.h"
#include "str.h"

CLASS VWindow;

extern VClass *VPrinterCls;

//	The VPrinter class supports text and graphics output to a
//	printer device that is connected to the computer running the
//	application.  The VPrinter class provides the basic methods to
//	instantiate itself on the standard default MS Windows or OS/2 
//	printer device. VPort objects may be used to support full 
//	graphic output to their printing device.  Information on the 
//	size and aspect ratio of a printing device can also be 
//	obtained through a VPort object that is opened on the 
//	respective VPrinter object.

CLASS VPrinter : public VDisplay {

public:
					VPrinter();
					VPrinter(const char*, boolean defer = FALSE);
					~VPrinter();

	VClass 		   *iam();

	VString&		documentName();
	void 			documentName(const char *s);

	void 			getDevice(HDC *hdc, HANDLE *devmode) const;
	void 			setDevice(HDC d, HANDLE h);

	void 			abort();
	void 			clean();
	void 			newPage();

	boolean 		isValid() const;

	HDC 			getDC() const;

	virtual HDC 	getHndl();
	virtual void  	ungetHndl();
	virtual void  	sizeOfImage(int *w, int *h);

	boolean 		init();

    int			getPrintError();

protected:

	VString 	docName;
	HANDLE 		hDevMode;

    int			printerror;

};

// ---------------------------------------------------------------------------
inline HDC VPrinter::getDC() const
{
	return hDC;
}
// ---------------------------------------------------------------------------
inline boolean VPrinter::isValid() const
{
	return (hDC != 0) ? TRUE: FALSE;
}
// ---------------------------------------------------------------------------
inline void VPrinter::getDevice(HDC *hdc, HANDLE *devmode) const
{
	*hdc 		= hDC;
	*devmode 	= hDevMode;
}
// ---------------------------------------------------------------------------
inline VString& VPrinter::documentName()
{
	return docName;
}
// ---------------------------------------------------------------------------
inline int VPrinter::getPrintError()
{
	return printerror;
}

#endif 	//	VPRINTER_H
