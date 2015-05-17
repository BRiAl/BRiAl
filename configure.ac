AC_INIT([PolyBoRi], [0.8.4])

AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
LT_INIT

AC_LANG([C++])

AC_CHECK_TYPE([long long],
              [AC_DEFINE([PBORI_HAVE_LONG_LONG],[],[has long long])])

AC_CHECK_SIZEOF(void *)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)

AC_CHECK_HEADER([unordered_map],[AC_DEFINE([PBORI_HAVE_UNORDERD_MAP],[],[has unordered map])])
if test "x$ac_cv_header_unordered_map" != xyes; then
    AC_CHECK_HEADER([tr1/unordered_map],[AC_DEFINE([PBORI_HAVE_TR1_UNORDERED_MAP],[],[has tr1 unordered map])])
    if test "x$ac_cv_header_tr1_unordered_map" != xyes; then
        AC_CHECK_HEADER([ext/hash_map],[AC_DEFINE([PBORI_HAVE_HASH_MAP],[],[has hash map])])
    fi
fi

AC_CHECK_HEADER([m4ri/m4ri.h],
                [AC_DEFINE([PBORI_HAVE_M4RI],[],[has m4ri])],
                [AC_MSG_ERROR([unable to find m4ri])])
AC_EGREP_CPP([pbori_have_m4ri_png],
             [#include <m4ri/io.h>
              #if defined(__M4RI_HAVE_LIBPNG)
              #if __M4RI_HAVE_LIBPNG
              pbori_have_m4ri_png
              #endif
              #endif
             ],
             [AC_DEFINE([PBORI_HAVE_M4RI_PNG],[],[has m4ri png support])])

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
                 ])
AC_OUTPUT
