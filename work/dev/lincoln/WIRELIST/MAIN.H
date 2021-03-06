/***********************************************************************
* PROGRAM:	VALID wirelist program header
* FILE:		main.h
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

/*
 *  Macro Descriptions (alphabetical)
 *  --------------------------------------------------------------------
 *  dev_loc	Return in board geometry form, the location
 *		(string) of the given dev_tbl (pointer)
 *  --------------------------------------------------------------------
 *  dev_xcoord	Return the x coordinate (int)
 *		of the given dev_tbl (pointer)
 *  --------------------------------------------------------------------
 *  dev_ycoord	Return the y coordinate (int)
 *		of the given dev_tbl (pointer)
 *  --------------------------------------------------------------------
 *  dev_xpin	Return the x coordinate (int) of pin number
 *		`n' (int) of the given dev_tbl (pointer)
 *  --------------------------------------------------------------------
 *  dev_ypin	Return the y coordinate (int) of pin number
 *		`n' (int) of the given dev_tbl (pointer)
 *  --------------------------------------------------------------------
 *  is_baby	TRUE if the given dev_tbl (pointer) refers to a BABY
 *  --------------------------------------------------------------------
 *  is_cnn	TRUE if the given pin_list (pointer)
 *		refers to a connector pin, otherwise FALSE
 *  --------------------------------------------------------------------
 *  is_dev	TRUE if the given pin_list (pointer)
 *		refers to a device pin, otherwise FALSE
 *  --------------------------------------------------------------------
 *  is_dip	TRUE if the given dev_tbl (pointer) refers to a DIP
 *  --------------------------------------------------------------------
 *  is_gnd	TRUE if the given signal (string) is a ground signal
 *  --------------------------------------------------------------------
 *  is_sip	TRUE if the given dev_tbl (pointer) refers to a SIP
 *  --------------------------------------------------------------------
 *  is_vcc	TRUE if the given signal (string) is a voltage signal
 *  --------------------------------------------------------------------
 *  no_pins	TRUE if the given signal (string) has no pins attached
 *  --------------------------------------------------------------------
 *  pin_loc	Return in board geometry form, the
 *		location (string) of the given pin (pointer)
 *  --------------------------------------------------------------------
 *  pin_ref	Return the reference designator name
 *		(string) of the given pin_list (pointer)
 *  --------------------------------------------------------------------
 *  pin_xcoord	Return the x coordinate (int)
 *		of the given pin_list (pointer)
 *  --------------------------------------------------------------------
 *  pin_ycoord	Return the y coordinate (int)
 *		of the given pin_list (pointer)
 *  --------------------------------------------------------------------
 *  str_eq	TRUE if the given two strings are equal, else FALSE
 *  --------------------------------------------------------------------
 *  strn_eq	TRUE if the first n characters of the
 *		given two strings are equal, else FALSE
 *  --------------------------------------------------------------------
 */

/*
 *	Global Constants
 */

#define MAXPKG		30	/* max number of pkg_tbl structures */

/*
 *	String/Buffer lengths
 */

#define REFLEN		24	/* max length of reference designator */
#define DEVLEN		24	/* max length of device type name */
#define PKGLEN		24	/* max length of package name */
#define LSTLEN		10	/* wirelist field width */
#define SIGLEN		LSTLEN	/* max signal name length */
#define BGLLEN		LSTLEN	/* max board geometry loc string length */
#define STR_BUFSIZE	257	/* miscellaneous char buffer size */
#define FNAME_BUFSIZE	110	/* max length of file name (incl. path) */

/*
 *	Miscellaneous macros
 */

#define str_eq(a,b)	(strcmp((a),(b)) == 0)
#define strn_eq(a,b,n)	(strncmp((a),(b),(n)) == 0)

/*
 *	Fatal error codes
 */

#define ERROR		(-1)	/* file syntax */
#define EFILE		(-2)	/* file acces */
#define ENOMEM		(-3)	/* no available memory */
#define ENOBOARD	(-4)	/* unknown board type */

/*
 *	Warning error codes (OR'ed together)
 */

#define EGEDSYN		00001	/* VALID ged file error */
#define EXNETSYN	00002	/* VALID net list file error */
#define EPGNDSYN	00004	/* VALID pwr/gnd list file error */
#define EPKGSYN		00010	/* package description error */
#define EPKGDESC	00020	/* package description error */
#define EDEVLOC		00040	/* device location error */
#define EWIRELST	00100	/* wirelist error */

