## $Id: Makefile.am,v 1.3 2004-04-15 18:36:09 sbooth Exp $

CLEANFILES = *~

DOXYFILE = Doxyfile

TEMPLATES = index.tmpl

EXTRA_DIST = html latex

all: doc

if MAINTAINER_MODE
################################################################
##
## Everything past here is useful to the maintainer, but probably not
## to anybody else (taken from libtool/Makefile.am).
##
doc: $(DOXYFILE) $(TEMPLATES) $(srcdir)/../ccl/ccl.h
	DATE=`date '+%-d %b %Y'` VERSION=$(VERSION) $(DOXYGEN) $(DOXYFILE)
else
doc:
	@if test ! -d html; then \
	echo "HTML documenation not present"; \
	fi
endif
