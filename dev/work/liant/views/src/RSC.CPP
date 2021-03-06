// $Header:   Q:/views/common/vcs/RSC.CPV   1.28   Mar 24 1997 16:13:12   scottt  $ 

//  rsc.cpp
//
//  VResource implementation [Common]
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
//  12/04/95 dgm    Added this header.
//  12/04/95 dgm    Added a new loadContents() -- from KerstinB.
//  01/05/96 tlf    Added needed include files for motif.
//  06/12/96 pkt	updated to support port of library to internet server.
//	06/12/96 pkt	added #include "popupmen.h"
//	07/03/96 pkt	preened.
//	08/21/96 pkt	removed 'loadHelpTable'.
//  09/04/96 tlf    Motif fixes in loadContents for operator() calls.
//	11/07/96 pkt	added #include "brush.h".
//	11/18/96 tlf	Made windows-specific code in loadPopupMenu conditional
//			on CV_WINDOWS.
//
//	02/04/97 pkt	eliminated the need to hack style when realizing a win.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "notifier.h"
#include "iterator.h"
#include "rscarch.h"
#include "menu.h"
#include "bitmap.h"
#include "bmapbttn.h"
#include "location.h"
#ifndef CV_NO_MDI
#include "stdappvw.h"
#endif
#include "view.h"
#include "dialog.h"
#ifndef CV_NO_MDI
#include "stdmdiap.h"
#include "stdmdivw.h"
#include "toolbar.h"
#include "statusbr.h"
#endif
#include "brush.h"
#include "rsc.h"
#include "popupmen.h"
#include "pathstr.h"
#if defined(CV_HTML)
#include "htmlstrm.h"
#endif

// ---------------------------------------------------------------------------
//
VResource::VResource()
#if defined(CV_HTML)
	:
	  isHtml(FALSE)
#endif
{
}

// ---------------------------------------------------------------------------
//
VResource::~VResource()
{
}

// ---------------------------------------------------------------------------
//
boolean VResource::open()
{
        // get path of executable
#ifdef WIN31
        char szBuff[261];
        GetModuleFileName(notifier->inst(), szBuff, 260);

        VPathString tmp(szBuff);
        tmp.suffix("vrf");
        m_vrfName = tmp;
        return m_vrf.open(VRscArchiver::ReadOnly, m_vrfName);
#else
        return FALSE;
#endif
}

// ---------------------------------------------------------------------------
//
boolean VResource::open(const VString& vrfName)
{
#if defined(CV_HTML)
	isHtml = FALSE;
	VPathString path = vrfName;
	VString suffix;
	if (path.suffixOf(suffix)) {
		suffix.toLower();
		isHtml = (suffix == "htm" || suffix == "html");
	}
#endif

        m_vrfName = vrfName;

#if defined(CV_HTML)
	if (isHtml) {
		return htmlFile.open(ReadOnly, m_vrfName);
	}
#endif
        return m_vrf.open(VRscArchiver::ReadOnly, m_vrfName);
}


// ---------------------------------------------------------------------------
//
void VResource::close()
{
        m_vrf.close();
}

// ---------------------------------------------------------------------------
//
boolean VResource::loadWindow(const VString& wname,
                        VWindow* window,
                        VWindow* parent,
                        VClass*  cls)
{
        if (!m_vrf.isOpen())
        return FALSE;

        if (!m_vrf.contains(wname, cls))
            return FALSE;

        // adjust parent to use client area
#ifndef CV_REMOTE_GUI
        if (parent != 0) {
            if (parent->isA(VStdAppViewCls)) {
                parent = (VWindow*) ((VStdAppView*) parent)->getClientWindow();
            }
            else if (parent->isA(VMdiAppViewCls)) {
                parent = (VWindow*) ((VMdiAppView*) parent)->clientOf();
            }
        }
#endif

        // default to VDialog
        if (cls == 0) {
            cls = VDialogCls;
        }

        VView* view = 0;
        view = (VView*) m_vrf.getObject(wname, window, cls, parent, TRUE);

        if (view && view->hwnd()) {
//      view->clipToChildren(FALSE);
//      loadHelpTable(wname, view);
                view->loadResources(m_vrf);
                return TRUE;
        }

        return FALSE;
}

