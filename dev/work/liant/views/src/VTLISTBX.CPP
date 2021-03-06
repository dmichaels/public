// $Header:   Q:/views/common/vcs/vtlistbx.cpv   1.67   Mar 20 1997 18:31:24   gregm  $ 

//  vtlistbx.cpp
//
//  VTreeListBox implementation [Common]
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
//  07/21/93 jmd    added bitmap
//  07/21/93 jmd    added row hilite, double click support
//  07/24/93 jmd    changed to use VString wherever possible
//  08/31/93 jmd    added inGotoNode flag
//  01/13/94 jmd    changed to use VColor::
//  02/04/94 dgm    minor Motif changes.
//  02/09/94 jmd    scrollbar now uses longs
//  02/26/94 jmd    changed setTree to check refresh flag
//  02/27/94 jmd    added simple expRect test in paint
//                  !!! add select call back,
//                      optimize painting (expRect, select)
//                  !!! add setFont etc...
//                  !!! getNodeRow
//                  !!! make this a control, not a view
//  06/23/95 dgm    Original check-in; original code from jmd.
//  09/20/95 dss    VStyle changes.
//  01/24/95 evc    Removed VStyleHorizontal from VView ctor call
//  02/19/95 evc    Changed the static VColor ExpSurface to be
//                  in the default data segment for 16 bit static builds
//                  with CV_NEARDATA.
//  02/20/96 tlf    Added brush to expand/contract box creation code.
//  02/21/96 tlf    Updated for HPUX.
//  04/30/96 glm    added archiving
//	09/17/96 pkt	updated for WebShop.
//	09/26/96 dgm	VColorScheme.
//	10/04/96 dgm	Added (hacked) VImage support.
//					Fixed selected foreground color problem.
//	12/16/96 pkt	Updated to work with Web platform.
//  01/06/97 dgm    Changed addNode() to unconditionally call
//					updateScrollBars(), even if we're hidden.
//  01/27/97 dgm    #ifdef'd OS/2 specific code in updateScrollBars().
//
//	02/04/97 pkt	Implemented 'getClassDefaultStyle'.
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
// ---------------------------------------------------------------------------

// Use #define CV_NOVIMAGE to disable VImage support.

#include "port.h"
#include "vtlistbx.h"
#include "vtree.h"
#include "notifier.h"
#include "scrollbr.h"
#include "bitmap.h"
#include "ordcllct.h"
#include "iterator.h"
#include "popupmen.h"
#include "clrscm.h"
#include "color.h"
#include "cvassert.h"
#include "editline.h"
#include "editnot.h"
#ifndef CV_NOVIMAGE
#include "image.h"
#endif

#include <ctype.h>  // KC for toupper()
#ifndef CV_NOARCHIVER
#include "cllbckls.h"
#endif

defineClass(VTreeListBox, VView)
defineArchiveRevision(VTreeListBox, 1)

static int indentOffset = 10;

#ifdef CV_HPUX
const Color &VTreeListBox_color_init = 0;
#endif


// ---------------------------------------------------------------------------
//
VTreeListBox::VTreeListBox(const VFrame &f, VWindow *p, const VStyle& style,
                            int minrowhgt)
{
	// djs 01/17/97 - Call super constructor via the () operator
	// for OS/2 ::createWin special case.  Should not affect windows.
#if defined(CVS_USE_TRANSIENT) && defined(CVS_LATER_DEFAULT)
	// StyleVertical and StyleHorizontal are handled as a default.
	// The re-addition of StyleHorizontal is justified because both scroll
	// bars are hidden automatically when not needed.
	(*this)(f, p, style);
#else
	(*this)(f, p, style + StyleVertical );
#endif
    init();
    fMultiSel = style.contains(StyleMultiSelect) ? TRUE : FALSE;

    if ( rowHeight <= minrowhgt )
        rowHeight = minrowhgt;

    if ( rowHeight == 0 )
        rowHeight = 8;
 
    if ( style.contains(StyleFlatList) )
        indentDist = 2;
}

// ---------------------------------------------------------------------------
VTreeListBox::VTreeListBox()
{
    init();
}

//----------------------------------------------------------------------------
void VTreeListBox::init()
{
	theImages = 0;

	clientObject = 0;

	selectClient = 0;
	dblClkClient = 0;
	expandClient = 0;
	expandedClient = 0;
	updatescrollClient = 0;
	preRenameClient = 0;
	rclickClient = 0;
	renameClient = 0;

	selectMthd = 0;
	dblClkMthd = 0;
	expandMthd = 0;
	expandedMthd = 0;
	updatescrollMthd = 0;
	renameMthd = 0;
	preRenameMthd = 0;
	rclickMthd = 0;

	doSelectCallback = FALSE;
	doRenameNode = FALSE;

#ifndef CV_NOARCHIVER
    clientObjectIndex = -1;
    rclickMthdIndex = -1;
    selectMthdIndex = -1;
    dblClkMthdIndex = -1;
    expandMthdIndex = -1;
    expandedMthdIndex = -1;
    updatescrollMthdIndex = -1;
#endif

    inScrollCallBack = 0;
    tree = new VTree();
    tree->setTreeView(this);
    rowHeight = 1;
    rowOffset = 0;
    horizontalOffset = 0;
    mouseNode = 0;
    mouseState = Normal;
    pneverDropCursor = 0;
    refresh = TRUE;
    dragEnabled = FALSE;
    expandEnabled = TRUE;
	renameEnabled = FALSE;
    drawTopLevelLines = FALSE;
    rowHilite = FALSE;      // highlight entire row...
    inGotoNode = FALSE;
    optimizeClick = TRUE;   // If optimize is TRUE, don't send click msg
                            // if item is already selected
	checkExpandCallback = FALSE;

    fMultiSel = 0;
    inFocus = FALSE;

	stippleBrush = 0;

	m_rmouseMx = 0;
	m_rmouseMy = 0;

    font = 0;

    int fw, fh;
    fontSize(&fw, &fh);

    if ((fw == 0) && (fh == 0)) {
        fw = 8;
        fh = 20;
    }
   
    expSize = (5 * fh) / 8;    // height and width of expandor button
    if (!(expSize & 1)) {
        expSize -= 1;
    }
    indentDist = fw * 2;    // amount to indent each new child level
    indentDist &= ~1;

    if (indentDist < indentOffset * 2) {
        indentDist = indentOffset * 2;
    }

    port = new VPort(this);
    port->useFont(font);
    port->open();
    rowHeight = port->textLineSpace();
    if (rowHeight == 0) {
        rowHeight = 20;
    }
	else {
		rowHeight += 3;
	}
    port->close();
   
    setBackground(&VBrush::system(VColorScheme::FlatBackground));
    externalTree = FALSE;

#ifndef CV_NO_MOUSE_EVENTS
    // set up scroll bars
    if (vScroll != 0) {
        vScroll->uponChange(this, methodOf(VTreeListBox, vtScrollBar));
        vScroll->tracking(TRUE);
    }
    if (hScroll != 0) {
		hScroll->setValues(0, 0, 1, 20);
        hScroll->uponChange(this, methodOf(VTreeListBox, htScrollBar));
        hScroll->tracking(TRUE);
    }
#endif

//    updateScrollBars();
}

// ---------------------------------------------------------------------------
//
void VTreeListBox::setFont(VFont *f)
{
	font = f;
#if defined(CV_WINDOWS) && !defined(CV_REMOTE_GUI)
	if (font == 0) {
		if (hWnd != 0) {
			SendMessage(hWnd, WM_SETFONT,
							  WPARAM(VFont::system(VFont::Icon).getHndl()),
							  LPARAM(1));
		}
	}
#endif
}

// ---------------------------------------------------------------------------
//
void VTreeListBox::systemFontChanged()
{
    port->useFont(font);
    port->open();
    rowHeight = port->textLineSpace();
    if (rowHeight == 0) {
        rowHeight = 20;
    }
	else {
		rowHeight += 3;
	}
    port->close();
	updateScrollBars();
}

