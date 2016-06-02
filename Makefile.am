ACLOCAL_AMFLAGS = -I m4

SUBDIRS = Cudd/cudd libbrial . groebner pyroot

lib_LTLIBRARIES = libbrial.la

libbrial_la_SOURCES =
libbrial_la_LIBADD = \
	Cudd/cudd/libcudd.la \
	libbrial/src/libbrial_base.la
libbrial_la_LDFLAGS = -no-undefined $(AM_LDFLAGS)

EXTRA_DIST = \
	Cudd/LICENSE \
	LICENSE \
	README
# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libbrial_la_SOURCES = dummy.cc
