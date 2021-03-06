// $Header:   Q:/views/win/vcs/VARIABLE.CPv   1.13   Mar 04 1997 14:14:12   gregm  $

//	variable.cpp
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
//	01/26/96 dss	cmpcprg.h changed to cmpprog.h.
//					Name change: VComponentProgrammable.
//	02/06/96 dss	use quotes and not angle brackets.
//  05/10/96 rhw    Changed default constructor to act like an optional argument
//  06/07/96 rhw    Changed casting constructors so that they would except
//					constants as arguments (i.e added the const to the argument
//					type).
//
// ----------------------------------------------------------------------------

#include "defs.h"
#include "cvassert.h"
#include "datetime.h"
#include "str.h"

#include "variable.h"

#include "money.h"
#include "cmpprog.h"
#include "status.h"
#include "array.h"
#include "dosdttm.h"
// Sanjay Start
#include "unknown.h"
// Sanjay End

defineClass(VVariable, VObject)

#define V_VT_REFTYPE(X) (V_VT(X) & ~VT_BYREF)

VVariable::VVariable()
	: m_pVariable(0)
	, m_type(VVariable::k_error)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const short& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_short)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const long& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_long)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const float& variable)						
	: m_pVariable(&variable)
	, m_type(VVariable::k_float)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const double& variable)						
	: m_pVariable(&variable)
	, m_type(VVariable::k_double)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const VMoney& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_VMoney)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const VDateTime& variable)						
	: m_pVariable(&variable)
	, m_type(VVariable::k_VDateTime)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const VString& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_VString)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const VComponentProgrammable& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_VProgrammable)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const VStatus& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_VStatus)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const boolean& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_boolean)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(const VVariable& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_VVariable)
	, m_isReference(TRUE)	// There is a VT_VARIANT type (i.e. not or'ed with
							// VT_BYREF), but the only VARIANT union member
							// is a VT_BYREF.  Therefore, internally we treat
							// it like a VT_BYREF variable.  Quite bogus hun?
							// How's that for MS consistency
	, m_pVariantArg(0)
{
	initializeObjectSystem();

	copyFrom((VObject*) &variable);
// rhw 5/24/96: I got this wrong the first time around.  Now I think that
// this constructor should be a copy constructor.
//	m_pVariantArg = new VARIANTARG;

//	VariantInit(m_pVariantArg);
	//*this = variable;
}

VVariable::VVariable(const VCD& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_VCD)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

// Sanjay Start
VVariable::VVariable(const CUnknown& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_CUnknown)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}
// Sanjay End

// 16 bit OLE Automation doesn't define a char type, so we let the user
// know this by not letting them construct a VVariable from one.
#if defined(CV_WIN32)
VVariable::VVariable(const unsigned char& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_unsigned_char)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}
#endif

/*----------------------------------------------------------------------------
This cast constructor for native C-type arrays can not be implicitly handled
by the compiler.  This means that the caller is responsible for explicitly
casting the native array to a VArray first.  For functions that take
generalized VVariables as parameters, "casting" from a native C-type array
to a VVariable can be conveniently done on the fly like this:

	long someArray[10] [20] [30];			// Declares a 3-dimensional array
	void someFunct(VVariable& variable);	// Pedagogical function prototype

	// "Cast" a native array to a generic VVariable on the fly
	someFunct(VArray(someArray, VVariable::k_long, 3));

This method requires a little more help from the caller since this type of
information can not be easily captured in a cast constructor.  RTTI might be
of use if there were some general way to have the compiler cast all arrays
to this constructor.  I don't think that will happen.  So, for arrays, the
caller will just have to do a little more work.
----------------------------------------------------------------------------*/
VVariable::VVariable(const VArray& variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_VArray)
	, m_isReference(FALSE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(short* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_short)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(long* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_long)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(float* variable)						
	: m_pVariable(variable)
	, m_type(VVariable::k_float)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(double* variable)						
	: m_pVariable(variable)
	, m_type(VVariable::k_double)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(VMoney* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_VMoney)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(VDateTime* variable)						
	: m_pVariable(variable)
	, m_type(VVariable::k_VDateTime)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(VString* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_VString)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(VComponentProgrammable* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_VProgrammable)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(VStatus* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_VStatus)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(boolean* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_boolean)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::VVariable(VVariable* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_VVariable)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();

// Sanjay's Code Start
	copyFrom((VObject*) variable);
// Old Code Start
	//m_pVariantArg = new VARIANTARG;

	//VariantInit(m_pVariantArg);
// Old Code End
// Sanjay's Code End
}