// ---------------------------------------------------------------------------
VTreeListBox::~VTreeListBox()
{
	delete stippleBrush;

    if (!externalTree) {
        delete tree;
    }
    else if (tree != 0) {
        tree->setTreeView(0);
    }
   
    delete port;

	if (theImages != 0) {
		theImages->freeContents();
		delete theImages;
		theImages = 0;
	}
}

// ---------------------------------------------------------------------------
// called by tree when it is deleted (t = 0)
// !!!jmd expand this implementation
//
void VTreeListBox::treeChanged(VTree *t)
{
    // Note: this could be called as a result of our own destructor-jmd
    // or because of setTree()
    if (t == 0) {
        tree = 0;
    }
    else {
        tree = t;
        rowOffset = 0;
    }
}

// ---------------------------------------------------------------------------
void VTreeListBox::setRefresh(boolean tf)
{
#ifndef CV_NO_PAINT_EVENTS
	if ( refresh = tf ) {
		if ( !updateScrollBars() )
			update();
	}
#endif
}

// ---------------------------------------------------------------------------
// set a new, external tree for this view
// Note, once you call this, you are responsible for deleting the tree
//
void VTreeListBox::setTree(VTree *t)
{
   if (t != tree) {
      if (!externalTree) {
     delete tree;
      }
      externalTree = TRUE;
      tree = t;

      if (tree != 0) {
     tree->setTreeView(this);
      }
      if (refresh) {
     updateScrollBars();
      }
   }
}

// ---------------------------------------------------------------------------
void VTreeListBox::reset()
{
	if (tree != 0) {
		tree->reset();

#ifndef CV_NO_PAINT_EVENTS
		if (refresh) {
			if (!updateScrollBars()) {
				update();
			}
		}
#endif
	}
}

//----------------------------------------------------------------------------
//  Called when this VTreeListBox should take the keyboard
//  focus for itself. Return TRUE if this VWindow is enabled and not hidden.
//  Override this function to return FALSE if the keyboard focus is not wanted.
//
boolean VTreeListBox::takeFocus()
{
#ifndef CV_NO_FOCUS_EVENTS
    return VView::takeFocus();
#else
	return TRUE;
#endif
}

// ---------------------------------------------------------------------------
// Called when this object has been given the keyboard focus.
//
boolean VTreeListBox::givenFocus()
{
#ifndef CV_NO_FOCUS_EVENTS
	boolean handled = VView::givenFocus();
	inFocus = TRUE;
	if (tree->clickNode) {
		updateNode(tree->clickNode, TRUE);
	}
	return handled;
#else
	return TRUE;
#endif
}

// ---------------------------------------------------------------------------
boolean VTreeListBox::clearFocus()
{
	boolean handled = VView::clearFocus();
	inFocus = FALSE;
	if (tree->clickNode != 0) {
		updateNode(tree->clickNode, TRUE);
	}
	return handled;
}

// ---------------------------------------------------------------------------
void VTreeListBox::uponClick(VObject *clnt, method mthd, method dblmthd,
                 method expmthd, method rclickmthd,
                 method updatescrollmthd, method renamemthd, 
                 method prerenamemthd)
{
   if (clnt != 0) {
      clientObject = clnt;
   }
   if (mthd != 0) {
      selectMthd = mthd;
      selectClient = clnt;
   }
   if (dblmthd != 0) {
      dblClkMthd = dblmthd;
      dblClkClient = clnt;
   }
   if (expmthd != 0) {
      expandMthd = expmthd;
      expandClient = clnt;
   }
   if (rclickmthd != 0) {
     rclickMthd = rclickmthd;
     rclickClient = clnt;
   }

   if (updatescrollmthd != 0) {
     updatescrollMthd = updatescrollmthd;
     updatescrollClient = clnt;
   }

   if (renamemthd != 0) {
     renameMthd = renamemthd;
     renameClient = clnt;
   }

   if (prerenamemthd != 0) {
     preRenameMthd = prerenamemthd;
     preRenameClient = clnt;
   }

   if (clnt == 0) {
      clientObject = 0;
      rclickClient = selectClient = dblClkClient = expandClient = updatescrollClient = renameClient = preRenameClient = 0;
      rclickMthd = selectMthd = dblClkMthd = expandMthd = updatescrollMthd = renameMthd = preRenameMthd = 0;
   }
}
// ---------------------------------------------------------------------------
void VTreeListBox::uponSelect(VObject *clnt, method mthd)
{
   clientObject = clnt;
   selectClient = clnt;
   selectMthd = mthd;
}

// ---------------------------------------------------------------------------
void VTreeListBox::uponDblClick(VObject *clnt, method mthd)
{
   clientObject = clnt;
   dblClkClient = clnt;
   dblClkMthd = mthd;
}

// ---------------------------------------------------------------------------
void VTreeListBox::uponRmouseClick(VObject *clnt, method mthd)
{
   clientObject = clnt;
   rclickClient = clnt;
   rclickMthd = mthd;
}


// ---------------------------------------------------------------------------
void VTreeListBox::uponExpand(VObject *clnt, method mthd)
{
   clientObject = clnt;
   expandClient = clnt;
   expandMthd = mthd;
}

// ---------------------------------------------------------------------------
void VTreeListBox::uponExpanded(VObject *clnt, method mthd)
{
   clientObject = clnt;
   expandedClient = clnt;
   expandedMthd = mthd;
}


// ---------------------------------------------------------------------------
void VTreeListBox::uponUpdateScroll(VObject *clnt, method mthd)
{
   clientObject = clnt;
   updatescrollClient = clnt;
   updatescrollMthd = mthd;
}

// ---------------------------------------------------------------------------
void VTreeListBox::uponNodeRename(VObject *clnt, method mthd)
{
   clientObject = clnt;
   renameClient = clnt;
   renameMthd = mthd;
}

// ---------------------------------------------------------------------------
void VTreeListBox::uponPreNodeRename(VObject *clnt, method mthd)
{
   clientObject = clnt;
   preRenameClient = clnt;
   preRenameMthd = mthd;
}
// ---------------------------------------------------------------------------
boolean VTreeListBox::resized(int w, int h)
{
	if ((w <= 0) || (h <= 0)) {
		return FALSE;
	}

   updateScrollBars();

   // call updatescrolling routine
   if (updatescrollMthd != 0)
       updatescrollClient->perform(updatescrollMthd, (long) this);

   return TRUE;
}

//----------------------------------------------------------------------------
boolean VTreeListBox::addNode(VTreeNode* node,
                              VTreeNode* parent,
                              VTreeNode* sibling)
{

	if ((node == 0) || (parent == 0)) {
		return FALSE;
	}

	boolean rc = parent->addChild(node, sibling);

	node = tree->getTopNode();

	if (refresh && !updateScrollBars() && !isHidden()) {
       updateNode(node, FALSE, node->isExpand());
	}

	return rc;

}

// ---------------------------------------------------------------------------
VTreeNode *VTreeListBox::addNode(const VString& name, VTreeNode *node,
                 VObject *data, boolean isparent,
                 VBitMap *pbmap,
                 const VColor& Color,
                 VFont *pFont,
                 boolean iscontainer)
{
	if (tree == 0) {
		return 0;
	}

	VTreeNode *tn = tree->addNode(name, node, data, isparent, iscontainer);

	if (tn != 0) {
		if (pbmap != 0) {
			tn->setBitMap(pbmap);
		}
		if (Color) {
			tn->setColor(Color);
		}
		if (pFont != 0) {
			tn->setFont(pFont);
		}
	}

	if (node == 0) {
		node = tree->getTopNode();
	}

	// djs 01/16/97 - Check for isHidden() also
	if ( refresh && !isHidden() ) {
		if (!updateScrollBars()) {
			if (!isHidden()) {
				updateNode(node, FALSE, node->isExpand());
			}
		}
		if (!isHidden()) {
			updateNode(node, TRUE, TRUE);
		}
	}

	return tn;
}

