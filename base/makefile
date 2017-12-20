# $Header: http://belov.homenet.org/svn/trunk/base/makefile 510 2016-07-18 14:58:33Z db $

SUBDIRS = core obj time exc store str vfs def trc map sock pth ora shm tpl
ifndef MAKEFILE
    MAKEFILE = makefile
endif
include ../$(MAKEFILE)

# poss. depedencies, when order is important
#tpl: def obj time exc str
sock: pth map

all:
	@echo "preinstall base header and libs ..."; \
	cp -p .inc/*.H ../.inc; \
	cp -p .lib/lib* ../.lib;

