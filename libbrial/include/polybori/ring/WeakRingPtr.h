// -*- c++ -*-
//*****************************************************************************
/** @file WeakRingPtr.h 
 *
 * @author Alexander Dreyer
 * @date 2011-06-14
 *
 * This file includes the definition of the class @c WeakRingPtr.
 *
 * @par Copyright:
 *   (c) 2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_WeakRingPtr_h_
#define polybori_WeakRingPtr_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/common/CWeakPtr.h>
#include <polybori/BoolePolyRing.h>

BEGIN_NAMESPACE_PBORI

/** @class WeakRingPtr
 * @brief This class defines WeakRingPtr.
 *
 * This is just a wrapper for @c CWeakPtr<BoolePolyRing::core_type>.
 * For aceesing such internals it is good @c friend with @c BoolePolyRing. 
 **/
class WeakRingPtr {
  /// Type of *this
  typedef WeakRingPtr self;

public:
  typedef BoolePolyRing value_type;

  /// Default constructor
  WeakRingPtr(const value_type& data): m_data(*data.core()) {}

  /// Copy constructor
  WeakRingPtr(const self& rhs): m_data(rhs.m_data) {}

  /// Destructor
  ~WeakRingPtr() {}

  /// Dereferencing 
  value_type operator*() const { return value_type(m_data); }

  /// Validity check
  operator bool() const { return m_data; }

private:
  CWeakPtr<value_type::core_type> m_data;
};

END_NAMESPACE_PBORI

#endif /* polybori_WeakRingPtr_h_ */
