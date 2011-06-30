// -*- c++ -*-
//*****************************************************************************
/** @file BlockOrderHelper.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c BlockOrderHelper.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_BlockOrderHelper_h_
#define polybori_groebner_BlockOrderHelper_h_

// include basic definitions
#include "groebner_defs.h"
#include "ReductionStrategy.h"
#include "nf.h"

BEGIN_NAMESPACE_PBORIGB

/** @class BlockOrderHelper
 * @brief This class defines BlockOrderHelper.
 *
 **/
class BlockOrderHelper{
    public:
    static bool irreducible_lead(const Monomial& m, const ReductionStrategy& strat){
      return PBORINAME::groebner::irreducible_lead(m,strat);
          }
    static Polynomial::ordered_iterator begin(const Polynomial & p){
        return p.orderedBegin();
    }
    static Polynomial::ordered_iterator end(const Polynomial & p){
        return p.orderedEnd();
    }
    static Polynomial nf(const ReductionStrategy& strat, const Polynomial& p, const Monomial& m){
        return nf3(strat,p,m);
    }
    typedef Polynomial::ordered_iterator iterator_type;
    const static bool isDegreeOrder=false;
    const static bool isLexicographicalOrder=false;
    static bool knowRestIsIrreducible(const iterator_type& it, const ReductionStrategy & strat){
      return false;
    }
    static Polynomial  sum_range(std::vector<Monomial>& vec,iterator_type it, iterator_type end, Polynomial init){
      return add_up_generic(vec, init);
        }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_BlockOrderHelper_h_ */