// ---------------------------------------------------------------------------
void VTreeListBox::renameNode(VTreeNode *node, const VString& newname)
{
   if (tree) {
      tree->renameNode(node, newname);
      if (refresh) {
     	updateNode(node, TRUE);
      }
   }
}

// ---------------------------------------------------------------------------
void VTreeListBox::renameNode(VTreeNode *node)
{
   renamingNode = node;

   //!!!jmd-test for node not visible
   if (tree != 0 && node != 0) {

      int w, h, x, index;
      VRectangle rect;

      sizeOfImage(&w, &h);
      index = getNodeIndex(node);

      int bw, bh;
      VBitMap *bmap = node->getBitMap();

      if (bmap == 0) {
        bw = bh = 0;
      }
      else {
         bmap->sizeOfImage(&bw, &bh);
      }

      x = node->getLevel() * indentDist + horizontalOffset + bw;
 
	  VEditLineNotify *el;
	  el = new VEditLineNotify(VFrame(x,((index - rowOffset) * rowHeight)-1,
	         		     w - x, rowHeight + 1, // row height
	                     CornerDim),
	                     this,
	                     StyleDefault+StyleBorder+StyleNo3D+StyleHorizontal);

	  VFont *pLastFont = port->currentFont();
      VFont *pUserFont;
      if (pUserFont = node->getFont()) {
        el->setFont(pUserFont);
      }  
      else {
	  	el->setFont(pLastFont);
	  }

	  el->putText(node->getName().gets());

	  int minX, minY;
	  int currX, currY;
	  int newX;
	  
	  currY = rowHeight + 1;
	  currX = w - x;
	  	  
	  el->getMin(&minX, &minY);
	  if ((int)(minX * 1.25) > currX)
	    newX = currX;
	  else
	    newX = (int)(minX * 1.25);

	  el->resize(newX, currY);

#ifndef CV_NO_FOCUS_EVENTS
	  notifier->setFocus(el);
#endif

	  el->selectAll(0);

	  el->uponAccept(this, methodOf(VTreeListBox, nodeRenameAccept));
	  el->uponCancel(this, methodOf(VTreeListBox, nodeRenameCancel));
	}
}

//------------------------------------------------------
void VTreeListBox::nodeRenameAccept(VEditLineNotify *el)
{
   if (tree) {
      VString newText;
	  el->getText(newText);
      tree->renameNode(renamingNode, newText);
   }

  nodeRenameCancel(el);

  if ((renameClient != 0) && (renameMthd != 0)) {
     renameClient->perform(renameMthd, (long)this);
  }
}

//------------------------------------------------------
void VTreeListBox::nodeRenameCancel(VEditLineNotify *el)
{
  this->removeChild(el);
  delete el;
  
  if (refresh) {
    updateNode(renamingNode, TRUE);
  }

  renamingNode = 0;
}

// ---------------------------------------------------------------------------
void VTreeListBox::bitmapNode(VTreeNode *node, VBitMap *bitmap)
{
   if (tree) {
      tree->bitmapNode(node, bitmap);
      if (refresh) {
     updateNode(node, TRUE);
      }
   }
}


// ---------------------------------------------------------------------------
void VTreeListBox::colorNode(VTreeNode *node, const VColor& color)
{
   if (tree) {
      tree->colorNode(node, color);
      if (refresh) {
     updateNode(node, TRUE);
      }
   }
}

// ---------------------------------------------------------------------------
void VTreeListBox::fontNode(VTreeNode *node, VFont *font)
{
   if (tree) {
      tree->fontNode(node, font);
      if (refresh) {
     updateNode(node, TRUE);
      }
   }
}

// ---------------------------------------------------------------------------
// update the thumb positions and ranges of the scroll bars
// returns TRUE if the window was repainted
//
boolean VTreeListBox::updateScrollBars()
{
#ifndef CV_NO_MOUSE_EVENTS
   int max, line, page;
   boolean ret = FALSE;

   int w, h;
   sizeOfImage(&w, &h);

	if ( (w <= 0) || (h <= 0) )
		return FALSE;

   // update scroll bars
   if ( vScroll != 0 ) {
		if ( tree == 0 )
			vScroll->hide();
		else { // coords are in rows
			int hy = tree->getNodeCount();
			line = 1;
			page = h / rowHeight;
			max = hy - page;

			if (max < 0) {
				if (!vScroll->isHidden() || rowOffset != 0) {
					vScroll->hide();
					rowOffset = 0;
					update();
					ret = TRUE;
				}
			}
			else {
#if defined(CV_WINDOWS)
				vScroll->track(rowOffset);
				vScroll->setScrollValues(0, max + page - 1, page + 1,
								 rowOffset, line, page);
#elif defined(PM)
				vScroll->setPos(long(rowOffset));
				vScroll->setValues(long(max), 0L, long(line), long(page));
				MPARAM mp1;
				mp1 = MPFROM2SHORT( page, hy );
				WinSendMsg( vScroll->hwnd(), SBM_SETTHUMBSIZE, mp1, 0L );
#else
				vScroll->setPos(long(rowOffset));
				vScroll->setValues(long(max), long(0L),
								   long(line), long(page));
#endif
				if ( vScroll->isHidden() )
					vScroll->show();
			}
      }
   }

	if ( hScroll != 0 ) {
		if ( tree == 0 )
			hScroll->hide();
		else {
			int max_extent = getMaxHorizontalExtent();
			if (max_extent <= w) {
				if (!hScroll->isHidden())
					hScroll->hide();
			}
			else {
				long pos = -horizontalOffset;
				if ( pos > max_extent - w )
					pos = max_extent - w;

#if defined(CV_WINDOWS)
				hScroll->track(pos);
				hScroll->setScrollValues(0, max_extent, w, pos);
#elif defined(PM)
				hScroll->setPos(pos);
				hScroll->setValues(max_extent, 0L, 1L, w);
				MPARAM mp1;
				mp1 = MPFROM2SHORT( w, max_extent );
				WinSendMsg( hScroll->hwnd(), SBM_SETTHUMBSIZE, mp1, 0L );
#else
				hScroll->setPos(pos);
				hScroll->setValues(max_extent, 0, 1, w);
#endif
				if ( hScroll->isHidden() )
					hScroll->show();
			}
		}
	}

   return ret;
#else
	return TRUE;
#endif
}

// ---------------------------------------------------------------------------
// the vertical scrollbar has changed position (i is in rows)
//
boolean VTreeListBox::vtScrollBar(int i)
{
#ifndef CV_NO_MOUSE_EVENTS
	inScrollCallBack++;

	if ( vScroll && inScrollCallBack <= 1 ) {
		if ( vScroll->getMax() <= 0 )
			i = 0;

		if ( i - rowOffset )
			scrollTree(i - rowOffset, 0);
	}

	inScrollCallBack--;
#endif

	return TRUE;
}
// ---------------------------------------------------------------------------
// the vertical scrollbar has changed position (i is in rows)
//
boolean VTreeListBox::htScrollBar(int i)
{
#ifndef CV_NO_MOUSE_EVENTS   

	inScrollCallBack++;
   
	if ( inScrollCallBack <= 1 ) {
		if (i != -horizontalOffset) {
			int dx = -(i + horizontalOffset);
			horizontalOffset = -i;
			scrollTree(0, dx);
		}
	}

	inScrollCallBack--;
#endif

   return TRUE;
}
// ---------------------------------------------------------------------------
// Scroll the tree the specified amount (in rows)
//
void VTreeListBox::scrollTree(int drow, int dcol)
{
   // adjust offsets
   drow = (-drow > rowOffset) ? -rowOffset : drow;
   rowOffset += drow;

   int w, h;
   sizeOfImage(&w, &h);

#ifndef CV_NO_PAINT_EVENTS
   if (drow * rowHeight > h || -(drow * rowHeight) > h)
      update();
   else
      scroll(dcol, -drow * rowHeight);
#endif

   // call updatescrolling routine
   if (updatescrollMthd != 0)
       updatescrollClient->perform(updatescrollMthd, (long) this);

	updateScrollBars();
}

