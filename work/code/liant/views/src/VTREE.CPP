// $Header:   Q:/views/common/vcs/VTREE.CPV   1.16   Mar 24 1997 16:14:30   scottt  $ 

//  vtree.cpp
//
//  VTree implementation [Common]
//
//	Allegris Foundation 1.1.00
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
//  05/13/93 jmd    revised
//  07/24/93 jmd    changed to use VString wherever possible
//  04/04/94 jmd    ifdef'd dependency on GrTreeView
//                  fix getNodeCountAll  (keep track from add/remove)
//                  rename the members to make more sense
//  06/23/95 dgm    Original check-in; original code from jmd.
//  11/20/95 dgm    Added getFrom() to suppress virtual hiding warning.
//	03/07/96 pkt	added VGrTreeView abstract class to eliminate dependency
//					on #ifdefs.
//	04/03/96 pkt	changed findNode(const VString&, boolean expand) to
//					pass the 'expand' parameter on to 'find'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "vtree.h"

#include "vtlistbx.h"   // for notification linkage

defineClass(VTree,VObject)
// ---------------------------------------------------------------------------
VTree::VTree() :
   treeView(0),
   graphView(0)
{
   topNode = new VTreeNode("", 0);
   clickNode = 0;
   topNode->expand = TRUE;
   selectedNode.reset();
   resetHints();
}

// ---------------------------------------------------------------------------
VTree::~VTree()
{
   if (treeView != 0) {
      treeView->treeChanged(0);      //!!!jmd expand on this
   }

   if (graphView != 0) {
      graphView->treeChanged(0);     //!!!jmd expand on this
   }

   delete topNode;
   topNode = 0;
}

// ---------------------------------------------------------------------------
// delete all the nodes and start anew
//
void VTree::reset()
{
   delete topNode;
   topNode = new VTreeNode("", 0);
   topNode->expand = TRUE;
   selectedNode.reset();
   clickNode = 0;
   resetHints();

   if (treeView != 0) {
      treeView->treeChanged(this);
   }

   if (graphView != 0) {
      graphView->treeChanged(this);
   }
}

// ---------------------------------------------------------------------------
// Create a new node with name 'childName' and
// insert it in the tree.  If parent == 0 for no parent (top of tree).
// Return 0 if unsuccessful, else a pointer to the new VTreeNode.
//
VTreeNode *VTree::addNode(const VString& childName, VTreeNode *node,
                          VObject *data, boolean isparent,
                          boolean iscontainer)
{
   VTreeNode *tn, *pn;

   if (!(tn = new VTreeNode(childName, data))) {
      return 0;
   }

   if (iscontainer)
      tn->setContainer(TRUE);

   if (isparent) {
      if (node == 0) {
         node = topNode;
      }
      node->addChild(tn);
   }
   else {
      // insert after 'node'
      if ((pn = node->parentNode) != 0) {
         pn->addChild(tn, node);
      }
   }

   resetHints();

   return tn;
}

// ---------------------------------------------------------------------------
boolean VTree::promoteNode(VTreeNode *node)
{
   VTreeNode *pn, *tn, *xn;

   // 1) don't promote if parent is topNode
   // 2) insert 'node' after parent as sibling
   // 3) turn the next siblings into children

   if (node != 0 && (pn = node->getParent()) != 0 && pn->getParent() != 0) {
      tn = node->getNext();

      pn->removeChild(node);

      // insert after parent node
      pn->getParent()->addChild(node, pn->getNext());

      // make following siblings, children
      while (tn != 0) {
         xn = tn;
         tn = tn->getNext();
         pn->removeChild(xn);
         node->addChild(xn);
      }

      resetHints();
      return TRUE;
   }

   return FALSE;
}

