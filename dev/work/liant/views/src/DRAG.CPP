// $Header:   Q:/views/common/vcs/drag.cpv   1.3   20 Mar 1997 17:13:28   DAVIDSM  $ 

//	drag.cpp
//
//	VDragManager, VDragHandler, and VDragItem implementation
//
//	Allegris Foundation 1.1.00 for Windows and OS/2 Presentation Manager
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
//	06/23/95 dgm	Original check-in; original code from kerstinb.
//	01/07/97 djs	OS/2 specific changes in methods: 
//                    VDragManagerRep::VDragManagerRep
//                    VDragManagerRep::whichWin
//                    VDragHandler::inWindow
//                    VDragHandler::hiliteWindow
//                    VDragManagerRep::setneverDropCursor (memory leak also fixed)
// --------------------------------------------------------------------------

#include "notifier.h"
#include "port.h"
#include "pen.h"
#include "rect.h"
#include "poppwndw.h"
#include "dictinry.h"
#include "iterator.h"
#include "mouscrsr.h"

#include "drag.h"

CLASS VDragManagerView;

// VDragManagerRep handles dragging
// from views within a single application

CLASS VDragManagerRep : public VObject
{
public:
   VDragManagerRep();
   ~VDragManagerRep();

   VClass *iam();

   void trackDrag(VWindow* a_pWindow, VDragItem* a_pDragItem);

   void addHandler(VDragHandler* a_dragHdl, VWindow* a_window);
   void removeHandler(VDragHandler* a_dragHdl, VWindow* a_window);
   void removeHandlers(VWindow* a_window); // Remove all handlers for this window
   void setneverDropCursor(VMouseCursor*);

private:
   void switchTarget(VWindow* a_pTarget);
   void switchHandler(VWindow* a_pTarget);
   boolean testHandler(VWindow* a_pTarget, VDragHandler* a_dragHdl);

   VDragItem*     m_pDragItem;   // drag item
   VDragHandler*  m_pHandler;    // current drag handler

   VWindow*       m_pSourceWin;  // source window
   VWindow*       m_pTargetWin;  // target window

   VOrdCollect    m_HandlerDict; // collection of window/handler pairs

   VMouseCursor  *m_copyCursor;
   VMouseCursor  *m_moveCursor;
   VMouseCursor  *m_noDropCursor;
   VMouseCursor  *m_neverDropCursor;

   VWindow*       whichWin(int mx, int my);
   boolean        mouseMv(int mx, int my);

   boolean        m_mouseMoved;  // mouse moved
   VDragManagerView *m_pView;

   friend VDragManagerView;
};

// VDragManagerView is a popup window that handles dragging
// from views within a single application

CLASS VDragManagerView : public VPopupWindow {
public:
   VDragManagerView() {};
   VDragManagerView(VDragManagerRep* a_pRep);
   ~VDragManagerView();

   VClass *iam();

   boolean  isHidden();

protected:
//   boolean  mouseUp(int mx, int my);
   boolean  mouseMv(int mx, int my, int bStat);

private:
   VDragManagerRep* m_pDragManagerRep;
};

CLASS VHandlerPair : public VObject {
public:
   VHandlerPair() {};
   VHandlerPair(VWindow *a_pWindow, VDragHandler *a_dragHdl);
   ~VHandlerPair() {};

   VClass *iam();

   HashKey hash() const;
   boolean isEqual(VObject *obj) const;

   friend VDragManagerRep;

private:
   VWindow*       m_pWindow;
   VDragHandler*  m_pHandler;    // current drag handler
};

defineClass(VDragManager, VGlobal)
defineClass(VDragManagerRep, VObject)
defineClass(VDragManagerView, VPopupWindow)
defineClass(VHandlerPair, VObject)


//*******************************************************************
//
//  VDragManager
//
//*******************************************************************

//-------------------------------------------------------------------
VDragManager::VDragManager()
{
	m_pDragManagerRep = 0;
}

