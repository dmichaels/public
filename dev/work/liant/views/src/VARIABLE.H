// $Header:   Q:/views/win/vcs/VARIABLE.H_v   1.11   Mar 04 1997 14:14:14   gregm  $

//	variable.h
//
//	VVariable class interface
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
//	01/26/96 dss	Name change: VComponentProgrammable.
//	02/06/96 dss	use quotes and not angle brackets.
//  05/10/96 rhw    Changed default constructor to act like an optional argument
//  06/07/96 rhw    Changed casting constructors so that they would except
//					constants as arguments (i.e added the const to the argument
//					type).
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Description of file:  The purpose of this class is to both encapsulate
// other type.  This allows code to be written in a very generalize fashion.
//
// The approach taken involves having the VVariable class determine the type
// (and address) of the variable during its construction.  The type
// determination is accomplished using C++ conversion constructors as
// discussed on page 271 of the ARM.
//
// This class is very useful as the type of a parameter in cases were the
// role of the parameter is more important than the type of the parameter.
// For example, the VComponentProgrammable::Invoke() method uses a VVariable as the
// type of the return value parameter.  This parameter is used by the caller
// to supply the variable that will hold the result of the invoke() method.
// In this case, its role as the "return value" is more important the its
// type.  Using VVariable as the type of that parameter allows the
// VComponentProgrammable class to generically receive all return variables through
// this parameter while still being able to specifically record the type and
// address of the user supplied return value.  This information can then be
// used to make decisions regarding the return value in the invoke() method.
//----------------------------------------------------------------------------

#ifndef VVARIABLE_H
#define VVARIABLE_H

#include "oledefs.h"

#include "object.h"

CLASS VMoney;
CLASS VDateTime;
CLASS VString;
CLASS VComponentProgrammable;
CLASS VStatus;
CLASS VCD;
CLASS VArray;
// Sanjay Start
CLASS CUnknown;
// Sanjay End

extern VClass *VVariableCls;

CLASS VVariable : public VObject
{
public:
	enum Type
	{
		k_error,
		k_void,
		k_short,
		k_long,
		k_float,
		k_double,
		k_VMoney,
		k_VDateTime,
		k_VString,
		k_VProgrammable,
		k_VStatus,
		k_boolean,
		k_VVariable,
		k_VCD,
		k_unsigned_char,
		k_VArray,
		// Sanjay Start
		k_CUnknown
		// Sanjay End
	};

	VVariable();
	
	// These constructors determine the type and address of a variable
	VVariable(const short&					variable);
	VVariable(const long&					variable);
	VVariable(const float&					variable);						
	VVariable(const double&					variable);						
	VVariable(const VMoney&					variable);
	VVariable(const VDateTime&				variable);						
	VVariable(const VString&				variable);
	VVariable(const VComponentProgrammable&	variable);
	VVariable(const VStatus&				variable);
	VVariable(const boolean &				variable);
	VVariable(const VVariable&				variable);
	VVariable(const VCD&					variable);
	VVariable(const unsigned char&			variable);
	VVariable(const VArray&					variable);
	// Sanjay Start
	VVariable(const CUnknown&				variable);
	// Sanjay End
		
	VVariable(short*						variable);
	VVariable(long*							variable);
	VVariable(float*						variable);						
	VVariable(double*						variable);						
	VVariable(VMoney*						variable);
	VVariable(VDateTime*					variable);						
	VVariable(VString*						variable);
	VVariable(VComponentProgrammable*		variable);
	VVariable(VStatus*						variable);
	VVariable(boolean*						variable);
	VVariable(VVariable*					variable);
	VVariable(VCD*							variable);
	VVariable(unsigned char*				variable);
	VVariable(VArray*						variable);
	// Sanjay Start
	VVariable(CUnknown*						variable);
	// Sanjay End

	~VVariable();

	VVariable& 			operator=(const VVariable& rhs);

	boolean				isReference();

	// Returns the address of the encapsulated variable
	void*				getAddress() const;

	// Views specific
	VClass*				iam();
	virtual boolean		copyFrom(VObject *pObject);
	// Since this class is all about encapsulating other types and making
	// them appear generic (i.e. as a VVariables), the class can't own the
	// values that it encapsulates.  Therefore, it does not make sense to
	// support archiving since it can't meaningfully restore something that
	// it does not own.

	// Sanjay Start Made them public)
	// COM-specific methods
	void				getVariantArg(VARIANTARG* pVArg);
	void				releaseVariant();
	void				operator=(VARIANTARG* pVArg);
	// Sanjay End
	// Sanjay Start
	VVariable::Type getType() { return m_type; }
	const void* getVariable() { return m_pVariable; }
	// Sanjay End

private:
	// Give these classes access to OLE specific routines, but no one else
	friend CLASS VParameters;
	friend CLASS VMethodData;

	// Non-COM provate methods
	void				initializeObjectSystem();
	void				uninitializeObjectSystem();

	const void*			m_pVariable;		// Holds the address of variable
	VVariable::Type		m_type;				// Indicates the type of variable
	boolean				m_isInitialized;	// Indicates initialization status
	boolean				m_isReference;		// Tells if its a pointer to m_type

	// COM-specific members
	VARIANTARG*			m_pVariantArg;		// Only used if m_type is k_VVariable
	DATE				m_date;				// Only used if m_type is k_VDateTime
	SCODE				m_scode;			// Only used if m_type is k_VStatus
	BSTR				m_pBSTR;			// Only used if m_type is k_VString
	LPDISPATCH			m_pIDispatch;		// Only used if m_type is k_VProgrammable
};

inline boolean VVariable::isReference()
{
	return m_isReference;
}
#endif // VVARIABLE_H
