// -*- c++ -*-
//*****************************************************************************
/** @file TermsFacade.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c TermsFacade.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_TermsFacade_h_
#define polybori_groebner_TermsFacade_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class TermsFacade
 * @brief This class defines TermsFacade.
 *
 **/

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
  
private:
  TermsFacade& operator=(const TermsFacade& rhs) {
    return static_cast<TermsFacade&>(static_cast<base&>(*this) = rhs);
  }

  /// Access complete type
  const NewType& get() const { return static_cast<const NewType&>(*this); }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_TermsFacade_h_ */