#ifdef DLL
//-------------------------------------------------------------------
VDragManager::VDragManager(boolean dll)
	:	VGlobal(GLOBALdragmanager)
{                         
	m_pDragManagerRep = 0;
	if (dll) {
		// initialize
	}
}
#endif

//-------------------------------------------------------------------
VDragManager::~VDragManager()
{
   delete m_pDragManagerRep;
}

//-------------------------------------------------------------------
void VDragManager::trackDrag(VWindow* a_pWindow, VDragItem* a_pDragItem)
{
   if (m_pDragManagerRep)
      m_pDragManagerRep->trackDrag(a_pWindow, a_pDragItem);
}

//-------------------------------------------------------------------
void VDragManager::addHandler(VDragHandler* a_dragHdl, VWindow* a_pWindow)
{
   if (!m_pDragManagerRep)
      m_pDragManagerRep = new VDragManagerRep;

   m_pDragManagerRep->addHandler(a_dragHdl, a_pWindow);
}

//-------------------------------------------------------------------
void VDragManager::removeHandler(VDragHandler* a_dragHdl, VWindow* a_pWindow)
{
   if (m_pDragManagerRep)
      m_pDragManagerRep->removeHandler(a_dragHdl, a_pWindow);
}

//-------------------------------------------------------------------
void VDragManager::removeHandlers(VWindow* a_pWindow)
{
   if (m_pDragManagerRep)
      m_pDragManagerRep->removeHandlers(a_pWindow);
}

//-------------------------------------------------------------------
void VDragManager::setneverDropCursor (VMouseCursor * pm)
{
   if (m_pDragManagerRep)
     m_pDragManagerRep->setneverDropCursor(pm);
}


//-------------------------------------------------------------------
VDragManager* VDragManager::operator->()
{
#ifdef DLL
	VDragManager* n;
	if (!(n = (VDragManager*) lookup(GLOBALdragmanager))) {
		return this;
	}
	return n;
#else
	return this;
#endif
}

//*******************************************************************
//
//  VDragManagerRep
//
//*******************************************************************

//-------------------------------------------------------------------
VDragManagerRep::VDragManagerRep()
{
   m_pDragItem = 0;    // drag data
   m_pHandler = 0;     // drag handler

   m_pSourceWin = 0;   // source window
   m_pTargetWin = 0;   // target window
   m_mouseMoved = FALSE; // mouse move

  #ifndef PM
   m_copyCursor  =    new VMouseCursor("COPYONE");
   m_moveCursor  =    new VMouseCursor("MOVEONE");
   m_noDropCursor=    new VMouseCursor("NODROPONE");
   m_neverDropCursor= new VMouseCursor("NEVERDROP");
  #else
	// djs 01/07/97 - OS/2 Supported cursors.  The cursors
	// instantiated here with numeric constants should probably be
	// replaced by 'set' methods - ie. setCopyCursor(), etc.
	// These numeric constants may interfere with other resources
	// and should be changed later.
	m_copyCursor  =    new VMouseCursor( 2000 ); // "COPYONE"
	m_moveCursor  =    new VMouseCursor( 2001 ); // "MOVEONE"
	m_noDropCursor=    new VMouseCursor( 2002 ); // "NODROPONE"
	m_neverDropCursor= new VMouseCursor( 2003 ); // "NEVERDROP"
  #endif

   m_mouseMoved  = FALSE;  // mouse moved
   m_pView = 0;
}

//-------------------------------------------------------------------
VDragManagerRep::~VDragManagerRep()
{
   delete m_copyCursor;
   delete m_moveCursor;
   delete m_noDropCursor;
   delete m_neverDropCursor;

   m_HandlerDict.freeContents();
}

