// $Header:   Q:/views/common/vcs/RESULT.CPV   1.5   Mar 24 1997 16:13:10   scottt  $

//	result.cpp
//
//	Result Class [Common]
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
//	04/20/94 dss	Initial release.
//	12/06/95 dss	moved definition of result reporter to globals2.cpp.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
// ---------------------------------------------------------------------------

#include "string.h"

#include "str.h"
#include "result.h"
#include "cvassert.h"

// ---------------------------------------------------------------------------
//
//   VResult
//
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//
VResult::VResult()
{	
	*(long*)&resultCode = 0;
	report = &nullResultReport;
}

// ---------------------------------------------------------------------------
void VResult::setResult(long r, const VResultReport& resultReport) 
{
	*(long*)&resultCode = r; 
	report = &(VResultReport&)resultReport; 
}
		
// ---------------------------------------------------------------------------
void VResult::setResult(void *theResult, int resultSize, 
						const VResultReport& resultReport) 
{
	cv_assert(resultSize <= sizeof(resultCode));
	memcpy(&resultCode, theResult, 
		resultSize <= sizeof(resultCode) ? resultSize : sizeof(resultCode)); 
	report = &(VResultReport&)resultReport; 
}
		
// ---------------------------------------------------------------------------
//
//   VResultReport
//
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//
const VString& VResultReport::asUserMessage(VResult& theResult)
{
	cv_assert(&theResult.getReport() == this);
	
	// since VResultReport::asSystemMessage() reports a message
	// for the programmer, return a message that the user can understand.
	resultText.puts("No information is available");
	return resultText;
}

// ---------------------------------------------------------------------------
//
const VString& VResultReport::asSystemMessage(VResult& theResult)
{
	cv_assert(&theResult.getReport() == this);
	resultText.puts("Uninitialized result");
	return resultText;
}

// ---------------------------------------------------------------------------
//
const VString& VResultReport::asResultType(VResult& theResult)
{
	cv_assert(&theResult.getReport() == this);
	return resultType;
}

// ---------------------------------------------------------------------------
//
const VString& VResultReport::asUserResultType(VResult& theResult)
{
	cv_assert(&theResult.getReport() == this);
	return userResultType;
}

// ---------------------------------------------------------------------------
//
//   CIOResultReport
//
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//
const VString& CIOResultReport::asSystemMessage(VResult& theResult)
{
	cv_assert(&theResult.getReport() == this);
	resultText.puts(strerror((int)theResult.asNumber()));
	return resultText;
}

// ---------------------------------------------------------------------------
//
//   MSWIOResultReport
//
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//
const VString& MSWIOResultReport::asSystemMessage(VResult& theResult)
{
	cv_assert(&theResult.getReport() == this);
	resultText.puts("No information is available");
	return resultText;
}
