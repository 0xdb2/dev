# $Id$

PROGS = oratst
#PROC = BaseOra # BaseOraEx
OBJS = BaseOra

ifndef MAKEFILE
    MAKEFILE = makefile
endif

.PHONY: proc
OBJS := $(OBJS:=.o)
PROC := $(PROC:=.CPP)
# export ORALIBS="\$(ORACLE_HOME)/lib32/libclntsh.so -ldl -lm -lpthread -lnsl "
ORA_LIBS = $(ORACLE_HOME)/lib/libclntsh.so # -lcl -lm -l:libcl.a
$(info $(PROC) $(OBJS))

%.o: %.CPP $(MAKEFILE)
	$(CXX) $(CFLAGS) $(INCFLAGS) -o $@ -c $<

include ../../../$(MAKEFILE)

INCFLAGS := $(INCFLAGS) -I ../../.inc
LIBFLAGS = -L ../../.lib -lBase

LDFLAGS := $(LDFLAGS) $(ORA_LIBS)
#preinstall:: proc # TODO in top-level-makefile(?)
BaseOra.o: BaseOra.CPP BaseOra.C $(MAKEFILE)

USERID    = hr/sample4You # @myoradb  with listener
PROCSQL   = SQLCHECK=FULL USERID=$(USERID)
PROCFLAGS := CODE=CPP PARSE=PARTIAL LINES=YES $(PROCSQL) # comment out PROCSQL to run without connect to db
PROCINC   := $(patsubst %,include=%,$(subst -I, ,$(INCFLAGS)))
#$(info $(INCLUDE))

%.CPP: %.C $(MAKEFILE)
	@echo "rebuilding $@ from $<"; #cp -pf $< $@
	proc  $(PROCFLAGS) $(PROCINC) iname=$< oname=$@ > $(<:.C=.lis);  Ret=$$?; cat -s $(<:.C=.lis); exit $$Ret
	@if grep "^Error" $(<:.C=.lis) > /dev/null; \
	then \
	    $(RM) $@; cat $(<:.C=.lis); exit 1; \
	else \
	    echo -e "*** proc=ENDOK *** \n"; \
	fi # proc exit

