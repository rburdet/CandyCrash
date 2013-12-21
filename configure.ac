# this is example-file: configure.ac

# initial information about the project
AC_INIT([CandyCrash],[1.0],[rburdet@gmail.com ncis20@gmail.com])

# check if the source folder is correct
AC_CONFIG_SRCDIR([src/server/server.cpp])
AC_CONFIG_SRCDIR([src/cliente/cliente.cpp])
AC_CONFIG_SRCDIR([src/editor/main.cpp])

# Checks for programs

# check for C++ compiler and the library compiler
AC_PROG_CXX
AC_PROG_RANLIB

# automake initialisation (mandatory) and check for minimal automake API version 1.9
AM_INIT_AUTOMAKE([1.9])

######################################################################
# DOXYGEN SUPPORT
######################################################################

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(ON)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN([$PACKAGE_NAME],[doxygen.cfg],[doc])

######################################################################

# use the C++ compiler for the following checks
AC_LANG([C++])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])
AC_CHECK_HEADERS([stringstream])
AC_CHECK_HEADERS([pthread.h])
AC_CHECK_HEADERS([libgen.h])
AC_CHECK_HEADERS([uinstd.h])
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([stdio.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Check for libraries
AC_ARG_WITH([jsoncpp-include-path],
  [AS_HELP_STRING([--with-jsoncpp-include-path],
    [location of the JsonCPP headers, defaults to /usr/include])],
  [JSONCPP_CFLAGS="-I$withval"],
  [JSONCPP_CFLAGS='-I/usr/include'])
AC_SUBST([JSONCPP_CFLAGS])

AC_ARG_WITH([jsoncpp-lib-path],
  [AS_HELP_STRING([--with-jsonpp-lib-path], [location of the JsonCPP libraries])],
  [JSONCPP_LIBS="-L$withval -ljsoncpp"],
  [JSONCPP_LIBS='-ljsoncpp'])
AC_SUBST([JSONCPP_LIBS])

#AC_ARG_WITH([asound-include-path],
#  [AS_HELP_STRING([--with-asound-include-path],
#    [location of the asound headers, defaults to /usr/include])],
#  [ASOUND_CFLAGS="-I$withval"],
#  [ASOUND_CFLAGS='-I/usr/include'])
#AC_SUBST([ASOUND_CFLAGS])
#
#AC_ARG_WITH([asound-lib-path],
#  [AS_HELP_STRING([--with-asound-lib-path], [location of the asound libraries])],
#  [ASOUND_LIBS="-L$withval -lasound"],
#  [ASOUND_LIBS='-lasound'])
#AC_SUBST([ASOUND_LIBS])

AC_ARG_WITH([pthread-include-path],
  [AS_HELP_STRING([--with-pthread-include-path],
    [location of the pThread headers, defaults to /usr/include])],
  [PTHREAD_CFLAGS="-I$withval"],
  [PTHREAD_CFLAGS='-I/usr/include'])
AC_SUBST([PTHREAD_CFLAGS])

AC_ARG_WITH([pthread-lib-path],
  [AS_HELP_STRING([--with-pthread-lib-path], [location of the pThread libraries])],
  [PTHREAD_LIBS="-L$withval -lpthread -pthread"],
  [PTHREAD_LIBS='-lpthread -pthread'])
AC_SUBST([PTHREAD_LIBS])

PKG_CHECK_MODULES([GTKMM3], [gtkmm-3.0 >= 3.8])
PKG_CHECK_MODULES([GTKMM2], [gtkmm-2.4 >= 2.6])
PKG_CHECK_MODULES([ASOUND], [alsa >= 1.0])

# distribute additional compiler and linker flags
# --> set these variables instead of CXXFLAGS or LDFLAGS
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])
AC_SUBST([LIBS])

# files to generate via autotools (.am or .in source files)
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])

# generate the final Makefile etc.
AC_OUTPUT
