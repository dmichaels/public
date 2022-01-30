/* -------------------------------------------------------------------------
 *  NAME
 *	money - income break down
 *
 * SYNOPSIS
 *	money y|m|w|h
 *
 * DESCRIPTION
 *	Prints to the standard output, a table of income figures for
 *	yearly, monthly, weekly, daily, and hourly income.  An option
 *	must be specified to indicate what form the INPUT will take:
 *
 *		"-y"	yearly
 *		"-m"	monthly
 *		"-w"	weekly
 *		"-d"	daily
 *		"-h"	hourly
 *
 *	Hourly figures are based on a 40 hour work week.
 *
 *  AUTHOR
 *	David Michaels (david@ll-sst) September 1984
 * ----------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * Include files
 * ----------------------------------------------------------------------- */
 
#include <stdio.h>

/* -------------------------------------------------------------------------
 * Definitions
 * ----------------------------------------------------------------------- */

#define NPERIOD		5
#define HOURLY		0
#define DAILY		1
#define WEEKLY		2
#define MONTHLY		3
#define YEARLY		4

#define DPY	356.0		/* days per year */
#define DPM	(DPY/MPY)	/* days per month */
#define DPW	7.0		/* days per week */
#define WPM	(DPM/DPW)	/* weeks per month */
#define MPY	12.0		/* months per year */
#define HPW	40.0		/* hours per week */

/* -------------------------------------------------------------------------
 * Type definitions
 * ----------------------------------------------------------------------- */

typedef float	DOLLAR;

struct money {
	DOLLAR	m_gross [NPERIOD];
	DOLLAR	m_net [NPERIOD];
	DOLLAR	m_fica [NPERIOD];
	DOLLAR	m_federal [NPERIOD];
	DOLLAR	m_state [NPERIOD];
	DOLLAR	m_total [NPERIOD];
	float	m_percent;
};

struct func_conv {
	DOLLAR	(*f_tocommon)();
	DOLLAR	(*f_fromcommon)();
};

/* -------------------------------------------------------------------------
 * Function declarations
 * ----------------------------------------------------------------------- */

DOLLAR ytom ();
DOLLAR mtoy ();
DOLLAR dtom ();
DOLLAR mtod ();
DOLLAR wtom ();
DOLLAR mtow ();
DOLLAR htom ();
DOLLAR mtoh ();
DOLLAR mtom ();

/* -------------------------------------------------------------------------
 * Static data
 * ----------------------------------------------------------------------- */

struct func_conv func [NPERIOD] =
{
	{ (DOLLAR (*)()) htom, (DOLLAR (*)()) mtoh },	/* Hourly */
	{ (DOLLAR (*)()) dtom, (DOLLAR (*)()) mtod },	/* Daily */
	{ (DOLLAR (*)()) wtom, (DOLLAR (*)()) mtow },	/* Weekly */
	{ (DOLLAR (*)()) mtom, (DOLLAR (*)()) mtom },	/* Monthly */
	{ (DOLLAR (*)()) ytom, (DOLLAR (*)()) mtoy },	/* Yearly */
};

/* -------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------- */

static
DOLLAR
ytom(d)
DOLLAR d;
{
	return (d / MPY);
}

static
DOLLAR
mtoy(d)
DOLLAR d;
{
	return (d * MPY);
}

static
DOLLAR
dtom(d)
DOLLAR d;
{
	return (d * DPM);
}

static
DOLLAR
mtod(d)
DOLLAR d;
{
	return (d / DPM);
}

static
DOLLAR
wtom(d)
DOLLAR d;
{
	return (d * WPM);
}

static
DOLLAR
mtow(d)
DOLLAR d;
{
	return (d / WPM);
}

static
DOLLAR
htom(d)
DOLLAR d;
{
	return (d * HPW * WPM);
}

static
DOLLAR
mtoh(d)
DOLLAR d;
{
	return (d / (HPW * WPM));
}

static
DOLLAR
mtom(d)
DOLLAR d;
{
	return (d);
}

/* -------------------------------------------------------------------------
 * main
 * ----------------------------------------------------------------------- */