// ---------------------------------------------------------------------------
// Loads 'window' with the contents specified in resource 'wname' of the
// current resource file. Lookup of 'wname' will be as a VDialog unless
// a different type of window is specified by 'cls'. Use 'ww' and 'wh' to
// retrieve the width and height of the window resource. If 'fSetTitle' is
// TRUE, sets 'window's title based on the window resource.
//
// Returns FALSE if the resource could not be found and retrieved.
//
boolean VResource::loadContents(const VString& wname,
                        VWindow* window,
                        VClass* cls,
                        int *ww,
                        int *wh,
                        boolean fSetTitle)
{
    if (ww != 0) {
                *ww = 0;
	}
    if (wh != 0) {
                *wh = 0;
	}

#if !defined(CV_HTML)
    if (!m_vrf.isOpen()) {
        return FALSE;
	}
#else
    if (!(isHtml ? htmlFile.isOpen() : m_vrf.isOpen())) {
            return FALSE;
	}
#endif

#if !defined(CV_HTML)
    if (!m_vrf.contains(wname, cls)) {
        return FALSE;
	}
#endif

        // get client area of window
        VWindow* clientArea = window;
        if (clientArea != 0) {
#ifndef CV_NO_MDI
            if (window->isA(VStdAppViewCls)) {
                clientArea = (VWindow*) ((VStdAppView*) window)->getClientWindow();
            }
            else if (window->isA(VStdMdiAppViewCls)) {
                clientArea = (VWindow*) window;
			}
            else if (window->isA(VMdiAppViewCls)) {
                clientArea = (VWindow*) ((VMdiAppView*) window)->clientOf();
            }
#endif
        }

        // default to VDialog
        if (cls == 0) {
            cls = VDialogCls;
        }

        VView* view = 0;
#if !defined(CV_HTML)
        view = (VView*) m_vrf.getObject(wname, 0, cls, window, FALSE);
#else
	if (isHtml) {
		view = VHtmlStream::getView(htmlFile, 0, window, FALSE);
	}
	else {
    	view = (VView*) m_vrf.getObject(wname, 0, cls, window, FALSE);
	}
#endif
    if (view != 0) {
                VString         title;

                view->realizeChildren(clientArea);

                // load any additional resource (such as bitmaps or custom controls)
                ((VView*) clientArea)->loadResources(m_vrf);

                // adjust attachments
                clientArea->frameChildren();

#ifndef CV_NO_MDI
				if (window->isA(VStdMdiAppViewCls)) {
		            clientArea = (VWindow*) ((VStdMdiAppView*) window)->clientOf();

					// find toolbar and statusbar and set them
					DO (window->childrenOf(), VWindow, child)
						if (child->isA(VToolBarCls)) {
							((VStdMdiAppView*) window)->setToolBar((VToolBar*) child);
						}
						else if (child->isA(VStatusBarCls)) {
							((VStdMdiAppView*) window)->setStatusBar((VStatusBar*) child);
						}
					END
		        }
#endif

                if (!view->getDefaultForeground()) {
	                clientArea->setForeground(view->getForeground());
				}
				VBrush* tmpBrush = view->getBackground();
				if (tmpBrush) {
                	clientArea->setBackground(new VBrush(tmpBrush->pattern(), tmpBrush->foreground(), tmpBrush->background()));
				}
                // Set title, using title assigned in the resource editor
                view->getTitle(title);
                if (fSetTitle && title.size() && window->isA(VViewCls) &&
                    window->getIsA() != VViewCls) {
                        ((VView*) window)->setTitle(title);
                }

        if (ww != 0 || wh != 0) {
                        VFrame  frame;
                        int             x, y, w, h;

                        view->getFrame(frame);
                        frame.get(window, VLocation(), &x, &y, &w, &h);

            if (ww != 0) {
                                *ww = w;
			}
            if (wh != 0) {
                                *wh = h;
			}
                }

                delete view;

                return TRUE;
        }
        return FALSE;
}

