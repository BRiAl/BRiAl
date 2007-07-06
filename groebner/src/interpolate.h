#include "groebner_defs.h"
BEGIN_NAMESPACE_PBORIGB
Polynomial interpolate(MonomialSet to_zero,MonomialSet to_one);
Polynomial interpolate_smallest_lex(MonomialSet to_zero,MonomialSet to_one);
MonomialSet zeroes(Polynomial p, MonomialSet candidates);
MonomialSet include_divisors(const MonomialSet& m);
END_NAMESPACE_PBORIGB
