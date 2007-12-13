/*  Copyright (c) 2005-2007 by The PolyBoRi Team */

#include "groebner_defs.h"
BEGIN_NAMESPACE_PBORIGB
MonomialSet variety_leading_terms(const MonomialSet& points, const Monomial& variables);
Polynomial interpolate(MonomialSet to_zero,MonomialSet to_one);
Polynomial interpolate_smallest_lex(MonomialSet to_zero,MonomialSet to_one);
MonomialSet zeroes(Polynomial p, MonomialSet candidates);
MonomialSet include_divisors(const MonomialSet& m);
END_NAMESPACE_PBORIGB
