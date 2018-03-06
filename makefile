export MAKEFILE=makefile

# for top- and sub-level-makefiles must be commented out
#OBJS= lib.o
#PROGS = tstprog
#echo: ;	@echo "srcdir=$(srcdir) =$(OBJS),$(PROGS)."
#SHLIBS =
#SCRIPTS =

ifneq ($(OBJS)$(PROGS)$(SHLIBS)$(SCRIPTS), )
    srcdir = yes
else
    srcdir = no
endif

.PHONY: all clean install new echo

ifeq ($(MAKELEVEL), 0)
######################################## START top-level-makefile
    arch :=  $(shell arch)
    MAKE := ${MAKE} arch=${arch}
endif
######################################## END top-level-makefile

ifeq ($(srcdir), yes)
######################################## START srcdir=yes
# Example for source directories
#
# # $Revision: 633 $
# OBJS = BaseFoo.o # none, when only headers
# PROGS = foo bar
#
# ifndef MAKEFILE
#     MAKEFILE = makefile
# endif
# include ../../$(MAKEFILE)

#CFLAGS = -g -Wall -Wno-unused-variable
#CFLAGS = -g -std=c++11 -Wall -Wno-unused-variable
CFLAGS = -g -m32 -std=c++11 -Wall -Wno-unused-variable
INCFLAGS = -I ./ -I ../.inc
LDFLAGS = -lrt
LIBFLAGS =  -L ../.lib -lBase
GCC = gcc
CXX = c++

# db-150511-BEGIN: oracle objects
SQLP = sqlplus
LOGON = hr/hr # @dbalias  with listener
SQLPFLAGS = -L -s $(LOGON)

%.lis: %.sql $(MAKEFILE)
	@echo "######################################################################### START $<"
	{ $(SQLP) $(SQLPFLAGS) $(CONNSTR) @$<; echo $(@:.lis=)=$$?; } | cat > $@
	@if grep -e"^Errors" -e"^ORA-" $@ > /dev/null; \
	then \
	    touch -r $< -d '-1 second' $@; \
	    cat -s $@; exit 3; \
	else \
	    echo -e "*** $(SQLP)=ENDOK $(@:.lis=) *** \n"; \
	fi # sqlp exit
# db-150511-END: oracle objects

# db-20160709-begin: java loader
LDJ = loadjava
USERSTR = -user db/db@mpsrv.local.net:1521:myoradb
LDJFLAGS = -v -r -t

%.ldj: %.java $(MAKEFILE)
	@echo "######################################################################### START $<"
	@if $(LDJ) $(USERSTR) $(LDJFLAGS) $< > $@ 2>&1; \
	then \
	    echo -e "*** $(LDJ)=ENDOK $@ *** \n"; \
	else \
	    touch -r $< -d '-1 second' $@; \
	    cat -s $@; \
	    exit 3; \
	fi # loadjava exit
# db-20160709-end: java loader

%.o: %.C $(MAKEFILE)
	$(CXX) $(CFLAGS) $(INCFLAGS) -o $@ -c $<

%.mk: %.C $(MAKEFILE)
	@echo "rebuilding $@ for '$(@:.mk=.o)' ..."; \
	set -e; rm -f $@; \
	$(CXX) $(CFLAGS) -MM -MG -MT '$(@:.mk=.o) $@' $(INCFLAGS) $< > $@;

SCRIPTS := $(BASH) $(PERL)

all: preinstall $(PROGS) $(SHLIB) $(SCRIPTS)

-include $(OBJS:.o=.mk)

preinstall:: $(OBJS:none=dummy.o)
    ifneq ($(OBJS), none)
	ar -ru ../.lib/libBase.a $<;
	cp -p $(<:.o=.H) ../.inc;
    else
	@echo "no objects found for $@"
    endif
	touch $@

dummy.o:
	@echo "nothing to do for $@"
	touch $@

install: all
    ifneq ($(OBJS), none)
	ar -ru ../../.lib/libBase.a *.o
    else
	@echo "no objects found for $@"
    endif
	@if ls *.H > /dev/null 2>&1; then \
	    echo "$@ includes ..."; \
	    cp -p *.H ../../.inc; \
	else \
	    echo "no includes found for $@"; \
	fi

-include $(PROGS:=.mk)

#static pattern-rules
$(PROGS): %: %.o $(OBJS:none=) $(MAKEFILE)
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -o $@ $(OBJS:none=) $(LIBFLAGS) $(LDFLAGS)