VVariable::VVariable(VCD* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_VCD)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

// Sanjay Start
VVariable::VVariable(CUnknown* variable)
	: m_pVariable(variable)
	, m_type(VVariable::k_CUnknown)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}
// Sanjay End

// 16 bit OLE Automation doesn't define a char type, so we let the user
// know this by not letting them construct a VVariable from one.
#if defined(CV_WIN32)
VVariable::VVariable(unsigned char* variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_unsigned_char)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}
#endif

/*----------------------------------------------------------------------------
This cast constructor for native C-type arrays can not be implicitly handled
by the compiler.  This means that the caller is responsible for explicitly
casting the native array to a VArray first.  For functions that take
generalized VVariables as parameters, "casting" from a native C-type array
to a VVariable can be conveniently done on the fly like this:

	long someArray[10] [20] [30];			// Declares a 3-dimensional array
	void someFunct(VVariable& variable);	// Pedagogical function prototype

	// "Cast" a native array to a generic VVariable on the fly
	someFunct(VArray(someArray, VVariable::k_long, 3));

This method requires a little more help from the caller since this type of
information can not be easily captured in a cast constructor.  RTTI might be
of use if there were some general way to have the compiler cast all arrays
to this constructor.  I don't think that will happen.  So, for arrays, the
caller will just have to do a little more work.
----------------------------------------------------------------------------*/
VVariable::VVariable(VArray* variable)
	: m_pVariable(&variable)
	, m_type(VVariable::k_VArray)
	, m_isReference(TRUE)
	, m_pVariantArg(0)
{
	initializeObjectSystem();
}

VVariable::~VVariable()
{
	releaseVariant();
	
	uninitializeObjectSystem();
}

VVariable& VVariable::operator=(const VVariable& rhs)
{
	copyFrom((VObject*) &rhs);

	return *this;
}

boolean VVariable::copyFrom(VObject *pObject)
{
	cv_assert(pObject->isA(VVariableCls));
	VVariable*	source = (VVariable*) pObject;

	// Don't bother if an attempt is made to copy from yourself.
	// If there were pointer in this class, it could even be dangerous.
	if (this == source)
		return TRUE;
	
// Sanjay's Code Start
	releaseVariant();	
	m_pVariantArg = new VARIANTARG;
	VariantInit(m_pVariantArg);
// Sanjay's Code End

	m_pVariable		= source->m_pVariable;
	m_type			= source->m_type;
	m_pVariantArg	= source->m_pVariantArg;
	m_isReference	= source->m_isReference;
	// m_isInitialized is set in the constructor so don't worry about it here

	return TRUE;
}

void* VVariable::getAddress() const
{
	return (void*) m_pVariable;
}

