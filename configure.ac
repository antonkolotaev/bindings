dnl Jérôme LELONG Feb 2012


AC_INIT([premia], [14])

AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE()

if test "X${CXXFLAGS}" != "X"; then
   user_set_cxxflags=yes
fi
if test "X${CFLAGS}" != "X"; then
   user_set_cflags=yes
fi

AM_PROG_LIBTOOL

## Initialize maintainer mode
AM_MAINTAINER_MODE


AC_ARG_ENABLE(debug,
   [  --enable-debug          compile with "-g" option, default behavious is to optimize],
   [enable_debug="yes"], [enable_debug="no"])
AC_ARG_ENABLE(optimize,
   [  --disable-optimize      disable "-0" options off gcc], [enable_optimize="no"], [enable_optimize="yes"])
AC_ARG_ENABLE(verbose,
   [  --disable-silent         tell libtool not to be silent], [], [enable_verbose="no"])
AC_ARG_WITH(premia-dir, 
    [AC_HELP_STRING([--with-premia-dir=<dir>], [Premia has been installed in directory <dir>])])
AC_ARG_WITH(premia-libdir, 
    [AC_HELP_STRING([--with-premia-libdir=<libdir>], [Premia library has been installed in directory <libdir>])])
AC_ARG_WITH(premia-includedir, 
    [AC_HELP_STRING([--with-premia-includedir=<includedir>], [Premia library has been installed in directory <includedir>])])
AC_ARG_WITH(python,
    [AC_HELP_STRING([--with-python=<python_binary>], [Use python_binary])])

if test "$enable_verbose" = no; then
   LIBTOOL="$LIBTOOL --silent"
fi

################
## build Paths
################

local_dir=`pwd`
local_home=$(dirname $(echo "$local_dir" | sed 's@/$@@'))

PREMIA_LIBDIR=
PREMIA_INCLUDEDIR=

if test -n "$with_premia_dir"; then
    PREMIA_LIBDIR="$with_premia_dir/lib"
    PREMIA_INCLUDEDIR="$with_premia_dir/include"
fi
if test -n "$with_premia_includedir"; then
    PREMIA_INCLUDEDIR="$with_premia_includedir"
fi
if test -n "$with_premia_libdir"; then
    PREMIA_LIBDIR="$with_premia_libdir"
fi
if test -z "$PREMIA_LIBDIR"; then
    PREMIA_LIBDIR="$local_home/lib"
fi
if test -z "$PREMIA_INCLUDEDIR"; then
    PREMIA_INCLUDEDIR="$local_home/include"
fi

libdir="$PREMIA_LIBDIR"
PREMIA_CPPFLAGS="-I$PREMIA_INCLUDEDIR"
AC_MSG_CHECKING(for Premia headers)
AC_MSG_RESULT($PREMIA_INCLUDEDIR)
AC_MSG_CHECKING(for Premia library)
AC_MSG_RESULT($PREMIA_LIBDIR)

##############################
## define C and C++ compilers
##############################
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_MAKE_SET
AC_PROG_INSTALL

# get the shared lib extension (need to force evaluation)
eval SHEXT=$shrext_cmds


##########################
## updating flags
##########################

# removing -ansi from CFLAGS
CC_OPTIONS="`echo $CFLAGS | sed -e 's/-ansi//g'`"
if test "x$GCC" = xyes ; then
   if test "x$enable_debug" = xyes || test "x$enable_optimize" = xno ; then
      CC_OPTIONS=`echo $CC_OPTIONS | sed -e 's/-O[[0-9]]*//g'`
      CC_OPTIONS=`echo $CC_OPTIONS | sed -e 's/-g[[0-9]]*$//g'`
      CC_OPTIONS=`echo $CC_OPTIONS | sed -e 's/-g[[0-9]]* //g'`
      CC_OPTIONS="${CC_OPTIONS} -Wall -g -Wdeclaration-after-statement"
   else
      CC_OPTIONS="${CC_OPTIONS} -Wall -Wdeclaration-after-statement"
   fi
fi


CXX_OPTIONS="`echo $CXXFLAGS | sed -e 's/-ansi//g'`"
if test "x$GXX" = xyes ; then
   if test "x$enable_debug" = xyes || test "x$enable_optimize" = xno; then
      CXX_OPTIONS=`echo $CXX_OPTIONS | sed -e 's/-O[[0-9]]*//g'`
      CXX_OPTIONS=`echo $CXX_OPTIONS | sed -e 's/-g[[0-9]]*$//g'`
      CXX_OPTIONS=`echo $CXX_OPTIONS | sed -e 's/-g[[0-9]]* //g'`
      CXX_OPTIONS="${CXX_OPTIONS} -Wall -g -fno-inline"
   else
      CXX_OPTIONS="${CXX_OPTIONS} -Wall"
   fi
