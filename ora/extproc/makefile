# $Id$

PROGS = socksrv
#OBJS = SockSrv.o
OBJS = none
SHLIB = libSocket.so

ifndef MAKEFILE
    MAKEFILE = makefile
endif

include ../../$(MAKEFILE)
CXX = gcc
INCFLAGS = -I ../../.inc
CFLAGS := $(CFLAGS) -fPIC
LIBFLAGS =  -L ../../.lib -lBase -lstdc++
LDFLAGS := -shared -static-libgcc $(LDFLAGS)
#$(info MakeLevel=$(MAKELEVEL)) # TODO in top-level-makefile

#%.so: %.C $(MAKEFILE)
#	$(CXX) $(CFLAGS) -shared $(INCFLAGS) $< -o $@ $<

$(SHLIB): lib%.so: %.o $(OBJS:none=) $(MAKEFILE)
	$(CXX) $(CFLAGS) $< -o $@ $(OBJS:none=) $(LDFLAGS) $(LIBFLAGS)

preinstall:: $(SHLIB)
	@cp -p $< $$HOME/rel/.lib/;
	@ls -la $$HOME/rel/.lib/;

#    @cp -p $< $$HOME/rel/.lib/$$(echo $< | tr [:upper:] [:lower:]);
#    OK
#    gcc -std=c++11 -Wall -Wno-unused-variable -fPIC -I ../../.inc -c SockSrv.c
#    gcc -shared -static-libgcc -o socksrv.so SockSrv.o -L ../../.lib -lBase -lrt -lstdc++
#    cp -p  socksrv.so $HOME/rel/.lib/;
#cp -p $(lc: <) socksrv.so $$HOME/rel/.lib/;

