/***********************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program
* FILE:		main.c
* AUTHOR:	David Michaels (david@ll-sst)
*		M.I.T. Lincoln Laborary, Group 24
* HISTORY:
*		* May 1984 -- first version.  david
*		* October 1984 -- misc. code improvements.  david.
*		* December 1984 -- changed xprt_parse.c to ignore
*		  "PART_NAME=..." lines which screws things up.
*		  This parsing business should really be cleaned up.
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include "valid.h"
#include <stdio.h>

/*
 *  NAME
 *	layout - VALID IC Package layout program
 *
 *  SYNOPSIS
 *	layout board_name board_geometry
 *	layout [-p parts_list] board_name board_geometry
 *	layout [-p parts_list] -g graphics_file -r ref_file board_geometry
 *
 *  INPUT FILES (default)
 *	VALID Parts List	pstxprt.dat
 *	Part/Package Map File	/u0/lincoln/layout/part_map
 *
 *  OUTPUT FILES (default)
 *	VALID Graphics File	board_name/logic.1.1
 *	Reference File		board_name.lay
 */

/***********************************************************************
*			GLOBALLY GLOBAL
***********************************************************************/

/***********************************************************************
*			LOCALLY GLOBAL
***********************************************************************/

static char	boardname[FNAME_BUFSIZE+1]; /* board name */
static char	ged_file[FNAME_BUFSIZE+1];  /* graphics file name */
static char	prt_file[FNAME_BUFSIZE+1];  /* stuff list file name */
static char	ref_file[FNAME_BUFSIZE+1];  /* reference file name */
static char	scald_dir[FNAME_BUFSIZE+1]; /* scald directory name */
static FILE	*map_fp;		    /* map file ptr */
static FILE	*prt_fp;		    /* parts file ptr */
static FILE	*ged_fp;		    /* graphics file ptr */
static FILE	*ref_fp;		    /* reference file ptr */

/**********************************************************************
*			FUNCTIONS
**********************************************************************/

main(ac,av)
int ac;
char *av[];
{
	void setup(), lets_do_it();
	LAYOUT layout;
	FILE *f;
	char line[STR_BUFSIZE], *gets();

	setup(&layout,ac,av);

	printf("** Making GED layout file \"%s\" for \"%s\".\n",ged_file,
	layout.l_bgeom->b_name,layout.l_bgeom->b_len,layout.l_bgeom->b_wid);

	if((map_fp = fopen(vf_prtmap(),"r")) == NULL)
		fatal(EFILE,vf_prtmap());

	if((prt_fp = fopen(prt_file,"r")) == NULL)
		fatal(EFILE,prt_file);

	if((ged_fp = fopen(ged_file,"r")) != NULL) {
		fclose(ged_fp);
		printf("** Overwrite graphics file \"%s\"? (y/n) ",ged_file);
		gets(line);
		if(line[0] != 'y' && line[0] != 'Y')
			exit(0);
	}
	if((ged_fp = fopen(ged_file,"w")) == NULL)
		fatal(EFILE,ged_file);

	if((ref_fp = fopen(ref_file,"w")) == NULL)
		fatal(EFILE,ref_file);

	lets_do_it(&layout);

	/* Make entry in the SCALD directory file */

	printf("** Enter your Scald directory name (e.g. username.wrk): ");
	scanf("%s",line);
	strncpy(scald_dir,line,FNAME_BUFSIZE);
	if(scald_entry(scald_dir,boardname) == -1)
		printf("** Scald directory (%s) entry not made.\n",scald_dir);
	else
		printf("** Scald directory (%s) entry made.\n",scald_dir);
	printf("** Done (%d devices plunked).\n",layout.l_nplunk);
}

/*
 *  lets_do_it	(local)
 *
 *  Do it!
 */

static void
lets_do_it(layout)
LAYOUT *layout;
{
	void	pr_devtbl(), pr_pkgtbl();
	int	xprt_parse(), map_parse(), ged_make();
	long	t = time(0);

	/* Parse PART_FILE */

	printf("** Parsing \"%s\".\n",prt_file);
	xprt_parse(prt_fp,layout);
	fclose(prt_fp);

	/* Parse vf_prtmap */

	printf("** Parsing \"%s\".\n",vf_prtmap());
	map_parse(map_fp,layout);
	fclose(map_fp);

	/* Join device & package tables */

	link_dev(layout);

	/* Make reference file */

	printf("** Making reference file \"%s\".\n",ref_file);
	fputs("********************************************************\n",
	ref_fp);
	fprintf(ref_fp, "VALID ged layout program reference file\n");
	fprintf(ref_fp, "DATE: %s",ctime(&t));
	fprintf(ref_fp, "BOARD: %s\n",boardname,layout->l_bgeom->b_name);
	fputs("********************************************************\n",
	ref_fp);
	pr_devtbl(ref_fp,layout);
	pr_pkgtbl(ref_fp,layout);
	fclose(ref_fp);

	/* Make graphics file */

	printf("** Making graphics file \"%s\".\n",ged_file);
	ged_make(ged_fp,layout);
	fclose(ged_fp);
}

