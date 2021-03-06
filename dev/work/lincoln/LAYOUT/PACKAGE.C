/******************************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program
* FILE:		package.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		May 1984
******************************************************************************/

#include "cdef.h"
#include "main.h"
#include "valid.h"
#include <stdio.h>

/*
 *  add_pkg
 *
 *  See if the package entry for the given package name is in the
 *  layout package table, if so return a pointer to it, if not then
 *  create a new entry and put it on the package table.  The table
 *  is kept ordered by the number of pins it has, SIPs go before DIPs
 *  which go before BABYs.
 */

PACKAGE *
add_pkg(pkgname,layout)
char *pkgname;
LAYOUT *layout;
{
	PACKAGE *find_pkg();
	PACKAGE *pkg, *new_pkg();
	register PACKAGE *p, *prev, *new;
	register int cmp;
	char errbuf[ERR_BUFSIZE];

	if((new = find_pkg(pkgname,layout)) != NULL)
		return(new);
	if((new = new_pkg(pkgname)) == NULL) {
		sprintf(errbuf,"Ignoring package \"%s\" (no description)",
		pkgname);
		error(errbuf);
		return(NULL);
	}
	if((p = layout->l_pkg) == NULL) {
		new->pk_next = NULL;
		layout->l_pkg = new;
		goto Out;
	}
	while(TRUE) {
		if(p == NULL) {
			prev->pk_next = new;
			new->pk_next = NULL;
			goto Out;
		}
		if((cmp = cmp_pkg(new,p)) < 0) { /* before p */
			new->pk_next = p;
			if(p == layout->l_pkg)	/* new first */
				layout->l_pkg = new;
			else
				prev->pk_next = new;
			goto Out;
		}
		if(cmp == 0)
			return(p);
		prev = p;
		p = p->pk_next;
	}
Out:
	layout->l_npkg++;
	return(new);
}

/*
 *  cmp_pkg	(local)
 *
 *  Compare two packages on the basis of total physical area.
 */

static int
cmp_pkg(p1,p2)
register PACKAGE *p1, *p2;
{
	return((p1->pk_len * p1->pk_wid) - (p2->pk_len * p2->pk_wid));
}

/*
 *  new_pkg	(local)
 *
 *  Allocate a new package table structure, read the package's
 *  description file and fill in the appropriate information.
 *  Translate measurements which are in 1000ths of an inch units
 *  into 10ths of an inch units.  Return a pointer to the new package
 *  entry or NULL if an error occurred.
 *  Package description file information format:
 *  -----------------------------------------------------------------------
 *  TYPE	<DIP, SIP, or BABY>
 *  NPINS	<number of pins>
 *  <some unused information>...
 *  LENGTH	<physical length of package (1000ths inch --> 10ths inch)>
 *  WIDTH	<physical width of package (1000ths inch --> 10ths inch)>
 *  PROP	<property name> <x offset> <y offset> <DISPLAY value>
 *  -----------------------------------------------------------------------
 *  To accommodate comments, if a '#' appears on a line,
 *  it and the whole rest of the line will be ignored.
 */

#define pkg_alloc()	((PACKAGE *)malloc(sizeof(PACKAGE)))

#define NWORDS	5	/* max number of words on a line */

