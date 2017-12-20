# $Revision: 286 $

OBJS = BaseExc.o
PROGS = exctst

ifndef MAKEFILE
    MAKEFILE = makefile
endif
include ../../$(MAKEFILE)

#.PHONY: all clean install test
#
#CFLAGS = -g -Wall -Wno-unused-variable
#INCFLAGS = -I ./ -I ../.inc
#CC = c++
#
#%.o: %.C
#	$(CC) $(CFLAGS) $(INCFLAGS) -c -o $@ -c $<
#
#all: BaseExc.o
#
#BaseObj.o: makefile BaseExc.H BaseExc.C
#
#install: all
#	ar -ru ../.lib/libBase.a *.o
#	ar -ru ../../.lib/libBase.a *.o
#	@#ar -tv ../../.lib/libBase.a
#	cp -p *.H ../.inc
#	cp -p *.H ../../.inc
#clean:
#	rm -f *.o
#
#test: test.C
#	$(CC) $(CFLAGS) $(INCFLAGS) test.C -o test -L ../.lib -lBase
