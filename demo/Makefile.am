## $Id: Makefile.am,v 1.4 2004-04-15 18:40:59 sbooth Exp $

CLEANFILES = *~

INCLUDES = -I$(top_srcdir)

EXTRA_DIST = example.conf simple.conf

noinst_PROGRAMS = tester

tester_SOURCES = tester.c
tester_LDADD = ../ccl/libccl.la
