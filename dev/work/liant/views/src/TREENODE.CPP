// $Header:   Q:/views/common/vcs/TREENODE.CPV   1.18   Mar 24 1997 16:13:58   scottt  $ 

//	treenode.cpp
//
//	VTreeNode implementation [Common]
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
//	06/23/95 dgm	Original check-in; original code from jmd.
//	11/20/95 dgm	Added getFrom() to suppress virtual hiding warning.
//  01/06/97 glm    Added setChildIndentLevel() to be called from addChild()
//					to ensure all descendants get their level updated.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifdef CVS
#include "cvs.h"
#endif
#include "treenode.h"

// ---------------------------------------------------------------------------
defineClass(VTreeNode,VObject)

VTreeNode::VTreeNode()   
   : bitMap(0),
     m_color(0ul),
     m_font(0),
     data(0)
{
   nextNode = 0;
   prevNode = 0;
   firstChild = 0;
   lastChild = 0;
   parentNode = 0;

   childCount = 0;

   expand = FALSE;
   isClosing = FALSE;
   indentLevel = 0;
   isContainer = 0;
   colorHasBeenSet = 0;
}

VTreeNode::VTreeNode(const VString& n, VObject *d)
   : name(n),
     bitMap(0),
     m_color(0ul),
     m_font(0),
     data(d)
{
   nextNode = 0;
   prevNode = 0;
   firstChild = 0;
   lastChild = 0;
   parentNode = 0;

   childCount = 0;

   expand = FALSE;
   isClosing = FALSE;
   indentLevel = 0;
   isContainer = 0;
   colorHasBeenSet = 0;
}

// ---------------------------------------------------------------------------
VTreeNode::~VTreeNode()
{
   isClosing = TRUE;

   // delete all children
   VTreeNode *child, *next;

   child = firstChild;
   while (child != 0) {
      next = child->nextNode;
      delete child;
      child = next;
   }

   if (parentNode != 0) {
      parentNode->removeChild(this);
   }
}

// ---------------------------------------------------------------------------
//
void VTreeNode::setChildIndentLevel(VTreeNode *child, int level)
{
	if (child != 0) {
		child->indentLevel = level;
		for (VTreeNode *p = child->firstChild ; p != 0 ; ) {
			setChildIndentLevel(p, level + 1);
			if ((p = p->nextNode) == child->firstChild) {
				break;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// Add a child to the node. If sibling != 0, places it before sibling,
// else places it after the last child
//
boolean VTreeNode::addChild(VTreeNode *child, VTreeNode *sibling)
{
   if (child != 0) {
      if (sibling != 0) {
         if (sibling->parentNode != this) {
            return FALSE;
         }

         if (sibling->prevNode != 0) {
            sibling->prevNode->nextNode = child;
         }
         else {
            firstChild = child;
         }
         child->prevNode = sibling->prevNode;
         child->nextNode = sibling;
         sibling->prevNode = child;
      }
      else {
         if (firstChild != 0) {
            lastChild->nextNode = child;
            child->prevNode = lastChild;
            lastChild = child;
         }
         else {
            firstChild = lastChild = child;
            child->prevNode = 0;
         }

         child->nextNode = 0;
      }
	  child->parentNode = this;
	  setChildIndentLevel(child, indentLevel + 1);
	  childCount++;
	  isContainer = TRUE;
   }

   return TRUE;
}

// ---------------------------------------------------------------------------
void VTreeNode::removeChild(VTreeNode *child)
{
   if (!isClosing && child && child->parentNode == this) {
      if (firstChild == child) {
         firstChild = child->nextNode;
      }
      if (lastChild == child) {
         lastChild = child->prevNode;
      }

      if (child->prevNode) {
         child->prevNode->nextNode = child->nextNode;
      }

      if (child->nextNode) {
         child->nextNode->prevNode = child->prevNode;
      }

      childCount--;
      child->parentNode = 0;
      child->prevNode = 0;
      child->nextNode = 0;
      child->indentLevel = 0;
	  if (childCount == 0) {
		  expand = FALSE;
	      isContainer = FALSE;
	  }
   }
}

// ---------------------------------------------------------------------------

#ifndef CV_NOARCHIVER
void	VTreeNode::putTo(VArchiver& a)
{
	a.putObject(&name);
	a << indentLevel;
	a << expand;
	a << childCount;
	// should the data,color,bitmap and font be saved?
	
	// save off your children
	VTreeNode *pChild = 0;
	pChild = getFirst();
	while (pChild != 0) {
		a.putObject(pChild);
		pChild = pChild->getNext();
	}

}

void	VTreeNode::getFrom(VArchiver& a)
{
	int tempChildCount;

	a.getObject(&name);
	a >> indentLevel;
	a >> expand;
	a >> tempChildCount;
	// get your children
	int i = tempChildCount;
	childCount = 0;
	VTreeNode *pChild = 0;
	while (i > 0) {
		pChild = (VTreeNode*) a.getObject(0);
		if (pChild != 0) {
			addChild(pChild);
		}
		i--;
	}

}
void VTreeNode::getFrom(VArchiver& a, VObject *, boolean)
{
	getFrom(a);
}

#endif
