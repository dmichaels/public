// $Header:   Q:/views/common/vcs/VTLISTBX.H_V   1.45   Mar 24 1997 16:14:28   scottt  $ 

//  vtlistbx.h
//
//  VTreeListBox interface [Common]
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
//  07/21/93 jmd    added row hilite
//  07/24/93 jmd    changed to use VString wherever possible
//  08/30/93 jmd    added getTree()
//  08/31/93 jmd    added inGotoNode flag
//  02/04/94 dgm    minor Motif changes.
//  06/23/95 dgm    Original check-in; original code from jmd.
//  07/14/95 pkt    fixed for platforms without drag and drop.
//  09/20/95 dss    VStyle changes.
//  02/21/96 tlf    Updated to compile on HPUX.
//  04/30/96 glm    added archiving
//  09/12/96 pkt	updated for WebShop.
//  09/12/96 pkt	updated for WebShop.
//  01/08/97 djs	Changed ::scrollTree to be PUBLIC
//	02/04/97 pkt	Implemented 'getClassDefaultStyle'.
// ---------------------------------------------------------------------------

#ifndef VTLISTBX_H
#define VTLISTBX_H
#include "view.h"
#include "vtree.h"

#ifndef CV_NO_DRAG_AND_DROP
#include "drag.h"
#endif

CLASS VEditLineNotify;
CLASS VFont;
CLASS VImage;
CLASS VMouseCursor;
CLASS VOrdCollect;
CLASS VPopupMenu;
CLASS VTreeNode;
CLASS VTableView;
CLASS VTableSubView;

extern VClass *VTreeListBoxCls;

#ifdef CV_HPUX
extern const Color &VTreeListBox_color_init;
#endif

