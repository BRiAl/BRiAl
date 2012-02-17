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
#include "PolyEntryVector.h"
#include "ReductionOptions.h"
#include "ReductionTerms.h"
#include "SetAssociatedMinimal.h"

BEGIN_NAMESPACE_PBORIGB

/** @class ReductionStrategy
 * @brief This class defines ReductionStrategy.
 *
 **/

class ReductionStrategy:
  public PolyEntryVector, public ReductionOptions, public ReductionTerms {
  typedef ReductionStrategy self;
public:

    ReductionStrategy(const BoolePolyRing& ring):
      PolyEntryVector(), ReductionOptions(),
      ReductionTerms(ring)  { }

    /// Adding next element
    void addGenerator(const PolyEntry& entry) {
      PolyEntryVector::append(entry);
      setupSetsForElement(back());
    }

    /// Adding next generator
    /// @note overwriting virtual to avoid inconsistency when casting
    void append(const PolyEntry& entry) { addGenerator(entry); }

    Polynomial nf(const Polynomial& p) const {
      return (optRedTail? reducedNormalForm(p): headNormalForm(p));
    }

    bool canRewrite(const Polynomial& p) const {
      return is_rewriteable(p, minimalLeadingTerms);
    }

    int select1(const Polynomial& p) const;
    int select1(const Monomial& m) const;

    int select_short(const Polynomial& p) const {
      return select_short_by_terms(p.leadDivisors());
    }
    int select_short(const Monomial& m) const {
      return select_short_by_terms(m.divisors());
    }

    Polynomial headNormalForm(const Polynomial& p) const;
    Polynomial reducedNormalForm(const Polynomial& p) const;

    void llReduceAll();

    operator const PolyEntryVector&() const {
      return static_cast<const PolyEntryVector&>(*this);
    }

protected:
  void llReduce(const PolyEntry& entry, const Exponent& ll_e);

  int select_short_by_terms(const MonomialSet&) const;

  void unmarkNonMinimalLeadingTerms(MonomialSet removed) {
    std::for_each(removed.expBegin(), removed.expEnd(),
		  SetAssociatedMinimal<Exponent, false>(*this));
  }
 
  void setupSetsForElement(const PolyEntry& entry);


  template <class Iterator, class CompareType>
  size_type minimum(Iterator start, Iterator finish, const CompareType& comp)
    const {
    start = std::min_element(start, finish, comp);
    if PBORI_UNLIKELY(start == finish)
      return size_type(-1);
    
    return index(*start);
  }

};


inline bool
irreducible_lead(Monomial lm, const ReductionStrategy& strat){

  return (!(strat.minimalLeadingTerms.hasTermOfVariables(lm)));//
  //        strat.generators.minimalLeadingTerms.intersect(lm.divisors()).isZero();
}




END_NAMESPACE_PBORIGB

#endif /* polybori_ReductionStrategy_h_ */
