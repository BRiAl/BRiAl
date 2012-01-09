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
#include "PolyEntryVector.h"
#include "ReductionOptions.h"
#include "ReductionTerms.h"

BEGIN_NAMESPACE_PBORIGB

template <class KeyType, bool Value>
class set_associated_minimal {
public:
  set_associated_minimal(PolyEntryVector& strat): m_strat(strat) {}

  void operator()(const KeyType& key) const { 
    m_strat.access(key).minimal = Value;
  }

private:
  PolyEntryVector& m_strat;
};

/** @class ReductionStrategy
 * @brief This class defines ReductionStrategy.
 *
 **/

class ReductionStrategy:
  public PolyEntryVector, public ReductionOptions, public ReductionTerms {

public:

    ReductionStrategy(const BoolePolyRing& ring):
      PolyEntryVector(), ReductionOptions(),
      ReductionTerms(ring)  { }

    Polynomial nf(const Polynomial& p) const {
      return (optRedTail? reducedNormalForm(p): headNormalForm(p));
    }

    bool canRewrite(const Polynomial& p) const {
      return is_rewriteable(p, minimalLeadingTerms);
    }


//   template <class ElementType>
//   void push_back(const ElementType& element) {
//     PolyEntryVector::push_back(element);
//     setupSetsForElement(back());
//   }

    void addGenerator(const Polynomial& p) {
        append(p);
        setupSetsForElement(back());
    }

    void setupSetsForLastElement() { setupSetsForElement(back());
    }  // @todo
  // needed
  // for GBred
  
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

protected:

  int select_short_by_terms(const MonomialSet&) const;

  void unmarkNonMinimalLeadingTerms(MonomialSet removed) {
    std::for_each(removed.expBegin(), removed.expEnd(),
		  set_associated_minimal<Exponent, false>(*this));
  }
 
  //  void setupSetsForElement(const PolyEntry& entry);
  void setupSetsForElement(const PolyEntry& entry) {
    
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


  template <class Iterator, class CompareType>
  size_type minimum(Iterator start, Iterator finish, const CompareType& comp)
    const {
    start = std::min_element(start, finish, comp);
    if UNLIKELY(start == finish)
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