// ---------------------------------------------------------------------------
//
int VTreeListBox::getMaxHorizontalExtent()
{
	int max_extent = 0;
	if (tree != 0) {
		for (VTreeNode *node = tree->getTopNode() ;
			 node != 0 ; node = findNextNode(node, TRUE)) {
			int extent = getNodeHorizontalExtent(node);
			if (extent > max_extent) {
				max_extent = extent;
			}
		}
	}
	return max_extent;
}

// ---------------------------------------------------------------------------
//
int VTreeListBox::getNodeHorizontalExtent(VTreeNode *node)
{
	if (node == 0) {
		return 0;
	}

	int extent = node->getLevel() * indentDist;

	VBitMap *bitmap = node->getBitMap();
	if (bitmap != 0) {
		int w, dummy;
		bitmap->sizeOfImage(&w, &dummy);
		extent += w + 2;
	}

	const char *string = node->getName().gets();
	if (string != 0) {
		int w, dummy;
		if (port->open()) {
			VFont *this_font = node->getFont();
			if (this_font == 0) {
				this_font = font;
			}
			VFont *save_font = port->currentFont();
			port->useFont(this_font);
			port->textSize(string, &w, &dummy);
			port->useFont(save_font);
			extent += w + 4;
			port->close();
		}
	}

	return extent;
}

// ---------------------------------------------------------------------------
// update the node, if paintbelow is TRUE, paint all nodes below
// if justtext, don't paint the whole line (ignored if paintbelow)
//
void VTreeListBox::updateNode(VTreeNode *node, boolean justtext,
							  boolean paintbelow)
{
   //!!!jmd-test for node not visible

   if ((tree != 0) && (node != 0)) {

      int w, h, x, index;
      VRectangle rect;

      sizeOfImage(&w, &h);
      index = getNodeIndex(node);

		// !!!jmd-test for bitmap
		if (rowHilite)
			x = 0;
		else {
			if (!justtext || paintbelow)
				x = 0;
  		   else
				x = node->getLevel() * indentDist + horizontalOffset;
		}

      rect.set(CornerDim, x, (index - rowOffset) * rowHeight, w - x,
        (paintbelow ? h - ((index - rowOffset) * rowHeight) : rowHeight));

#ifndef CV_NO_PAINT_EVENTS
      update(&rect);
#endif
   }
}

// ---------------------------------------------------------------------------
//
VImage *VTreeListBox::getImage(VBitMap *bitmap) const
{
#ifndef CV_NOVIMAGE
	if (theImages != 0) {
		DO (*theImages, VImage, image)
			if (image->getOriginalBitmap() == bitmap) {
				return image;
			}
		END
	}
	VImage *image = new VImage(bitmap);
	if (image != 0) {
		image->mapColor(VColor(VColor::White),
						VColor::system(VColorScheme::FlatBackground));
		if (theImages == 0) {
			((VTreeListBox *)this)->theImages = new VOrdCollect;
			if (theImages == 0) {
				delete image;
				return 0;
			}
		}
		theImages->add(image);
	}
	return image;
#else
	return 0;
#endif
}

// ---------------------------------------------------------------------------
boolean VTreeListBox::paint()
{
#ifndef CV_NO_PAINT_EVENTS
   VTreeNode  *node;
   int        i, count, indent, y, y1, adj;
   VRectangle rect;
   VFont      *pLastFont;
   VPen       userPen;

   if ( tree == 0 || port == 0 )
      return TRUE;

   VRectangle *exprect = notifier->exposedRect();

   if ( !(exprect->getWidth() > 0 && exprect->getHeight() > 0) )
		return FALSE;

   int ww, wh;

   sizeOfImage(&ww, &wh);
	port->open();
	for (i = rowOffset, y = 0;; i++) {
		if ( (node = tree->getNodeAt(i)) == 0 )
			break;

		int bw=0, bh=0;
		VBitMap *bmap = node->getBitMap();

		if ( bmap != 0 )
			bmap->sizeOfImage(&bw, &bh);

		pLastFont = port->currentFont();
		VFont *pUserFont;
		if ( pUserFont = node->getFont() )
        port->useFont(pUserFont);

		int tw, th;
		port->textSize((char *)node->getName().gets(), &tw, &th);

		if ( rowHilite )
			rect.set(CornerDim, 0, y, ww, rowHeight);
		else
			rect.set(CornerDim, node->getLevel() * indentDist +
						bw + 2 + horizontalOffset,
						y, tw + 4, rowHeight);

		if (tree->getSelectedNode().findSame((VObject *)node)) {
			port->useBrush(&VBrush::system(VColorScheme::FlatBackgroundSelect));
			port->fillRegion(&rect, FALSE);
			port->usePen(&VPen::system(VColorScheme::FlatForegroundSelect));
		}
		else if (node->colorHasBeenSet) {
			userPen.color(node->getColor());
			port->usePen(&userPen);
		}
		else if (isEnabled())
			port->usePen(&VPen::system(VColorScheme::FlatForeground));
		else
			port->usePen(&VPen::system(VColorScheme::ShadedShadow));

		int ty = y + (rowHeight / 2) - (th / 2) - 1;
		port->wrtText(node->getName(), 
                    node->getLevel() * indentDist + bw + 4 + horizontalOffset, ty);
		port->useFont(pLastFont);

		if ( inFocus && (node == tree->clickNode) ) {
#if defined(CV_WINDOWS)
			HDC hdc = port->getHDC();
			RECT r; int left, right, top, bottom;
			rect.get(Corners, &left, &right, &top, &bottom);
			r.left = left;  r.right  = top;
			r.top  = right; r.bottom = bottom;
			SetBkColor(hdc, COLOR_HIGHLIGHT);
			SetTextColor(hdc, RGB(255, 255, 255));
			DrawFocusRect(hdc, &r);
#endif
		}

		if ( bmap != 0 ) {
			int by = y + (rowHeight / 2) - (bh / 2);
#ifndef CV_NOVIMAGE
			VImage *image = getImage(bmap);
			if ( image != 0 ) {
				image->draw(port, node->getLevel() * indentDist + horizontalOffset, by,
				            0, 0, VImage::MappedOpaque);
			}
			else
				port->drawBitMap(bmap, node->getLevel() * indentDist + horizontalOffset, by);
#else
			port->drawBitMap(bmap, node->getLevel() * indentDist + horizontalOffset, by);
#endif
		}

		// draw connecting lines
		y1 = y + rowHeight / 2;
		indent = node->getLevel() * indentDist;

		port->usePen(&VColorScheme::systemPen(VColorScheme::ShadedShadow));

		if ( drawTopLevelLines || !tree->isTopLevelNode(node) ) {
			if ( node->getPrev() != 0 ) {
				// connect to previous sibling
				count = tree->expChildCount(node->getPrev());
				adj = (node->getPrev()->getContainer()) ? expSize / 2 : 0;

				drawConnectingLine(indent + horizontalOffset +2,
							   y1,
							   indent - indentDist +
							   			indentOffset + horizontalOffset +1, y1);
				drawConnectingLine(indent - indentDist +
										indentOffset + horizontalOffset,
							   y1 +1,
							   indent - indentDist +
							   			indentOffset + horizontalOffset,
							   y1 - (rowHeight * count) + adj +1);
			}
			else {
				// connect to parent
				drawConnectingLine(indent + horizontalOffset +2,
							   y1,
							   indent - indentDist + indentOffset + horizontalOffset +1,
							   y1);
				if (i != 0) {
					drawConnectingLine(indent - indentDist +
										indentOffset + horizontalOffset,
							   		y1 +1,
							   		indent - indentDist +
							   		indentOffset + horizontalOffset,
							   		y1 - (rowHeight / 2) );
				}
			}
		}

		port->usePen(0);

		// draw expand/contract button
		if (expandEnabled && node->getContainer()) {
			int rx = (node->getLevel() - 1) * indentDist + indentOffset;
//            int rx = indentDist - indentDist / 2;
			int ry = y + rowHeight / 2;
			rect.set(CenterDim, rx + horizontalOffset, ry, expSize, expSize);

			port->usePen(&VPen::system(VColorScheme::ShadedShadow));
			port->useBrush(&VBrush::system(VColorScheme::FlatBackground));
			port->fillRegion(&rect, TRUE);

			if (isEnabled())
				port->usePen(&VPen::system(VColorScheme::FlatForeground));

			port->moveTo(rx - 2 + horizontalOffset, ry);
			port->lineTo(rx + 3 + horizontalOffset, ry);

			if (!node->isExpand()) {
				// draw '+' in button
				port->moveTo(rx + horizontalOffset, ry - 2);
				port->lineTo(rx + horizontalOffset, ry + 3);
			}
		}

		int bottom;
		exprect->get(Corners, 0, 0, 0, &bottom);

		#ifdef PM
		if ( hScroll && !hScroll->isHidden() ) {
			int sw, sh;
			hScroll->sizeOfImage( &sw, &sh );
			bottom -= sh;
		}
		#endif

		if (y > bottom) {
			VTreeNode* nextNode = 0;

			while (node) {
				node = node->getParent();
				if (node) {
					nextNode = node->getNext();
					if (nextNode)
						break;
				}
			}

			if (nextNode) { // draw next node
				int offset = -i;
				i = getNodeIndex(nextNode) - 1;
				offset += i;
				y += offset * rowHeight;
			}
			else // no more connections possible
				break;
		}

		y += rowHeight;
	}

	port->close();
   delete exprect;
#endif

   return TRUE;
}

