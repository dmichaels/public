########################################################################
##  --- LPI MC68020/68881 Disassembler ---
########################################################################

########################################################################

CC 		= cc
PL1		= lpipl1
CC_FLAGS	= -c -O
PL1_FLAGS	= -stat -nom68020
LD		= lpild
LD_FLAGS	= -nom68020

########################################################################

PROGRAM		= dis

OBJECTS		= dismain.o dis.o dis881.o disea.o prntop.o prntea.o

########################################################################

$(PROGRAM):	dismain.o dis.o dis881.o disea.o prntop.o prntea.o
		$(LD) -o $(PROGRAM) $(OBJECTS)

dismain.o:	dismain.c
		$(CC) $(CC_FLAGS) dismain.c

dis.o:		dis.pl1 dis.in opdef.in regdef.in regname.in ea.in
		$(PL1) $(PL1_FLAGS) dis.pl1

dis881.o:	dis881.pl1 dis.in opdef.in regdef.in regname.in ea.in
		$(PL1) $(PL1_FLAGS) dis881.pl1

disea.o:	disea.pl1 dis.in regdef.in ea.in
		$(PL1) $(PL1_FLAGS) disea.pl1

prntea.o:	prntea.pl1 dis.in opsize.in regdef.in regname.in ea.in
		$(PL1) $(PL1_FLAGS) prntea.pl1

prntop.o:	prntop.pl1 dis.in opdef.in opname.in opsize.in
		$(PL1) $(PL1_FLAGS) prntop.pl1
