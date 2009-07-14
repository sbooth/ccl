dnl -*- m4 -*-

dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(ccl, 0.1.1)
AC_CONFIG_SRCDIR(ccl/ccl.h)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(support)
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_PREFIX_DEFAULT(/usr/local)

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