//-------------------------------------------------------------------
void VDragManagerRep::trackDrag(VWindow* a_pWindow, VDragItem* a_pDragItem)
{
   m_pDragItem = a_pDragItem;

   if (a_pWindow && m_pDragItem && m_HandlerDict.count()) {
      m_pSourceWin = a_pWindow;
      m_pTargetWin = 0;
      m_pHandler = 0;
      m_mouseMoved = FALSE;  // mouse moved

      m_pDragItem->m_hasLeftSenderWindow = FALSE;
      m_pDragItem->m_insideSenderWindow = FALSE;
      m_pDragItem->m_pSourceWin = m_pSourceWin;

      if (notifier->keyStatus(K_Meta1))
         m_pDragItem->m_mode = VDragItem::Copy;
      else
         m_pDragItem->m_mode = VDragItem::Move;

      // switch to new target
      switchTarget(m_pSourceWin);

      // create drag manager view
      m_pView = new VDragManagerView(this);

      // capture mouse into drag manager view
      notifier->captureMouseFor(m_pView);
      notifier->mouseTracking(TRUE);

      // loop until button up
      while (notifier->buttonStatus()) {
         notifier->yield();
      }

      // uncapture mouse from drag manager view
      notifier->mouseTracking(FALSE);
      notifier->releaseCapture();

      // destroy drag manager view
      delete m_pView;

      // drop target
      if (m_mouseMoved) {
        boolean result = FALSE;
        if (m_pTargetWin && m_pHandler) {
           result = m_pHandler->dropWindow(m_pTargetWin, m_pDragItem);
        }

        if (!result) {
#ifdef WIN31
           MessageBeep(0);
#endif
        }
      }

      // switch to no target
      switchTarget(0);
   }
}

//-------------------------------------------------------------------
void VDragManagerRep::switchTarget(VWindow* a_pTarget)
{
   // notify old target
   if (m_pTargetWin) {
      if (m_pHandler)
         m_pHandler->leaveWindow(m_pTargetWin, m_pDragItem);

      if (m_pTargetWin == m_pSourceWin) {
         m_pDragItem->m_hasLeftSenderWindow = TRUE;
         m_pDragItem->m_insideSenderWindow = FALSE;
      }
   }

   m_pTargetWin = a_pTarget;

   // notify new target
   if (m_pTargetWin) {
      if (m_pTargetWin == m_pSourceWin) {
         m_pDragItem->m_insideSenderWindow = TRUE;
      }
   }

   switchHandler(m_pTargetWin);
}

//-------------------------------------------------------------------
void VDragManagerRep::switchHandler(VWindow* a_pTarget)
{
   // if no target, leave old handler
   if (!a_pTarget && m_pHandler) {
      m_pHandler->leaveHandler(a_pTarget, m_pDragItem);
      m_pHandler = 0;
      return;
   }

   // check against current handler
   if (m_pHandler) {
      VDragHandler* newHandler = 0;
      VHandlerPair temp(0, m_pHandler);

      // validate against current handler/window
      temp.m_pWindow = a_pTarget;
      if (m_HandlerDict.find((VObject *) &temp))
         newHandler = m_pHandler;

      // validate against generic handler/window
      if (!newHandler) {
         temp.m_pWindow = 0;
         if (m_HandlerDict.find((VObject *) &temp))
            newHandler = m_pHandler;
      }

      // test against handler
      if (newHandler && testHandler(a_pTarget, newHandler)) {
         return;
      }
   }


// Find a handler for this window ...
   DO (m_HandlerDict, VHandlerPair, pHandlerPair)
      if (pHandlerPair->m_pWindow == a_pTarget &&
          testHandler(a_pTarget, pHandlerPair->m_pHandler)) {
         return;
      }
   END

// If didn't find a handler for this window,
// look for a general 0-window handler
   DO (m_HandlerDict, VHandlerPair, pHandlerPair)
      if (pHandlerPair->m_pWindow == 0 &&
          testHandler(a_pTarget, pHandlerPair->m_pHandler)) {
         return;
      }
   END

// If didn't find a handler for this window,
// make sure last handler is removed
   if (m_pHandler) {
      m_pHandler->leaveHandler(a_pTarget, m_pDragItem);
      m_pHandler = 0;
   }

   return;
}