// ---------------------------------------------------------------------------
//
void VTreeListBox::drawConnectingLine(int x1, int y1, int x2, int y2)
{

#if defined(CV_WINDOWS) && !defined(CV_REMOTE_GUI)

	if (stippleBrush == 0) {
		stippleBrush =
			new VBrush(StipplePat,
					   VColor::system(VColorScheme::ShadedShadow),
					   VColor::system(VColorScheme::FlatBackground));
	}
	HDC hdc = port->getHDC();
	HBRUSH hbrush = stippleBrush->getHndl();
	RECT r;
	r.left = x1; r.right  = x2; if (r.left == r.right)  { r.right  += 1; }
	r.top  = y1; r.bottom = y2; if (r.top  == r.bottom) { r.bottom += 1; }
	UnrealizeObject(hbrush);
#ifdef CV_WIN32
	SetBrushOrgEx(hdc, x1 % 8, y1 % 8, 0);
#else
	SetBrushOrg(hdc, x1 % 8, y1 % 8);
#endif
	SelectObject(hdc, hbrush);
	SetBkColor(hdc, GetSysColor(COLOR_BTNSHADOW));
	SetTextColor(hdc, GetSysColor(COLOR_WINDOW));
	FillRect(hdc, &r, hbrush);

#else

	port->moveTo(x1, y1);
	port->lineTo(x2, y2);

#endif

}

// ---------------------------------------------------------------------------
//
void VTreeListBox::systemColorChanged()
{
#ifndef CV_NOVIMAGE
	if (theImages != 0) {
		DO (*theImages, VImage, image)
			image->systemColorChanged();
			image->mapColor(VColor(VColor::White),
							VColor::system(VColorScheme::FlatBackground));
		END
	}
#endif
	if (stippleBrush != 0) {
		//delete stippleBrush;
		//stippleBrush = 0;
	}
}

// ---------------------------------------------------------------------------
// Select the specified node
// if nopaint, don't paint or notify tree
//
void VTreeListBox::gotoNode(VTreeNode *node, boolean nopaint,
                            boolean expand, boolean fromClickAt)
{
	VTreeNode *oldNode;

	if ( tree == 0 )
		return;

	inGotoNode++;

	boolean fSelected = TRUE;

#ifndef CV_NO_PAINT_EVENTS
	if ( !nopaint ) {
		if (fMultiSel)
			oldNode = tree->clickNode;
		else
			oldNode = (VTreeNode *) (tree->getSelectedNode().idAt(0));

		VTreeNode *lastNode = getCurrNode();

		tree->clickNode = node;
		boolean multipleSelect = fMultiSel && (notifier->keyStatus(K_Control));
		boolean shiftSelect = fMultiSel && (notifier->keyStatus(K_Shift));

		if ((shiftSelect) && (lastNode != node)) {
			VTreeNode *nextNode;
			deselectAll();
			boolean lastHigher = FALSE;
			nextNode = lastNode;
			while ((nextNode != 0) && (nextNode != node) && (!lastHigher)) {
				nextNode = findNextNode(nextNode, FALSE);
				lastHigher = (nextNode == node);
			}
			if (lastHigher) { // lastNode higher than node
				tree->selectNode(lastNode, this, TRUE);
				nextNode = findNextNode(lastNode, FALSE);
				while ((nextNode != 0) && (nextNode != node)) {
					tree->selectNode(nextNode, this, TRUE);
					nextNode = findNextNode(nextNode, FALSE);
				}
				fSelected = tree->selectNode(node, this, TRUE);
			}
			else { // node higher than lastNode
				tree->selectNode(node, this, TRUE);
				nextNode = findNextNode(node, FALSE);
				while ((nextNode != 0) && (nextNode != lastNode)) {
					tree->selectNode(nextNode, this, TRUE);
					nextNode = findNextNode(nextNode, FALSE);
				}
				fSelected = tree->selectNode(lastNode, this, TRUE);
			}
			update();
		}
		else if (fMultiSel && !multipleSelect)
			preMultiSelect();	

		fSelected = tree->selectNode(node, this, multipleSelect);
	  
		if (!fSelected) ;
		else { // redraw old node
			if ((oldNode != 0) && (oldNode != node))
				updateNode(oldNode, TRUE);
		}

		// draw new node
		if (node != 0)
			updateNode(node, TRUE);
	}
#endif

   // test if new node is visible
	if (node != 0) {
		int ww, wh;
		sizeOfImage(&ww, &wh);

		int index, k;
		index = getNodeIndex(node);

		if (index < rowOffset)
			scrollTree(index - rowOffset, 0);
      else if (index >= (k = rowOffset + (wh / rowHeight)))
			scrollTree(index - k + 1, 0);
   }

//   if (inGotoNode == 1 && fSelected) {
	if (inGotoNode == 1 && (fSelected || !optimizeClick)) {
		// notify client
		if (node != 0 && selectClient != 0 && selectMthd != 0) {
			if ( fromClickAt )
				doSelectCallback = TRUE;
			else
				selectClient->perform(selectMthd, (long) node);
		}
	}

	// if fSelected == FALSE, its not already selected, and not being selected to expand
	if (inGotoNode == 1  && !fSelected && !expand && renameEnabled)
		doRenameNode = TRUE;
//      renameNode(node);

	inGotoNode--;
}

// ---------------------------------------------------------------------------
// !!!jmd make this more efficient !!!
// if expand is TRUE, only count expanded branches
//
int VTreeListBox::getNodeIndex(VTreeNode *node, boolean expand)
{
   if (tree != 0) {
      return tree->getNodeIndex(node, expand);
   }

   return 0;
}

// ---------------------------------------------------------------------------
// Return node at mouse position
//
VTreeNode* VTreeListBox::clickAt(int mx, int my, boolean fLeftMouse)
{
   VTreeNode *node;
   VTreeNode *selectedNode;
   VRectangle rect;
   int row = my / rowHeight;

   if (tree == 0 || port == 0) {
      return 0;
   }

   if ((node = tree->getNodeAt(row + rowOffset)) != 0) {
    if (expandEnabled && node->getContainer()) {
     // test for expandor hit
     int expX = (node->getLevel() - 1) * indentDist +
	 		 indentOffset + horizontalOffset - 7;
     int expY = row * rowHeight;
     rect.set(CornerDim, expX, expY, expSize + 10, rowHeight);

     if (fLeftMouse && rect.pointIn(mx, my)) {
	    selectedNode = getCurrNode();
		if (tree->isParentOf(selectedNode, node)) {
          gotoNode(node, FALSE, TRUE, TRUE);
		}
        expandNode(node);
        return 0;
     }
    }

    getNodeRect(node, row, rect);

    if (rect.pointIn(mx, my)) {
     	gotoNode(node, FALSE, !fLeftMouse, TRUE);
     	return node;
    }
   }

   return 0;
}

