// $Header:   Q:/views/win/vcs/cmpitem.h_v   1.12   Jun 03 1997 09:51:38   scottt  $

//      cmpitem.h
//
//  Compound Document Item
//
//      Allegris Foundation 1.1.00 for Microsoft Windows
//      Copyright (c) 1997 by INTERSOLV, Inc.
//      +-----------------------------------------------------------------+
//      | This product is the property of INTERSOLV, Inc. and is licensed |
//      | pursuant to a written license agreement.  No portion of  this   |
//      | product may be reproduced without the written permission of     |
//      | INTERSOLV, Inc. except pursuant to the license agreement.       |
//      +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  12/09/94 dss    Initial release.
//  12/08/95 dss    allowLocalActivation(): return boolean. Name
//                                      change: VCOMPONENTITEM_H.
//      01/09/96 dss    VComponent name change.
//      01/22/96 dss    Added onClipboard data member.
//      01/30/96 dss    added includes.
//      01/31/96 dss    removed insertSelf(). Replaced static create()'s with
//                                      realize(). Replace different item attribute enums with
//                                      ItemStyle enum. InitObject changed to initFromNative.
//
//      05/10/96 evc    Added some changes from Ray.  Added new constructor.
//      05/21/96  evc   Added function to return IStorage pointer, ::getStg
//  06/05/96 evc        Added uponViewAdvise called thru IadviseSink::OnViewChange
//                  also
//                                      VComponentItem::sizeCtoV()
//                                      VComponentItem:: uponInplaceObjectMoveOrResize()
//                                      VComponentItem::uponDataAdvise()
//  11/25/96 mlw    Added fixes from Sanjay Chopra and support for VMoney
//                  by adding include of money.h.
//      ---------------------------------------------------------------------------
#ifndef VCOMPONENTITEM_H
#define VCOMPONENTITEM_H
#include "archiver.h"
#include "rect.h"
#include "window.h"
#include "pathstr.h"

#include "cmpwin.h"

#include "advsink.h"
#include "oclsite.h"
#include "oipsite.h"
#include "dropsrc.h"
#include "oobject.h"
#include "money.h"

extern VClass *VComponentItemCls;

typedef enum {
    ItemShortName               = 1 << 0,
    ItemLongName                = 1 << 1
} ItemNameType;

typedef enum {
    LinkedItem                  = 1 << 0,
    EmbeddedItem                = 1 << 1,
    StaticItem                  = 1 << 2,

    FileSource                  = 1 << 3,
    EmptySource                 = 1 << 4,
    DataSource                  = 1 << 5
} ItemStyle;