// ---------------------------------------------------------------------------
boolean VTree::demoteNode(VTreeNode *node)
{
   VTreeNode *pn, *tn, *prev, *xn;

   // 1) only demote if there is a previous sibling
   // 2) turn the node into the last child of the previous sibling
   // 3) turn all children into siblings

   if (node != 0 && (pn = node->getParent()) != 0 && (prev = node->getPrev()) != 0) {
      pn->removeChild(node);

      // make child of previous sibling
      prev->addChild(node);

      // make children, silbings
      tn = node->firstChild;
      while (tn != 0) {
         xn = tn;
         tn = tn->getNext();
         node->removeChild(xn);
         prev->addChild(xn);
      }

      resetHints();
      return TRUE;
   }

   return FALSE;
}

// ---------------------------------------------------------------------------
// remove 'node'
//
void VTree::removeNode(VTreeNode *node)
{
   VTreeNode *pn1;

   if (node != 0) {
      if ((pn1 = node->getParent()) != 0) {
         deSelectNode(node);
         pn1->removeChild(node);
      }
   }

   resetHints();
}


// ---------------------------------------------------------------------------
// move 'node' to a slot following prev
//
void VTree::moveNode(VTreeNode *node, VTreeNode *prev)
{
   VTreeNode *pn1, *pn2;

   if (node != 0 && prev != 0) {
      if ((pn1 = node->getParent()) != 0 && (pn2 = prev->getParent()) != 0) {
         pn1->removeChild(node);
         pn2->addChild(node, prev->getNext());
      }
   }
}

// ---------------------------------------------------------------------------
// delete the node
//
void VTree::deleteNode(VTreeNode *node)
{
   if (node != 0) {
      delete node;
      resetHints();
   }
}

// ---------------------------------------------------------------------------
// Select the specified node, return FALSE if it's already selected
// 'who' is the window selecting the node
//
boolean VTree::selectNode(VTreeNode *node, VWindow *who, boolean fMultiSel)
{
   VTreeNode *oldNode; 

   if (selectedNode.findSame((VObject *)node)) {
      if (fMultiSel) {
          selectedNode.removeSame((VObject *)node);
          return TRUE;
      }
      else {
          return FALSE;
      }
   }

   int i = (int) selectedNode.count();
   if (!fMultiSel && selectedNode.count()) {    // new selected node replaces old selected node
       oldNode = (VTreeNode *)selectedNode.removeAt(0);
   }
   if (node) {  // Always insert most recently selected node as first
       selectedNode.insertAt(0, (VObject *)node);
    }


   if (graphView != 0 && who != graphView->win()) {
      if (!fMultiSel) {
          graphView->updateNode(oldNode);
      }
      graphView->updateNode(node);
      graphView->gotoNode(node, TRUE);
   }

   if (treeView != 0 && who != treeView) {
      if (!fMultiSel) {
          treeView->updateNode(oldNode);
      }
      treeView->updateNode(node);
      treeView->gotoNode(node, TRUE);
   }

   return TRUE;
}

// ---------------------------------------------------------------------------
// link refresh mode for the various views
//
void VTree::setRefresh(boolean tf, VWindow *who)
{
   if (treeView != 0 && who != treeView) {
      treeView->setRefresh(tf);
   }
   if (graphView != 0 && who != graphView->win()) {
      graphView->setRefresh(tf);
   }
}

// ---------------------------------------------------------------------------
void VTree::renameNode(VTreeNode *tn, const VString& newname)
{
   if (tn != 0) {
      tn->name.puts(newname);
   }
}

// ---------------------------------------------------------------------------
void VTree::bitmapNode(VTreeNode *tn, VBitMap *bitmap)
{
   if (tn != 0) {
      tn->setBitMap(bitmap);
   }
}

// ---------------------------------------------------------------------------
void VTree::colorNode(VTreeNode *tn, const VColor& color)
{
   if (tn != 0) {
      tn->setColor(color);
   }
}

// ---------------------------------------------------------------------------
void VTree::fontNode(VTreeNode *tn, VFont *font)
{
   if (tn != 0) {
      tn->setFont(font);
   }
}

