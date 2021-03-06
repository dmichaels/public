// $Header:   Q:/views/common/vcs/STYLES.CPV   1.23   Mar 24 1997 16:13:36   scottt  $

//  styles.cpp [Common]
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
//  09/01/95 dss    created.
//  09/20/95 dss    fix borland compile errors. Added StyleMultSelect,
//					StyleFlatList.
//  09/20/95 dss    added vStyleSelectionSet.
//  10/11/95 tomf   added StyleButtonShift for motif.
//	10/12/95 jld	added styles for notebooks and treelistbox
//	10/18/95 jld	changed StyleTabCenter to StyleTabTop
//	11/01/95 dgm	Added VDockableWindow styles.
//	11/02/95 jld	added StyleDragEnabled
//	11/10/95 dgm	added StyleUndocked.
//	12/06/95 dss	made styles objects const.
//	12/15/95 dss	Added CV_EXPORT.
//  02/19/96 evc    Fixed up the macros so they
//					dont make data FAR for static 16 bit case
//					i.e. #if defined(CV_WIN16) && !defined(DLL)
//						#define CV_EXPORT( type, name) type CV_NEARDATA name
//	02/19/96 evc	defined style static objects FAR in the 116 bit DLL case
//	12/17/96 dgm	Added StyleHelpBox/StyleNoHelpBox.
// ----------------------------------------------------------------------------

#include "object.h"
#include "implstyl.h"

// DECORATION is a quick solution.
// should use a macro and get rid of that 'extern' decoration
#if defined (CV_WIN16) && !defined (DLL)
// The 16 bit static case; all definitions are NEAR
#define DECORATION CV_NEARDATA
#else
#define DECORATION  CV_DLL_EXPORT  FAR
#endif


// MSVC2.0 bug: need the assignment to export StyleDefault.
extern const VStyle		  DECORATION StyleDefault = VStyle();

extern const VExclusiveStyle DECORATION StyleCloseBox(1);
extern const VExclusiveStyle DECORATION StyleNoCloseBox(2);

extern const VExclusiveStyle DECORATION Style3D(3);
extern const VExclusiveStyle DECORATION StyleNo3D(4);

extern const VExclusiveStyle DECORATION StyleSizable(5);
extern const VExclusiveStyle DECORATION StyleUnsizable(6);

extern const VExclusiveStyle DECORATION StyleHorizontal(7);
extern const VExclusiveStyle DECORATION StyleNoHorizontal(8);

extern const VExclusiveStyle DECORATION StyleVertical(9);
extern const VExclusiveStyle DECORATION StyleNoVertical(10);

extern const VExclusiveStyle DECORATION StyleMaximize(11);
extern const VExclusiveStyle DECORATION StyleNoMaximize(12);

extern const VExclusiveStyle DECORATION StyleMinimize(13);
extern const VExclusiveStyle DECORATION StyleNoMinimize(14);

extern const VExclusiveStyle DECORATION StyleMaximized(15);
extern const VExclusiveStyle DECORATION StyleMinimized(16);
extern const VExclusiveStyle DECORATION StyleRestored(17);

extern const VExclusiveStyle DECORATION StyleComboEdit(18);
extern const VExclusiveStyle DECORATION StyleComboDrop(19);
extern const VExclusiveStyle DECORATION StyleComboEditDrop(20);

extern const VExclusiveStyle DECORATION StyleBorder(21);
extern const VExclusiveStyle DECORATION StyleNoBorder(22);
extern const VExclusiveStyle DECORATION StyleModalBorder(23);

extern const VExclusiveStyle DECORATION StyleAutoToggle(24);
extern const VExclusiveStyle DECORATION StyleNoAutoToggle(25);

extern const VExclusiveStyle DECORATION StyleDefaultButton(26);
extern const VExclusiveStyle DECORATION StyleNoDefaultButton(27);
                                            
extern const VExclusiveStyle DECORATION StyleAutoDefault(28);
extern const VExclusiveStyle DECORATION StyleNoAutoDefault(29);

extern const VExclusiveStyle DECORATION StyleLeft(30);
extern const VExclusiveStyle DECORATION StyleRight(31);
extern const VExclusiveStyle DECORATION StyleCenter(32);

