# $Id: makefile 286 2014-09-14 08:48:27Z db $

PROGS = pthtst
OBJS = BasePth.o

ifndef MAKEFILE
    MAKEFILE = makefile
endif
include ../../$(MAKEFILE)

CFLAGS := $(CFLAGS) -pthread -DMTSAFE
INCFLAGS := $(INCFLAGS) -I ../sock/
