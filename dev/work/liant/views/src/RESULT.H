// $Header:   Q:/views/common/vcs/RESULT.H_V   1.15   Mar 24 1997 16:13:10   scottt  $

//  result.h
//
//  Result Class [Common]
//
//  Allegris Foundation 1.1.00
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  04/20/94 dss	Initial release.
//  11/14/95 dss	VC2.0 debugger can't see unnamed structs.
//  12/06/95 dss	import the result reports, not export.
//  12/06/95 dss	VRESULT_H not _RESULT_H_.
//  12/07/95 dgm	Added operator=().
//  12/07/95 dss	Added some includes.
//  02/19/96 evc	Changed some declarations to CV_NEARDATA
//  02/19/96 evc	Made the declaration FAR for CV_WIN16 DLL
//  02/20/96 evc	typo
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VRESULT_H
#define VRESULT_H

#include "defs.h"
#include "str.h"

CLASS VResult;

CLASS VResultReport
{
public:
    VResultReport() : resultType("Unknown"), userResultType("Unknown") {;}
    VResultReport(const VString& type, const VString& userType)
	 : resultType(type), userResultType(userType) {;}

    virtual const VString&  asUserMessage(VResult&);
    virtual const VString&  asSystemMessage(VResult&);

    virtual const VString&  asResultType(VResult&);
    virtual const VString&  asUserResultType(VResult&);

protected:
    VString         resultType;
    VString         userResultType;
    VString         resultText;
};

CLASS CIOResultReport : public VResultReport
{
public:
    CIOResultReport() : VResultReport((const char*)"C Library I/O", (const char*)"File") {;}

    const VString&  asSystemMessage(VResult&);
};

CLASS MSWIOResultReport : public VResultReport
{
public:
    MSWIOResultReport() : VResultReport((const char*)"MS-Windows IO", (const char*)"File") {;}

    const VString&  asSystemMessage(VResult&);
};

// ---------------------------------------------------------------------------
//
CLASS VResult
{
public:
    VResult();

    long                asNumber() const {return *(long*)&resultCode;}
    const void *        asAddress() const {return &resultCode;}

    const VResultReport& getReport() {return *report;} 

    void                setResult(long, const VResultReport&);
    void                setResult(void *, int, const VResultReport&);
    
    boolean             isA(const VResultReport& resultReport)
			{return &resultReport == report;}
    
    const VString&  asUserMessage() {return report->asUserMessage(*this);}
    const VString&  asSystemMessage() {return report->asSystemMessage(*this);}

    const VString&  asResultType() {return report->asResultType(*this);}

    VResult&            operator=(const VResult& r)
			    { resultCode = r.resultCode;
			      report = r.report;
			      return *this; }

protected:
    //!!!dss - VC2.0 debugger can't see unnamed structs.
    struct _unnamed1 {char data[4];}    resultCode;

    VResultReport   *report;
};



// 16 bit static microsoft compiler will always
// put a global object in a 'DATA_FAR' segment, which 
// prevents more that one copy of a program linked with
// our library to run a a time. CV_NEARDATA is defined
// to __near and will fix this.
// (We are explicitly matching the 'extern' declaration with the 
// definition in globals2.cpp)

#if defined (CV_WIN16) && !defined (DLL)
    extern VResultReport        CV_NEARDATA nullResultReport;
    extern CIOResultReport      CV_NEARDATA cioResultReport;
    extern MSWIOResultReport    CV_NEARDATA mswioResultReport;

#elif defined (CV_WIN16) && !defined (DLL)
    extern VResultReport        FAR nullResultReport;
    extern CIOResultReport      FAR cioResultReport;
    extern MSWIOResultReport    FAR mswioResultReport;
#elif defined (CV_WIN32) && defined(_MSC_VER)
    extern VResultReport        CV_DLL_LINKAGE nullResultReport;
    extern CIOResultReport      CV_DLL_LINKAGE cioResultReport;
    extern MSWIOResultReport    CV_DLL_LINKAGE mswioResultReport;
#else
    extern VResultReport        CV_DLL_IMPORT nullResultReport;
    extern CIOResultReport      CV_DLL_IMPORT cioResultReport;
    extern MSWIOResultReport    CV_DLL_IMPORT mswioResultReport;
#endif

#endif // VRESULT_H