// Converts this VVariable into a VARIANTARG by setting pVArg to reflect
// the nature of this instance.
void VVariable::getVariantArg(VARIANTARG* pVArg)
{
	DOSDate		dosDate;
	DOSTime		dosTime;

	if (m_isReference)
	{
		switch (m_type)
		{
			case k_error:
				// This is the value given by the default constructor.  It is 
				// indicative of a VVariable that has no value yet.  For this
				// reason, it is suitable for use as an OLE optional argument.
				// The following code initializes the variant arg to OLE
				// values that indicate that this VVariable should be
				// interpreted as an unused (or optional) argument.
				V_VT(pVArg) = VT_ERROR;
				pVArg->scode = DISP_E_PARAMNOTFOUND;

				break;
			case k_void:
				V_VT(pVArg) = VT_EMPTY | VT_BYREF;

				break;
			case k_short:
				V_VT(pVArg) = VT_I2 | VT_BYREF;
				V_I2REF(pVArg) = ((short*)m_pVariable);

				break;
			case k_long:
				V_VT(pVArg) = VT_I4 | VT_BYREF;
				V_I4REF(pVArg) = ((long*)m_pVariable);

				break;
			case k_float:
				V_VT(pVArg) = VT_R4 | VT_BYREF;
				V_R4REF(pVArg) = ((float*)m_pVariable);

				break;
			case k_double:
				V_VT(pVArg) = VT_R8 | VT_BYREF;
				V_R8REF(pVArg) = ((double*)m_pVariable);

				break;
			case k_VMoney:
				V_VT(pVArg) = VT_CY | VT_BYREF;
			
				// Convert the VMoney to a Microsoft CY
				// Note: We can use the address returned by VMoney
				// instead of having to create an internal member variable
				// as was done in other cases.
				V_CYREF(pVArg) = ((VMoney*)m_pVariable)->getCurrency();

				break;
			case k_VDateTime:
				V_VT(pVArg) = VT_DATE | VT_BYREF;

				// Prepare to convert VDateTime to variant date
				dosDate.dosDateBits.year		= ((VDateTime*)m_pVariable)->getYear() - 1980;
				dosDate.dosDateBits.month		= ((VDateTime*)m_pVariable)->getMonth();
				dosDate.dosDateBits.dayOfMonth	= ((VDateTime*)m_pVariable)->getDayOfMonth();

				// Prepare to convert VDateTime to variant time
				// Note:  Due to the storage technique used for DOS times (seconds
				// must be divided by 2 so that they fit in 5 bits), odd numbered
				// seconds will round down by one second.  That is, 45 seconds
				// will become 44 seconds in the resulting time.  I know of no
				// work-around.
				dosTime.dosTimeBits.hour	= ((VDateTime*)m_pVariable)->getHour();
				dosTime.dosTimeBits.minute	= ((VDateTime*)m_pVariable)->getMinute();
				dosTime.dosTimeBits.second	= ((VDateTime*)m_pVariable)->getSecond() / 2;
			
				// At this point, we usually give the address of our native
				// type, but since VDateTime is not native to OLE we needed to
				// use an internal DATE and use its address instead.
				DosDateTimeToVariantTime(dosDate.dosDate,
										 dosTime.dosTime,
										 &m_date);
				V_DATEREF(pVArg) = &m_date;

				break;
			case k_VString:
				V_VT(pVArg) = VT_BSTR | VT_BYREF;

				// Note:
				// The tests I ran on a MSVC++ 2.2 ClassWizard automation
				// object showed that it expected a wide character string.
				// Even when called from DispTest, the string printed
				// successfully.  I am assuming that the rest of the world
				// works that way also (i.e. assumes BSTRs are wide).
				// Therefore we convert our VString to a wide string.

				// At this point, we usually give the address of our native
				// type, but since VString is not native to OLE we needed to
				// use an internal BSTR and use its address instead.
				m_pBSTR = SysAllocString(*(VString*)m_pVariable);
				V_BSTRREF(pVArg) = &m_pBSTR;

				break;
			case k_VProgrammable:
				V_VT(pVArg) = VT_DISPATCH | VT_BYREF;

				// At this point, we usually give the address of our native
				// type, but since VComponentProgrammable is not native to OLE we needed
				// to use an internal pointer to an LPDISPATCH and use its
				// address instead.
				m_pIDispatch = ((VComponentProgrammable*)m_pVariable)->getIDispatch();
				V_DISPATCHREF(pVArg) = &m_pIDispatch;

				break;
			case k_VStatus:
				V_VT(pVArg) = VT_ERROR | VT_BYREF;

				// At this point, we usually give the address of our native
				// type, but since VStatus is not native to OLE we needed to
				// use an internal SCODE and use its address instead.
				m_scode = ((VStatus*)m_pVariable)->getStatusCode();
				V_ERRORREF(pVArg) = &m_scode;

				break;
			case k_boolean:
				V_VT(pVArg) = VT_BOOL | VT_BYREF;
				// Sanjay Start
				if(*((boolean*)m_pVariable)	== 1)
					*((boolean*)m_pVariable) = 0xffff;
				// Sanjay End	
				V_BOOLREF(pVArg) = ((VARIANT_BOOL*)(boolean*)m_pVariable);

				break;
			case k_VVariable:
				V_VT(pVArg) = VT_BYREF | VT_VARIANT;

				// Note: We can use the address returned by VVariable
				// instead of have to create an internal member variable
				// as was done in other cases.
				// Sanjay Start
				// Commented out
				//((VVariable*)m_pVariable)->getVariantArg(m_pVariantArg);
				// Sanjay End
				V_VARIANTREF(pVArg) = m_pVariantArg;

				break;
			case k_VCD:
				V_VT(pVArg) = VT_UNKNOWN| VT_BYREF;

				// rhw unfinished:  We need the VCD class from the Views folks
				// before we can get this working.
				// Note: Pointer assignment for this type
//				V_UNKNOWNREF(pVArg) = ((VCD*)m_pVariable)->getIUnknown();

				break;
			// Sanjay Start
			case k_CUnknown:
				V_VT(pVArg) = VT_UNKNOWN| VT_BYREF;
				V_UNKNOWNREF(pVArg) = &(((CUnknown*)m_pVariable)->lpIUnknown);
				break;
			// Sanjay End
// Unavailable on WIN16. Assert is used (default)
#if defined(CV_WIN32)
			case k_unsigned_char:
				V_VT(pVArg) = VT_UI1 | VT_BYREF;
				V_UI1REF(pVArg) = ((unsigned char*)m_pVariable);

				break;

#endif
			case k_VArray:
				// In this case, VArray has to set BOTH the type and the pointer
				// rhw unfinished: Not sure here
				// Sanjay Start
				((VArray*)m_pVariable)->convertToSafeArray(&V_VT(pVArg), V_ARRAY(pVArg));
				// Sanjay End

				break;
			default:
				// If we get here, something bad happended and type is unknown
				cv_assert(0);
				break;
		}
	}
	else
	{
		switch (m_type)
		{
			case k_error:
				// This is the value given by the default constructor.  It is 
				// indicative of a VVariable that has no value yet.  For this
				// reason, it is suitable for use as an OLE optional argument.
				// The following code initializes the variant arg to OLE
				// values that indicate that this VVariable should be
				// interpreted as an unused (or optional) argument.
				V_VT(pVArg) = VT_ERROR;
				pVArg->scode = DISP_E_PARAMNOTFOUND;

				break;
			case k_void:
				V_VT(pVArg) = VT_EMPTY;

				break;
			case k_short:
				V_VT(pVArg) = VT_I2;
				V_I2(pVArg) = *((short*)m_pVariable);

				break;
			case k_long:
				V_VT(pVArg) = VT_I4;
				V_I4(pVArg) = *((long*)m_pVariable);

				break;
			case k_float:
				V_VT(pVArg) = VT_R4;
				V_R4(pVArg) = *((float*)m_pVariable);

				break;
			case k_double:
				V_VT(pVArg) = VT_R8;
				V_R8(pVArg) = *((double*)m_pVariable);

				break;
			case k_VMoney:
				V_VT(pVArg) = VT_CY;
			
				// Convert the VMoney to a Microsoft CY
				V_CY(pVArg) = *((VMoney*)m_pVariable)->getCurrency();

				break;
			case k_VDateTime:
				V_VT(pVArg) = VT_DATE;

				// Prepare to convert VDateTime to variant date
				dosDate.dosDateBits.year		= ((VDateTime*)m_pVariable)->getYear() - 1980;
				dosDate.dosDateBits.month		= ((VDateTime*)m_pVariable)->getMonth();
				dosDate.dosDateBits.dayOfMonth	= ((VDateTime*)m_pVariable)->getDayOfMonth();

				// Prepare to convert VDateTime to variant time
				// Note:  Due to the storage technique used for DOS times (seconds
				// must be divided by 2 so that they fit in 5 bits), odd numbered
				// seconds will round down by one second.  That is, 45 seconds
				// will become 44 seconds in the resulting time.  I know of no
				// work-around.
				dosTime.dosTimeBits.hour	= ((VDateTime*)m_pVariable)->getHour();
				dosTime.dosTimeBits.minute	= ((VDateTime*)m_pVariable)->getMinute();
				dosTime.dosTimeBits.second	= ((VDateTime*)m_pVariable)->getSecond() / 2;

			
				// Convert DOS date and time to Variant time
				DosDateTimeToVariantTime(dosDate.dosDate,
										 dosTime.dosTime,
										 &V_DATE(pVArg));

				break;
			case k_VString:
				V_VT(pVArg) = VT_BSTR;

				// Note:
				// The tests I ran on a MSVC++ 2.2 ClassWizard automation
				// object showed that it expected a wide character string.
				// Even when called from DispTest, the string printed
				// successfully.  I am assuming that the rest of the world
				// works that way also (i.e. assumes BSTRs are wide).
				// Therefore we convert our VString to a wide string.

				V_BSTR(pVArg) = SysAllocString(*(VString*)m_pVariable);

				break;
			case k_VProgrammable:
				V_VT(pVArg) = VT_DISPATCH;
				// Note: Pointer assignment for this type
				V_DISPATCH(pVArg) = ((VComponentProgrammable*)m_pVariable)->getIDispatch();

				break;
			case k_VStatus:
				V_VT(pVArg) = VT_ERROR;
				V_ERROR(pVArg) = ((VStatus*)m_pVariable)->getStatusCode();

				break;
			case k_boolean:
				V_VT(pVArg) = VT_BOOL;
				// Sanjay Start
				if(*((boolean*)m_pVariable)	== 1)
					*((boolean*)m_pVariable) = 0xffff;
				// Sanjay End	
				V_BOOL(pVArg) = *((boolean*)m_pVariable);

				break;
			case k_VVariable:
				// rhw: I am not sure that this case statement is needed
				// since VARIANTs are always passed by reference.  For
				// debugging purposes, complain about it here since this
				// should not happen and we need to bring this situation
				// to light if it does.
				cv_assert(0);

//				V_VT(pVArg) = VT_BYREF | VT_VARIANT;

//				((VVariable*)m_pVariable)->getVariantArg(m_pVariantArg);
				// Note: Pointer assignment for this type
//				V_VARIANTREF(pVArg) = m_pVariantArg;

				break;
			case k_VCD:
				V_VT(pVArg) = VT_UNKNOWN;
				// rhw unfinished:  We need the VCD class from the Views folks
				// before we can get this working.
				// Note: Pointer assignment for this type
//				V_UNKNOWN(pVArg) = ((VCD*)m_pVariable)->getIUnknown();

				break;
			// Sanjay Start
			case k_CUnknown:
				V_VT(pVArg) = VT_UNKNOWN;
				V_UNKNOWN(pVArg) = ((CUnknown*)m_pVariable)->getIUnknown();
				break;
			// Sanjay End
// Unavailable on WIN16. Assert is used (default)
#if defined(CV_WIN32)
			case k_unsigned_char:
				V_VT(pVArg) = VT_UI1;
				V_UI1(pVArg) = *((unsigned char*)m_pVariable);

				break;
#endif
			case k_VArray:
				// In this case, VArray has to set BOTH the type and the pointer
				// Sanjay Start
				((VArray*)m_pVariable)->convertToSafeArray(&V_VT(pVArg), V_ARRAY(pVArg));
				// Sanjay End

				break;
			default:
				// If we get here, something bad happended and type is unknown
				cv_assert(0);
				break;
		}
	}
}