CLASS VTreeListBox : public VView {
public:
   enum HiliteState { OnNode, BetweenNode };

   VTreeListBox(const VFrame& f,
                            VWindow *pwin,
                            const VStyle& style = StyleDefault,
                            int minrowhgt = 1);

   VTreeListBox();
   virtual ~VTreeListBox();

   VClass *iam();

   void init();
   void setTreeListBox(int minrowhgt, const VStyle& style);

   void uponClick(VObject *clnt, method mthd, method dblmthd = 0,
                  method expmthd = 0, method rclickmthd = 0,
                  method updatescrollmthd = 0, method renamemthd = 0,
                  method prerenamemthd = 0);

   void             uponSelect(VObject *, method);
   void             uponDblClick(VObject *, method);
   void             uponRmouseClick(VObject *, method);
   void             uponExpand(VObject *, method);
   void             uponExpanded(VObject *, method);
   void             uponUpdateScroll(VObject *, method);
   void				uponNodeRename(VObject *, method);
   void				uponPreNodeRename(VObject *, method);


   boolean    key(int, char);
   void       setFont(VFont *);
   VFont	 *getFont() const;


   VTreeNode  *addNode(const VString& name, VTreeNode *node = 0,
                       VObject *data = 0, boolean isparent = TRUE,
                       VBitMap *pbmap = 0,

#ifndef CV_HPUX
                       const VColor &Color = 0ul,
#else
                       const VColor &Color = VTreeListBox_color_init,
#endif
                       VFont *pFont = 0,
                       boolean iscontainer = FALSE);

   boolean addNode(VTreeNode* node, VTreeNode* parent, VTreeNode* sibling = 0);

   VTreeNode  *getCurrNode();

   // Return a list of the currently selected nodes.
   VOrdCollect &getSelectedNodes();

   boolean    isNodeSelected(VTreeNode *node);

   VTreeNode  *findNode(const VString& n, boolean expand = TRUE);
   VTreeNode  *findNode(VObject *d, boolean expand = TRUE);

   void        gotoNode(VTreeNode *node, boolean nopaint = FALSE, boolean expand = FALSE, boolean fromClickAt = FALSE);
   void        expandNode(VTreeNode *node, boolean nonotify = FALSE);
   VTreeNode  *findNextNode(VTreeNode *node, boolean expand = TRUE);
   VTreeNode  *findPrevNode(VTreeNode *node, boolean expand = TRUE);

   void        renameNode(VTreeNode *node, const VString& newname);
   void        renameNode(VTreeNode *node);
   void        bitmapNode(VTreeNode *node, VBitMap *bitmap);
   void        colorNode(VTreeNode *node, const VColor& color);
   void        fontNode(VTreeNode *node, VFont *font);

   void        updateNode(VTreeNode *node, boolean justtext = TRUE, boolean paintbelow = FALSE);
   int         getNodeChildNo(VTreeNode *node);

   boolean     promoteNode(VTreeNode *node);
   boolean     demoteNode(VTreeNode *node);
   void        removeNode(VTreeNode *node);
   boolean     moveNode(VTreeNode *node, VTreeNode *prev);

   VTreeNode  *getNodeAt(int index, boolean expand = TRUE);
   VTreeNode  *getNodeAt(int mx, int my, HiliteState /* = OnNode */ );
   void        deleteNode(VTreeNode *node);
   void        deSelectNode(VTreeNode *node);

   void        setRefresh(boolean tf);
   void        reset();

   void        selectNodeAt(int n, boolean expand = TRUE);
   int         getNodeIndex(VTreeNode *node, boolean expand = TRUE);

   void        setTree(VTree *t);
   VTree      *getTree();

   boolean     getRowHighlight();
   void        setRowHighlight(boolean tf);

   boolean     getClickOptimized();
   void        setClickOptimized(boolean tf);

   boolean     getDragEnable();
   void        setDragEnable(boolean tf);

   boolean     getExpandEnable();
   void        setExpandEnable(boolean tf);

   boolean     getAutoRenameEnable();
   void        setAutoRenameEnable(boolean tf);

   virtual VObject* getObject(VTreeNode* overNode);

   void hiliteNode(VTreeNode *node, HiliteState state = OnNode);
   void setneverDropCursor(VMouseCursor *);
   int  getRowHeight();
   int  getRowOffset() {return rowOffset;}
   boolean     popupMenuAt(VPopupMenu *pPopup);

   boolean     getCheckExpandCallback();
   void        setCheckExpandCallback(boolean var);

   boolean     rmouseDn(int mx, int my);
   boolean     mouseDn(int mx, int my);
   boolean     mouseDbl(int mx, int my);
   boolean     mouseUp(int mx, int my);
   boolean     mouseMv(int mx, int my, int bStat);
   boolean     paint();
	#ifdef PM
	void			sizeOfImage(int *w, int *h);
	#endif

   void		   deselectAll();

#ifndef CV_NOARCHIVER
    void        putTo(VArchiver& a);
    void        putTo(VArchiver& a, boolean most_derived);
    void        getFrom(VArchiver& a);
    void        getFrom(VArchiver& a, VObject *pwin,
                        boolean do_realize = TRUE);
   void    realize(VWindow *parent);
#endif

	int				getNodeHorizontalExtent(VTreeNode *);
	int				getMaxHorizontalExtent();
	void			drawConnectingLine(int, int, int, int);

	virtual VStyle&	getDefaultStyle() const;
	static VStyle&	getClassDefaultStyle();

protected:
	void			enableBool(boolean);
	VImage		   *getImage(VBitMap *) const;
    virtual void    systemColorChanged();
    virtual void    systemFontChanged();

private:

#ifndef CV_NOARCHIVER
    integer     clientObjectIndex;
    integer     rclickMthdIndex;
    integer     selectMthdIndex;
    integer     dblClkMthdIndex;
    integer     expandMthdIndex;
    integer     expandedMthdIndex;
    integer     updatescrollMthdIndex;
#endif

   enum MouseState { Normal, ExpandorDown, ExpandorUp, Drag };

   VObject *clientObject;

   method   selectMthd;
   VObject *selectClient;

   method   dblClkMthd;
   VObject *dblClkClient;

   method   expandMthd;
   VObject *expandClient;

   method   expandedMthd;
   VObject *expandedClient;

   method   rclickMthd;
   VObject *rclickClient;

   method   updatescrollMthd;
   VObject *updatescrollClient;

   method   renameMthd;
   VObject *renameClient;
   
   method   preRenameMthd;
   VObject *preRenameClient;

   int    rowHeight;
   VPort  *port;
   VFont  *font;
   VTree  *tree;
   boolean externalTree;           // are we responsible for the tree?

   int     rowOffset;                      // number of rows scrolled
   int     horizontalOffset;

   VTreeNode  *mouseNode;
   MouseState  mouseState;
   VRectangle  mouseRect;
   VMouseCursor *pneverDropCursor;
   boolean     dragEnabled;
   boolean     expandEnabled;
   boolean	   renameEnabled;
   boolean	   doSelectCallback;
   boolean	   doRenameNode;

   boolean     drawTopLevelLines;
   int         inScrollCallBack;

   VBrush	  *stippleBrush;

   int         expSize;
   int         indentDist;

   boolean     refresh;
   boolean     rowHilite;
   boolean     inGotoNode;

   boolean      optimizeClick;

   int          m_rmouseMx;
   int          m_rmouseMy;

   VOrdCollect *theImages;

public:
   boolean     takeFocus();
private:
   boolean     givenFocus();
   boolean     clearFocus();
   void		   preMultiSelect();
   boolean	   paletteChanged(boolean = -1);

   boolean     inFocus;
   boolean     fMultiSel;
   boolean     checkExpandCallback;

   VTreeNode*  clickAt(int mx, int my, boolean fLeftMouse);
   virtual boolean     resized(int w, int h);
   void        getNodeRect(VTreeNode *node, int row, VRectangle& rect);

   boolean     vtScrollBar(int i);
   boolean     htScrollBar(int i);
  
public:
   boolean     updateScrollBars();
   void        scrollTree(int drow, int dcol);

private:
   void        treeChanged(VTree *t);
   void		   nodeRenameCancel(VEditLineNotify *el);
   void		   nodeRenameAccept(VEditLineNotify *el);
   VTreeNode   *renamingNode;

   friend  VTree;
   friend  VTableView;
   friend  VTableSubView;

};