/*
 *  setup	(local)
 *
 *  Parse the command line arguments and set things up.
 */

static void
setup(layout,ac,av)
LAYOUT *layout;
register int  ac;
register char **av;
{
	void usage();
	char *strlower();
	BOARD *find_bgeom();
	bool gflg = FALSE, pflg = FALSE, rflg = FALSE;
	bool gotboard = FALSE;
	register int i;
	char errbuf[ERR_BUFSIZE];

	/*
	 *  Process the command line
	 */

	if(ac < 3)
		usage();

	if((layout->l_bgeom = find_bgeom(av[--ac])) == NULL) {
		sprintf(errbuf,"Unknown board geometry \"%s\"\n",av[ac]);
		fatal(errbuf);
		exit(1);
	}

	for(i = 1 ; i < ac ; i++) {
		if(av[i][0] == '-')
		  switch(av[i][1]) {
			case 'r':
				rflg = TRUE;
				strncpy(ref_file,av[++i],FNAME_BUFSIZE);
				break;
			case 'p':
				pflg = TRUE;
				strncpy(prt_file,av[++i],FNAME_BUFSIZE);
				break;
			case 'g':
				gflg = TRUE;
				strncpy(ged_file,av[++i],FNAME_BUFSIZE);
				break;
		  }
		else if(!gotboard) {
			strncpy(boardname,strlower(av[i]),FNAME_BUFSIZE);
			gotboard = TRUE;
		}
		else
			usage();
	}

	/*
	 *  If the board name was not given then both
	 *  the 'r' flag and 'g' flag are required (i.e.
	 *  the reference file and the graphics file need to
	 *  be specified) since the board name is needed to
	 *  create the reference and graphics file names.
	 */

	if(!gotboard && !(rflg && gflg))
		usage();

	if(!rflg)
		sprintf(ref_file,"%s.lay",boardname);
	if(!pflg)
		strncpy(prt_file,vf_xprt(),FNAME_BUFSIZE);
	if(!gflg)
		strcpy(ged_file,vf_ged(boardname));

	/*
	 *  Initialize the layout structure
	 */

	layout->l_dev = NULL;
	layout->l_pkg = NULL;
	layout->l_ndev = 0;
	layout->l_npkg = 0;
	layout->l_nplunk = 0;
}

/*
 *  scald_entry		(local)
 *
 *  Make an entry in the given SCALD directory file (name)
 *  for the given drawing (directory name).
 *  The scald direcory file has the form:
 *  -------------------------------------
 *	FILE_TYPE = LOGIC_DIR;
 *	"NAME" 'name';
 *	"NAME1" 'name1';
 *	"NAME2" 'name2';
 *	END.
 *  -------------------------------------
 *  If the file `scald' does not exist, then create one with that name
 *  with the preceding format.  Return 0 if all is well, -1 otherwise.
 */

static int
scald_entry(scald,entry)
char *scald;
char *entry;
{
	char *sindex(), *mktemp();
	FILE *fopen();
	char *tmpname, line[STR_BUFSIZE], buf1[STR_BUFSIZE], buf2[STR_BUFSIZE]; 
	char *errbuf[ERR_BUFSIZE];
	register FILE *f, *tmp;
	register char c;
	bool empty = TRUE, done = FALSE;

	if((f = fopen(scald,"r")) == NULL) {
		if((f = fopen(scald,"w")) == NULL)
			return(-1);
	}
	if((tmp = fopen(tmpname = mktemp("/tmp/layoutXXXXXX"),"w")) == NULL) {
		sprintf(errbuf,"Can't open tmp file \"%s\"\n",tmpname);
		error(errbuf);
		return(-1);
	}
	while(fgets(line,STR_BUFSIZE,f) != NULL) {
		if(sindex(line,"END.",1) != NULL) {
			strcpy(buf1,entry);
			strcpy(buf2,entry);
			strupper(buf1);
			strlower(buf2);
			fprintf(tmp, "\"%s\" '%s';\n",buf1,buf2);
			done = TRUE;
		}
		fputs(line,tmp);
		empty = FALSE;
	}
	if(empty)
		fputs("FILE_TYPE = LOGIC_DIR;\n",tmp);
	if(!done) {
		strcpy(buf1,entry);
		strcpy(buf2,entry);
		strupper(buf1);
		strlower(buf2);
		fprintf(tmp, "\"%s\" '%s';\nEND.\n", buf1,buf2);
	}
	fclose(tmp);

	if((tmp = fopen(tmpname,"r")) == NULL)
		return(-1);
	if((f = fopen(scald,"w")) == NULL)
		return(-1);
	while((c = fgetc(tmp)) != EOF)
		fputc(c,f);
	fclose(f);
	fclose (tmp);
	unlink(tmpname);
	return(0);
}

/*
 *  usage	(local)
 *
 *  Give usage and die.
 */

static void
usage()
{
	fprintf(stderr,
	"layout [-p parts_list] board_name board_geom\n");
	fprintf(stderr,
	"layout [-p parts_list] -g graphics_file -r ref_file board_geom\n");
	exit(1);
}
