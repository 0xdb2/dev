# $Revision: 286 $

OBJS = BaseStr.o
PROGS = strtst

ifndef MAKEFILE
    MAKEFILE = makefile
endif
include ../../$(MAKEFILE)

#.PHONY: all clean install test
#
#CFLAGS = -g -std=c++11 -Wall -Wno-unused-variable
#INCFLAGS = -I ./ -I ../.inc
#CC = c++
#
#%.o: %.C
#	$(CC) $(CFLAGS) $(INCFLAGS) -o $@ -c $<
#
#all: BaseStr.o strtst
#
#BaseStr.o: makefile BaseStr.H BaseStr.C
#
#install: all
#	ar -ru ../.lib/libBase.a *.o
#	ar -ru ../../.lib/libBase.a *.o
#	cp -p *.H ../.inc
#	cp -p *.H ../../.inc
#clean:
#	rm -f *.o; rm -f *.a
#
#strtst: strtst.C
#	ar -ru libBaseStr.a *.o
#	$(CC) $(CFLAGS) $(INCFLAGS) strtst.C -o strtst -L ./ -lBaseStr -L ../.lib -lBase
