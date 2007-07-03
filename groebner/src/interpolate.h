#include "groebner_defs.h"
BEGIN_NAMESPACE_PBORIGB
Polynomial interpolate(MonomialSet to_zero,MonomialSet to_one);
Polynomial interpolate_smallest_lex(MonomialSet to_zero,MonomialSet to_one);
MonomialSet zeroes(Polynomial p, MonomialSet candidates);
END_NAMESPACE_PBORIGB
