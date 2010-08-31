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

// include basic definitions
#include "pbori_defs.h"

#include <utility>              // std::pair
#include <boost/type_traits/function_traits.hpp>

#ifndef PBORI_CCallback_Wrapper_h_
#define PBORI_CCallback_Wrapper_h_

BEGIN_NAMESPACE_PBORI

/** @class CMemberFunctionTraits
 * @brief This template class defines related types for member function pointer.
 * 
 * @note Currently, the class supports functions with up to one argument only.
 **/
template<class MemberFuncPtr>
class CMemberFunctionTraits;

/// Axuiliary class
template <class Type, class ResultType, class ArgType, class ObjRef>
class CMemberFunctionTraitsBase {
public:
  typedef ResultType result_type;
  typedef ArgType argument_type;
  typedef Type object_type;
  typedef ObjRef object_reference;
};


template <class Type, class ResultType>
class CMemberFunctionTraits<ResultType (Type::*)()>:
  public CMemberFunctionTraitsBase<Type, ResultType, void, Type&> { };

template <class Type, class ResultType>
class CMemberFunctionTraits<ResultType (Type::*)() const>:
  public CMemberFunctionTraitsBase<Type, ResultType, void, const Type&> { };

template <class Type, class ResultType, class ArgType>
class CMemberFunctionTraits<ResultType (Type::*)(ArgType)>:
  public CMemberFunctionTraitsBase<Type, ResultType, ArgType, Type&> { };


template <class Type, class ResultType, class ArgType>
class CMemberFunctionTraits<ResultType (Type::*)(ArgType) const>:
  public CMemberFunctionTraitsBase<Type, ResultType, ArgType, const Type&> { };


/** @class CCallbackFacade
 * @brief This template class defines a facade for applying operator @c .* in
 * @c operator() .
 * 
 * If applies a stored member function pointer (together with a stored object
 * reference) to the argument of operator().
 *
 * It is to be used as a face of @Type, e. g. for @c CCallbackWrapper below.
 *
 * @note Specialized variant to void member fucntions
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
 
  /// Apply member function pointer to argument
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

  /// Reference to object
  typedef typename traits::object_reference reference;

  friend class CCallbackFacade<self, typename traits::result_type,
                               typename traits::argument_type>;

  /// Constructor
  CCallbackWrapper(reference value, MemberFuncPtr ptr):
    object(value), function(ptr) { }

private:
  reference object;
  MemberFuncPtr function;
};
END_NAMESPACE_PBORI

#endif
