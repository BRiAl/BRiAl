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
       
        
        assert(!(Polynomial(lm).isZero()));
        MonomialSet lm_multiples_min=minimalLeadingTerms.multiplesOf(lm);
        //generators.minimalLeadingTerms.intersect(lm.multiples(minimalLeadingTerms.usedVariables()));
        lm_multiples_min=lm_multiples_min.diff(lm.diagram());
        //(lm.diagram()).diff(lm.diagram());
    
        assert(lm_multiples_min.intersect(minimalLeadingTerms).intersect(lm.diagram()).isZero());

        {
        
        
            MonomialSet::exp_iterator mfm_start=lm_multiples_min.expBegin();
            MonomialSet::exp_iterator mfm_end=lm_multiples_min.expEnd();
            while(mfm_start!=mfm_end){
                assert((*mfm_start)!=e.leadExp);
                assert((*mfm_start).reducibleBy(e.leadExp));
                (*this)[exp2Index[*mfm_start]].minimal=false;
                mfm_start++;
            }
        }
        minimalLeadingTerms = minimalLeadingTerms.diff(lm_multiples_min).unite(lm.diagram());
        

        
    } else 
    {
        //cerr<<"Warning:adding non minimal element to strategy"<<std::endl;
        //assert(false);
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
        assert(e.p.length()==1);
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


 
int ReductionStrategy::select_short(const Polynomial& p) const{
  MonomialSet ms=leadingTerms.intersect(p.leadDivisors());
  //Polynomial workaround =Polynomial(ms);
  
  if (ms.isZero())
    return -1;
  else {
    
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(*this)));
    
    int res=lm2Index.find(min)->second;
    if (((*this)[res].weightedLength<=2)/*||(strat.generators[res].ecart()==0)*/) return res;
    else return -1;
  }
  
}

int ReductionStrategy::select_short(const Monomial& m) const{
  MonomialSet ms=leadingTerms.intersect(m.divisors());
  if (ms.isZero())
    return -1;
  else {
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(*this)));
    int res=lm2Index.find(min)->second;
    if (((*this)[res].weightedLength<=2)/*||(strat.generators[res].ecart()==0)*/) return res;
    else return -1;

  }
}

typedef LessWeightedLengthInStratModified StratComparerForSelect;

int ReductionStrategy::select1( const Polynomial& p) const{
  MonomialSet ms=leadingTerms.divisorsOf(p.lead());//strat.leadingTerms.intersect(p.leadDivisors());
  //Polynomial workaround =Polynomial(ms);
  
  if (ms.isZero())
    return -1;
  else {
#ifdef LEX_LEAD_RED_STRAT
    if (p.ring().ordering().isLexicographical()){
      Exponent min=*(ms.expBegin());
      return exp2Index.find(min)->second;
    }
#endif
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Exponent min=*(std::min_element(ms.expBegin(),ms.expEnd(), StratComparerForSelect(*this)));

    return exp2Index.find(min)->second;
     
  }
  
}
int ReductionStrategy::select1(const Monomial& m) const {
  MonomialSet ms=leadingTerms.divisorsOf(m);
  if (ms.isZero())
    return -1;
  else {
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Exponent min=*(std::min_element(ms.expBegin(),ms.expEnd(), StratComparerForSelect(*this)));
    return exp2Index.find(min)->second;
  }
}

Polynomial ReductionStrategy::reducedNormalForm(Polynomial p) const{
    if UNLIKELY(p.isZero()) return p;
    
    Polynomial res(p.ring());
    if (p.ring().ordering().isDegreeOrder()) res=nf3_degree_order(*this,p,p.lead());
    else res=nf3(*this,p,p.lead());
    if ((res.isZero())) return res;
    res=red_tail(*this,res);
    return res;
}
Polynomial ReductionStrategy::headNormalForm(Polynomial p) const{
    if UNLIKELY(p.isZero()) return p;
    
    Polynomial res(p.ring());
    if (p.ring().ordering().isDegreeOrder()) res=nf3_degree_order(*this,p,p.lead());
    else res=nf3(*this,p,p.lead());
    return res;
}
Polynomial ReductionStrategy::nf(Polynomial p) const{
    if (optRedTail) return reducedNormalForm(p);
    else return headNormalForm(p);
}

END_NAMESPACE_PBORIGB