//-------------------------------------------------------------------
boolean VDragManagerRep::testHandler(VWindow* a_pWindow, VDragHandler* a_dragHdl)
{
   if (!a_dragHdl) {
      return FALSE;
   }

   if (m_pHandler != a_dragHdl) {
      // leave old handler
      if (m_pHandler) {
         m_pHandler->leaveHandler(a_pWindow, m_pDragItem);
         m_pHandler = 0;
      }

      // enter new handler
      a_dragHdl->enterHandler(a_pWindow, m_pDragItem);
   }

   // check window for drag
   if (a_dragHdl->enterWindow(a_pWindow, m_pDragItem)) {
      m_pHandler = a_dragHdl;
      return TRUE;
   }

   // leave new handler
   a_dragHdl->leaveHandler(a_pWindow, m_pDragItem);

   return FALSE;
}

//-------------------------------------------------------------------
VWindow* VDragManagerRep::whichWin(int mx, int my)
{
#if defined(CV_WINDOWS)
	POINT pt;
	pt.x = mx;
	pt.y = my;
	return notifier->getThis(WindowFromPoint(pt));
#elif defined(MOTIF)
	return notifier.whereIsMouse();
#elif defined(PM)
	POINTL pt;
	HWND hwnd;
	WinQueryPointerPos( HWND_DESKTOP, &pt );
	hwnd = WinWindowFromPoint( HWND_DESKTOP, &pt, TRUE );
	return notifier->getThis( hwnd );
#endif
}

//-------------------------------------------------------------------
boolean VDragManagerRep::mouseMv(int mx, int my)
{
   m_mouseMoved = TRUE;

   VWindow* newTarget = whichWin(mx, my);

   // if target changes
   if (m_pTargetWin != newTarget) {
      switchTarget(newTarget);
   }

   DROPMODE result = NEVERDROP;
   VMouseCursor* pCursor = m_neverDropCursor;

   if (m_pTargetWin) {
      m_pTargetWin->mapToWin(&mx, &my);
      if (m_pHandler)
         result = m_pHandler->inWindow(m_pTargetWin, m_pDragItem, mx, my);
   }

   if (m_pHandler)
   switch (result) {
      // use cursors of m_pHandler instead of m_pDragItem

      case DROP:
         if (m_pDragItem->m_mode == VDragItem::Move)
            if (m_pHandler->m_moveCursor)
                pCursor = m_pHandler->m_moveCursor;
            else
                pCursor = m_moveCursor;
         else
            if (m_pHandler->m_copyCursor)
                pCursor = m_pHandler->m_copyCursor;
             else
                pCursor = m_copyCursor;
         break;
      case NODROP:
         if (m_pHandler->m_noDropCursor)
             pCursor = m_pHandler->m_noDropCursor;
         else
             pCursor = m_noDropCursor;
         break;
      case NEVERDROP:
         if (m_pHandler->m_neverDropCursor)
             pCursor = m_pHandler->m_neverDropCursor;
         break;
   }

   m_pView->setCursor(pCursor);

   return FALSE;
}

//-------------------------------------------------------------------
void VDragManagerRep::addHandler(VDragHandler* a_dragHdl, VWindow* a_pWindow)
{
   VHandlerPair temp(a_pWindow, a_dragHdl);

   // add handler
   if (!m_HandlerDict.find((VObject *) &temp)) {
      m_HandlerDict.add(new VHandlerPair(a_pWindow, a_dragHdl));
   }
}

//-------------------------------------------------------------------
void VDragManagerRep::removeHandler(VDragHandler* a_dragHdl, VWindow* a_pWindow)
{
   VHandlerPair temp(a_pWindow, a_dragHdl);
   VObject *pHandler;

   pHandler = m_HandlerDict.find((VObject *) &temp);

   if (pHandler) {
      pHandler = m_HandlerDict.removeSame((VObject *) pHandler);
      delete pHandler;
   }
}

//-------------------------------------------------------------------
void VDragManagerRep::removeHandlers(VWindow* a_pWindow)
{
   VObject *pRemoved;

   DO (m_HandlerDict, VHandlerPair, pHandler)
      if (pHandler->m_pWindow == a_pWindow) {
         pRemoved = m_HandlerDict.removeSame((VObject *) pHandler);
         delete pRemoved;
      }
   END
}

