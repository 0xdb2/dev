# $Id: makefile 238 2013-10-29 13:37:27Z db $

PROG = mthtst
OBJS = BaseMth.o

ifndef MAKEFILE
    MAKEFILE = makefile
endif
include ../../$(MAKEFILE)

CFLAGS := $(CFLAGS) -pthread -DMTSAFE
INCFLAGS := $(INCFLAGS) -I ../sock/