/*
 *	Device Related Macros
 */

#define IS_CNN		0	/* connector */
#define IS_SIP		1	/* single in-line package */
#define IS_DIP		2	/* dual in-line package */
#define IS_BABY		3	/* baby board */

#define dev_loc(d)	((Bgeom->b_boardloc)(dev_xcoord(d), dev_ycoord(d)))
#define dev_xcoord(d)	((d)->d_xcoord)
#define dev_ycoord(d)	((d)->d_ycoord)
#define dev_xpin(d,n)	(dev_xcoord(d) + (d)->d_pkg->pk_pin[(n) - 1].po_xoff)
#define dev_ypin(d,n)	(dev_ycoord(d) + (d)->d_pkg->pk_pin[(n) - 1].po_yoff)
#define is_baby(devp)	((devp)->d_desc->pk_type == IS_BABY)
#define is_dip(devp)	((devp)->d_desc->pk_type == IS_DIP)
#define is_sip(devp)	((devp)->d_desc->pk_type == IS_SIP)
#define is_cnn(pin)	((pin)->p_type == IS_CNN)
#define is_dev(pin)	(pin->p_type == IS_SIP || \
			 pin->p_type == IS_DIP || \
			 pin->p_type == IS_BABY)

/*
 *	Pin Related Macros
 */

#define pin_loc(p)	((Bgeom->b_boardloc)(pin_xcoord(p), pin_ycoord(p)))
#define pin_ref(pin)	(is_cnn((pin)) ? (pin)->p_owner.cnn->c_lstdes \
			 : (pin)->p_owner.dev->d_refdes)
#define pin_xcoord(p)	((p)->p_owner.dev->d_xcoord + \
			 (p)->p_owner.dev->d_pkg->pk_pin[(p)->p_pin-1].po_xoff)
#define pin_ycoord(p)	((p)->p_owner.dev->d_ycoord + \
			 (p)->p_owner.dev->d_pkg->pk_pin[(p)->p_pin-1].po_yoff)

/*
 *	Signal Related Macros
 */

#define GND_SIG		"GND"
#define VCC_SIG		"VCC"

#define is_gnd(s)	(strncmp((s),GND_SIG,3) == 0 || \
			 strcmp((s),"\'0\'") == 0 || \
			 strcmp((s),"\'GND\'") == 0)
#define is_vcc(s)	(strncmp((s),VCC_SIG,3) == 0 || \
			 strcmp((s),"\'VCC\'") == 0)
#define no_pins(s)	(strcmp((s),"NC") == 0)

/*
 *  Data Structures
 *
 *  `DEVICE' is used as a table (linked list) of designer defined devices,
 *	and is filled in using the VALID ged file.  Each `DEVICE' entry in
 *	the table (defined in WIRELIST), contains the device's reference
 *	designator, a pointer to an entry in the device package description
 *	table (PACKAGE), the xy coordinates on the board of the device's
 *	pin number one in VALID board form (with the origin subracted out),
 *	that is, (x,y) represent real board distances measured in 1000ths
 *	of an inch), and also the device's location in board geometry form
 *	(string), a per device ground pin list, and a per device voltage
 *	pin list.
 *
 *  `NET' is used as a table (doubly linked list) of defined nets,
 *	and is filled in using the expanded netlist file.  Each `NET'
 *	entry has a signal name, a pointer to a linked list (PIN) of
 *	pins to be wrapped, and pointers for the doubly linked list.
 *
 *  `PIN' is used as a list (singly linked) of device pins to be wrapped.
 *	Each entry contains a pointer to a DEVICE table entry, a number
 *	`p_pin' corresponding to the pin number of the pointed to device
 *	`p_dev' (or connector `p_cnn') to be wrapped, and a link to the
 *	next entry.
 *
 *  `PACKAGE' is used as a table (array) of device package descriptions,
 *	one per possible device package (e.g. DIP8), and is filled in
 *	(in pkg_desc()/package.c) using the apporopriate package description
 *	file (given by pkg_file()/package.c).  Each entry contains the
 *	device type (e.g. IS_DIP), its size (number of pins), and a pointer
 *	to an array of (dynamically allocated) pin offset structures which
 *	contains the relative (to pin 1 in 1000ths inch) pin coordinates for
 *	each pin of the package.
 *
 *  `PINOFF' is used as a list (array) of relative pin offsets for each pin
 *	of a given device package (e.g. DIP40).  There is one structure in
 *	the array for each pin on the package. The first structure corresponds
 *	to pin number one of the package and its x,y offsets are always (0,0);
 *	the following structures are the x,y coordinates of each succesive pin
 *	relative to (0,0) measured in 1000ths of an inch.  For example, a
 *	typical 8 pin DIP would have the entries:
 *	(0,0) (100,0) (200,0) (300,0) (300,300) (200,300) (100,300) (0,300)
 */