// ---------------------------------------------------------------------------
// Right Mouse pressed in window
//
boolean VTreeListBox::rmouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return TRUE;
	}

   VWindow *pParent = getParent();
   if (pParent) {
#ifndef CV_NO_FOCUS_EVENTS
		if (!VWindow::checkLosingFocus()) {
			this->takeFocus();
		}
#endif

      VTreeNode* node = clickAt(mx, my, FALSE);
      m_rmouseMx = mx;
      m_rmouseMy = my;

      if (node != 0 && rclickClient != 0 && rclickMthd != 0) {
         rclickClient->perform(rclickMthd, (long) node);
      }
      return TRUE;
   }
   return FALSE;
}

boolean VTreeListBox::popupMenuAt(VPopupMenu *pPopup)
{
    if (pPopup) {
    mapToScreen(&m_rmouseMx, &m_rmouseMy);
    pPopup->popupAt(m_rmouseMx, m_rmouseMy);
    return TRUE;
    }
    else {
    return FALSE;
    }
}

// ---------------------------------------------------------------------------
// Mouse pressed in window
//
boolean VTreeListBox::mouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return TRUE;
	}

   VTreeNode *node;

   doSelectCallback = FALSE;
   doRenameNode = FALSE;

   node = clickAt(mx, my, TRUE);

#ifndef CV_NO_DRAG_AND_DROP
   if (node && dragEnabled) {
      VRectangle rect;

      mouseNode = node;
      mouseState = Drag;
      mouseRect.set(CenterDim, mx, my, rowHeight/2, rowHeight/2);
      mouseRect.combine(&rect, RgnIntersect);
      notifier->captureMouseFor(this);
      notifier->mouseTracking(TRUE);
   }
#endif

   return TRUE;
}

// ---------------------------------------------------------------------------
// Fill in the rectangle with the location of the node text
// (used for mouse selection)
//
void VTreeListBox::getNodeRect(VTreeNode *node, int row, VRectangle& rect)
{
   if ((node != 0) && (port != 0)) {
		if (rowHilite) {
			int ww, dummy;
			sizeOfImage(&ww, &dummy);
			rect.set(CornerDim, 0, row * rowHeight, ww, rowHeight);
		}
		else {
			int tw, bw, dummy;
			port->open();
			port->useFont(font);
			port->textSize((char *)node->getName().gets(), &tw, &dummy);
			port->close();
			VBitMap *bitmap = node->getBitMap();
			if (bitmap == 0) {
				bw = 0;
			}
			else {
				bitmap->sizeOfImage(&bw, &dummy);
			}
			rect.set(CornerDim, node->getLevel() * indentDist +
								horizontalOffset,
								row * rowHeight, bw + 2 + tw + 4, rowHeight);
		}
	}
}

// ---------------------------------------------------------------------------
boolean VTreeListBox::mouseDbl(int mx, int my)
{
	if (!isEnabled()) {
		return TRUE;
	}

//   mouseDn(mx, my); // dgm

   if (tree == 0) {
      return TRUE;
   }

#if 0
   VTreeNode *node = getCurrNode();
#else
   VTreeNode *node = clickAt(mx, my, TRUE);
#endif

   if (node != 0) {
      VRectangle rect;
      int row = my / rowHeight;
      getNodeRect(node, row, rect);

      if (rect.pointIn(mx, my) && dblClkClient != 0 && dblClkMthd != 0) {
         dblClkClient->perform(dblClkMthd, (long) node);
      }
   }

   return TRUE;
}

// ---------------------------------------------------------------------------
// Mouse moved in window
//
boolean VTreeListBox::mouseMv(int mx, int my, int bStat)
{
    if (tree == 0 || port == 0) {
        return TRUE;
    }

#ifndef CV_NO_MOUSE_EVENTS
    // drag
    if (bStat && mouseNode != 0 && mouseState == Drag) {
        if (!mouseRect.pointIn(mx, my)) {
            notifier->releaseCapture();

            VObject*  pObject = getObject(mouseNode);
            if (pObject) {
#if !defined(CV_NO_DRAG_AND_DROP) && !defined(CV_NOARCHIVER)
                VDragItem dragItem;
                // set never drop cursor if exists
                if (pneverDropCursor) {
                    g_DragManager->setneverDropCursor(pneverDropCursor);
                }
                dragItem.put(pObject, FALSE);
                g_DragManager->trackDrag(this, &dragItem);
                // reset neverdrop cursor
                g_DragManager->setneverDropCursor();
#endif
            }
            notifier->mouseTracking(FALSE);
            mouseState = Normal;
            mouseNode  = 0;
        }
    }
#endif

   return TRUE;
}

// ---------------------------------------------------------------------------
// Mouse released in window
//
boolean VTreeListBox::mouseUp(int mx, int my)
{
   if (tree == 0) {
      return TRUE;
   }

#ifndef CV_NO_MOUSE_EVENTS
   if (doSelectCallback) {
     doSelectCallback = FALSE;
	 VTreeNode *node = getCurrNode();
   	 selectClient->perform(selectMthd, (long) node);
   }

   if (doRenameNode) {
     doRenameNode = FALSE;
	 VTreeNode *node = getCurrNode();
	 boolean rc = TRUE;
	 if ((preRenameMthd != 0) && (preRenameClient != 0)) {
	    rc = preRenameClient->perform(preRenameMthd, (long) node);
	 }
	 if (rc) {
   	   renameNode(node);
	 }
   }

   // drag
   if (mouseNode != 0 && mouseState == Drag) {
      // reset neverdrop cursor
      notifier->mouseTracking(FALSE);
      notifier->releaseCapture();
      mouseNode  = 0;
   }
   else if (mouseNode != 0) {
      notifier->mouseTracking(FALSE);
      notifier->releaseCapture();
   }

   mouseState = Normal;
#endif
   return TRUE;
}

// ---------------------------------------------------------------------------
// Key pressed within the VTreeListBox
//
boolean VTreeListBox::key(int k, char ch)
{
	if (!isEnabled()) {
		return TRUE;
	}

   VTreeNode *node, *tn;

   node = getCurrNode();

#ifndef CV_NO_KEY_EVENTS
   switch(k) {
   case K_Up:
      if ((tn = findPrevNode(node)) != 0) {
     gotoNode(tn);
      }
      break;

   case K_Down:
      if ((tn = findNextNode(node, TRUE)) != 0) {
		gotoNode(tn);
      }
      break;

   case K_space:
      expandNode(getCurrNode());
      break;

   case K_F2:
//    promoteNode(getCurrNode());
      break;

   case K_F3:
//    demoteNode(getCurrNode());
      break;

   case K_Return:
      if (node != 0 && dblClkClient != 0 && dblClkMthd != 0) {
         dblClkClient->perform(dblClkMthd, (long) node);
      }
      break;


   case K_Right:
		if ((node != 0) && ((tn = node->getFirst()) != 0)) {
		  	if (!node->expand) {
				expandNode(node);
			}
			else {
				gotoNode(tn);
			}
		}
		break;

   case K_Left:
		if ((node != 0) && (tree != 0)) {
			if (node->expand) {
				expandNode(node);
			}
			else if ((tn = node->getParent()) != tree->getTopNode()) {
				gotoNode(tn);
			}
		}
		break;

   case K_End:
// KC 4/5/95 getNodeCount() seems to return 1 more than actually exists!
      if ((tree != 0) &&
	  	  ((tn = tree->getNodeAt(tree->getNodeCount() - 2)) != 0)) {
	     gotoNode(tn);
      }
      break;

   case K_Home:
      if ((tree != 0) && ((tn = tree->getNodeAt(0)) != 0)) {
	     gotoNode(tn);
      }
      break;

  case K_Next:
  case K_Prior:
  {
      int w, h, page;

      sizeOfImage(&w, &h);
      page = h / rowHeight;
      if (k == K_Next)
     scrollTree(page, 0);
      else
    scrollTree(page * -1, 0);
      break;
  }

   default:
		if ((k >= K_A) && (k <= K_Z)) {
			while (TRUE) {
				if (!(tn = findNextNode(node))) {
					break;
				}
				if (toupper(tn->getName().charAt(0)) == toupper(k)) {
					gotoNode(tn);
					break;
				}
				else {
					node = tn;
				}
			}
			break;
		}
		else if (VView::key(k, ch)) {
			return TRUE;
		}
		else if ((parent != 0) && !isTopLevel()) {
			return parent->key(k, ch);
		}
		else {
			return FALSE;
		}
	}
#endif

   return TRUE;
}

