// $Header:   Q:/views/common/vcs/CLASS.CPV   1.10   Mar 24 1997 16:08:22   scottt  $ 

//	class.cpp
//
//	VClass implementation [Common]
//
//	Allegris Foundation 1.1.00
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	04/28/93 pmcm 	Preened
//	07/27/93 pmcm	Added ghost callback facility
//	07/28/93 dgm	Applied a couple pmcm fixes for the ghost lists.
//	07/30/93 pkt	Eliminated #ifdef macintosh.
//	08/09/93 jmd	Dealt with case of callback already in ghost list
//	08/25/93 pmcm	Callback facility is now through VCallbackList
//	09/29/93 pkt	Eliminated dead code supporting memory pool.
//	10/18/93 dgm	UNIX changes.
//	11/09/93 pkt	Added #include string.h.
//	01/13/94 dgm	merged and preened
//	01/22/94 pkt	added alwaysFalse.
//	02/08/94 pmcm	removed notifier.h
//	06/07/94 dgm	Changed #include "strings.h" to <string.h>.
//	07/14/94 jld	removed includes for string.h
//	08/27/94 pkt	updated for Metrowerks C++.
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	10/24/94 pkt	removed 'alwaysFalse'.
//	04/11/95 pkt	made 'construct' and 'of' char* parameters const.
//	04/12/95 pkt	merged Mac with NT.
//	05/19/95 pkt	merged for common source.
//	05/26/95 pkt	combined international changes into common version.
//	09/06/95 pkt	removed OFI code.
//	01/19/97 dgm	Added getBase().
// ---------------------------------------------------------------------------

#include "object.h"
#include "class.h"
#include "classtbl.h"

extern "C" {
#	include <stdio.h>
}

// ---------------------------------------------------------------------------
//	Constructor used in importClass macro.  Looks up meta-class instance
//	of the class named by n in the global table of classes (classTable).
//	Initializes cp to point to the meta-class, if found.
//
VClass::VClass(VClass **cp, const char *n) 
{
	VClass *p = VClass::of(n);
	if (p != 0) {
		*cp = p;
	}
}

// ---------------------------------------------------------------------------
//	Constructor used in defineClass macro.
//	Upon construction this meta-class instance is entered into the global
//	table of classes, classTable.
//
VClass::VClass(VClass **cp, const char *s, idfunc con, int i, VClass **sp) 
{
	setClass(cp, s, con, i);
	super[0] = sp;
	super[1] = 0;
	super[2] = 0;
}

// ---------------------------------------------------------------------------
//	Constructor used in defineSubClass macro.
//	Upon construction this meta-class instance is entered into the global
//	table of classes, classTable.
//
VClass::VClass(VClass **cp, const char *s, idfunc con,
			   int i, VClass **s1, VClass **s2) 
{
	setClass(cp, s, con, i);
	super[0] = s1;
	super[1] = s2;
	super[2] = 0;
}

// ---------------------------------------------------------------------------
//	Constructor used in defineSubSubClass macro.
//	Upon construction this meta-class instance is entered into the global
//	table of classes, classTable.
//
VClass::VClass(VClass **cp, const char *s, idfunc con,
			   int i, VClass **s1, VClass **s2, VClass **s3) 
{
	setClass(cp, s, con, i);
	super[0] = s1;
	super[1] = s2;
	super[2] = s3;
}

// ---------------------------------------------------------------------------
//	Destroy the meta-class instance and remove it from the global table
//	of classes (classTable).
//
VClass::~VClass()
{
	(*classTable)->remove(this); // remove from app classTable
}

// ---------------------------------------------------------------------------
//	Return a newly constructed instance of the class named by name.
//
void *VClass::construct(const char *name) 
{
	VClass *c;
	if ((c = VClass::of(name)) != 0) {
		return c->construct();
	}
	return 0;
}

// ---------------------------------------------------------------------------
//	Return a pointer to an instance of the VClass object that 
//	represents the C++ class with name name.
//
VClass *VClass::of(const char *name) 
{
	return (*classTable)->of(name);
}

// ---------------------------------------------------------------------------
//	Enter this meta-class instance into the global table of classes, 
//	classTable.  Also, establish all instance variables of this meta-class
//	instance except for the super class referents.
//
void VClass::setClass(VClass **cp, const char *s, idfunc con, int i) 
{
	name = s;
	constructor = con;
	size = i;
	(*classTable)->add(this);

	if (cp != 0) {
		*cp = this;
	}
}

// ---------------------------------------------------------------------------
//	Return o construed as an object of class c.
//
// Note: Assumes canonical construction of derived classes.
//
void *VClass::asa(char * /* o */, VClass * /* c */, int /* reverse */)
{ 
#if 0
	if (c == this || c == super[0]) {
		return (void*)o;
	}
	if (super[0] && (o = super[0]->asa(o, c))) {
		return (void*)o;
	}
	if (super[1] && (o = super[1]->asa(o, c))) {
		return (void*)(o + super[0]->size);
	}
	if (super[2] && (o = super[2]->asa(o, c))) {
		return (void*)(o + super[0]->size + super[1]->size);
	}
	if (reverse) {
		return 0;
	}
	return c->asa(this, 1);
#endif
	return 0;
}

// ---------------------------------------------------------------------------
//	Return TRUE if the class this VClass represents is a base class
//	of class o.
//
boolean VClass::isa(VClass *o) 
{ 
	int i;
	VClass *s = this; 

	if (o == s) {
		return TRUE; 
	}

	for (i = 0; i < 3; i++) {
		if (s->super[i] != 0) {
			if ((*s->super[i])->isa(o)) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//	Return the number of bytes in the instance variables in the class
//	this VClass object represents (excluding any inherited instance 
//	variables).
//
int VClass::getprivate() 
{
	if (*super == 0) {
		return size;
	}
	return size - (**super)->size;
}

// ---------------------------------------------------------------------------
//
VClass *VClass::getBase(int i) const
{
	if ((i < 1) || (i > 3) || (super[i - 1] == 0)) {
		return 0;
	}
	else {
		return *super[i - 1];
	}
}
