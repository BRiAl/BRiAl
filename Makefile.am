ACLOCAL_AMFLAGS = -I m4

SUBDIRS = cudd libbrial . groebner tests

lib_LTLIBRARIES = libbrial.la
pkgconfig_DATA = brial.pc

libbrial_la_SOURCES =
libbrial_la_LIBADD = \
	cudd/libcudd.la \
	libbrial/src/libbrial_base.la
libbrial_la_LDFLAGS = -no-undefined $(AM_LDFLAGS) -version-info @LT_CURRENT@:@LT_REVISION@:@LT_AGE@

EXTRA_DIST = \
	cudd/LICENSE \
	LICENSE \
	README

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libbrial_la_SOURCES = dummy.cc

brial.pc: ${top_builddir}/config.status