// ---------------------------------------------------------------------------
VTreeNode *VTreeListBox::findNextNode(VTreeNode *node, boolean expand)
{
   if (tree == 0) {
      return 0;
   }

   return tree->findNext(node, expand);
}

// ---------------------------------------------------------------------------
VTreeNode *VTreeListBox::findPrevNode(VTreeNode *node, boolean expand)
{
   if (tree == 0) {
      return 0;
   }

   node = tree->findPrev(node, expand);

   return (node == tree->topNode) ? 0 : node;
}

// ---------------------------------------------------------------------------
// Expand the specified node
// if nonotify, don't notify tree
//
void VTreeListBox::expandNode(VTreeNode *node, boolean nonotify)
{
   if (tree != 0 && node != 0 && node->getContainer() != 0) {
      // call expand callback
      if (!nonotify && node != 0 && expandClient != 0 && expandMthd != 0) {
          boolean rc = expandClient->perform(expandMthd, (long) node);
          if (rc && checkExpandCallback)
            return;
      }
      if (!nonotify) {
          if (!node->isExpand() && node->getFirst() == 0) {
             node->setContainer(FALSE);
          }
          else {
             tree->setNodeExpand(node, !node->isExpand(), this);
          }
      }
      if (refresh && !updateScrollBars()) {
          updateNode(node, FALSE, node->getContainer());
      }
	  if ((expandedClient != 0) && (expandedMthd != 0)) {
		  expandedClient->perform(expandedMthd, long(node));
	  }
   }
}

// ---------------------------------------------------------------------------
boolean VTreeListBox::promoteNode(VTreeNode *node)
{
   if (tree != 0) {
      tree->promoteNode(node);
#ifndef CV_NO_PAINT_EVENTS
      if (refresh && !updateScrollBars()) {
     update();
      }
#endif
   }

   return TRUE;
}

// ---------------------------------------------------------------------------
boolean VTreeListBox::demoteNode(VTreeNode *node)
{
   if (tree != 0) {
      tree->demoteNode(node);
#ifndef CV_NO_PAINT_EVENTS
      if (refresh && !updateScrollBars()) {
     update();
      }
#endif
   }

   return TRUE;
}

// ---------------------------------------------------------------------------
void VTreeListBox::removeNode(VTreeNode *node)
{
   if (tree != 0 && node) {
      tree->removeNode(node);
#ifndef CV_NO_PAINT_EVENTS
      if (refresh && !updateScrollBars()) {
     update();
      }
#endif
   }
}

// ---------------------------------------------------------------------------
boolean VTreeListBox::moveNode(VTreeNode *node, VTreeNode *prev)
{
   if (tree != 0 && node && prev) {
      tree->moveNode(node, prev);
#ifndef CV_NO_PAINT_EVENTS
      if (refresh && !updateScrollBars()) {
     update();
      }
#endif
   }

   return TRUE;
}

// ---------------------------------------------------------------------------
void VTreeListBox::deSelectNode(VTreeNode *node)
{
   if(tree != 0) {
     tree->deSelectNode(node);
   }
}

// ---------------------------------------------------------------------------
VOrdCollect& VTreeListBox::getSelectedNodes()
   // Return a list of the currently selected nodes.
{

    return tree->getSelectedNode();
}

//----------------------------------------------------------------------------
boolean VTreeListBox::isNodeSelected(VTreeNode *node)
{
    if (tree->getSelectedNode().findSame((VObject *)node))
    return TRUE;
    else
    return FALSE;
}

//----------------------------------------------------------------------------
VTreeNode *VTreeListBox::getNodeAt(int mx, int my, HiliteState state)
{
   if (tree == 0 || port == 0)
      return 0;
   int row;
   switch (state) {
      case OnNode:
     row = my / rowHeight;
     break;
      case BetweenNode:
     row = my - rowHeight / 2;
     if (row < 0) {
        if (rowOffset == 0)
           return 0;
        else
           row = -1;
     }
     else {
        row = row / rowHeight;
     }
     break;
   }
   VTreeNode* newNode = tree->getNodeAt(row + rowOffset, TRUE);
   return newNode;
}

//----------------------------------------------------------------------------
VObject* VTreeListBox::getObject(VTreeNode* overNode)
{
   VObject* pObject;

   pObject = overNode->getData();
   if (!pObject) {
      pObject = &overNode->getName();
   }
   return pObject;
}

// ---------------------------------------------------------------------------
// update the node, if paintbelow is TRUE, paint all nodes below
// if justtext, don't paint the whole line (ignored if paintbelow)
//
void VTreeListBox::hiliteNode(VTreeNode *node, HiliteState state)
{
   if (tree != 0) {
      if (!node && state == OnNode)
     return;

      port->open();

      VRectangle rect;
      int index = -1;
      int level = 1;
      int tw, th;
      int ww, wh;
      int bw, bh;

      VBitMap *bmap = 0;

      tw = th = 0;
      ww = wh = 0;
      bw = bh = 0;

      sizeOfImage(&ww, &wh);

      if (node) {
     index = getNodeIndex(node);
     level = node->getLevel();
     bmap = node->getBitMap();
      }

      if (bmap) {
     bmap->sizeOfImage(&bw, &bh);
      }

      switch (state) {
     case BetweenNode:
        rect.set(CornerDim,
             level * indentDist + horizontalOffset,
             (index - rowOffset + 1) * rowHeight - 1,
             ww/2,
             2);
        break;

     case OnNode:
        port->textSize((char *)node->getName().gets(), &tw, &th);

        if (rowHilite) {
           rect.set(CornerDim,
            0,
            (index - rowOffset) * rowHeight,
            ww,
            rowHeight);
        }
        else {
           rect.set(CornerDim,
            level * indentDist + bw + 2 + horizontalOffset,
            (index - rowOffset) * rowHeight,
            tw + 4,
            rowHeight);
        }
        break;
      }


      VPen hiPen(VColor::Magenta);

      port->usePen(&hiPen);

      port->rule(XorRule);

      port->fillRegion(&rect);

      port->rule(CopyRule);

      port->usePen(0);

      port->close();
   }
}

// --------------------------------------------------------------------------
void VTreeListBox::setTreeListBox(int minrowhgt, const VStyle& style)
{
    if (rowHeight <= minrowhgt) {
       rowHeight = minrowhgt;
    }

    if (style.contains(StyleFlatList)) {
       indentDist = 2;
    }

    fMultiSel = style.contains(StyleMultiSelect) ? TRUE : FALSE;

#ifndef CV_NO_PAINT_EVENTS
    update();
#endif
}

//----------------------------------------------------------------------------
void VTreeListBox::deselectAll()
{
	boolean changed = FALSE;

	REVERSEDO (getSelectedNodes(), VTreeNode, aNode) 
		tree->deSelectNode(aNode);
		changed = TRUE;
	END

#ifndef CV_NO_PAINT_EVENTS
	if (refresh && changed) {
		update();
	}
#endif
}

