// $Header:   Q:/views/common/vcs/workbook.h_v   1.12   May 30 1997 17:17:26   scottt  $ 

//	workbook.h
//
//	VWorkBook interface [Common]
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
//	06/23/95 dgm	Original check-in; original code from kerstinb.
//	08/21/95 dgm	Changed #ifdef ARCHIVER to #ifndef CV_NOARCHIVER.
//	08/22/95 dgm	Removed unnecessary mouseDbl() implementation.
//	09/20/95 dss	VStyle changes.
//	09/09/96 dgm	Implemented CTRL-PgDn and CTRL-PgUp.
//	09/17/96 dgm	Implemented insert-page capabilities to addPage().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef workbook_h
#define workbook_h
#include "control.h"
#include "ordcllct.h"

extern VClass *VWorkBookCls;

#ifndef CV_NOARCHIVER
#	include "rscarch.h"
#endif

#ifndef CV_INLINE
#	define CV_INLINE
#endif

CLASS VWorkPage;

CLASS VWorkBook : public VControl {
public:
	enum PagePos {       NextPage  = 0, 
	                     PrevPage  = 1, 
	                     FirstPage = 2, 
	                     LastPage  = 3 };
	
	enum buttonState {	 up          =   1,
	                     down        =   2 };
	

    VWorkBook();
    VWorkBook(const VFrame& f, VWindow *parentwin, 
		const VStyle& style = StyleDefault);
    VWorkBook(const VString& name, const VFrame& f, 
		VWindow *parentwin, const VStyle& style = StyleDefault);
    ~VWorkBook();

	VClass *iam();
	
#if !defined(CV_NOARCHIVER)
	long addPage(const VString& caption, 
	            VRscArchiver& rf, const VString& rname,
	            PagePos pgpos = LastPage,
	            long pgno = -1);
#endif
	
	long addPage(const VString& caption, VWindow *win,
	            PagePos pgpos = LastPage, long pgno = -1);
	
	void removePage(long pgno);

	void removeAllPages();
	long findPage(const VString& caption);
	
	boolean gotoPage(long pgno);
	void uponPageActive(VObject *clnt, method mthd);
	
	void enablePage(long pgno, boolean tf = TRUE);
	
	void setFont(VFont *f);
	
	long getPageNo() const;
	VWindow* getPageWin() const;
	int  getPageCount() const;
	
	void getPageCaption(long pgno, VString& result) const;
	
	void setTabDims(int width = 0,
	               int height = 0);
	
	boolean resized(int w, int h);
	boolean mouseUp(int x, int y);
	boolean mouseDn(int mx, int my);
	boolean erased();
	boolean paint();
	boolean	key(int, char);
	void	refresh(boolean b); // Tab update
	void drawTabs();
	void drawButtons(buttonState left = up, buttonState right = up);
	
#ifndef CV_NOARCHIVER
	void putTo(VArchiver& a);
	void putTo(VArchiver& a, boolean most_derived);
	
	void getFrom(VArchiver& a);
	void getFrom(VArchiver& a, VObject *pwin,
		boolean do_realize = TRUE);
	virtual void    realize(VWindow *pwin);
	
	integer getActiveMethodIndex() const;
	void setActiveMethodIndex(integer);
	
	boolean loadResources(VRscArchiver& a);
#endif

protected:
	VOrdCollect	thePages;               // collection of VWorkPage objects
	VFrame		pageFrame;              // location of page windows
	
	VWindow*	currPageWin;
	long		currPageNo;
	
	int			userTabWidth;   		// user requested tab width, -1 is auto
	   			
	int			tabHeight;				// actual values used internally
	int			tabRows;

	int			currPageOffset;
	VPort*		m_pPort;
	
	method		activeMthd;             // called before new page is activated
	
	
	boolean recalcSizes(boolean force = FALSE);
#if !defined(CV_NOARCHIVER)
	VWindow *loadPageWin(VRscArchiver& rf, const VString& rname);
#endif
	
	VWorkPage*	getPage(long pgno) const;
	void		updateTabs();
				
	void		getInfo(VWinInfo& wbinfo);

	boolean		refreshFlag;

private:
#ifndef CV_NOARCHIVER
	integer     activeMthdIndex;
#endif

	void    init();
};

// ---------------------------------------------------------------------------
inline int VWorkBook::getPageCount() const
{
	return (int) thePages.count();
}

// ---------------------------------------------------------------------------
inline VWorkPage *VWorkBook::getPage(long pgno) const
{
	return (VWorkPage *) thePages.idAt(pgno);
}

// ---------------------------------------------------------------------------
inline long VWorkBook::getPageNo() const
{
	return currPageNo;
}

// ---------------------------------------------------------------------------
inline VWindow* VWorkBook::getPageWin() const
{
   return currPageWin;
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
inline integer VWorkBook::getActiveMethodIndex() const
{
	return activeMthdIndex;
}

// ---------------------------------------------------------------------------
inline void VWorkBook::setActiveMethodIndex(integer index)
{
	activeMthdIndex = index;
}

#endif

#endif // workbook_h
