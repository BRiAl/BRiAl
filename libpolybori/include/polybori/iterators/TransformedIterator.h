// -*- c++ -*-
//*****************************************************************************
/** @file TransformedIterator.h 
 *
 * @author Alexander Dreyer
 * @date 2012-01-19
 *
 * This file includes the definition of the class @c TransformedSequence.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_iterators_TransformedIterator_h_
#define polybori_iterators_TransformedIterator_h_

// include basic definitions
#include <polybori/pbori_defs.h>

#include <boost/iterator/iterator_facade.hpp>
#include <iterator>

BEGIN_NAMESPACE_PBORI

template <class UnaryFunc>
class UnaryOperatorTraits {
public: 
  typedef typename UnaryFunc::result_type result_type;
  typedef typename UnaryFunc::argument_type argument_type;
};

template <class ResultType, class ArgType>
class UnaryOperatorTraits<ResultType (*)(ArgType)> {
 public:
  typedef ResultType result_type;
  typedef ArgType argument_type;
};

/** @class TransformedIterator
 * @brief This class defines the iterator of Trnasformed sequence.
 *
 * It store changes a given iterator by transforming the dereference value
 * using a unary functional.
 **/
template <class IteratorType, class OperatorType>
class TransformedIterator:
  public boost::iterator_facade<
  TransformedIterator<IteratorType, OperatorType>,
  typename UnaryOperatorTraits<OperatorType>::result_type,
  std::forward_iterator_tag,
  typename UnaryOperatorTraits<OperatorType>::result_type > {

  typedef IteratorType iterator;
  typedef TransformedIterator self;

public:
  typedef OperatorType operator_type;
  typedef typename operator_type::result_type result_type;

  /// Construct from given iterator and unary functional
  TransformedIterator(iterator iter, const operator_type& func):
    m_iter(iter), m_func(func) { }


  /// Increment operation
  void increment() { ++m_iter; }

  /// Constant dereference operator
  result_type dereference() const { return m_func(*m_iter); }

  /// Equality check
  bool equal(const self& rhs) const {  return m_iter == rhs.m_iter; }

private:
  iterator m_iter;
  const operator_type& m_func;
};

END_NAMESPACE_PBORI

#endif /* polybori_TransformedIterator_h_ */