CLASS VComponentItem : public VComponentWindow
{
public:

	VComponentItem();

	// construct a VComponentItem from an IDataObject.
	VComponentItem(const LPDATAOBJECT lpDataObj,
				   const VComponentWindow& CDParent,
				   VWindow& win,
				   int itemStyle = EmbeddedItem,
				   boolean isActiveXControl = FALSE);

	VComponentItem(const LPDATAOBJECT lpDataObj,
				   const VFrame& frame,
				   VComponentWindow& CDParent,
				   const VStyle& style = StyleDefault,
				   int itemStyle = EmbeddedItem,
				   boolean isActiveXControl = FALSE);

	// construct a VComponentItem from storage.
	VComponentItem(const VPathString& storage,
				   VComponentWindow& CDParent,
				   VWindow& win,
				   int itemStyle = EmbeddedItem,
				   boolean isActiveXControl = FALSE);

	VComponentItem(const VPathString& storage,
				   const VFrame& frame,
				   VComponentWindow& CDParent,
				   const VStyle& style = StyleDefault,
				   int itemStyle = EmbeddedItem,
				   boolean isActiveXControl = FALSE);

	// construct an empty VComponentItem from an Object type.
	// Note, CDParent not used.
	VComponentItem(const VString& objectType,
				   VComponentWindow& CDParent,
				   VWindow& win,
				   boolean isActiveXControl = FALSE);

	// construct a VComponentItem
	VComponentItem(const VString& objectType,
				   VWindow& win,
				   boolean isActiveXControl = FALSE);

	VComponentItem(const VString& objectType,
				   const VFrame& frame,
				   VComponentWindow& CDParent,
				   const VStyle& style = StyleDefault,
				   boolean isActiveXControl = FALSE);

    ~VComponentItem();

    VClass  *iam();

    LPSTORAGE       getStg(){
		return m_lpStorage;
	}

    LPTSTR          getClassName() {return _T("VComponentItem");}

    // Pseudo Inherited Interfaces (no ';' follows these macros)
    CONTAIN_INTERFACE1(C,       AdviseSink)
    CONTAIN_INTERFACE1(C,       OleClientSite)
    CONTAIN_INTERFACE1(C,       OleInPlaceSite)
    CONTAIN_INTERFACE1(C,       DropSource)
    CONTAIN_INTERFACE2(CR,      OleObject)

public:

	// realize from the registry
    boolean                                             realize(const VString& objectType);

	// realize from an Object Courier
    boolean                                             realize(LPDATAOBJECT lpDataObj,
										int itemStyle = EmbeddedItem);

	// realize from storage
    boolean                                             realize(const VPathString& storage,
										int itemStyle = EmbeddedItem);

    boolean                                             isRealized(void);

    boolean                                             initFromNative(LPOLEOBJECT pOleObject, int itemStyle);

    // object commands
    boolean                                     closeItem(boolean bSave);
    boolean                                     unloadItem(boolean bSave);
    boolean                                     doVerb(int iVerb, MSG FAR *lpMsg = 0);

    DWORD                                               getAspect();
    void                                                setAspect(DWORD dwDrawAspect);

    VWinRect&                           bounds();
    VWinRect&                           setBounds(VWinRect& r);

    VOleSize&                                   getSize() {return objBounds.getSize();}
    VOlePosition&                               getPosition() {return objBounds.getPosition();}

    // size/position operations
    VWinRect&           move(const VOlePosition& pos);
    VWinRect&           size(const VOleSize& size);
	void                            sizeCtoV(); // resizes
	// an in-place object to the dementions of its parent
	// window (the itemwindow retrived with getVThis())


	virtual void             uponViewAdvise();
	virtual void             uponDataAdvise();
	virtual void             uponInplaceObjectMoveOrResize();


    // menu callbacks
    boolean             oleVerbMenu(VMenuItem *mitem);

    // focus
    boolean             givenFocus(void);
    boolean             clearFocus(void);
    void                drawFocus(void);
    void                undrawFocus(void);

    // ClipBoard
    void                copyToClipboard(void);
    void                deleteFromClipboard(void);

    typedef enum ActivationStates {
	inactive,
	locallyActive,
	remotelyActive
    } ActivationStates;

    // Activation
    boolean                             allowLocalActivation(boolean bAllow);
    boolean                             canLocallyActivate(void);
    boolean                             isRemotelyActive(void);
    boolean             isLocallyActive(void);
    boolean                             isActive(void);
    void                                setActivationState(VComponentItem::ActivationStates state);
    ActivationStates    getActivationState();

    // event callbacks
    boolean             paint();
	// Sanjay Start
    boolean             moved(int x, int y);
	// Sanjay End
    boolean             resized(int w, int h);
    boolean             mouseDbl(int x, int y);
    boolean             mouseDn(int x, int y);
    boolean             mouseUp(int x, int y);
    boolean             mouseMv(int x, int y, int bDown);
    boolean             close();
    boolean             quit();

    // Object Types
    boolean             isLinkedItem() {return itemStyle & LinkedItem;}
    boolean             isEmbeddedItem() {return itemStyle & EmbeddedItem;}
    boolean             isStaticItem() {return itemStyle & StaticItem;}

    boolean             isFromFileSource()  {return itemStyle == FileSource;}
    boolean             isFromEmptySource() {return itemStyle == EmptySource;}
    boolean             isFromDataSource()  {return itemStyle == DataSource;}

    boolean             canBeLinkedItem() {return bCanBeLinkedItem;}
    boolean             canBeEmbeddedItem() {return bCanBeEmbeddedItem;}
    boolean             canBeStaticItem() {return bCanBeStaticItem;}

    boolean             canBeEmptyItem(void)  {return bCanBeEmptyItem;}
    boolean             canBeFileItem(void) {return bCanBeFileItem;}
    boolean             canBeDataItem(void)  {return bCanBeDataItem;}
// Start of Sanjay's Code
    boolean             isActiveXControl() {return bIsActiveXControl;}
// End of Sanjay's Code

    void                allowLinkedItems(boolean allow) {bCanBeLinkedItem = allow;}
    void                allowEmbeddedItems(boolean allow) {bCanBeEmbeddedItem = allow;}

    void                allowEmptyItems(boolean allow) {bCanBeEmptyItem = allow;}
    void                allowFileItems(boolean allow) {bCanBeFileItem = allow;}
    void                allowDataItems(boolean allow) {bCanBeDataItem = allow;}

    const VString&      getName(ItemNameType nameType) const
			{ if (ItemShortName == nameType) return itemShortName;
			  else if (ItemLongName == nameType) return itemLongName;
			  else cv_assert(0); return itemShortName;
			}

    REFCLSID            getClassID(void) const {return classId;}

    // Release/destructor hack
    void                destroy(void);

    // notifications
//  void activationNotify( boolean bActive, boolean bIPActive);

    // misc.
    VObject         *addVerbs(VPopupMenu *theMenu, int iPosition);

    boolean         createStorage(void);
    boolean         disconnectStorage(void);
    boolean         connectStorage(void);

	// Misc.
	void                    setWindow(VWindow       &win) {lpVObject = &win;}

    // Archiving
#ifndef CV_NOOLEARCHIVER
    void            putTo(VArchiver& a);
    static void     putToX(VObject& obj, VArchiver& a,
			   VArchiver::AtMethodGetType, VArchiver::AtMethodSetType);
    void            getFrom(VArchiver& a);
    void            getFrom(VArchiver& a, VObject *, boolean);
    static void     getFromX(VObject& obj, VArchiver& a,
			     VArchiver::AtMethodGetType, VArchiver::AtMethodSetType);
#endif

protected:
    DWORD           dwDrawAspect;

//  RECTL           bounds;     // bounds in Himetric
    // site bounds data is managed by the VWindow class
//  VWinRect        siteBounds; // item bounds inside its view
    VWinRect        objBounds;  // object bounds inside its site

//  SIZEL           sizel;
//  POINTL          posl;

    // Drag Support
    boolean         bLocalDrag;     // is doc source of the drag
    boolean         bPendingDrag;   // LButtonDown--possible drag pending
    boolean         bDragLeave;     // has drag left
    POINT           ptButDown;      // LButtonDown coordinates
    DWORD           DoDragDrop (void);

    // Activation
    ActivationStates activationState;       // user interface activation states (not object states)
    boolean         bCanActivateLocally;    // object can locally activate

    VTimer          *timer;

//  virtual void    createOleClientSite(void);
//  virtual void    createOleInPlaceSite(void);

    boolean         dragDelayTimeout();

    int             getDragDelay();
    int             getDragMinDist();
    int             getScrollDelay();
    int             getScrollInset();
    int             getScrollInterval();

    void            DrawOleObjectBorder(boolean erase, boolean drawHandles);

    VObject         *lpOleObjectVerbMenu;

//  short               style;
    boolean         inFocus;

    ItemStyle           itemStyle;

    boolean         bCanBeLinkedItem;
    boolean         bCanBeEmbeddedItem;

    boolean         bCanBeEmptyItem;
    boolean         bCanBeFileItem;
    boolean         bCanBeDataItem;
    boolean         bCanBeStaticItem;
// Start of Sanjay's Code
	boolean                 bIsActiveXControl;
// End of Sanjay's Code

    DWORD           OleAdvisory;
    DWORD           DataAdvisory;

    VString         itemLongName;       // server's long name for item
    VString         itemShortName;      // server's short name for item

    CLSID           classId;            // item COM ClassID
	VPathString             m_componentArchiveFileName; // rhw 3/21/96: Tempory until we can save to ArchiverBytes.
												// Then we won't be saving to a file (hence, we won't need this variable)
												// but instead, we will be saving into the the applications archive file
	// RG: new begin
	LPSTREAM                m_lpStream;
	IPersistStream  *pIPersistStream;
	LPLOCKBYTES     m_lpLockBytes;
	// RG: new end

    LPSTORAGE       m_lpStorage;
	// rhw 3/21/96: I moved this local definition to here since I
	// did not understand how david was handling storage and I
	// can work around his approach with this member storage

	static VComponentItem   *onClipboard;

private:
    void init();

};

