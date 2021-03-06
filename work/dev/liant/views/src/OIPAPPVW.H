// $Header:   Q:/views/win/vcs/oipappvw.h_v   1.5   Jun 03 1997 09:51:54   scottt  $

//	VOleInPlaceAppView
//
//  Ole In Place AppView Interface
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
//	02/17/95 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#if !defined( _VOLEINPLACEAPPVIEW_H_ )
#define _VOLEINPLACEAPPVIEW_H_

// BASIC Views OLE types
#include "oledefs.h"

// Views Ole Classes
#include "oipframe.h"

// Forward Declarations
CLASS VOleInPlaceAppView;
typedef	VOleInPlaceAppView FAR *LPVOLEINPLACEAPPVIEW;

CLASS VOleInPlaceAppView : public COleInPlaceFrame
{
public:

	VOleInPlaceAppView(REFVCOMPONENTAPPVIEW refCDAppView);
	~VOleInPlaceAppView();

	LPTSTR			getClassName() {return _T("VOleInPlaceAppView");}

	// *** IOleWindow Overriddables ***
	STDMETHODIMP GetWindow(HWND FAR* lphwnd);

	// *** IOleIPFrame Overriddables ***
	STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS lpBorderWidths);

	// *** VOleInPlaceAppView methods ***
};

#define  STD_OLEINPLACEAPPVIEW_METHODS() \
	virtual STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS lpBorderWidths) \
		{return getOleInPlaceAppView().SetBorderSpace(lpBorderWidths);}

#define  STD_OleInPlaceAppView_METHODS() STD_OLEINPLACEAPPVIEW_METHODS()

#define  STD_OLEINPLACEAPPVIEW_ACCESS(access) \
		CONTAIN_ACCESS(V, OleInPlaceAppView, access)

#define  INHERIT_OLEINPLACEAPPVIEW_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*OleInPlaceAppView) \
	STD_IOLEWINDOW_METHODS() \
	STD_IOLEWINDOW_ACCESS(*OleInPlaceAppView) \
	STD_IOLEINPLACEUIWINDOW_METHODS() \
	STD_IOLEINPLACEUIWINDOW_ACCESS(*OleInPlaceAppView) \
	STD_IOLEINPLACEFRAME_METHODS() \
	STD_IOLEINPLACEFRAME_ACCESS(*OleInPlaceAppView) \
	STD_OLEINPLACEAPPVIEW_METHODS() \
	STD_OLEINPLACEAPPVIEW_ACCESS(*OleInPlaceAppView) \
protected: \
	VOleInPlaceAppView	*OleInPlaceAppView;

// No Methods exported, other than OleInPlaceAppView methods, to allow
// the other class to be sub-classed. (This needs to be fixed).
#define  CONTAIN_OLEINPLACEAPPVIEW_INTERFACE() \
public: \
	STD_IUNKNOWN_ACCESS(*OleInPlaceAppView) \
	STD_IOLEWINDOW_ACCESS(*OleInPlaceAppView) \
	STD_IOLEINPLACEUIWINDOW_ACCESS(*OleInPlaceAppView) \
	STD_IOLEINPLACEFRAME_ACCESS(*OleInPlaceAppView) \
	STD_OLEINPLACEAPPVIEW_METHODS() \
	STD_OLEINPLACEAPPVIEW_ACCESS(*OleInPlaceAppView) \
protected: \
	VOleInPlaceAppView	*OleInPlaceAppView;

#endif // _VOLEINPLACEAPPVIEW_H_