//-------------------------------------------------------------------
void VDragManagerRep::setneverDropCursor (VMouseCursor * pm)
{
   if ( m_neverDropCursor )
      delete m_neverDropCursor;

   if ( pm )
      m_neverDropCursor = pm;
   else   {
     // reset to default cursor
	  #ifndef PM
     m_neverDropCursor= new VMouseCursor("NEVERDROP");
	  #else
	  m_neverDropCursor= new VMouseCursor( 2003 ); // "NEVERDROP"
	  #endif
   }
}

//*******************************************************************
//
//  VDragManagerView
//
//*******************************************************************

//-------------------------------------------------------------------
VDragManagerView::VDragManagerView(VDragManagerRep* a_pRep)
   :  VPopupWindow(VFrame(0, 0, 50, 50), 0, StyleHidden)
   ,  m_pDragManagerRep(a_pRep)
{
}

//-------------------------------------------------------------------
VDragManagerView::~VDragManagerView()
{
}

// this is needed to keep startModal from showing popup
//-------------------------------------------------------------------
boolean VDragManagerView::isHidden()
{
   return FALSE;
}

#if 0
//-------------------------------------------------------------------
boolean VDragManagerView::mouseUp(int mx, int my)
{
   if (m_pDragItem) {
      notifier->endModal(FALSE);
   }
   return FALSE;
}
#endif

//-------------------------------------------------------------------
boolean VDragManagerView::mouseMv(int mx, int my, int bStat)
{
   mapToScreen(&mx, &my);

   return m_pDragManagerRep->mouseMv(mx, my);
}

//*******************************************************************
//
//  VDragHandler
//
//*******************************************************************

//-------------------------------------------------------------------
VDragHandler::VDragHandler()
{
   m_canAcceptDrag = FALSE;
   m_canAcceptDrop = FALSE;
   m_inContent = FALSE;
   m_trackContent = FALSE;

   m_copyCursor = 0 ;
   m_moveCursor= 0 ;
   m_noDropCursor= 0 ;
   m_neverDropCursor= 0 ;
}

//-------------------------------------------------------------------
VDragHandler::~VDragHandler()
{
}

//-------------------------------------------------------------------
void VDragHandler::enterHandler(VWindow* a_pWindow, VDragItem* a_dragItem)
{
   m_canAcceptDrag = FALSE;
   m_canAcceptDrop = FALSE;
   m_inContent = FALSE;
}

//-------------------------------------------------------------------
void VDragHandler::leaveHandler(VWindow* a_pWindow, VDragItem* a_dragItem)
{
   m_canAcceptDrag = FALSE;
   m_canAcceptDrop = FALSE;
   m_inContent = FALSE;
}

//-------------------------------------------------------------------
boolean VDragHandler::enterWindow(VWindow* a_pWindow, VDragItem* a_dragItem)
{
   m_canAcceptDrag = acceptDrag(a_pWindow, a_dragItem);
   m_canAcceptDrop = m_canAcceptDrag;
   m_inContent = FALSE;

   return m_canAcceptDrag;
}

//-------------------------------------------------------------------
void VDragHandler::leaveWindow(VWindow* a_pWindow, VDragItem* a_dragItem)
{
   if (m_canAcceptDrag) {
      if (m_inContent) {
         if (m_trackContent)
            leaveTrack(a_pWindow, a_dragItem);

         if (a_dragItem->hasLeftSenderWindow())
            hiliteWindow(a_pWindow, FALSE);
      }
   }
   m_canAcceptDrag = FALSE;
   m_canAcceptDrop = FALSE;
   m_inContent = FALSE;
}