void VVariable::releaseVariant()
{
	if (m_pVariantArg && m_isInitialized)
	{
		VariantClear(m_pVariantArg);
		delete m_pVariantArg;
		m_pVariantArg = 0;
	}
}

// Converts a VARIANTARG into a VVariable by setting this instance to reflect
// the nature of pVArg.
void VVariable::operator=(VARIANTARG* pVArg)
{
	DOSDate dosDate;
	DOSTime	dosTime;

	if (!pVArg)
		return;	

	if (V_VT(pVArg) == VT_EMPTY)
	{
		m_isReference = FALSE;
		m_type = k_void;
	}
	else if (V_VT(pVArg) == VT_I2)
	{
		m_isReference = FALSE;
		m_type = k_short;

		*((short*)m_pVariable) = V_I2(pVArg);
	}
	else if (V_VT(pVArg) == VT_I4)
	{
		m_isReference = FALSE;
		m_type = k_long;

		*((long*)m_pVariable) = V_I4(pVArg);
	}
	else if (V_VT(pVArg) == VT_R4)
	{
		m_isReference = FALSE;
		m_type = k_float;

		*((float*)m_pVariable) = V_R4(pVArg);
	}
	else if (V_VT(pVArg) == VT_R8)
	{
		m_isReference = FALSE;
		m_type = k_double;

		*((double*)m_pVariable) = V_R8(pVArg);
	}
	else if (V_VT(pVArg) == VT_CY)
	{
		m_isReference = FALSE;
		m_type = k_VMoney;

		// Convert the VMoney to a Microsoft CY
		((VMoney*)m_pVariable)->setCurrency(&V_CY(pVArg));
	}
	else if (V_VT(pVArg) == VT_DATE)
	{
		m_isReference = FALSE;
		m_type = k_VDateTime;

		// Prepare to convert variant date and time to VDateTime
		VariantTimeToDosDateTime(V_DATE(pVArg),
								 &dosDate.dosDate,
								 &dosTime.dosTime);

		// Set variable to variant-converted date
		((VDateTime*)m_pVariable)->setDate((VDate::Year) dosDate.dosDateBits.year + 1980,
										   (VDate::Month) dosDate.dosDateBits.month,
										   (VDate::DayOfMonth) dosDate.dosDateBits.dayOfMonth);

		// Set variable to variant-converted time
		// Note:  Due to the storage technique used for DOS times (seconds
		// must be divided by 2 so that they fit in 5 bits), odd numbered
		// seconds will round down by one second.  That is, 45 seconds
		// will become 44 seconds in the resulting time.  I know of no
		// work-around.
		((VDateTime*)m_pVariable)->setTime(dosTime.dosTimeBits.hour,
										   dosTime.dosTimeBits.minute,
										   dosTime.dosTimeBits.second * 2);
	}
	else if (V_VT(pVArg) == VT_BSTR)
	{
		m_isReference = FALSE;
		m_type = k_VString;
		
		// The tests I ran on a MSVC++ 2.2 ClassWizard automation
		// object showed that it returned a wide character string
		// in the variant argument.  I am assuming that the rest
		// of the world works that way also (i.e. assumes BSTRs
		// are wide).  Therefore we convert our VString to a wide
		// string.
		((VString*)m_pVariable)->puts((VWideChar *)V_BSTR(pVArg));
	}
	else if (V_VT(pVArg) == VT_DISPATCH)
	{
		m_isReference = FALSE;
		m_type = k_VProgrammable;

		// Note: Pointer assignment for this type
		((VComponentProgrammable*)m_pVariable)->setIDispatch(V_DISPATCH(pVArg));
	}
	else if (V_VT(pVArg) == VT_ERROR)
	{
		m_isReference = FALSE;
		m_type = k_VStatus;

		((VStatus*)m_pVariable)->setStatusCode(V_ERROR(pVArg));
	}
	else if (V_VT(pVArg) == VT_BOOL)
	{
		m_isReference = FALSE;
		m_type = k_boolean;

		*((boolean*)m_pVariable) = V_BOOL(pVArg);
		// Sanjay Start
		if(*((boolean*)m_pVariable)	== 0xffff)
			*((boolean*)m_pVariable) = 1;
		// Sanjay End	
	}
	else if (V_VT(pVArg) == VT_VARIANT)
	{
		// rhw: I am not sure that this case statement is needed
		// since VARIANTs are always passed by reference.  For
		// debugging purposes, complain about it here to bring this
		// situation to light.
		cv_assert(0);

//		m_isReference = FALSE;
//		m_type = k_VVariable;

//		((VVariable*)m_pVariable)->operator=(V_VARIANTREF(pVArg));
	}
	else if (V_VT(pVArg) == VT_UNKNOWN)
	{
		// rhw unfinished:  We need the VCD class from the Views folks
		// before we can complete this.
		m_isReference = FALSE;
		// Sanjay Start
		((CUnknown*)m_pVariable)->setIUnknown(V_UNKNOWN(pVArg));
		// Sanjay End
	}
// Unavailable on WIN16. Assert is used (plain else)
#if defined(CV_WIN32)
	else if (V_VT(pVArg) == VT_UI1)
	{
		m_isReference = FALSE;
		m_type = k_unsigned_char;

		*((unsigned char*)m_pVariable) = V_UI1(pVArg);
	}
#endif
	else if (V_ISARRAY(pVArg))
	{
		m_isReference = FALSE;

		// In this case, VArray gets BOTH the type and the array from pVArg
		// Sanjay Start
		((VArray*)m_pVariable)->operator=(pVArg);
		// Old Start
		//((VArray*)m_pVariable)->operator=(V_ARRAY(pVArg));
		// Old End
		// Sanjay End
	}
	// Now repeat checking the same types again, but this time as pointers
	// to those types of variables
	else if (V_VT_REFTYPE(pVArg) == VT_I2)
	{
		m_isReference = TRUE;
		m_type = k_short;

		*((short*)m_pVariable) = *V_I2REF(pVArg);
	}
	else if (V_VT_REFTYPE(pVArg) == VT_I4)
	{
		m_isReference = TRUE;
		m_type = k_long;

		*((long*)m_pVariable) = *V_I4REF(pVArg);
	}
	else if (V_VT_REFTYPE(pVArg) == VT_R4)
	{
		m_isReference = TRUE;
		m_type = k_float;

		*((float*)m_pVariable) = *V_R4REF(pVArg);
	}
	else if (V_VT_REFTYPE(pVArg) == VT_R8)
	{
		m_isReference = TRUE;
		m_type = k_double;

		*((double*)m_pVariable) = *V_R8REF(pVArg);
	}
	else if (V_VT_REFTYPE(pVArg) == VT_CY)
	{
		m_isReference = TRUE;
		m_type = k_VMoney;

		// Convert the VMoney to a Microsoft CY
		((VMoney*)m_pVariable)->setCurrency(V_CYREF(pVArg));
	}
	else if (V_VT_REFTYPE(pVArg) == VT_DATE)
	{
		m_isReference = TRUE;
		m_type = k_VDateTime;

		// Prepare to convert variant date and time to VDateTime
		VariantTimeToDosDateTime(*V_DATEREF(pVArg),
								 &dosDate.dosDate,
								 &dosTime.dosTime);

		// Set variable to variant-converted date
		((VDateTime*)m_pVariable)->setDate((VDate::Year) dosDate.dosDateBits.year + 1980,
										   (VDate::Month) dosDate.dosDateBits.month,
										   (VDate::DayOfMonth) dosDate.dosDateBits.dayOfMonth);

		// Set variable to variant-converted time
		// Note:  Due to the storage technique used for DOS times (seconds
		// must be divided by 2 so that they fit in 5 bits), odd numbered
		// seconds will round down by one second.  That is, 45 seconds
		// will become 44 seconds in the resulting time.  I know of no
		// work-around.
		((VDateTime*)m_pVariable)->setTime(dosTime.dosTimeBits.hour,
										   dosTime.dosTimeBits.minute,
										   dosTime.dosTimeBits.second * 2);
	}
	else if (V_VT_REFTYPE(pVArg) == VT_BSTR)
	{
		m_isReference = TRUE;
		m_type = k_VString;

		// The tests I ran on a MSVC++ 2.2 ClassWizard automation
		// object showed that it returned a wide character string
		// in the variant argument.  I am assuming that the rest
		// of the world works that way also (i.e. assumes BSTRs
		// are wide).  Therefore we convert our VString to a wide
		// string.
		// -rwh

		// Note: I think that BSTR's depend on is the object was compiled
		// with OLE2ANSI or not. Runtime detection of ANSI or UNICODE may
		// be an issue.
		// -dss 2/13/96
		((VString*)m_pVariable)->puts((VWideChar *)*V_BSTRREF(pVArg));
	}
	else if (V_VT_REFTYPE(pVArg) == VT_DISPATCH)
	{
		m_isReference = TRUE;
		m_type = k_VProgrammable;

		// Note: Pointer assignment for this type
		((VComponentProgrammable*)m_pVariable)->setIDispatch(*V_DISPATCHREF(pVArg));
	}
	else if (V_VT_REFTYPE(pVArg) == VT_ERROR)
	{
		m_isReference = TRUE;
		m_type = k_VStatus;

		((VStatus*)m_pVariable)->setStatusCode(*V_ERRORREF(pVArg));
	}
	else if (V_VT_REFTYPE(pVArg) == VT_BOOL)
	{
		m_isReference = TRUE;
		m_type = k_boolean;

		*((boolean*)m_pVariable) = *V_BOOLREF(pVArg);
		// Sanjay Start
		if(*((boolean*)m_pVariable)	== 0xffff)
			*((boolean*)m_pVariable) = 1;
		// Sanjay End	
	}
	else if (V_VT_REFTYPE(pVArg) == VT_VARIANT)
	{
		m_isReference = TRUE;
		m_type = k_VVariable;

// Sanjay's Code Start
		releaseVariant();	
		m_pVariantArg = new VARIANTARG;
		VariantInit(m_pVariantArg);
		*m_pVariantArg = *V_VARIANTREF(pVArg);
// Old Code Start
		//((VVariable*)m_pVariable)->operator=(V_VARIANTREF(pVArg));
// Old Code End
// Sanjay's Code End
	}
	else if (V_VT_REFTYPE(pVArg) == VT_UNKNOWN)
	{
		// rhw unfinished:  We need the VCD class from the Views folks
		// before we can complete this.
		m_isReference = TRUE;
		// Sanjay Start
		((CUnknown*)m_pVariable)->setIUnknown(*V_UNKNOWNREF(pVArg));
		// Sanjay End
	}
// Unavailable on WIN16. Assert is used (plain else)
#if defined(CV_WIN32)
	else if (V_VT_REFTYPE(pVArg) == VT_UI1)
	{
		m_isReference = TRUE;
		m_type = k_unsigned_char;

		*((unsigned char*)m_pVariable) = *V_UI1REF(pVArg);
	}
#endif
	else if (V_VT_REFTYPE(pVArg) & VT_ARRAY)
	{
		m_isReference = TRUE;

		// In this case, VArray gets BOTH the type and the array from pVArg
		// Sanjay Start
		((VArray*)m_pVariable)->operator=(pVArg);
		// Old Start
		//((VArray*)m_pVariable)->operator=(*V_ARRAYREF(pVArg));
		// Old End
		// Sanjay End
	}
	else
	{
		// If we get here, something bad happended and type is unknown
		cv_assert(0);
	}
}

void VVariable::initializeObjectSystem()
{
	// Multiple calls to this API cause no harm and simply need to be balanced
	// with a call to OleUninitialize().  This approach assures that OLE will
	// still be active so that this object can safely release its OLE resources.
	m_isInitialized = SUCCEEDED(OleInitialize(NULL));

	cv_assert(m_isInitialized);
}

void VVariable::uninitializeObjectSystem()
{
	// If OLE was successfully initialized, then balance out with an uninitialize
	if (m_isInitialized)
	{
		OleUninitialize();
		m_isInitialized = FALSE;
	}
}
