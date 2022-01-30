// $Header:   Q:/views/common/vcs/UTILWIN.H_V   1.18   Mar 24 1997 16:14:04   scottt  $

//	utilwin.h
//
//	VUtilityWindow interface [Common]
//
//  Allegris Foundation 1.1.00
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
//	10/18/95 dgm	Original.
//	10/19/95 dgm	Updated.
//	11/01/95 dgm	Updated.
//	11/01/95 dgm	Changed activate()/deactivate() signature.
//	11/02/95 dgm	Changed getNonClientAreaSize() to getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	11/15/95 dgm	Updated.
//	12/05/95 dgm	Removed extraneous declarations.
//	12/07/95 dgm	Added setOpaqueResizing/Moving() support.
//	12/08/95 dgm	Added setDisableDecorations().
//	02/07/96 dgm	Added NoClose.
//	03/04/96 dgm	Publicized activateCloseBox() & other additions.
//	03/21/96 dgm	Archiver support.
//	04/19/96 dgm	Minor changes initialize the opaque-move/resize bits.
//	09/17/96 pkt	updated for WebShop.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef UTILWIN_H
#define UTILWIN_H

// --------------------------------------------------------------------------
//
#include "poppwndw.h"
#include "geomgr.h"
#include "popupmen.h"
#ifndef CV_NO_PAINT_EVENTS
#include "rband.h"
#endif

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VFont;
CLASS	VGeoManager;
CLASS	VMouseCursor;
CLASS	VPopupWindow;
CLASS	VRectangle;
CLASS	VString;
CLASS	VStyle;
CLASS	VWinInfo;

