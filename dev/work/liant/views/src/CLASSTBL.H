// $Header:   Q:/views/common/vcs/CLASSTBL.H_V   1.11   Mar 24 1997 16:08:26   scottt  $ 

//  classtbl.h
//
//  VClassTable class interface [Common]
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
//  Revision History:
//  -----------------
//  03/05/93 pmcm   preened, added VInitDLL::get(), commented
//  01/13/94 dgm    merged and preened
//  01/17/94 pkt    preened.
//  01/31/94 pmcm   added destructor for VInitDLL
//  04/11/95 pkt    made 'of' char* parameter const.
//  05/26/95 pkt    combined international changes into common version.
//	06/12/96 pkt	updated HANDLE -> VHANDLE.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VCLASSTBL_H
#define VCLASSTBL_H

// ---------------------------------------------------------------------------
//
#include "global.h"
#include "winclass.h"

// ---------------------------------------------------------------------------
// global type of the VClassTable class
//
#define GLOBALclassTable 0 

// ---------------------------------------------------------------------------
//
CLASS VClass;

// ---------------------------------------------------------------------------
//
typedef struct {    //  a cell in the class table
    VClass *cls;    //      meta-class instance
    VHANDLE hMe;     //   applctn task handle (always 0 in non-DLL context)
} ClsObjs;

// ---------------------------------------------------------------------------
//  VClassTable is a VGlobal class that provides meta-class information.
//  Its methods are not to be called directly, but through VClass 
//  (see class.h).  In a Views application, there exists a single 
//  global instance of VClassTable called `classTable`.
//

CLASS VClassTable : public VGlobal {

public:
    // constructor/destructor
                    VClassTable();
                   ~VClassTable();

    //  the usual class identity checkers
    VClass         *iam();
    VClass         *of(const char *);

    //  table operations
    void            add(VClass *);
    void            remove(VClass *);

    //  indirect reference looks up the application's global class table
    //  for DLL
    VClassTable    *operator->();

protected:
    void            init();

protected:
    short           top;        // number of classes in this table
    short           max;        // remaining space in class table 
    ClsObjs        *clsTbl;     // table of classes defined in this app
    VHANDLE			hClsTbl;    // handle to class table

private:
    friend      VClass;
};

extern VClass *VClassTableCls;

// ---------------------------------------------------------------------------
//  pointer to the class table object
//
extern VClassTable *classTable;

// ---------------------------------------------------------------------------
//

#if defined (DLL) && defined (CV_WIN16)

//#ifdef DLL
//  
//  The VInitDLL class provides a DLL with a flag to indicate to
//  the Allegris library whether or not the DLL is initializing.
//  This flag is shared by all Allegris DLLs.  The VMemory, 
//  VGlobal, and VClassTable classes are sensitive to 
//  this flag, particularly in respect to memory allocations.
//
//  This flag should be pre-set for DLL initialization, cleared 
//  when initialization is complete, and set again, when the DLL
//  exits.  
//
//
//  Typically, a DLL contains a module with a static global 
//  VInitDLL object and presets the flag to 1 like this:
//
//      VInitDLL initDLL(1);        //  static global VInitDLL object
//  
//  After initialization, the flag is cleared like this:
//  
//      initDLL.clear();            //  initialization complete
//
//  Under MS-Windows, you would clear the flag in the DLL's LibMain() 
//  routine and set it upon exit in the DLL's WEP() procedure.
//

CLASS VInitDLL {
public:
    VInitDLL(short isInitializing);
    ~VInitDLL();
    void    clear();
    void    set();
    short   get();
};
#endif // DLL

#endif // VCLASSTBL_H
