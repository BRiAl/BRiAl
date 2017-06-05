include $(top_srcdir)/common.mk

lib_LTLIBRARIES = libbrial_groebner.la

libbrial_groebner_la_CXXFLAGS = $(AM_CXXFLAGS) $(SIMMD_CFLAGS)

libbrial_groebner_la_LIBADD = \
	$(top_builddir)/libbrial.la \
	$(LIBPNG_LIBADD) \
	$(M4RI_LIBS) \
	$(GD_LIBS)

libbrial_groebner_la_LDFLAGS = -no-undefined $(AM_LDFLAGS) -version-info @LT_CURRENT@:@LT_REVISION@:@LT_AGE@

libbrial_groebner_la_SOURCES = \
	dlex4data.cc \
	dp_asc4data.cc \
	FGLMStrategy.cc \
	groebner.cc \
	groebner_alg.cc \
	GroebnerStrategy.cc \
	interpolate.cc \
	LexBucket.cc \
	LiteralFactorization.cc \
	LiteralFactorizationIterator.cc \
	lp4data.cc \
	MatrixMonomialOrderTables.cc \
	nf.cc \
	PairManager.cc \
	pairs.cc \
	PolyEntry.cc \
	polynomial_properties.cc \
	randomset.cc \
	ReductionStrategy.cc