// ---------------------------------------------------------------------------
//
boolean VResource::loadContents(const VString& wname,
                        VWindow* window,
                        VClass* cls,
                        VWindow* parent)
{
    if (!m_vrf.isOpen())
        return FALSE;

    if (!m_vrf.contains(wname, cls))
        return FALSE;

    // adjust parent to use client area
    VWindow* parentClientArea = parent;
    if (parent != 0) {
#ifndef CV_NO_MDI
        if (parentClientArea->isA(VStdAppViewCls)) {
            parentClientArea = (VWindow*) ((VStdAppView*) parent)->getClientWindow();
        }
        else if (parent->isA(VMdiAppViewCls)) {
            parentClientArea = (VWindow*) ((VMdiAppView*) parent)->clientOf();
        }
#endif
    }

    // default to VDialog
    if (cls == 0) {
        cls = VDialogCls;
    }

    // read in contents into temporary view
    VView* tmpView = 0;
#ifdef CVS_LATER_DEFAULT
	// No hack necessary, because the defaults are applied to 'window'
	// (which has the appropriate class) when operator() is called.
#else
    if (cls == VDialogCls) {
        // hack to fix style problems
        tmpView = new VDialog;
        VWinStyle& style = tmpView->getStyle();
        style = VView::getClassDefaultStyle();
    }
#endif
    tmpView = (VView*) m_vrf.getObject(wname, tmpView, cls, window, FALSE);

    if (tmpView != 0) {
        // get frame of temporary view
        VFrame frame;
        tmpView->getFrame(frame);

        // finish realizing object with frame and style from archive
        if (window->hwnd() == 0) {
            VWinStyle& style = window->getStyle();
            style = tmpView->getStyle() + StyleHidden;

            // because operator() is not virtual, try to find the closest
            // known Views class to invoke
#ifndef CV_NO_MDI
            if (window->isA(VStdAppViewCls)) {
#ifdef CV_MOTIF
                (*((VStdAppView*)window))(frame, style, wname);
#else
                window->setName(wname);
                (*((VStdAppView*)window))(frame, style);
#endif
            }
            else if (window->isA(VStdMdiAppViewCls)) {
                window->setName(wname);
                (*((VStdMdiAppView*)window))(frame, style);
            }
            else if (window->isA(VStdMdiViewCls)) {
                window->setName(wname);
#ifdef PM
              (*((VStdMdiView*)window))((const char*)0, frame, (VStdMdiAppView*) parent, style, 0);
#else
              (*((VStdMdiView*)window))(wname, frame, (VStdMdiAppView*) parent, style, 0);
#endif
            }
            else if (window->isA(VMdiAppViewCls)) {
#ifdef CV_MOTIF
                (*((VMdiAppView*)window))(frame, style, wname);
#else
                window->setName(wname);
                (*((VMdiAppView*)window))(frame, style);
#endif
            }
            else if (window->isA(VMdiViewCls)) {
                window->setName(wname);
#ifdef PM
                (*((VMdiView*)window))((const char*)0, frame, parent, style, 0);
#else
                (*((VMdiView*)window))(wname, frame, parent, style, 0);
#endif
            }
            else
#endif 
                 if (window->isA(VAppViewCls)) {
#ifdef CV_MOTIF
                (*((VAppView*)window))(frame, style, wname);
#else
                window->setName(wname);
                (*((VAppView*)window))(frame, style);
#endif

            }
            else if (window->isA(VDialogCls)) {
                (*((VDialog*)window))(frame, parentClientArea, style, wname);
                ((VDialog*) window)->setSysMenu();
            }
            else if (window->isA(VPopupWindowCls)) {
                (*((VPopupWindow*)window))(frame, parentClientArea, style, wname);
            }
            else if (window->isA(VViewCls)) {
                (*((VView*)window))(frame, parentClientArea, style, wname);
            }
            else {
                (*window)(frame, parentClientArea, style, wname);
            }
        }

        // get client area of window
        VWindow* clientArea = window;
        if (clientArea != 0) {
#ifndef CV_NO_MDI
            if (window->isA(VStdAppViewCls)) {
                clientArea = (VWindow*) ((VStdAppView*) window)->getClientWindow();
            }
            else if (window->isA(VStdMdiAppViewCls)) {
                clientArea = (VWindow*) window;
            }
            else if (window->isA(VMdiAppViewCls)) {
                clientArea = (VWindow*) ((VMdiAppView*) window)->clientOf();
            }
#endif
        }

        // realize children in real view
        tmpView->realizeChildren(clientArea);

        // load any additional resource (such as bitmaps or custom controls)
        clientArea->loadResources(m_vrf);

        // adjust attachments of real view
        clientArea->frameChildren();

#ifndef CV_NO_MDI
        if (window->isA(VStdMdiAppViewCls)) {
            clientArea = (VWindow*) ((VStdMdiAppView*) window)->clientOf();

			// find toolbar and statusbar and set them
			DO (window->childrenOf(), VWindow, child)
				if (child->isA(VToolBarCls)) {
					((VStdMdiAppView*) window)->setToolBar((VToolBar*) child);
				}
				else if (child->isA(VStatusBarCls)) {
					((VStdMdiAppView*) window)->setStatusBar((VStatusBar*) child);
				}
			END
        }
#endif

        // adjust colors
        if (!tmpView->getDefaultForeground()) {
	        clientArea->setForeground(tmpView->getForeground());
		}
		VBrush* tmpBrush = tmpView->getBackground();
		if (tmpBrush) {
        	clientArea->setBackground(new VBrush(tmpBrush->pattern(), tmpBrush->foreground(), tmpBrush->background()));
		}

        // Set title, using title from archive
        VString title;
        tmpView->getTitle(title);
        if (title.size() > 0 && window->isA(VViewCls)) {
            ((VView*)window)->setTitle(title);
        }

        // clean up temporary view
        delete tmpView;

        return TRUE;
    }
    return FALSE;
}

