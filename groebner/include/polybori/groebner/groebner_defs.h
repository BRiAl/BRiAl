/*
 *  groebner_defs.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 19.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#include <polybori.h>
#include <boost/integer.hpp>
#ifndef PBORI_GB_DEFS_H
#define PBORI_GB_DEFS_H
#ifndef PBORI_NO_NAMESPACES
#define BEGIN_NAMESPACE_PBORIGB namespace PBORINAME { namespace groebner{
#define END_NAMESPACE_PBORIGB }} // end of namespace
#define USING_NAMESPACE_PBORIGB using namespace PBORINAME::groebner;
#else
#endif

// algorithmic variants
#define EXP_FOR_PAIRS

BEGIN_NAMESPACE_PBORIGB

class GBTypes{
  typedef polybori::BoolePolynomial poly_type;
  typedef polybori::BooleMonomial monom_type;
};
typedef polybori::BoolePolynomial Polynomial;
typedef polybori::BooleMonomial Monomial;
typedef polybori::BooleVariable Variable;
typedef polybori::BooleExponent Exponent;
typedef std::vector<Polynomial> PolynomialVector;

#ifdef PBORI_HAVE_LONG_LONG
typedef long long wlen_type;
#else
typedef long wlen_type;
#endif
typedef long len_type;
typedef int deg_type;

typedef polybori::CTypes::idx_type idx_type;
typedef polybori::BooleSet MonomialSet;
END_NAMESPACE_PBORIGB

#endif
