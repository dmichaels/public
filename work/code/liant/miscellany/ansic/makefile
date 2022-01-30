# makefile - LPI ANSI C makefile for sample programs.

########################################################################
#  This product is the property of Language Processors, Inc. and is    #
#  licensed pursuant to a written license agreement.  No portion of    #
#  this product may be reproduced without the written permission of    #
#  Language Processors, Inc. except pursuant to the license agreement. #
########################################################################

ANSI_CC		= newc
LD		= ldc

ANSI_CFLAGS	=
LDFLAGS		=

PROGRAMS	= arg env ctoe etoc hello mega roman sample

########################################################################

all:		$(PROGRAMS)

arg:		arg.o
		$(LD) $(LDFLAGS) -o arg arg.o

arg.o:		arg.c
		$(ANSI_CC) $(ANSI_CFLAGS) arg.c

env:		env.o
		$(LD) $(LDFLAGS) -o env env.o

env.o:		env.c
		$(ANSI_CC) $(ANSI_CFLAGS) env.c

ctoe:		ctoe.o
		$(LD) $(LDFLAGS) -o ctoe ctoe.o

ctoe.o:		ctoe.c
		$(ANSI_CC) $(ANSI_CFLAGS) ctoe.c

etoc:		etoc.o
		$(LD) $(LDFLAGS) -o etoc etoc.o

etoc.o:		etoc.c
		$(ANSI_CC) $(ANSI_CFLAGS) etoc.c

hello:		hello.o
		$(LD) $(LDFLAGS) -o hello hello.o

hello.o:	hello.c
		$(ANSI_CC) $(ANSI_CFLAGS) hello.c

mega:		mega.o
		$(LD) $(LDFLAGS) -o mega mega.o

mega.o:		mega.c
		$(ANSI_CC) $(ANSI_CFLAGS) mega.c

roman:		roman.o
		$(LD) $(LDFLAGS) -o roman roman.o

roman.o:	roman.c
		$(ANSI_CC) $(ANSI_CFLAGS) roman.c