// ---------------------------------------------------------------------------
//
VMenu* VResource::loadMenu(const VString& wname,
        VWindow* parent)
{
        if (!m_vrf.isOpen())
                return FALSE;

        if (!m_vrf.contains(wname, VMenuCls))
            return FALSE;

        boolean fRealize = TRUE;

        VMenu* pMenu = 0;

        pMenu = (VMenu*) m_vrf.getObject(wname, 0, VMenuCls, parent, fRealize);

        return pMenu;
}

// ---------------------------------------------------------------------------
//
VBitMap* VResource::loadBitMap(const VString& wname)
{
        if (!m_vrf.isOpen())
                return FALSE;

        if (!m_vrf.contains(wname, VBitMapCls))
            return FALSE;

        boolean fRealize = TRUE;

        VBitMap* pBitMap = 0;

        pBitMap = (VBitMap*) m_vrf.getObject(wname, 0, VBitMapCls);

        return pBitMap;
}

// ---------------------------------------------------------------------------
//
boolean VResource::realizeWindow(const VString& wname,
                      VWindow* window,
                      VClass* cls,
                      VWindow* parent)
{
    // if already realized, do nothing
    if (window->hwnd() != 0) {
        return TRUE;
    }

    if (!m_vrf.isOpen())
        return FALSE;

    if (!m_vrf.contains(wname, cls))  {
        return FALSE;
    }

    // adjust parent to use client area
    VWindow* parentClientArea = parent;
    if (parent != 0) {
#ifndef CV_NO_MDI
        if (parentClientArea->isA(VStdAppViewCls)) {
            parentClientArea = (VWindow*) ((VStdAppView*) parent)->getClientWindow();
        }
        else if (parent->isA(VMdiAppViewCls)) {
            parentClientArea = (VWindow*) ((VMdiAppView*) parent)->clientOf();
        }
#endif
    }

    // default to VDialog
    if (cls == 0) {
        cls = VDialogCls;
    }

    // read in contents into temporary view
    VView* tmpView = 0;
#ifdef CVS_LATER_DEFAULT
	// No hack necessary, because the defaults are applied to 'window'
	// (which has the appropriate class) when operator() is called.
#else
    if (cls == VDialogCls) {
        // hack to fix style problems
        tmpView = new VDialog;
        VWinStyle& style = tmpView->getStyle();
        style = VView::getClassDefaultStyle();
    }
#endif
    tmpView = (VView*) m_vrf.getObject(wname, tmpView, cls, window, FALSE);

    if (tmpView != 0) {
        // get frame of temporary view
        VFrame frame;
        tmpView->getFrame(frame);

        // finish realizing object with frame and style from archive
        VWinStyle& style = window->getStyle();
        style = tmpView->getStyle() + StyleHidden;

        // because operator() is not virtual, try to find the closest
        // know Views class to invoke
#ifndef CV_NO_MDI
        if (window->isA(VStdAppViewCls)) {
            window->setName(wname);
            (*((VStdAppView*)window))(frame, style);
        }
        else if (window->isA(VStdMdiAppViewCls)) {
            window->setName(wname);
            (*((VStdMdiAppView*)window))(frame, style);
        }
        else if (window->isA(VStdMdiViewCls)) {
            window->setName(wname);
#ifdef PM
          (*((VStdMdiView*)window))((const char*)0, frame, (VStdMdiAppView*) parent, style, 0);
#else
          (*((VStdMdiView*)window))(wname, frame, (VStdMdiAppView*) parent, style, 0);
#endif
        }
        else if (window->isA(VMdiAppViewCls)) {
            window->setName(wname);
            (*((VMdiAppView*)window))(frame, style);
        }
        else if (window->isA(VMdiViewCls)) {
            window->setName(wname);
#ifdef PM
            (*((VMdiView*)window))((const char*)0, frame, parent, style, 0);
#else
            (*((VMdiView*)window))(wname, frame, parent, style, 0);
#endif
        }
        else
#endif
		     if (window->isA(VAppViewCls)) {
            window->setName(wname);
            (*((VAppView*)window))(frame, style);
        }
        else if (window->isA(VDialogCls)) {
            (*((VDialog*)window))(frame, parentClientArea, style, wname);
            ((VDialog*) window)->setSysMenu();
        }
        else if (window->isA(VPopupWindowCls)) {
            (*((VPopupWindow*)window))(frame, parentClientArea, style, wname);
        }
        else if (window->isA(VViewCls)) {
            (*((VView*)window))(frame, parentClientArea, style, wname);
        }
        else {
            (*window)(frame, parentClientArea, style, wname);
        }

        delete tmpView;

        return TRUE;
    }

    return FALSE;
}

// ---------------------------------------------------------------------------
//
VPopupMenu* VResource::loadPopupMenu(const VString& wname,
                                     VWindow* parent)
{
    if (!m_vrf.isOpen())
        return FALSE;

    if (!m_vrf.contains(wname, VPopupMenuCls))
        return FALSE;

    boolean fRealize = TRUE;

    VPopupMenu* tempMenu = new VPopupMenu("Popup Menu");
    VPopupMenu* pMenu = tempMenu;

    pMenu = (VPopupMenu*) m_vrf.getObject(wname, pMenu, VPopupMenuCls, 
                                          0, fRealize);

#ifdef CV_WINDOWS
	if (pMenu) {
	   	// Special case for menus which haven't been realized yet,
	   	// i.e. which were read from a Portable Views Resource File (VRF).
	   	//
	    DO (pMenu->mitems(), VObject, menu_item)
	        pMenu->realizeMenuItem(menu_item);
	    END
	    if (parent != 0) {
	        pMenu->initializeMenuClients(parent);
	    }
	}
	else {
		delete tempMenu;
	}
#endif

    return pMenu;
}
