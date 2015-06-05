ACLOCAL_AMFLAGS = -I m4

SUBDIRS = Cudd/cudd libpolybori . groebner pyroot/polybori

lib_LTLIBRARIES = libpolybori.la

libpolybori_la_SOURCES =
libpolybori_la_LIBADD = \
	Cudd/cudd/libcudd.la \
	libpolybori/src/libpolybori_base.la

EXTRA_DIST = \
	Cudd/LICENSE \
	LICENSE \
	README