extern const VExclusiveStyle DECORATION StyleVisible(33);
extern const VExclusiveStyle DECORATION StyleHidden(34);

extern const VExclusiveStyle DECORATION StyleWordWrap(35);
extern const VExclusiveStyle DECORATION StyleNoWordWrap(36);

extern const VExclusiveStyle DECORATION StyleAscending(37);
extern const VExclusiveStyle DECORATION StyleDescending(38);
extern const VExclusiveStyle DECORATION StyleUnsorted(39);

extern const VExclusiveStyle DECORATION StyleCaption(40);
extern const VExclusiveStyle DECORATION StyleNoCaption(41);

extern const VExclusiveStyle DECORATION StyleHelpBox(79);
extern const VExclusiveStyle DECORATION StyleNoHelpBox(80);

extern const VExclusiveStyle DECORATION StyleDialogPM(42);
extern const VExclusiveStyle DECORATION StyleDialogWin(43);

extern const VExclusiveStyle DECORATION StyleEditable(44);
extern const VExclusiveStyle DECORATION StyleEditSecure(45);
extern const VExclusiveStyle DECORATION StyleReadOnly(46);

extern const VExclusiveStyle DECORATION StyleShadow(47);
extern const VExclusiveStyle DECORATION StyleNoShadow(48);

extern const VExclusiveStyle DECORATION StyleSizeRedraw(49);
extern const VExclusiveStyle DECORATION StyleNoSizeRedraw(50);

#if defined(MOTIF)
    extern const VExclusiveStyle DECORATION StyleAutoScroll(51);
    extern const VExclusiveStyle DECORATION StyleNoAutoScroll(52);
#endif

extern const VStyle DECORATION                StyleNative(53);
// 54 is reserved for whatever the exclusive counterpart to StyleNative is.

extern const VExclusiveStyle DECORATION StyleSingleSelect(55);
extern const VExclusiveStyle DECORATION   StyleMultiSelect(56);
// 57-59 reserved for selection styles

// Display styles for tree lists
extern const VExclusiveStyle DECORATION StyleFlatList(60);
extern const VExclusiveStyle DECORATION StyleTreeList(61);

#if defined(MOTIF)
    extern const VExclusiveStyle DECORATION StyleButtonShift(62);
    extern const VExclusiveStyle DECORATION StyleNoButtonShift(63);
#endif

// Tab placement styles for notebooks
extern const VExclusiveStyle DECORATION   StyleTabLeft(64);
extern const VExclusiveStyle DECORATION   StyleTabRight(65);
extern const VExclusiveStyle DECORATION   StyleTabTop(66);
extern const VExclusiveStyle DECORATION   StyleTabBottom(67);

// Tab types for notebooks
extern const VExclusiveStyle DECORATION   StyleSingleRowTabs(68);
extern const VExclusiveStyle DECORATION   StyleMultiRowTabs(69);
extern const VExclusiveStyle DECORATION   StyleButtonTabs(70);

// Binding type for notebooks
extern const VExclusiveStyle DECORATION   StyleSolidBinding(71);
extern const VExclusiveStyle DECORATION   StyleSpiralBinding(72);

// styles for tree list box
extern const VStyle DECORATION	StyleLocalSelect(71);
extern const VStyle DECORATION	StyleDetachFocus(72);
extern const VStyle DECORATION	StyleRowHilite(73);
extern const VStyle DECORATION	StyleSelectByText(74);

extern const VStyle DECORATION	StyleWorkTab(75);

// Styles for VDockableWindow.
// We will probably only end up needing one of these;
// the other will then be removed.

extern const VStyle DECORATION	StyleSimpleDock(76);
extern const VStyle DECORATION	StyleUndocked(77);

extern const VStyle DECORATION	StyleDragEnabled(78);

// some compilers are picky about export syntax
// This macro does the DEFINITION for that data

#if (defined(__BCPLUSPLUS__) || defined(__SC__)) && defined(DLL)
	#define CV_EXPORT( type, name) type FAR CV_DLL_EXPORT name
