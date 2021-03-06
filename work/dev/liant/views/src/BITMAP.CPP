// $Header:   Q:/views/win/vcs/bitmap.cpv   1.12   Mar 04 1997 17:16:38   gregm  $ 

// bitmap.cpp
//
//	VBitMap implementation
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
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
//	01/23/97 dgm	Redone.
// --------------------------------------------------------------------------

#include "bitmap.h"
#include "color.h"
#include "location.h"
#include "port.h"
#include "ordcllct.h"
#include "iterator.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "rscarch.h"
#endif

#include <windows.h>
#include <stdlib.h>
#include <memory.h>

#ifdef CV_WIN32
#	undef	CV_NO_CHANGE_DIB_FORMAT
#	undef	CV_NO_OPTIMIZE_PALETTE
#else
#	define	CV_NO_CHANGE_DIB_FORMAT
#	define	CV_NO_OPTIMIZE_PALETTE
	typedef unsigned long	DWORD;
	typedef int				BOOL;
	typedef unsigned char	BYTE;
	typedef long			LONG;
	typedef int				INT;
	typedef unsigned short	WORD;
	typedef unsigned int	UINT;
#	ifndef BI_BITFIELDS
#		define BI_BITFIELDS	3L
#	endif
	void CopyMemory(void *, void *, unsigned long);
#endif

// ==========================================================================
// ::: VPalettes Implementation (internal)
// ==========================================================================

class VPalettes {
public:
				   ~VPalettes();
	static HPALETTE	createPalette(LPLOGPALETTE);
	static void		deletePalette(HPALETTE);
	static void		sharePalette(HPALETTE);
protected:
	CLASS VPaletteData : public VObject {
	public:
					VPaletteData(HPALETTE);
				   ~VPaletteData();
		void		countUp();
		void		countDown();
		HPALETTE	getPalette() const;
		long		getUseCount() const;
	protected:
		void		destruct();
	private:
		HPALETTE	hPalette;
		long		useCount;
	};
protected:
	static VPaletteData  *findPalette(HPALETTE);
	static void			  deleteAllPalettes();
private:
	static VOrdCollect	 *sharedPalettes;
};

// --------------------------------------------------------------------------
//
VOrdCollect *VPalettes::sharedPalettes = 0;
static VPalettes CV_NEARDATA shared_palettes_deleter;

// --------------------------------------------------------------------------
//
inline VPalettes::VPaletteData::VPaletteData(HPALETTE hpalette)
{
	hPalette = hpalette;
	useCount = 1;
}

// --------------------------------------------------------------------------
//
inline VPalettes::VPaletteData::~VPaletteData()
{
	destruct();
}

// --------------------------------------------------------------------------
//
inline HPALETTE VPalettes::VPaletteData::getPalette() const
{
	return hPalette;
}

// --------------------------------------------------------------------------
//
inline long VPalettes::VPaletteData::getUseCount() const
{
	return useCount;
}

// --------------------------------------------------------------------------
//
inline void VPalettes::VPaletteData::countUp()
{
	useCount++;
}

// --------------------------------------------------------------------------
//
inline void VPalettes::VPaletteData::countDown()
{
	if (--useCount <= 0) {
		destruct();
	}
}

// --------------------------------------------------------------------------
//
void VPalettes::VPaletteData::destruct()
{
	useCount = 0;
	if (hPalette != 0) {
		DeleteObject(HGDIOBJ(hPalette));
		hPalette = 0;
	}
}

// --------------------------------------------------------------------------
//
void VPalettes::sharePalette(HPALETTE hpalette)
{
	if (sharedPalettes == 0) {
		sharedPalettes = new VOrdCollect;
		if (sharedPalettes != 0) {
			sharedPalettes->add(new VPaletteData(hpalette));
		}
	}
	else {
		VPaletteData *spd = findPalette(hpalette);
		if (spd != 0) {
			spd->countUp();
		}
		else {
			sharedPalettes->add(new VPaletteData(hpalette));
		}
	}
}

// --------------------------------------------------------------------------
//
VPalettes::VPaletteData *VPalettes::findPalette(HPALETTE hpalette)
{
	if (sharedPalettes != 0) {
		DO (*sharedPalettes, VPaletteData, spd)
			if (spd->getPalette() == hpalette) {
				return spd;
			}
		END
	}
	return 0;
}

// --------------------------------------------------------------------------
//
HPALETTE VPalettes::createPalette(LPLOGPALETTE logpalette)
{
	HPALETTE hpalette = CreatePalette(logpalette);
	sharePalette(hpalette);
	return hpalette;
}

// --------------------------------------------------------------------------
//
void VPalettes::deletePalette(HPALETTE hpalette)
{
	VPaletteData *spd = findPalette(hpalette);
	if (spd != 0) {
		spd->countDown();
	}
}

// --------------------------------------------------------------------------
//
void VPalettes::deleteAllPalettes()
{
	if (sharedPalettes != 0) {
		sharedPalettes->freeContents();
		delete sharedPalettes;
		sharedPalettes = 0;
	}
}

// --------------------------------------------------------------------------
//
VPalettes::~VPalettes()
{
	deleteAllPalettes();
}

// ==========================================================================
// ::: Archiver/File Open/Create/Read/Write/Close Interface (internal)
// ==========================================================================

class FileOrArchive {
public:
					FileOrArchive(const char *);
					FileOrArchive(VArchiver *);
	int				openForRead();
	int				openForWrite();
	int				read(char *, long);
	int				write(char *, long);
	void			close();
	long			getPosition() const;
	int				setPosition(long);
	int				shiftPosition(long);
	int				isArchive() const;
	int				isFile() const;
private:
	VArchiver  *archiver;
	const char *filename;
	HFILE		hfile;
};

// --------------------------------------------------------------------------
//
FileOrArchive::FileOrArchive(const char *s)
{
	archiver = 0;
	filename = s;
	hfile = HFILE_ERROR;
}

// --------------------------------------------------------------------------
//
FileOrArchive::FileOrArchive(VArchiver *a)
{
	archiver = a;
	filename = 0;
	hfile = HFILE_ERROR;
}

