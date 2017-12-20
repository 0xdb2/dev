# $Header: http://belov.homenet.org/svn/trunk/base/vfs/makefile 286 2014-09-14 08:48:27Z db $

OBJS = BaseVfs.o
PROGS = vfstst

ifndef MAKEFILE
    MAKEFILE = makefile
endif
include ../../$(MAKEFILE)

#.PHONY: all test clean install
#
#CFLAGS = -g -Wall -Wno-unused-variable
#INCFLAGS = -I ./ -I ../.inc
#CXX = c++
#
#TSTPROG = vfstst
#%.o: %.C
#	$(CXX) $(CFLAGS) $(INCFLAGS) -c -o $@ -c $<
#
#lib%.a: %.o
#	ar -ru $@ $<
#
#%.mk: %.C makefile
#	@ echo "rebuilding $@"; \
#	set -e; rm -f $@; \
#	$(CXX) -MM -MT '$($@:.mk=) $@' $(INCFLAGS) $< > $@;
#
##	sed 's,\($*\)\.o[ :]*,\1.o $@ : makefile ,g' < $@.$$$$ > $@; \
##	rm -f $@.$$$$
#
#all: BaseVfs.o $(TSTPROG)
#
#BaseVfs.o: makefile BaseVfs.H BaseVfs.C
#
#install: all
#	ar -ru ../.lib/libBase.a *.o
#	ar -ru ../../.lib/libBase.a *.o
#	@#ar -tv ../../.lib/libBase.a
#	cp -p *.H ../.inc
#	cp -p *.H ../../.inc
#
#-include $(TSTPROG:=.mk)
#
#$(TSTPROG) : % : %.C libBaseVfs.a makefile
#	$(CXX) $(CFLAGS) $(INCFLAGS) $< -o $@ -L ./ -lBaseVfs -L ../.lib -lBase
#
#clean:
#	$(RM) *.o *.a
#
