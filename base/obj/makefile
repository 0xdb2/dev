# $Revision: 286 $

OBJS = BaseObj.o
PROGS = objtst

#echo: ; echo $(if $(MAKEFILE), $(MAKEFILE)xx, makefileyy)
ifndef MAKEFILE
    MAKEFILE = makefile
endif
include ../../$(MAKEFILE)

#.PHONY: all clean install
#
#CFLAGS = -std=c++11 -g -Wall -Wno-unused-variable
#INCFLAGS = -I ./ -I ../.inc
#CC = c++
#
#%.o: %.C
#	$(CC) $(CFLAGS) $(INCFLAGS) -c -o $@ -c $<
#
#lib%.a: %.o
#	ar -ru $@ $<
#
#all: BaseObj.o objtst
#
#BaseObj.o: makefile BaseObj.H BaseObj.C
#
#install: all
#	ar -ru ../.lib/libBase.a *.o
#	ar -ru ../../.lib/libBase.a *.o
#	@#ar -tv ../../.lib/libBase.a
#	cp -p *.H ../.inc
#	cp -p *.H ../../.inc
#clean:
#	rm -f *.o; rm -f *.a
#
#objtst: libBaseObj.a objtst.C
#	$(CC) $(CFLAGS) $(INCFLAGS) objtst.C -o objtst -L ./ -lBaseObj

#$(@:.o=.C)
#for file in $PgmSrcCXX
#do
#    base=$(basename $file .C)
#    echo -e $base : $base.o '$(LIB) $(LIBRARIES)'
#    echo -e '     $(CXX) $(CXXFLAGS) $@.o -o $@ $(LDFLAGS)'
#    echo
#done


