// $Header:   Q:/views/win/vcs/ARRAY.H_v   1.7   Mar 04 1997 14:11:40   gregm  $

//	array.h
//
//	VArray class interface
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
//	01/23/96 rhw	Initial Revision.
//	02/06/96 dss	use quotes and not angle brackets.
// ----------------------------------------------------------------------------

#ifndef VARRAY_H
#define VARRAY_H

#include "object.h"

#include "variable.h"

extern VClass *VArrayCls;

CLASS VArray : public VObject
{
public:
	VArray();
	VArray(VARTYPE varType, SAFEARRAY* pSafeArray);

	~VArray();

	VArray& 		operator=(const VArray& rhs);

	VARTYPE			getVarType() { return m_varType; }
	SAFEARRAY*		getSafeArray() const { return m_pSafeArray; }

	// Views specific
	VClass*			iam();
	virtual boolean	copyFrom(VObject *pObject);
	void		operator=(VARIANTARG* pVArg);
	void		convertToSafeArray(VARTYPE* type, SAFEARRAY* pSafeArray);
private:
	// Give VVariable access to OLE specific routines, but no one else
	friend CLASS VVariable;
	VARTYPE		m_varType;
	SAFEARRAY*	m_pSafeArray;
};

#endif // VARRAY_H