#elif defined(CV_WIN16) && !defined(DLL)
	#define CV_EXPORT( type, name) type CV_NEARDATA name
#elif defined(CV_WIN32) && !defined(DLL)
	#define CV_EXPORT( type, name) type name
#elif defined(CV_WIN32)
	#define CV_EXPORT( type, name) CV_DLL_EXPORT type name
#elif defined(CV_WIN16) && defined(DLL)
	#define CV_EXPORT( type, name) type FAR CV_DLL_EXPORT name
#elif defined(IBMCPP)
	#define CV_EXPORT( type, name) type FAR  CV_DLL_EXPORT name
#else
	#define CV_EXPORT( type, name) type FAR  name
#endif

CV_EXPORT( const VExclusiveStyle * , vStyleCloseBoxSet[])
		= {&StyleCloseBox, &StyleNoCloseBox, 0};
CV_EXPORT( const VExclusiveStyle * , vStyle3DSet[])
		= {&Style3D, &StyleNo3D, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleSizeableSet[])
		= {&StyleSizable, &StyleUnsizable, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleHorizontalSet[])
		= {&StyleHorizontal, &StyleNoHorizontal, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleVerticalSet[])
		= {&StyleVertical, &StyleNoVertical, 0};
CV_EXPORT( const VExclusiveStyle *  , vStyleMaximizeSet[])
		= {&StyleMaximize, &StyleNoMaximize, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleMinimizeSet[])
		= {&StyleMinimize, &StyleNoMinimize, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleMaximizedSet[])
		= {&StyleMaximized, &StyleMinimized, &StyleRestored, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleComboSet[])
		= {&StyleComboEdit, &StyleComboDrop, &StyleComboEditDrop, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleBorderSet[])
		= {&StyleBorder, &StyleNoBorder, &StyleModalBorder, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleAutoToggleSet[])
		= {&StyleAutoToggle, &StyleNoAutoToggle, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleDefaultButtonSet[])
		= {&StyleDefaultButton, &StyleNoDefaultButton, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleAutoDefaultSet[])
		= {&StyleAutoDefault, &StyleNoAutoDefault, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleJustificationSet[])
		= {&StyleLeft, &StyleRight, &StyleCenter, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleVisiblitySet[])
		= {&StyleHidden, &StyleVisible, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleWordWrapSet[])
		= {&StyleWordWrap, &StyleNoWordWrap, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleSortedSet[])
		= {&StyleAscending, &StyleDescending, &StyleUnsorted, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleCaptionSet[])
		= {&StyleCaption, &StyleNoCaption, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleHelpBoxSet[])
		= {&StyleHelpBox, &StyleNoHelpBox, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleDialogSet[])
		= {&StyleDialogPM, &StyleDialogWin, 0};
#if defined(MOTIF)
	CV_EXPORT( const VExclusiveStyle * , vStyleAutoScrollSet[])
		= {&StyleAutoScroll, &StyleNoAutoScroll, 0};
#endif
CV_EXPORT( const VExclusiveStyle * , vStyleEditSet[])
		= {&StyleEditable, &StyleEditSecure, &StyleReadOnly, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleShadowSet[])
		= {&StyleShadow, &StyleNoShadow, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleSizeRedrawSet[])
		= {&StyleSizeRedraw, &StyleNoSizeRedraw, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleSelectionSet[])
		= {&StyleSingleSelect, &StyleMultiSelect, 0};
#if defined(MOTIF)
	CV_EXPORT( const VExclusiveStyle * , vStyleButtonShiftSet[])
		= {&StyleButtonShift, &StyleNoButtonShift, 0};
#endif
CV_EXPORT( const VExclusiveStyle * , vStyleTabPlacementSet[])
		= {&StyleTabLeft, &StyleTabRight, &StyleTabTop, &StyleTabBottom, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleTabTypeSet[])
		= {&StyleSingleRowTabs, &StyleMultiRowTabs, &StyleButtonTabs, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleNotebookBindingSet[])
		= {&StyleSolidBinding, &StyleSpiralBinding, 0};
CV_EXPORT( const VExclusiveStyle * , vStyleTreeDisplaySet[])
		= { &StyleFlatList, &StyleTreeList, 0 };
