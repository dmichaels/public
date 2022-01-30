/* env - Print the environment variables and values */

/***********************************************************************
/* This product is the property of Language Processors, Inc. and is    *
/* licensed pursuant to a written license agreement.  No portion of    *
/* this product may be reproduced without the written permission of    *
/* Language Processors, Inc. except pursuant to the license agreement. *
/***********************************************************************/

/***********************************************************************
/*
/*  LPI EDIT HISTORY
/*
/*  01.09.89  DGM  Original.
/*
/**********************************************************************/

#include <stddef.h>

main (int argc, char *argv[], char *envp[])
{
	int i;

	for (i = 0 ; envp[i] != NULL ; i++)
		printf ("envp [%2d]: \"%s\"\n", i, envp[i]);

	exit (0);
}