// ---------------------------------------------------------------------------
inline int VTreeListBox::getNodeChildNo(VTreeNode *node)
{
   return (tree == 0) ? 0 : tree->getNodeChildNo(node);
}

// ---------------------------------------------------------------------------
inline VTreeNode *VTreeListBox::getCurrNode()
{
   return (tree == 0) ? 0 : (VTreeNode *)(tree->getSelectedNode()).first();
}

// ---------------------------------------------------------------------------
inline void     VTreeListBox::setRowHighlight(boolean tf)
{
   rowHilite = tf;
}

// ---------------------------------------------------------------------------
inline boolean VTreeListBox::getRowHighlight()
{
   return rowHilite;
}

inline boolean VTreeListBox::getClickOptimized()
{
    return optimizeClick;
}

inline void VTreeListBox::setClickOptimized(boolean tf)
{
    optimizeClick = tf;
}

inline boolean VTreeListBox::getDragEnable()
{
        return dragEnabled;
}

inline void VTreeListBox::setDragEnable(boolean tf)
{
        dragEnabled = tf;
}

inline boolean VTreeListBox::getExpandEnable()
{
        return expandEnabled;
}

inline void VTreeListBox::setExpandEnable(boolean tf)
{
        expandEnabled = tf;
}

// ---------------------------------------------------------------------------
inline VTree *VTreeListBox::getTree()
{
   return tree;
}

// ---------------------------------------------------------------------------
inline VTreeNode *VTreeListBox::getNodeAt(int index, boolean expand)
{
   return (tree == 0) ? 0 : tree->getNodeAt(index, expand);
}

// ---------------------------------------------------------------------------
inline VTreeNode *VTreeListBox::findNode(const VString& n, boolean expand)
{
   return (tree == 0) ? 0 : tree->findNode(n, expand);
}

// ---------------------------------------------------------------------------
inline VTreeNode *VTreeListBox::findNode(VObject *d, boolean expand)
{
   return (tree == 0) ? 0 : tree->findNode(d, expand);
}

// ---------------------------------------------------------------------------
inline void VTreeListBox::selectNodeAt(int n, boolean expand)
{
   if (n >= 0 && tree != 0) {
      gotoNode(tree->getNodeAt(n, expand));
   }

   updateScrollBars();
}

// ---------------------------------------------------------------------------
inline void VTreeListBox::deleteNode(VTreeNode *node)
{
   if (tree != 0) {
      tree->deleteNode(node);
#ifndef CV_NO_PAINT_EVENTS
      if (refresh && !updateScrollBars()) {
         update();
      }
#endif
   }
}
// ---------------------------------------------------------------------------
inline void VTreeListBox::setneverDropCursor(VMouseCursor * pcur)
{
   pneverDropCursor = pcur;
}

// ---------------------------------------------------------------------------
inline int VTreeListBox::getRowHeight()
{
   return rowHeight;
}

inline boolean VTreeListBox::getCheckExpandCallback()
{
   return checkExpandCallback;
}

inline void VTreeListBox::setCheckExpandCallback(boolean var)
{
   checkExpandCallback = var;
}

inline boolean VTreeListBox::getAutoRenameEnable()
{
   return renameEnabled;
}

inline void VTreeListBox::setAutoRenameEnable(boolean var)
{
   renameEnabled = var;
}

inline VFont *VTreeListBox::getFont() const
{
	return font;
}

#endif // VTLISTBX_H
