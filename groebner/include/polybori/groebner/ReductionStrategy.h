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

BEGIN_NAMESPACE_PBORIGB

/** @class ReductionOptions
 * @brief This class defines option values for the ReductionStrategy.
 *
 **/
class ReductionOptions {
public:
  ReductionOptions():
    reducibleUntil(-1),
    optLL(false), optBrutalReductions(true),
    optRedTail(true), optRedTailDegGrowth(true) {}

  bool optBrutalReductions;
  bool optLL;
  bool optRedTailDegGrowth;
  bool optRedTail;
  idx_type reducibleUntil;
};


template <class Type>
class ExponentView {
public:
  ExponentView(const Type& val): m_val(val) {}

  typename Type::exp_iterator begin() const { return m_val.expBegin(); }
  typename Type::exp_iterator end() const { return m_val.expEnd(); }

 private:
  const Type& m_val;
};

/** @class ReductionStrategy
 * @brief This class defines ReductionStrategy.
 *
 **/
class ReductionStrategy:
  public PolyEntryVector, public ReductionOptions {

public:
    MonomialSet leadingTerms;
    MonomialSet minimalLeadingTerms;
    MonomialSet leadingTerms11;
    MonomialSet leadingTerms00;
    MonomialSet llReductor;
    MonomialSet monomials;
    MonomialSet monomials_plus_one;

    ReductionStrategy(const BoolePolyRing& ring):
      PolyEntryVector(), ReductionOptions(),

      leadingTerms(ring), minimalLeadingTerms(ring),
      leadingTerms11(ring), leadingTerms00(ring),
      llReductor(ring), monomials(ring), monomials_plus_one(ring)  { }

    Polynomial nf(const Polynomial& p) const {
      return (optRedTail? reducedNormalForm(p): headNormalForm(p));
    }

    void setupSetsForLastElement();



    bool canRewrite(const Polynomial& p) const {
      return is_rewriteable(p, minimalLeadingTerms);
    }
  
    void addGenerator(const Polynomial& p) {
        push_back(p);
        setupSetsForLastElement();
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

protected:
  int select_short_by_terms(const MonomialSet&) const;


  template <class Iterator>
  void unmarkNonminimalLeadingTerms(const MonomialSet&, Iterator, Iterator);
  void removeNonminimalLeadingTerms(const MonomialSet&, MonomialSet);

  void updateMonomials(const PolyEntry&);
  void updateLeadingTerms(const PolyEntry&);
  void updateMinimalLeadingTerms(PolyEntry&);
  void updateLLReductor(PolyEntry&);
  void setupSetsForElement(PolyEntry& entry);


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
