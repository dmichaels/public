// $Header:   Q:/views/common/vcs/TREENODE.H_V   1.18   Mar 24 1997 16:13:58   scottt  $ 

//  treenode.h
//
//  VTreeNode interface [Common]
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
//  06/23/95 dgm    Original check-in; original code from jmd.
//  11/20/95 dgm    Added getFrom() to suppress virtual hiding warning.    
//  01/09/96 glm    Added deleteChildren()
//  01/06/97 dgm    Added setChildIndentLevel().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTREENODE_H
#define VTREENODE_H

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

#include "defs.h"
#include "str.h"
#include "color.h"

CLASS VBitMap;
CLASS VTree;
CLASS VTreeListBox;
CLASS VColor;
CLASS VFont;

extern VClass *VTreeNodeCls;
CLASS VTreeNode :public VObject{
public:
   VTreeNode();
   VTreeNode(const VString& n, VObject *d = 0);
   ~VTreeNode();
   VClass* iam();
   VString&   getName()   { return(name); }
   int        getLevel() { return(indentLevel); }
   boolean    isExpand()  { return(expand); }

   VObject    *getData()  { return(data); }
   void       setData(VObject *d) { data = d; }
   VTreeNode  *getParent() { return(parentNode); }

   VBitMap    *getBitMap()            { return(bitMap); }
   void       setBitMap(VBitMap *b)   { bitMap = b; }

   VColor&    getColor()              { return (m_color); }
   void       setColor(const VColor& color)  { m_color = color; colorHasBeenSet = 1; }

   VFont     *getFont()              { return (m_font); }
   void       setFont(VFont *font)    { m_font = font; }

   VTreeNode  *getNext()   { return(nextNode); }
   VTreeNode  *getPrev()   { return(prevNode); }

   VTreeNode  *getFirst()  { return(firstChild); }
   VTreeNode  *getLast()   { return(lastChild); }

   boolean    getContainer() { return isContainer; }
   void       setContainer(boolean tf)  { isContainer = tf; }
   #ifndef CV_NOARCHIVER
    virtual void            putTo(VArchiver&);
    virtual void            getFrom(VArchiver&);
    virtual void            getFrom(VArchiver&, VObject *, boolean = TRUE);
   #endif

protected:
	static void				setChildIndentLevel(VTreeNode *, int);

public:
   VTreeNode  *parentNode;
   VTreeNode  *nextNode;
   VTreeNode  *prevNode;
   VTreeNode  *firstChild;
   VTreeNode  *lastChild;
   int         childCount;
private:
   boolean     expand;
   boolean     isClosing;
   int         indentLevel;
   boolean     isContainer;

   VObject     *data;
   VString     name;
   VBitMap     *bitMap;    //!!!jmd
   VColor      m_color;
   VFont       *m_font;

   int			colorHasBeenSet : 1;

   friend   VTree;
   friend   VTreeListBox;

   boolean  addChild(VTreeNode *child, VTreeNode *sibling = 0);
   void     removeChild(VTreeNode *child);
   void     deleteChildren();

};

#endif // VTREENODE_H
