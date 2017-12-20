# $Header: http://belov.homenet.org/svn/trunk/base/time/makefile 286 2014-09-14 08:48:27Z db $

OBJS = BaseTime.o # none, when only headers
PROGS = timetst

ifndef MAKEFILE
    MAKEFILE = makefile
endif
include ../../$(MAKEFILE)

#.PHONY: all test install clean
#
#CFLAGS = -g -std=c++11 -Wall -Wno-unused-variable
#INCFLAGS = -I ./ -I ../.inc
#CC = c++
#
#%.o: %.C
#	$(CC) $(CFLAGS) $(INCFLAGS) -c -o $@ -c $<
#
#all: BaseTime.o timetst
#
#BaseTime.o: makefile BaseTime.H BaseTime.C
#
#libBaseTime.a: BaseTime.o
#	ar -ru $@ $<
#
#install:  all
#	ar -ru ../.lib/libBase.a *.o
#	ar -ru ../../.lib/libBase.a *.o
#	@#ar -tv ../../.lib/libBase.a
#	cp -p *.H ../.inc
#	cp -p *.H ../../.inc
#
#timetst: timetst.C libBaseTime.a
#	$(CC) $(CFLAGS) $(INCFLAGS) timetst.C -o timetst -L ./ -lBaseTime -L ../.lib -lBase
#
#clean:
#	rm -f *.o libBaseTime.a
#
