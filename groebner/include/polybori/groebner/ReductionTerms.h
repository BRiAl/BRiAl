// -*- c++ -*-
//*****************************************************************************
/** @file ReductionTerms.h 
 *
 * @author Alexander Dreyer
 * @date 2012-01-05
 *
 * This file includes the definition of the class @c ReductionTerms.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ReductionTerms_h_
#define polybori_groebner_ReductionTerms_h_

#include "ll_red_nf.h"

// include basic definitions
#include "groebner_defs.h"


BEGIN_NAMESPACE_PBORIGB

// groebner_alg.h
MonomialSet recursively_insert(MonomialSet::navigator p,
			       idx_type idx, MonomialSet mset);

class LLReductor:
  public MonomialSet {

  typedef MonomialSet base;

public:
  /// Construct reductor from Ring
  LLReductor(const BoolePolyRing& ring): base(ring.one()) {}

  /// Construct copy or MonomialSet
  template <class Type>
  LLReductor(const Type& value): base(value) { PBORI_ASSERT(!isZero()); }

  /// Assignment
  template <class Type>
  LLReductor& operator=(const Type& value) {
    PBORI_ASSERT(!base(value).isZero());
    return static_cast<LLReductor&>(base::operator=(*this));
  }

  /// Test whether polynomial is a compatible reductor element
  bool isCompatible(const PolyEntry& entry) {

    PBORI_ASSERT (!isZero());
    return  (entry.leadDeg == 1) && 
      (*(entry.p.navigation()) == entry.lead.firstIndex() ) &&
      (!expBegin()->reducibleBy(entry.lead.firstIndex()));
  }

  /// Insert polynomial if compatible, return updated polynomial
  Polynomial update(const PolyEntry& entry) {
    return (isCompatible(entry)? insert(entry): entry.p);
  }

private:
  Polynomial insert(const PolyEntry& entry) {
    
    Polynomial poly = ll_red_nf(entry.p, *this);
    PBORI_ASSERT(poly.lead() == entry.lead);
    
    operator=(recursively_insert(poly.navigation().elseBranch(),
                                 entry.lead.firstIndex(),
                                 ll_red_nf(*this, poly.set())));
    return poly;
  }

};


class MinimalLeadingTerms:
  public MonomialSet {

  typedef MonomialSet base;

public:

  template <class Type>
  MinimalLeadingTerms(const Type& value): base(value) { }

  /// Insert leading term and return monomials, that are not minimal any more
  MonomialSet update(const Monomial& lm) {

    MonomialSet divisors(divisorsOf(lm));
    if(divisors.isZero()) {
      MonomialSet removed(multiplesOf(lm).diff(lm.set()));
      PBORI_ASSERT(removed.intersect(*this).intersect(lm.set()).isZero());

      *this = diff(removed).unite(lm.set());
      PBORI_ASSERT(assertion(lm, removed.expBegin(),removed.expEnd()));

      return removed;
    }
    else if (divisors.diff(lm.set()).isZero())
      return lm.set();

    return lm.ring().zero();
  }

private:
  bool assertion(const Monomial& lm,
                 MonomialSet::exp_iterator start,
                 MonomialSet::exp_iterator finish) const {
    while (start != finish) {
      if ( (*start) == lm.exp() || !start->reducibleBy(lm.exp()) )
        return false;
      ++start;
    }
    return true;
  }
};

template <class NewType>
class TermsFacade:
  public MonomialSet {

  typedef MonomialSet base;

public:
  /// Construct copy or MonomialSet from value
  template <class Type>
  TermsFacade(const Type& value): base(value) { }

  /// Insert data (polynomial or leading term) from entry
  void update(const PolyEntry& entry) { 
    if (get().isCompatible(entry))
      operator=(unite(entry.lead.set()));
  }
  
  /// Cheap and save conversion to monomial set
  operator const base& () { return static_cast<const base&>(*this);}
  
private:
  /// Access complete type
  const NewType& get() const { return static_cast<const NewType&>(*this); }
};

class MonomialTerms:
  public TermsFacade<MonomialTerms> {

public:
  template <class Type>
  MonomialTerms(const Type& rhs): TermsFacade<MonomialTerms>(rhs) {}

  bool isCompatible(const PolyEntry& entry) const {
    PBORI_ASSERT( (entry.length != 1) || (entry.p == entry.lead) );
    return (entry.length == 1);
  }
};

class LeadingTerms:
  public TermsFacade<LeadingTerms> {

public:
  template <class Type>
  LeadingTerms(const Type& rhs): TermsFacade<LeadingTerms>(rhs) {}

  bool isCompatible(const PolyEntry&) const { return true; }
};

class LeadingTerms00:
  public TermsFacade<LeadingTerms00> {

public:
  template <class Type>
  LeadingTerms00(const Type& rhs): TermsFacade<LeadingTerms00>(rhs) {}

  bool isCompatible(const PolyEntry& entry) const {
    return entry.literal_factors.is00Factorization();
  }
};

class LeadingTerms11:
  public TermsFacade<LeadingTerms11> {

public:
  template <class Type>
  LeadingTerms11(const Type& rhs): TermsFacade<LeadingTerms11>(rhs) {}

  bool isCompatible(const PolyEntry& entry) const {
    return entry.literal_factors.is11Factorization();
  }
};


class MonomialPlusOneTerms:
  public TermsFacade<MonomialPlusOneTerms> {

public:
  template <class Type>
  MonomialPlusOneTerms(const Type& rhs): TermsFacade<MonomialPlusOneTerms>(rhs) {}

  bool isCompatible(const PolyEntry& entry) const { 
    PBORI_ASSERT( (entry.length != 2) || entry.p.isPair());
    return (entry.length == 2) && (entry.p.hasConstantPart());
  }
};



/** @class ReductionTerms
 * @brief This class defines term for @c ReductionStrategy
 *
 **/

class ReductionTerms {
  /// Type of *this
  typedef ReductionTerms self;

public:
  LeadingTerms leadingTerms;
  MinimalLeadingTerms minimalLeadingTerms;
  LeadingTerms11 leadingTerms11;
  LeadingTerms00 leadingTerms00;
  LLReductor llReductor;
  MonomialTerms monomials;
  MonomialPlusOneTerms monomials_plus_one;
  
  ReductionTerms(const BoolePolyRing& ring):
    leadingTerms(ring), minimalLeadingTerms(ring),
    leadingTerms11(ring), leadingTerms00(ring),
    llReductor(ring), monomials(ring), monomials_plus_one(ring)  { }

};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_ReductionTerms_h_ */
