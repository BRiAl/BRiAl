// -*- c++ -*-
//*****************************************************************************
/** @file ReductionStrategy.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c ReductionStrategy.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ReductionStrategy_h_
#define polybori_groebner_ReductionStrategy_h_

// include basic definitions
#include "groebner_defs.h"
#include "polynomial_properties.h"
#include "PairManager.h"

BEGIN_NAMESPACE_PBORIGB

typedef Monomial::idx_map_type lm2Index_map_type;
typedef Exponent::idx_map_type exp2Index_map_type;

/** @class ReductionStrategy
 * @brief This class defines ReductionStrategy.
 *
 **/
class ReductionStrategy:public PolyEntryVector{
public:
    MonomialSet leadingTerms;
    MonomialSet minimalLeadingTerms;
    MonomialSet leadingTerms11;
    MonomialSet leadingTerms00;
    MonomialSet llReductor;
    MonomialSet monomials;
    MonomialSet monomials_plus_one;
    lm2Index_map_type lm2Index;
    exp2Index_map_type exp2Index;
    bool optBrutalReductions;
    
    bool optLL;

    Polynomial nf(Polynomial p) const;
    bool optRedTailDegGrowth;
    bool optRedTail;
    idx_type reducibleUntil;
    void setupSetsForLastElement();

    // ReductionStrategy(){ set_defaults(); }

    ReductionStrategy(const BoolePolyRing& theRing):
      leadingTerms(theRing.zero()), minimalLeadingTerms(theRing.zero()),
      leadingTerms11(theRing.zero()), leadingTerms00(theRing.zero()), llReductor(theRing.zero()),
      monomials(theRing.zero()), monomials_plus_one(theRing.zero()), lm2Index(),
      exp2Index() {
      set_defaults(); 
    }

    bool canRewrite(const Polynomial& p) const{
        return is_rewriteable(p,minimalLeadingTerms);
    }
    void addGenerator(const Polynomial& p){
        push_back(p);
        setupSetsForLastElement();
    }
    int select1(const Polynomial& p) const;
    int select1(const Monomial& m) const;

    int select_short(const Polynomial& p) const;
    int select_short(const Monomial& m) const;
    Polynomial headNormalForm(Polynomial p) const;
    
    Polynomial reducedNormalForm(Polynomial p) const;

 protected:
    void set_defaults(){
        optLL=false;
        reducibleUntil=-1;
        optBrutalReductions=true;
        optRedTail=true;
        optRedTailDegGrowth=true;
    }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_ReductionStrategy_h_ */
