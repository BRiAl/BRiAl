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

BEGIN_NAMESPACE_PBORIGB

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
END_NAMESPACE_PBORIGB
