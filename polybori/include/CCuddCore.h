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

// intrisive (shared) pointer functionality
#include <boost/intrusive_ptr.hpp>

// get PolyBoRi routines and functionals
#include "pbori_func.h"
#include "pbori_traits.h"

#include "CVariableNames.h"

BEGIN_NAMESPACE_PBORI

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
class CCuddCore {

public:
  ///@name Get CUDD-related type definitions
  PB_DECLARE_CUDD_TYPES(mgrcore_traits<Cudd>)

  /// Fix type of *this
  typedef CCuddCore self;

  /// Fix type for supported smart pointer
  typedef boost::intrusive_ptr<self> mgrcore_ptr;

  /// Define type for storing names of variables
  typedef CVariableNames variable_names_type;

  /// Define type for getting names of variables
  typedef variable_names_type::const_reference const_varname_reference;

  /// Current raw decision diagram management
  mgrcore_type manager;

  /// Functions for handling errors from CUDD functions
  errorfunc_type errorHandler;

  /// Control eloquence of CUDD functionality
  bool verbose;

  /// Count instances pointing here
  refcount_type ref;

  /// Stores names of variables
  variable_names_type m_names;

  /// Initialize raw decision diagram management
  CCuddCore(size_type numVars = 0,
            size_type numVarsZ = 0,
            size_type numSlots = CUDD_UNIQUE_SLOTS,
            size_type cacheSize = CUDD_CACHE_SLOTS,
            large_size_type maxMemory = 0):  
    ref(0), m_names(numVarsZ) {
    manager = Cudd_Init(numVars,numVarsZ,numSlots,cacheSize,maxMemory);
    errorHandler = defaultError; // CUDD's default error handle
    verbose = 0;		// initially terse
  }

  /// Destructor
  ~CCuddCore(){ release(); }

  /// Increment reference count
  void addRef(){ ++ref; }

  /// Release this by decrementing reference counting
  void release() {
    if (--(ref) == 0){
      int retval = Cudd_CheckZeroRef(manager);
      if UNLIKELY(retval != 0) {
        std::cerr << retval << " unexpected non-zero reference counts\n";
      } else if (verbose) {
        std::cerr << "All went well\n";
      }
      Cudd_Quit(manager);
    }
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
  pCore->release();
}
//@}

END_NAMESPACE_PBORI

#endif


