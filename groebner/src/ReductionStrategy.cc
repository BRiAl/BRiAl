// -*- c++ -*-
//*****************************************************************************
/** @file ReductionStrategy.cc 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c ReductionStrategy.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************


// include basic definitions
#include <polybori/groebner/ReductionStrategy.h>
#include <polybori/groebner/LessWeightedLengthInStrat.h>
#include <polybori/groebner/LessWeightedLengthInStratModified.h>
#include <polybori/groebner/nf.h>
#include <polybori/groebner/red_tail.h>


BEGIN_NAMESPACE_PBORIGB


/// @note do not inline (optimization screws for some reason)
/// @todo Check whether this is still the case!
/// see: HeuristicQuickTestCase.test_mult6x6mit_lp 
///      HeuristicCryptoTestCase.test_ctc_Nr3_B7_n20joined_dp_asc
void ReductionStrategy::setupSetsForElement(const PolyEntry& entry) {
  
  PBORI_ASSERT(entry.lead.exp() == entry.leadExp);
  unmarkNonMinimalLeadingTerms( minimalLeadingTerms.update(entry.lead) );
  
  leadingTerms.update(entry);
  leadingTerms00.update(entry); //doesn't need to be undone on simplification
  leadingTerms11.update(entry);
  
  monomials.update(entry);
  
#ifdef LL_RED_FOR_GROEBNER
  if (optLL) {
    Polynomial updated = llReductor.update(entry);
    if (updated != entry.p)
      exchange(entry.lead, updated);
  }
#endif
}

int
ReductionStrategy::select_short_by_terms(const MonomialSet& terms) const {
  MonomialSet ms(leadingTerms.intersect(terms));
  int res = minimum(ms.begin(), ms.end(), LessWeightedLengthInStrat(*this));

  if (res == -1 || (*this)[res].weightedLength<=2)
    return res;

  return -1;
}
 

typedef LessWeightedLengthInStratModified StratComparerForSelect;


int
ReductionStrategy::select1(const Polynomial& p) const {

#ifdef LEX_LEAD_RED_STRAT
  if (p.ring().ordering().isLexicographical()) {
    MonomialSet ms = leadingTerms.divisorsOf(p.lexLead());
    return (ms.isZero()?  -1:  index(*ms.expBegin()));
  }
#endif

  return select1(p.lead());
}

int
ReductionStrategy::select1(const Monomial& m) const {
  MonomialSet ms(leadingTerms.divisorsOf(m));
  return minimum(ms.expBegin(), ms.expEnd(), StratComparerForSelect(*this));
}

Polynomial
ReductionStrategy::reducedNormalForm(const Polynomial& p) const {
  
  return red_tail(*this, headNormalForm(p));
}

Polynomial
ReductionStrategy::headNormalForm(const Polynomial& p) const {
  if PBORI_UNLIKELY(p.isZero()) return p;
    
  return (p.ring().ordering().isDegreeOrder()?
	  nf3_degree_order(*this, p, p.lead()): nf3(*this, p, p.lead()));
}


void ReductionStrategy::llReduceAll() {
  Exponent ll_e = *(llReductor.expBegin());
  const_iterator start(begin()), finish(end());
  for(; start != finish; ++start)
    llReduce(*start, ll_e);
}

void ReductionStrategy::llReduce(const PolyEntry& entry, const Exponent& ll_e){

  if ((entry.minimal) && (ll_e.GCD(entry.tailVariables).deg() > 0)) {
    Polynomial tail = ll_red_nf(entry.tail, llReductor);
    if (tail != entry.tail) {
      operator()(entry) = tail + entry.lead;
      monomials.update(entry);
    }
  }
}

END_NAMESPACE_PBORIGB
