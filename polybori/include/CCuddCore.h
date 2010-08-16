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
 *   (c) 2007 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.11  2009/06/21 22:57:28  dreyer
 * CHANGE: preparing ring-cloning (deep copy) even more
 *
 * Revision 1.10  2009/06/21 22:46:28  dreyer
 * CHANGE: preparing ring-cloning (deep copy)
 *
 * Revision 1.9  2008/04/12 22:22:36  dreyer
 * + CCuddCore even cleaner
 *
 * Revision 1.8  2008/04/10 12:45:01  dreyer
 * Fix: memory leak
 *
 * Revision 1.7  2007/12/19 10:40:34  dreyer
 * CHANGE: make Cudd-related globals initialization static
 *
 * Revision 1.6  2007/12/19 09:03:16  dreyer
 * CHANGE: make Cudd-related globals static
 *
 * Revision 1.5  2007/12/18 22:05:40  dreyer
 * CHANGE: persistent variables computed on manaer initialization
 *
 * Revision 1.4  2007/12/18 10:20:16  dreyer
 * CHANGE CNamedManager removed, names are in core now
 *
 * Revision 1.3  2007/12/18 08:28:03  dreyer
 * CHANGE: minor changes
 *
 * Revision 1.2  2007/11/06 15:03:33  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.1  2007/07/19 11:41:47  dreyer
 * CHANGE: clean-up
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef CCuddCore_h
#define CCuddCore_h

// include basic definitions
#include "pbori_defs.h"

#include "CCuddInterface.h"
#include "CVariableNames.h"

// get PolyBoRi routines and functionals
#include "pbori_func.h"
#include "pbori_traits.h"

// intrisive (shared) pointer functionality
#include <boost/intrusive_ptr.hpp>

#include <vector>


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
  public CTypes::orderenums_type {

public:
  ///@name Get CUDD-related type definitions
  PB_DECLARE_CUDD_TYPES(mgrcore_traits<Cudd>)

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
  typedef PBORI_SHARED_PTR(order_type) order_ptr;

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
    ref(0), m_names(numVarsZ), 
    m_mgr(0, numVarsZ), pOrder(order) {

  }

  /// Copy Constructor (nearly deep copy, but shallow copy of manager, names and
  /// ordering)
  CCuddCore(const self& rhs):
    ref(0), m_mgr(rhs.m_mgr), m_names(rhs.m_names), pOrder(rhs.pOrder) { }

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


