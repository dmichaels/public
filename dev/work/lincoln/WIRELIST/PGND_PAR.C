/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		pwr_gnd.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

extern FILE	*Errfp;
extern char	Pgndfile[];

/*
 *  pgnd_parse
 *
 *  Parse a VALID Power and Ground List
 *
 *  This file is column formatted and is a real pain to parse.
 *
 *  The basic format is:
 *  -------------------------------------------------------------
 *  POWER AND GROUND LIST
 *  <header>
 *  DESIGNATOR     PART TYPE     GND      VCC     VBB     VEE
 *  <designator>   <dev name>    <pin>    <pin>   <pin>   <pin>
 *  ...	           ...           ...      ...     ...     ...
 *  ...	           ...           ...      ...     ...     ...
 *  ...	           ...           ...      ...     ...     ...
 *  END POWER AND GROUND LIST
 *  -------------------------------------------------------------
 *
 *  The first column (DESIGNATOR) must be present.  All the
 *  other (power labelled) columns are optional.  At the moment,
 *  only GND, VBB, VCC, VDD, VEE are recognized, but more can easily
 *  be added;  just put entry in the string array `Headers', and
 *  increment the MAXFIELD constant.
 *
 *  With this we want to add to the wirelist, a signal
 *  (GND, VCC, VBB etc.) wrapping a pin <pin> associated
 *  with some device referenced by the reference designator.
 *
 *  If a designator is missing from any row, the last mentioned
 *  designator is used (therefore, there must be a first designator).
 */

#define MAXLINE		100	/* maximum length of input line */
#define FWIDTH		20	/* maximum field width */
#define	MAXFIELD	7	/* maximum number of fields */

#define NO_FIELD	-1	/* non existent field flag */
#define LAST_FIELD	-1	/* last field on line flag */

typedef struct field_info {
	char	*f_header;		/* column header (Header below) */
	char	f_string[FWIDTH+1];	/* contents of field */
	int	f_position;		/* index of field (0 indexed) */
	int	f_width;		/* width of field (chars) */
} FIELD;

char	*Header[MAXFIELD] = {

#define	REFDES	0
			"DESIGNATOR"	/* REFDES - mandatory */
#define	PART	1
			,"PART"		/* PART */

			/*    The power pins	*/
#define PWR_PIN	2
			,"GND"		/* GND */
#define	GND	2
			,"VBB"		/* VBB */
#define	VBB	3
			,"VCC"		/* VCC */
#define	VCC	4
			,"VDD"		/* VDD */
#define	VDD	5
			,"VEE"		/* VEE */
#define	VEE	6

};

void
pgnd_parse(f,w)
FILE *f;
WIRELIST *w;
{
	void put_pins();
	unsigned int get_header();
	FIELD *field, *get_fields();
	char cur_desig[FWIDTH + 1];
	bool first = TRUE;

	if(get_header(f) == 0) {
		fprintf(Errfp,"Ignoring \"%s\" (no header)\n",Pgndfile);
		w->w_error |= EPGNDSYN;
		return;
	}
	while((field = get_fields()) != NULL) {
		if(first && field[REFDES].f_string[0] == EOF) {
			fprintf(Errfp,"Ignoring \"%s\" (no refdes)\n",Pgndfile);
			return;
		}
		if(field[REFDES].f_string[0] != EOS)
			strcpy(cur_desig,field[REFDES].f_string);
		put_pins(cur_desig,field,w);
		first = FALSE;
	}
}

/*
 *  put_pins	(local)
 *
 *  Add a new pin entry onto the wirelist for signal
 *  name `Header[i]' for each of the MAXFIELD pin number
 *  fields of reference designator `refdes'.
 */

static void
put_pins(refdes,field,w)
char *refdes;
FIELD *field;
WIRELIST *w;
{
	PIN *add_pin();
	char *sig;
	int pin;
	unsigned int i;

	for(i = PWR_PIN ; i < MAXFIELD ; i++) {
		if(field[i].f_position == NO_FIELD
		   || field[i].f_string[0] == EOS
		   || (pin = atoi(field[i].f_string)) <= 0)
			continue;

		if(i == GND)
			sig = GND_SIG;
		else if(i == VCC)
			sig = VCC_SIG;
		else
			sig = Header[i];
		if(add_pin(sig, pin, refdes, w) == NULL) {
			fprintf(Errfp,
			"Ignoring \"%s\" net: %s (%s)-%d\n",
			Pgndfile,sig,refdes,pin);
			w->w_error |= EPGNDSYN;
		}
	}
}