fi

case "$host" in
    *-*-cygwin*)
    LIBTOOLIZE="glibtoolize"
    CC_OPTIONS="$CC_OPTIONS -mno-cygwin"
    CXX_OPTIONS="${CXX_OPTIONS} -mno-cygwin"
    ;;

    i*86-*-linux-gnu | *-pc-linux-gnu | x86_64-*-linux-gnu)
    LIBTOOLIZE="libtoolize"
    ;;

    # Darwin/MacOSX
    *-*-darwin*)
    LIBTOOLIZE="glibtoolize"
    ;;
esac

CFLAGS=${CC_OPTIONS}
CXXFLAGS=${CXX_OPTIONS}
AM_CFLAGS=${CC_OPTIONS}
AM_CXXFLAGS=${CXX_OPTIONS}

# if the user has not changed the flags, reset them.
# They are passed to AM_xxxx
if test X$user_set_cxxflags != Xyes; then CFLAGS=""; fi
if test X$user_set_cflags != Xyes; then CXXFLAGS=""; fi

LDFLAGS="${LDFLAGS} -lm"


####################
## Check for Boost #
####################

builtin(include,config/ax_boost_base.m4)
builtin(include,config/ax_boost_system.m4)
builtin(include,config/ax_boost_filesystem.m4)
builtin(include,config/ax_boost_program_options.m4)
AX_BOOST_BASE()
AX_BOOST_SYSTEM()
AX_BOOST_FILESYSTEM()
AX_BOOST_PROGRAM_OPTIONS()

######################################
## Check for Python and Boost Python #
######################################

AC_MSG_CHECKING(for Python)
if test -n "$with_python"; then
   PYTHON_BIN="$with_python"
else
   PYTHON_BIN="python"
fi
PYTHON_DIR=`$PYTHON_BIN -c 'import sys; print sys.prefix'`
PYTHON_VERSION=`$PYTHON_BIN -c 'import sys; print repr(sys.version_info[[0]],) + "." + repr(sys.version_info[[1]])'`
PYTHON_INCLUDE_DIR="$PYTHON_DIR/include/python$PYTHON_VERSION"
PYTHON_LIB_DIR="$PYTHON_DIR/lib"
PYTHON_CPPFLAGS="-I$PYTHON_INCLUDE_DIR"
PYTHON_LDFLAGS="-L$PYTHON_LIB_DIR"
AC_MSG_RESULT(found python$PYTHON_VERSION)
PYTHON_BIN="python$PYTHON_VERSION"

save_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS $PYTHON_LDFLAGS"
AC_CHECK_LIB($PYTHON_BIN, main, ax_python_lib=$PYTHON_BIN, ax_python_lib=no)
LDFLAGS="$save_LDFLAGS"
PYTHON_LDFLAGS="$PYTHON_LDFLAGS -l$ax_python_lib"

save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $PYTHON_CPPFLAGS"
AC_CHECK_HEADER([Python.h], [[ax_python_header=$PYTHON_INCLUDE_DIR]], ax_python_header=no)
CPPFLAGS="$save_CPPFLAGS"
if test "$ax_python_lib" = "no" || test "$ax_python_header" = "no"; then
   AC_MSG_ERROR(Cannot find Header or Library for $PYTHON_BIN in $PYTHON_DIR)
fi
AC_SUBST(PYTHON_CPPFLAGS)
AC_SUBST(PYTHON_LDFLAGS)


builtin(include,config/ax_boost_python.m4)
AX_BOOST_PYTHON()

# tweak to recover the --with-python in PYTHON_BIN if any
if test -n "$with_python"; then
   PYTHON_BIN="$with_python"
fi
AC_SUBST(PYTHON_BIN)
# end of tweak


###################
# Some more subst #
###################
AC_SUBST(CC)
AC_SUBST(CXX)
AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(MKDIR_P)
AC_SUBST(LIBTOOLIZE)
AC_SUBST(SHEXT)
AC_SUBST(PREMIA_CPPFLAGS)

AC_OUTPUT(src/Makefile)

echo
echo
echo "*******************"
echo "*     Summary     *"
echo "*******************"
echo
echo "C++ compiler..............:" $CXX
echo "C++ options...............:" $AM_CXXFLAGS $CXXFLAGS
echo "linker flags..............:" $LDFLAGS
echo "Python....................:" $(which $PYTHON_BIN)
echo "Premia lib directory......:" $PREMIA_LIBDIR
echo "Premia header directory...:" $PREMIA_INCLUDEDIR
echo
echo