static PACKAGE *
new_pkg(pkgname)
char *pkgname;
{
	int			wordvec();
	char			*vf_pkgfile(), *fgets(), *index();
	char			*descfile;
	register char		*wv[NWORDS], *p, line[STR_BUFSIZE];
	int			wc;
	register PACKAGE	*pkg;
	FILE			*f;
	bool			got_npins = FALSE;
	bool			got_length = FALSE;
	bool			got_width = FALSE;
	bool			got_type = FALSE;
	bool			got_refdes = FALSE;
	bool			got_chip = FALSE;
	char			errbuf[ERR_BUFSIZE];

	if((descfile = vf_pkgfile(pkgname)) == NULL) {
		sprintf(errbuf,"Can't access \"%s\" description file",pkgname);
		error(errbuf);
		return(NULL);
	}
	if((f = fopen(descfile,"r")) == NULL) {
		sprintf(errbuf,"Can't open \"%s\" (%s description)",
		descfile, pkgname);
		error(errbuf);
		return(NULL);
	}
	if((pkg = pkg_alloc()) == NULL)
		fatal(ENOMEM,"new package");
	while(fgets(line,STR_BUFSIZE,f)) {
	   if((p = index(line,'#')) != NULL)
		*p = EOS;
	   if((wc = wordvec(line,wv,NWORDS," \t\n")) <= 0)
		continue;
	   if(str_eq("TYPE",wv[0])) {
		if(wc < 2) {
			sprintf(errbuf,
			"Bad TYPE entry in \"%s\" (%s description)",
			descfile,pkgname);
			error(errbuf);
			goto Bad;
		}
		if(str_eq(wv[1],"DIP"))
			pkg->pk_type = IS_DIP;
		else if(str_eq(wv[1],"SIP"))
			pkg->pk_type = IS_SIP;
		else if(str_eq(wv[1],"BABY"))
			pkg->pk_type = IS_BABY;
		else {
			sprintf(errbuf,
			"Unknown TYPE in \"%s\" (%s description)",
			descfile,pkgname);
			error(errbuf);
			goto Bad;
		}
		got_type = TRUE;
	   }
	   else if(str_eq("NPINS",wv[0])) {
		if(wc < 2 || (pkg->pk_npins = atoi(wv[1])) <= 0) {
			sprintf(errbuf,"Bad NPINS in \"%s\" (%s description)",
			descfile,pkgname);
			error(errbuf);
			goto Bad;
		}
		got_npins = TRUE;
	   }
	   else if(str_eq("LENGTH",wv[0])) {
		if((pkg->pk_len = atoi(wv[1])) <= 0) {
			sprintf(errbuf,"Bad LENGTH in \"%s\" (%s description)",
			descfile,pkgname);
			error(errbuf);
			goto Bad;
		}
		pkg->pk_len = norm_coord(pkg->pk_len);
		got_length = TRUE;
	   }
	   else if(str_eq("WIDTH",wv[0])) {
		if((pkg->pk_wid = atoi(wv[1])) <= 0) {
			sprintf(errbuf,"Bad WIDTH in \"%s\" (%s description)",
			descfile,pkgname);
			error(errbuf);
			goto Bad;
		}
		pkg->pk_wid = norm_coord(pkg->pk_wid);
		got_width = TRUE;
	   }
	   else if(str_eq("PROP",wv[0])) {
		if(wc != 5) {
			sprintf(errbuf,"Bad PROP in \"%s\" (%s description)",
			descfile,pkgname);
			error(errbuf);
			goto Bad;
		}
		if(str_eq("REFDES",wv[1])) {
			pkg->pk_refdes.pr_xoff = atoi(wv[2]);
			pkg->pk_refdes.pr_yoff = atoi(wv[3]);
			strncpy(pkg->pk_refdes.pr_disp,wv[4],DISPLEN);
			pkg->pk_refdes.pr_disp[DISPLEN] = EOS;
			got_refdes = TRUE;
		}
		if(str_eq("CHIP",wv[1])) {
			pkg->pk_chip.pr_xoff = atoi(wv[2]);
			pkg->pk_chip.pr_yoff = atoi(wv[3]);
			strncpy(pkg->pk_chip.pr_disp,wv[4],DISPLEN);
			pkg->pk_chip.pr_disp[DISPLEN] = EOS;
			got_chip = TRUE;
		}
	   }
	}
	if(!got_type || !got_npins || !got_length || !got_width ||
	   !got_refdes || !got_chip) {
		sprintf(errbuf,"Bad format in \"%s\" (%s description)",
		descfile,pkgname);
		error(errbuf);
		goto Bad;
	}
	fclose(f);
	strcpy(pkg->pk_name,pkgname);
	pkg->pk_devlist = NULL;
	pkg->pk_next = NULL;
	return(pkg);
Bad:
	fclose(f);
	free(pkg);
	return(NULL);
	
}

/*
 *  find_pkg	(local)
 *
 *  Search the layout package table for the package with the given
 *  name, return a pointer to it if found, otherwise return NULL.
 */

static PACKAGE *
find_pkg(name,layout)
register char *name;
register LAYOUT *layout;
{
	register PACKAGE *pkg;

	for(pkg = layout->l_pkg ; pkg != NULL ; pkg = pkg->pk_next)
		if(str_eq(name,pkg->pk_name))
			return(pkg);
	return(NULL);
}
