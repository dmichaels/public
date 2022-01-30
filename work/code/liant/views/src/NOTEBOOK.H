// $Header:   Q:/views/common/vcs/notebook.h_v   1.29   May 30 1997 17:15:58   scottt  $ 

//  notebook.h
//
//  VNoteBook interface [Common]
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
//  Revision History:
//  -----------------
//  06/23/95 dgm    Original check-in; original code from jmd.
//  08/15/95 krb    Added variable size tabs.
//  08/21/95 dgm    Changed #ifdef  ARCHIVER to #ifndef CV_NOARCHIVER.
//  08/22/95 dgm    Removed unnecessary mouseDbl() implementation.
//  08/29/95 dgm    For krb; added setPageCaption().
//  09/15/95 dss    VStyle changes.
//  09/09/96 dgm    Fixed LEFT/RIGHT arrows to wrap around.
//					Fixed TAB to go to first child of current notepage.
//					Added en/disableTabFocus()/isEn/DisabledTabFocus().
//  09/18/96 dgm    More focus fiddling.
//  11/18/96 tlf    Made two windows-only declarations conditional on 
//                  CV_WINDOWS.
//  01/14/97 djs    Removed #ifdef WINDOWS for focus updates on OS2
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VNOTEBOOK_H
#define VNOTEBOOK_H
#include "control.h"
#include "ordcllct.h"

extern VClass *VNoteBookCls;

#ifndef CV_NOARCHIVER
#       include "rscarch.h"
#endif

#ifndef CV_INLINE
#       define CV_INLINE
#endif

CLASS VNotePage;
CLASS VbNoteBookInspector;

