#$Revision: 486 $

PROGS = sockclt socksrv
OBJS = BaseSock.o

ifndef MAKEFILE
    MAKEFILE = makefile
endif
include ../../$(MAKEFILE)

#CFLAGS := $(CFLAGS) -pthread -DPTHREAD
CFLAGS := $(CFLAGS) -DNOPTHREAD

#preinstall::
#	cp -p *.H ../../.inc/