// --------------------------------------------------------------------------
//
int FileOrArchive::openForRead()
{
	if (archiver != 0) {
		//
		// Assume caller has opened VArchiver for reading.
		//
		return 1;
	}
	else if (hfile != HFILE_ERROR) {
		return 1;
	}
	else if (filename != 0) {
		OFSTRUCT dummy;
	    hfile = OpenFile(filename, &dummy, UINT(OF_READ));
		if (hfile != HFILE_ERROR) {
			return 1;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
FileOrArchive::openForWrite()
{
	if (archiver != 0) {
		//
		// Assume caller has opened VArchiver for writing.
		//
		return 1;
	}
	else if (hfile != HFILE_ERROR) {
		return 1;
	}
	else if (filename != 0) {
		OFSTRUCT dummy;
	    hfile = OpenFile(filename, &dummy, UINT(OF_CREATE | OF_READWRITE));
		if (hfile != HFILE_ERROR) {
			return 1;
		}
	}
	return 0;
}


// --------------------------------------------------------------------------
//
int FileOrArchive::read(char *buffer, long nbytes)
{
	if (archiver != 0) {
		if (archiver->read((char *)buffer, nbytes) == nbytes) {
			return 1;
		}
	}
	else if (hfile != HFILE_ERROR) {
		if (_hread(hfile, LPSTR(buffer), UINT(nbytes)) == nbytes) {
			return 1;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
int FileOrArchive::write(char *buffer, long nbytes)
{
	if (archiver != 0) {
		if (archiver->write((const char *)buffer, nbytes) == nbytes) {
			return 1;
		}
	}
	else if (hfile != HFILE_ERROR) {
		if (_hwrite(hfile, LPSTR(buffer), UINT(nbytes)) == nbytes) {
			return 1;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
void FileOrArchive::close()
{
	if (archiver != 0) {
		//
		// Assume caller will close VArchiver.
		//
	}
	else if (hfile != 0) {
		_lclose(hfile);
		hfile = 0;
	}
}

// --------------------------------------------------------------------------
//
long FileOrArchive::getPosition() const
{
	if (archiver != 0) {
		VRscArchiver *ra = archiver->isA(VRscArchiverCls) ?
						   (VRscArchiver *)archiver : 0;
		if (ra != 0) {
			return ra->getFile().getFile().at();
		}
		else {
			return archiver->getFile().at();
		}
	}
	else if (hfile != 0) {
		return _llseek(hfile, 0L, UINT(SEEK_CUR));
	}
	return 0;
}

// --------------------------------------------------------------------------
//
int FileOrArchive::setPosition(long position)
{
	if (archiver != 0) {
		VRscArchiver *ra = archiver->isA(VRscArchiverCls) ?
						   (VRscArchiver *)archiver : 0;
		long current;
		if (ra != 0) {
			current = ra->getFile().getFile().at();
			if (position < current) {
				ra->getFile().getFile().at(position);
				return 1;
			}
		}
		else {
			current = archiver->getFile().at();
			if (position < current) {
				archiver->getFile().at(position);
				return 1;
			}
		}
		if (position > current) {
			char *buffer = new char[position - current];
			archiver->read(buffer, position - current);
			delete buffer;
		}
		return 1;
	}
	else if (hfile != 0) {
		_llseek(hfile, position, UINT(SEEK_SET));
		return 1;
	}
	return 0;
}

// --------------------------------------------------------------------------
//
int FileOrArchive::shiftPosition(long position)
{
	if (archiver != 0) {
		if (position > 0) {
			char *buffer = new char[position];
			archiver->read(buffer, position);
			delete buffer;
		}
		else {
			VRscArchiver *ra = archiver->isA(VRscArchiverCls) ?
							   (VRscArchiver *)archiver : 0;
			if (ra != 0) {
				ra->getFile().getFile().at
					(position + ra->getFile().getFile().at());
			}
			else {
				archiver->getFile().at(position + archiver->getFile().at());
			}
		}
		return 1;
	}
	else if (hfile != 0) {
		return _llseek(hfile, position, UINT(SEEK_CUR));
		return 1;
	}
	return 0;
}

// --------------------------------------------------------------------------
//
int FileOrArchive::isArchive() const
{
	return archiver != 0;
}

// --------------------------------------------------------------------------
//
int FileOrArchive::isFile() const
{
	return archiver == 0;
}

// ==========================================================================
// ::: COLORMAP Interface (internal)
// ==========================================================================

#define MAKE556WORD(r,g,b) ((((WORD)(r) >> 3) << 11) | (((WORD)(g) >> 3) << 6)  | ((WORD)(b) >> 2));    
#define MAKE565WORD(r,g,b) ((((WORD)(r) >> 3) << 11) | (((WORD)(g) >> 2) << 5)  | ((WORD)(b) >> 3));    
#define MAKE555WORD(r,g,b) ((((WORD)(r) >> 3) << 10) | (((WORD)(g) >> 3) << 5)  | ((WORD)(b) >> 3)); 

#define MAKE565RGB(w,r,g,b) (r = (w & 0xF800) >> 8, g = (w & 0x07E0) >> 3, b = (w & 0x001F) << 3); 
#define MAKE555RGB(w,r,g,b) (r = (w & 0x7C00) >> 7, g = (w & 0x03E0) >> 2, b = (w & 0x001F) << 3); 


static HANDLE OpenColorMapArray();
static void AddColorToArray(HANDLE, RGBTRIPLE*);
static void GetOptimizedPalette(HANDLE, RGBQUAD*, int);
static HANDLE CloseColorMapArray(HANDLE);

// ==========================================================================
// ::: PALETTE Interface (internal)
// ==========================================================================

#define PALVERSION      0x300
#define MAXPALETTE      256       /* max. # supported palette entries */

static HPALETTE    CreatePaletteFromRGBQUAD(LPRGBQUAD, WORD);
static int         PalEntriesOnDevice(HDC);
static HPALETTE    GetSystemPalette();
static HPALETTE    CreateSpectrumPalette();  
static HPALETTE    CopyPalette(HPALETTE);
static HPALETTE    CopyPaletteEx(HPALETTE, BYTE);
static HPALETTE    CreateIdentityPalette(LPRGBQUAD, int);
static void        ClearSystemPalette(void);

// ==========================================================================
// ::: DIB Interface (internal)
// ==========================================================================
 
/* Macro to restrict a given value to an upper or lower boundary value */
#define BOUND(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/* Macro to swap two values */
#define SWAP(x, y)   ((x)^=(y)^=(x)^=(y))

#define NEW_DIB_FORMAT(lpbih) (lpbih->biSize != sizeof(BITMAPCOREHEADER))

/* Macro to align given value to the closest DWORD (unsigned long) */
#define ALIGNULONG(i)   ((((i)+3) >> 2) << 2)

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)   ((((i)+31) >> 5) << 2)

/* Macro to determine the number of bytes in a DWORD aligned DIB scanline */
#define BYTESPERLINE(Width, BPP) ((WORD)((((DWORD)(Width) * (DWORD)(BPP) + 31) >> 5)) << 2)

#define MAXPALETTE      256       /* max. # supported palette entries */

/* Header signatutes for various resources */
#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

/* macro to determine if resource is a DIB */
#define ISDIB(bft) ((bft) == BFT_BITMAP)

#define RAISE_AN_EXCEPTION()
#define __try
#define __except(e)

/***********************************************************/
/* Declarations of functions used in dib.c module          */
/***********************************************************/

static HANDLE      OpenDIB (FileOrArchive&);
static BOOL        WriteDIB (FileOrArchive&, HANDLE);
static BOOL        WriteDIBEx(FileOrArchive&, HANDLE, WORD, DWORD);
static WORD        ColorTableSize (LPVOID);
static WORD        DIBNumColors (LPVOID);
static HPALETTE    CreateDIBPalette (HANDLE);
static HPALETTE    CreateBIPalette (LPBITMAPINFOHEADER);
static HANDLE      DIBFromBitmap (HBITMAP, DWORD, WORD, HPALETTE);
static HBITMAP     BitmapFromDIB (HANDLE, HPALETTE);
static BOOL        DIBBlt (HDC, INT, INT, INT, INT, HANDLE, INT, INT, LONG);
static BOOL        StretchDIBBlt (HDC, INT, INT, INT, INT, HANDLE, INT, INT, INT, INT, LONG);
static BOOL        DIBInfo (HANDLE hdib, LPBITMAPINFOHEADER);
static HANDLE      ReadDIBBitmapInfo (FileOrArchive&);
static BOOL        DrawBitmap (HDC, INT, INT, HBITMAP, DWORD);
static VOID        WritePackedFileHeader(FileOrArchive&, LPBITMAPFILEHEADER);
static HANDLE 		ChangeDIBFormat(HANDLE, WORD, DWORD);
static BOOL        ChangeDIBPalette(HANDLE, HPALETTE);
static HANDLE 		CopyDIB(HANDLE);
static HBITMAP     CopyBitmap(HBITMAP);
static HBITMAP     CropBitmap(HBITMAP, LPRECT);
static BOOL 		CopyDIBData(HANDLE, HANDLE);
static BOOL 		GetDIBResolution(HANDLE, LPLONG, LPLONG);
static DWORD       GetDIBCompression(HANDLE);
static WORD        GetDIBBitCount(HANDLE);
static HPALETTE    CreateDIBPaletteEx(HANDLE, BYTE);
static DWORD 		GetDIBColorUsed(HANDLE);
static void        SetOptimizedPaletteState(BOOL);
static BOOL        GetOptimizedPaletteState();

/* The following should be removed from the sample */
 
BOOL 		GetAndLockDIBPointers(HANDLE, LPVOID*, LPVOID*, LPVOID*);
HANDLE 		CreateRGBDIB(DWORD, DWORD, WORD, DWORD);

/*****************************************************/
/* make new sizeof structs to cover dword alignment  */
/*****************************************************/

#define SIZEOF_BITMAPFILEHEADER_PACKED  (   \
    sizeof(WORD) +      /* bfType      */   \
    sizeof(DWORD) +     /* bfSize      */   \
    sizeof(WORD) +      /* bfReserved1 */   \
    sizeof(WORD) +      /* bfReserved2 */   \
    sizeof(DWORD))      /* bfOffBits   */

// ==========================================================================
// ::: COLORMAP Implementation (internal)
// ==========================================================================

/*******************************************************************************
 *                                                                             *
 *  MODULE      : COLORMAP.C                                                   *
 *                                                                             *
 *  DESCRIPTION : Routines for creating a resonable palette for a DIB          *
 *                                                                             *
 *  FUNCTIONS:                                                                 * 
 *                                                                             *
 *    OpenColorMapArray() - Returns a handle to an initialized color           *
 *                          histogram                                          *
 *                                                                             *
 *    AddColorToArray()   - Adds a color to a color histogram                  *
 *                                                                             *
 *    GetOptimizedPalette() - Returns a list of the most frequently added      *
 *                          colors in the histogram.                           *  
 *                                                                             *
 *    CloseColorMapArray() - Deletes a handle to a color histogram             * 
 *                                                                             *
 *  *Note:  Using this code to obtain a palette for 16,24, and 32BPP DIBs is   *
 *          not always going to give you the best looking palette. One example *
 *          where this method would crash and burn would be calculating a      *
 *          palette for a DIB that used an even sampling of the spectrum.      * 
 *          In this case, the sort would take a long time and the most         *
 *          frequently used colors would cover the spectrum unevenly.          *
 *                                                                             *  
 ******************************************************************************/

#ifndef MAXWORD
#define MAXWORD 0xffff
#endif

#if defined(CV_WIN16) && (MAXWORD == 0xffff)
#	undef MAXWORD
#	define MAXWORD 16382
#endif

typedef struct {
    WORD wFreq;
	WORD wPos;
} COLORMAP;
typedef COLORMAP COLORARR[MAXWORD+1];
typedef COLORMAP *LPCOLORMAP;

static HANDLE OpenColorMapArray() 
{
#ifdef	CV_NO_OPTIMIZE_PALETTE
	return 0;
#else
  LPCOLORMAP cp = (LPCOLORMAP)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(COLORARR));
  WORD wPos;
  
  if (cp != NULL) {
    for (wPos = 0; wPos < MAXWORD; wPos++)
	  cp[wPos].wPos = wPos;
    cp[wPos].wPos = wPos; // Avoid rolling over in for-loop
  }

  return HANDLE(cp); 
#endif
}

static void AddColorToArray(HANDLE hColorMap, RGBTRIPLE *rgb) 
{
#ifndef	CV_NO_OPTIMIZE_PALETTE
   LPCOLORMAP cp = (LPCOLORMAP)hColorMap;
   WORD wPos = MAKE565WORD(rgb->rgbtRed,rgb->rgbtGreen,rgb->rgbtBlue);
    
   if (cp[wPos].wFreq < MAXWORD) 
     cp[wPos].wFreq++;
#endif
}

static int _cdecl Compare(const void *elem1, const void *elem2)
{ 
  return (int)((LPCOLORMAP)elem2)->wFreq - (int)((LPCOLORMAP)elem1)->wFreq;
}

static void GetOptimizedPalette(HANDLE hColorMap, RGBQUAD *rgbt, int iNumColors)
{
#ifdef	CV_NO_OPTIMIZE_PALETTE
	return;
#else
   WORD wPos;
   LPCOLORMAP cp = (LPCOLORMAP)hColorMap;
  
   if (iNumColors < 1)
     return;
   
   qsort((void*)cp, (size_t)MAXWORD, sizeof(COLORMAP), Compare); 
   
   for (wPos = 0; wPos < iNumColors; wPos++)
     MAKE565RGB(cp[wPos].wPos, rgbt[wPos].rgbRed, 
                               rgbt[wPos].rgbGreen, 
                               rgbt[wPos].rgbBlue);
#endif
}

static HANDLE CloseColorMapArray(HANDLE hColorMap)
{
#ifdef	CV_NO_OPTIMIZE_PALETTE
	return 0;
#else
  return GlobalFree(hColorMap);
#endif
}

// ==========================================================================
// ::: PALETTE Implementation (internal)
// ==========================================================================

/*******************************************************************************
 *                                                                             *
 *  MODULE      : PALETTE.C                                                    *
 *                                                                             *
 *  DESCRIPTION : Routines for dealing with palettes                           *
 *                                                                             *
 *    PalEntriesOnDevice()   - number of palette entries on device             *
 *																			   *
 *    CreatePaletteFromRGBQUAD() - Build a valid HPALETTE when given an array  *
 *                          of RGBQUADs                                        *
 *                                                                             *
 *    GetSystemPalette()  - returns a handle to a copy of the current system   *
 *                          palette                                            *  
 *                                                                             *
 *    CreateSpectrumPalette() - Build a palette with a spectrum of colors.     *
 *                                                                             *
 *    CopyPalette()       - Make a copy of a palette.                          *
 *																			   *
 *    CopyPaletteEx()     - Copy a palette and alter the flags                 *
 *                                                                             *
 *    CreateIdentityPalette() - Creates an identity palette from an array of   *
 *                          RGBQUADS.                                          *
 *                                                                             *
 *    ClearSystemPalette()- Clears the system palette.                         *
 *                                                                             *
 ******************************************************************************/

/******************************************************************************
 *                                                                            *
 * PalEntriesOnDevice()                                                       *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HDC hDC          - device context                                          *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * int              - number of palette entries on device                     *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function gets the number of palette entries on the specified device   *
 *                                                                            *
 * History:   Date      Author               Reason                           *
 *            6/01/91   Garrett McAuliffe    Created                          *
 *            9/15/91   Patrick Schreiber    Added header and comments        *
 *                                                                            *
 *****************************************************************************/   
static int PalEntriesOnDevice(HDC hDC)                                      
{                                                                        
   int nColors;  // number of colors                                     

   /*  Find out the number of palette entries on this
    *  device.
    */

   nColors = GetDeviceCaps(hDC, SIZEPALETTE);

   /*  For non-palette devices, we'll use the # of system
    *  colors for our palette size.
    */
   if (!nColors)
      nColors = GetDeviceCaps(hDC, NUMCOLORS);   

   return nColors;
}

/******************************************************************************
 *                                                                            *
 * HPALETTE CreatePaletteFromRGBQUAD(LPRGBQUAD rgbqPalette, WORD wEntries)    *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * LPRGBQUAD        - pointer to RGBQUADs                                     *
 * WORD             - the number of RGBQUADs we are pointing to               *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HPALETTE         - returns a handle to a palette or NULL if it failes      *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will build a valid HPALETTE when given an array of RGBQUADs  *
 *                                                                            *
 *****************************************************************************/
static HPALETTE CreatePaletteFromRGBQUAD(LPRGBQUAD rgbqPalette, WORD wEntries)
{ 
    HPALETTE hPal;
    LPLOGPALETTE lplgPal;
    int i;

    lplgPal = (LPLOGPALETTE)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * wEntries); 
    if (!lplgPal)
      return NULL;

    lplgPal->palVersion = PALVERSION;
    lplgPal->palNumEntries = wEntries;

    for (i=0; i<wEntries; i++) {
      lplgPal->palPalEntry[i].peRed   = rgbqPalette[i].rgbRed;
      lplgPal->palPalEntry[i].peGreen = rgbqPalette[i].rgbGreen;
      lplgPal->palPalEntry[i].peBlue  = rgbqPalette[i].rgbBlue;
      lplgPal->palPalEntry[i].peFlags = 0;
    }

    // hPal = CreatePalette(lplgPal);
	hPal = VPalettes::createPalette(lplgPal);
    GlobalFree(HGLOBAL(lplgPal));

    return hPal;
}

/******************************************************************************  
 *                                                                            *  
 * GetSystemPalette()                                                         *  
 *                                                                            *  
 * Parameters:                                                                *  
 *                                                                            *  
 * None                                                                       *  
 *                                                                            *  
 * Return Value:                                                              *  
 *                                                                            *  
 * HPALETTE         - handle to a copy of the current system palette          *  
 *                                                                            *  
 * Description:                                                               *  
 *                                                                            *  
 * This function returns a handle to a palette which represents the system    *  
 * palette.  The system RGB values are copied into our logical palette        *  
 * using the GetSystemPaletteEntries function.                                *  
 *                                                                            *  
 * History:                                                                   *  
 *                                                                            *  
 *    Date      Author               Reason                                   *
 *    6/01/91   Garrett McAuliffe    Created                                  *
 *    9/15/91   Patrick Schreiber    Added header and comments                *
 *    12/20/91  Mark Bader           Added GetSystemPaletteEntries call       *
 *                                                                            *
 *****************************************************************************/  
static HPALETTE GetSystemPalette()
{
   HDC hDC;                // handle to a DC
   static HPALETTE hPal = NULL;   // handle to a palette
   HANDLE hLogPal;         // handle to a logical palette
   LPLOGPALETTE lpLogPal;  // pointer to a logical palette
   int nColors;            // number of colors

   /* Find out how many palette entries we want. */

   hDC = GetDC(NULL);
   if (!hDC)
      return NULL;
   nColors = PalEntriesOnDevice(hDC);   // Number of palette entries

   /* Allocate room for the palette and lock it. */
   hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColors * sizeof(
             PALETTEENTRY));

   /* if we didn't get a logical palette, return NULL */
   if (!hLogPal)
      return NULL;

   /* get a pointer to the logical palette */
   lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal);

   /* set some important fields */
   lpLogPal->palVersion = PALVERSION;
   lpLogPal->palNumEntries = nColors;

   /* Copy the current system palette into our logical palette */

   GetSystemPaletteEntries(hDC, 0, nColors, 
                           (LPPALETTEENTRY)(lpLogPal->palPalEntry));

   /*  Go ahead and create the palette.  Once it's created, 
    *  we no longer need the LOGPALETTE, so free it.
    */

	// hPal = CreatePalette(lpLogPal);
	hPal = VPalettes::createPalette(lpLogPal);

   /* clean up */
   GlobalUnlock(hLogPal);
   GlobalFree(hLogPal);
   ReleaseDC(NULL, hDC);

   return hPal;
}

/******************************************************************************
 *                                                                            *
 * HPALETTE CreateSpectrumPalette()                                           *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * (none)                                                                     *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HPALETTE         - Returns a handle to a spectrum palette or NULL if it    *
 *                    fails.                                                  *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will build a palette with a spectrum of colors.  It is       *
 * useful when you want to display a number of DIBs each with a different     *
 * palette yet still have an a good selection of colors for the DIBs to map   *
 * to.                                                                        *
 *                                                                            *
 *****************************************************************************/
static HPALETTE CreateSpectrumPalette() 
{                 
    HPALETTE hPal;
    LPLOGPALETTE lplgPal;
    BYTE red, green, blue;
    int i; 
    
    lplgPal = (LPLOGPALETTE)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * MAXPALETTE); 
    if (!lplgPal)
      return NULL;
    
    lplgPal->palVersion = PALVERSION;
    lplgPal->palNumEntries = MAXPALETTE;
     
    red = green = blue = 0;
    for (i = 0; i < MAXPALETTE; i++) {
        lplgPal->palPalEntry[i].peRed   = red;
        lplgPal->palPalEntry[i].peGreen = green;
        lplgPal->palPalEntry[i].peBlue  = blue;
        lplgPal->palPalEntry[i].peFlags = (BYTE)0;

        if ((red += 32) == 0)
            if ((green += 32) == 0)
                blue += 64;
    }

    // hPal = CreatePalette(lplgPal);
	hPal = VPalettes::createPalette(lplgPal);
    GlobalFree(HGLOBAL(lplgPal));

    return hPal;
}

/******************************************************************************
 *                                                                            *
 * HANDLE CopyPalette(HPALETTE hPal)                                          *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HPALETTE         - The handle to the palette you want to make a copy of.   *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HPALETTE         - A copy of the palette passed as a parameter             *  
 *                                                                            *
 *****************************************************************************/
static HPALETTE CopyPalette(HPALETTE hPal)
{
  HPALETTE hPalNew; 
  LPLOGPALETTE lplgPal;
  WORD iNumEntries;
  
  GetObject(hPal, sizeof(iNumEntries), &iNumEntries);
    
  lplgPal = (LPLOGPALETTE)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * iNumEntries); 
  if (!lplgPal)
    return NULL;

  lplgPal->palVersion = PALVERSION;
  lplgPal->palNumEntries = GetPaletteEntries(hPal, 0, iNumEntries, lplgPal->palPalEntry);
  
  // hPalNew = CreatePalette(lplgPal);
  hPalNew = VPalettes::createPalette(lplgPal);
  GlobalFree(HGLOBAL(lplgPal));

  return (hPalNew);
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : CopyPaletteEx(HPALETTE hPal, BYTE bFlags)                    *
 *                                                                            *
 *  PURPOSE    : Given a handle to a palette, this function return a copy of  *
 *               the palette with all of the flags set to a the specified     *
 *               value.											              *
 *                                                                            *
 *  RETURNS    : A handle to the palette.                                     *
 *                                                                            *
 *****************************************************************************/
static HPALETTE CopyPaletteEx(HPALETTE hPal, BYTE bFlag)
{
   int i;
   WORD wNumColors;
   PALETTEENTRY pe[MAXPALETTE];
   HPALETTE hNewPal;

   if (!hPal || !GetObject(hPal, sizeof(wNumColors), &wNumColors))
     return NULL;
   
   hNewPal = CopyPalette(hPal);
   GetPaletteEntries(hNewPal, 0, wNumColors, (LPPALETTEENTRY)&pe);

   for (i=0; i<(int)wNumColors; i++) 
      pe[i].peFlags = bFlag;
   
   SetPaletteEntries(hNewPal, 0, wNumColors, (LPPALETTEENTRY)&pe);

   return hNewPal;
}

/******************************************************************************
 *                                                                            *
 *  HPALETTE CreateIdentityPalette(LPRGBQUAD lprgbq, int nColors)             *
 *                                                                            *
 * LPRGBQUAD        - pointer to RGBQUADs                                     *
 * int              - the number of RGBQUADs we are pointing to               *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HPALETTE         - returns a handle to an identity palette or NULL if it   *
 *                    fails                                                   *
 *                                                                            *
 * *Note: This code was originally taken from the WinG online docs            *
 *****************************************************************************/
static HPALETTE CreateIdentityPalette(LPRGBQUAD lprgbq, int nColors)
{
	int i;
	struct {
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[MAXPALETTE];
	} Palette =
	{
		0x300, 
		MAXPALETTE
	};

	//*** Just use the screen DC where we need it
	HDC hDC = GetDC(NULL);

	//*** For SYSPAL_NOSTATIC, just copy the color table into
	//*** a PALETTEENTRY array and replace the first and last entries
	//*** with black and white
	if (GetSystemPaletteUse(hDC) == SYSPAL_NOSTATIC)

	{
		//*** Fill in the palette with the given values, marking each
		//*** as PC_NOCOLLAPSE
		for(i = 0; i < nColors; i++)
		{
			Palette.aEntries[i].peRed = lprgbq[i].rgbRed;
			Palette.aEntries[i].peGreen = lprgbq[i].rgbGreen;
			Palette.aEntries[i].peBlue = lprgbq[i].rgbBlue;
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE;
		}

		//*** Mark any unused entries PC_NOCOLLAPSE
		for (; i < MAXPALETTE; ++i)
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE;

		//*** Make sure the last entry is white
		//*** This may replace an entry in the array!
		Palette.aEntries[255].peRed = 255;
		Palette.aEntries[255].peGreen = 255;
		Palette.aEntries[255].peBlue = 255;
		Palette.aEntries[255].peFlags = 0;

		//*** And the first is black
		//*** This may replace an entry in the array!
		Palette.aEntries[0].peRed = 0;
		Palette.aEntries[0].peGreen = 0;
		Palette.aEntries[0].peBlue = 0;
		Palette.aEntries[0].peFlags = 0;

	}
	else
	//*** For SYSPAL_STATIC, get the twenty static colors into
	//*** the array, then fill in the empty spaces with the
	//*** given color table
	{
		int nStaticColors;
		int nUsableColors;

		//*** Get the static colors from the system palette
		nStaticColors = GetDeviceCaps(hDC, NUMCOLORS);
		GetSystemPaletteEntries(hDC, 0, MAXPALETTE, Palette.aEntries);

		//*** Set the peFlags of the lower static colors to zero
		nStaticColors = nStaticColors / 2;

		for (i=0; i<nStaticColors; i++)
			Palette.aEntries[i].peFlags = 0;

		//*** Fill in the entries from the given color table
		nUsableColors = nColors - nStaticColors;   
		for (; i<nUsableColors; i++)
		{
			Palette.aEntries[i].peRed = lprgbq[i].rgbRed;
			Palette.aEntries[i].peGreen = lprgbq[i].rgbGreen;
			Palette.aEntries[i].peBlue = lprgbq[i].rgbBlue;
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE;
		}

		//*** Mark any empty entries as PC_NOCOLLAPSE
        for (; i<MAXPALETTE - nStaticColors; i++)
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE;

		//*** Set the peFlags of the upper static colors to zero
		for (i = MAXPALETTE - nStaticColors; i<MAXPALETTE; i++)
			Palette.aEntries[i].peFlags = 0;
	}

	//*** Remember to release the DC!
	ReleaseDC(NULL, hDC);

	//*** Return the palette
	// return CreatePalette((LPLOGPALETTE)&Palette);
	return VPalettes::createPalette(LPLOGPALETTE(&Palette));
}

/******************************************************************************
 *                                                                            *
 *  void ClearSystemPalette(void)                                             *
 *                                                                            *
 * Purpose:  Clears the system palette by realizing an all black palette.     *
 *           This will ensure that palette-managed applications executed      *
 *           before your application will not affect the identity mapping of  *
 *           your palette.                                                    * 
 *                                                                            *
 *                                                                            *
 * *Note: This code was originally taken from the WinG online docs            *
 *****************************************************************************/
static void ClearSystemPalette(void)
{
	//*** A dummy palette setup
	struct
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[MAXPALETTE];
	} Palette =
	{
		0x300, 
		MAXPALETTE
	};

	HPALETTE ScreenPalette = 0;
	HDC ScreenDC;
	int Counter;

	//*** Reset everything in the system palette to black
	for (Counter = 0; Counter < MAXPALETTE; Counter++)
	{
		Palette.aEntries[Counter].peRed = 0;
		Palette.aEntries[Counter].peGreen = 0;
		Palette.aEntries[Counter].peBlue = 0;
		Palette.aEntries[Counter].peFlags = PC_NOCOLLAPSE;
	}

	//*** Create, select, realize, deselect, and delete the palette
	ScreenDC = GetDC(NULL);
	//ScreenPalette = CreatePalette((LPLOGPALETTE)&Palette);
	ScreenPalette = VPalettes::createPalette(LPLOGPALETTE(&Palette));
	if (ScreenPalette)
	{
		ScreenPalette = SelectPalette(ScreenDC, ScreenPalette, FALSE);
		RealizePalette(ScreenDC);
		ScreenPalette = SelectPalette(ScreenDC, ScreenPalette, FALSE);
		// DeleteObject(ScreenPalette);
		VPalettes::deletePalette(ScreenPalette);
	}
	ReleaseDC(NULL, ScreenDC);
}

// ==========================================================================
// ::: DIB Implementation (internal)
// ==========================================================================

/*******************************************************************************
 *                                                                             *
 *  MODULE      : DIB.C                                                        *
 *                                                                             *
 *  DESCRIPTION : Routines for dealing with both Device Independent Bitmaps    *
 *                                                                             *
 *                                                                             *
 *  FUNCTIONS:                                                                 * 
 *                                                                             *
 *    OpenDIB()           - Opens DIB file and creates a memory DIB            *
 *                                                                             *
 *    WriteDIB()          - Writes a global handle in CF_DIB format to a file. *                           
 *                                                                             *
 *    WriteDIBEx()        - Writes a global handle in CF_DIB format to a file. *                           
 *                                                                             *
 *    DIBInfo()           - Retrieves the info. block associated with a CF_DIB *
 *                          format memory block.                               *
 *                                                                             *
 *    CreateBIPalette()   - Creates a GDI palette given a pointer to a         *
 *                          BITMAPINFO structure.                              *
 *                                                                             *
 *    CreateDIBPalette()  - Creates a GDI palette given a HANDLE to a          *
 *                          BITMAPINFO structure.                              *
 *                                                                             *
 *    ReadDIBBitmapInfo() - Reads a file in DIB format and returns a global    *
 *                          handle to it's BITMAPINFO                          *
 *                                                                             *
 *    ColorTableSize()    - Calculates the size in bytes of the color table    *
 *                          for the given DIB                                  *
 *                                                                             *
 *    DIBNumColors()      - Determines the number of colors in DIB.            *
 *                                                                             *
 *    BitmapFromDIB()     - Creates a DDB given a global handle to a block in  *
 *                          CF_DIB format.                                     *
 *                                                                             *
 *    DIBFromBitmap()     - Creates a DIB out of a DDB                         *
 *                                                                             *
 *    DrawBitmap()        - Draws a bitmap at a specified position in a DC.    *
 *                                                                             *
 *    DIBBlt()            - Draws a DIB at a specified position in a DC.       *
 *                                                                             *
 *    StretchDIBBlt()     - Draws DIBbitmap in CIF_DIB format using            *
 *                          StretchDIBits()                                    *
 *                                                                             *
 *    ChangeDIBFormat()   - Convert a DIB to another compression and bits per  *
 *                          pixel format.                                      *
 *                                                                             *
 *    ChangeDIBPalette()  - Map the colors in the given DIB to a new palette   *
 *                                                                             *
 *    CopyDIB()           - Make a copy of a DIB.                              *  
 *                                                                             *
 *    CopyBitmap()        - Make a copy of a bitmap.                           *
 *                                                                             *
 *    CropBitmap()        - Crops a bitmap to a new size specified by the      *
 *                          lpRect parameter.                                  *
 *                                                                             *
 *    CopyDIBData()       - Copy the data from one DIB to another.             *
 *                                                                             *
 *    GetDIBResolution()  - Returns the width and the height of a DIB as       *
 *                          specified in the BITMAPINFOHEADER                  *
 *                                                                             *
 *    GetDIBColorUsed()   - Returns the number of colors used by a DIB as      *
 *                          specified in the BITMAPINFOHEADER                  *
 *                                                                             *
 *    GetDIBCopmression() - Returns the compression used by a DIB as specified *
 *                          in the BITMAPINFOHEADER                            *
 *                                                                             *
 *    GetDIBBitcount()    - Returns the number of bits per pixel as specified  *
 *                          in the BITMAPINFOHEADER                            * 
 *                                                                             *
 *    CreateDIBPaletteEx()- Create a palette for a DIB and set the palette     *
 *                          entry flags to a specified value.                  *
 *                                                                             *
 *    SetOptimizedPaletteState() - Use an optimized palette (rather than a     *
 *                          spectrum palette) when loading (16/24/32bpp) DIBs  *
 *                                                                             *
 *    GetOptimizedPaletteState() - Returns true if bitmaps will be loaded      *
 *                          using an optimized palette.                        *
 *                                                                             *
 ******************************************************************************/
 
static BOOL bUseOptimizedPalette = FALSE;

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :CountBits(DWORD dw)                                           *
 *                                                                            *
 *  PURPOSE    :Count the number of set bits in a given DWORD                 * 
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette() for extracting RGB values      *
 *              from 16 and 32bpp DIBS                                        *
 *                                                                            *
 *****************************************************************************/
static BYTE CountBits(DWORD dw)
{
  int i, Count = 0;

  for (i=0; i<sizeof(DWORD) * 8; i++)
    Count += (((1 << i) & dw) != 0);

  return Count;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :RightmostBit(DWORD dw)                                        *
 *                                                                            *
 *  PURPOSE    :Returns the position of the rightmost set bit in a DWORD      * 
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette() for extracting RGB values      *
 *              from 16 and 32bpp DIBS                                        *
 *                                                                            *
 *****************************************************************************/
static BYTE RightmostBit(DWORD dw)
{
  int i;

  for (i=0; i<sizeof(DWORD) * 8; i++)
    if (((1 << i) & dw) != 0)
      return i;

  return (BYTE)-1; // No bits are set
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :LeftmostBit(DWORD dw)                                         *
 *                                                                            *
 *  PURPOSE    :Returns the position of the leftmost set bit in a DWORD       * 
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette() for extracting RGB values      *
 *              from 16 and 32bpp DIBS                                        *
 *                                                                            *
 *****************************************************************************/
static BYTE LeftmostBit(DWORD dw)
{
  int i;

  for (i=(sizeof(DWORD) * 8)-1; i > 0; i--)
    if (((1 << i) & dw) != 0)
      return i;

  return (BYTE)-1; // No bits are set
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :ValidMask(DWORD dw)                                           *
 *                                                                            *
 *  PURPOSE    :Determines if a given DWORD is valid as a color mask for      * 
 *              16 or 32bpp DIBS (are all the set bits contiguous)            *
 *                                                                            *
 *  *NOTE:     :Used only in CreateBIPalette()                                *
 *                                                                            *
 *****************************************************************************/
static BOOL ValidMask(DWORD dw)
{
  int iStart, iStop, i;
  
  iStart = LeftmostBit(dw);
  iStop  = RightmostBit(dw);

  if (iStart == iStop)
    return TRUE;

  for (i=iStart; i>=iStop; i--) 
     if (((1 << i) & dw) == 0)
       return FALSE;

  return TRUE;
}
 
/******************************************************************************
 *                                                                            *
 *  FUNCTION   :OpenDIB(LPSTR szFilename)                                     *
 *                                                                            *
 *  PURPOSE    :Open a DIB file and create a memory DIB -- a memory handle    *
 *              containing BITMAPINFO, palette data and the bits.             * 
 *                                                                            *
 *  RETURNS    :A handle to the DIB.                                          *
 *                                                                            *
 *****************************************************************************/
static HANDLE OpenDIB (FileOrArchive& foa)
{
    BITMAPINFOHEADER    bih;
    LPBITMAPINFOHEADER  lpbih;
    DWORD               dwLen = 0;
    DWORD               dwBits;
    HANDLE              hDIB;
    HANDLE              hMem;

    /* Open the file and read the DIB information */
	if (!foa.openForRead()) {
		return NULL;
	}

    hDIB = ReadDIBBitmapInfo(foa);
    if (!hDIB)
        return NULL;
    DIBInfo(hDIB, &bih);

    /* Calculate the memory needed to hold the DIB */
    dwBits = bih.biSizeImage;
    dwLen  = bih.biSize + (DWORD)ColorTableSize (&bih) + dwBits;

    /* Try to increase the size of the bitmap info. buffer to hold the DIB */
    hMem = GlobalReAlloc(hDIB, dwLen, GHND);

    if (!hMem){
        GlobalFree(hDIB);
        hDIB = NULL;
    }
    else
        hDIB = hMem;

    /* Read in the bits */
    if (hDIB){
        lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
		foa.read(LPSTR(lpbih) +
				 WORD(lpbih->biSize) + ColorTableSize(lpbih), dwBits);
        GlobalUnlock(hDIB);
    }
	foa.close();

    return hDIB;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : WriteDIB(LPSTR szFilename, HANDLE hDIB)                      *
 *                                                                            *
 *  PURPOSE    : Write a global handle in CF_DIB format to a file.            *
 *                                                                            *
 *  RETURNS    : TRUE  - if successful.                                       *
 *               FALSE - otherwise                                            *
 *                                                                            *
 *****************************************************************************/
static BOOL WriteDIB (FileOrArchive& foa, HANDLE hDIB)
{
    BITMAPFILEHEADER    bmfhdr;
    LPBITMAPINFOHEADER  lpbih;

    if (!hDIB)
        return FALSE;

	if (!foa.openForWrite()) {
		return FALSE;
	}

    lpbih = (LPBITMAPINFOHEADER)GlobalLock (hDIB);

    /* Fill in the fields of the file header */
    bmfhdr.bfType          = BFT_BITMAP;
    bmfhdr.bfSize          = GlobalSize (hDIB) + SIZEOF_BITMAPFILEHEADER_PACKED;
    bmfhdr.bfReserved1     = 0;
    bmfhdr.bfReserved2     = 0;
    bmfhdr.bfOffBits       = (DWORD)(SIZEOF_BITMAPFILEHEADER_PACKED + 
                                  lpbih->biSize + ColorTableSize(lpbih));

    WritePackedFileHeader(foa, &bmfhdr);

    /* Write the DIB header and the bits */
	foa.write(LPSTR(lpbih), GlobalSize(hDIB));

    GlobalUnlock (hDIB);
	foa.close();
 
    return TRUE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION  : WriteDIBEx(LPSTR szFilename, HANDLE hDIB,                     *
 *                         WORD wBPP, DWORD dwComp)                           *                           
 *                                                                            *
 *  PURPOSE   : Write a global handle in CF_DIB format to a file with a       *
 *              specified pixel depth and compression format                  *
 *                                                                            *
 *  RETURNS   : TRUE  - if successful.                                        *
 *              FALSE - otherwise                                             *
 *                                                                            *
 *****************************************************************************/
static BOOL WriteDIBEx(FileOrArchive& foa, HANDLE hDIB, WORD wBPP, DWORD dwComp)
{
    BITMAPINFOHEADER bih;
    HANDLE hNewDIB;
    BOOL bReturn;

    DIBInfo(hDIB, &bih);

    if (((bih.biBitCount == wBPP) && (dwComp == bih.biCompression)) || ((wBPP == 0) && (dwComp == 0)))
       return WriteDIB(foa, hDIB);

#ifndef CV_NO_CHANGE_DIB_FORMAT
    hNewDIB = CopyDIB(hDIB);
    hDIB = ChangeDIBFormat(hNewDIB, wBPP, dwComp);
    if (hDIB) {
      bReturn = WriteDIB(foa, hDIB);
      GlobalFree(hDIB);
    } else {
      bReturn = FALSE;
      GlobalFree(hNewDIB);
    }
#else
      bReturn = WriteDIB(foa, hDIB);
#endif

    return bReturn;
 }

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DIBInfo(HANDLE hbi, LPBITMAPINFOHEADER lpbih)                *
 *                                                                            *
 *  PURPOSE    : Retrieves the DIB info associated with a CF_DIB              *
 *               format memory block.                                         *
 *                                                                            *
 *  RETURNS    : TRUE  - if successful.                                       *
 *               FALSE - otherwise                                            *
 *                                                                            *
 *****************************************************************************/
static BOOL DIBInfo (HANDLE hbi, LPBITMAPINFOHEADER lpbih)
{
    if (hbi){
      *lpbih = *(LPBITMAPINFOHEADER)GlobalLock (hbi);

      /* fill in the default fields */
      if (NEW_DIB_FORMAT(lpbih)) {
        if (lpbih->biSizeImage == 0L)
          lpbih->biSizeImage = WIDTHBYTES(lpbih->biWidth*lpbih->biBitCount) * lpbih->biHeight;

        if (lpbih->biClrUsed == 0L)
          lpbih->biClrUsed = DIBNumColors (lpbih);
      }

      GlobalUnlock (hbi);
      return TRUE;
    }
    return FALSE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : CreateBIPalette(LPBITMAPINFOHEADER lpbih)                    *
 *                                                                            *
 *  PURPOSE    : Given a Pointer to a BITMAPINFO struct will create a         *
 *               a GDI palette object from the color table.                   *
 *                                                                            *
 *  RETURNS    : A handle to the palette.                                     *
 *                                                                            *
 *****************************************************************************/
static HPALETTE CreateBIPalette (LPBITMAPINFOHEADER lpbih)
{
    HPALETTE       hPal = NULL;
    WORD           nNumColors;
    LPRGBQUAD      lprgbq;

    if (!lpbih)
        return NULL;

    if (!NEW_DIB_FORMAT(lpbih))  // Return if we dont have a BITMAPINFOHEADER
        return NULL;

    /* Get a pointer to the color table and the number of colors in it */
    lprgbq = (LPRGBQUAD)((LPSTR)lpbih + (WORD)lpbih->biSize);
    /* Adjust pointer for cases where BITFIELDS bitmaps might contain an optimal palette */
    if (lpbih->biCompression == BI_BITFIELDS)
      lprgbq = (LPRGBQUAD)((LPDWORD)lprgbq + 3); 
    nNumColors = DIBNumColors(lpbih);
	lpbih->biClrUsed = nNumColors;

    if (nNumColors)   /* Create a palette from the entries in the DIBs color table */
        hPal = CreatePaletteFromRGBQUAD(lprgbq, nNumColors);
    else
      if (!bUseOptimizedPalette) /* Create a halftone palette */
        hPal = CreateSpectrumPalette();
#ifndef	CV_NO_OPTIMIZE_PALETTE
      else
        if (lpbih->biBitCount > 8)  /* Create a "most frequently used colors" palette */
            switch (lpbih->biBitCount) {
              case 24: 
                { 
        		  HANDLE    hColorMap = OpenColorMapArray();
        		  RGBQUAD   rgbq[MAXPALETTE];
        		  WORD      dwX, dwY;
        		  RGBTRIPLE *rgbt = (RGBTRIPLE *)((LPSTR)lpbih + (WORD)lpbih->biSize + ColorTableSize(lpbih));
                  DWORD     dwRGBAdd = (DWORD)BYTESPERLINE(lpbih->biWidth, 24);

        		  if (hColorMap == 0) { // If we couldnt get a handle to a colormap...
                    hPal = CreateSpectrumPalette();
                    break;
                  }
        		  
        		  // Walk the bits and add all the RGB values to our color histogram
        		  for (dwY = 0; dwY < lpbih->biHeight; dwY++) {
                    for (dwX = 0; dwX < lpbih->biWidth; dwX++) 
                       AddColorToArray(hColorMap, &rgbt[dwX]);
                    rgbt = (RGBTRIPLE*)((LPSTR)rgbt + dwRGBAdd);
                  }

        		  // Create a palette from the most frequently used colors
        		  GetOptimizedPalette(hColorMap, (LPRGBQUAD)&rgbq, MAXPALETTE);
                  hPal = CreatePaletteFromRGBQUAD((LPRGBQUAD)&rgbq, MAXPALETTE);
        
                  CloseColorMapArray(hColorMap);
                };
                break;
          
              case 16:
              case 32: 
                { 
        		  HANDLE    hColorMap = OpenColorMapArray();
        		  RGBQUAD   rgbq[MAXPALETTE];
        		  DWORD     dwInd, dwMax;
                  RGBTRIPLE rgbt;
        		  DWORD     dwWidth;
                  LPDWORD   lpMasks = (LPDWORD)((LPSTR)lpbih + (WORD)lpbih->biSize);
                  LPDWORD   lpBits32 = (LPDWORD)((LPSTR)lpbih + ColorTableSize(lpbih));
                  LPWORD    lpBits16 = (LPWORD)lpBits32;
                  BYTE      bLRed, bLGreen, bLBlue; 
                  BYTE      bRRed, bRGreen, bRBlue; 
                  DWORD     dwRedMask, dwGreenMask, dwBlueMask;
                  
                  if (hColorMap == 0) { // If we couldnt get a handle to a colormap...
                    hPal = CreateSpectrumPalette();
                    break;
                  }
                                          
                  // If not using BI_BITFIELDS then set masks to default values
                  if (lpbih->biCompression == BI_RGB) {
                    if (lpbih->biBitCount == 16) {
                      dwRedMask   = MAKE555WORD(0xFF, 0, 0);   // Make 5, 5, 5 mask
                      dwGreenMask = MAKE555WORD(0, 0xFF, 0);
                      dwBlueMask  = MAKE555WORD(0, 0, 0xFF);
                    } else {
                      dwRedMask   = 0xFF0000;                  // Make 8, 8, 8 mask
                      dwGreenMask = 0x00FF00;
                      dwBlueMask  = 0x0000FF;
                    } 
                  } else {
                    dwRedMask   = lpMasks[0];                  // Use the specified mask
                    dwGreenMask = lpMasks[1];
                    dwBlueMask  = lpMasks[2];
                  }
                                                                                                           
                  // Do a quick check to see if the masks look reasonable (contiguous bits)
                  if (!ValidMask(dwRedMask) || !ValidMask(dwGreenMask) || !ValidMask(dwBlueMask)) {
                    MessageBox(NULL, "These DWORD masks look strange!\n\nI'm going to substitute a halftone palette.", "Problem?", MB_ICONINFORMATION);
                    hPal = CreateSpectrumPalette();
                    break;
                  }

        		  dwWidth = lpbih->biWidth;
        		  if (lpbih->biBitCount == 16)  // DWORD align for 16bpp
                    dwWidth += dwWidth & 1;
                
        		  dwMax = dwWidth * lpbih->biHeight;  
              
                  // While more that 8 bits per mask, turn off the righmost bit 
                  while (CountBits(dwRedMask) > 8) dwRedMask -= (1 << RightmostBit(dwRedMask));
                  while (CountBits(dwGreenMask) > 8) dwGreenMask -= (1 << RightmostBit(dwGreenMask));
                  while (CountBits(dwBlueMask) > 8) dwBlueMask -= (1 << RightmostBit(dwBlueMask));

                  // How much will we need to shift right in order to "right justify" the bits
                  bRRed   = RightmostBit(dwRedMask);
                  bRGreen = RightmostBit(dwGreenMask);
                  bRBlue  = RightmostBit(dwBlueMask);

                  // How much will we need to shift left in order to pad to 8 bits per mask
                  bLRed   = 8 - CountBits(dwRedMask);  
                  bLGreen = 8 - CountBits(dwGreenMask);
                  bLBlue  = 8 - CountBits(dwBlueMask); 
           
        		  // Walk the bits and add the extracted RGB values to our color histogram
        		  if (lpbih->biBitCount == 16)
        		    for (dwInd = 0; dwInd < dwMax; dwInd++) {
                       rgbt.rgbtRed   = (BYTE)((lpBits16[dwInd] & dwRedMask)   >> bRRed)   << bLRed;
                       rgbt.rgbtGreen = (BYTE)((lpBits16[dwInd] & dwGreenMask) >> bRGreen) << bLGreen;
                       rgbt.rgbtBlue  = (BYTE)((lpBits16[dwInd] & dwBlueMask)  >> bRBlue)  << bLBlue;
    		           AddColorToArray(hColorMap, &rgbt);
                    } 
                  else
                    for (dwInd = 0; dwInd < dwMax; dwInd++) {
                       rgbt.rgbtRed   = (BYTE)((lpBits32[dwInd] & dwRedMask)   >> bRRed)   << bLRed;
                       rgbt.rgbtGreen = (BYTE)((lpBits32[dwInd] & dwGreenMask) >> bRGreen) << bLGreen;
                       rgbt.rgbtBlue  = (BYTE)((lpBits32[dwInd] & dwBlueMask)  >> bRBlue)  << bLBlue;
    		           AddColorToArray(hColorMap, &rgbt);
                    } 
		  
        		  // Get the most frequently used colors and create a palette out of them
        		  GetOptimizedPalette(hColorMap, (LPRGBQUAD)&rgbq, MAXPALETTE);
                  hPal = CreatePaletteFromRGBQUAD((LPRGBQUAD)&rgbq, MAXPALETTE);
        
        	      CloseColorMapArray(hColorMap);
                };
                break;
            }
#endif

    return hPal;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : CreateDIBPalette(HANDLE hbi)                                 *
 *                                                                            *
 *  PURPOSE    : Given a Global HANDLE to a BITMAPINFO Struct will create a   *
 *               GDI palette object from the color table. (BITMAPINFOHEADER   *
 *               format DIBs only)                                            *
 *                                                                            *
 *  RETURNS    : A handle to the palette.                                     *
 *                                                                            *
 *****************************************************************************/
static HPALETTE CreateDIBPalette (HANDLE hbi)
{
    HPALETTE hPal;

    if (!hbi)
        return NULL;

    hPal = CreateBIPalette((LPBITMAPINFOHEADER)GlobalLock(hbi));
    GlobalUnlock(hbi);

    return hPal;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : ReadDIBBitmapInfo(int hFile)                                 *
 *                                                                            *
 *  PURPOSE    : Will read a file in DIB format and return a global HANDLE    *
 *               to it's BITMAPINFO.  This function will work with both       *
 *               "old" (BITMAPCOREHEADER) and "new" (BITMAPINFOHEADER)        *
 *               bitmap formats, but will always return a "new" BITMAPINFO    *
 *                                                                            *
 *  RETURNS    : A handle to the BITMAPINFO of the DIB in the file.           *
 *                                                                            *
 *****************************************************************************/
static HANDLE ReadDIBBitmapInfo (FileOrArchive& foa)
{
    DWORD              dwOffset;
    HANDLE             hbi = NULL;
    INT                size;
    INT                i;
    WORD               nNumColors;
    LPRGBQUAD          lprgbq;
    BITMAPINFOHEADER   bih;
    BITMAPCOREHEADER   bch;
    LPBITMAPINFOHEADER lpbih;
    BITMAPFILEHEADER   bf;
    DWORD              dwDWMasks= 0;
    DWORD              dwWidth = 0;
    DWORD              dwHeight = 0;
    WORD               wPlanes, wBitCount;

	if (!foa.openForRead()) {
		return NULL;
	}

	//
	// Read the bitmap file header.
	//
    // ReadPackedFileHeader(hFile, &bf, &dwOffset);
	//
 	// Read file header (which is packed) and convert
	// into unpacked BITMAPFILEHEADER structure.
	//
    dwOffset = foa.getPosition();
	foa.read(LPSTR(&bf.bfType), sizeof(WORD)); // read in bfType
    foa.read(LPSTR(&bf.bfSize), sizeof(DWORD) * 3); // read last 3 dwords

    /* Do we have a RC HEADER? */
    if (!ISDIB (bf.bfType)) {    
      bf.bfOffBits = 0L;               
        foa.setPosition(dwOffset); /* seek back to beginning of file */
    }

	if (!foa.read(LPSTR(&bih), sizeof(bih))) {
		return FALSE;
	}

    nNumColors = DIBNumColors (&bih);

    /* Check the nature (BITMAPINFO or BITMAPCORE) of the info. block
     * and extract the field information accordingly. If a BITMAPCOREHEADER, 
     * transfer it's field information to a BITMAPINFOHEADER-style block
     */
    switch (size = (INT)bih.biSize){
        case sizeof (BITMAPINFOHEADER):
            break;

        case sizeof (BITMAPCOREHEADER):

            bch = *(LPBITMAPCOREHEADER)&bih;

            dwWidth   = (DWORD)bch.bcWidth;
            dwHeight  = (DWORD)bch.bcHeight;
            wPlanes   = bch.bcPlanes;
            wBitCount = bch.bcBitCount;

            bih.biSize           = sizeof(BITMAPINFOHEADER);
            bih.biWidth          = dwWidth;
            bih.biHeight         = dwHeight;
            bih.biPlanes         = wPlanes;
            bih.biBitCount       = wBitCount;
            bih.biCompression    = BI_RGB;
            bih.biSizeImage      = 0;
            bih.biXPelsPerMeter  = 0;
            bih.biYPelsPerMeter  = 0;
            bih.biClrUsed        = nNumColors;
            bih.biClrImportant   = nNumColors;

			foa.shiftPosition(sizeof(BITMAPCOREHEADER) -
							  sizeof(BITMAPINFOHEADER));
            break;

        default:
            /* Not a DIB! */
            return NULL;
    }

    /*  Fill in some default values if they are zero */
    if (bih.biSizeImage == 0){
        bih.biSizeImage = WIDTHBYTES((DWORD)bih.biWidth * bih.biBitCount) * bih.biHeight;
    }
    if (bih.biClrUsed == 0)
        bih.biClrUsed = DIBNumColors(&bih);

    /* Allocate for the BITMAPINFO structure and the color table. */
    if ((bih.biBitCount == 16) || (bih.biBitCount == 32))
      dwDWMasks = sizeof(DWORD) * 3;
    hbi = GlobalAlloc (GHND, (LONG)bih.biSize + nNumColors * sizeof(RGBQUAD) + dwDWMasks);
    if (!hbi)
        return NULL;
    lpbih = (LPBITMAPINFOHEADER)GlobalLock (hbi);
    *lpbih = bih;

    /* Get a pointer to the color table */
    lprgbq = (LPRGBQUAD)((LPSTR)lpbih + bih.biSize);
    if (nNumColors){
        if (size == sizeof(BITMAPCOREHEADER)){
            /* Convert a old color table (3 byte RGBTRIPLEs) to a new
             * color table (4 byte RGBQUADs)
             */
			foa.read(LPSTR(lprgbq), nNumColors * sizeof(RGBTRIPLE));

            for (i = nNumColors - 1; i >= 0; i--){
                RGBQUAD rgbq;

                rgbq.rgbRed      = ((RGBTRIPLE*)lprgbq)[i].rgbtRed;
                rgbq.rgbBlue     = ((RGBTRIPLE*)lprgbq)[i].rgbtBlue;
                rgbq.rgbGreen    = ((RGBTRIPLE*)lprgbq)[i].rgbtGreen;
                rgbq.rgbReserved = (BYTE)0;

                lprgbq[i] = rgbq;
            }
        }
        else
			foa.read(LPSTR(lprgbq), nNumColors * sizeof(RGBQUAD));
    } else
        if (dwDWMasks)
		   foa.read(LPSTR(lprgbq), dwDWMasks);

    if (!foa.isArchive() && (bf.bfOffBits != 0L)) {
		foa.setPosition(dwOffset + bf.bfOffBits);
	}
    GlobalUnlock(hbi);
    return hbi;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   :  ColorTableSize(LPVOID lpv)                                  *
 *                                                                            *
 *  PURPOSE    :  Calculates the palette size in bytes. If the info. block    *
 *                is of the BITMAPCOREHEADER type, the number of colors is    *
 *                multiplied by 3 to give the palette size, otherwise the     *
 *                number of colors is multiplied by 4.                        *
 *                                                                            *
 *  RETURNS    :  Color table size in number of bytes.                        *
 *                                                                            *
 *****************************************************************************/
static WORD ColorTableSize (LPVOID lpv)
{
    LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)lpv;
    
    if (NEW_DIB_FORMAT(lpbih))
    {
      if (((LPBITMAPINFOHEADER)(lpbih))->biCompression == BI_BITFIELDS)
         /* Remember that 16/32bpp dibs can still have a color table */
         return (sizeof(DWORD) * 3) + (DIBNumColors (lpbih) * sizeof (RGBQUAD));
      else
         return (DIBNumColors (lpbih) * sizeof (RGBQUAD));
    }
    else
      return (DIBNumColors (lpbih) * sizeof (RGBTRIPLE));
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DIBNumColors(LPVOID lpv)                                     *
 *                                                                            *
 *  PURPOSE    : Determines the number of colors in the DIB by looking at     *
 *               the BitCount and ClrUsed fields in the info block.           *
 *                                                                            *
 *  RETURNS    : The number of colors in the DIB. With DIBS with more than    *
 *               8-bits-per-pixel that have a color table table included,     *
 *               then the return value will be the number of colors in the    *
 *               color table rather than the number of colors in the DIB.     *
 *                                                                            *
 *                                                                            *
 *****************************************************************************/
static WORD DIBNumColors (LPVOID lpv)
{
    INT                 bits;
    LPBITMAPINFOHEADER  lpbih = (LPBITMAPINFOHEADER)lpv;
    LPBITMAPCOREHEADER  lpbch = (LPBITMAPCOREHEADER)lpv;

    /*  With the BITMAPINFO format headers, the size of the palette
     *  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
     *  is dependent on the bits per pixel ( = 2 raised to the power of
     *  bits/pixel).
     */
    if (NEW_DIB_FORMAT(lpbih)) {
      if (lpbih->biClrUsed != 0)
        return (WORD)lpbih->biClrUsed;
      bits = lpbih->biBitCount;
    }
    else
      bits = lpbch->bcBitCount;

    if (bits > 8) 
      return 0; /* Since biClrUsed is 0, we dont have a an optimal palette */
    else
      return (1 << bits); 
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DIBFromBitmap()                                              *
 *                                                                            *
 *  PURPOSE    : Will create a global memory block in DIB format that         *
 *               represents the Device-dependent bitmap (DDB) passed in.      *
 *                                                                            *
 *  RETURNS    : A handle to the DIB                                          *
 *                                                                            *
 *****************************************************************************/
static HANDLE DIBFromBitmap (HBITMAP hBitmap, DWORD biStyle, WORD biBits, HPALETTE hPal)
{
    BITMAP               Bitmap;
    BITMAPINFOHEADER     bih;
    LPBITMAPINFOHEADER   lpbih;
    DWORD                dwLen;
    HANDLE               hDIB;
    HANDLE               hMem;
    HDC                  hDC;

    if (!hBitmap)
        return NULL;

    if (hPal == NULL)
        hPal = HPALETTE(GetStockObject(DEFAULT_PALETTE));

    GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);

    if (biBits == 0)
        biBits =  Bitmap.bmPlanes * Bitmap.bmBitsPixel;

    bih.biSize            = sizeof(BITMAPINFOHEADER);
    bih.biWidth           = Bitmap.bmWidth;
    bih.biHeight          = Bitmap.bmHeight;
    bih.biPlanes          = 1;
    bih.biBitCount        = biBits;
    bih.biCompression     = biStyle;
    bih.biSizeImage       = 0;
    bih.biXPelsPerMeter   = 0;
    bih.biYPelsPerMeter   = 0;
    bih.biClrUsed         = 0;
    bih.biClrImportant    = 0;

    dwLen  = bih.biSize + ColorTableSize(&bih);

    hDC = GetDC(NULL);
    hPal = SelectPalette(hDC, hPal, FALSE);
    RealizePalette(hDC);

    hDIB = GlobalAlloc(GHND, dwLen);

    if (!hDIB){
        SelectPalette(hDC, hPal, FALSE);
        ReleaseDC(NULL, hDC);
        return NULL;
    }

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

    *lpbih = bih;

    /*  call GetDIBits with a NULL lpBits param, so it will calculate the
     *  biSizeImage field for us
     */
    GetDIBits(hDC, 
              hBitmap, 
              (UINT)0, 
              (UINT)bih.biHeight, 
              (LPVOID)NULL, 
              (LPBITMAPINFO)lpbih, 
              DIB_RGB_COLORS);

    bih = *lpbih;
    GlobalUnlock(hDIB);

    /* If the driver did not fill in the biSizeImage field, make one up */
    if (bih.biSizeImage == 0){
        bih.biSizeImage = WIDTHBYTES((DWORD)Bitmap.bmWidth * biBits) * Bitmap.bmHeight;

        if (biStyle != BI_RGB)
            bih.biSizeImage = (bih.biSizeImage * 3) / 2;
    }

    /*  realloc the buffer big enough to hold all the bits */
    dwLen = bih.biSize + ColorTableSize(&bih) + bih.biSizeImage;
    if (hMem = (HANDLE)GlobalReAlloc(hDIB, dwLen, 0))
        hDIB = hMem;
    else{
        GlobalFree(hDIB);
        hDIB = NULL;

        SelectPalette(hDC, hPal, FALSE);
        ReleaseDC(NULL, hDC);
        return hDIB;
    }

    /*  call GetDIBits with a NON-NULL lpBits param, and actualy get the
     *  bits this time
     */
    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

    if (!GetDIBits(hDC, 
                   hBitmap, 
                   (UINT)0, 
                   (UINT)bih.biHeight, 
                   (LPBYTE)lpbih + (WORD)lpbih->biSize + ColorTableSize(lpbih), 
                   (LPBITMAPINFO)lpbih, 
                   DIB_RGB_COLORS)) {
         GlobalUnlock(hDIB);
         hDIB = (HANDLE)NULL;
         SelectPalette(hDC, hPal, FALSE);
         ReleaseDC(NULL, hDC);
         return NULL;
    }

    bih = *lpbih;
    GlobalUnlock(hDIB);

    SelectPalette(hDC, hPal, FALSE);
    ReleaseDC(NULL, hDC);

    return hDIB;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : BitmapFromDIB(HANDLE hDIB, HPALETTE hPal)                    *
 *                                                                            *
 *  PURPOSE    : Will create a DDB (Device Dependent Bitmap) given a global   *
 *               handle to a memory block in CF_DIB format                    *
 *                                                                            *
 *  RETURNS    : A handle to the DDB.                                         *
 *                                                                            *
 *****************************************************************************/
static HBITMAP BitmapFromDIB (HANDLE hDIB, HPALETTE  hPal)
{
    LPBITMAPINFOHEADER  lpbih;
    HPALETTE            hPalOld;
    HDC                 hDC;
    HBITMAP             hBitmap;  

    if (!hDIB)
        return NULL;

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

    if (!lpbih)
        return NULL;

    hDC = GetDC(NULL);

    if (hPal){
        hPalOld = SelectPalette(hDC, hPal, FALSE);
        RealizePalette(hDC);     // GDI Bug...????
    }                              
   
    hBitmap = CreateDIBitmap(hDC, 
                lpbih, 
                CBM_INIT, 
                (LPSTR)lpbih + lpbih->biSize + ColorTableSize(lpbih), 
                (LPBITMAPINFO)lpbih, 
                DIB_RGB_COLORS ); 

    if (hPal)
        SelectPalette(hDC, hPalOld, FALSE);

    ReleaseDC(NULL, hDC);
    GlobalUnlock(hDIB);

    return hBitmap;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DrawBitmap(HDC hDC, int x, int y,                            *
 *                          HBITMAP hBitmap, DWORD dwROP)                     * 
 *                                                                            *
 *  PURPOSE    : Draws bitmap <hBitmap> at the specifed position in DC <hDC>  *
 *                                                                            *
 *  RETURNS    : Return value of BitBlt()                                     *
 *                                                                            *
 *****************************************************************************/
static BOOL DrawBitmap (HDC hDC, INT x, INT y, HBITMAP hBitmap, DWORD dwROP)
{
    HDC       hDCBits;
    BITMAP    Bitmap;
    BOOL      bResult;

    if (!hDC || !hBitmap)
        return FALSE;

    hDCBits = CreateCompatibleDC(hDC);
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    SelectObject(hDCBits, hBitmap);
    bResult = BitBlt(hDC, x, y, Bitmap.bmWidth, Bitmap.bmHeight, hDCBits, 0, 0, dwROP);
    DeleteDC(hDCBits);

    return bResult;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : DIBBlt( HDC hDC,                                             *
 *                       int x0, int y0,                                      *
 *                       int dx, int dy,                                      *
 *                       HANDLE hDIB,                                         *
 *                       int x1, int y1,                                      *
 *                       LONG dwROP)                                          *
 *                                                                            *
 *  PURPOSE    : Draws a bitmap in CF_DIB format, using SetDIBits to device.  *
 *               taking the same parameters as BitBlt().                      *
 *                                                                            *
 *  RETURNS    : TRUE  - if function succeeds.                                *
 *               FALSE - otherwise.                                           *
 *                                                                            *
 ******************************************************************************/
static BOOL DIBBlt (HDC hDC,     INT x0, INT y0, INT dx, INT dy, 
             HANDLE hDIB, INT x1, INT y1, LONG  dwROP)
{
    LPBITMAPINFOHEADER   lpbih;
    LPSTR                pBuf;

    if (!hDIB)
        return PatBlt(hDC, x0, y0, dx, dy, dwROP);

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

    if (!lpbih)
        return FALSE;

    pBuf = (LPSTR)lpbih + (WORD)lpbih->biSize + ColorTableSize(lpbih);
    SetDIBitsToDevice (hDC, x0, y0, dx, dy, 
                       x1, y1, 
                       x1, 
                       dy, 
                       pBuf, (LPBITMAPINFO)lpbih, 
                       DIB_RGB_COLORS );

    GlobalUnlock(hDIB);
    return TRUE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : StretchDIBBlt( HDC hDC,                                      *
 *                              int x, int y,                                 *
 *                              int dx, int dy,                               *
 *                              HANDLE hDIB,                                  *
 *                              int x0, int y0,                               *
 *                              int dx0, int dy0,                             *
 *                              LONG dwROP)                                   *
 *                                                                            *
 *  PURPOSE    : Draws a bitmap in CF_DIB format, using StretchDIBits()       *
 *               taking the same parameters as StretchBlt().                  *
 *                                                                            *
 *  RETURNS    : TRUE  - if function succeeds.                                *
 *               FALSE - otherwise.                                           *
 *                                                                            *
 ******************************************************************************/
static BOOL StretchDIBBlt (HDC hDC,     INT x,  INT y,  INT dx,  INT dy, 
                    HANDLE hDIB, INT x0, INT y0, INT dx0, INT dy0, LONG dwROP)

{
    LPBITMAPINFOHEADER lpbih;
    LPSTR        pBuf;
    BOOL         bResult;

    if (!hDIB)
        return PatBlt(hDC, x, y, dx, dy, dwROP);

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

    if (!lpbih)
        return FALSE;

    pBuf = (LPSTR)lpbih + (WORD)lpbih->biSize + ColorTableSize(lpbih);
    
    bResult = StretchDIBits (hDC, x,  y,  dx,  dy, 
                                  x0, y0, dx0, dy0, 
                             pBuf, (LPBITMAPINFO)lpbih, 
                             DIB_RGB_COLORS, dwROP);

    GlobalUnlock(hDIB);
    return bResult;
}
  
/******************************************************************************
 *                                                                            *
 *  FUNCTION   : WritePackedFileHeader(HFILE hFile, LPBITMAPFILEHEADER pbf)   *
 *                                                                            *
 *  PURPOSE    : write header structure (which NOT packed) and write          *
 *               it out in PACKED format                                      *
 *                                                                            *
 *  RETURNS    : VOID                                                         *
 *                                                                            *
 *****************************************************************************/
static VOID WritePackedFileHeader(FileOrArchive& foa, LPBITMAPFILEHEADER lpbmfhdr)
{
	foa.write(LPSTR(&lpbmfhdr->bfType), sizeof(WORD));

    /* pass over extra word and write next 3 DWORDS! */
    foa.write(LPSTR(&lpbmfhdr->bfSize), sizeof(DWORD) * 3);
}

/******************************************************************************
 *                                                                            *
 * HANDLE ConvertRGBDIB(HANDLE hSrcDIB, DWORD dwDstComp, WORD wDstBPP)        *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HDIB             - handle to packed-DIB in memory                          *
 * WORD             - desired bits per pixel                                  *
 * DWORD            - desired compression format                              *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HDIB             - handle to the new DIB if successful, else NULL          *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will create a new DIB with the specified BPP format using an *
 * existing DIB as the source of the bits.  The conversion method will keep   *
 * much of the color information as possible yet, because of the method, this *
 * function will only work if the compression (both source and dest) is       *
 * BI_RGB and the number of bits per pixel is at least 16.  This must be the  *
 * case because CreateDIBSection will not permit the use of DIBs with non     *
 * BI_RGB compression types.                                                  *
 *                                                                            *
 * *Notes:                                                                    *
 *    The source DIB is never destroyed                                       *
 *    This function is not exposed and it is only called by ChangeDIBFormat() *
 *    The function will return NULL if it fails                               *
 *                                                                            *
 *****************************************************************************/
static HANDLE ConvertRGBDIB(HANDLE hSrcDIB, DWORD dwDstComp, WORD wDstBPP)
{
#ifdef	CV_NO_CHANGE_DIB_FORMAT
	return 0;
#else
    HDC hDC = GetDC(NULL);  
   	HDC hSrcMemDC = CreateCompatibleDC(hDC);
    HDC hDstMemDC = CreateCompatibleDC(hDC);
    HBITMAP hSrcBmp = NULL, hDstBmp = NULL;
    HANDLE hDstDIB = NULL;
 	LPBITMAPINFOHEADER biSrcPtr, biDstPtr;
    LPRGBQUAD rgbqSrcPtr, rgbqDstPtr; 
    RGBTRIPLE *bmSrcPtr, *bmDstPtr;
    LPVOID pSrcDisp, pDstDisp;                                

    __try {
        /* This method of conversion only works with BI_RGB format DIBs */
        if ((dwDstComp != BI_RGB) || (GetDIBCompression(hSrcDIB) != BI_RGB) || 
            (wDstBPP < 16) || (GetDIBBitCount(hSrcDIB) < 16))
          RAISE_AN_EXCEPTION();

        /* Initialize pointers for the source DIB */
        if (!GetAndLockDIBPointers(HANDLE(hSrcDIB), (LPVOID *)&biSrcPtr, (LPVOID *)&rgbqSrcPtr, (LPVOID *)&bmSrcPtr))
          RAISE_AN_EXCEPTION();

        /* Create destination DIB and initialize pointers for it */
        hDstDIB = CreateRGBDIB(biSrcPtr->biWidth, biSrcPtr->biHeight, wDstBPP, dwDstComp);
        if (!GetAndLockDIBPointers(hDstDIB, (LPVOID *)&biDstPtr, (LPVOID *)&rgbqDstPtr, (LPVOID *)&bmDstPtr))
          RAISE_AN_EXCEPTION();
    
    	/* Create and load source DIB into source bitmap */
    	hSrcBmp = CreateDIBSection(hDC, (BITMAPINFO*)biSrcPtr, 0, &pSrcDisp, NULL, 0);
        if (!SelectObject(hSrcMemDC, hSrcBmp))
          RAISE_AN_EXCEPTION();
    	CopyMemory(pSrcDisp, bmSrcPtr, biSrcPtr->biSizeImage);

        /* Create destination bitmap */
        hDstBmp = CreateDIBSection(hDC, (BITMAPINFO*)biDstPtr, 0, &pDstDisp, NULL, 0);
        if (!SelectObject(hDstMemDC, hDstBmp))
          RAISE_AN_EXCEPTION();

        /* Make GDI do the conversion between the different BPP formats */
        BitBlt(hDstMemDC, 0, 0, biSrcPtr->biWidth, biSrcPtr->biHeight,
               hSrcMemDC, 0, 0, SRCCOPY);
        GdiFlush();
          
        /* Copy the converted data to our destination DIB */
        CopyMemory(bmDstPtr, pDstDisp, biDstPtr->biSizeImage);
    } __except (EXCEPTION_EXECUTE_HANDLER) {

        /* Something got hosed, make sure we deallocate our destination DIB */
        if (hDstDIB) {
          GlobalUnlock(hDstDIB);
          GlobalFree(hDstDIB);
          hDstDIB = NULL;
        }                
    }

    /* Clean up the mess we made */
    if (hDC) ReleaseDC(NULL, hDC);
    if (hSrcMemDC) DeleteDC(hSrcMemDC);
    if (hDstMemDC) DeleteDC(hDstMemDC);
    if (hSrcBmp) DeleteObject(hSrcBmp);
    if (hDstBmp) DeleteObject(hDstBmp);
    if (hSrcBmp) GlobalUnlock(hSrcDIB);
    if (hDstDIB) GlobalUnlock(hDstDIB);

    return hDstDIB;          
#endif
}

/******************************************************************************
 *                                                                            *
 * HANDLE ChangeDIBFormat(HANDLE hDIB, WORD wBPP, DWORD dwComp)               *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HDIB             - handle to packed-DIB in memory                          *
 * WORD             - desired bits per pixel                                  *
 * DWORD            - desired compression format                              *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HDIB             - handle to the new DIB if successful, else NULL          *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will convert the bits per pixel and/or the compression       *
 * format of the specified DIB. Note: If the conversion was unsuccessful,     *
 * we return NULL. The original DIB is left alone. Don't use code like the    *
 * following:                                                                 *
 *                                                                            *
 *    hMyDIB = ChangeDIBFormat(hMyDIB, 8, BI_RLE4);                           *
 *                                                                            *
 * The conversion will fail, but hMyDIB will now be NULL and the original     *
 * DIB will now hang around in memory. We could have returned the old         *
 * DIB, but we wanted to allow the programmer to check whether this           *
 * conversion succeeded or failed.                                            *
 *                                                                            *
 *****************************************************************************/
static HANDLE ChangeDIBFormat(HANDLE hDIB, WORD wBPP, DWORD dwComp)
{
#ifdef CV_NO_CHANGE_DIB_FORMAT
	return 0;
#else
   HBITMAP            hBitmap;              // Handle to bitmap
   LPBITMAPINFOHEADER lpbih;                // Pointer to bitmap info
   HANDLE             hNewDIB = NULL;       // Handle to new DIB
   HPALETTE           hPal;                 // Handle to palette, prev pal
   WORD               wOldBPP, wNewBPP;     // DIB bits per pixel, new bpp
   DWORD              dwOldComp, dwNewComp; // DIB compression, new compression
    
   /* Check for a valid DIB handle */
   if (!hDIB)
      return NULL;

   /* Get the old DIB's bits per pixel and compression format */
   lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
   wOldBPP = lpbih->biBitCount;
   dwOldComp = lpbih->biCompression;
   GlobalUnlock(hDIB);

   /* Validate wBPP and dwComp
    * They must match correctly (i.e., BI_RLE4 and 4 BPP or
    * BI_RLE8 and 8BPP, etc.) or we return failure */
   if (wBPP == 0) {
      wNewBPP = wOldBPP;
      if ((dwComp == BI_RLE4 && wNewBPP == 4) ||
          (dwComp == BI_RLE8 && wNewBPP == 8) ||
          (dwComp == BI_RGB))
        dwNewComp = dwComp;
      else
        return NULL;
      }
   else if (wBPP == 1 && dwComp == BI_RGB) {
      wNewBPP = wBPP;
      dwNewComp = BI_RGB;
      }
   else if (wBPP == 4) {
      wNewBPP = wBPP;
      if (dwComp == BI_RGB || dwComp == BI_RLE4)
        dwNewComp = dwComp;
      else
        return NULL;
      }
   else if (wBPP == 8) {
      wNewBPP = wBPP;
      if (dwComp == BI_RGB || dwComp == BI_RLE8)
        dwNewComp = dwComp;
      else
        return NULL;
      }
   else if (wBPP == 24 && dwComp == BI_RGB) {
      wNewBPP = wBPP;
      dwNewComp = BI_RGB;
      }
   else if (((wBPP == 16) || (wBPP == 32)) && 
            ((dwComp == BI_BITFIELDS) || (dwComp == BI_RGB))) {
        wNewBPP = wBPP;
        dwNewComp = dwComp;
      }
   else
        return NULL;

   if ((wOldBPP > 8) && (wNewBPP > 8) && (dwOldComp == BI_RGB) && (dwNewComp == BI_RGB)) 
     /* Keep as much color info as possible */
     hNewDIB = ConvertRGBDIB(hDIB, dwNewComp, wNewBPP);
   else {
       /* Save the old DIB's palette */
       hPal = CreateDIBPalette(hDIB);  
       if (!hPal)
          return NULL;
   
       /* Convert old DIB to a bitmap */
       hBitmap = BitmapFromDIB(hDIB, hPal);

       /* Convert Bitmap into a DIB with the new compression and BPP  */
       hNewDIB = DIBFromBitmap(hBitmap, dwNewComp, wNewBPP, hPal);
   }

   if (hNewDIB)
         GlobalFree(hDIB);

   DeleteObject(hBitmap);
   // DeleteObject(hPal);
   VPalettes::deletePalette(hPal);

   return hNewDIB;
#endif
}

/******************************************************************************
 *                                                                            *
 * BOOL ChangeDIBPalette(HANDLE hDIB, HPALETTE hPal)                          *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HDIB             - handle to packed-DIB in memory                          *
 * HPALETTE         - the new palette for the DIB                             *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * BOOL             - returns success or failure status                       *
 *                                                                            *
 * Description:                                                               *
 *                                                                            *
 * This function will remap the colors in the given DIB so that they          *
 * reference the colors in the new palette - note that this is not the same   *
 * changing the DIBs color table since we are forcing the bits to be          *
 * remapped to new values in addition to changing the color table.            *
 *                                                                            *
 *****************************************************************************/
static BOOL ChangeDIBPalette(HANDLE hDIB, HPALETTE hPal)
{
   HDC                hDC;      
   HBITMAP            hBitmap;  
   LPBITMAPINFOHEADER lpbih;      // Pointer to bitmap info
   HPALETTE           hOldPal;   // Handle to palette, prev pal
      
   /* Check for a valid DIB/Palette handle */
   if (!hDIB || !hPal)
      return FALSE;

   /* Convert old DIB to a bitmap */
   hBitmap = BitmapFromDIB(hDIB, hPal);

   /* Make sure it worked */
   if (!hBitmap) 
      return FALSE;

   /* Get a pointer to the DIB header */
   lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

   /* Get a DC and select/realize our palette in it */
   hDC  = GetDC(NULL);
   hOldPal = SelectPalette(hDC, hPal, FALSE);
   RealizePalette(hDC);

   /* Call GetDIBits and get the new DIB bits */
   if (!GetDIBits(hDC, 
                  hBitmap, 
                  (UINT)0, 
                  (UINT)lpbih->biHeight, 
                  (LPBYTE)lpbih + (WORD)lpbih->biSize + ColorTableSize((LPSTR)lpbih), 
                  (LPBITMAPINFO)lpbih, 
                  DIB_RGB_COLORS)) 
      return FALSE;

   /* Clean up and return */
   SelectPalette(hDC, hOldPal, TRUE);
   RealizePalette(hDC);
   ReleaseDC(NULL, hDC);

   /* Unlock the new DIB's memory block */
   if (hDIB)
      GlobalUnlock(hDIB);

   DeleteObject(hBitmap);

   return TRUE;
}

/******************************************************************************
 *                                                                            *
 * HANDLE CopyDIB(HANDLE hDIB)                                                *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to make a copy of.       *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HANDLE           - A copy of the DIB passed as a parameter                 *  
 *                                                                            *
 *****************************************************************************/
static HANDLE CopyDIB(HANDLE hDIB)
{
#ifdef CV_NO_CHANGE_DIB_FORMAT
	return 0;
#else
  HANDLE hDst = NULL;
  LPBYTE pSrc, pDst;
  DWORD dwSize = GlobalSize(hDIB);

  hDst = GlobalAlloc(GHND, dwSize);
  if (!hDst) return NULL;

  pSrc = LPBYTE(GlobalLock(hDIB));
  if (pSrc == NULL) {
    GlobalUnlock(hDst);
    GlobalFree(hDst);
    return NULL;
  }

  pDst = LPBYTE(GlobalLock(hDst));

  CopyMemory(pDst, pSrc, dwSize); 

  GlobalUnlock(hDIB);
  GlobalUnlock(hDst);

  return (hDst);
#endif
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : CopyBitmap (HBITMAP hBitmap)                               *
 *                                                                          *
 *  PURPOSE    : Copies the given bitmap to another.                        *
 *                                                                          *
 *  RETURNS    : A handle to the new bitmap.                                *
 *                                                                          *
 ****************************************************************************/
static HBITMAP CopyBitmap (HBITMAP hBitmap)
{
    BITMAP  Bitmap;
    RECT    rect;

    if (!hBitmap)
         return NULL;

    GetObject (hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = Bitmap.bmWidth;
    rect.bottom = Bitmap.bmHeight;

    return CropBitmap (hBitmap, &rect);
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   :  CropBitmap (HBITMAP hBitmap, LPRECT lpRect)               *
 *                                                                          *
 *  PURPOSE    :  Crops a bitmap to a new size specified by the lpRect      *
 *                parameter.                                                *
 *                                                                          *
 *  RETURNS    :  A handle to the new bitmap.                               *
 *                                                                          *
 ****************************************************************************/
static HBITMAP CropBitmap (HBITMAP hBitmap, LPRECT lpRect)
{
    HDC     hMemDCsrc;
    HDC     hMemDCdst;
    HDC     hDC;
    HBITMAP hNewBm;
    BITMAP  Bitmap;
    INT     dx, dy;

    if (!hBitmap)
         return NULL;

    hDC = GetDC (NULL);
    hMemDCsrc = CreateCompatibleDC (hDC);
    hMemDCdst = CreateCompatibleDC (hDC);

    GetObject (hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    dx = lpRect->right  - lpRect->left;
    dy = lpRect->bottom - lpRect->top;

    /*hNewBm = +++CreateBitmap - Not Recommended(use CreateDIBitmap)+++ (dx, dy, Bitmap.BitmapPlanes, Bitmap.BitmapBitsPixel, NULL);*/
    hNewBm = CreateBitmap(dx, dy, Bitmap.bmPlanes, Bitmap.bmBitsPixel, NULL);
    if (hNewBm) {
        SelectObject (hMemDCsrc, hBitmap);
        SelectObject (hMemDCdst, hNewBm);

        BitBlt (hMemDCdst, 
                0, 
                0, 
                dx, 
                dy, 
                hMemDCsrc, 
                lpRect->left, 
                lpRect->top, 
                SRCCOPY);
    }

    ReleaseDC (NULL, hDC);
    DeleteDC (hMemDCsrc);
    DeleteDC (hMemDCdst);

    return hNewBm;
}

/******************************************************************************
 *                                                                            *
 * BOOL CopyDIBData(HANDLE hDIBDst, HANDLE hDIBSrc)                           *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to copy to.              *
 * HANDLE           - The handle to the DIB you want to copy from.            *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HANDLE           - Success or failure of the function                      *  
 *                                                                            *
 *  Replaces the contents of the destination DIB with the contents of the     *
 *  source DIB.  This will fail if the initial sizes of the DIBS are          * 
 *  not equal.                                                                *
 *                                                                            *
 *****************************************************************************/
static BOOL CopyDIBData(HANDLE hDIBDst, HANDLE hDIBSrc)
{
#ifdef CV_NO_CHANGE_DIB_FORMAT
	return FALSE;
#else
  DWORD dwDstSize = GlobalSize(hDIBDst);
  DWORD dwSrcSize = GlobalSize(hDIBSrc);
  void *pSrc, *pDst;

  if (dwDstSize != dwDstSize)
    return FALSE;

  pDst = GlobalLock(hDIBDst);
  pSrc = GlobalLock(hDIBSrc);

  CopyMemory(pDst, pSrc, dwDstSize); //Win32s problem?

  GlobalUnlock(hDIBSrc);
  GlobalUnlock(hDIBDst);

  return (TRUE);
#endif
}

/******************************************************************************
 *                                                                            *
 * BOOL GetDIBResolution(HANDLE hDIB, LONG *iXRes, LONG *iYRes)               *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want get the resolution of    *
 * LONG*            - Pointer to x resolution                                 *
 * LONG*            - Pointer to y resolution                                 *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * HANDLE           - Success or failure of the function                      *  
 *                                                                            *
 *  Reads the width and the height of a DIB from the BITMAPINFOHEADER and     *
 *  returns the values.                                                       *
 *                                                                            *
 *****************************************************************************/
static BOOL GetDIBResolution(HANDLE hDIB, LONG *iXRes, LONG *iYRes)
{
  LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
  
  if (!lpbih) return FALSE;
  
  *iXRes = lpbih->biWidth;
  *iYRes = lpbih->biHeight;

  GlobalUnlock(hDIB);

  return TRUE;
}

/******************************************************************************
 *                                                                            *
 * DWORD GetDIBColorUsed(HANDLE hDIB)                                         *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to check                 *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * DWORD            - Number of colors used by the DIB                        *  
 *                                                                            *
 *  Reads and returns the number of colors used from the BITMAPINFOHEADER     *
 *                                                                            *
 *****************************************************************************/
static DWORD GetDIBColorUsed(HANDLE hDIB)
{
  LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
  DWORD dwClrUsed = 0;

  if (!lpbih) return 0;
  
  dwClrUsed = lpbih->biClrUsed;

  GlobalUnlock(hDIB);

  return dwClrUsed;
}

/******************************************************************************
 *                                                                            *
 * DWORD GetDIBCopmression(HANDLE hDIB)                                       *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to check                 *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * DWORD            - The compression type for the given DIB                  *  
 *                                                                            *
 *****************************************************************************/
static DWORD GetDIBCompression(HANDLE hDIB)
{
  LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
  DWORD dwComp = 0;

  if (!lpbih) return 0;
  
  dwComp = lpbih->biCompression;

  GlobalUnlock(hDIB);

  return dwComp;
}

/******************************************************************************
 *                                                                            *
 * DWORD GetDIBBitcount(HANDLE hDIB)                                          *
 *                                                                            *
 * Parameter:                                                                 *
 *                                                                            *
 * HANDLE           - The handle to the DIB you want to check                 *
 *                                                                            *
 * Return Value:                                                              *
 *                                                                            *
 * DWORD            - The number of bits per pixel for given DIB              *  
 *                                                                            *
 *****************************************************************************/
static WORD GetDIBBitCount(HANDLE hDIB)
{
  LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
  WORD wBitCount = 0;

  if (!lpbih) return 0;
  
  wBitCount = lpbih->biBitCount;

  GlobalUnlock(hDIB);

  return wBitCount;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : CreateDIBPaletteEx(HANDLE hbi, BYTE bFlags)                  *
 *                                                                            *
 *  PURPOSE    : Given a Global HANDLE to a BITMAPINFO Struct will create a   *
 *               GDI palette object from the color table. (BITMAPINFOHEADER   *
 *               format DIBs only)  In addition, you can specify the palette  *
 *               palette flags for the palette entries.                       *
 *                                                                            *
 *  RETURNS    : A handle to the palette.                                     *
 *                                                                            *
 *****************************************************************************/
static HPALETTE CreateDIBPaletteEx(HANDLE hDIB, BYTE bFlags)
{
    HPALETTE hPal, hNewPal;

    if (!hDIB)
        return NULL;

    hPal = CreateDIBPalette(hDIB);
    hNewPal = CopyPaletteEx(hPal, bFlags);
    // DeleteObject(hPal);
	VPalettes::deletePalette(hPal);
       
    return hNewPal;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : SetOptimizedPaletteState(BOOL bSet)                          *
 *                                                                            *
 *  PURPOSE    : Sets a flag that determines if a bitmap being loaded will    *
 *               use an optimized palette or a spectrum palette.              *
 *                                                                            *
 *****************************************************************************/
void SetOptimizedPaletteState(BOOL bSet) {
#ifndef	CV_NO_OPTIMIZE_PALETTE
  bUseOptimizedPalette = bSet;
#endif
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : GetOptimizedPaletteState()                                   *
 *                                                                            *
 *  PURPOSE    : Retrieves the flag that determines if a bitmap being loaded  *
 *               will use an optimized palette or a spectrum palette.         *
 *                                                                            *
 *  RETURNS    : TRUE if loads will used an optimized palette                 *
 *                                                                            *
 *****************************************************************************/
static BOOL GetOptimizedPaletteState()
{
#ifdef	CV_NO_OPTIMIZE_PALETTE
  return FALSE;
#else
  return bUseOptimizedPalette;
#endif
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : GetAndLockDIBPointers(HANDLE hDIB, LPVOID *biPtr,            *
 *                                     LPVOID *rgbqPtr, LPVOID *bmPtr)        *
 *                                                                            *
 *  PURPOSE    : Given a handle to a DIB in CF_DIB format, this function will *
 *               retrieve pointers for the bitmap info, color table, and bits *
 *                                                                            *
 *****************************************************************************/
static BOOL GetAndLockDIBPointers(HANDLE hDIB, LPVOID *biPtr, LPVOID *rgbqPtr, LPVOID *bmPtr)
{
   BITMAPINFO* pbi;
   
   pbi = (BITMAPINFO *)GlobalLock(hDIB);  
   if (!pbi)
     return FALSE;

   *biPtr   = (LPVOID)pbi;
   
   *rgbqPtr = (LPVOID)((LPSTR)pbi + (WORD)((LPBITMAPINFOHEADER)pbi)->biSize);
   if (((LPBITMAPINFOHEADER)pbi)->biCompression == BI_BITFIELDS)
      rgbqPtr = (LPVOID *)((LPDWORD)rgbqPtr + 3); 

   *bmPtr   = (LPBYTE)pbi + (WORD)pbi->bmiHeader.biSize + ColorTableSize(pbi); 

   return TRUE;
}

/******************************************************************************
 *                                                                            *
 *  FUNCTION   : HANDLE CreateRGBDIB(DWORD dwWidth, DWORD dwHeight,           *
 *                                    WORD wBPP, DWORD dwComp);               *
 *                                                                            *
 *  PURPOSE    : Returns a handle to a RGB DIB (no color table) with the      *
 *               specified width, height, bits per pixel, and compression.    *
 *                                                                            *
 *****************************************************************************/
static HANDLE CreateRGBDIB(DWORD dwWidth, DWORD dwHeight, WORD wBPP, DWORD dwComp)
{
   HANDLE hDIB;
   LPBITMAPINFOHEADER lpbih;
   DWORD dwSize;
   LPDWORD lpMasks;
   
   /* Only allow DIBs with RGB with RGB data in bits */
   if ((wBPP < 16) || (dwComp == BI_RLE8) || (dwComp == BI_RLE4)) return NULL;

   /* Allocate enough memory to hold the DIB */
   switch (wBPP) {
     case 16: dwSize = ((dwWidth + (dwWidth & 1)) << 1) * dwHeight; break;
     case 24: dwSize = (DWORD)BYTESPERLINE(dwWidth, 24) * dwHeight; break;
     case 32: dwSize = ((dwWidth * dwHeight) << 2); break;
     default: return NULL;
   }                                               
   
   if (dwComp == BI_BITFIELDS) /* Add in space for DWORD masks */
     hDIB = GlobalAlloc(GHND, (DWORD)sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * 3) + dwSize);
   else
     hDIB = GlobalAlloc(GHND, (DWORD)sizeof(BITMAPINFOHEADER) + dwSize);

   lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
   lpbih->biSize         = sizeof(BITMAPINFOHEADER);
   lpbih->biWidth        = dwWidth;
   lpbih->biHeight       = dwHeight;
   lpbih->biPlanes       = 1;
   lpbih->biBitCount     = wBPP;
   lpbih->biCompression  = dwComp;    
   lpbih->biSizeImage    = dwSize; 

   lpMasks = (LPDWORD)((LPSTR)lpbih + (WORD)lpbih->biSize);
   
   if (dwComp == BI_BITFIELDS) 
       if (wBPP == 16) {
         lpMasks[0] = MAKE565WORD(0xff, 0, 0);
         lpMasks[1] = MAKE565WORD(0, 0xff, 0);
         lpMasks[2] = MAKE565WORD(0, 0, 0xff);
       } else if (wBPP == 16) {
           lpMasks[0] = 0xff;
           lpMasks[1] = 0xff;
           lpMasks[2] = 0xff;
         }
      
   GlobalUnlock(hDIB);

   return hDIB;
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : CopyHandle (HANDLE h)                                      *
 *                                                                          *
 *  PURPOSE    : Makes a copy of the given global memory block.             *
 *                                                                          *
 *  RETURNS    : A handle to the new block.                                 *
 *                                                                          *
 ****************************************************************************/
static HANDLE CopyHandle (HANDLE h)
{
    BYTE *lpCopy;
    BYTE *lp;
    HANDLE hCopy;
    DWORD  dwLen;

    dwLen = GlobalSize (h);
    if (hCopy = GlobalAlloc (GHND, dwLen)) {

        lpCopy = (BYTE *)GlobalLock (hCopy);
        lp     = (BYTE *)GlobalLock (h);
        while (dwLen--) *lpCopy++ = *lp++;
        GlobalUnlock (hCopy);
        GlobalUnlock (h);
    }
    return hCopy;
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : RenderFormat(int cf)                                       *
 *                                                                          *
 *  PURPOSE    : Renders the currently displayed DIB in CF_DIB or           *
 *               CF_BITMAP format.The bitmap is clipped to the current      *
 *               rcClip.                                                    *
 *                                                                          *
 *  RETURNS    : A handle to the DIB                                        *
 *                                                                          *
 ****************************************************************************/
static HANDLE RenderFormat (INT cf, HBITMAP hbmCurrent,
									HANDLE hdibCurrent, HANDLE hpalCurrent)
{
    HANDLE  h = NULL;
    HBITMAP hbm;

    switch (cf){
        case CF_BITMAP:
#if 0
            if (hbmCurrent && !IsRectEmpty (&rcClip))
                h = CropBitmap (hbmCurrent, &rcClip);
            else
#endif
			{
                if (hbmCurrent)
                    h = CopyBitmap (hbmCurrent);
                else if (hdibCurrent)
                    h = BitmapFromDIB (hdibCurrent, HPALETTE(hpalCurrent));
#if 0
                else if (achFileName[0] && (hdibCurrent = OpenDIB (achFileName)))
                    h = BitmapFromDIB (hdibCurrent, hpalCurrent);
#endif
                else
                    h = NULL;

#if 0
                if (h && !IsRectEmpty (&rcClip)){
                    hbm = CropBitmap (h,&rcClip);
                    DeleteObject (h);
                    h = hbm;
                }
#endif
            }
            break;

        case CF_DIB:
#if 0
            if (!IsRectEmpty (&rcClip)){
                if (hbm = RenderFormat (CF_BITMAP)){
                    h = DIBFromBitmap (hbm, BI_RGB, 0, hpalCurrent);
                    DeleteObject (hbm);
                }
            }
            else
#endif
			{
                if (!hdibCurrent && hbmCurrent)
                    h = DIBFromBitmap (hbmCurrent, BI_RGB, 0,
									   HPALETTE(hpalCurrent));
                else if (hdibCurrent)
                    h = CopyHandle (hdibCurrent);
#if 0
                else if (achFileName[0])
                    h = OpenDIB (achFileName);
#endif
                else
                    h = NULL;
            }
            break;

        case CF_PALETTE:
            if (hpalCurrent)
                h = CopyPalette (HPALETTE(hpalCurrent));
            break;
    }
    return h;
}

// ==========================================================================
// ::: VBitMap Implementation
// ==========================================================================

defineClass(VBitMap, VDisplay)
defineArchiveRevision(VBitMap, 1)

// --------------------------------------------------------------------------
//
VBitMap::VBitMap()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VBitMap::VBitMap(const char *native_resource_name)
{
	initialize();
	hBitmap = LoadBitmap(notifier->inst(), LPSTR(native_resource_name));
	if (hBitmap != 0) {
		haveValidBitmap = 1;
	}
	else {
		loadFile(native_resource_name);
	}
}

// --------------------------------------------------------------------------
//
VBitMap::VBitMap(int w, int h, boolean monochrome)
{
	initialize();
	if (monochrome) {
		if ((hBitmap = CreateBitmap(w, h, 1, 1, LPSTR(0))) == 0) {
			return;
		}
	}
	else {
		HDC hdc = GetDC(0);
		if (hdc == 0) {
			return;
		}
		if ((hBitmap = CreateCompatibleBitmap(hdc, w, h)) == 0) {
			return;
		}
		ReleaseDC(0, hdc);
	}
	haveValidBitmap = 1;
}

// --------------------------------------------------------------------------
//
VBitMap::VBitMap(int w, int h, VDisplay *display)
{
	initialize();

	HDC hdc = display != 0 ? display->getHndl() : 0;

	if (hdc == 0) {
		return;
	}

	hBitmap = CreateCompatibleBitmap(hdc, w, h);
	if (hBitmap != 0) {
		//
		// Uh-oh, we're now sharing this palette, let's start tracking
		// it using the VPalettes mechanism; not exactly the way I
		// would've designed it from scratch, but it'll work for now.
		//
		HPALETTE hpalette = display->getPalette();
		VPalettes::sharePalette(hpalette);
		setPalette(hpalette);
		compatibleDisplay = display;
	}

	if (display != 0) {
		display->ungetHndl();
	}

	haveValidBitmap = 1;
}

// --------------------------------------------------------------------------
//
VBitMap::VBitMap(HANDLE hdib)
{
	initialize();
	if (hdib != 0) {
		HPALETTE hpalette = CreateDIBPalette(hdib);
		hDib = hdib;
		setPalette(hpalette);
		haveValidBitmap = 1;
		//
		// ToDo! Temporary hack till I'm able to
		// figure how to get DIBBlt() to work correctly.
		//
		hBitmap = BitmapFromDIB(hdib, hpalette);
	}
}

// --------------------------------------------------------------------------
//
VBitMap::~VBitMap()
{
	clean();
}

// --------------------------------------------------------------------------
//
void VBitMap::initialize()
{
	hBitmap				= 0;
	hDib				= 0;
	hOldBitmap			= 0;
	hOldPalette			= 0;
	compatibleDisplay	= 0;
	frameCount			= 0;
	sequenceLength		= 0;
	haveValidBitmap		= 0;
	optimizePalette		= 0;
}

// --------------------------------------------------------------------------
//
void VBitMap::clean()
{
	HPALETTE hpalette = getPalette();
	if (hpalette != 0) {
		setPalette(0);
		VPalettes::deletePalette(hpalette);
	}
	if (hBitmap != 0) {
		DeleteObject(hBitmap);
	}
	if (hDib != 0) {
		GlobalFree(hDib);
	}
	initialize();
}

// --------------------------------------------------------------------------
//
boolean VBitMap::loadFile(const char *filename)
{
	if ((filename == 0) || (filename[0] == '\0')) {
		return FALSE;
	}
	FileOrArchive foa(filename);
	return loadFile(foa);
}

// --------------------------------------------------------------------------
//
boolean VBitMap::loadFile(FileOrArchive& foa)
{
	HANDLE hdib;

    if ((hdib = OpenDIB(foa)) == 0) {
		return FALSE;
	}

    BITMAPINFOHEADER bih;
    DIBInfo(hdib, &bih);

	HPALETTE hpalette = bih.biBitCount > 4 ? CreateDIBPalette(hdib) : 0;

#if 0
	//
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// This doesn't seem to work ... this would explain
	// the duplicate OpenDIB() below which undoes its work.
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//
	// Convert the DIB color table to palette relative indexes, so
	// SetDIBits() and SetDIBitsToDevice() can avoid color matching.
	// We can do this because the palette we realize is identical
	// to the color table of the bitmap, i.e. the indexes match 1 to 1.
	// Now that the DIB color table is palette indices not RGB values
	// we must use DIB_PAL_COLORS as the wUsage parameter to SetDIBits()
	//
    LPBITMAPINFOHEADER lpbih = (VOID FAR *)GlobalLock(hdib);
	if (lpbih->biBitCount != 24) { // fPalColors = TRUE;
		WORD FAR *pw = (WORD FAR *)(LPSTR(lpbih) + lpbih->biSize);
		for (INT i = 0 ; i < INT(lpbih->biClrUsed) ; i++) {
			*pw++ = WORD(i);
		}
	}
	GlobalUnlock(hdib);
	hdib = OpenDIB(foa); // Again?!
#endif

#if 1

	// We will set hBitmap to a device-dependent bitmap (DDB) only
	// if/when we need it.  Note that if haveValidBitmap is set, it
	// means that either hBitmap or hDib or both are valid (non-zero).

	HBITMAP hbitmap;
	if ((hbitmap = BitmapFromDIB(hdib, hpalette)) == 0) {
		GlobalFree(hdib);
		DeleteObject(hpalette);
		VPalettes::deletePalette(hpalette);
		return FALSE;
	}
	hBitmap = hbitmap;

#endif

	hDib = hdib;
	setPalette(hpalette);
    
    return haveValidBitmap = 1;
}

// --------------------------------------------------------------------------
//
boolean VBitMap::saveFile(const char *filename)
{
	return saveFile(filename, getBitCount(), getCompression());
}

// --------------------------------------------------------------------------
//
boolean VBitMap::saveFile(const char *filename,
						  short bitcount, long compression)
{
	if (!haveValidBitmap || (filename == 0) || (filename[0] == '\0')) {
		return FALSE;
	}
	FileOrArchive foa(filename);
	return saveFile(foa, bitcount, compression);
}

// --------------------------------------------------------------------------
//
boolean VBitMap::saveFile(FileOrArchive& foa,
						  short bitcount, long compression)
{
	if (!haveValidBitmap) {
		return FALSE;
	}
	return WriteDIBEx(foa,
					  getDIB(bitcount, compression), bitcount, compression);
}

// --------------------------------------------------------------------------
//
HBITMAP VBitMap::getBitmap() const
{
	if (haveValidBitmap) {
		if (hBitmap == 0) {
			HPALETTE hpalette = ((VBitMap *)this)->getPalette();
			((VBitMap *)this)->hBitmap = BitmapFromDIB(hDib, hpalette);
		}
		return HBITMAP(hBitmap);
	}
	return 0;
}

// --------------------------------------------------------------------------
//
HANDLE VBitMap::getDIB(short bitcount, long compression) const
{
	if (haveValidBitmap) {
		if (hDib == 0) {
			((VBitMap *)this)->hDib =
				DIBFromBitmap(HBITMAP(hBitmap), compression, bitcount,
							  ((VBitMap *)this)->getPalette());
		}
		return hDib;
	}
	return 0;
}

// --------------------------------------------------------------------------
//
HDC VBitMap::getHndl()
{
	if (!haveValidBitmap) {
		return 0;
	}

	if (this->hDC != 0) {
		this->refCount++;
		return this->hDC;
	}

	if (compatibleDisplay != 0) {
		HDC dc = compatibleDisplay->getHndl();
		if (dc == 0) {
			return 0;
		}
		this->hDC = CreateCompatibleDC(dc);
		compatibleDisplay->ungetHndl();
	}
	else  {
		HDC dc = GetDC(0);
		this->hDC = CreateCompatibleDC(dc);
		ReleaseDC(0, dc);
	}

	if (this->hDC == 0) {
		return 0;
	}

	HPALETTE hpalette = getPalette();

	if (hBitmap == 0) {
		if ((hBitmap = BitmapFromDIB(hDib, hpalette)) == 0) {
			return 0;
		}
	}

	if (hpalette != 0) {
		hOldPalette = SelectPalette(this->hDC, hpalette, FALSE);
	}
	hOldBitmap = SelectObject(this->hDC, hBitmap);

	this->refCount++;
	return this->hDC;
}

// ---------------------------------------------------------------------------
// Release the device handle for this VBitmap.
//
void VBitMap::ungetHndl()
{
	if ((this->hDC != 0) && (--refCount == 0)) {
		if (hOldBitmap != 0) {
			SelectObject(this->hDC, hOldBitmap);
			hOldBitmap = 0;
		}
		if (hOldPalette != 0) {
			SelectPalette(this->hDC, HPALETTE(hOldPalette), FALSE);
			hOldPalette = 0;
		}
		DeleteDC(this->hDC);
		this->hDC = 0;
	}
}

// --------------------------------------------------------------------------
//
void VBitMap::sizeOfImage(int *w, int *h)
{
	if (haveValidBitmap) {
		if (hBitmap != 0) {
			BITMAP bm;
			GetObject(hBitmap, sizeof(BITMAP), LPSTR(&bm));
			if (w != 0) { *w = bm.bmWidth; }
			if (h != 0) { *h = bm.bmHeight; }
			return;
		}
		else {
			BITMAPINFOHEADER bih;
			DIBInfo(hDib, &bih);
			if (w != 0) { *w = bih.biWidth; }
			if (h != 0) { *h = bih.biHeight; }
			return;
		}
	}
	if (w != 0) { *w = 0; }
	if (h != 0) { *h = 0; }
}

// --------------------------------------------------------------------------
//
int VBitMap::depthOfImage()
{
	if (haveValidBitmap) {
getBitmap();
		if (hDib != 0) {
			BITMAPINFOHEADER bih;
			DIBInfo(hDib, &bih);
			return bih.biBitCount / bih.biPlanes;
		}
		else {
			BITMAP bm;
			GetObject(hBitmap, sizeof(BITMAP), LPSTR(&bm));
			return bm.bmBitsPixel;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
void VBitMap::getByteDim(int *w, int *h) const
{
	if (haveValidBitmap) {
		if (hBitmap != 0) {
			BITMAP bm;
			GetObject(hBitmap, sizeof(BITMAP), LPSTR(&bm));
			if (w != 0) { *w = bm.bmWidthBytes; }
			if (h != 0) { *h = bm.bmHeight / 8; }
			return;
		}
		else {
			BITMAPINFOHEADER bih;
			DIBInfo(hDib, &bih);
			if (w != 0) { *w = bih.biWidth / 8; }
			if (h != 0) { *h = bih.biHeight / 8; }
			return;
		}
	}
	if (w != 0) { *w = 0; }
	if (h != 0) { *h = 0; }
}

// --------------------------------------------------------------------------
//
int VBitMap::planes() const
{
	if (haveValidBitmap) {
		if (hBitmap != 0) {
			BITMAP bm;
			GetObject(hBitmap, sizeof(BITMAP), LPSTR(&bm));
			return bm.bmPlanes;
		}
		else {
			BITMAPINFOHEADER bih;
			DIBInfo(hDib, &bih);
			return bih.biPlanes;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
int VBitMap::getBitCount() const
{
	if (haveValidBitmap) {
		BITMAPINFOHEADER bih;
		DIBInfo(hDib, &bih);
		return bih.biBitCount;
	}
	return 8;
}

// --------------------------------------------------------------------------
//
long VBitMap::getCompression() const
{
	if (haveValidBitmap) {
		BITMAPINFOHEADER bih;
		DIBInfo(hDib, &bih);
		return bih.biCompression;
	}
	return BI_RGB;
}

// --------------------------------------------------------------------------
//
VColor VBitMap::readPixel(const VLocation& location) const
{
	HDC hdc = ((VBitMap *)this)->getHndl();
	if (hdc != 0) {
		VColor color = GetPixel(hdc, location.x(), location.y());
		((VBitMap *)this)->ungetHndl();
		return color;
	}
	return VColor(VColor::White);
}

// --------------------------------------------------------------------------
//
void VBitMap::readPixelRow(const VLocation& location,
						   VColor *colors, int n) const
{
	HDC hdc = ((VBitMap *)this)->getHndl();
	if (hdc != 0) {
		for (int i = 0 ; i < n ; i++) {
			colors[i] = GetPixel(hdc, location.x() + i, location.y());
		}
		((VBitMap *)this)->ungetHndl();
	}
	else {
		VColor white(VColor::White);
		for (int i = 0 ; i < n ; i++) {
			colors[i] = white;
		}
	}
}

// --------------------------------------------------------------------------
//
VColor VBitMap::writePixel(const VLocation& location, const VColor& color)
{
	HDC hdc = getHndl();
	if (hdc != 0) {
		VColor result = SetPixel(hdc, location.x(), location.y(), color);
		ungetHndl();
		return result;
	}
	return VColor(VColor::White);
}

// --------------------------------------------------------------------------
//
void VBitMap::eraseBlack()
{
	HDC hdc = getHndl();
	if (hdc != 0) {
		int w, h;
		sizeOfImage(&w, &h);
		PatBlt(hdc, 0, 0, w, h, BLACKNESS);
		ungetHndl();
	}
}

// --------------------------------------------------------------------------
//
void VBitMap::eraseWhite()
{
	HDC hdc = getHndl();
	if (hdc != 0) {
		int w, h;
		sizeOfImage(&w, &h);
		PatBlt(hdc, 0, 0, w, h, WHITENESS);
		ungetHndl();
	}
}

// --------------------------------------------------------------------------
//
void VBitMap::setCompObj(VDisplay *display)
{
	compatibleDisplay = display;
}

// --------------------------------------------------------------------------
//
void VBitMap::setBitmap(HBITMAP hbitmap)
{
	if (hbitmap == 0) {
		if (haveValidBitmap) {
			if (hBitmap != 0) {
				DeleteObject(hBitmap);
				hBitmap = 0;
			}
		}
	}
	else if (hbitmap != hBitmap) {
		int bitcount = getBitCount();
		long compression = getCompression();
		HPALETTE hpalette = getPalette();
		HANDLE hdib = DIBFromBitmap(HBITMAP(hBitmap), compression,
													  bitcount,
													  HPALETTE(hpalette));
		if (hdib != 0) {
			if (haveValidBitmap) {
				if (hBitmap != 0) {
					DeleteObject(hBitmap);
				}
				if (hDib != 0) {
					GlobalFree(hDib);
				}
			}
			hBitmap = hbitmap;
			hDib = hdib;
		}
	}
}

// --------------------------------------------------------------------------
//
void VBitMap::bitblt(VDisplay *dst, int dx, int dy, long rop)
{
	HDC dst_hdc = dst->getHndl();

	if (dst_hdc == 0) {
		return;
	}

	switch (rop) {
		case CopyRule:		rop = SRCCOPY;		break;
		case OrRule:		rop = SRCPAINT;		break;
		case XorRule:		rop = SRCINVERT;	break;
		case NoCopyRule:	rop = NOTSRCCOPY;	break;
		case NotOrRule:		rop = MERGEPAINT;	break;
		case NotXorRule:	rop = 0x990066;		break;
		case EraseRule:		rop = 0x7700E6;		break;
		case AndRule:		rop = SRCAND;		break;
		default:			rop = SRCCOPY;		break;
	}

	extern VClass *VPrinterCls;

	if (dst->isA(VPrinterCls)) {
		int w, h;
		if (hBitmap != 0) {
			BITMAP bm;
			GetObject(hBitmap, sizeof(BITMAP), LPSTR(&bm));
			w = bm.bmWidth;
			h = bm.bmHeight;
		}
		else {
			BITMAPINFOHEADER bi;
			DIBInfo(hDib, &bi);
			w = bi.biWidth;
			h = bi.biHeight;
		}
		StretchDIBBlt(dst_hdc, dx, dy, w, h, hDib, 0, 0, w, h, rop);
	}
	else {
		HPALETTE hpalette = getPalette(), old_hpalette = 0;
		if (hpalette != 0) {
			old_hpalette = SelectPalette(dst_hdc, hpalette, TRUE);
			RealizePalette(dst_hdc);
		}
		if (hBitmap == 0) {
			//
			// ToDo! Temporary hack till I'm able to
			// figure how to get DIBBlt() to work correctly.
			//
			getBitmap();
		}
		if (hBitmap != 0) {
			BITMAP bm;
			GetObject(hBitmap, sizeof(BITMAP), LPSTR(&bm));
			HDC src_hdc = CreateCompatibleDC(dst_hdc);
			SelectObject(src_hdc, hBitmap);
			BitBlt(dst_hdc, dx, dy, bm.bmWidth,
									bm.bmHeight, src_hdc, 0, 0, rop); 
			DeleteDC(src_hdc);
		}
		else {
			BITMAPINFOHEADER bi;
			DIBInfo(hDib, &bi);
			DIBBlt(dst_hdc, 0, 0, bi.biWidth, bi.biHeight, hDib, 0, 0, rop);
		}
		if (old_hpalette != 0) {
			SelectPalette(dst_hdc, old_hpalette, TRUE);
		}
	}

	dst->ungetHndl();
}

// --------------------------------------------------------------------------
//
void VBitMap::bitblt(VDisplay *dst,
					 int dx, int dy, int w, int h, int sx, int sy, long rop)
{
	HDC dst_hdc = dst->getHndl();

	if (dst_hdc == 0) {
		return;
	}

	switch (rop) {
		case CopyRule:		rop = SRCCOPY;		break;
		case OrRule:		rop = SRCPAINT;		break;
		case XorRule:		rop = SRCINVERT;	break;
		case NoCopyRule:	rop = NOTSRCCOPY;	break;
		case NotOrRule:		rop = MERGEPAINT;	break;
		case NotXorRule:	rop = 0x990066;		break;
		case EraseRule:		rop = 0x7700E6;		break;
		case AndRule:		rop = SRCAND;		break;
		default:			rop = SRCCOPY;		break;
	}

	extern VClass *VPrinterCls;

	if (dst->isA(VPrinterCls)) {
		StretchDIBBlt(dst_hdc, dx, dy, w, h, hDib, sx, sy, w, h, rop);
	}
	else {
		HPALETTE hpalette = getPalette(), old_hpalette = 0;
		if (hpalette != 0) {
			old_hpalette = SelectPalette(dst_hdc, hpalette, TRUE);
			RealizePalette(dst_hdc);
		}
		if (hBitmap == 0) {
			//
			// ToDo! Temporary hack till I'm able to
			// figure how to get DIBBlt() to work correctly.
			//
			getBitmap();
		}
		if (hBitmap != 0) {
			HDC src_hdc = CreateCompatibleDC(dst_hdc);
			SelectObject(src_hdc, hBitmap);
			BitBlt(dst_hdc, dx, dy, w, h, src_hdc, sx, sy, rop); 
			DeleteDC(src_hdc);
		}
		else {
			DIBBlt(dst_hdc, 0, 0, w, h, hDib, 0, 0, rop);
		}
		if (old_hpalette != 0) {
			SelectPalette(dst_hdc, old_hpalette, TRUE);
		}
	}

	dst->ungetHndl();
}

// --------------------------------------------------------------------------
//
void VBitMap::bitblt(VDisplay *dst, int dx, int dy, int dw, int dh,
									int sx, int sy, int sw, int sh, long rop)
{
	HDC dst_hdc = dst->getHndl();

	if (dst_hdc == 0) {
		return;
	}

	switch (rop) {
		case CopyRule:		rop = SRCCOPY;		break;
		case OrRule:		rop = SRCPAINT;		break;
		case XorRule:		rop = SRCINVERT;	break;
		case NoCopyRule:	rop = NOTSRCCOPY;	break;
		case NotOrRule:		rop = MERGEPAINT;	break;
		case NotXorRule:	rop = 0x990066;		break;
		case EraseRule:		rop = 0x7700E6;		break;
		case AndRule:		rop = SRCAND;		break;
		default:			rop = SRCCOPY;		break;
	}

	extern VClass *VPrinterCls;

	if (dst->isA(VPrinterCls)) {
		StretchDIBBlt(dst_hdc, dx, dy, dw, dh, hDib, sx, sy, dw, dh, rop);
	}
	else {
		HPALETTE hpalette = getPalette(), old_hpalette = 0;
		if (hpalette != 0) {
			old_hpalette = SelectPalette(dst_hdc, hpalette, TRUE);
			RealizePalette(dst_hdc);
		}
		if (hBitmap == 0) {
			//
			// ToDo! Temporary hack till I'm able to
			// figure how to get DIBBlt() to work correctly.
			//
			getBitmap();
		}
		if (hBitmap != 0) {
			HDC src_hdc = CreateCompatibleDC(dst_hdc);
			SelectObject(src_hdc, hBitmap);
			StretchBlt(dst_hdc, dx, dy, dw, dh,
					   src_hdc, sx, sy, sw, sh, rop);
			DeleteDC(src_hdc);
		}
		else {
			StretchDIBBlt(dst_hdc, dx, dy, dw, dh,
						  hDib, sx, sy, sw, sh, rop);
		}
		if (old_hpalette != 0) {
			SelectPalette(dst_hdc, old_hpalette, TRUE);
		}
	}

	dst->ungetHndl();
}

// --------------------------------------------------------------------------
//
boolean VBitMap::canPasteFromClipboard()
{
	VWindow *window = notifier->getFocus();
	if (window != 0) {
		HWND hwnd = window->hwnd();
		if (OpenClipboard(hwnd)) {
			if ((IsClipboardFormatAvailable(CF_DIB) != 0) ||
				(IsClipboardFormatAvailable(CF_BITMAP)) != 0) {
				CloseClipboard();
				return TRUE;
			}
			CloseClipboard();
		}
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VBitMap::pasteFromClipboard()
{
	VWindow *window = notifier->getFocus();
	if (window == 0) {
		return FALSE;
	}

	HWND hwnd = window->hwnd();
	if (hwnd == 0) {
		return FALSE;
	}

	if (!OpenClipboard(hwnd)) {
		return FALSE;
	}

    HANDLE hdib;
	
	if ((hdib = GetClipboardData(CF_DIB)) != 0) {
		hdib = CopyHandle(hdib);
		if (hdib == 0) {
			return FALSE;
		}
		HPALETTE hpalette = HPALETTE(GetClipboardData(CF_PALETTE));
		if (hpalette != 0) {
			hpalette = CopyPalette(hpalette);
		}
		else {
			hpalette = CreateDIBPalette(hdib);
		}
		clean();
		hDib = hdib;
		hBitmap = 0;
		setPalette(hpalette);
		haveValidBitmap = 1;
		//
		// ToDo! Temporary hack till I'm able to
		// figure how to get DIBBlt() to work correctly.
		//
		hBitmap = getBitmap();
		CloseClipboard();
		return TRUE;
	}

	HBITMAP hbitmap;

	if ((hbitmap = HBITMAP(GetClipboardData(CF_BITMAP))) != 0) {
		HPALETTE hpalette = HPALETTE(GetClipboardData(CF_PALETTE));
		if (hpalette != 0) {
			hpalette = CopyPalette(hpalette);
		}
		else {
			hpalette = HPALETTE(GetStockObject(DEFAULT_PALETTE));
		}
		hdib = DIBFromBitmap(hbitmap, BI_RGB, 0, hpalette);
		if (hdib != 0) {
			clean();
			hDib = hdib;
			hBitmap = hbitmap;
			setPalette(hpalette);
			haveValidBitmap = 1;
			CloseClipboard();
			return TRUE;
		}
		else {
			DeleteObject(hbitmap);
		}
	}
	CloseClipboard();
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VBitMap::copyToClipboard()
{
	if (!haveValidBitmap) {
		return FALSE;
	}

	VWindow *window = notifier->getFocus();
	if (window == 0) {
		return FALSE;
	}

	HWND hwnd = window->hwnd();
	if (hwnd == 0) {
		return FALSE;
	}

	if (!OpenClipboard(hwnd)) {
		return FALSE;
	}

	EmptyClipboard();

#if 1
	HANDLE hdata;
	HPALETTE hpalette = getPalette();
	HBITMAP hbitmap = getBitmap();
	if ((hdata = RenderFormat(CF_DIB, hbitmap, hDib, hpalette)) != 0) {
		SetClipboardData(CF_DIB, hdata);
	}
	if ((hdata = RenderFormat(CF_PALETTE, hbitmap, hDib, hpalette)) != 0) {
		SetClipboardData(CF_PALETTE, hdata);
	}
	if ((hdata = RenderFormat(CF_BITMAP, hbitmap, hDib, hpalette)) != 0) {
		SetClipboardData(CF_BITMAP, hdata);
	}
#else
	SetClipboardData(CF_DIB,     NULL);
	SetClipboardData(CF_BITMAP,  NULL);
	SetClipboardData(CF_PALETTE, NULL);
#endif

	CloseClipboard();
	return TRUE;

#if 0
	case WM_RENDERALLFORMATS:
		// Ensure that clipboard data can be
		// rendered even though app is being destroyed.
		SendMessage(hWnd,WM_RENDERFORMAT, CF_DIB, 0L);
		SendMessage(hWnd,WM_RENDERFORMAT, CF_BITMAP, 0L);
		SendMessage(hWnd,WM_RENDERFORMAT, CF_PALETTE, 0L);
		break;
	case WM_RENDERFORMAT:
		// Format data in manner specified
		// and pass the data handle to clipboard.
		if (h = RenderFormat(wParam))
			SetClipboardData((WORD)wParam, h);
		break;
#endif

}

// ==========================================================================
// Archiving
// ==========================================================================

#ifndef CV_NOARCHIVER

// --------------------------------------------------------------------------
//
void VBitMap::putTo(VArchiver &a)
{
	VDisplay::putTo(a);

	a << VBitMapArchiveTag;

	const long reserved = 0;
	a << reserved;
	a << reserved;

	a << frameCount;
	a << sequenceLength;	

	VString filename;
	a.putObject(&filename);

	// Write the DIB.

	if (haveValidBitmap) {
		a << TRUE;
		FileOrArchive foa(&a);
		saveFile(foa, getBitCount(), getCompression());
	}
	else {
		a << FALSE;
	}
}

// --------------------------------------------------------------------------
//
void VBitMap::getFrom(VArchiver& a)
{
	VDisplay::getFrom(a);

	long tag; a >> tag;
	if (!archiveTagOk(tag, VBitMapArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VBitMapArchiveTag)); return;
	}

	long reserved;
	a >> reserved;
	a >> reserved;	

	a >> frameCount;
	a >> sequenceLength;

	VString filename;
	a.getObject(&filename);

	boolean dib;
	a >> dib;
	
	if (!dib) {
	 	return;
	}

	FileOrArchive foa(&a);
	loadFile(foa);
}

// --------------------------------------------------------------------------
//
void VBitMap::getFrom(VArchiver& a, VObject *, boolean)
{
	getFrom(a);
}

#endif // !CV_NOARCHIVER
