// -*- c++ -*-
//*****************************************************************************
/** @file traits.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-24
 *
 * This file defines properties of built-in an polybori types.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_common_common_traits_h_
#define polybori_common_common_traits_h_

#include <set>
#include <vector>

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/common/tags.h>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/stringize.hpp>

BEGIN_NAMESPACE_PBORI

typedef PBORI_PREFIX(DdManager) DdManager;

/** @class pbori_traits 
 * @brief This class contains property definitions to be used in polybori 
 * classes
 *
 **/
template <class ValueType>
class pbori_traits {

public:
  //-------------------------------------------------------------------------
  // types for treatment of decision diagrams
  //-------------------------------------------------------------------------
  typedef ValueType value_type;

  //-------------------------------------------------------------------------
  // types for several purposes
  //-------------------------------------------------------------------------

  /// Type for standard true/false statements
  typedef typename value_type::bool_type bool_type;

  /// Type for lengths, dimensions, etc.
  typedef typename value_type::size_type size_type;

  /// Type for integer numbers
  typedef typename value_type::integer_type integer_type;

  /// Type for indices
   typedef typename value_type::idx_type idx_type;

  /// Type for comparisons
  typedef typename value_type::comp_type comp_type;

  /// Type for hashing
  typedef typename value_type::hash_type hash_type;

  /// Type for out-stream
  typedef typename value_type::ostream_type ostream_type;

  /// The property whether the equality check is easy
  typedef typename value_type::easy_equality_property easy_equality_property;
};

// Built-in types inherit global definitions
template <>
class pbori_traits<void>:
  public CTypes, public equality_property<valid_tag> {
};

template <>
class pbori_traits<int>:
  public CTypes, public equality_property<valid_tag> {
};

template <>
class pbori_traits<unsigned int>:
  public CTypes, public equality_property<valid_tag> {
};

template <>
class pbori_traits<long int>:
  public CTypes, public equality_property<valid_tag> {
};


template <class ValueType>
class pbori_traits< std::set<ValueType> >:
  public CTypes, public equality_property<invalid_tag> {
};

template <class ValueType>
class pbori_traits< std::vector<ValueType> >:
  public CTypes, public equality_property<invalid_tag> {
};


/** @class pbori_binary_traits 
 * @brief This class contains property definitions related to two types
 *  to be used in polybori classes.
 *
 **/
template <class FirstType, class SecondType>
class pbori_binary_traits;

template <class OnlyType>
class pbori_binary_traits<OnlyType, OnlyType> {
public:
  typedef typename OnlyType::easy_equality_property easy_equality_property;
};

template <class FirstType, class SecondType>
class pbori_binary_traits:
  public equality_property<invalid_tag>{
};




template <class MgrType>
struct manager_traits;

template <class CuddLike>
struct manager_traits {

  typedef unsigned long large_size_type;
  typedef long int refcount_type;

  typedef CTypes::idx_type idx_type;
  typedef CTypes::size_type size_type;

  typedef DdNode* node_type;
  typedef DdManager* mgrcore_type;

  typedef node_type (*unary_int_function)(mgrcore_type, int);
  typedef node_type (*void_function)(mgrcore_type);

  typedef DD_CTFP binary_function;
  typedef node_type (*binary_int_function)(mgrcore_type, node_type, int);
  typedef 
  node_type (*ternary_function)(mgrcore_type, node_type, node_type, node_type);

  typedef int (*int_unary_function)(mgrcore_type, node_type);
};


#define PB_DECLARE_CUDD_TYPES(fromspace) \
  typedef fromspace::errorfunc_type errorfunc_type;           \
  typedef fromspace::large_size_type large_size_type;         \
  typedef fromspace::refcount_type refcount_type;             \
  typedef fromspace::node_type node_type;                     \
  typedef fromspace::mgrcore_type mgrcore_type;               \
  typedef fromspace::unary_int_function unary_int_function;   \
  typedef fromspace::void_function void_function;             \
  typedef fromspace::binary_function binary_function;         \
  typedef fromspace::binary_int_function binary_int_function; \
  typedef fromspace::ternary_function ternary_function;       \
  typedef fromspace::int_unary_function int_unary_function;   \
  typedef fromspace::size_type size_type;\
  typedef fromspace::idx_type idx_type;


#define PB_BINARY_FUNC_CALL(count, funcname, arg_pair)                        \
  BOOST_PP_EXPAND(funcname(BOOST_PP_SEQ_HEAD(arg_pair),                       \
                           BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(arg_pair))))

template<unsigned ErrorNumber>
struct cudd_error_traits {
  typedef const char* result_type;

  result_type operator()() const;
};




#define PB_CUDD_ERROR_TRAITS(errcode, errstr)                                \
  template<> inline cudd_error_traits<errcode>::result_type                  \
  cudd_error_traits<errcode>::operator()() const {                           \
    return BOOST_PP_STRINGIZE(errstr); }

BOOST_PP_SEQ_FOR_EACH( PB_BINARY_FUNC_CALL, PB_CUDD_ERROR_TRAITS, 
                       ((CUDD_MEMORY_OUT)(Out of memory.))
                       ((CUDD_TOO_MANY_NODES)(Too many nodes.))
                       ((CUDD_MAX_MEM_EXCEEDED)(Maximum memory exceeded.))
                       ((CUDD_INVALID_ARG)(Invalid argument.))
                       ((CUDD_INTERNAL_ERROR)(Internal error.))
                       ((CUDD_NO_ERROR)(Unexpected error.))
                       )

#undef PB_CUDD_ERROR_TRAITS

END_NAMESPACE_PBORI

#endif
