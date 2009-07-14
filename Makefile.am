## $Id: Makefile.am,v 1.3 2004-04-15 18:38:11 sbooth Exp $

CLEANFILES = *~

MAINTAINERCLEANFILES = aclocal.m4 ccl-*.tar.gz

SUBDIRS = ccl doc demo

if MAINTAINER_MODE
################################################################
##
## Everything past here is useful to the maintainer, but probably not
## to anybody else (taken from libtool/Makefile.am).
##

## set it to `:' to avoid CVS operations
CVS = cvs

## Tag sources before making distribution, but don't make a
## distribution if checks fail.
cvs-dist: distcheck
	cd $(srcdir) && \
	 $(CVS) -q tag `echo "release-$(VERSION)" | sed 's/\./-/g'`
	$(MAKE) dist

## Generate a context diff file containing the patch info for the
## current version.
cvs-diff:
	thisver=`echo "release-$(VERSION)" | sed 's/\./-/g'`; \
	if test -z "$$OLDVERSION"; then \
	  prevno=`echo "$(VERSION)" - 0.01 | bc | sed 's/^\./0./'`; \
	else prevno="$$OLDVERSION"; fi; \
	prevver=release-`echo $$prevno | sed 's/\./-/g'`; \
	$(CVS) -f rdiff -c -r $$prevver -r $$thisver $(PACKAGE) \
	    > $(PACKAGE)-$$prevno-$(VERSION).diff
endif
