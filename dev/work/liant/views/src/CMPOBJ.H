/*
$Header:   Q:/views/win/vcs/cmpobj.h_v   1.3   Jun 03 1997 09:51:40   scottt  $
*/
//  Compound Document Object
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
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
//	01/25/95 dss	Initial release.
//  12/08/95 dss    added VComponentObjectCls. added default ctor.
//  12/12/95 dss    added realize, inner VComponentObject.
//	01/09/96 dss	VComponent name change.
//
//	---------------------------------------------------------------------------

#ifndef VCOMPONENTOBJECT_H
#define VCOMPONENTOBJECT_H

// Views Compound Document Types
#include "oledefs.h"

// OLE Includes
#include "unknown.h"

// Views Compound Document Includes
#include "cmpstrg.h"
 
CLASS VComponentObject : public CUnknown
{
public:

	// construct a Default Unknown VObject
	VComponentObject();

	// construct an inner Unknown VObject
	VComponentObject(LPVOBJECT lpVObject, REFCUNKNOWN refUnkOuter);

	// construct an outer Unknown VObject
	VComponentObject(LPVOBJECT lpVObject);
	virtual ~VComponentObject();

	VClass	*iam();


	LPTSTR			getClassName() {return _T("VComponentObject");}

	// conversions
	operator LPVOBJECT()	{return getVThis();}

	// operations
	LPVOBJECT getVThis()	{return lpVObject;}
	LPVOBJECT getVObject()	{return lpVObject;}

	// Storage
	CONTAIN_INTERFACE2(VComponent, Storage)

public:
	virtual boolean			createStorage(VComponentObject *storageParent = 0,
										  VString *lpStorageName = 0);

	virtual void			realize(LPCUNKNOWN lpCUnkOuter);
	
protected:
	LPVOBJECT		lpVObject;

	static long		storageCreationCount;

private:
	void			init(LPVOBJECT lpVObject);
};

extern VClass	*VComponentObjectCls;

#endif // VCOMPONENTOBJECT_H
