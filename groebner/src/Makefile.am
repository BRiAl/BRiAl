include $(top_srcdir)/common.mk

lib_LTLIBRARIES = libpolybori_groebner.la

libpolybori_groebner_la_LIBADD = \
	$(top_builddir)/libpolybori.la \
	$(LIBPNG_LIBADD) \
	$(M4RI_LIBS) \
	$(GD_LIBS)

libpolybori_groebner_la_SOURCES = \
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
