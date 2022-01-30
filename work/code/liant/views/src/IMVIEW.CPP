//	imview.cpp
//
//	VImageView implementation
//
//  Allegris Foundation 1.1.00
//	Copyright (c) 1996 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	04/01/96 dgm	Original.
// --------------------------------------------------------------------------

#include "imview.h"
#include "popupmen.h"
#include "menuitem.h"
#include "menudata.h"
#include "panner.h"
#include "docktool.h"
#include "image.h"
#include "imbutton.h"
#include "bitmap.h"
#include "mouscrsr.h"
#include "location.h"
#include "bitbltr.h"
#include "fileslct.h"
#include "report.h"
#include "input.h"
#include "listslct.h"
#include "pathstr.h"
#include "str.h"
#include "brush.h"
#include "color.h"
#include "clrscm.h"
#include "port.h"
#include "dialog.h"
#include "group.h"
#include "pushbttn.h"
#include "textbox.h"
#include "editline.h"
#include "georow.h"
#include "geocol.h"
#include "notifier.h"
#include "rscarch.h"
#include "bfdir.h"
#include "bfdiritm.h"
#include "ordcllct.h"
#include "clipbord.h"
#include "main.rc"

// --------------------------------------------------------------------------
//
defineClass(VImageView, VScrollWindow)
defineArchiveRevision(VImageView, 0)

// --------------------------------------------------------------------------
//
#if defined(CV_WINDOWS)
#	define	CV_SUPPORTS_STRETCHBLT
#else
#	undef	CV_SUPPORTS_STRETCHBLT
#endif

// --------------------------------------------------------------------------
//
static long			objectCount			= 0;
static VPopupMenu  *configurationMenu	= 0;
static VPort	   *thePort				= 0;
static VRectangle  *theRectangle		= 0;
static VString	   *theString			= 0;

// --------------------------------------------------------------------------
//
VImageView::VImageView()
	: VScrollWindow()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VImageView::VImageView(const VFrame& frame, VWindow *parentwin,
					   const VStyle& style)
	: VScrollWindow()
{
	initialize();
	(*this)(frame, parentwin, style);
	construct();
}

// --------------------------------------------------------------------------
//
VImageView::VImageView(VWindow *parentwin, const VStyle& style)
	: VScrollWindow()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
	construct();
}

