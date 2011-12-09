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


void addPolynomialToReductor(Polynomial& p, MonomialSet& m); // groebner_alg.cc


void ReductionStrategy::setupSetsForLastElement(){
    const int s=size()-1;
    PolyEntry e=(*this)[s];
    Monomial lm=e.lead;
    MonomialSet divisors_from_minimal=minimalLeadingTerms.divisorsOf(lm);//intersect(lm.divisors());

    if(divisors_from_minimal.isZero()){
       
        
        PBORI_ASSERT(!(Polynomial(lm).isZero()));
        MonomialSet lm_multiples_min=minimalLeadingTerms.multiplesOf(lm);
        //generators.minimalLeadingTerms.intersect(lm.multiples(minimalLeadingTerms.usedVariables()));
        lm_multiples_min=lm_multiples_min.diff(lm.diagram());
        //(lm.diagram()).diff(lm.diagram());
    
        PBORI_ASSERT(lm_multiples_min.intersect(minimalLeadingTerms).intersect(lm.diagram()).isZero());

        {
        
        
            MonomialSet::exp_iterator mfm_start=lm_multiples_min.expBegin();
            MonomialSet::exp_iterator mfm_end=lm_multiples_min.expEnd();
            while(mfm_start!=mfm_end){
                PBORI_ASSERT((*mfm_start)!=e.leadExp);
                PBORI_ASSERT((*mfm_start).reducibleBy(e.leadExp));
                (*this)[exp2Index[*mfm_start]].minimal=false;
                mfm_start++;
            }
        }
        minimalLeadingTerms = minimalLeadingTerms.diff(lm_multiples_min).unite(lm.diagram());
        

        
    } else 
    {
        //cerr<<"Warning:adding non minimal element to strategy"<<std::endl;
        //PBORI_ASSERT(false);
        if (!(divisors_from_minimal.diff(lm.diagram()).isZero()))
            (*this)[s].minimal=false;
    }
    leadingTerms = leadingTerms.unite(Polynomial(lm).diagram());
    if ((*this)[s].literal_factors.is11Factorization())
        leadingTerms11 = leadingTerms11.unite(Polynomial(lm).diagram());
    //doesn't need to be undone on simplification
    if ((*this)[s].literal_factors.is00Factorization())
        leadingTerms00 = leadingTerms00.unite(Polynomial(lm).diagram());
    lm2Index[(*this)[s].lead]=s;
    exp2Index[(*this)[s].leadExp]=s;

    
    if (e.length==1){
        PBORI_ASSERT(e.p.length()==1);
        monomials=monomials.unite(e.p.diagram());
    } //else treat_m_p_1_case(e);
    #ifdef LL_RED_FOR_GROEBNER
    if (optLL){

            if (((*this)[s].leadDeg==1) &&(*((*this)[s].p.navigation())==*((*this)[s].lead.diagram().navigation()))){
                addPolynomialToReductor((*this)[s].p,llReductor);
            }
    }
    #endif
}

template <class CompareType>
Exponent
min_exponent(const MonomialSet& ms, const CompareType& comp) {
  
  return *(std::min_element(ms.expBegin(), ms.expEnd(), comp));
}

template <>
Exponent
min_exponent(const MonomialSet& ms, const lex_tag&) {

  return *ms.expBegin();
}

template <class CompareType>
int
ReductionStrategy::min_element_index(const MonomialSet& ms,
				     const CompareType& comp) const {
  if (ms.isZero())
    return -1;

  return exp2Index.find(min_exponent(ms, comp))->second;
}

int
ReductionStrategy::select_short_by_terms(const MonomialSet& terms) const {

  int res = min_element_index(leadingTerms.intersect(terms),
			      LessWeightedLengthInStrat(*this));

  if (res == -1 || (*this)[res].weightedLength<=2)
    return res;

  return -1;
}
 
int
ReductionStrategy::select_short(const Polynomial& p) const {
  return select_short_by_terms(p.leadDivisors());
}

int
ReductionStrategy::select_short(const Monomial& m) const {
  return select_short_by_terms(m.divisors());
}

typedef LessWeightedLengthInStratModified StratComparerForSelect;

int
ReductionStrategy::select1(const Polynomial& p) const {

#ifdef LEX_LEAD_RED_STRAT
  if (p.ring().ordering().isLexicographical())
    return min_element_index(leadingTerms.divisorsOf(p.lexLead()), lex_tag());
#endif

  return select1(p.lead());
}

int
ReductionStrategy::select1(const Monomial& m) const {

  return min_element_index(leadingTerms.divisorsOf(m),
			   StratComparerForSelect(*this));
}

Polynomial
ReductionStrategy::reducedNormalForm(const Polynomial& p) const {
  
  Polynomial res(headNormalForm(p));  
  return (res.isZero()? res: red_tail(*this, res));
}

Polynomial
ReductionStrategy::headNormalForm(const Polynomial& p) const {
  if UNLIKELY(p.isZero()) return p;
    
  return (p.ring().ordering().isDegreeOrder()?
	  nf3_degree_order(*this, p, p.lead()): nf3(*this, p, p.lead()));
}

Polynomial
ReductionStrategy::nf(const Polynomial& p) const {
  return (optRedTail? reducedNormalForm(p): headNormalForm(p));
}

END_NAMESPACE_PBORIGB
