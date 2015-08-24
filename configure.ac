AC_INIT([BRiAl], [0.8.4])

AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror tar-ustar])

AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
LT_INIT

AC_LANG([C++])

AM_PATH_PYTHON([2.7])

AC_CHECK_TYPE([long long],
              [AC_DEFINE([PBORI_HAVE_LONG_LONG],[],[has long long])])

AC_CHECK_SIZEOF(void *)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)

AC_CHECK_HEADER([unordered_map],[AC_DEFINE([PBORI_HAVE_UNORDERD_MAP],[],[has unordered map])])
if ! test "x$ac_cv_header_unordered_map" = xyes; then
    AC_CHECK_HEADER([tr1/unordered_map],[AC_DEFINE([PBORI_HAVE_TR1_UNORDERED_MAP],[],[has tr1 unordered map])])
    if ! test "x$ac_cv_header_tr1_unordered_map" = xyes; then
        AC_CHECK_HEADER([ext/hash_map],[AC_DEFINE([PBORI_HAVE_HASH_MAP],[],[has hash map])])
    fi
fi

PKG_CHECK_MODULES([M4RI], m4ri, [AC_DEFINE([PBORI_HAVE_M4RI],[],[has m4ri])])
AC_EGREP_CPP([pbori_have_m4ri_png],
             [#include <m4ri/io.h>
              #if defined(__M4RI_HAVE_LIBPNG)
              #if __M4RI_HAVE_LIBPNG
              pbori_have_m4ri_png
              #endif
              #endif
             ],
             [m4ri_png="yes"], [m4ri_png="no"])

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
        ])
    if test "x$have_gd" = "xyes" ; then
        AC_SUBST(GD_LIBS)
        AC_DEFINE([PBORI_HAVE_GD],[],[has gd png support])
    fi
fi

AC_CONFIG_HEADERS([libpolybori/include/polybori/config.h])
AC_CONFIG_FILES([
                 Makefile
                 Cudd/cudd/Makefile
                 groebner/Makefile
                 groebner/include/Makefile
                 groebner/src/Makefile
                 libpolybori/Makefile
                 libpolybori/include/Makefile
                 libpolybori/src/Makefile
                 pyroot/polybori/Makefile
                 ])
AC_OUTPUT
