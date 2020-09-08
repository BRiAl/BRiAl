ACLOCAL_AMFLAGS = -I m4

SUBDIRS =
if !EXTERNAL_CUDD
SUBDIRS += cudd
endif
SUBDIRS += libbrial . groebner tests

lib_LTLIBRARIES = libbrial.la

libbrial_la_SOURCES =
libbrial_la_LIBADD = libbrial/src/libbrial_base.la ${CUDD_LIBS}
libbrial_la_LDFLAGS = -no-undefined $(AM_LDFLAGS) -version-info @LT_CURRENT@:@LT_REVISION@:@LT_AGE@

EXTRA_DIST = \
	cudd/LICENSE \
	LICENSE \
	README \
	sage-brial

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libbrial_la_SOURCES = dummy.cc