#ifndef CV_NOARCHIVER
CLASS   VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VUtilityWindow : public VPopupWindow {
public:
	enum CloseBehavior
	{					NothingOnClose		=  0x0000,
						HideOnClose			=  0x0001,
						DeleteOnClose		=  0x0002,
						NoClose				=  0x0004 };
	enum MoveArea
	{					TitleBar			=  1,
						ClientArea			=  2 };
#ifndef CV_NO_PAINT_EVENTS
	enum ResizeArea
	{					Left				= VRubberBand::ResizeLeft,
						Right				= VRubberBand::ResizeRight,
						Top					= VRubberBand::ResizeTop,
						Bottom				= VRubberBand::ResizeBottom,
						TopLeft				= VRubberBand::ResizeTopLeft,
						TopRight			= VRubberBand::ResizeTopRight,
						BottomLeft			= VRubberBand::ResizeBottomLeft,
						BottomRight			= VRubberBand::ResizeBottomRight };
#endif
	enum OtherArea
	{					CloseBox			= 11,
						NoWhere				=  0 };
	enum MovingStyle
	{					NoMoving			=  0x0001,
						TitleBarMoving		=  0x0002,
						ClientAreaMoving	=  0x0004 };
	enum ResizingStyle
	{					NoResizing			=  0,
						NormalResizing		=  1,
						PreferredResizing	=  2 };

public:
						VUtilityWindow();
						VUtilityWindow(const VFrame&, VWindow *,
									   const VStyle& = StyleDefault);
						VUtilityWindow(VWindow *,
									   const VStyle& = StyleDefault);
						VUtilityWindow(const VString&,
									   const VFrame&, VWindow *,
									   const VStyle& = StyleDefault);
						VUtilityWindow(const VString&,
									   VWindow *,
									   const VStyle& = StyleDefault);
					   ~VUtilityWindow();
	VClass			   *iam();

	void				operator()(const VFrame&, VWindow *);
	void				operator()(const VFrame&, VWindow *, const VStyle&);
	void				operator()(const VFrame&, VWindow *, const VStyle&,
								   const VString&);

	void				setTitle(const VString&);
	void				getTitle(VString&) const;
	VWindow			   *getClientAreaWindow() const;

	void				setCloseBehavior(int);
	CloseBehavior		getCloseBehavior() const;

	void				setMovingStyle(int);
	MovingStyle			getMovingStyle() const;

	void				setResizingStyle(int);
	ResizingStyle		getResizingStyle() const;

	void				confineToScreen(boolean);
	boolean				isConfinedToScreen() const;

	void				setOpaqueResizing(boolean);
	boolean				isOpaqueResizing() const;

	void				setOpaqueMoving(boolean);
	boolean				isOpaqueMoving() const;

	void				setDisableDecorations(boolean);
	boolean				isDisableDecorations() const;

	void				useConfigurationMenu(boolean);

	int					moveWithRubberBand(int&, int&,
										   int&, int&, int&, int&);
	int					resizeWithRubberBand(int&, int&, int&, int&,
											 int&, int&, int);

	void				activateCloseBox();

	void				uponBeginMoving(VObject *, method);
	void				uponMoving(VObject *, method);
	void				uponEndMoving(VObject *, method);

	void				uponBeginResizing(VObject *, method);
	void				uponResizing(VObject *, method);
	void				uponEndResizing(VObject *, method);

	void				uponMouseDbl(VObject *, method);
	void				uponRmouseDn(VObject *, method);

	void				uponClose(VObject *, method);

	static boolean		supportsStyleWindows95();
	static boolean		supportsStyleWindows31();
	static boolean		supportsStyleOs2();
	static boolean		supportsStyleMotif();

	static boolean		isStyleWindows95();
	static boolean		isStyleWindows31();
	static boolean		isStyleOs2();
	static boolean		isStyleMotif();

	static void			setStyleWindows95();
	static void			setStyleWindows31();
	static void			setStyleOs2();
	static void			setStyleMotif();

	char				getBorderStyle() const;

	static void			setNewStyle();

	// Public base class overrides.

    virtual void		resizeClientArea(int w, int h);
    virtual void		getSystemBorderSize(int *, int *) const;

    static void         stdSize(const VStyle&, int *w, int *h);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
	CLASS Manager : public VGeoManager {
	public:
							Manager() { }
		virtual void		getPreferredSize(int *, int *);
		virtual void		getMinimumSize(int *, int *);
		virtual boolean		getConstrainedSize(int *, int *, int, int);
		virtual void		frameChildren(int, int);
	};

	CLASS ClientWindow : public VWindow {
	public:
						ClientWindow(VWindow *);
					   ~ClientWindow();
	protected:
		virtual boolean	mouseDn(int, int);
		virtual boolean	mouseDbl(int, int);
		virtual boolean	rmouseDn(int, int);
	private:
		friend			VUtilityWindow;
	};

public:
	CLASS ConfigurationMenu : public VPopupMenu {
	public:
						ConfigurationMenu();
		void			popup(int, int, VUtilityWindow *);
	private:
		void			poppedUp(VPopupMenu *);
		void			poppedUpOpaque(VPopupMenu *);
		void			poppedUpStyle(VPopupMenu *);
		void			setMove(VMenuItem *);
		void			setSize(VMenuItem *);
		void			setClose(VMenuItem *);
		void			setOpaque(VMenuItem *);
		void			setStyle(VMenuItem *);
	private:
		VUtilityWindow   *utilityWindow;
	};

public:
    virtual void		getInternalBorderGeometry(int *, int *,
												  int *, int *) const;
#ifndef CV_NOARCHIVER
    virtual void        putTo(VArchiver&);
    virtual void        getFrom(VArchiver&);
    virtual void        getFrom(VArchiver&, VObject *, boolean = TRUE);
    virtual void        realize(VWindow *);
#endif

	virtual VStyle&		getDefaultStyle() const;
	static VStyle&		getClassDefaultStyle();
protected:
	virtual void		getMinimumSize(int *, int *) const;
	virtual boolean		close();
    virtual void		getInfo(VWinInfo&);
protected:
	virtual boolean		beginMoving();
	virtual boolean		moving(int& x, int& y,
							   int& w, int& h, int mx, int my);
	virtual boolean		endMoving(int x, int y, int w, int h,
								  int mx, int my, int& status);
	virtual boolean		beginResizing();
	virtual boolean		resizing(int x, int y, int& w, int& h,
								 int resize_area);
	virtual boolean		endResizing(int x, int y, int w, int h,
									int resize_area, int& status);

protected:
	virtual boolean		erased();
	virtual boolean		paint();
	virtual boolean		mouseEnter(int);
	virtual boolean		mouseExit(int);
	virtual boolean		mouseDn(int, int);
	virtual boolean		mouseUp(int, int);
	virtual boolean		mouseMv(int, int, int);
	virtual boolean		mouseDbl(int, int);
	virtual boolean		rmouseDn(int, int);
	virtual boolean		givenFocus();
	virtual boolean		clearFocus();
	virtual boolean		key(int, char);
	virtual boolean		activate(VView *w);
	virtual boolean		deactivate(VView *w);
	virtual void		systemFontChanged();
#ifndef CV_NOARCHIVER
    void                putTo(VArchiver&, boolean);
#endif

protected:
	void				initialize();
	void				construct();
	void				constructFonts();
	void				setupMetrics();
	void				deleteFonts();
	VWindow			   *newClientWindow(VWindow *);
	int					hitTest(int, int, boolean = FALSE);
	void				noteFocus(boolean);
	void				paintBorder(int = 0, int = 0);
	void				paintTitleBar(int = 0, int = 0);
	void				paintTitle(int = 0, int = 0);
	void				paintCloseBox(int = 0, int = 0);
	void				armCloseBox(boolean);
	boolean				resizeRubberBandCallback(int, int, int&, int&, int);
	boolean				moveRubberBandCallback(int&, int&, int&, int&,
											   int, int);
	void				constructRubberBand();
	void				deleteRubberBand();
	void				drawRubberBand(int, int, int, int);
	void				initializeOpaqueMoveResize();
	int					getTitleBarHeight() const;
protected:
	static void			constructStaticData();
	static void			deleteStaticData(boolean = TRUE);
	static void			constructStaticColors();
	static void			deleteStaticColors();
	static void			constructStaticFonts();
	static void			deleteStaticFonts();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	VWindow			   *clientWindow;
	void			   *unusedA;
	VString			   *theTitle;
	VObject			   *clientObject;
	boolean  (VObject::*beginResizingMethod)();
	boolean  (VObject::*resizingMethod)(int, int, int&, int&, int);
	boolean  (VObject::*endResizingMethod)(int, int, int, int, int, int&);
	boolean  (VObject::*beginMovingMethod)();
	boolean  (VObject::*movingMethod)(int&, int&, int&, int&, int, int);
	boolean  (VObject::*endMovingMethod)(int, int, int, int, int, int, int&);
	boolean  (VObject::*mouseDblMethod)(int, int);
	boolean  (VObject::*rmouseDnMethod)(int, int);
	void	 (VObject::*closeMethod)(VWindow *);
#ifndef CV_NOARCHIVER
	integer				clientObjectIndex;
	integer				beginResizingMethodIndex;
	integer				resizingMethodIndex;
	integer				endResizingMethodIndex;
	integer				beginMovingMethodIndex;
	integer				movingMethodIndex;
	integer				endMovingMethodIndex;
	integer				mouseDblMethodIndex;
	integer				rmouseDnMethodIndex;
	integer				closeMethodIndex;
#endif
	short				borderThicknessX;
	short				borderThicknessY;
	short				titleBarHeight;				////
	short				closeBoxWidth;				////
	short				closeBoxHeight;				////
	short				closeBoxSpacingX;			////
	short				closeBoxSpacingY;			////
	short				titleSpacing;				////
	short				resizeCornerLength;			////
	short				minimumWidth;
	short				minimumHeight;
	short				maximumWidth;
	short				maximumHeight;
	VMouseCursor	   *currentCursor;
	int					drawTitleBar : 1;
	int					drawCloseBox : 1;
	int					drawBorder : 1;
	int					clientAreaMoving : 1;
	int					titleBarMoving : 1;
	int					noMoving : 1;
	int					noResizing : 1;
	int					preferredResizing : 1;
	int					hasFocus : 1;
	int					mouseOverhead : 1;
	int					closeBoxArmed : 1;
	int					closeBoxPressed : 1;
	int					deleteOnClose : 1;
	int					hideOnClose : 1;
	int					confinedToScreen : 1;
	int					smallTitleBar : 1;
	int					opaqueResizing : 1;
	int					opaqueMoving : 1;
	int					decorationsDisabled : 1;
	int					configurationMenuDisabled : 1;
	int					opaqueResizingExplicitlySet : 1;
	int					opaqueMovingExplicitlySet : 1;

private:
	friend				Manager;
	friend				ClientWindow;
	friend				VNotifier;
};