//-------------------------------------------------------------------
boolean VDragHandler::dropWindow(VWindow* a_pWindow, VDragItem* a_dragItem)
{
   boolean result = FALSE;

   if (m_canAcceptDrag && m_inContent) {
      if (m_trackContent)
         leaveTrack(a_pWindow, a_dragItem);

      if (a_dragItem->hasLeftSenderWindow())
         hiliteWindow(a_pWindow, FALSE);

      if (m_canAcceptDrop)
         result = acceptDrop(a_pWindow, a_dragItem);

      m_canAcceptDrag = FALSE;
      m_canAcceptDrop = FALSE;
      m_inContent = FALSE;
   }

   return result;
}

//-------------------------------------------------------------------
VDragHandler::DropMode VDragHandler::inWindow(VWindow* a_pWindow, VDragItem* a_dragItem, int mx, int my)
{
   DropMode dmode = neverDrop;

   if (!m_canAcceptDrag)
      return dmode;

   if (!a_dragItem)
      return dmode;

   // if point in client
   int x=0, y=0, w=0, h=0;
	#ifndef PM
   a_pWindow->getClientRel(&x, &y, &w, &h);
	#else
	 // djs 01/07/97 - VWindow::getClient??? does not
	 // properly return the client area with regard to
	 // scroll bars on OS/2.  May need to add this code to
	 // the VWindow::getClient??? methods.
	 HWND clientHwnd = WinWindowFromID(a_pWindow->framehwnd(), FID_CLIENT);
	 SWP swp;
	 WinQueryWindowPos( clientHwnd, &swp );
	 w = swp.cx;
	 h = swp.cy + swp.y;
  #endif

   VRectangle rect(CornerDim, x, y, w, h);

   if (rect.pointIn(mx, my)) {
      if (!m_inContent) {
         if (a_dragItem->hasLeftSenderWindow())
            hiliteWindow(a_pWindow, TRUE);

         if (m_trackContent)
            enterTrack(a_pWindow, a_dragItem);

         m_inContent = TRUE;
      }
   }
   else {
      if (m_inContent) {
         if (m_trackContent)
            leaveTrack(a_pWindow, a_dragItem);

         if (a_dragItem->hasLeftSenderWindow())
            hiliteWindow(a_pWindow, FALSE);

         m_inContent = FALSE;
      }
   }

   if (m_inContent)
      dmode = drop;

   if (m_inContent && m_trackContent) {
      m_canAcceptDrop = inTrack(a_pWindow, a_dragItem, mx, my);
      if (!m_canAcceptDrop)
         dmode = noDrop;
   }

   return dmode;
}

//-------------------------------------------------------------------
// outset hilite if control, inset hilite if window
void VDragHandler::hiliteWindow(VWindow* a_pWindow, boolean bHilite)
{
   if (!a_pWindow)
      return;

   VWindow *pwin;
   if (a_pWindow->isA(VControlCls)) {
      pwin = a_pWindow->getParent();
   }
   else {
      pwin = a_pWindow;
   }

   if (pwin != 0) {
      // draw shadow
      VPort port(pwin);
      VPen  hiPen(VColor::White);

      port.open();

      int x, y, w, h;

      VRectangle rect;

      // if control
      if (pwin != a_pWindow) {
         a_pWindow->getRel(&x, &y, &w, &h);
         rect.set(CornerDim, x, y, w, h);
      }
      else {
		  #ifndef PM
         a_pWindow->getClientRel(&x, &y, &w, &h);
         rect.set(CornerDim, x+1, y+1, w-1, h-1);
		  #else
			// djs 01/07/97 - VWindow::getClient??? does not
			// properly return the client area with regard to
			// scroll bars on OS/2.  May need to add this code to
			// the VWindow::getClient??? methods.
			HWND clientHwnd = WinWindowFromID(a_pWindow->framehwnd(), FID_CLIENT);
			SWP swp;
			WinQueryWindowPos( clientHwnd, &swp );
			rect.set(CornerDim, 1, 1, (int)swp.cx - 2, (int)swp.cy - 2);
		  #endif
      }

      // if control
      if (pwin != a_pWindow) {
         rect.inset(-3, -3);
      }

      hiPen.width(2);
      port.usePen(&hiPen);
      port.rule(XorRule);
      port.frameRegion(&rect);
      port.usePen(0);
      port.close();
   }
}

