AUTOMAKE_OPTIONS = gnu
ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS=${SIMD_CFLAGS} ${OPENMP_CFLAGS} ${DEBUG_FLAGS}

lib_LTLIBRARIES = libm4ri.la

libm4ri_la_SOURCES = \
	m4ri/brilliantrussian.c \
	m4ri/misc.c \
	m4ri/packedmatrix.c \
	m4ri/grayflex.c \
	m4ri/strassen.c \
	m4ri/permutation.c \
	m4ri/trsm.c \
	m4ri/pls.c \
	m4ri/solve.c \
	m4ri/pls_mmpf.c \
	m4ri/echelonform.c \
	m4ri/mmc.c \
	m4ri/debug_dump.c \
	m4ri/io.c

BUILT_SOURCES = m4ri/m4ri_config.h

pkgincludesubdir = $(includedir)/m4ri
pkgincludesub_HEADERS = m4ri/m4ri.h \
	m4ri/brilliantrussian.h \
	m4ri/misc.h \
	m4ri/packedmatrix.h \
	m4ri/grayflex.h \
	m4ri/strassen.h \
	m4ri/parity.h \
	m4ri/permutation.h \
	m4ri/trsm.h \
	m4ri/pls.h \
	m4ri/solve.h \
	m4ri/pls_mmpf.h \
	m4ri/echelonform.h \
	m4ri/xor.h \
	m4ri/mmc.h \
	m4ri/debug_dump.h \
	m4ri/io.h

nodist_pkgincludesub_HEADERS = m4ri/m4ri_config.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = m4ri.pc

libm4ri_la_LDFLAGS = -release 0.0.20111203 -no-undefined
libm4ri_la_LIBADD = -lm $(LIBPNG_LIBADD)

check_PROGRAMS=test_multiplication test_elimination test_trsm test_pls test_solve test_kernel test_random test_smallops test_transpose test_colswap
test_multiplication_SOURCES=testsuite/test_multiplication.c
test_multiplication_LDFLAGS=-lm4ri -lm
test_multiplication_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

test_elimination_SOURCES=testsuite/test_elimination.c
test_elimination_LDFLAGS=-lm4ri -lm
test_elimination_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

test_trsm_SOURCES=testsuite/test_trsm.c
test_trsm_LDFLAGS=-lm4ri -lm
test_trsm_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

test_pls_SOURCES=testsuite/test_pluq.c
test_pls_LDFLAGS=-lm4ri -lm
test_pls_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

test_solve_SOURCES=testsuite/test_solve.c
test_solve_LDFLAGS=-lm4ri -lm
test_solve_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

test_kernel_SOURCES=testsuite/test_kernel.c
test_kernel_LDFLAGS=-lm4ri -lm
test_kernel_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

test_random_SOURCES=testsuite/test_random.c
test_random_LDFLAGS=-lm4ri -lm
test_random_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

test_smallops_SOURCES=testsuite/test_smallops.c testsuite/testing.c testsuite/testing.h
test_smallops_LDFLAGS=-lm4ri -lm
test_smallops_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

test_transpose_SOURCES=testsuite/test_transpose.c
test_transpose_LDFLAGS=-lm4ri -lm
test_transpose_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

test_colswap_SOURCES=testsuite/test_colswap.c
test_colswap_LDFLAGS=-lm4ri -lm
test_colswap_CFLAGS=-I$(srcdir)/src $(AM_CFLAGS)

TESTS = test_multiplication test_elimination test_trsm test_pls test_solve test_kernel test_random test_smallops test_transpose test_colswap