extern VClass *VUtilityWindowCls;

// --------------------------------------------------------------------------
//
inline VWindow *VUtilityWindow::getClientAreaWindow() const
{
	if (clientWindow == 0) {
		return (VWindow *)this;
	}
	else {
		return (VWindow *)clientWindow;
	}
}

inline VUtilityWindow::ResizingStyle VUtilityWindow::getResizingStyle() const
{
	if (noResizing) {
		return NoResizing;
	}
	else if (preferredResizing) {
		return PreferredResizing;
	}
	else {
		return NormalResizing;
	}
}

inline VUtilityWindow::MovingStyle VUtilityWindow::getMovingStyle() const
{
	if (noMoving) {
		return NoMoving;
	}
	int ms = 0;
	if (clientAreaMoving) {
		ms |= ClientAreaMoving;
	}
	if (titleBarMoving) {
		ms |= TitleBarMoving;
	}
	return MovingStyle(ms);
}

inline VUtilityWindow::CloseBehavior VUtilityWindow::getCloseBehavior() const
{
	if (!drawCloseBox) {
		return NoClose;
	}
	else if (deleteOnClose) {
		return DeleteOnClose;
	}
	else if (hideOnClose) {
		return HideOnClose;
	}
	else {
		return NothingOnClose;
	}
}

inline void VUtilityWindow::confineToScreen(boolean b)
{
	confinedToScreen = char(b);
}

inline boolean VUtilityWindow::isConfinedToScreen() const
{
	return confinedToScreen;
}

inline boolean VUtilityWindow::isOpaqueResizing() const
{
	return opaqueResizing;
}

inline boolean VUtilityWindow::isOpaqueMoving() const
{
	return opaqueMoving;
}

inline boolean VUtilityWindow::isDisableDecorations() const
{
	return decorationsDisabled;
}

inline void VUtilityWindow::useConfigurationMenu(boolean b)
{
	configurationMenuDisabled = !b;
}

inline int VUtilityWindow::getTitleBarHeight() const
{
	return decorationsDisabled ? 0 : titleBarHeight + 1;
}

#endif // UTILWIN_H
