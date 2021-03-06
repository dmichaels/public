/***********************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program
* FILE:		ged_make.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		May 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

/*
 *  ged_make
 *
 *  Make entries in the graphics file for each of the devices in
 *  the device table `layout->l_dev` so that they (the devices) will
 *  appear on the screen around the board when the user invokes ged.
 */

ged_make(f,layout)
register FILE *f;
register LAYOUT *layout;
{
	bool			find_place();
	void			init_board(), place_dev();
	char			*pt, *point();
	register DEVICE		*d;
	register PACKAGE	*p;
	int			x, y, xboard, yboard;
	register bool		first = TRUE;

#ifdef DEBUG
	fprintf(stderr,
	"ged_make: \"%s\" (%d %d ; %d %d)\n",
	layout->l_bgeom->b_name,layout->l_bgeom->b_len,layout.l_bgeom->b_wid,
	layout->l_bgeom->b_xoff,layout->l_bgeom->b_yoff);
#endif DEBUG

	init_board(layout->l_bgeom->b_len, layout->l_bgeom->b_wid,
		   &xboard, &yboard);

#ifdef DEBUG
	fprintf(stderr,"ged_make: init board - (%d %d)\n",xboard,yboard);
#endif DEBUG

	fprintf(f,
	" FILE_TYPE = MACRO_DRAWING;\n");

	pt = point(valid_coord(xboard), valid_coord(yboard));

	fprintf(f,
	"FORCEADD %s..1\n%s\n", layout->l_bgeom->b_name, pt);

	fprintf(f,
	"FORCEPROP 1 LAST REF_PIN %d %d\n%s\n",
	layout->l_bgeom->b_xoff, layout->l_bgeom->b_yoff, pt);

	fprintf(f,
	"DISPLAY I %s\n", pt);

	for(p = layout->l_pkg ; p != NULL ; p = p->pk_next) {
	   for(d = p->pk_devlist ; d != NULL ; d = d->d_pkgnext) {

		if(find_place(dev_len(d), dev_wid(d), &x, &y) == FALSE) {

			/*
			 *  If there is no more room,
			 *  then put the packages ON
			 *  the board (slovenly).
			 */

			static int n = 5;

			x = xboard + n;
			y = yboard + n;
			n += 2;
		}
		else
			place_dev(dev_len(d), dev_wid(d), x, y);

		/*
		 *  FORCEADD <package_name>..1
		 *  <point>
		 *  FORCEPROP 1 LAST REFDES <reference_designator>
		 *  <point>
		 *  DISPLAY <refdes_display_value>
		 *  <point>
		 *  FORCEPROP 1 LAST CHIP <part_name>
		 *  <point>
		 *  DISPLAY <chip_display_value>
		 *  <point>
		 */

		fprintf(f,
		"FORCEADD %s..1\n%s\n",
		dev_pkgname(d), point(valid_coord(x),valid_coord(y)));

		pt = point(valid_coord(x) + p->pk_refdes.pr_xoff,
			   valid_coord(y) + p->pk_refdes.pr_yoff);

		fprintf(f,
		"FORCEPROP 1 LAST REFDES %s\n%s\n", d->d_refdes, pt);

		/*
		 *  If the DISPLAY value is "-",
		 *  then skip it.
		 */

		if(!str_eq(p->pk_refdes.pr_disp,"-")) {
			fprintf(f,
			"DISPLAY %s %s\n", p->pk_refdes.pr_disp, pt);
		}

		pt = point(valid_coord(x) + p->pk_chip.pr_xoff,
			   valid_coord(y) + p->pk_chip.pr_yoff);

		fprintf(f,
		"FORCEPROP 1 LAST CHIP %s\n%s\n", d->d_name, pt);

		/*
		 *  If the DISPLAY value is "-",
		 *  then skip it.
		 */

		if(!str_eq(p->pk_chip.pr_disp,"-")) {
			fprintf(f,
			"DISPLAY %s %s\n", p->pk_chip.pr_disp, pt);
		}

		layout->l_nplunk++;
	   }
	}

	fprintf(f,"GRID .100 5 OFF\n");
	fprintf(f,"QUIT\n");
}

/*
 *  point	(local)
 *
 *  Return a string in proper VALID logic drawing format
 *  of a point.  Format:   "(x_coord y_coord);"
 */

static char *
point(x,y)
register int x, y;
{
	static char p[20];

	sprintf(p, "(%d %d);", x, y);
	return(p);
}
