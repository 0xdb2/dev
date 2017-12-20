# $Header$

PROGS   =
OBJS    = none
UOBJS   = pack_orderman test_orderman pack_form test_form # trig_UpdUser
JCLSS   = core/log/myLogger orderman formdb
#JCLSS   = orderman

ifndef MAKEFILE
    MAKEFILE = makefile
endif

.PHONY: user_objects $(UOBJS) $(JCLSS)

user_objects: $(JCLSS) $(UOBJS) $(MAKEFILE)

$(UOBJS): $(UOBJS:=.lis)
$(JCLSS): $(JCLSS:=.ldj)

include ../../$(MAKEFILE)

form.ldj: form.jar $(MAKEFILE)
	@echo "######################################################################### START $<"
	@if $(LDJ) $(USERSTR) -v -r $< > $@ 2>&1; \
	then \
	    echo -e "*** $(LDJ)=ENDOK $@ *** \n"; \
	else \
	    touch -r $< -d '-1 second' $@; \
	    cat -s $@; \
	    exit 3; \
	fi # loadjava exit

orderman: core/log/myLogger
pack_orderman.lis: orderman.ldj
test_orderman.lis: pack_orderman.lis orderman.ldj core/log/myLogger.java
formdb.ldj: form.ldj
test_form.lis: pack_form.lis formdb.ldj test_form.tpl

LOGON = db/db # @oradb  with listener

CLEAN := *.lis *.ldj