//-------------------------------------------------------------------
void VDragHandler::enterTrack(VWindow* a_pWindow, VDragItem* a_dragItem)
{
}

//-------------------------------------------------------------------
void VDragHandler::leaveTrack(VWindow* a_pWindow, VDragItem* a_dragItem)
{
}

//-------------------------------------------------------------------
boolean VDragHandler::inTrack(VWindow* a_pWindow, VDragItem* a_dragItem, int mx, int my)
{
   return TRUE;
}

//-------------------------------------------------------------------
boolean VDragHandler::acceptDrag(VWindow* a_pWindow, VDragItem* a_dragItem)
{
   return FALSE;
}

//-------------------------------------------------------------------
boolean VDragHandler::acceptDrop(VWindow* a_pWindow, VDragItem* a_dragItem)
{
   return FALSE;
}

//*******************************************************************
//
//  VDragItem
//
//*******************************************************************
//-------------------------------------------------------------------
VDragItem::VDragItem()
//-------------------------------------------------------------------
{
   m_itemCls = 0;
   m_itemData = 0;
   m_archived = FALSE;
   m_pSourceWin = 0;

   m_hasLeftSenderWindow = FALSE;
   m_insideSenderWindow = FALSE;
   m_mode = Move;

}

//-------------------------------------------------------------------
VDragItem::
~VDragItem()
//-------------------------------------------------------------------
{
}

//-------------------------------------------------------------------
boolean VDragItem::
contains(VClass *cls)
//-------------------------------------------------------------------
{
   return (m_itemCls && m_itemCls->isa(cls)) ? TRUE : FALSE;
}

//-------------------------------------------------------------------
boolean VDragItem::
isEmpty()
//-------------------------------------------------------------------
{
   return (m_itemCls == 0 && m_itemData == 0) ? TRUE : FALSE;
}

//-------------------------------------------------------------------
boolean VDragItem::
clear()
//-------------------------------------------------------------------
{
   m_itemCls = 0;
   m_itemData = 0;
   m_archived = FALSE;

   m_hasLeftSenderWindow = FALSE;
   m_insideSenderWindow = FALSE;

   return TRUE;
}

// ===========================================================================
#ifndef CV_NOARCHIVER

//-------------------------------------------------------------------
boolean VDragItem::put(VObject *data, boolean archive)
{
   m_itemCls = 0;
   m_itemData = 0;
   m_archived = archive;

   if (!data)
      return FALSE;

   m_itemCls = data->getIsA();
   if (!m_itemCls)
      return FALSE;

   if (m_archived) {
      m_archiver.rewind();
      return m_archiver.putObject(data);
   }
   else {
      m_itemData = data;
      return TRUE;
   }
}

//-------------------------------------------------------------------
VObject *VDragItem::get()
{
   if (!m_itemCls)
      return 0;

   if (m_archived) {
      m_archiver.rewind();
      return m_archiver.getObject(NIL);
   }
   else {
      return m_itemData;
   }
}

#endif
// ===========================================================================

//*******************************************************************
//
//  VHandlerPair
//
//*******************************************************************

//-------------------------------------------------------------------
HashKey VHandlerPair::hash() const
{
   return (long) m_pWindow & HASH_KEY_MASK;
}

//-------------------------------------------------------------------
boolean VHandlerPair::isEqual(VObject *obj) const
{
   if (!obj->isA(VHandlerPairCls)) {
      return FALSE;
   }
   else {
   // providers match and any dependent or dependents match
      if (m_pWindow == ((VHandlerPair *) obj)->m_pWindow &&
          (((VHandlerPair *) obj)->m_pHandler == 0 ||
          m_pHandler == ((VHandlerPair *) obj)->m_pHandler)) {
         return TRUE;
      }

      return FALSE;
   }
}

//-------------------------------------------------------------------
VHandlerPair::VHandlerPair(VWindow *a_pWindow, VDragHandler *a_dragHdl)
   :  m_pWindow(a_pWindow)
   ,  m_pHandler(a_dragHdl)
{
}
