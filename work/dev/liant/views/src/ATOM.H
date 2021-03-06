// $Header:   Q:/views/win/vcs/atom.h_v   1.6   Mar 04 1997 16:31:50   gregm  $ 

//	atom.h
//
//	VAtom class interface
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
//	10/04/93 jmd	preened
//	07/18/95 dgm	Miscellaneous const char * changes.
// ----------------------------------------------------------------------------

#ifndef VATOM_H
#define VATOM_H

#define ATOMmaxLength 50

//
// The VAtom class is specifically designed to implement the VAtom
// name space facility of MS Windows and OS/2 PM. It is used by the VDde
// classes. An VAtom object has a name (char *) and a handle. 
//

CLASS VAtom {
public:
			VAtom(const char *);
			VAtom(unsigned int);
			~VAtom();

	operator char *();

	// Return the MS Windows or OS/2 PM handle associated with this VAtom.
	operator	unsigned() { return hAtom; }

	void		operator !();

protected:
	ushort	hAtom;	// The VAtom handle.
	char   *pAtom;	// The VAtom name string.
};

#endif // VATOM_H
