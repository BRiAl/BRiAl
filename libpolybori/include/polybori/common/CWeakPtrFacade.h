// -*- c++ -*-
//*****************************************************************************
/** @file CWeakPtrFacade.h 
 *
 * @author Alexander Dreyer
 * @date 2011-06-14
 *
 * This file includes the definition of the class @c CWeakPtrFacade.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_CWeakPtrFacade_h_
#define polybori_CWeakPtrFacade_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <boost/shared_ptr.hpp>


BEGIN_NAMESPACE_PBORI

/// Forward declaration of friend
template <class ValueType>
class CWeakPtr;

/** @class CWeakPtrFacade
 * @brief This class defines CWeakPtrFacade. 
 *
 * It should be the inheritance base for all classes supporting @c CWeakPtr.
 **/
template <class ValueType>
class CWeakPtrFacade {
  /// Type of *this
  typedef CWeakPtrFacade self;

public:
  typedef ValueType value_type;
  typedef value_type* data_type;
  typedef boost::shared_ptr<data_type> ptr_type;

  friend class CWeakPtr<value_type>;

  /// Default constructor (only data is the pointer to here)
  CWeakPtrFacade():
    m_data(new data_type(static_cast<data_type>(this)))  {}

  /// Copy constructor (constructs new facade)
  CWeakPtrFacade(const self& rhs):
    m_data(new data_type(static_cast<data_type>(this)))  {}

  /// Destructor (invalidates pointer to here)
  ~CWeakPtrFacade() { (*m_data) = NULL; }

protected:
  ptr_type m_data;
};

END_NAMESPACE_PBORI

#endif /* polybori_CWeakPtrFacade_h_ */
