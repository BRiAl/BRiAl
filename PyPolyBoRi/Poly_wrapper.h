/*
 *  Poly_wrapper.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 04.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#ifndef POLY_WRAPPER_HEADER
#define POLY_WRAPPER_HEADER
USING_NAMESPACE_PBORI
void export_poly();
static inline BooleMonomial lead_wrap(const BoolePolynomial& p){
    return p.lead();
}
static inline BooleMonomial lex_lead_wrap(const BoolePolynomial& p){
    return p.lexLead();
}
static inline BoolePolynomial::deg_type lead_deg_wrap(const BoolePolynomial& p){
    return p.leadDeg();
}
static inline BoolePolynomial::deg_type lex_lead_deg_wrap(const BoolePolynomial& p){
    return p.lexLeadDeg();
}
#endif

