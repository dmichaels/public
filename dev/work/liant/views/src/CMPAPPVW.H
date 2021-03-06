// $Header:   Q:/views/win/vcs/CMPAPPVW.H_v   1.3   Mar 04 1997 14:11:52   gregm  $

//	cdappveiw.h
//
//  Compound Document AppView
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
//	01/30/95 dss	Initial release.
//  12/08/95 dss    added VComponentAppViewCls. Name change: COMPONENTAPPVIEW_H.
//					added default ctor.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#ifndef COMPONENTAPPVIEW_H
#define COMPONENTAPPVIEW_H

// Views includes
#include "stack.h"

// Views Compound Document Types
#include "oledefs.h"

// OLE Includes
#include "unknown.h"
#include "oipappvw.h"

// Views Compound Document Includes
#include "cmpview.h"

//#include "oipframe.h"
#include "oipappvw.h"

CLASS VComponentAppView : public VComponentView
{
public:	

	VComponentAppView();

	// construct an outer Unknown VView
	VComponentAppView(LPVAPPVIEW lpAppView, VPopupMenu *editMenu,
			   VString *path = 0);
	~VComponentAppView();

	VClass		*iam();

	LPTSTR		getClassName() {return _T("VComponentAppView");}

	// *** Inherited Interfaces (no ';' follows these macros) ***
	CONTAIN_INTERFACE(OLEINPLACEAPPVIEW, OleInPlaceAppView)

public:	

	// conversions
	operator LPVAPPVIEW()		{return getVThis();}

	// operations
	LPVAPPVIEW	getVThis()		{return (LPVAPPVIEW) getVObject();}
	LPVVIEW		getVSuperThis()	{return (LPVVIEW) getVObject();}
	LPVAPPVIEW	getVAppView()	{return (LPVAPPVIEW) getVObject();}
	VComponentAppView	*getCDParent()	{return (VComponentAppView*) VComponentWindow::getCDParent();}

	// Notifications
	boolean			itemActivate(LPVCOMPONENTITEM lpItem);
	boolean			itemDeactivate(LPVCOMPONENTITEM lpItem);
	boolean			activate(VWindow *w);
	boolean			deactivate(VWindow *w);

	boolean			editMenuSelected(VPopupMenu *editMenu);

//	LPVDROPWINDOW	getFocusedDropWindow();

	static void		OleStart(HINSTANCE hInstance);
	static void		OleStop(void);

	// misc.
	void			loseFocus();

	// mouse movement
	int				getDragDelay();
	void			setDragDelay(int nDragDelay);
	int				getDragMinDist();
	void			setDragMinDist(int nMinDist);
	int				getScrollDelay();
	void			setScrollDelay(int nScrollDelay);
	int				getScrollInset();
	void			setScrollInset(int nScrollInset);
	int				getScrollInterval();
	void			setScrollInterval(int nScrollInterval);

	void			setMenuBar(VMenu *m);
	void			setMenuBar(HMENU m);
//	void			saveMenuBar();
//	VMenu			*restoreMenuBar();
	void			restoreMenuBar();

	STDMETHODIMP	setToolSpace(LPCBORDERWIDTHS lpToolSpace);
	STDMETHODIMP	adjustMdiClient(LPCBORDERWIDTHS lpToolSpace);

	virtual boolean	createStorage(VComponentObject *storageParent = 0,
								  VString *lpStorageName = 0);
										  
	boolean			closeMenuItem(VMenuItem *);

protected:
	VObject			*lpCurrentOleObjectVerbMenu;
	
	VMenuItem		*lpCloseMenuItem;

	VPopupMenu		*lpFileMenu;
	VPopupMenu		*lpEditMenu;

	static boolean	bOleInitialized;
	
	// mouse related data
	int 			nDragDelay;
	int 			nDragMinDist;
	int 			nScrollDelay;
	int 			nScrollInset;
	int				nScrollInterval;

//	VStack			menuBars;

private:
	void			init(VPopupMenu *editMenu);
};

// ---------------------------------------------------------------------------
//
inline int VComponentAppView::getDragDelay() {return nDragDelay;}

// ---------------------------------------------------------------------------
//
inline void VComponentAppView::setDragDelay(int nDragDelay)
		{this->nDragDelay = nDragDelay;}

// ---------------------------------------------------------------------------
//
inline int VComponentAppView::getDragMinDist() {return nDragMinDist;}

// ---------------------------------------------------------------------------
//
inline void VComponentAppView::setDragMinDist(int nMinDist)
		{this->nDragMinDist = nDragMinDist;}

// ---------------------------------------------------------------------------
//
inline int VComponentAppView::getScrollDelay() {return nScrollDelay;}

// ---------------------------------------------------------------------------
//
inline void VComponentAppView::setScrollDelay(int nScrollDelay)
		{this->nScrollDelay = nScrollDelay;}

// ---------------------------------------------------------------------------
//
inline int VComponentAppView::getScrollInset() {return nScrollInset;}

// ---------------------------------------------------------------------------
//
inline void VComponentAppView::setScrollInset(int nScrollInset)
		{this->nScrollInset = nScrollInset;}

// ---------------------------------------------------------------------------
//
inline int VComponentAppView::getScrollInterval() {return nScrollInterval;}

// ---------------------------------------------------------------------------
//
inline void VComponentAppView::setScrollInterval(int nScrollInterval)
		{this->nScrollInterval = nScrollInterval;}

// ---------------------------------------------------------------------------
//
#if 0
inline void VComponentAppView::saveMenuBar()
{
	menuBars.push(getVThis()->getMenu());
}
#endif

extern VClass	*VComponentAppViewCls;

#endif // COMPONENTAPPVIEW_H

