/*
 *  nf.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 25.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */


#include <algorithm>
#include <vector>
#include <queue>

#include "SlimgbReduction.h"
#include "groebner_alg.h"


#ifdef PBORI_HAVE_M4RI
extern "C"{
#include <m4ri/m4ri.h>

#ifndef __M4RI_TWOPOW
#define __M4RI_TWOPOW TWOPOW
#endif
}
#endif
#ifndef PBORI_GB_NF_H
#define PBORI_GB_NF_H
BEGIN_NAMESPACE_PBORIGB

class ReductionStrategy;
int select_no_deg_growth(const ReductionStrategy& strat, const Monomial& m);

typedef SlimgbReduction<SLIMGB_SIMPLEST> slimgb_reduction_type;

std::vector<Polynomial> parallel_reduce(std::vector<Polynomial> inp, GroebnerStrategy& strat, int average_steps, double delay_f);

Polynomial nf3(const ReductionStrategy& strat, Polynomial p, Monomial rest_lead);
Polynomial nf3_short(const ReductionStrategy& strat, Polynomial p);

Polynomial plug_1(const Polynomial& p, const MonomialSet& m_plus_ones);
MonomialSet mod_mon_set(const MonomialSet& as, const MonomialSet &vs);

Polynomial cheap_reductions(const ReductionStrategy& strat, Polynomial p);

Polynomial nf3_lexbuckets(const GroebnerStrategy& strat, Polynomial p, Monomial rest_lead);
Polynomial nf3_no_deg_growth(const ReductionStrategy& strat, Polynomial p, Monomial rest_lead);
Polynomial nf3_degree_order(const ReductionStrategy& strat, Polynomial p, Monomial lead);
Polynomial nf3_short(const ReductionStrategy& strat, Polynomial p);


END_NAMESPACE_PBORIGB
#endif
