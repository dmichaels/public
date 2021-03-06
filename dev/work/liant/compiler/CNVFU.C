/* LPI-c++ cnvfu.c - float/double to/from unsigned long conversions */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  08.19.91  DGM  000	Original; from CFE c_f4u4.c.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Version and copyright stamp
/* ------------------------------------------------------------------- */

static char VERSION__[] = "@(#)LPI 08.16.91 000 CNVFU";

/* ---------------------------------------------------------------------
/* CFTOUL
/*
/* Convert "float" to "unsigned long"
/* ------------------------------------------------------------------- */

extern
unsigned long
CFTOUL (f)
float *f;
{
	return *f;
}

/* ---------------------------------------------------------------------
/* CDTOUL
/*
/* Convert "double" to "unsigned long"
/* ------------------------------------------------------------------- */

extern
unsigned long
CDTOUL (d)
double *d;
{
	return *d;
}


/* ---------------------------------------------------------------------
/* CULTOF
/*
/* Convert "unsigned long" to "float"
/* ------------------------------------------------------------------- */

extern
float
CULTOF (ul)
unsigned long *ul;
{
	return *ul;
}

/* ---------------------------------------------------------------------
/* CULTOD
/*
/* Convert "unsigned long" to "double"
/* ------------------------------------------------------------------- */

extern
double
CULTOD (ul)
unsigned long *ul;
{
	return *ul;
}
