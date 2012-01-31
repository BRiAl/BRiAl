// -*- c++ -*-
//*****************************************************************************
/** @file LLReductor.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c LLReductor.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LLReductor_h_
#define polybori_groebner_LLReductor_h_

#include "ll_red_nf.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

// groebner_alg.h
MonomialSet recursively_insert(MonomialSet::navigator p,
			       idx_type idx, MonomialSet mset);

/** @class LLReductor
 * @brief This class defines LLReductor.
 *
 **/
class LLReductor:
  public MonomialSet {

  typedef MonomialSet base;
  typedef LLReductor self;
public:
  /// Construct reductor from Ring
  LLReductor(const BoolePolyRing& ring): base(ring.one()) {}

  /// Construct copy or MonomialSet
  template <class Type>
  LLReductor(const Type& value): base(value) { PBORI_ASSERT(!isZero()); }


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
  self& operator=(const self& rhs) {
    return static_cast<self&>(static_cast<base&>(*this) = rhs);
  }

  Polynomial insert(const PolyEntry& entry) {
    
    Polynomial poly = ll_red_nf(entry.p, *this);
    PBORI_ASSERT(poly.lead() == entry.lead);
    
    operator=(recursively_insert(poly.navigation().elseBranch(),
                                 entry.lead.firstIndex(),
                                 ll_red_nf(*this, poly.set())));
    return poly;
  }

};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_LLReductor_h_ */
