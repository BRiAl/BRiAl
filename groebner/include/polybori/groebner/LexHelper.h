// -*- c++ -*-
//*****************************************************************************
/** @file LexHelper.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c LexHelper.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LexHelper_h_
#define polybori_groebner_LexHelper_h_

// include basic definitions
#include "groebner_defs.h"
#include "IsEcart0Predicate.h"
#include "ReductionStrategy.h"
#include "nf.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LexHelper
 * @brief This class defines LexHelper.
 *
 **/
class LexHelper{
    public:
    static bool irreducible_lead(const Monomial& m, const ReductionStrategy& strat){
        if (strat.optRedTailDegGrowth) return PBORINAME::groebner::irreducible_lead(m,strat);
        else{
            BooleSet ms=strat.leadingTerms.intersect(m.divisors());
            if (ms.isZero())
                return true;
            else {
                return std::find_if(ms.expBegin(),ms.expEnd(),IsEcart0Predicate(strat))==ms.expEnd();
            }
        }
        
    }
    static Polynomial::const_iterator begin(const Polynomial & p){
        return p.begin();
    }
    static Polynomial::const_iterator end(const Polynomial & p){
        return p.end();
    }
    static Polynomial nf(const ReductionStrategy& strat, const Polynomial& p, const Monomial& m){
        //return nf3_lexbuckets(strat,p,m);
        if (strat.optRedTailDegGrowth) return nf3(strat,p,m);
        else return nf3_no_deg_growth(strat,p,m);
    }
    typedef Polynomial::const_iterator iterator_type;
    const static bool isDegreeOrder=false;
    const static bool isLexicographicalOrder=true;
    static bool knowRestIsIrreducible(const iterator_type& it, const ReductionStrategy & strat){
      if ( (it.deg()>0) && (it.firstIndex()>strat.reducibleUntil))
        return true;
      else return false;
      
    }
  static Polynomial sum_range(std::vector<Monomial>& vec,const iterator_type&
  it, const iterator_type& end, Polynomial init){
        if (vec.size()==1) return vec[0];
        if (it!=end)
          return term_accumulate(it,end, init);
        else return init;
    }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LexHelper_h_ */
