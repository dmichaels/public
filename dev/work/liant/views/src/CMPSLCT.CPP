// $Header:   Q:/views/win/vcs/CMPSLCT.CPv   1.6   Mar 04 1997 14:12:02   gregm  $ 

//	cmpslct.cpp
//
//	VComponentSelect implementation
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
//	01/31/96 dss	Initial revision.
//  02/23/96 dss    Borland conversion.
//  03/04/96 dss    was using wrong ComponentItem constructor
//  06/12/96 evc	A fix to the borland 5.0 inspired removal of 
// 					conversion operators.   getOleObject() 
//					returns aaa CROleObject& that needs to be converted
//					to an LPOLEOBJECT by saying (LPOLEOBJECT)
//					&getOleObject().getunKnown()
// ----------------------------------------------------------------------------

// Views Includes
#include "notifier.h"

// C Includes
#include "commdlg.h"
#include "limits.h"

// Component Includes
#include "pre.h"
#include "cmpslct.h"

#if !defined(PATH_MAX)
    #define PATH_MAX    256
#endif

defineClass(VComponentSelect, VDialog)

// ---------------------------------------------------------------------------
VComponentSelect::VComponentSelect()
{
	;
}

// ---------------------------------------------------------------------------
// Construct a component select dialog object.
//
VComponentSelect::VComponentSelect(const VString& title,
								   VComponentItem *& component,
								   VComponentWindow& parent,		// component's parent
								   VWindow *win,
								   int itemStyle)
{
	(*this)(title, component, parent, win, itemStyle);
}

// ---------------------------------------------------------------------------
VComponentSelect::~VComponentSelect()
{
	;
}

// ---------------------------------------------------------------------------
// Construct a component select dialog object.
//
boolean VComponentSelect::dialog(const VString& title,
								 VComponentItem *& component,
								 VComponentWindow& parent,		// component's parent
								 VWindow *win,
								 int itemStyle)
{
	VComponentSelect f(title, component, parent, win, itemStyle);

	return component != 0;
}


// ---------------------------------------------------------------------------
// Where it actually happens...
//
// Note:
//
// On 32 bit MSW, OLE2UI defaults to ANSI but is controlled by the UNICODE define.
// On Borland (16 & 23 bit) is defaults to UNICODE and is controlled with OLE2ANSI.
//
void VComponentSelect::operator()(const VString& title,
									 VComponentItem *& component,
									 VComponentWindow& parent,		// component's parent
									 VWindow *win,
									 int itemStyle)
{

	// On MSW32 bit OLEUIINSERTOBJECT is OLEUIINSERTOBJECTW if UNICODE, but
	// OLEUIINSERTOBJECTA if not. On Borland it is always UNICODE and there are
	// no A & W postfixes.
	OLEUIINSERTOBJECT	io;
	UINT				iResult;

#if defined(_MSC_VER) || defined(CV_WIN16)
	TCHAR				szFile[PATH_MAX];	// TCHAR is UNICODE controlled
#else	// 16 or 32 bit borland
	OLECHAR				szFile[PATH_MAX];	// OLECHAR is OLE2ANSI controlled
#endif
	HRESULT				hResult;
                                                        
    notifier->beginWait();

	component = 0;

    // clear the structure
    _fmemset(&io, 0, sizeof(OLEUIINSERTOBJECT));

    // fill the structure
    io.cbStruct = sizeof(OLEUIINSERTOBJECT);
    io.dwFlags = IOF_SELECTCREATENEW | IOF_DISABLEDISPLAYASICON;

    if ((itemStyle & LinkedItem) == 0) {
        io.dwFlags |= IOF_DISABLELINK;
    }

    io.hWndOwner = parent.getVThis()->hwnd();
    io.lpszCaption = title;
    io.lpszFile = szFile;
    io.cchFile = sizeof(szFile);
    _fmemset((LPSTR)szFile, 0, sizeof(szFile));

    iResult = OleUIInsertObject(&io);

    if (iResult == OLEUI_OK) {

        if (io.dwFlags & IOF_SELECTCREATEFROMFILE) {
			component = new VComponentItem(VPathString(io.lpszFile), parent, *win,
                       ((io.dwFlags & IOF_CHECKLINK) ? LinkedItem : EmbeddedItem) |
                       FileSource);
        }
        else {
			LPOLESTR	pProgId;
			ProgIDFromCLSID(io.clsid, &pProgId);
			component = new VComponentItem(VString(pProgId), parent, *win);
			// TO DO: does the pProgId have to be freed?
        }

        // tell the newly inserted ole object to cache its CF_TEXT data, if any.
        hResult = component->getOleObject().Cache(NULL, CF_TEXT);
        cv_assert(SUCCEEDED(hResult));

        // if file was selected, then tell the ole object to update.
        // This will get the data we asked for, from the cache.
        // NOTE: update starts the server: we don't need to do this
        // if insert new is called because there is no data to be cached.
        // After the ole object is edited, the cache setting will cache the
        // requested formats.
        if (io.dwFlags & IOF_SELECTCREATEFROMFILE) {
            // ask the object to save to initialize the cache
            LPPERSISTSTORAGE pPersistStorage;
			LPDATAOBJECT lpDataObject;

            DebugOutputEnumStatStg((LPSTORAGE) &component->getStorage());


    		component->getOleObject().QueryInterface(IID_IDataObject,
			(LPVOID FAR *) &lpDataObject);

            DebugOutputEnumFmtEtc((LPDATAOBJECT)  lpDataObject );

            DebugOutputDataObj((LPDATAOBJECT) lpDataObject );

            component->getOleObject().QueryInterface(IID_IPersistStorage,
                    (LPVOID FAR*)&pPersistStorage);
            cv_assert(pPersistStorage);
            OleSave(pPersistStorage, (LPSTORAGE) &component->getStorage(), TRUE);

            // pass NULL so that object application won't forget the real stg
            pPersistStorage->SaveCompleted(NULL);
            pPersistStorage->Release();

            DebugOutputEnumStatStg((LPSTORAGE) &component->getStorage());
            DebugOutputEnumFmtEtc((LPDATAOBJECT) 
			lpDataObject );

            DebugOutputDataObj((LPDATAOBJECT) 
			lpDataObject );
        }   // if from file

    }

    notifier->endWait();
}