/*
 *  get_header	(local)
 *
 *  Find the power/ground list column headers (from `Header'),
 *  and fill in the FIELD structure.  Return the number of headers
 *  present.  Return 0 if any file format errors are encountered,
 *  e.g. if the mandatory designator header is not present.	
 */

#ifndef MIN
#define MIN(a,b)	((a) <= (b) ? (a) : (b))
#endif MIN

static FIELD	Field[MAXFIELD];
static char	*Tmpfile;
static FILE	*Tmpfp;

static
unsigned int
get_header(f)
FILE *f;
{
	FILE *fopen();
	void fdetab();
	char *mktemp(), *fgets();
	char line[MAXLINE], *p, *sindex();
	int nfields = 0;
	register unsigned int i, j, next;

	if((Tmpfp = fopen(Tmpfile = mktemp("/tmp/wireXXXXXX"),"w")) == NULL)
		fatal(EFILE,Tmpfile);
	fdetab(f,Tmpfp);
	rewind(f);
	fclose(Tmpfp);
	if((Tmpfp = fopen(Tmpfile,"r")) == NULL)
		fatal(EFILE,Tmpfile);

	/* Initialize */

	for(i = 0 ; i < MAXFIELD ; i++)
		Field[i].f_header = Header[i];

	while(TRUE) {
		if(fgets(line,MAXLINE,Tmpfp) == NULL) {
			fclose(Tmpfp);
			unlink(Tmpfile);
			return(0);
		}
		/*
		 *  Get the field positions
		 */
		for(i = 0 ; i < MAXFIELD ; i++) {
			if((p = sindex(line,Header[i],0)) == NULL) {
				Field[i].f_position = NO_FIELD;
				Field[i].f_width = NO_FIELD;
			}
			else {
				Field[i].f_position = p - line; /* 0 index */
				nfields++;
			}
		}
		/*
		 *  Get the field widths
		 */
		for(i = 0 ; i < MAXFIELD ; i++) {
			if(Field[i].f_position == NO_FIELD)
				continue;
			for(next = MAXLINE+1, j = 0 ; j < MAXFIELD ; j++) {
				if(Field[j].f_position > Field[i].f_position
				   && Field[j].f_position < next) {
					next = Field[j].f_position;
				}
			}
			if(next == MAXLINE+1)
				Field[i].f_width = FWIDTH;  /* last field */
			else
				Field[i].f_width =
					MIN(FWIDTH, next-Field[i].f_position);
		}
		if(nfields > 0 && Field[REFDES].f_position != NO_FIELD)
			break;
	}
	return(nfields);
}

/*
 *  get_fields	(local)
 *
 *  Return a pointer to the field_info structure of the next
 *  relavent line of input from the power/ground list file.
 *  Return NULL if there are no more relavent lines.
 */

static
FIELD *
get_fields()
{
	char line[MAXLINE], *sindex();
	int rmchar();
	register unsigned int i, len;

	if(fgets(line,MAXLINE,Tmpfp) == NULL) {
		fclose(Tmpfp);
		unlink(Tmpfile);
		return(NULL);
	}
	if(sindex(line,"END",0) != NULL
	   && sindex(line,"LIST",0) != NULL) {
		fclose(Tmpfp);
		unlink(Tmpfile);
		return(NULL);
	}

	len = strlen(line);
	line[len-1] = EOS;  /* kill newline */

	/*
	 *  Read in each fields
	 */
	for(i = 0 ; i < MAXFIELD ; i++) {
		if(Field[i].f_position == NO_FIELD)
			continue;
		if(Field[i].f_position > len)
			Field[i].f_string[0] = EOS;
		else
			strncpy(Field[i].f_string,
				line + Field[i].f_position, Field[i].f_width);
	}
	/*
	 *  Remove white spaces from each field
	 */
	for(i = 0 ; i < MAXFIELD ; i++) {
		if(Field[i].f_position == NO_FIELD)
			continue;
		rmchar(Field[i].f_string," \t",0);
	}
	return(Field);
}