CLASS VNoteBook : public VControl {
public:
      // note: styles may not be supported on all platforms
      enum PageType                   {       MajorPage = 0, 
                           MinorPage = 1};

      enum PagePos                    {       NextPage  = 0, 
                           PrevPage  = 1, 
                           FirstPage = 2, 
                           LastPage  = 3 };

      enum TabType                    {       SingleRowTabs  = 0,
                           MultiRowTabs = 1,
                           ButtonTabs = 2 }; 

      enum BindingType                {       SolidBinding = 0,
                           SpiralBinding = 1 };
      
      enum TabPlacement               {       TabRight = 0,
                           TabLeft = 1,
                           TabTop = 2,
                           TabBottom  = 3 };


         VNoteBook();
         VNoteBook(const VFrame& f, VWindow *parentwin, 
                  const VStyle& style = StyleDefault);

         VNoteBook(const VString& name, const VFrame& f, 
                  VWindow *parentwin, const VStyle& style = StyleDefault);
         ~VNoteBook();
   VClass *iam();

#if !defined(CV_NOARCHIVER)
   long             addPage(const VString& caption, 
               VRscArchiver& rf, const VString& rname,
               PageType pgtype = MajorPage, PagePos pgpos = LastPage,
               long pgno = -1);
#endif

   long             addPage(const VString& caption, VWindow *win,
               PageType pgtype = MajorPage, PagePos pgpos = LastPage,
               long pgno = -1);

   void    removePage(long pgno);
   void    removeAllPages();
   long    findPage(const VString& caption);

   boolean gotoPage(long pgno);
   void    uponPageActive(VObject *clnt, method mthd);

   void    enablePage(long pgno, boolean tf = TRUE);

   void    setFont(VFont *f);

   long             getPageNo() const;
   VWindow*        getPageWin() const;
   int             getPageCount() const;
   int             getTabWidth() const;

   void    setPageCaption(long pgno, const VString& result);
   void    getPageCaption(long pgno, VString& result) const;

   void    setTabDims(int width = 0,
                  int height = 0, 
                  TabType type = SingleRowTabs);

   boolean resized(int w, int h);
   boolean frameChildren(int w, int h, boolean = FALSE);
   boolean mouseDn(int mx, int my);
   boolean erased();
   boolean paint();
   void    refresh(boolean b); // Tab update

   void    setTabStyles(   BindingType binding = SolidBinding, 
                     TabPlacement tabplace = TabTop);


	void	enableTabFocus(boolean = TRUE);
	void	disableTabFocus(boolean = TRUE);
	boolean	isDisabledTabFocus() const;
	boolean	isEnabledTabFocus() const;

   friend  VbNoteBookInspector;

	boolean				key(int, char);
	boolean				takeFocus();
	boolean				givenFocus();
	boolean				clearFocus();
	boolean				nextChildFocus(VWindow *);
	boolean				prevChildFocus(VWindow *);
	boolean				controlNextFocus();
	boolean				controlPrevFocus();
	VWindow			   *getNextChildFocusCandidate(VWindow * = 0) const;
	VWindow			   *getPrevChildFocusCandidate(VWindow * = 0) const;
	VWindow			   *getFocusCandidate(int = 0) const;

#ifndef CV_NOARCHIVER
   void                    putTo(VArchiver& a);
   void                    putTo(VArchiver& a, boolean most_derived);

   void                    getFrom(VArchiver& a);
   void                    getFrom(VArchiver& a, VObject *pwin,
                     boolean do_realize = TRUE);
   virtual void    realize(VWindow *pwin);

   integer                 getActiveMethodIndex() const;
   void                    setActiveMethodIndex(integer);

   boolean                 loadResources(VRscArchiver& a);
#endif

protected:
   VOrdCollect     thePages;               // collection of VNotePage objects
   VFrame          pageFrame;              // location of page windows

   int			   nativeMode : 1;        // use native control (if available)
   int			   tabFocusEnabled : 1;
   int			   tabHasFocus : 1;
   int			   handlingMouseDn : 1;
   int			   handlingMouseDnInTab : 1;

   VWindow    *currPageWin;
   long                     currPageNo;

   int                     userTabWidth;   // user requested tab width, -1 is auto

   int                     tabWidth;               // actual values used internally
   int                     tabHeight;
   int                     tabRows;

   TabType                 tabType;
   BindingType             tabBinding;
   TabPlacement    tabPlacement;

   method          activeMthd;             // called before new page is activated

   boolean recalcSizes(boolean force = FALSE);

#if !defined(CV_NOARCHIVER)
   VWindow *loadPageWin(VRscArchiver& rf, const VString& rname);
#endif

   VNotePage       *getPage(long pgno) const;
   void            updateTabs();

   void            getInfo(VWinInfo& nbinfo);

   boolean         refreshFlag;

private:
#ifndef CV_NOARCHIVER
   integer         activeMthdIndex;
#endif

   void    init();

};

// ---------------------------------------------------------------------------
inline int VNoteBook::getPageCount() const
{
   return (int) thePages.count();
}

// ---------------------------------------------------------------------------
inline VNotePage *VNoteBook::getPage(long pgno) const
{
   return (VNotePage *) thePages.idAt(pgno);
}

// ---------------------------------------------------------------------------
inline long VNoteBook::getPageNo() const
{
   return currPageNo;
}

// ---------------------------------------------------------------------------
inline VWindow* VNoteBook::getPageWin() const
{
   return currPageWin;
}

// ---------------------------------------------------------------------------
inline int VNoteBook::getTabWidth() const
{
   return (userTabWidth <= 0) ? tabWidth : userTabWidth;
}

inline void VNoteBook::disableTabFocus(boolean b)
{
	enableTabFocus(!b);
}

inline boolean VNoteBook::isEnabledTabFocus() const
{
	return tabFocusEnabled;
}

inline boolean VNoteBook::isDisabledTabFocus() const
{
	return !tabFocusEnabled;
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
inline integer VNoteBook::getActiveMethodIndex() const
{
   return activeMthdIndex;
}

// ---------------------------------------------------------------------------
inline void VNoteBook::setActiveMethodIndex(integer index)
{
   activeMthdIndex = index;
}

#endif

#endif // VNOTEBOOK_H