typedef struct pin_off {
   unsigned int		po_xoff;	/* x VALID offset (1000ths in.) */
   unsigned int		po_yoff;	/* y VALID offset (1000ths in.) */
} PINOFF;

typedef struct pkg_tbl {
   char		   pk_name[PKGLEN+1];	/* VALID package name */
   int		   pk_type;		/* package type, e.g. DIP, SIP */ 
   unsigned int	   pk_npins;		/* package size (# of pins) */ 
   struct pin_off *pk_pin;		/* ptr to relative pin offsets */
} PACKAGE;

typedef struct pin_list {		/* list of pins to be wrapped */
   int		 	 p_type;	/* type of this pin's owner */
   union {				/* the owner of this pin; it is	*/
	 struct dev_tbl     *dev;	/*	--> a device,	OR	*/
	 struct cnn_tbl	    *cnn;	/*	--> a connector.	*/
   }		 	 p_owner;	/* see macros is_cnn & is_dev */
   unsigned int	 	 p_pin;		/* pin number (to be wrapped) */
   struct pin_list	*p_next;	/* pointer to next in the list */
} PIN;

typedef struct dev_tbl {
  char		   d_refdes[REFLEN+1];	/* ref. designator, e.g. U4 */ 
  char		   d_name[DEVLEN+1];	/* device name e.g. LS04 */
  struct pkg_tbl  *d_pkg;		/* ptr to package description */
  struct pin_list *d_gndlist;		/* per device ground pin list */ 
  struct pin_list *d_vcclist;		/* per device voltage pin list */ 
  int	 	   d_xcoord;		/* x,y coordinates of devices */
  int	 	   d_ycoord;		/*     on VALID ged drawing */ 
  struct dev_tbl  *d_next;		/* pointer to next node in list */
} DEVICE;

typedef struct cnn_tbl {
   char		   *c_refdes;		/* VALID name of connector */
   char		   *c_lstdes;		/* wirelist name of connector */
   char		   *c_name;		/* name of the connector */
   unsigned int	    c_npins;		/* number of pins on connector */
   struct pin_list *c_vcclist;		/* per connector vcc pin list */
   struct pin_list *c_gndlist;		/* per connector gnd pin list */
} CNN;

typedef struct net_tbl {
  char		  *n_name;		/* VALID physical net name */
  char		   n_sig[SIGLEN+1];	/* wirelist signal name */
  struct pin_list *n_pinlist;		/* pin linked list */
  struct net_tbl  *n_prev;		/*   pointers (doub linked list) */
  struct net_tbl  *n_next;		/* next and prev net list node */
} NET;

typedef struct wire_list {
   struct dev_tbl   *w_dev;		/* device table (linked list) */
   struct pkg_tbl    w_pkg[MAXPKG];	/* package description table */
   struct net_tbl   *w_net;		/* net table (linked list) */
   unsigned int	     w_ndev;		/* number of devices defined */
   unsigned int	     w_npkg;		/* number of packages defined */
   unsigned int	     w_nnet;		/* number of nets defined */
   int		     w_bxorigin;	/* x VALID coord of board */
   int		     w_byorigin;	/* y VALID coord of board */
   int		     w_error;		/* error codes (OR'ed together) */
} WIRELIST;

typedef struct bgeom_tbl {
   char		  *b_name;		/* board geometry name */
   char		*(*b_boardloc)();	/* get geometry form location */
   char		*(*b_neargnd)();	/* find nearest gnd pin to board loc */
   char		*(*b_nearvcc)();	/* find nearest vcc pin to board loc */
   char		*(*b_cnngnd)();		/* find nearest gnd pin to connector */
   char		*(*b_cnnvcc)();		/* find nearest vcc pin to connector */
   bool		 (*b_okdevloc)();	/* check device/pin locations */
   struct cnn_tbl *b_cnn;		/* the per-board connector table */
} BOARD;
