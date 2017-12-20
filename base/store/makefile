#$Revision: 286 $

OBJS = BaseStore.o
PROGS = store

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
#all: BaseStore.o store
#
#BaseStore.o: makefile BaseStore.H BaseStore.C
#
#install: all
#	ar -ru ../.lib/libBase.a *.o
#	ar -ru ../../.lib/libBase.a *.o
#	cp -p *.H ../.inc
#	cp -p *.H ../../.inc
#clean:
#	rm -f *.o; rm -f *.a
#
#store: store.C
#	ar -ru libBaseStore.a *.o
#	$(CC) $(CFLAGS) $(INCFLAGS) store.C -o store -L ./ -lBaseStore -L ../.lib -lBase
