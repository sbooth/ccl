## $Id: Makefile.am,v 1.1 2004-04-14 15:36:39 sbooth Exp $

CLEANFILES = *~

DOXYFILE = Doxyfile

docdir = $(prefix)/doc/$(PACKAGE)-$(VERSION)

TEMPLATES = index.tmpl license.tmpl

EXTRA_DIST = html

all: html

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	@for file in $(srcdir)/html/*; do \
		bn="`basename $$file`"; \
		echo " $(INSTALL_DATA) $$file $(DESTDIR)$(docdir)/$$bn"; \
	 	$(INSTALL_DATA) $$file $(DESTDIR)$(docdir)/$$bn; \
	done

uninstall-local:
	@for file in $(srcdir)/html/*; do \
		bn="`basename $$file`"; \
		echo " rm -f $(DESTDIR)$(docdir)/$$bn"; \
		rm -f $(DESTDIR)$(docdir)/$$bn; \
	done

if MAINTAINER_MODE
################################################################
##
## Everything past here is useful to the maintainer, but probably not
## to anybody else (taken from libtool/Makefile.am).
##
html: $(DOXYFILE) $(TEMPLATES) $(srcdir)/../ccl/ccl.h
	DATE=`date '+%-d %b %Y'` VERSION=$(VERSION) $(DOXYGEN) $(DOXYFILE)
else
html:
	@if test ! -d html; then \
	echo "HTML documenation not present"; \
	fi
endif