$(PERL): %: %.pl $(MAKEFILE)

$(BASH): %: %.sh $(MAKEFILE)

#	@echo "found $>"
#define Script_tpl
#enddef
#else
#	@echo "no chek for suffix=++$(suffix $<)++prereq=$<"
#endif

$(SCRIPTS):
	$(if $(findstring .sh, $<), @bash -n $<, \
	$(if $(findstring .pl, $<), @perl -c $<, \
	@echo "no chek for $(suffix $<)" \
	))
	cp $< $@; chmod +x $@

CLEAN = *.o *.a *.mk *.tmp preinstall *~
clean:
	$(RM) $(CLEAN) $(PROGS) $(SCRIPTS)

else
######################################## srcdir=no
# Example for makefiles in sub-directories
# # $Revision: 633 $
# SUBDIRS = foo bar
#
# ifndef MAKEFILE
#     MAKEFILE = makefile
# endif
# include ../$(MAKEFILE)

ifndef SUBDIRS
    SUBDIRS = base ora cgi cron
endif

INSDIRS = .lib .inc
.PHONY: $(INSDIRS) $(SUBDIRS)

all: $(INSDIRS) $(SUBDIRS)

$(INSDIRS):
	@[ -d $@ ] || { echo "making dir $@ ..."; mkdir $@; }
#	#echo "include ../$(MAKEFILE)" > $@/$(MAKEFILE); ls -la $@/$(MAKEFILE)
#	#echo "$(MAKEFILE): ; ls -l $(MAKEFILE)" > $@/$(MAKEFILE); ls -la $@/$(MAKEFILE)

$(SUBDIRS): $(MAKEFILE)
	$(MAKE) -C $@

.PHONY: cleanlocal $(CLEANDIR)
CLEANDIR = $(addsuffix .clean,$(SUBDIRS))

clean: cleanlocal $(CLEANDIR)

$(CLEANDIR):
	$(MAKE) -C $(@:.clean=) clean

