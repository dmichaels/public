// $Header:   Q:/views/common/vcs/styles.h_v   1.22   16 May 1997 10:58:10   gregm  $

//	styles.h [Common]
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
//	09/01/95 dss	created.
//	09/20/95 dss	fix borland compile errors. Added StyleSingleSelect,
//					StyleMultiSelect, StyleFlatList.
//  10/11/95 tomf   added changes for motif.
//	10/12/95 jld	added styles for notebook and treelistbox
//	11/01/95 dgm	Added VDockableWindow styles.
//	11/02/95 jld	added StyleDragEnabled
//	11/10/95 dgm	added StyleUndocked
//	11/15/95 dss	added winclass.h to define windows defines.
//	12/06/95 dss	made styles objects const.
//	12/15/95 dss	Added CV_IMPORT.
//  02/17/96 evc    Fixed macro CV_IMPORT for static 16 bit build 
//					that had been forcing the data into a separate 
//					data segment
//  02/17/96 evc    More simplification via macros
//	12/17/96 dgm	Added StyleHelpBox/StyleNoHelpBox.
// ----------------------------------------------------------------------------

#ifndef	STYLES_H
#define STYLES_H

#include "defs.h"
#include "winclass.h"
#include "implstyl.h"
#include "stylexpr.h"

// some compilers are picky about import syntax
#if defined (CV_WIN16) && !defined (DLL)
	// This is the static 16 bit case : all static data is near
	// and the declaration corresponds to the NEAR definition
	#define CV_IMPORT( storage, type, name) storage type CV_NEARDATA  name

#elif defined(CV_WIN16) && defined (DLL)
	// The 16  bit DLL  case - these are __exported so they must be far.
	// The declaration matchs the definition
	#define CV_IMPORT( storage, type, name) storage type _far name

#elif defined(__BCPLUSPLUS__) && defined (CV_WIN32) && defined (DLL)
	#define CV_IMPORT( storage, type, name) storage type CV_DLL_IMPORT name
#elif defined(_MSC_VER) && defined(CV_WIN32)
	#define CV_IMPORT( storage, type, name) storage CV_DLL_IMPORT type name
#else
	#define CV_IMPORT( storage, type, name) storage CV_DLL_IMPORT type name
#endif


CV_IMPORT(extern , const VStyle,  StyleDefault);
CV_IMPORT(extern, const  VExclusiveStyle, StyleCloseBox);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoCloseBox);

CV_IMPORT(extern, const  VExclusiveStyle, Style3D);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNo3D);

CV_IMPORT(extern, const  VExclusiveStyle, StyleSizable);
CV_IMPORT(extern, const  VExclusiveStyle, StyleUnsizable);

CV_IMPORT(extern, const  VExclusiveStyle, StyleHorizontal);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoHorizontal);

CV_IMPORT(extern, const  VExclusiveStyle, StyleVertical);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoVertical);

CV_IMPORT(extern, const  VExclusiveStyle, StyleMaximize);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoMaximize);

CV_IMPORT(extern, const  VExclusiveStyle, StyleMinimize);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoMinimize);

CV_IMPORT(extern, const  VExclusiveStyle, StyleMaximized);
CV_IMPORT(extern, const  VExclusiveStyle, StyleMinimized);
CV_IMPORT(extern, const  VExclusiveStyle, StyleRestored);

CV_IMPORT(extern, const  VExclusiveStyle, StyleComboEdit);
CV_IMPORT(extern, const  VExclusiveStyle, StyleComboDrop);
CV_IMPORT(extern, const  VExclusiveStyle, StyleComboEditDrop);

CV_IMPORT(extern, const  VExclusiveStyle, StyleBorder);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoBorder);
CV_IMPORT(extern, const  VExclusiveStyle, StyleModalBorder);

CV_IMPORT(extern, const  VExclusiveStyle, StyleAutoToggle);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoAutoToggle);

CV_IMPORT(extern, const  VExclusiveStyle, StyleDefaultButton);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoDefaultButton);
                                            
CV_IMPORT(extern, const  VExclusiveStyle, StyleAutoDefault);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoAutoDefault);

CV_IMPORT(extern, const  VExclusiveStyle, StyleLeft);
CV_IMPORT(extern, const  VExclusiveStyle, StyleRight);
CV_IMPORT(extern, const  VExclusiveStyle, StyleCenter);

CV_IMPORT(extern, const  VExclusiveStyle, StyleVisible);
CV_IMPORT(extern, const  VExclusiveStyle, StyleHidden);

CV_IMPORT(extern, const  VExclusiveStyle, StyleWordWrap);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoWordWrap);

CV_IMPORT(extern, const  VExclusiveStyle, StyleAscending);
CV_IMPORT(extern, const  VExclusiveStyle, StyleDescending);
CV_IMPORT(extern, const  VExclusiveStyle, StyleUnsorted);