// ---------------------------------------------------------------------------
int VTree::getNodeCount()
{
   if (dirtyCount) {
      // calculate node count
      topNode->expand = TRUE;
      nodeCount = expChildCount(topNode);
      dirtyCount = FALSE;
   }

   return nodeCount;
}

// ---------------------------------------------------------------------------
// Return what number child the node is
//
int VTree::getNodeChildNo(VTreeNode *node)
{
   VTreeNode *tn;
   int cno = 0;

   if (node != 0 && node->getParent() != 0) {
      for(tn = node->getParent()->firstChild; tn != 0; tn = tn->getNext(), cno++) {
         if (tn == node) {
            break;
         }
      }
   }

   return cno;
}

//-----------------------------------------------------------
boolean VTree::isParentOf(VTreeNode *node, VTreeNode *parent)
{
  boolean    rc = FALSE;
  VTreeNode *tn;

  if (node == 0)
    return rc;

  for (tn = node->getParent(); tn != 0; tn = tn->getParent()) {
    rc = rc || (tn == parent);
  }

  return rc;
}
    
// ---------------------------------------------------------------------------
// Find a node given its name.
// Return 0 is unsuccessful.
// if expand is TRUE, only search through expanded branches
//
VTreeNode *VTree::findNode(const VString& n, boolean expand)
{
   // return (n == 0) ? 0 : find(n, 0);
   return find(n, 0, expand);
}

// ---------------------------------------------------------------------------
// Find a node given its data.
// Return 0 is unsuccessful.
// if expand is TRUE, only search through expanded branches
//
VTreeNode *VTree::findNode(VObject *d, boolean expand)
{
   if (d == 0) {
      return 0;
   }
   else {
      return find("", d, expand);
   }
}

// ---------------------------------------------------------------------------
// look for the node with name 'n', if n == "", look for the node with data 'd'
// return 0 if unsuccessful
// if expand is TRUE, only search through expanded branches
//
VTreeNode *VTree::find(const VString& n, VObject *d, boolean expand)
{
   VTreeNode *node = topNode->firstChild;

   while (node != 0) {
      if (n == (const char *)"") {
         if (node->data == d) {
            return node;
         }
      }
      else {
         if (node->name == n.gets()) {
            return node;
         }
      }

      // get next node
      node = findNext(node, expand);
   }

   return 0;
}

// ---------------------------------------------------------------------------
//  Get the index'th node in the tree:
//  If Tree style, Count from top, going into all expanded nodes
// if expand is TRUE, only search through expanded branches
//
// !!!jmd--If Alpha style, start at first alpha-sorted node
//
// Return 0 if unsuccessful.
//
VTreeNode *VTree::getNodeAt(int index, boolean expand)
{
   VTreeNode *node = 0;
   register int i;

   if (hintNode == 0 || hintExpand != expand) {
      hintNode = topNode->firstChild;
      hintIndex = 0;
   }

   if (index <= 0) {
      node = topNode->firstChild;
      hintNode = node;
      hintIndex = 0;
   }
   else if (index == hintIndex) {
      node = hintNode;
   }
   else if (index > hintIndex) {
      // search forwards
      node = hintNode;
      for (i = hintIndex; node != 0 && i < index; i++) {
         // get next node
         node = findNext(node, expand);
      }
   }
   else {
      // search backwards
      node = hintNode;
      for (i = hintIndex; node && i > index; i--) {
         // get prev node
         node = findPrev(node, expand);
      }
   }

   // set hints for next time
   hintNode = node;
   hintIndex = (node) ? index : 0;
   hintExpand = expand;

   return node;
}