cleanlocal:
	rm -f *~
	rm -f .inc/*.H
	rm -f .lib/*.a
#	@#for dir in $(SUBDIRS); do (cd $$dir; $(MAKE) clean ); done

.PHONY: $(INSTALLDIR)
INSTALLDIR = $(addsuffix .install, $(SUBDIRS))

install: $(INSTALLDIR)

$(INSTALLDIR):
	$(MAKE) -C $(@:.install=) install

endif
######################################## END srcdir=all

new:: clean all tags

ifeq ($(MAKELEVEL), 0)
######################################## START top-level-makefile


.PHONY: tags
CTAGS = ctags -R -n --extra=+fq --fields=+i+K+S+l+m+a
tags:
	@echo "building tags ..."; \
	if $(CTAGS) -f - | grep -v '.inc/' > $@; \
	then echo ok; \
	else echo "nok: .inc/ not found"; fi
	@ls -l $@
#! cat tags | { grep -v '.inc/'; echo Ret=$$?; } | less
#$(CTAGS) -f $@

.PHONY: newdir

#/\$$.*:.*\$$/ { gensub(/(.*)(\$$.*:)(.*\$$)(.*)/, "\\1\\2\$$\\4", 1) }
#substcmd := awk -v newdir=$(newdir) ' \
#    BEGIN { newDir=toupper(substr(newdir, 1, 1)) substr(newdir, 2); print newDir > "/dev/stderr"; } \
#    /\$$.*:.*\$$/ { match($$0, /\$$.*:.*\$$/); print substr($$0, 0, RSTART) "$$Id: makefile 633 2018-03-06 12:22:20Z db $$" substr($$0, RSTART+RLENGTH); next; } \
#    { gsub("tpl", newdir); gsub("BaseTpl", "Base" newDir); print $$0; } \
#    '
#newDir = $(shell echo | $(substcmd) 2>&1)
#newdir:
#	@echo "$@: MAKEFLAGS=$(MAKEFLAGS) newdir=$(newdir) newDir=$(newDir)"
#	@echo "substcmd=$(substcmd)"
#	mkdir base/$(newdir)
#	cat base/tpl/makefile  | $(substcmd) > base/$(newdir)/makefile
#	cat base/tpl/BaseTpl.C | $(substcmd) > base/$(newdir)/Base$(newDir).C
#	cat base/tpl/BaseTpl.H | $(substcmd) > base/$(newdir)/Base$(newDir).H
#	cat base/tpl/tpltst.C  | $(substcmd) > base/$(newdir)/$(newdir)tst.C
#	svn add base/$(newdir)

define newdir_sh
######## RUN NEWDIR_SH ########
#set -e
SubstCmd()
{
    awk -v newdir=${newdir} '
    BEGIN {
	NewDir=toupper(substr(newdir, 1, 1)) substr(newdir, 2);
        #print "BEGIN 0=" $0 ",NF=" NF ",NR=" NR;
        #print NewDir > "/dev/stderr";
	#print "*** substcmd ***";
    }
    /\$.*:.*\$/ {
	match($0, /\$.*:.*\$/);
	print substr($0, 0, RSTART) "$" "Id$" substr($0, RSTART+RLENGTH);
       	next;
    }
    {
	gsub("tpl", newdir);
	gsub("BaseTpl", "Base" NewDir);
        #print "all: 0=" $0 ",NF=" NF ",NR=" NR;
	print $0;
    }
    END {
	if (NR==0 && NF==0) print NewDir > "/dev/stderr";
    }'
}

#echo "++$(echo -n | SubstCmd 2<&1)++";
NewDir=$(echo -n | SubstCmd 2>&1); Ret=$?; echo "${NewDir} Ret=${Ret}"
[ ${Ret} = 0 ] || exit ${Ret}

mkdir base/${newdir}; Ret=$?
[ ${Ret} = 0 ] || exit ${Ret}

cat base/tpl/makefile  | SubstCmd > base/${newdir}/makefile
cat base/tpl/BaseTpl.C | SubstCmd > base/${newdir}/Base${NewDir}.C
cat base/tpl/BaseTpl.H | SubstCmd > base/${newdir}/Base${NewDir}.H
cat base/tpl/tpltst.C  | SubstCmd > base/${newdir}/${newdir}tst.C
ls -la base/${newdir}

svn add base/${newdir}
${MAKE} -C base/${newdir}
#if false
if [ $? = 0 ]
then
    svn ci -m"base/${newdir} added" base/${newdir}
else
    svn revert  base/${newdir}
fi

######## END NEWDIR_SH ########
endef

export NEWDIR_SH = $(value newdir_sh)
export newdir
export MAKE
#$(info newdir=$(newdir) $(newdir_sh))

newdir:
	#@echo "$@: MAKEFLAGS=$(MAKEFLAGS): newdir=$$newdir" # NewDir=$$NewDir"
	@echo "$$NEWDIR_SH" | cat -n
	@eval "$$NEWDIR_SH"

#.PHONY: $(MAKEFILE) makegen
#
#$(MAKEFILE):
#	ls -l $(MAKEFILE)

#makegen: $(MAKEFILE) $(SUBDIRS)

#echo : ; @echo "OBJ=$(OBJS)=$(OBJS:none=dummy.o)"
#define mline =
#    echo line1=PID=$$$($$PPID)
#    echo line2=PID=$$$($$PPID)
#endef

### samples of shell execution
#.ONESHELL: # for make  ver. >= 3.82 only
define multiline
echo | awk '
    {
    print "test multi line1 PID=$$($PPID)";
    print "test multi line2 PID=$$($PPID)";
    print "test multi line3 PID=$$($PPID)";
    }
'
endef
export MULTILINE := $(value multiline)
echo :
	echo "$$MULTILINE"
	eval "$$MULTILINE"

define scrsh
#! /bin/bash
    ###### BEGIN ######
    echo "SHELL=$SHELL"
    echo "test multi line1 PID=$$($PPID)";
    echo "test multi line2 PID=$$($PPID)";
    echo "test multi line3 PID=$$($PPID)";
    ###### ENDOF ######
endef
export SCRSH = $(value scrsh)

scrsh : $(MAKEFILE)
	@echo "$$SCRSH" > $@; \
	ls -la $@; cat $@ ; \
	chmod a+x $@

runscr: scrsh
	@scrsh




endif
######################################## END top-level-makefile

######################################## BEG all-level-makefile

#$(SUBDIRS):
#	$(MAKE) -f $(MAKEFILE) -C $@
# find  base -mindepth 1 -maxdepth 1 -type d ! -path "*/.*"
#awk ' '

######################################## END all-level-makefile
