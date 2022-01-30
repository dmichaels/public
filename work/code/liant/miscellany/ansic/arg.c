/* arg - Print the command-line arguments passed to this program */

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

main (int argc, char *argv[])
{
	int i;

	printf ("argc:       %d\n", argc);
	for (i = 0 ; i < argc ; i++)
		printf ("argv [%2d]: \"%s\"\n", i, argv[i]);

	exit (0);
}

