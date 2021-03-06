// $Header:   Q:/views/common/vcs/DRAG.H_v   1.1   Mar 04 1997 14:12:28   gregm  $ 

//	drag.h
//
//	VDragManager, VDragHandler, and VDragItem interface
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
// --------------------------------------------------------------------------

#ifndef VDRAG_h
#define VDRAG_h

#include "global.h"
#include "archiver.h"

extern VClass *VDragManagerCls;

CLASS VDragManager;
CLASS VDragManagerRep;
CLASS VDragItem;
CLASS VDragHandler;
CLASS VWindow;
CLASS VMouseButton;
CLASS VMouseCursor;

extern VDragManager CV_NEARDATA g_DragManager;

#define GLOBALdragmanager 6

// VDragManager is a global memory class that handles dragging
// from views within a single application

CLASS VDragManager : public VGlobal {
public:
   VDragManager();
#ifdef DLL
   VDragManager(boolean dll);
#endif
   ~VDragManager();
   VClass *iam();

   VDragManager* operator->();

   void trackDrag(VWindow* a_pWindow, VDragItem* a_pDragItem);

   void addHandler(VDragHandler* a_dragHdl, VWindow* a_window);
   void removeHandler(VDragHandler* a_dragHdl, VWindow* a_window);
   void removeHandlers(VWindow* a_window);  // Remove all handlers assoc. w/this window
   void setneverDropCursor(VMouseCursor * pm = 0);    // set cursor used when start dragging

private:
   VDragManagerRep *m_pDragManagerRep;
};

// VDragHandler is an base class for handling default dragging
// behavior. This should be subclassed to change the default behavior

CLASS VDragHandler {
public:
   enum DropMode { neverDrop, noDrop, drop };

   VDragHandler();
   virtual ~VDragHandler();

   void trackContent(boolean tf) { m_trackContent = tf; }
   boolean trackContent() { return m_trackContent; }

   // these methods may rarely be overridden
   virtual DropMode inWindow(VWindow* a_pWindow, VDragItem* a_dragItem, int mx, int my);
   virtual boolean  dropWindow(VWindow* a_pWindow, VDragItem* a_dragItem);

   // this method may optionally be overridden
   virtual void enterHandler(VWindow* a_pWindow, VDragItem* a_dragItem);
   virtual void leaveHandler(VWindow* a_pWindow, VDragItem* a_dragItem);
   virtual boolean enterWindow(VWindow* a_pWindow, VDragItem* a_dragItem);
   virtual void leaveWindow(VWindow* a_pWindow, VDragItem* a_dragItem);
   virtual void hiliteWindow(VWindow* a_pWindow, boolean bHilite);

protected:
   // this method may optionally be overridden
   virtual void enterTrack(VWindow* a_pWindow, VDragItem* a_dragItem);
   virtual void leaveTrack(VWindow* a_pWindow, VDragItem* a_dragItem);
   virtual boolean inTrack(VWindow* a_pWindow, VDragItem* a_dragItem, int mx, int my);

   // these methods should always be overridden
   virtual boolean acceptDrag(VWindow* a_pWindow, VDragItem* a_dragItem) = 0;
   virtual boolean acceptDrop(VWindow* a_pWindow, VDragItem* a_dragItem) = 0;

   VMouseCursor *m_copyCursor;
   VMouseCursor *m_moveCursor;
   VMouseCursor *m_noDropCursor;
   VMouseCursor *m_neverDropCursor;

   friend VDragManagerRep;
private:
   boolean m_canAcceptDrag;
   boolean m_canAcceptDrop;
   boolean m_inContent;
   boolean m_trackContent;

};

#define DROPMODE  VDragHandler::DropMode
#define DROP      VDragHandler::drop
#define NODROP    VDragHandler::noDrop
#define NEVERDROP VDragHandler::neverDrop

// VDragItem is a container for drag item.
// this should be similar to clipboard/archive data

CLASS VDragItem {
public:
   VDragItem();
   ~VDragItem();

   enum DragMode { Move, Copy };

   boolean  contains(VClass *cls);
   boolean  isEmpty();
   boolean  clear();

#ifndef CV_NOARCHIVER
   VObject *get();
   boolean  put(VObject *data, boolean archive = TRUE);
#endif

   VClass  *getClass() { return m_itemCls; }

   VWindow* getSourceWin() { return m_pSourceWin; }
   boolean isArchived() { return m_archived; }
   boolean hasLeftSenderWindow() { return m_hasLeftSenderWindow; }
   boolean insideSenderWindow()  { return m_insideSenderWindow; }
   DragMode getDragMode() { return m_mode; }

   friend VDragManagerRep;
private:
   VClass   *m_itemCls;  // item class
   VObject  *m_itemData; // item data

#ifndef CV_NOARCHIVER
   VArchiver m_archiver; // memory archiver for objects
#endif


   boolean  m_archived;  // archive flag

   // need to add drop/no drop cursor support

   VWindow* m_pSourceWin;  // source window
   boolean  m_hasLeftSenderWindow;
   boolean  m_insideSenderWindow;
   DragMode m_mode;

// Cursors may optionally be defined, overriding the defaults

};

#endif // VDRAG_h