main (argc, argv)
int	argc;
char	*argv[];
{
	char		*timeperiod, tp;
	DOLLAR		gross, fica, federal, state, net, total;
	struct money	m;
	register int	i;

	if ((argc!= 2) || (argv[1][0] != '-')) {
		fprintf (stderr, "usage: %s -y|-m|-w|-d|-h\n", *argv);
		exit (1);
	}
	else if (argv[1][1] == 'y') {
		tp = YEARLY;
		timeperiod = "yearly";
	}
	else if (argv[1][1] == 'm') {
		tp = MONTHLY;
		timeperiod = "monthly";
	}
	else if (argv[1][1] == 'w') {
		tp = WEEKLY;
		timeperiod = "weekly";
	}
	else if (argv[1][1] == 'd') {
		tp = DAILY;
		timeperiod = "daily";
	}
	else if (argv[1][1] == 'h') {
		tp = HOURLY;
		timeperiod = "hourly";
	}
	else {
		fprintf (stderr,
		"%s: argument must be one of the following:\n\n", *argv);
		fprintf (stderr, "   \"-y\"  (for yearly input figures)\n");
		fprintf (stderr, "   \"-m\"  (for monthly input figures)\n");
		fprintf (stderr, "   \"-w\"  (for weekly input figures)\n");
		fprintf (stderr, "   \"-d\"  (for daily input figures)\n");
		fprintf (stderr, "   \"-h\"  (for hourly input figures)\n");
		exit (1);
	}

	printf ("Enter %s gross salary: ", timeperiod);
	scanf ("%f", &gross);

	printf ("Enter %s FICA deductions: ", timeperiod);
	scanf ("%f", &fica);

	printf ("Enter %s federal deductions: ", timeperiod);
	scanf ("%f", &federal);

	printf ("Enter %s state deductions: ", timeperiod);
	scanf ("%f", &state);

	total           = fica + federal + state;
	net             = gross - total;
	m.m_gross[tp]   = gross;
	m.m_fica[tp]    = fica;
	m.m_federal[tp] = federal;
	m.m_state[tp]   = state;
	m.m_total[tp]   = total;
	m.m_net[tp]     = net;

	/* ----------------------------------------
	 * We will use the month as our common
	 * time period so that we need only
	 * ((NPERIOD * 2) - 1) conversion functions
	 * rather than (factorial(NPERIOD) - 1).
	 * -------------------------------------- */

	gross   = (*((func[tp]).f_tocommon))(gross);
	net     = (*func[tp].f_tocommon)(net);
	fica    = (*func[tp].f_tocommon)(fica);
	federal = (*func[tp].f_tocommon)(federal);
	state   = (*func[tp].f_tocommon)(state);
	total   = (*func[tp].f_tocommon)(total);

	for (i = 0 ; i < NPERIOD ; i++) {
		if (i == tp) continue;
		m.m_gross[i]   = (*func[i].f_fromcommon)(gross);
		m.m_net[i]     = (*func[i].f_fromcommon)(net);
		m.m_fica[i]    = (*func[i].f_fromcommon)(fica);
		m.m_federal[i] = (*func[i].f_fromcommon)(federal);
		m.m_state[i]   = (*func[i].f_fromcommon)(state);
		m.m_total[i]   = (*func[i].f_fromcommon)(total);
	}

	/* -------------------
	 * Print some results.
	 * ----------------- */

	printf ("\n%10s%10s%10s%10s%10s%10s%10s\n",
		"","yearly","monthly","weekly","daily","hourly","percent");
	printf ("%10s%10s%10s%10s%10s%10s%10s\n",
		"","------","-------","------","-----","------","-------");
	printf ("%-10s%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f\n","gross",
		m.m_gross[YEARLY],m.m_gross[MONTHLY],
		m.m_gross[WEEKLY],m.m_gross[DAILY],m.m_gross[HOURLY],
		gross/gross*100.0);
	printf ("%-10s%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f\n","net",
		m.m_net[YEARLY],m.m_net[MONTHLY],
		m.m_net[WEEKLY],m.m_net[DAILY],m.m_net[HOURLY],
		net/gross*100.0);
	printf ("%-10s%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f\n","FICA",
		m.m_fica[YEARLY],m.m_fica[MONTHLY],
		m.m_fica[WEEKLY],m.m_fica[DAILY],m.m_fica[HOURLY],
		fica/gross*100.0);
	printf ("%-10s%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f\n","federal",
		m.m_federal[YEARLY],m.m_federal[MONTHLY],
		m.m_federal[WEEKLY],m.m_federal[DAILY],m.m_federal[HOURLY],
		federal/gross*100.0);
	printf ("%-10s%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f\n","state",
		m.m_state[YEARLY],m.m_state[MONTHLY],
		m.m_state[WEEKLY],m.m_state[DAILY],m.m_state[HOURLY],
		state/gross*100.0);
	printf ("%-10s%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f\n\n","total tax",
		m.m_total[YEARLY],m.m_total[MONTHLY],
		m.m_total[WEEKLY],m.m_total[DAILY],m.m_total[HOURLY],
		total/gross*100.0);
	putchar ('\n');
}
