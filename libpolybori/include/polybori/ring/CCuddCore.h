// -*- c++ -*-
//*****************************************************************************
/** @file CCuddCore.h
 *
 * @author Alexander Dreyer
 * @date 2007-07-19
 *
 * This files defines a 
 *
 * @par Copyright:
 *   (c) 2007-2010by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_ring_CCuddCore_h
#define polybori_ring_CCuddCore_h

// include basic definitions
#include <polybori/pbori_defs.h>

#include "CCuddInterface.h"
#include "CVariableNames.h"

// get PolyBoRi routines and functionals
#include <polybori/routines/pbori_func.h>
#include <polybori/common/traits.h>

// intrisive (shared) pointer functionality
#include <boost/intrusive_ptr.hpp>

#include <vector>
#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE_PBORI

class COrderingBase;

/** @class CCuddCore
 * @brief This class prepares the CUDD's raw decision diagram manager structure
 * for the use with instrinsive pointers.
 *
 * The purpose of this wrapper is mainly to provide the necessary functionality
 * for application of intrisive pointers (refernce counting). In addition, some
 * global settings are stored within.
 *
 * @attention This class is intented for internal use only. See CCuddDD,
 * CCuddZDD, and CCuddInterface.
 **/
class CCuddCore:
  public CTypes::orderenums_type, public CAuxTypes,
  public CWeakPtrFacade<CCuddCore> {

public:

  /// Fix type of *this
  typedef CCuddCore self;

  /// Define type for storing names of variables
  typedef CVariableNames variable_names_type;

  /// Define type for getting names of variables
  typedef variable_names_type::const_reference const_varname_reference;

  /// Current decision diagram management
  CCuddInterface m_mgr;

  /// Type for handling mterm orderings
  typedef COrderingBase order_type;
  
  /// Smart pointer for handling mterm orderings
  typedef boost::shared_ptr<order_type> order_ptr;

  /// Reference for handling mterm orderings
  typedef order_type& order_reference;

  /// Enum for ordering codes
  typedef CTypes::ordercode_type ordercode_type;

  /// Count instances pointing here
  refcount_type ref;

  /// Stores names of variables
  variable_names_type m_names;

 
  /// *Ordering of *this
  order_ptr pOrder;


  /// Initialize raw decision diagram management
  CCuddCore(size_type numVarsZ, const order_ptr& order):  
    m_mgr(0, numVarsZ), ref(0), m_names(numVarsZ), 
    pOrder(order) {  }

  /// Copy Constructor (nearly deep copy, but shallow copy of manager, names and
  /// ordering)
  CCuddCore(const self& rhs):
    m_mgr(rhs.m_mgr), ref(0), m_names(rhs.m_names), pOrder(rhs.pOrder) { }

  /// Destructor
  ~CCuddCore(){ }
    
  /// Increment reference count
  void addRef(){ ++ref; }

  /// Release this by decrementing reference counting
  refcount_type release() {
    return (--ref);
  }

  void change_ordering(const order_ptr& newOrder) {
    pOrder = newOrder;
  }

};

/// @name Prepare for the application of intrinsive pointers
//@{
/// Increment reference count
inline void 
intrusive_ptr_add_ref(CCuddCore* pCore){
  pCore->addRef();
}

/// Release current pointer by decrementing reference counting
inline void 
intrusive_ptr_release(CCuddCore* pCore) {
  if (!(pCore->release())) {
    delete pCore;
  }
}



//@}

END_NAMESPACE_PBORI

#endif


