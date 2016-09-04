CLEANFILES = *~

AM_CPPFLAGS = -I$(top_srcdir)

EXTRA_DIST = example.conf simple.conf

noinst_PROGRAMS = tester

tester_SOURCES = tester.c
tester_LDADD = ../ccl/libccl.la
