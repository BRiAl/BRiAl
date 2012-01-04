// -*- c++ -*-
//*****************************************************************************
/** @file PolyEntryReference.h 
 *
 * @author Alexander Dreyer
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
#define polybori_groebner_PolyEntry_h_

#include "PolyEntry.h"

// include basic definitions
#include "groebner_defs.h"
#include <set>

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
  /// Construct from plain non-constant reference
  PolyEntryReference(PolyEntry &entry): 
    m_entry(entry), minimal(entry.minimal),
    vPairCalculated(entry.vPairCalculated) { }

  /// Equality check
  bool operator==(const self& rhs) const { return m_entry == rhs; }

  /// @name The following data is accessible without yielding inconsistencies
  //@{
  void markVariablePairsCalculated() {
    return m_entry.markVariablePairsCalculated(); }

  std::set<idx_type>& vPairCalculated;
  bool& minimal;
  //@}

  /// Constant access
  const PolyEntry& get() const { return const_cast<const PolyEntry&>(m_entry); }

  /// Conversion to constant reference
  operator const PolyEntry&() const { return get(); }

private:
  PolyEntry& m_entry;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyEntryReference_h_ */
