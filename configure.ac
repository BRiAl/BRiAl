AC_INIT([BRiAl],[1.2.13],[https://github.com/BRiAl/BRiAl/issues],,[https://github.com/BRiAl/BRiAl])

AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror tar-ustar])
AM_PROG_AR

LT_INIT
LT_CURRENT=3
LT_REVISION=7
LT_AGE=0
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
LT_INIT

dnl C++11 is now the default
AX_CXX_COMPILE_STDCXX(11,,mandatory)

AC_LANG([C++])

AC_CHECK_TYPE([long long],
              [AC_DEFINE([PBORI_HAVE_LONG_LONG],[],[has long long])])

AC_CHECK_SIZEOF(void *)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)

dnl Check that boost is available
AX_BOOST_BASE([1.58.0],[],
              [AC_MSG_ERROR("No suitable version of boost was found (1.58.0 or higher)")])

dnl Check that the boost unit test framework is present for running the testsuite
AX_BOOST_UNIT_TEST_FRAMEWORK

AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug],
                              [enable extra debugging statements in BRiAl's code [default=no]])],
              [AC_DEFINE([PBORI_DEBUG],[],[enable extra debugging statements])])

dnl version constraint to prevent broken pkg-config file from m4ri
PKG_CHECK_MODULES([M4RI], [m4ri >= 20250128], [AC_DEFINE([PBORI_HAVE_M4RI],[],[has m4ri])])
AC_EGREP_CPP([pbori_have_m4ri_png],
             [#include <m4ri/io.h>
              #if defined(__M4RI_HAVE_LIBPNG)
              #if __M4RI_HAVE_LIBPNG
              pbori_have_m4ri_png
              #endif
              #endif
             ],
             [m4ri_png="yes"], [m4ri_png="no"])

dnl Find out M4RI SIMMD flags
AX_M4RI_CFLAGS()
SIMMD_CFLAGS=$ax_cv_m4ri_cflags
AC_SUBST(SIMMD_CFLAGS)


if test "x$m4ri_png" = "xyes"; then
    PKG_CHECK_MODULES([PNG], [libpng],
        [have_libpng="yes"; LIBPNG_LIBADD="$PNG_LIBS"],
        [AC_CHECK_LIB([png],
            [png_create_write_struct],
            [have_libpng="yes"; LIBPNG_LIBADD="-lpng"],
            [AC_CHECK_LIB([png14],
                [png_create_write_struct],
                [have_libpng="yes"; LIBPNG_LIBADD="-lpng14"],
                [AC_CHECK_LIB([png12],
                    [png_create_write_struct],
                    [have_libpng="yes"; LIBPNG_LIBADD="-lpng12"],
                    [AC_CHECK_LIB([png10],
                        [png_create_write_struct],
                        [have_libpng="yes"; LIBPNG_LIBADD="-lpng10"],
                        [have_libpng="no"])
                    ])
                ])
            ])
        ])
fi

if test "x$have_libpng" = "xyes"; then
    AC_SUBST(LIBPNG_LIBADD)
    AC_DEFINE([PBORI_HAVE_M4RI_PNG],[],[has m4ri png support])
else
    LIBPNG_LIBADD=""

    PKG_CHECK_MODULES([GD], [gdlib],
        [AC_CHECK_LIB([gd], [gdImagePng],
            [have_gd="yes"],
            [have_gd="no"])
        ],[have_gd="no"])
    if test "x$have_gd" = "xyes" ; then
        AC_SUBST(GD_LIBS)
        AC_DEFINE([PBORI_HAVE_GD],[],[has gd png support])
    fi
fi

AC_CONFIG_HEADERS([config_dummy.h libbrial/include/polybori/config.h])
AC_CONFIG_FILES([
                 Makefile
                 cudd/Makefile
                 groebner/Makefile
                 groebner/include/Makefile
                 groebner/src/Makefile
                 libbrial/Makefile
                 libbrial/include/Makefile
                 libbrial/src/Makefile
                 tests/Makefile
                 brial.pc
                 ])
AC_OUTPUT
