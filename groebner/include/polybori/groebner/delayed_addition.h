#ifndef PBORI_GB_DELAYED_ADDITION_H
#define PBORI_GB_DELAYED_ADDITION_H
BEGIN_NAMESPACE_PBORIGB
//consumes vector content
std::vector<Polynomial> translate_from_lex_sorted_exponent_vectors(std::vector<std::vector<Exponent> >& conversion_vector, BoolePolyRing ring);
END_NAMESPACE_PBORIGB
#endif