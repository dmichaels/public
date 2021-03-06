// $Header:   Q:/views/common/vcs/savedlg.cpv   1.9   May 30 1997 17:16:20   scottt  $ 

//	savedlg.cpp
//
//	VSaveDialog function implementation [Common]
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
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#include "window.h"
#include "dialog.h"
#include "control.h"
#include "frame.h"
#include "rscarch.h"
#include "archiver.h"
#include "str.h"
#include "ordcllct.h"
#include "iterator.h"
#include "notifier.h"
#include "brush.h"

#if !defined(CV_NOGEOFRAME)
#include "geomgr.h"
#endif

// --------------------------------------------------------------------------
// saveDialog
//
// Save to the given resource-archiver, the given VDialog dialog (which is
// typically an object of a class derived from VDialog) as a VDialog object.
//
boolean saveDialog(VDialog *dialog,
				   const VString& resource_name,
				   VRscArchiver *resource_archiver)
{
	VWindow		*dialog_parent = 0;
	VFrame		 dialog_frame;
	VString		*name;
	VDialog		*tmp_dialog;
	char		 title_buffer[256], *title;
	VOrdCollect	*dialog_children;
	boolean		 archiver_was_open;
	boolean		 return_status;

	// Sanity checks.

	if ((dialog == 0) || (resource_archiver == 0)) {
		return FALSE;
	}

	// Make a temporary archivable dialog
	// with the appropriate size and attributes.

	dialog_parent = dialog->getParent();

	if (dialog->getStyle().contains(StyleCaption)) {
		VString s;
		dialog->getTitle(s);
		title = title_buffer;
		s.gets(title, 255);
	}
	else {
		title = 0;
	}

	if (!dialog->isA(VDialogCls)) {
		//
		// Adjust the geometry...
		//
		dialog->getFrame(dialog_frame);
	}
	else {
		dialog->getFrame(dialog_frame);
	}

	if ((name = dialog->getName()) != 0) {
		tmp_dialog =
			new VDialog(*name, dialog_frame,
						dialog_parent, title, dialog->getStyle());
	}
	else {
		tmp_dialog =
			new VDialog(dialog_frame, dialog_parent, title, dialog->getStyle());
	}

	if (tmp_dialog == 0) {
		return FALSE;
	}

	// Temporarily, re-parent the actual child controls of the
	// given dialog to be children of the archivable dialog.

#if !defined(CV_NOGEOFRAME)
    VGeoManager *dialog_manager = dialog->getGeoManager(); 
    VGeoManager *tmp_dialog_manager = tmp_dialog->setGeoManager(); 
    if ((dialog_manager != 0) && (tmp_dialog_manager != 0)) {
        dialog_manager->moveChildren(tmp_dialog_manager, TRUE);
    }
#endif

	dialog_children = dialog->childrenOf();
	if (dialog_children != 0) {
		DO (dialog_children, VWindow, child)
			if (child->isA(VPopupWindowCls)) {
				continue;
			}
			VControl *child_control = (VControl *)child;
			child_control->setParent(0);
			tmp_dialog->addChild(child_control);
		END
	}

	// copy colors
    if (!dialog->getDefaultForeground()) {
		tmp_dialog->setForeground(dialog->getForeground());
	}
	VBrush* tmpBrush = dialog->getBackground();
	if (tmpBrush) {
		tmp_dialog->setBackground(new VBrush(tmpBrush->pattern(), tmpBrush->foreground(), tmpBrush->background()));
	}
    
	// Open the resource-archiver.

	archiver_was_open = resource_archiver->isOpen();

	if (!archiver_was_open) {
		VArchiver::AccessType mode;
		if (resource_archiver->file().exists()) {
			mode = VRscArchiver::Update;
		}
		else {
			mode = VRscArchiver::Create;
		}
		if (!resource_archiver->open(mode)) {
			return FALSE;
		}
	}

	return_status = TRUE;

	// Dump the dialog object into the resource-archiver.

    notifier->setPainterMode(TRUE);

	if (!resource_archiver->putObject(resource_name, tmp_dialog)) {
		return_status = FALSE;
	}

    notifier->setPainterMode(FALSE);

	// Close the resource-archiver if necessary.

	if (!archiver_was_open) {
		resource_archiver->close();
	}

	// Undo the re-parenting of the given dialog child controls.

	dialog_children = tmp_dialog->childrenOf();
	if (dialog_children != 0) {
		DO (dialog_children, VControl, child)
			child->setParent(0);
			child->setParent(dialog_parent);
		END
	}

#if !defined(CV_NOGEOFRAME)
    if ((dialog_manager != 0) && (tmp_dialog_manager != 0)) {
        tmp_dialog_manager->moveChildren(dialog_manager, TRUE);
    }
#endif

	// Delete the temporary archivable dialog;
	// careful not take the children with it.

	dialog_children->reset();
	delete tmp_dialog;

	// Done.

	return return_status;
}