// ---------------------------------------------------------------------------
// find the next sequential node,
// if expand is TRUE, only search through expanded branches
//
VTreeNode *VTree::findNext(VTreeNode *node, boolean expand)
{
   if (node != 0) {
      if (node->firstChild != 0 && (!expand || node->expand)) {
         // jump to children
         node = node->firstChild;
      }
      else if (node->nextNode != 0) {
         // jump to next sibling
         node = node->nextNode;
      }
      else if (node->parentNode != 0) {
         // jump back up to parent's sibling (uncle, great uncle, etc.)
         VTreeNode *n = node->parentNode;
         while(n != 0 && n->nextNode == 0) {
            n = n->parentNode;
         }
         node = (n != 0) ? n->nextNode : 0;
      }
      else {
         node = 0;
      }
   }

   return node;
}

// ---------------------------------------------------------------------------
// find the previous sequential node
// if expand is TRUE, only search through expanded branches
//
VTreeNode *VTree::findPrev(VTreeNode *node, boolean expand)
{
   if (node != 0) {
      if (node->prevNode) {
         // jump to prev sibling
         node = node->prevNode;

         // find bottom of tree
         while ((!expand || node->expand) && node->lastChild) {
            node = node->lastChild;
         }
      }
      else if (node->parentNode) {
         // jump back up to parent
         node = node->parentNode;
      }
      else {
         node = 0;
      }
   }

   return node;
}

// ---------------------------------------------------------------------------
// Count the number of (expanded) children between node and its next sibling.
// Count includes 'node'
//
int   VTree::expChildCount(VTreeNode *node)
{
   int count = 0;
   VTreeNode *last;

   if (node != 0) {
      last = node->nextNode;
      while (node != last && node != 0) {
         count++;

         // get next node
         node = findNext(node, TRUE);
      }
   }

   return count;
}

// ---------------------------------------------------------------------------
// Set the expand flag for the specified node
// 'who' is the window expanding the node
//
void VTree::setNodeExpand(VTreeNode *node, boolean tf, VWindow *who)
{
   if (node == 0 || node->expand == tf) {
      return;
   }

   node->expand = tf;
   resetHints();

   if (graphView != 0 && who != graphView->win()) {
      graphView->expandNode(node, TRUE);
   }

   if (treeView != 0 && who != treeView) {
      treeView->expandNode(node, TRUE);
   }
}

// ---------------------------------------------------------------------------
// Recalculate hint data for tree
//
void VTree::resetHints()
{
   hintNode = 0;
   hintIndex = -1;
   dirtyCount = TRUE;
}

// ---------------------------------------------------------------------------
// !!!jmd make this more efficient !!!
// if expand is TRUE, only count expanded branches
//
int VTree::getNodeIndex(VTreeNode *node, boolean expand)
{
   if (node != 0) {
      VTreeNode *tn;

      for (int i = 0;; i++) {
         if ((tn = getNodeAt(i, expand)) == 0) {
            break;
         }

         if (tn == node) {
            return i;
         }
      }
   }

   return 0;
}

// ---------------------------------------------------------------------------
// deSelect the node
//
void VTree::deSelectNode(VTreeNode *node)
{
#if KRB
   if (selectedNode.find(node)) {
      return FALSE;
   }

   VTreeNode *oldnode = selectedNode.removeSame(node);

   if (graphView != 0 && who != graphView->win()) {
      graphView->updateNode(oldnode);
//      graphView->updateNode(selectedNode);
//      graphView->gotoNode(selectedNode, TRUE);
   }

   if (treeView != 0 && who != treeView) {
     treeView->updateNode(oldnode);
//      treeView->updateNode(selectedNode);
//      treeView->gotoNode(selectedNode, TRUE);
   }
#endif

   VTreeNode *oldnode = (VTreeNode *)selectedNode.removeSame((VObject *)node);
}

#ifndef CV_NOARCHIVER
void VTree::getFrom(VArchiver& a)
{   
    topNode = (VTreeNode*) a.getObject(0);
}

void VTree::getFrom(VArchiver& a, VObject *, boolean)
{
    getFrom(a);
}

void VTree::putTo(VArchiver& a)
{
    a.putObject(topNode);
}

#endif

// ---------------------------------------------------------------------------
