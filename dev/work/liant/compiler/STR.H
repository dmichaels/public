/* ANSI-C string.h - PL/I string string declarations */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  08.10.88  DGM  (001) Added NULL_STRING (a PL/I null pointer).
 *
 ***********************************************************************/

typedef struct string_type {
	short int	length;
	char		text [1];
} STRING;

#define NULL_STRING	((STRING *)(0xFFFFFFFF))  /* PL/I null pointer */

