/***********************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program.
* FILE:		main.h
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		May 1984
***********************************************************************/

/*
 *	String & Buffer Lengths
 */

#define FNAME_BUFSIZE	110	/* max length of file name (with path) */
#define REFLEN		20	/* max length of reference designator */
#define PRTLEN		20	/* max length of device part name */
#define PKGLEN		20	/* max length of device package name */
#define DISPLEN		20	/* max length of property DISPLAY value */
#define BGEOMLEN	20	/* max length of board geometry name */
#define BNAMELEN	20	/* max length of board name */
#define STR_BUFSIZE	257	/* for miscellaneous string buffers */
#define ERR_BUFSIZE	120	/* for error message */

/*
 *	Fatal Error Codes
 */

#define ERROR		(-1)	/* generic error */
#define ESYNTAX		(-2)	/* file syntax */
#define ENOMEM		(-5)	/* no available memory */
#define EFILE		(-6)	/* file access */

/*
 *	Miscellaneous Macros
 */

#define str_eq(a,b)	(strcmp((a),(b)) == 0)
#define strn_eq(a,b,n)	(strncmp((a),(b),(n)) == 0)

/*
 *	Device related macros
 */

#define dev_type(devp)		((devp)->d_pkg->pk_type)
#define dev_pkgname(devp)	((devp)->d_pkg->pk_name)
#define dev_npins(devp)		((devp)->d_pkg->pk_npins)
#define dev_len(devp)		((devp)->d_pkg->pk_len)
#define dev_wid(devp)		((devp)->d_pkg->pk_wid)

#define IS_BOARD		0
#define IS_SIP			1
#define IS_DIP			2
#define IS_BABY			3

#define is_board(devp)		((devp)->d_pkg->pk_type == IS_BOARD)
#define is_sip(devp)		((devp)->d_pkg->pk_type == IS_SIP)
#define is_dip(devp)		((devp)->d_pkg->pk_type == IS_DIP)
#define is_baby(devp)		((devp)->d_pkg->pk_type == IS_BABY)

/*
 *	VALID coordinates (on the graphics (logic) drawing are in
 *	units of 1000ths of an inch.  We will deal with units of
 *	10ths of an inch for our bit mapping routines.
 *
 *	(Maybe define:  norm_coord(n)  ((n + 99) / 100))
 */

#define norm_coord(n)	((n) / 100)	/* to normalize VALID coords */
#define valid_coord(n)	((n) * 100)	/* VALID coords are 1000ths in. */

/*
 *  Data Structures
 *
 *  `DEVICE' is a table (linked list) of defined devices, and is filled
 *	in  using the graphics file, the mapping file (device chip names
 *	to package names), and the part list file. Each `DEVICE' entry
 *	in the list (defined in LAYOUT), contains the device's reference
 *	designator, its chip name property, and a pointer to the
 *	appropriate package description structure `d_pkg' entry in the
 *	PACKAGE table.  Ordering is alphabetical by reference designator.
 *
 *  `PACKAGE' is a table (linked list) of device package descriptions,
 *	one per possible device package (i.e. DIP14...).  Each entry
 *	is filled in using information found in the file given by
 *	pkg_desc() (see package.c), and contains the package type,
 *	and its actual overall physical length and width measured
 *	in 1000ths of an inch, and a (linked) list of DEVICE structures
 *	associated with the the particular package type (this is a 
 *	pointer into the DEVICE list linked by the `d_pkgnext' field
 *	of the DEVICE structure).  Ordering is by physical package
 *	size (small to large, see package.c)
 *
 * `LAYOUT' is the structure which binds everything together, it
 *	contains the DEVICE table (linked list) ordered alphabetically
 *	by reference designator, the PACKAGE table (linked list) ordered
 *	by package size, and a count of entries in each.
 *
 *	Reference designators can thus be referenced in two ways:
 *
 *    1	Through the `l_dev' table in the LAYOUT structure; this will
 *	give us a (alphabetical) list of all the referenced designators
 *	defined, (along with there package name and description).
 *
 *    2	Through the PACKAGE device list `pk_devlist', which will give us
 *	a list (per package) of all the reference designators (devices)
 *	which are of that package type.
 */

typedef struct dev_tbl {
   char		    d_refdes[REFLEN+1];	/* reference designator e.g. U1 */
   char		    d_name[PRTLEN+1];	/* device part name, e.g. LS04 */
   struct pkg_tbl  *d_pkg;		/* pointer to PACKAGE table entry */
   struct dev_tbl  *d_next;		/* ptr to next in DEVICE table */
   struct dev_tbl  *d_pkgnext;		/* ptr to next in per PACKAGE list */
} DEVICE;

typedef struct prop_entry {
   char		pr_disp[DISPLEN+1];	/* DISPLAY value string */
   int		pr_xoff;		/* x offset of property */
   int		pr_yoff;		/* y offset of property */
} PROPERTY;

typedef struct pkg_tbl {
   char		   pk_name[PKGLEN+1];	/* package name, e.g. "DIP14" */
   int		   pk_type; 		/* package type, e.g. DIP, SIP */ 
   unsigned int	   pk_npins;		/* no. of pins on the package */
   unsigned int	   pk_len; 		/* package length (1000ths in.) */ 
   unsigned int	   pk_wid;		/* package width (1000ths in.) */
   PROPERTY	   pk_chip;		/* chip property */
   PROPERTY	   pk_refdes;		/* reference designator prop */
   struct dev_tbl *pk_devlist;		/* per package device list */
   struct pkg_tbl *pk_next;		/* pointer to next in pkg_tbl */
} PACKAGE;

typedef struct bgeom_tbl {
   char		*b_name;		/* board geometry name */
   unsigned int	b_len;			/* physical board length */
   unsigned int	b_wid;			/* physical board width */
   int	 	b_xoff;			/* x offset to pin 1 of board */
   int	 	b_yoff;			/* y offset to pin 1 of board */
} BOARD;

typedef struct {
   DEVICE	*l_dev;		 /* device table (ordered by refdes) */
   PACKAGE	*l_pkg;		 /* package table (ordered by size) */
   unsigned int	 l_ndev;	 /* number of devices */
   unsigned int	 l_npkg;	 /* number of packages */
   unsigned int	 l_nplunk;	 /* number devices put on the board */
   BOARD	*l_bgeom;	 /* board geometry table pointer */
} LAYOUT;
