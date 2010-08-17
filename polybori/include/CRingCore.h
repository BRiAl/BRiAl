// -*- c++ -*-
//*****************************************************************************
/** @file CRingCore.h
 *
 * @author Alexander Dreyer
 * @date 2010-08-09
 *
 * Defines a class, which stores the data of a polynomial ring
 *
 * @par Copyright:
 *   (c) 2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// intrusive (smart shared) pointer functionality
#include <boost/intrusive_ptr.hpp>

// Store names of variables
#include "CVariableNames.h"

// get Cudd library
#include "cudd.h"

BEGIN_NAMESPACE_PBORI

/// @class CRingCore
class CRingCore {
public:
  /// Type of Cudd's decision diagram manager
  typedef DdManager* mgr_type;

  /// Smartpointer to manager
  typedef boost::intrusive_ptr<mgr_type> mgr_ptr;

  /// Define type for storing names of variables
  typedef CVariableNames variable_names_type;

  /// Define type for getting names of variables
  typedef variable_names_type::const_reference const_varname_reference;

  /// Type for handling mterm orderings
  typedef COrderingBase order_type;
  
  /// Smart pointer for handling mterm orderings
  typedef PBORI_SHARED_PTR(order_type) order_ptr;

};


END_NAMESPACE_PBORI
