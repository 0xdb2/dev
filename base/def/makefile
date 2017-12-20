
OBJS = none

ifndef MAKEFILE
    MAKEFILE = makefile
endif

include ../../$(MAKEFILE)

preinstall:: BaseDef.H
	cp -p $< ../.inc/

#.PHONY: all preinstall install
#
#all: preinstall
#
#lastinstall: BaseDef.H 
#	cp -p $< ../.inc/
#	@echo "$<" >> lastinstall; echo "Header $< pre-installed to ../.inc/"
#
#preinstall: lastinstall
#
#install: all
#	@cp -p *.H ../.inc
#	@echo "Header $$(ls *.H) installed to ../.inc"
#	@cp -p *.H ../../.inc
#	@echo "Header $$(ls *.H) installed to ../../.inc"
#
#clean:
#	rm -f *.o lastinstall
