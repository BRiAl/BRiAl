AM_CPPFLAGS = \
	-I$(top_srcdir)/groebner/include \
	-I$(top_builddir)/libbrial/include \
	-I$(top_srcdir)/libbrial/include \
	-DBOOST_TEST_DYN_LINK

AM_CXXFLAGS = -ftemplate-depth-100

AM_DEFAULT_SOURCE_EXT = .cc

unittests_SOURCES = \
	BlockDegLexOrderTest.cc \
	BlockDegRevLexAscOrderTest.cc \
	BooleConstantTest.cc \
	BooleEnvTest.cc \
	BooleExponentTest.cc \
	BooleMonomialTest.cc \
	BoolePolynomialTest.cc \
	BoolePolyRingTest.cc \
	BooleSetTest.cc \
	BooleVariableTest.cc \
	CCuddDDFacadeTest.cc \
	CFactoryBaseTest.cc \
	CStringLiteralTest.cc \
	DegLexOrderTest.cc \
	DegRevLexAscOrderTest.cc \
	FGLMStrategyTest.cc \
	GroebnerStrategyTest.cc \
	LexOrderTest.cc \
	MonomialFactoryTest.cc \
	PBoRiErrorTest.cc \
	PolynomialFactoryTest.cc \
	PseudoLongProductTest.cc \
	SetFactoryTest.cc \
	spolyTest.cc \
	term_accumulateTest.cc \
	VariableFactoryTest.cc \
	weak_pointersTest.cc \
	unittests.cc

AM_LDFLAGS = $(BOOST_UNIT_TEST_FRAMEWORK_LIB)
LIBS = $(top_builddir)/groebner/src/libbrial_groebner.la $(top_builddir)/libbrial.la -lm4ri

check_PROGRAMS = unittests

check: unittests
	./unittests
