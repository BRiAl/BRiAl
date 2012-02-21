// -*- c++ -*-
//*****************************************************************************
/** @file PolyEntryReference.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-04
 *
 * This file includes the definition of the class @c PolyEntryReference.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyEntryReference_h_
#define polybori_groebner_PolyEntryReference_h_

// include basic definitions
#include "groebner_defs.h"
#include <set>

#include "PolyEntryIndices.h"
#include "PolyEntry.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PolyEntryReference
 * @brief This class defines @c PolyEntryReference. 
 *
 * It allows non-constant access to those attributes, that
 * could not cause inconsistencies, but only constant access to others
 **/
class PolyEntryReference {
  typedef PolyEntryReference self;

public:
  typedef PolyEntryIndices vector_type;
  typedef PolyEntry value_type;

  /// Construct from plain non-constant reference
  PolyEntryReference(value_type &entry, vector_type& parent): 
    vPairCalculated(entry.vPairCalculated), minimal(entry.minimal),
    m_entry(entry), m_parent(parent) { }

  /// Equality check
  bool operator==(const self& rhs) const { return m_entry == rhs; }

  /// @name The following data is accessible without yielding inconsistencies
  //@{
  void markVariablePairsCalculated() {
    return m_entry.markVariablePairsCalculated(); }

  bool propagatableBy(const PolyEntry& other) const {
    return m_entry.propagatableBy(other); 
  }

  std::set<idx_type>& vPairCalculated;
  bool& minimal;
  //@}

  /// Assignment also triggers changes in the parent
  template <class Type>
  self& operator=(const Type& rhs) {
    Monomial lm(m_entry.lead);
    m_entry = rhs;
    m_parent.update(lm, m_entry);
    return *this;
  }

  /// Constant access
  const value_type& get() const { return const_cast<const value_type&>(m_entry); }

  /// Conversion to constant reference
  operator const value_type&() const { return get(); }

private:
  value_type& m_entry;
  vector_type& m_parent;
};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyEntryReference_h_ */
