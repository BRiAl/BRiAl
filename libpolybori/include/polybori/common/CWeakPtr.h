// -*- c++ -*-
//*****************************************************************************
/** @file CWeakPtr.h 
 *
 * @author Alexander Dreyer
 * @date 2011-06-14
 *
 * This file includes the definition of the class @c CWeakPtr.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_CWeakPtr_h_
#define polybori_CWeakPtr_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include "CWeakPtrFacade.h"

BEGIN_NAMESPACE_PBORI

/** @class CWeakPtr
 * @brief This class defines CWeakPtr.
 *
 **/

template <class ValueType>
class CWeakPtr {

  typedef CWeakPtr self;

public:
  typedef ValueType value_type;

  typedef value_type* data_type;
  typedef boost::shared_ptr<data_type> ptr_type;


  /// Construct from something, which supports weak pointers
  explicit CWeakPtr(const CWeakPtrFacade<ValueType>& val):
    m_data(val.m_data) { PBORI_ASSERT(m_data);}

  /// Copy constructor
  CWeakPtr(const self& rhs): m_data(rhs.m_data) {}

  /// Destructor
  ~CWeakPtr() {}

  /// Dereferencing generates strong-referenced instance
  const value_type& operator*() const {
    return *(operator->()); 
  }

  /// Dereferencing generates strong-referenced instance
  value_type* operator->() const {
    if (!*m_data)
      throw std::runtime_error("Outdated weak pointer dereferenced.");

    return (*m_data);
  }
  /// Validity test
  operator bool() const { return *m_data; }

private:
  ptr_type m_data;
};

END_NAMESPACE_PBORI

#endif /* polybori_CWeakPtr_h_ */
