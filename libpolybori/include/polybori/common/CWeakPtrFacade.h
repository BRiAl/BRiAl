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

/** @class CWeakPtrFacade
 * @brief This class defines CWeakPtrFacade. 
 *
 * It should be the inheritance base for all classes supporting @c CWeakPtr.
 **/

class CWeakPtrFacade {
  /// Type of *this
  typedef CWeakPtrFacade self;

public:
  typedef self* data_type;
  typedef boost::shared_ptr<data_type> ptr_type;

  /// Default constructor (only data is the pointer to here)
  CWeakPtrFacade(): p_data(new data_type(this)) {}

  /// Copy constructor (constructs new facade)
  CWeakPtrFacade(const self& rhs): p_data(new data_type(this)) {}

  /// Destructor (invalidates pointer to here)
  ~CWeakPtrFacade() { (*p_data) = NULL; }

  const ptr_type get() const { return p_data; }
private:
  ptr_type p_data;
};

END_NAMESPACE_PBORI

#endif /* polybori_CWeakPtrFacade_h_ */
