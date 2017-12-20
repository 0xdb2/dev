# $Header$

PROGS   =
OBJS    = none
UOBJS   = pack_Bas type_Trace test_Trace pack_Extproc test_Extproc test_Pattern # trig_UpdUser

ifndef MAKEFILE
    MAKEFILE = makefile
endif

.PHONY: uobj $(UOBJS)

uobj: $(UOBJS) $(MAKEFILE)
$(UOBJS): $(UOBJS:=.lis)

file_Tst.lis: type_Trace.lis

include ../../$(MAKEFILE)

CLEAN := $(CLEAN) *.lis

#preinstall:: proc # TODO in top-level-makefile(?)


