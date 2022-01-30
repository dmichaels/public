// $Header:   Q:/views/common/vcs/VTREE.H_V   1.16   Mar 24 1997 16:14:30   scottt  $ 

//	vtree.h
//
//	VTree interface [Common]
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
//	07/24/93 jmd	changed to use VString wherever possible
//	04/04/94 jmd	ifdef'd dependency on GrTreeView
//					!!!jmd--add AddNode(VTreeNode *node, VTreeNode *parent);
//	06/23/95 dgm	Original check-in; original code from jmd.
//	11/20/95 dgm	Added getFrom() to suppress virtual hiding warning.
//	03/07/96 pkt	added VGrTreeView abstract class to eliminate dependency
//					on #ifdefs.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTREE_H
#define VTREE_H

#include "treenode.h"
#include "ordcllct.h"

CLASS VTreeListBox;
CLASS VWindow;
CLASS VOrdCollect;
CLASS VGrTreeView;
CLASS GrTreeView;
#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

extern VClass *VTreeCls;
CLASS VTree :public VObject{
public:
   VTree();
   ~VTree();
	VClass* iam();
   VTreeNode  *addNode(const VString& childName, VTreeNode *node, VObject *data = 0,
                       boolean isparent = TRUE, boolean iscontainer = FALSE);
   void       renameNode(VTreeNode *node, const VString& newname);
   void       bitmapNode(VTreeNode *node, VBitMap *bitmap);
   void       colorNode(VTreeNode *node, const VColor& color);
   void       fontNode(VTreeNode *node, VFont *font);
   void       removeNode(VTreeNode *node);
   void       moveNode(VTreeNode *node, VTreeNode *prev);
   void       deleteNode(VTreeNode *node);

   VTreeNode  *getNodeAt(int index, boolean expand = TRUE);
   int        getNodeCount();

   int        childCount(VTreeNode *node);
   int        expChildCount(VTreeNode *node);

   void       setRefresh(boolean tf, VWindow *who = 0);

   void       setNodeExpand(VTreeNode *node, boolean tf, VWindow *who = 0);

   int        getNodeChildNo(VTreeNode *node);
   int        getNodeIndex(VTreeNode *node, boolean expand);
   boolean	  isParentOf(VTreeNode *node, VTreeNode *parent);
   
   VTreeNode  *findNode(const VString &n, boolean expand = TRUE);
   VTreeNode  *findNode(VObject *d, boolean expand = TRUE);

   boolean    promoteNode(VTreeNode *node);
   boolean    demoteNode(VTreeNode *node);

   boolean    isTopLevelNode(VTreeNode *node);

   VTreeNode  *getTopNode();

   VOrdCollect &getSelectedNode();
   boolean    selectNode(VTreeNode *node, VWindow *who = 0, boolean fMultiSel = FALSE);
   void       deSelectNode(VTreeNode *node);

   void       reset();
   #ifndef CV_NOARCHIVER
	virtual void			putTo(VArchiver& a);
	virtual void			getFrom(VArchiver& a);
	virtual void 			getFrom(VArchiver&, VObject *, boolean = TRUE);
   #endif

private:
   VTreeNode  *topNode;
   VTreeNode  *clickNode;   // Node last clicked (not necessarily selected)
   VOrdCollect selectedNode;
   VTreeNode  *hintNode;
   int         hintIndex;
   boolean     hintExpand;
   int         nodeCount;          // number of visible (expanded) nodes
   boolean     dirtyCount;

   VTreeListBox *treeView;         // view we are connected to (!!!use sub list)
   VGrTreeView  *graphView;

   void       resetHints();

   VTreeNode  *find(const VString &n, VObject *d, boolean expand = TRUE);
   VTreeNode  *findNext(VTreeNode *node, boolean expand = TRUE);
   VTreeNode  *findPrev(VTreeNode *node, boolean expand = TRUE);

   friend VTreeListBox;
   void       setTreeView(VTreeListBox *v);

   friend GrTreeView;
   void       setGraphView(VGrTreeView *v);

};

CLASS VGrTreeView {
public:
	virtual void		setRefresh(boolean) = 0;
	virtual void		treeChanged(VTree *) = 0;
	virtual void		updateNode(VTreeNode *) = 0;
	virtual void		gotoNode(VTreeNode *, boolean nopaint = FALSE) = 0;
	virtual void		expandNode(VTreeNode *, boolean nonotify = FALSE) = 0;
	virtual VWindow	   *win() = 0;
};
  
// ---------------------------------------------------------------------------
inline int VTree::childCount(VTreeNode *node)
{
   if (node != 0) {
      return node->childCount;
   }
   else {
      return 0;
   }
}

// ---------------------------------------------------------------------------
inline boolean VTree::isTopLevelNode(VTreeNode *node)
{
   if (node != 0) {
      return (node->getParent() == topNode);
   }
   else {
      return FALSE;
   }
}

// ---------------------------------------------------------------------------
inline VTreeNode *VTree::getTopNode()
{
   return topNode;
}

// ---------------------------------------------------------------------------
inline VOrdCollect &VTree::getSelectedNode()
{
   return selectedNode;
}

// ---------------------------------------------------------------------------
inline void VTree::setTreeView(VTreeListBox *v)
{
   treeView = v;
}

// ---------------------------------------------------------------------------
inline void VTree::setGraphView(VGrTreeView *v)
{
   graphView = v;
}

#endif // VTREE_H