//----------------------------------------------------------------------------
void VTreeListBox::preMultiSelect()
{
  if (getSelectedNodes().count() > 1) 
    deselectAll();
}

// ---------------------------------------------------------------------------
//
void VTreeListBox::enableBool(boolean b)
{
	if (b) {
		if (isEnabled()) {
			return;
		}
	}
	else if (!isEnabled()) {
		return;
	}
	VView::enableBool(b);
	update();
}

// ---------------------------------------------------------------------------
//
boolean VTreeListBox::paletteChanged(boolean)
{

#if !defined(CV_NOVIMAGE) && defined(CV_WINDOWS)

	unsigned int nmapped = 0;

	if (theImages != 0) {
		HDC hdc = getHndl();
		if (hdc != 0) {
			HPALETTE hpalette, old_hpalette = 0;
			boolean palette_changed = FALSE;
			DO (*theImages, VImage, image)
				VBitMap *bitmap = image->getMappedBitmap();
				if (bitmap != 0) {
					hpalette = bitmap->getPalette();
					HPALETTE hop = SelectPalette(hdc, hpalette, FALSE);
					if (old_hpalette == 0) {
						old_hpalette = hop;
					}
					if (RealizePalette(hdc) > 0) {
						palette_changed = TRUE;
					}
				}
			END
			if (old_hpalette != 0) {
				SelectPalette(hdc, old_hpalette, FALSE);
			}
			if (palette_changed) {
				InvalidateRect(hWnd, NULL, TRUE);
			}
			ungetHndl();
		}
	}

	return nmapped;

#else

	return 0;

#endif

}

#ifdef PM
//---------------------------------------------------------------
//
// Overload ::sizeOfImage to account for scrollbars on OS/2
//
void VTreeListBox::sizeOfImage(int *w, int *h)
{
	VView::sizeOfImage( w, h );

	int sw, sh;
	if ( vScroll && !vScroll->isHidden() ) {
		vScroll->sizeOfImage( &sw, &sh );
		*w -= sw;
	}
	if ( hScroll && !hScroll->isHidden() ) {
		hScroll->sizeOfImage( &sw, &sh );
		*h -= sh;
	}
}
#endif

// --------------------------------------------------------------------------
//
VStyle& VTreeListBox::getClassDefaultStyle(void)
{
	static VWinStyle CV_NEARDATA default_style
								 (VView::getClassDefaultStyle() +
								  setTransient +
								  StyleHorizontal +
								  StyleVertical);
	return default_style;
}

// --------------------------------------------------------------------------
//
VStyle& VTreeListBox::getDefaultStyle(void) const
{
	return getClassDefaultStyle();
}

// ===========================================================================
#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
void VTreeListBox::putTo(VArchiver& a)
{
    VTreeListBox::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
void VTreeListBox::putTo(VArchiver& a, boolean most_derived)
{
    // Write the base object data.
    VView::putTo(a, FALSE);

    // Write the object version tag.
    a << VTreeListBoxArchiveTag;

    // Write any object specific data.
    a << fMultiSel;
    a << rowHeight;
    a << rowHilite;
    a << optimizeClick;
    a << dragEnabled;
    a << expandEnabled;

    // Write any methods.
	// the renameNode callback is not archived yet

    a.putObject(notifier->
                getClientName(clientObject, clientObjectIndex), TRUE);

    const char *class_name = 0, *method_name = 0;
    VCallbackList::findCallback(method(selectMthd),
                                class_name, method_name,
                                selectMthdIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(dblClkMthd),
                                class_name, method_name,
                                dblClkMthdIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(rclickMthd),
                                class_name, method_name,
                                rclickMthdIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(expandMthd),
                                class_name, method_name,
                                expandMthdIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(expandedMthd),
                                class_name, method_name,
                                expandedMthdIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(updatescrollMthd),
                                class_name, method_name,
                                updatescrollMthdIndex);
    a << (char *)class_name;
    a << (char *)method_name;


    // Write any children.
    VTree *theTree =0;
    theTree = getTree();
    a.putObject(theTree);

    if (most_derived) {
        VWindow::putChildrenTo(a);
    }
}

// ---------------------------------------------------------------------------
void VTreeListBox::getFrom(VArchiver& a)
{
    long viewTag;

    // Read the base object data.
    VView::getFromWithTag(a, &viewTag);

    if (getArchiveTagRev(viewTag) < 2){
        return;
    }

    // Read object version tag and verify.
    long    tag;
    a >> tag;
    if (!archiveTagOk(tag, VTreeListBoxArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VTreeListBoxArchiveTag));
        return;
    }

    // Read any object specific data.
    a >> fMultiSel;
    a >> rowHeight;
    a >> rowHilite;
    a >> optimizeClick;
    a >> dragEnabled;
    a >> expandEnabled;


    VString client_name;
    a.getObject(&client_name);
    clientObject = notifier->getClient(client_name, &clientObjectIndex);
    rclickClient = selectClient = dblClkClient = expandClient = expandedClient = updatescrollClient = renameClient = preRenameClient = clientObject;

    char method_name[256], *method_name_ptr = method_name;
    char class_name[256], *class_name_ptr = class_name;
    method m;

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
                                    &selectMthdIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
        uponSelect(clientObject, m);
    }
	else {
	    selectClient = 0;
	}

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
                                    &dblClkMthdIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
        uponDblClick(clientObject, m);
    }
	else {
		dblClkClient = 0;
	}

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
                                    &rclickMthdIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
        uponRmouseClick(clientObject, m);
    }
	else {
		rclickClient = 0;
	}

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
                                    &expandMthdIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
        uponExpand(clientObject, m);
    }
	else {
		expandClient = 0;
	}

    if (getArchiveTagRev(tag) >= 1) {
	    a >> class_name_ptr;
	    a >> method_name_ptr;
	    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
	                                    &expandedMthdIndex);
	    if ((clientObject != 0) && (m != 0)) {
	        if (!clientObject->isA(VClass::of(class_name_ptr))) {
	            a.abort(VArchiver::ClientCallbackMismatch); return;
	        }
	        uponExpanded(clientObject, m);
	    }
		else {
			expandedClient = 0;
		}
	}

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
                                    &updatescrollMthdIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
        uponUpdateScroll(clientObject, m);
    }
	else {
		updatescrollClient = 0;
	}

	// renameMthd isn't archived yet, preRenameMthd
	renameMthd = 0;
	renameClient = 0;
	preRenameMthd = 0;
	preRenameClient = 0;

    VTree *theTree  =0;
    theTree = (VTree *) a.getObject(0);


}

// ---------------------------------------------------------------------------
void VTreeListBox::getFrom(VArchiver& a, VObject *pwin,
                            boolean do_realize)
{
    VTreeListBox::getFrom(a);

    if (do_realize){
        VTreeListBox::realize((VWindow *)pwin);
    }

    if (pwin != 0){
        VView::getChildrenFrom(a, do_realize);
    }
}


// ---------------------------------------------------------------------------
void VTreeListBox::realize(VWindow *parent)
{
#if defined(CVS_USE_TRANSIENT) && defined(CVS_LATER_DEFAULT)
	// StyleVertical is handled as a default.
#else
    style += StyleVertical + StyleHorizontal;
#endif
    VView::realize(parent);

#ifndef CV_NO_MOUSE_EVENTS
   // set up scroll bars
   if (vScroll != 0) {
      vScroll->uponChange(this, methodOf(VTreeListBox, vtScrollBar));
      vScroll->tracking(TRUE);
   }

   if (hScroll != 0) {
      hScroll->uponChange(this, methodOf(VTreeListBox, htScrollBar));
      hScroll->tracking(TRUE);
   }
#endif

   updateScrollBars();
}
// ---------------------------------------------------------------------------

#endif  // CV_NOARCHIVER
// ===========================================================================