CV_IMPORT(extern, const  VExclusiveStyle, StyleCaption);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoCaption);

CV_IMPORT(extern, const  VExclusiveStyle, StyleHelpBox);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoHelpBox);

CV_IMPORT(extern, const  VExclusiveStyle, StyleDialogPM);
CV_IMPORT(extern, const  VExclusiveStyle, StyleDialogWin);

CV_IMPORT(extern, const  VExclusiveStyle, StyleEditable);
CV_IMPORT(extern, const  VExclusiveStyle, StyleEditSecure);
CV_IMPORT(extern, const  VExclusiveStyle, StyleReadOnly);

CV_IMPORT(extern, const  VExclusiveStyle, StyleShadow);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoShadow);

CV_IMPORT(extern, const  VExclusiveStyle, StyleSizeRedraw);
CV_IMPORT(extern, const  VExclusiveStyle, StyleNoSizeRedraw);

CV_IMPORT(extern, const  VStyle,		  StyleNative);

CV_IMPORT(extern, const  VExclusiveStyle, StyleSingleSelect);
CV_IMPORT(extern, const  VExclusiveStyle, StyleMultiSelect);

CV_IMPORT(extern, const  VExclusiveStyle, StyleFlatList);
CV_IMPORT(extern, const  VExclusiveStyle, StyleTreeList);


CV_IMPORT(extern, const  VExclusiveStyle, StyleTabRight);
CV_IMPORT(extern, const  VExclusiveStyle, StyleTabLeft);
CV_IMPORT(extern, const  VExclusiveStyle, StyleTabTop);
CV_IMPORT(extern, const  VExclusiveStyle, StyleTabBottom);

CV_IMPORT(extern, const  VExclusiveStyle, StyleSolidBinding);
CV_IMPORT(extern, const  VExclusiveStyle, StyleSpiralBinding);

CV_IMPORT(extern, const  VExclusiveStyle, StyleSingleRowTabs);
CV_IMPORT(extern, const  VExclusiveStyle, StyleMultipleRowTabs);
CV_IMPORT(extern, const  VExclusiveStyle, StyleButtonsTabs);

CV_IMPORT(extern, const  VStyle,	StyleLocalSelect);
CV_IMPORT(extern, const  VStyle, 	StyleDetachFocus);
CV_IMPORT(extern, const  VStyle,	StyleRowHilite);
CV_IMPORT(extern, const  VStyle,	StyleSelectByText);

CV_IMPORT(extern, const  VStyle,	StyleWorkTab);

CV_IMPORT(extern, const  VStyle,	StyleSimpleDock);
CV_IMPORT(extern, const  VStyle,	StyleUndocked);

CV_IMPORT(extern, const  VStyle,	StyleDragEnabled);

#if defined(MOTIF)
	extern CV_DLL_IMPORT const VExclusiveStyle StyleAutoScroll;
	extern CV_DLL_IMPORT const VExclusiveStyle StyleNoAutoScroll;

	extern CV_DLL_IMPORT const VExclusiveStyle StyleButtonShift;
    extern CV_DLL_IMPORT const VExclusiveStyle StyleNoButtonShift;
#endif


CV_IMPORT(extern, const VExclusiveStyle * , vStyleCloseBoxSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyle3DSet[]);

CV_IMPORT(extern, const VExclusiveStyle * , vStyleSizeableSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleHorizontalSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleVerticalSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleMaximizeSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleMinimizeSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleMaximizedSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleComboSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleBorderSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleAutoToggleSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleDefaultButtonSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleAutoDefaultSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleJustificationSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleVisiblitySet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleWordWrapSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleSortedSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleCaptionSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleHelpBoxSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleDialogSet[]);

#if defined(MOTIF)
CV_IMPORT(extern, const VExclusiveStyle * , vStyleAutoScrollSet[]);
#endif

CV_IMPORT(extern, const VExclusiveStyle * , vStyleEditSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleShadowSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleSizeRedrawSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleSelectionSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleButtonShiftSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleTabPlacementSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleTabTypeSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleNotebookBindingSet[]);
CV_IMPORT(extern, const VExclusiveStyle * , vStyleTreeDisplaySet[]);

// Alternate style names
#define StyleTitle		StyleCaption
#define StyleZoom			StyleMaximize
#define StyleGrowBox		StyleSizable
#define StyleZoomed		StyleMaximized
#define StyleIconic		StyleMinimized

#define StyleScrolls		(StyleHorizontal + StyleVertical)
#define StyleMaxMin		(StyleMaximize + StyleMinimize)
#define StyleSorted		StyleAscending

#undef CV_IMPORT

#endif // STYLES_H