// --------------------------------------------------------------------------
//
void VImageView::operator()(const VFrame& frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VImageView::operator()(const VFrame& frame, VWindow *parentwin,
						    const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VImageView::operator()(const VFrame& frame, VWindow *parentwin,
						    const VStyle& style, const VString& name)
{
	VScrollWindow::operator()(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VImageView::~VImageView()
{
	if (theBitmap != 0) {
		delete theBitmap; theBitmap = 0;
	}
	if (--objectCount == 0) {
		if (configurationMenu != 0) {
			delete configurationMenu;
			configurationMenu = 0;
		}
		if (thePort != 0) {
			delete thePort;
			thePort = 0;
		}
		if (theRectangle != 0) {
			delete theRectangle;
			theRectangle = 0;
		}
		if (theString != 0) {
			delete theString;
			theString = 0;
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageView::initialize()
{
	if (objectCount++ == 0) {
		thePort = new VPort;
		theRectangle = new VRectangle;
		theString = new VString;
	}

	theBitmap						= 0;
	bitmapName						= 0;
	bitmapWidth						= 0;
	bitmapHeight					= 0;
	amBitmapOwner					= 0;
	cellWidth						= 5;
	cellHeight						= 5;
	cellView						= 0;
	toolBarSaveAs					= 0;
	toolBarClose					= 0;
	toolBarProperty					= 0;
	toolBarTogglePanner				= 0;
	toolBarPanner					= 0;
	toolBarZoom						= 0;
	toolBarZoomIn					= 0;
	toolBarZoomOut					= 0;
	clientObject					= 0;
	imageOpenedMethod				= 0;
	imageClosedMethod				= 0;
	pannerShownMethod				= 0;
	toolBarShownMethod				= 0;
	clickMethod						= 0;
	toolBarIsShowing				= 0;
	useStretchBltForCellView		= 1;
	useConfigurationMenu			= 1;
	pannerWasShowing				= 0;
	lastOpenFilterIndex				= 1;
	lastSaveFilterIndex				= 1;
}

// --------------------------------------------------------------------------
//
void VImageView::construct()
{
	initializeDropSite();
	clipToChildren();
}

// --------------------------------------------------------------------------
//
boolean VImageView::openImage()
{
	VPathString path;
	static VString filter("MS/Windows bitmap files (*.bmp)|*.bmp|"
						  "Allegris resource files (*.vrf)|*.vrf|"
						  "All files (*.*)|*.*");
	int index = lastOpenFilterIndex;
	if (VFileSelect::dialog(VFileSelect::FileOpen,
							"Open Image", path, this, &filter, &index)) {
		lastOpenFilterIndex = (unsigned char)index;
		if (index == 2) {
			VString name;
			int status = openImageFromVrf(path, name);
			if (status == 0) {
				if (name != "") {
					VString s;
					s.prints("Cannot retrieve bitmap resource "
							 "\"%s\" from \"%s\"",
							 name.gets(), path.gets());
					VReport::dialog("Image Open Error", this, s);
				}
			}
			else if (status < 0) {
				VString s;
				s.prints("No bitmap resources in \"%s\"", path.gets());
				VReport::dialog("Image Open Error", this, s);
			}
			else {
				return TRUE;
			}
		}
		else if (!openImage(path)) {
			VString s;
			s.prints("Cannot open bitmap file: %s", path.gets());
			VReport::dialog("Image Open Error", this, s);
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VImageView::openImage(const VString& filename)
{
	notifier->beginWait();
	VBitMap *bitmap = new VBitMap;
	if (!bitmap->loadFile(filename)) {
		delete bitmap;
		notifier->endWait();
		return FALSE;
	}
	boolean status = openImage(bitmap);
	if (bitmapName == 0) {
		bitmapName = new VString(filename);
	}
	else {
		*bitmapName = filename;
	}
	notifier->endWait();
	imageOpened((VObject *)&filename);
	return status;
}

// --------------------------------------------------------------------------
//
boolean VImageView::openImageFromVrf(const VPathString& vrf_file_name,
									 VString& vrf_bitmap_name)
{
	notifier->beginWait();
	vrf_bitmap_name = "";
	VRscArchiver ra(vrf_file_name);
	if (!ra.open(VRscArchiver::ReadOnly)) {
		notifier->endWait();
		return 0;
	}
	VBlockFileDir& rd = ra.getDirectory();
	VOrdCollect *ri = rd.getItems();
	if (ri == 0) {
		notifier->endWait();
		return -1;
	}
	VOrdCollect names;
	DO (*ri, VBlockFileDirItem, b)
		if (b->getType() == "VBitMap") {
			names.add((VObject *)&b->getName());
		}
	END
	if (names.count() <= 0) {
		notifier->endWait();
		return -1;
	}
	names.sort(TRUE);
	VString s, file;
	((VPathString&)vrf_file_name).nameOf(file);
	s.prints("Bitmap Resources - %s", file.gets());
	notifier->endWait();
	int index = VListSelect::dialog(TRUE, (char *)s.gets(), &names, this);
	if (index < 0) {
		return 0;
	}
	VString *name = (VString *)names.idAt(index);
	if (name == 0) {
		return 0;
	}
	notifier->beginWait();
	vrf_bitmap_name = *name;
	VBitMap *bitmap = (VBitMap *)
					  ra.getObject(*name, 0, VBitMapCls, 0, TRUE);
	ra.close();
	boolean status = openImage(bitmap);
	if (bitmapName == 0) {
		bitmapName = new VString(*name);
	}
	else {
		*bitmapName = *name;
	}
	notifier->endWait();
	return status ? 1 : 0;
}

// --------------------------------------------------------------------------
//
boolean VImageView::openImage(VBitMap *bitmap)
{
	if (bitmap == 0) {
		return FALSE;
	}
	int w, h;
	bitmap->sizeOfImage(&w, &h);
	if ((w <= 0) || (h <= 0)) {
		return FALSE;
	}
	if (isImageOpen()) {
		closeImage();
	}
	if ((theBitmap != 0) && amBitmapOwner) {
		delete theBitmap;
	}
	theBitmap = bitmap;
	bitmapWidth = w;
	bitmapHeight = h;
	updateToolBar();
	updateScrollIncrements();
	delete bitmapName; bitmapName = 0;
	if (pannerWasShowing) { showPanner(); }
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VImageView::closeImage()
{
	pannerWasShowing = isPannerShowing();
	hidePanner();
	if ((theBitmap != 0) && amBitmapOwner) {
		delete theBitmap;
	}
	theBitmap = 0;
	updateToolBar();
	updateView();
	imageClosed();
}

// --------------------------------------------------------------------------
//
boolean VImageView::saveImage()
{
	if (theBitmap == 0) {
		return FALSE;
	}
	VPathString path;
	static VString filter("MS/Windows bitmap file (*.bmp)|*.bmp|"
						  "Allegris resource files (*.vrf)|*.vrf|"
						  "Any file (*.*)|*.*");
	int index = lastSaveFilterIndex;
	if (VFileSelect::dialog(VFileSelect::SaveAs,
							"Save Image", path, this, &filter, &index)) {
		lastSaveFilterIndex = (unsigned char)index;
		if (index == 2) {
			VString *i;
			if ((i = VInput::
					 dialog(0, "Save Bitmap", this,
					 		"Enter VRF bitmap resource name:")) != 0) {
				VString suffix;
				if (!path.suffixOf(suffix)) {
					path.prints("%s.vrf", path.gets());
				}
				VRscArchiver ra(path);
				if (ra.open(VRscArchiver::Update) ||
					ra.open(VRscArchiver::Create)) {
					VString s;
					if (!ra.putObject(*i, theBitmap)) {
						s.prints("Cannot save bitmap resource "
								 " %s in VRF %s", *i, path.gets());
						VReport::dialog("Bitmap Save Error", this, s);
						ra.close();
					}
					else {
						ra.close();
						return TRUE;
					}
				}
				else {
					VString s;
					s.prints("Cannot open VRF file %s", path.gets());
					VReport::dialog("Bitmap Save Error", this, s);
				}
			}
		}
		else {
			VString suffix;
			if (!path.suffixOf(suffix)) {
				path.prints("%s.bmp", path.gets());
			}
			if (!theBitmap->saveFile(path.gets())) {
				VString s;
				s.prints("Cannot save bitmap file %s", path.gets());
				VReport::dialog("Bitmap Save Error", this, s);
			}
			else {
				return TRUE;
			}
		}
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
void VImageView::uponImageOpened(VObject *o, method m)
{
	clientObject = o;
	imageOpenedMethod = (void (VObject::*)(VImageView *, VObject *))m;
}

// --------------------------------------------------------------------------
//
void VImageView::uponImageClosed(VObject *o, method m)
{
	clientObject = o;
	imageClosedMethod = (void (VObject::*)(VImageView *))m;
}

// --------------------------------------------------------------------------
//
void VImageView::uponPannerShown(VObject *o, method m)
{
	clientObject = o;
	pannerShownMethod = (void (VObject::*)(VImageView *, boolean))m;
}

// --------------------------------------------------------------------------
//
void VImageView::uponToolBarShown(VObject *o, method m)
{
	clientObject = o;
	toolBarShownMethod = (void (VObject::*)(VImageView *, boolean))m;
}

// --------------------------------------------------------------------------
//
void VImageView::uponClick(VObject *o, method m)
{
	clientObject = o;
	clickMethod = (void (VObject::*)(VImageView *, int, int))m;
}

// --------------------------------------------------------------------------
//
void VImageView::imageOpened(VObject *object)
{
	updateGeometry();
	if ((clientObject != 0) && (imageOpenedMethod != 0)) {
		(clientObject->*imageOpenedMethod)(this, object);
	}
}

// --------------------------------------------------------------------------
//
void VImageView::imageClosed()
{
	if ((clientObject != 0) && (imageClosedMethod != 0)) {
		(clientObject->*imageClosedMethod)(this);
	}
}

// --------------------------------------------------------------------------
//
void VImageView::pannerShown(boolean b)
{
	updateToolBar();
	if ((clientObject != 0) && (pannerShownMethod != 0)) {
		(clientObject->*pannerShownMethod)(this, b);
	}
}

// --------------------------------------------------------------------------
//
void VImageView::toolBarShown(boolean b)
{
	if ((clientObject != 0) && (toolBarShownMethod != 0)) {
		(clientObject->*toolBarShownMethod)(this, b);
	}
}

// --------------------------------------------------------------------------
//
void VImageView::editCut()
{
	if (isImageOpen()) {
		notifier->beginWait();
		VClipBoard clipboard;
		clipboard.put(getBitmap());
		closeImage();
		notifier->endWait();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::editCopy()
{
	if (isImageOpen()) {
		notifier->beginWait();
		VClipBoard clipboard;
		clipboard.put(getBitmap());
		notifier->endWait();
	}
}

// --------------------------------------------------------------------------
//
boolean VImageView::editPaste()
{
	notifier->beginWait();
	boolean status = FALSE;
	VClipBoard clipboard;
	if (clipboard.contains(VStringCls)) {
		VString *string = (VString *)clipboard.get();
		if (string != 0) {
			if (openImage(*string)) {
				notifier->endWait();
				status = TRUE;
			}
			else {
				VString s;
				s.prints("Cannot open bitmap file: %s", string->gets());
				VReport::dialog("Image Open Error", this, s);
			}
		}
	}
	if (clipboard.contains(VBitMapCls)) {
		VBitMap *bitmap = (VBitMap *)clipboard.get();
		if (openImage(bitmap)) {
			status = TRUE;
		}
	}
	notifier->endWait();
	return status;
}

// --------------------------------------------------------------------------
//
boolean VImageView::isZoomed() const
{
	return cellView;
}

// --------------------------------------------------------------------------
//
void VImageView::zoom(boolean b)
{
	if (b) {
		if (!cellView) {
			setCellView(TRUE);
		}
	}
	else if (cellView) {
		setCellView(FALSE);
	}
}

// --------------------------------------------------------------------------
//
void VImageView::toggleZoom(VObject *)
{
	setCellView(!cellView);
}

// --------------------------------------------------------------------------
//
void VImageView::zoomIn(VObject *)
{
	if (!isImageOpen()) {
		return;
	}
	if (cellView) {
		cellWidth++; cellHeight++;
		updateScrollIncrements();
	}
	else {
		cellWidth = cellHeight = 2;
		setCellView(TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VImageView::zoomOut(VObject *)
{
	if (!isImageOpen() || !cellView) {
		return;
	}
	if ((cellWidth <= 2) && (cellHeight <= 2)) {
		setCellView(FALSE);
	}
	else {
		cellWidth--; cellHeight--;
		updateScrollIncrements();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::popupConfigurationMenu(int x, int y)
{
	if (useConfigurationMenu) {
		if (configurationMenu == 0) {
			configurationMenu = newConfigurationMenu();
		}
		mapToScreen(&x, &y);
		configurationMenu->popupAt(x, y);
	}
}

// --------------------------------------------------------------------------
//
void VImageView::togglePanner()
{
	if (isPannerShowing()) {
		hidePanner();
	}
	else {
		showPanner();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::createToolBar()
{
	if (toolBar == 0) {
		toolBar = createToolBar(this, StyleUndocked + StyleHidden);
		toolBar->setUndockedCloseBehavior
			(VDockableToolBar::HideOnClose);
		toolBar->uponUndockedClose
			(this, methodOf(VImageView, toolBarClosed));
		toolBar->setUndockedSquare(TRUE);
		toolBar->setUndockedCenter(TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VImageView::showProperties(VObject *)
{
	if (theBitmap != 0) {
		Properties properties(this);
		properties.modal();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::showPropertiesAt(int x, int y)
{
	if (theBitmap != 0) {
		Properties properties(this, x, y);
		properties.modal();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::toolBarClosed(VDockableToolBar *)
{
	toolBarIsShowing = 0;
}

// --------------------------------------------------------------------------
//
void VImageView::updateToolBar()
{
	if (toolBarSaveAs != 0) {
		if (isImageOpen()) {
			toolBarSaveAs->enable();
		}
		else {
			toolBarSaveAs->disable();
		}
	}
	if (toolBarClose != 0) {
		if (isImageOpen()) {
			toolBarClose->enable();
		}
		else {
			toolBarClose->disable();
		}
	}
	if (toolBarTogglePanner != 0) {
		toolBarTogglePanner->disableClickCallback();
		if (isPannerShowing()) {
			toolBarTogglePanner->toggleIn(TRUE, FALSE);
			toolBarTogglePanner->setControlHelpMsg("Hide Panner"); 
		}
		else {
			toolBarTogglePanner->toggleOut(TRUE, FALSE);
			toolBarTogglePanner->setControlHelpMsg("Show Panner"); 
		}
		toolBarTogglePanner->enableClickCallback();
	}
	if (toolBarProperty != 0) {
		if (isImageOpen()) {
			toolBarProperty->enable();
		}
		else {
			toolBarProperty->disable();
		}
	}
	if (toolBarZoom != 0) {
		toolBarZoom->disableClickCallback();
		toolBarZoom->toggleIn(cellView, FALSE);
		toolBarZoom->enableClickCallback();
	}
	if (toolBarZoomIn != 0) {
		if (isImageOpen()) {
			toolBarZoomIn->enable();
		}
		else {
			toolBarZoomIn->disable();
		}
	}
	if (toolBarZoomOut != 0) {
		if (isImageOpen() && cellView) {
			toolBarZoomOut->enable();
		}
		else {
			toolBarZoomOut->disable();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageView::getImageSize(long& w, long& h) const
{
	if (theBitmap == 0) {
		w = h = 0;
		return;
	}
	if (cellView) {
		w = bitmapWidth * cellWidth;
		h = bitmapHeight * cellHeight;
	}
	else {
		w = bitmapWidth;
		h = bitmapHeight;
	}
}

// ---------------------------------------------------------------------------
//
void VImageView::getPreferredSize(int *w, int *h) const
{
	if (theBitmap != 0) {
		VScrollWindow::getPreferredSize(w, h);
	}
	else {
		if (w != 0) { *w = 150; }
		if (h != 0) { *h = 150; }
	}
}

// --------------------------------------------------------------------------
//
void VImageView::getWorkAreaSize(long *w, long *h) const
{
	if (theBitmap != 0) {
		if (w != 0) { *w = bitmapWidth; }
		if (h != 0) { *h = bitmapHeight; }
	}
	else {
		if (w != 0) { *w = 0; }
		if (h != 0) { *h = 0; }
	}
}

// --------------------------------------------------------------------------
//
void VImageView::setCellView(boolean b)
{
	if (b) {
		if (!cellView) {
			cellView = 1;
			if (isImageOpen()) {
				updateScrollIncrements();
			}
			updateToolBar();
		}
	}
	else if (cellView) {
		cellView = 0;
		if (isImageOpen()) {
			updateScrollIncrements();
		}
		updateToolBar();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::updateScrollIncrements()
{
	if (cellView) {
		int cw = cellWidth;
		int ch = cellHeight;
		setUnits(cw, ch);
	}
	else {
		setUnits(1, 1);
	}
	updateView();
}

// --------------------------------------------------------------------------
//
VWindow *VImageView::newViewWindow() const
{
	VWindow *window = new ViewWindow((VImageView *)this);
	window->setBackground(&VColorScheme::
						   systemBrush(VColorScheme::ShadedShadow));
	return window;
}

// ==========================================================================
// ViewWindow
// ==========================================================================

// --------------------------------------------------------------------------
//
VImageView::ViewWindow::ViewWindow(VImageView *parentwin)
	: VScrollWindow::ViewWindow(parentwin)
{
	imageView = parentwin;
}

// --------------------------------------------------------------------------
//
boolean	VImageView::ViewWindow::erased()
{
	if (imageView->theBitmap == 0) {
		return VWindow::erased();
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VImageView::paintWorkArea(VWindow *window, long unit_x, long unit_y,
												int  unit_w, int  unit_h)
{
	if (theBitmap == 0) {
		return;
	}

	int x, y;

	getCellPosition(unit_x, unit_y, x, y);

	if (!cellView) {
		//
		// Here, we are not in cell-view;
		// simply blt the bitmap out there (fast).
		//
		VBitBltr bitblt(theBitmap, window);
		bitblt.bitbltr(unit_x, unit_y, unit_w, unit_h, x, y, CopyRule);
		return;
	}

	int cw, ch;
	getUnits(cw, ch);

#if defined(CV_SUPPORTS_STRETCHBLT)
	if (useStretchBltForCellView) {
		//
		// Here, we are in cell-view;
		// use the MS/Windows dependent StretchBlt API (fast).
		//
		VBitBltr bitblt(theBitmap, window);
		bitblt.bitbltr(unit_x, unit_y, unit_w, unit_h, x, y,
					   unit_w * cw, unit_h * ch, CopyRule);
		return;
	}
#endif

	// Here, we are in cell-view;
	// draw the appropriate pixels (pretty slow).

	if (!thePort->openOn(window)) {
		return;
	}
	VBrush brush(VColor::Red);
	thePort->useBrush(&brush);
	VColor *row = new VColor[unit_w];
	for (int vy = y, j = unit_y ; j < unit_y + unit_h ; j++) {
		theBitmap->readPixelRow(VLocation(unit_x, j), row, unit_w);
		for (int vx = x, i = 0 ; i < unit_w ; i++) {
			brush.background(row[i]);
			theRectangle->set(CornerDim, vx, vy, cw, ch);
			thePort->fillRegion(theRectangle, FALSE);
			vx += cw;
		}
		vy += ch;

	}
	thePort->close();
	delete [] row;
}

// --------------------------------------------------------------------------
// Map the given pixel rectangle within the cell-view to the corresponding
// pixel rectangle in the actual bitmap.
//
void VImageView::mapFromCellView(int& ix, int& iy, int& iw, int& ih,
								 int& bx, int& by, int& bw, int& bh) const
{
	int cw = cellWidth;
	int ch = cellHeight;
	int rx = ix % cw;
	int ry = iy % ch;
	if (rx != 0) { iw += rx; }
	if (ry != 0) { ih += ry; }
	bx = ix / cw;
	by = iy / ch;
	bw = iw / cw; if (iw % cw != 0) { bw++; }
	bh = ih / ch; if (ih % ch != 0) { bh++; }
	ix = bx * cw;
	iy = by * ch;
	iw = bw * cw;
	ih = bh * ch;
}

// --------------------------------------------------------------------------
// Map the given pixel rectangle within the cell-view to the corresponding
// pixel rectangle in the actual bitmap.
//
void VImageView::mapFromBitmapView(int bx, int by, int bw, int bh,
								   int& ix, int& iy, int& iw, int& ih) const
{
	int cw = cellWidth;
	int ch = cellHeight;
	ix = bx * cw;
	iy = by * ch;
	iw = bw * cw;
	ih = bh * ch;
}

// --------------------------------------------------------------------------
//
boolean VImageView::ViewWindow::mouseEnter(int b)
{
	if (b == 0) {
		notifier->mouseTracking(TRUE);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImageView::ViewWindow::mouseExit(int b)
{
	if (b == 0) {
//		setCursor(0);
		notifier->mouseTracking(FALSE);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
static int	mouseScroll				= 0;
static long	mouseScrollScrollValueX	= 0;
static long	mouseScrollScrollValueY	= 0;
static long	mouseScrollX			= 0;
static long	mouseScrollY			= 0;

// --------------------------------------------------------------------------
//
boolean VImageView::ViewWindow::mouseDn(int x, int y)
{
	if (imageView->isAutoScrolling()) {
		return VScrollWindow::ViewWindow::mouseDn(x, y);
	}
	if (imageView->isImageOpen()) {
		if ((imageView->clientObject != 0) && (imageView->clickMethod != 0)) {
			(imageView->clientObject->*imageView->clickMethod)
				(imageView, x, y);
		}
		if (imageView->mapToCell(x, y)) {
			mouseScrollX = x;
			mouseScrollY = y;
			imageView->getViewableWorkAreaUnitPosition
				(mouseScrollScrollValueX, mouseScrollScrollValueY);
			mouseScroll = 1;
			notifier->captureMouseFor(this);
			notifier->mouseTracking(TRUE);
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImageView::ViewWindow::mouseMv(int x, int y, int b)
{
	if (mouseScroll) {
		int dx = x - mouseScrollX;
		int dy = y - mouseScrollY;
		if (imageView->isHorizontalSmoothScrolling()) {
			mouseScrollX = x;
			if (imageView->isVerticalSmoothScrolling()) {
				mouseScrollY = y;
				imageView->scrollPixel(-dx, -dy);
			}
			else {
				imageView->scrollHorizontalPixel(-dx);
				dy /= imageView->getVerticalUnits();
				dy += imageView->getViewableWorkAreaUnitY() -
					  mouseScrollScrollValueY;
				imageView->scrollVerticalTo
					(dy / imageView->getVerticalUnits());
			}
		}
		else if (imageView->isVerticalSmoothScrolling()) {
			mouseScrollY = y;
			dx /= imageView->getHorizontalUnits();
			dx += imageView->getViewableWorkAreaUnitX() -
				  mouseScrollScrollValueX;
			imageView->scrollHorizontalTo
				(dx / imageView->getHorizontalUnits());
			imageView->scrollVerticalPixel(-dy);
		}
		else {
			dx /= imageView->getHorizontalUnits();
			dx += imageView->getViewableWorkAreaUnitX() -
				  mouseScrollScrollValueX;
			dy /= imageView->getVerticalUnits();
			dy += imageView->getViewableWorkAreaUnitY() -
				  mouseScrollScrollValueY;
			imageView->scroll(dx, dy);
		}
		return TRUE;
	}
	else {
		return VScrollWindow::ViewWindow::mouseMv(x, y, b);
	}
}

// --------------------------------------------------------------------------
//
boolean VImageView::ViewWindow::mouseUp(int x, int y)
{
	if (mouseScroll) {
		mouseMv(x, y, 1);
		mouseScroll = 0;
		notifier->mouseTracking(FALSE);
		notifier->releaseCapture();
		imageView->endScroll();
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean	VImageView::ViewWindow::rmouseDn(int x, int y)
{
	if (imageView->isAutoScrolling()) {
		return VScrollWindow::ViewWindow::rmouseDn(x, y);
	}
	mapToScreen(&x, &y);
	imageView->mapToWin(&x, &y);
	imageView->popupConfigurationMenu(x, y);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean	VImageView::ViewWindow::mouseDbl(int x, int y)
{
	imageView->showPropertiesAt(x, y);
	return TRUE;
}

// --------------------------------------------------------------------------
//
HPALETTE VImageView::ViewWindow::getPalette()
{
	return (imageView != 0) && (imageView->getBitmap() != 0) ?
			imageView->getBitmap()->getPalette() : 0;
}

// --------------------------------------------------------------------------
//
VDockableToolBar *VImageView::createToolBar(VWindow *parentwin,
											const VStyle& style)
{
	VDockableToolBar *toolBar = new VDockableToolBar(parentwin, style);
	toolBar->setShadeStyle(VDockableToolBar::NoShade); 
	toolBar->setMargins(4, 4);
	toolBar->setSpacing(6, 6);
	toolBar->setGroupSpacing(0, 0);
	toolBar->setUndockedWindowMargins(0); 
	toolBar->setUndockedCloseBehavior(VDockableWindow::DockOnClose); 
	toolBar->setUndockedTitle("ImageView ToolBar"); 
	toolBar->setUndockedCenter(TRUE);
	toolBar->setUndockedSquare(TRUE);

	// Create the toolbar image-buttons.

	toolBar->beginGroup();
	VImageButton *open = new VImageButton(toolBar); 
	toolBarClose = new VImageButton(toolBar); 
	toolBarSaveAs = new VImageButton(toolBar); 
	toolBar->endGroup();

	toolBar->beginGroup();
	toolBarTogglePanner = new VImageButton(toolBar, StyleAutoToggle); 
	toolBarPanner = new VPanner(toolBar, StyleBorder + Style3D); 
	toolBar->endGroup();

	toolBar->beginGroup();
	toolBarZoom = new VImageButton(toolBar, StyleAutoToggle); 
	toolBarZoomIn = new VImageButton(toolBar); 
	toolBarZoomOut = new VImageButton(toolBar); 
	toolBar->endGroup();

	toolBar->beginGroup();
	toolBarProperty = new VImageButton(toolBar); 
	toolBar->endGroup();

	// Set up the images for the image-buttons.
     
	VImage *image1 = new VImage(VString(CvRcStdImage1), 15);
	if (!image1->isValid()) {
		delete image1;
		return toolBar;
	}

	open->setImage(image1, 7); 
	toolBarClose->setImage(image1, 5); 
	toolBarSaveAs->setImage(image1, 8); 
	toolBarProperty->setImage(image1, 10); 
	toolBarTogglePanner->setImage(image1, 9); 
	toolBarZoom->setImage(image1, 12); 
	toolBarZoomIn->setImage(image1, 4); 
	toolBarZoomOut->setImage(image1, 3); 

#if 0
	open->useHoverMode();
	toolBarClose->useHoverMode();
	toolBarSaveAs->useHoverMode();
	toolBarProperty->useHoverMode();
	toolBarTogglePanner->useHoverMode();
	toolBarZoom->useHoverMode();
	toolBarZoomIn->useHoverMode();
	toolBarZoomOut->useHoverMode();
#endif

	toolBarSaveAs->disable();
	toolBarClose->disable();
	toolBarZoomIn->disable();
	toolBarZoomOut->disable();
	toolBarProperty->disable();

	// Adjust the initial size of the panner.

	int w, h;
	toolBarTogglePanner->determinePreferredSize(&w, &h);
	toolBarPanner->setFrame(VFrame(w, h));
	toolBarPanner->updateGeometry();

	// Set appropriate properties of the panner.

	associatePanner(toolBarPanner);

	// Set up the image-button callbacks.

	open->uponClick(this, methodOf(VImageView, openSelectedImage));
	toolBarClose->uponClick(this, methodOf(VImageView, closeImage));
	toolBarSaveAs->uponClick(this, methodOf(VImageView, saveImage));
	toolBarProperty->uponClick(this, methodOf(VImageView, showProperties));
	toolBarTogglePanner->uponClick(this, methodOf(VImageView, togglePanner));
	toolBarZoom->uponClick(this, methodOf(VImageView, toggleZoom));
	toolBarZoomIn->uponClick(this, methodOf(VImageView, zoomIn));
	toolBarZoomOut->uponClick(this, methodOf(VImageView, zoomOut));

	// Set up the image-button tool-tips.

	open->setControlHelpMsg("Open image"); 
	toolBarClose->setControlHelpMsg("Close image"); 
	toolBarSaveAs->setControlHelpMsg("Save image as"); 
	toolBarProperty->setControlHelpMsg("Properties"); 
	toolBarTogglePanner->setControlHelpMsg("Show Panner"); 
	toolBarPanner->setControlHelpMsg("Panner"); 
	toolBarZoom->setControlHelpMsg("Zoom View"); 
	toolBarZoomIn->setControlHelpMsg("Zoom in"); 
	toolBarZoomOut->setControlHelpMsg("Zoom out"); 

	return toolBar;
}

// ==========================================================================
// Drag & Drop Support
// ==========================================================================

#ifndef CV_NO_DRAG_DROP

#include "dragsite.h"
#include "dropsite.h"
#include "objcour.h"

// --------------------------------------------------------------------------
//
void VImageView::initializeDropSite()
{
	if (viewWindow == 0) {
		return;
	}
	dropSite = new VDropSite;
	dropSite->uponObjectDropped
		(this, methodOf(VImageView, dragObjectDropped));
	dropSite->uponDragObjectEnter
		(this, methodOf(VImageView, dragObjectEnter));
	dropSite->uponDragObjectOver
		(this, methodOf(VImageView, dragObjectOver));
	dropSite->setDragTracking(TRUE);
	dropSite->registerWindow(viewWindow);
}

// --------------------------------------------------------------------------
//
long VImageView::dragObjectDropped(VDropSite *dropsite, int x, int y)
{
	VObjectCourier *oc = dropsite->getObjectCourier();
	if (oc->contains(VStringCls)) {
		VString *string = (VString *)(oc->getObject(VStringCls));
		if (string != 0) {
			if (openImage(*string)) {
				return VDropSite::AnyOp;
			}
			else {
				VString s;
				s.prints("Cannot open bitmap file: %s", string->gets());
				VReport::dialog("Image Open Error", this, s);
			}
		}
	}
	if (oc->contains(VBitMapCls)) {
		VBitMap *bitmap = (VBitMap *)(oc->getObject(VBitMapCls));
		if (openImage(bitmap)) {
			return VDropSite::AnyOp;
		}
	}
	return VDropSite::NoOp;
}

// --------------------------------------------------------------------------
//
long VImageView::dragObjectEnter(VDropSite *dropsite)
{
	return dragObjectOver(dropsite, 0, 0);
}

// --------------------------------------------------------------------------
//
long VImageView::dragObjectOver(VDropSite *dropsite, int, int)
{
	VObjectCourier *oc = dropsite->getObjectCourier();
	if (oc->contains(VStringCls) || oc->contains(VBitMapCls)) {
		return VDropSite::AnyOp;
	}
	else {
		return VDropSite::NoOp;
	}
}

#endif

// ==========================================================================
// Configuration Menu
// ==========================================================================

VPopupMenu *VImageView::newConfigurationMenu() const
{
	return new ConfigurationMenu((VImageView *)this);
}

// --------------------------------------------------------------------------
//
VImageView::ConfigurationMenu::ConfigurationMenu(VImageView *iv)
	: VPopupMenu(VMenuData("", "",
						   methodOf(ConfigurationMenu, poppedUp), this))
{
	imageView = iv;

	addMenuItem(VMenuData
		("&Open", "",
		 methodOf(ConfigurationMenu, menuOpen), this, 1));
	addMenuItem(VMenuData
		("&Close", "",
		 methodOf(ConfigurationMenu, menuClose), this, 2));
	addMenuItem(VMenuData
		("Save &As", "",
		 methodOf(ConfigurationMenu, menuSaveAs), this, 3));

	addMenuItem(VMenuData());

	VPopupMenu *edit = new VPopupMenu
		(VMenuData("&Edit", "",
				   methodOf(ConfigurationMenu, poppedUpEdit), this));
	edit->addMenuItem(VMenuData
		("Cu&t", "",
		 methodOf(ConfigurationMenu, menuCut), this, 1));
	edit->addMenuItem(VMenuData
		("&Copy", "",
		 methodOf(ConfigurationMenu, menuCopy), this, 2));
	edit->addMenuItem(VMenuData
		("&Paste", "",
		 methodOf(ConfigurationMenu, menuPaste), this, 3));
	addMenuItem(VMenuData(edit));

	addMenuItem(VMenuData());

	addMenuItem(VMenuData
		("&Zoom", "",
		 methodOf(ConfigurationMenu, menuZoom), this, 4));
	addMenuItem(VMenuData
		("Zoom I&n", "",
		 methodOf(ConfigurationMenu, menuZoomIn), this, 5));
	addMenuItem(VMenuData
		("Zoom O&ut", "",
		 methodOf(ConfigurationMenu, menuZoomOut), this, 6));
	addMenuItem(VMenuData());

	VPopupMenu *options = iv->VScrollWindow::newConfigurationMenu();
	addMenuItem(VMenuData(options));

	addMenuItem(VMenuData());

	addMenuItem(VMenuData
		("P&roperties", "",
		 methodOf(ConfigurationMenu, menuProperties), this, 7));
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::poppedUp(VPopupMenu *pm)
{
	if ((pm != 0) && (imageView != 0)) {
		pm->enableMenuItem(2, imageView->isImageOpen());
		pm->enableMenuItem(3, imageView->isImageOpen());
		pm->enableMenuItem(5, imageView->isImageOpen());
		pm->enableMenuItem(6, imageView->isImageOpen() &&
							  imageView->isZoomed());
		pm->enableMenuItem(7, imageView->isImageOpen());
		pm->checkMenuItem(4, imageView->isZoomed());
		pm->enableMenuItem(8, imageView->isImageOpen());
		pm->enableMenuItem(9, imageView->isImageOpen());
		VClipBoard clipboard;
		pm->enableMenuItem(10, clipboard.contains(VBitMapCls) ||
							   clipboard.contains(VStringCls));
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::poppedUpEdit(VPopupMenu *pm)
{
	if ((pm != 0) && (imageView != 0)) {
		pm->enableMenuItem(1, imageView->isImageOpen());
		pm->enableMenuItem(2, imageView->isImageOpen());
		VClipBoard clipboard;
		pm->enableMenuItem(3, clipboard.contains(VBitMapCls) ||
							  clipboard.contains(VStringCls));
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuOpen(VObject *)
{
	if (imageView != 0) {
		imageView->openImage();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuClose(VObject *)
{
	if (imageView != 0) {
		imageView->closeImage();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuSaveAs(VObject *)
{
	if (imageView != 0) {
		imageView->saveImage();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuCut(VObject *)
{
	if (imageView != 0) {
		imageView->editCut();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuCopy(VObject *)
{
	if (imageView != 0) {
		imageView->editCopy();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuPaste(VObject *)
{
	if (imageView != 0) {
		imageView->editPaste();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuZoom(VObject *)
{
	if (imageView != 0) {
		imageView->toggleZoom();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuZoomIn(VObject *)
{
	if (imageView != 0) {
		imageView->zoomIn();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuZoomOut(VObject *)
{
	if (imageView != 0) {
		imageView->zoomOut();
	}
}

// --------------------------------------------------------------------------
//
void VImageView::ConfigurationMenu::menuProperties(VObject *)
{
	if (imageView != 0) {
		imageView->showProperties();
	}
}

// ==========================================================================
// Properties Dialog
// ==========================================================================

// --------------------------------------------------------------------------
//
VImageView::Properties::Properties(VImageView *parentwin, int x, int y)
	: VDialog(VFrame(), parentwin, "ImageView Properties",
			  StyleCaption + StyleSizable + StyleCloseBox + StyleHelpBox)
{
	initialize();
	imageView = parentwin;
	construct(x, y);
	updateData(x, y);
}

// --------------------------------------------------------------------------
//
VImageView::Properties::~Properties()
{
	delete backgroundBrush;
}

// --------------------------------------------------------------------------
//
void VImageView::Properties::initialize()
{
	imageView			= 0;
	nameControl			= 0;
	sizeControl			= 0;
	depthControl		= 0;
	viewControl			= 0;
	zoomControl			= 0;
	pixelControl		= 0;
	pixelColorControl	= 0;
	rgbControl			= 0;
	backgroundBrush		= 0;
}

// --------------------------------------------------------------------------
//
void VImageView::Properties::construct(int x, int y)
{
	VBrush *brush = &VColorScheme::
					 systemBrush(VColorScheme::ShadedBackground);

	setBackground(brush);

	// Layout the controls.

	VGeoRow *mainrow = setGeoRow();
	mainrow->setMargins(5);
	mainrow->setSpacingStyle(VGeoColumn::EqualFlushSpacing);
	mainrow->setPreferredSpacing(5);
	mainrow->setDefaultChildJustifyStyle(VGeoColumn::NearJustify);
	mainrow->setSpaceStretchability(0.0);
	mainrow->setSpaceShrinkability(0.0);

	VControl *control;

	// Group column.

	VGeoColumn *groupcolumn = new VGeoColumn(mainrow);
	groupcolumn->setSpacingStyle(VGeoColumn::NoSpacing);
	groupcolumn->setPreferredSpacing(5);
	groupcolumn->setDefaultChildJustifyStyle(VGeoColumn::FilledJustify);
	groupcolumn->setEqualMinorSize(TRUE);
	mainrow->setChildStretchability(groupcolumn->getIndex(), 1.0);

	// Group.

	VGroup *group1 = new VGroup(VFrame(), this, "Properties");
	groupcolumn->addChild(group1);

	VGeoColumn *groupcolumn1 = group1->setGeoColumn();
	groupcolumn1->setMargins(5);
	groupcolumn1->setSpacingStyle(VGeoColumn::NoSpacing);
	groupcolumn1->setPreferredSpacing(5);
	groupcolumn1->setDefaultChildJustifyStyle(VGeoColumn::FilledJustify);
	groupcolumn1->setDefaultChildMinorStretchability(1.0);

	VGeoManager::SizeGroup *sizegroup =
		groupcolumn1->newSizeGroup(VGeoManager::Width);

	// Name.

	VGeoRow *row0 = new VGeoRow(group1);
	row0->setSpacingStyle(VGeoColumn::NoSpacing);
	row0->setPreferredSpacing(4);
	row0->setDefaultChildJustifyStyle(VGeoColumn::CenterJustify);
	row0->setSpaceStretchability(0.0);

	control = new VTextBox(VFrame(), group1, StyleRight, "Name:");
	control->setEventVector(VWindow::ShowContextHelp, this,
							methodOf(VImageView::Properties, contextHelp));
	control->setEventVector(VWindow::ShowContextMenu, this,
							methodOf(VImageView::Properties, contextMenu));
	row0->addChild(control);
	sizegroup->add(control);

	nameControl =
		new VEditLine(VFrame(0, 0, 14*16, 0), group1,
					  StyleBorder + StyleReadOnly);
	row0->addChild(nameControl);
	row0->setChildStretchability(nameControl->getGeoIndex(), 1.0);

	// Size.

	VGeoRow *row1 = new VGeoRow(group1);
	row1->setSpacingStyle(VGeoColumn::NoSpacing);
	row1->setPreferredSpacing(4);
	row1->setDefaultChildJustifyStyle(VGeoColumn::CenterJustify);
	row1->setSpaceStretchability(0.0);

	control = new VTextBox(VFrame(), group1, StyleRight, "Size:");
	row1->addChild(control);
	sizegroup->add(control);

	sizeControl =
		new VEditLine(VFrame(0, 0, 14*16, 0), group1,
					  StyleBorder + StyleReadOnly);
	row1->addChild(sizeControl);
	row1->setChildStretchability(sizeControl->getGeoIndex(), 1.0);

	// Depth.

	VGeoRow *row2 = new VGeoRow(group1);
	row2->setSpacingStyle(VGeoColumn::NoSpacing);
	row2->setPreferredSpacing(4);
	row2->setDefaultChildJustifyStyle(VGeoColumn::CenterJustify);
	row2->setSpaceStretchability(0.0);

	control = new VTextBox(VFrame(), group1, StyleRight, "Depth:");
	row2->addChild(control);
	sizegroup->add(control);

	depthControl =
		new VEditLine(VFrame(0, 0, 14*16, 0), group1,
					  StyleBorder + StyleReadOnly);
	row2->addChild(depthControl);
	row2->setChildStretchability(depthControl->getGeoIndex(), 1.0);

	// View.

	VGeoRow *row3 = new VGeoRow(group1);
	row3->setSpacingStyle(VGeoColumn::NoSpacing);
	row3->setPreferredSpacing(4);
	row3->setDefaultChildJustifyStyle(VGeoColumn::CenterJustify);
	row3->setSpaceStretchability(0.0);

	control = new VTextBox(VFrame(), group1, StyleRight, "View:");
	row3->addChild(control);
	sizegroup->add(control);

	viewControl =
		new VEditLine(VFrame(0, 0, 14*16, 0), group1,
					  StyleBorder + StyleReadOnly);
	row3->addChild(viewControl);
	row3->setChildStretchability(viewControl->getGeoIndex(), 1.0);

	// Zoom.

	VGeoRow *row4 = new VGeoRow(group1);
	row4->setSpacingStyle(VGeoColumn::NoSpacing);
	row4->setPreferredSpacing(4);
	row4->setDefaultChildJustifyStyle(VGeoColumn::CenterJustify);
	row4->setSpaceStretchability(0.0);

	control = new VTextBox(VFrame(), group1, StyleRight, "Zoom:");
	row4->addChild(control);
	sizegroup->add(control);

	zoomControl =
		new VEditLine(VFrame(0, 0, 14*16, 0), group1,
					  StyleBorder + StyleReadOnly);
	row4->addChild(zoomControl);
	row4->setChildStretchability(zoomControl->getGeoIndex(), 1.0);

	// Pixel.

	if (imageView->mapToCell(x, y)) {
		VGeoRow *row5 = new VGeoRow(group1);
		row5->setSpacingStyle(VGeoColumn::NoSpacing);
		row5->setPreferredSpacing(4);
		row5->setDefaultChildJustifyStyle(VGeoColumn::CenterJustify);
		row5->setSpaceStretchability(0.0);

		control = new VTextBox(VFrame(), group1, StyleRight, "Pixel:");
		row5->addChild(control);
		sizegroup->add(control);

		pixelControl =
			new VEditLine(VFrame(), group1,
						  StyleBorder + StyleReadOnly);
		pixelControl->putText("0000,0000");
		row5->addChild(pixelControl);
		row5->setChildStretchability(pixelControl->getGeoIndex(), 1.0);

		control = new VTextBox(VFrame(), group1, StyleRight, "RGB:");
		row5->addChild(control);

		rgbControl =
			new VEditLine(VFrame(), group1,
						  StyleBorder + StyleReadOnly);
		rgbControl->putText("000,000,000");
		row5->addChild(rgbControl);
		row5->setChildStretchability(rgbControl->getGeoIndex(), 1.0);

		pixelColorControl =
			new VWindow(VFrame(), group1, StyleBorder + Style3D);
		pixelColorControl->setTabStop(FALSE);
		row5->addChild(pixelColorControl);

		int h;
		rgbControl->determinePreferredSize(0, &h);
		pixelColorControl->setFrame(VFrame(h, h));
		pixelColorControl->updateGeometry();
	}

	// Button column (OK, Cancel, Help).

	VGeoColumn *buttoncolumn = new VGeoColumn(mainrow);
	buttoncolumn->setSpacingStyle(VGeoColumn::NoSpacing);
	buttoncolumn->setPreferredSpacing(5);
	buttoncolumn->setEqualMinorSize(TRUE);

	VPushButton *ok =
		new VPushButton(VFrame(), this, StyleDefault, "OK");
	ok->uponClick(this, methodOf(VDialog, okPressed));
	buttoncolumn->addChild(ok);
	ok->setDefault(TRUE);
	setDefButton(ok);

	VPushButton *cancel =
		new VPushButton(VFrame(), this, StyleDefault, "Cancel");
	cancel->disable();
	cancel->uponClick(this, methodOf(VDialog, cancelPressed));
	buttoncolumn->addChild(cancel);
#if defined(CV_WINDOWS)
	setEscButton(cancel);
#endif

	VPushButton *help =
		new VPushButton(VFrame(), this, StyleDefault, "Help");
	help->disable();
	buttoncolumn->addChild(help);

	// Set the minimum size and frame.

	int w, h;
	determinePreferredSize(&w, &h);
	constrainMinimumSize(w, h);
	constrainMaximumSize(0, h);
	frameWindow();

	// Center the dialog position.

	centerWindow(getParent());
}

// --------------------------------------------------------------------------
//
void VImageView::Properties::contextHelp()
{
	VReport::dialog("Context Help", this, "Not yet implemented.");
}

// --------------------------------------------------------------------------
//
void VImageView::Properties::contextMenu()
{
	VReport::dialog("Context Menu", this, "Not yet implemented.");
}

// --------------------------------------------------------------------------
//
void VImageView::Properties::centerWindow(VWindow *window)
{
	int x, y, w, h, wx, wy, ww, wh, sw, sh;

	sizeOfWin(&w, &h);
	notifier->getScreenSize(&sw, &sh);

	if (window == 0) {
		wx = 0;
		wy = 0;
		ww = sw;
		wh = sh;
	}
	else {
		window->getAbs(&wx, &wy, &ww, &wh);
	}

	x = wx + (ww - w) / 2;
	y = wy + (wh - h) / 2;

	if (x < 0) { x = 0; } else if (x + w > sw) { x = sw - w; }
	if (y < 0) { y = 0; } else if (y + h > sh) { y = sh - h; }

	setGeometryPosition(x, y);
} 

// --------------------------------------------------------------------------
//
void VImageView::Properties::updateData(int x, int y)
{
	if (imageView == 0) {
		return;
	}
	VBitMap *bitmap = imageView->getBitmap();
	if (bitmap != 0) {
		if (nameControl != 0) {
			nameControl->putText(imageView->getBitmapName());
			nameControl->selectAll(0);
		}
		VString s;
		int width, height, depth;
		bitmap->sizeOfImage(&width, &height);
		if (sizeControl != 0) {
			s.prints("%dx%d", width, height);
			sizeControl->putText(s);
		}
		depth = bitmap->depthOfImage();
		if (depthControl != 0) {
			s.prints("%d", depth);
			depthControl->putText(s);
		}
		if (viewControl != 0) {
			long vx, vy; int vw, vh;
			imageView->getViewableWorkAreaUnitGeometry(vx, vy, vw, vh);
			s.prints("%d,%d %dx%d", vx, vy, vw, vh);
			viewControl->putText(s);
		}
		if (zoomControl != 0) {
			s.prints("%d%%", imageView->getHorizontalUnits() * 100);
			zoomControl->putText(s);
		}
		long ux, uy;
		if ((pixelControl != 0) && imageView->mapToCell(x, y, &ux, &uy)) {
			theString->prints("%d,%d", ux, uy);
			pixelControl->putText(*theString);
			VColor color = bitmap->readPixel(VLocation(ux, uy));
			theString->prints("%d,%d,%d", color.getRed(),
										  color.getGreen(),
										  color.getBlue());
			if (rgbControl != 0) {
				rgbControl->putText(*theString);
			}
			if (pixelColorControl != 0) {
				if (backgroundBrush == 0) {
					backgroundBrush = new VBrush(color);
					pixelColorControl->setBackground(backgroundBrush);
				}
				else {
					backgroundBrush->background(color);
					pixelColorControl->update();
				}
			}
		}
	}
}

// ==========================================================================
// Archiving
// ==========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VImageView::putTo(VArchiver& a)
{
	VImageView::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VImageView::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VWindow::putTo(a, FALSE);

	// Write the object version tag.

	a << VImageViewArchiveTag;

	// Write any object specific data.

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VImageView::getFrom(VArchiver& a)
{
	// Read the base object data.

	VWindow::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VImageViewArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VImageViewArchiveTag));
		return;
	}

	// Read any object specific data.
}

// ---------------------------------------------------------------------------
//
void VImageView::getFrom(VArchiver& a,
								VObject *parentwin, boolean do_realize)
{
	VImageView::getFrom(a);

	if (do_realize) {
		VImageView::realize((VWindow *)parentwin);
	}

	if (parentwin != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VImageView::realize(VWindow *parentwin)
{
	VWindow::realize(parentwin);

	construct();
}

#endif // !CV_NOARCHIVER