//**********************************************************************
// inline Helpers
//

inline DWORD        VComponentItem::getAspect() {return dwDrawAspect;}
inline void         VComponentItem::setAspect(DWORD dwDrawAspect)
		    {this->dwDrawAspect = dwDrawAspect;}

#if 0
inline const VWinRect& VComponentItem::getBounds() {
		    getPhysical().get(CornerDim, &x, &y, &w, &h)
		    }
inline void         VComponentItem::getSize(SIZEL& sizel)
		    {sizel.cx = bounds.right - bounds.left;
		     sizel.cy = bounds.bottom - bounds.top;}
inline void         VComponentItem::setSize(const SIZEL& sizel)
		    {bounds.bottom = bounds.top + sizel.cy;
		     bounds.right = bounds.left + sizel.cx;}
inline void         VComponentItem::getPos(POINTL& posl)
		    {posl.x = bounds.left; posl.y = bounds.top;}
inline void         VComponentItem::setPos(const POINTL& posl)
		    {bounds.top = posl.y;
		     bounds.left = posl.x;}
inline void         VComponentItem::setBounds(const RECTL& boundsl)
		    {this->bounds = boundsl;}
inline void         VComponentItem::getBounds(RECTL& boundsl)
		    {boundsl = this->bounds;}
#endif

inline boolean      VComponentItem::canLocallyActivate(void) {return bCanActivateLocally;}
inline boolean          VComponentItem::allowLocalActivation(boolean bAllow)
		    {boolean tmp = bCanActivateLocally;
		     bCanActivateLocally = bAllow;
		     return tmp;}
inline boolean      VComponentItem::isRemotelyActive(void) {return activationState == remotelyActive;}
inline boolean      VComponentItem::isLocallyActive(void) {return activationState == locallyActive;}
inline boolean      VComponentItem::isActive(void)
		    {return (activationState == locallyActive) ||
			    (activationState == remotelyActive);}
inline VComponentItem::ActivationStates    VComponentItem::getActivationState() {return activationState;}

#endif // VCOMPONENTITEM_H
