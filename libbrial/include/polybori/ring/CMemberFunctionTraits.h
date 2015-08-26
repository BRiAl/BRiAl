// -*- c++ -*-
//*****************************************************************************
/** @file CMemberFunctionTraits.h 
 *
 * @author Alexander Dreyer
 * @date 2010-08-30
 *
 * This file define a template, which extracts related types from a member
 * function pointer.
 *
 * We use the notation of boost::function_traits as much as possible.
 *
 * @par Copyright:
 *   (c) 2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_ring_CMemberFunctionTraits_h_
#define polybori_ring_CMemberFunctionTraits_h_

// include basic definitions
#include <polybori/pbori_defs.h>

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


template <class MemberFct>
class MemberFunctionOperator {
  typedef CMemberFunctionTraits<MemberFct> traits;

public:
  MemberFunctionOperator(MemberFct func): m_func(func) {}

  typename traits::result_type operator()(typename traits::object_reference obj,
                                          typename traits::argument_type arg) {
    return (obj .* m_func)(arg);
  }

private:
  MemberFct m_func;
};


template <class MemberFct>
MemberFunctionOperator<MemberFct>
member_function_operator(MemberFct func) {
  return MemberFunctionOperator<MemberFct>(func);
}

END_NAMESPACE_PBORI

#endif
