// -*- c++ -*-
//*****************************************************************************
/** @file CPrintOperation.h
 *
 * @author Alexander Dreyer
 * @date 2006-07-04
 *
 * This file prints a string literal as operation.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************
// include standard stream capabilities
#include <iostream>

// include basic definitions
#include "polybori/pbori_defs.h"

#ifndef CPrint_Operation_h_
#define CPrint_Operation_h_

BEGIN_NAMESPACE_PBORI

/** @class CPrintOperation
 * @brief This template prints a string literal to stream as operation.
 **/
template <class ValueType, class LiteralType, class StreamType = std::ostream>
class CPrintOperation {
public:
 
  /// Define stream type
  typedef StreamType stream_type;

  /// Define value type
  typedef ValueType value_type;

  /// Define for preceeding string literal
  typedef LiteralType literal_type;

  /// (Default) constructor
  CPrintOperation(stream_type& os_): os(os_){}

  /// Print operator
  value_type& operator()(value_type& val, ...) const {
    os << literal_type()();
    return val;
  }
  /// Print operator
  const value_type& operator()(const value_type& val, ...) const {
    os << literal_type()();
    return val;
  }
protected:
  std::ostream& os;
};



END_NAMESPACE_PBORI

#endif
