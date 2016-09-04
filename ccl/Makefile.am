CLEANFILES = *~

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir)

lib_LTLIBRARIES = libccl.la
libccl_la_SOURCES = ccl_parse.c ccl_release.c ccl_get.c ccl_iterate.c	\
ccl_reset.c bst.c
libccl_la_LDFLAGS = -version-info 0:0:0

pkginclude_HEADERS = ccl.h bst.h
