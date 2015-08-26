// -*- c++ -*-
//*****************************************************************************
/** @file CCallbackWrapper.h 
 *
 * @author Alexander Dreyer
 * @date 2010-08-30
 *
 * This file define a functional, which wraps the callback operator .*, by
 * managing  member function pointers and the corresponding object.
 *
 * @par Copyright:
 *   (c) 2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_ring_CCallback_Wrapper_h_
#define polybori_ring_CCallback_Wrapper_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include "CMemberFunctionTraits.h"

BEGIN_NAMESPACE_PBORI

/** @class CCallbackFacade
 * @brief This template class defines a facade for applying operator @c .* in
 * @c operator() .
 * 
 * If applies a stored member function pointer (together with a stored object
 * reference) to the argument of operator().
 *
 * It is to be used as a face of @c Type, e. g. for @c CCallbackWrapper below.
 *
 * @note Specialized variant to void member functions
 * @attention Currently, the class supports unary functions only.
 **/

/// Variant for constant non-void member functions
template <class Type, class ResultType, class ArgType>
class CCallbackFacade {
public:
 
  /// Apply member function pointer to argument
  ResultType operator()(ArgType arg) const {
    return (static_cast<const Type&>(*this).object .* 
            static_cast<const Type&>(*this).function)(arg);
  }
};

/// Specialized variant for constant void member functions
template <class Type, class ArgType>
class CCallbackFacade<Type, void, ArgType> {
public:
 
  /// Apply member function pointer to argument (avoid returning @c void())
  void operator()(ArgType arg) const {
    (static_cast<const Type&>(*this).object .* 
     static_cast<const Type&>(*this).function)(arg);
  }
};

/** @class CCallbackWrapper
 * @brief This template class defines a functional, which wraps operator .*,
 * which is the callback of a dynamic member function wrt. a given object. 
 *
 * A reference of a given object is stored as well as the function pointer. @c
 * operator() does the actual application.
 *
 **/
template <class MemberFuncPtr>
class CCallbackWrapper:
  public CCallbackFacade< CCallbackWrapper<MemberFuncPtr>, 
              typename CMemberFunctionTraits<MemberFuncPtr>::result_type,
              typename CMemberFunctionTraits<MemberFuncPtr>::argument_type> {
  /// Type of *this
  typedef CCallbackWrapper self;

public:
  /// Related types
  typedef CMemberFunctionTraits<MemberFuncPtr> traits;

  /// Facade which defines @c operator() needs to access private members
  friend class CCallbackFacade<self, typename traits::result_type,
                               typename traits::argument_type>;
  /// Reference to object
  typedef typename traits::object_reference reference;

  /// Constructor
  CCallbackWrapper(reference value, MemberFuncPtr ptr):
    object(value), function(ptr) { }

private:
  reference object;
  MemberFuncPtr function;
};

END_NAMESPACE_PBORI

#endif
