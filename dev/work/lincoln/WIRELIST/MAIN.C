/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		main.c
* AUTHOR:	David Michaels (david@ll-sst)
*		M.I.T. Lincoln Laboratory, Group 24
* HISTORY:
*		* December 1983 -- first version.  david
*		* April 1984 -- complete rewrite: more modular;
*		  able to handle multiple board geometries using
*		  the BOARD switch table, etc...  david.
*		* 28 August 1984 -- added "pr_destbl" function to
*		  produce a file with just a reference designator
*		  table (requested by Don Malpass).  david.
*		* 6 September 1984 -- created "valid.c", and "valid.h"
*		  to contain all relevant VALID file names.  Also,
*		  made minor coding improvements.  david.
*		* December 1984 -- created a VALID library of functions
*		  to get VALID file & directory names.
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include "valid.h"
#include <stdio.h>

/*
 *  USAGE
 *	wirelist board_name
 *
 *  DESCRIPTION
 *	Formats a wirelist according to the conventions of
 *	the Lincoln Laboratory wirewrap service (Group 72).
 *
 *  INPUT FILES (from the VALID post-processor):
 *
 *	Graphics file		board_name/logic.1.1
 *	Expanded net list file	pstxnet.dat
 *	Power/ground list file	pstgnd.dat
 *
 *  OUTPUT FILES
 *
 *	Wirelist file		board_name.lst
 *	Reference file		board_name.ref
 *	Designator list file	board_name.des
 *	Error file		board_name.err
 */

/***********************************************************************
*			GLOBALLY GLOBAL
***********************************************************************/

char	Gedfile[FNAME_BUFSIZE];	 /* graphics file name */
char	Xnetfile[FNAME_BUFSIZE]; /* netlist file name */
char	Pgndfile[FNAME_BUFSIZE]; /* power/ground list file name */
FILE	*Errfp;			 /* error file ptr */
FILE	*Reffp;			 /* reference file ptr */
BOARD	*Bgeom;			 /* board geometry switch table ptr */

/***********************************************************************
*			LOCALLLY GLOBAL
***********************************************************************/

static char	lst_file[FNAME_BUFSIZE];   /* final wirelist */
static char	err_file[FNAME_BUFSIZE];   /* error log file name */
static char	ref_file[FNAME_BUFSIZE];   /* output reference file */
static char	des_file[FNAME_BUFSIZE];   /* refdes list file */
static char	board_name[FNAME_BUFSIZE]; /* board_name argument */
static FILE	*lst_fp;		   /* wirelist file ptr */
static FILE	*des_fp;		   /* refdes list file ptr */
static FILE	*ged_fp;		   /* graphics file ptr */
static FILE	*net_fp;		   /* net list file ptr */
static FILE	*pwr_fp;		   /* pwr/gnd list file ptr */

/**********************************************************************
*			FUNCTIONS
**********************************************************************/

main(ac,av)
char	*av[];
int	ac;
{
	void	setup(), lets_do_it(), fatal(), usage();
	WIRELIST w;

	if(ac != 2)
		usage();
	strcpy(board_name,av[1]);
	setup(&w);

	/* Open all necessary files */

	if((net_fp = fopen(Xnetfile,"r")) == NULL)
		fatal(EFILE,Xnetfile);

	if((ged_fp = fopen(Gedfile,"r")) == NULL)
		fatal(EFILE,Gedfile);

	if((pwr_fp = fopen(Pgndfile,"r")) == NULL)
		fatal(EFILE,Pgndfile);

	if((lst_fp = fopen(lst_file,"w")) == NULL)
		fatal(EFILE,lst_file);

	if((des_fp = fopen(des_file,"w")) == NULL)
		fatal(EFILE,des_file);

	if((Reffp = fopen(ref_file,"w")) == NULL)
		fatal(EFILE,ref_file);

	if((Errfp = fopen(err_file,"w")) == NULL)
		fatal(EFILE,ref_file);

	/*  Do unbuffered error reporting */

	setbuf(Errfp,NULL);

	lets_do_it(&w);
}

/*
 *  lets_do_it	(local)
 *
 *  Do it!
 */

static void
lets_do_it(w)
WIRELIST *w;
{
	unsigned int ck_error();
	void	wirelist();
	void	ged_parse(), xnet_parse();
	void	pgnd_parse(), mv_pwrpins();
	void	pr_devtbl(), pr_destbl();
	void	pr_nettbl(), pr_netmap(), pr_pwrpins();
	long	time();
	long	t = time(0);

	/* Parse graphics file */

	fprintf(stderr,"** Parsing graphics file \"%s\"\n",Gedfile);
	ged_parse(ged_fp,w);
	fclose(ged_fp);

	/*
	 *  Check the legality
	 *  of device locations.
	 */

	fprintf(stderr,"** Checking device/pin locations\n");
	if((Bgeom->b_okdevloc)(w->w_dev) == FALSE) {
		pr_devtbl(Reffp,w);
		fatal(ERROR,"illegal device/pin location(s)");
	}

	/* Parse expanded net list file */

	fprintf(stderr,"** Parsing expanded net list file \"%s\"\n",Xnetfile);
	xnet_parse(net_fp,w);
	fclose(net_fp);

	/* Parse power/ground list file */

	fprintf(stderr,"** Parsing power/ground list file \"%s\"\n",Pgndfile);
	pgnd_parse(pwr_fp,w);
	fclose(pwr_fp);

	/* Make reference designator list file */

	pr_destbl(des_fp,w);

	/* Make reference file */

	fputs("********************************************************\n",
	Reffp);
	fprintf(Reffp,
	"VALID wirelist reference file\nDATE:  %s",ctime(&t));
	fprintf(Reffp,
	"BOARD: %s    [%s (%d %d)]\n",
	board_name,Bgeom->b_name,w->w_bxorigin,w->w_byorigin);
	fputs("********************************************************\n",
	Reffp);

	pr_devtbl(Reffp,w);
	pr_pwrpins(Reffp,w);
	mv_pwrpins(w);
	pr_nettbl(Reffp,w);
	pr_netmap(Reffp,w);
	fclose(Reffp);

	/* Make the final wirelist */

	wirelist(lst_fp,w);
	fclose(lst_fp);

	if(ck_error(w->w_error) > 0)
		fprintf(stderr,"**\n** %-25s\"%s\"\n","Error file:",err_file);
	else {
		unlink(err_file);
		fputs("**\n",stderr);
	}
	fprintf(stderr,"** %-25s\"%s\"\n","Wirelist file:",lst_file);
	fprintf(stderr,"** %-25s\"%s\"\n","Reference file:",ref_file);
	fprintf(stderr,"** %-25s\"%s\"\n","Designator list file:",des_file);
}

/*
 *  setup	(local)
 *
 *  Set things up
 */

static void
setup(w)
register WIRELIST *w;
{	
	/* Make all necessary file names */

	strcpy(Gedfile,vf_ged(board_name));
	strcpy(Xnetfile,vf_xnet());
	strcpy(Pgndfile,vf_pgnd());
	sprintf(lst_file,"%s.lst",board_name);
	sprintf(ref_file,"%s.ref",board_name);
	sprintf(des_file,"%s.des",board_name);
	sprintf(err_file,"%s.err",board_name);

	/* Initialize the wirelist */

	w->w_net = NULL;
	w->w_dev = NULL;
	w->w_nnet = 0;
	w->w_ndev = 0;
	w->w_npkg = 0;
}

/*
 *  usage	(local)
 *
 *  Print usage and die.
 */

static void
usage()
{
	fprintf(stderr,"usage: wirelist board_name\n");
	exit(1);
}
