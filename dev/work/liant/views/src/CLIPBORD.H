// $Header:   Q:/views/win/vcs/clipbord.h_v   1.5   Mar 04 1997 16:32:02   gregm  $ 

//	clipbord.h
//
//	VClipBoard class interface
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
//	04/08/93 pmcm	preened, added destructor, replaced id with VObject *
//  08/09/93 jmd	removed free
//  10/06/93 dgm	changed get() to support GUI objects.
//----------------------------------------------------------------------------

#ifndef VCLIPBORD_H
#define VCLIPBORD_H

#include "object.h"

CLASS VStream;

//	The VClipBoard class provides access to the system clipboard. 
//	Data in text format can be put or retrieved from an instance of the
//	VClipBoard object by using the put() and get() methods with VString
//	objects.  Otherwise, any object that is archivable can be put on the
//	clipboard (with the put() method) or retrieved from the clipboard (with
//	the get() method).  VBitMap objects are stored and retrieved directly
//	from the clipboard in system dependent format and not through the VArchiver.  
CLASS VClipBoard : public VObject {

public:
				 VClipBoard();
				~VClipBoard();
	VClass		*iam();

	//	Clipboard operations

	boolean 	clear();
	boolean 	contains(VClass *cls);
	boolean 	isEmpty();
	VObject    *get(VObject *data = 0, boolean do_realize = TRUE);
	boolean 	put(VObject *);
};

extern VClipBoard clipboard;	//	the global clipboard

#endif // VCLIPBORD_H
