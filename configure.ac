dnl -*- m4 -*-
dnl $Id: configure.ac,v 1.3 2004-06-14 03:03:45 sbooth Exp $

dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(ccl, 0.1.1)
AC_CONFIG_SRCDIR(ccl/ccl.h)
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_REVISION([$Revision: 1.3 $])
AC_PREFIX_DEFAULT(/usr)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_CHECK_PROG(DOXYGEN, doxygen, doxygen, /bin/echo)

dnl Checks for libraries.

dnl Checks for header files.
AC_CONFIG_HEADERS(ccl/config.h)

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

AC_CONFIG_FILES([Makefile ccl/Makefile doc/Doxyfile doc/Makefile 
  demo/Makefile])
AC_OUTPUT
