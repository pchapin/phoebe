############################################################################
# FILE        : makefile
# LAST REVISED: 2003-02-22
# AUTHOR      : (C) Copyright 2003 by Peter Chapin
#
# This is the makefile for the pcode project.
############################################################################

CC=gcc
CFLAGS=-Wall -g
OBJS=main.o pcode.tab.o lex.yy.o tree.o vtcstr.o

# Main target
main:	$(OBJS)
	gcc -o main $(OBJS) -lfl

#
# Generator dependences.
#

lex.yy.c:	pcode.l
	flex pcode.l

pcode.tab.c:	pcode.y
	bison -d pcode.y

pcode.tab.h:	pcode.y
	bison -d pcode.y

#
# Object file dependencies.
#

lex.yy.o:	lex.yy.c pcode.tab.h vtcstr.h

pcode.tab.o:	pcode.tab.c pcode.tab.h tree.h vtcstr.h

main.o:		main.c tree.h vtcstr.h

tree.o:		tree.c tree.h vtcstr.h

vtcstr.o:	vtcstr.c vtcstr.h

#
# Other nicities.
#

clean:
	rm -f *.o

distclean:
	rm -f *.o
	rm -f lex.yy.c pcode.tab.c pcode.tab.h
	rm -f main.exe
