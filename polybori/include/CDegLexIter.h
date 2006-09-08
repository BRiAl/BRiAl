// -*- c++ -*-
//*****************************************************************************
/** @file CDegLexIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-06
 *
 * This file defines a degree lexicographic iterator.
 *
 * @par Copyright:
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.2  2006/09/08 10:22:59  dreyer
 * FIX: Gcc 4 ist more pedantic
 *
 * Revision 1.1  2006/09/07 16:04:32  dreyer
 * ADD: CDegLexIter.h
 *
 * @endverbatim
**/
//*****************************************************************************


// include basic definitions
#include "pbori_defs.h"


#include "BoolePolynomial.h"
#include "CDelayedTermIter.h"

#include <algorithm>

#ifndef CDegLexIter_h_
#define CDegLexIter_h_

BEGIN_NAMESPACE_PBORI

template <class FuncType, template<class> class BoundFunc = std::less>
class bounded_func:
  public FuncType {

public:
  typedef FuncType base;
  typedef typename base::second_argument_type value_type;
  typedef BoundFunc<value_type> boundop_type;
  typedef bounded_func<base, BoundFunc> self;


  bounded_func(const value_type& bound):
    m_bound(bound), bound_op() {}

 typename  base::result_type 
 operator()(const typename base::first_argument_type& first,
            const typename base::second_argument_type& second) const {
              std::cout <<"test: "<<first <<" "<< second << " "<<m_bound <<std::endl;
    return (!bound_op(first, m_bound)) || base::operator()(first, second);
  }


private:
  value_type m_bound;
  boundop_type bound_op;
};

class dummy_less {
public:


  dummy_less (unsigned bound): m_bound(bound){}

  template<class Type>
  bool operator ()(Type first, Type second){
    std::cout <<first <<":" <<second <<"?"<<m_bound<<std::endl;

    return !(first < m_bound) || 
      ( (second < m_bound) && (first < second));
  }

  unsigned m_bound;
};


template <class ValueType>
class default_value {
public:
  typedef ValueType value_type;

  value_type operator()(...) const{
    return value_type();
  }

};

template <template<class> class BindType, class BinaryFunction, 
          class ValueType, class ConstantOp>
class constant_binder_base :
  public BindType<BinaryFunction>{
public:
  typedef BinaryFunction bin_op; 
  typedef ConstantOp const_type;
  typedef BindType<bin_op> base;

  typedef ValueType value_type;

  constant_binder_base(const bin_op& op = bin_op()): base(op, const_type()()) {}
};

template <class BinaryFunction, class ConstantOp>
class constant_binder2nd :
  public constant_binder_base<std::binder2nd, BinaryFunction,
                              typename BinaryFunction::second_argument_type,
                              ConstantOp> {
};


template <class BinaryFunction, class ConstantOp>
class constant_binder1st :
  public constant_binder_base<std::binder1st, BinaryFunction,
                              typename BinaryFunction::first_argument_type,
                              ConstantOp> {
};

template <template<class> class BindType,
          class BinaryFunction, class ValueType>
class default_binder_base :
  public BindType<BinaryFunction>{
public:
  typedef BinaryFunction bin_op; 
  typedef BindType<bin_op> base;

  typedef ValueType value_type;

  default_binder_base(const value_type&  val): base(bin_op(), val) {}
};

template <class BinaryFunction>
class default_binder2nd :
  public default_binder_base<std::binder2nd, BinaryFunction,
                              typename BinaryFunction::second_argument_type> {
public:
  typedef default_binder_base<std::binder2nd, BinaryFunction,
                              typename BinaryFunction::second_argument_type>
  base;

  default_binder2nd(const typename base::value_type&  val): base(val) {}
};


template <class BinaryFunction>
class default_binder1st :
  public default_binder_base<std::binder1st, BinaryFunction,
                              typename BinaryFunction::first_argument_type> {
};


template <class Iterator, 
          class RestrictOp = 
            default_binder2nd< std::less<typename Iterator::value_type> >,
          class IsValidTest = constant_binder2nd< std::not_equal_to<Iterator>, 
                                                default_value<Iterator> > >
class CRestrictedIter:
  public Iterator {
public:

  typedef Iterator base;
  typedef IsValidTest is_valid_type;
  typedef RestrictOp restrictop_type;
  typedef CRestrictedIter<base, restrictop_type, is_valid_type> self;
  typedef typename base::value_type value_type;

  CRestrictedIter(const base& src, 
                  const restrictop_type& in_range = restrictop_type(),
                  const is_valid_type& is_valid = is_valid_type() ):
    base(src), m_in_range(in_range), m_is_valid(is_valid) {
    goToValid();
  }


  self& operator++() {
    base::operator++();
    goToValid();
    return *this;
  }
  self operator++(int) {
    self result(*this);
    self::operator++();
    return result;
  }

  void goToValid() {

    while( isValid() && !inRange() ) {
      base::operator++();
    }
  }

  bool isValid() const {
    return m_is_valid(*this);
  }

  bool inRange() const {
    return m_in_range(base::operator*());
  }

private:
  restrictop_type m_in_range;
  is_valid_type m_is_valid;
};

template<class PolyType, class PolyDegIter = typename PolyType::deg_iterator>
class CDegLexIter {

public:

  /// Fix type for polynomials
  typedef PolyType poly_type;

  /// Fix type for sizes
  typedef typename poly_type::size_type size_type;

  /// Fix type for Boolean values
  typedef typename poly_type::bool_type bool_type;

  /// Fix type for monomials
  typedef typename poly_type::monom_type monom_type;

  /// Set type for terms
  typedef monom_type term_type;

  /// Fix type for polynomials
  typedef typename poly_type::deg_iterator deg_iterator;

  /// @name Interface types for standard iterator access
  //@{
  typedef term_type value_type;
  typedef std::forward_iterator_tag iterator_category;
  typedef typename deg_iterator::difference_type difference_type;
  typedef void pointer;
  typedef value_type reference;
  //@}

  /// Get type, this is inherited from
  typedef CDelayedTermIter<monom_type, 
                           change_assign<monom_type>, project_ith<2>, 
                           deg_iterator> delayed_term_iterator;

  typedef CRestrictedIter<delayed_term_iterator> bounded_iterator;

  /// Generic access to type of *this
  typedef CDegLexIter self;

  // Constructor
  CDegLexIter(const delayed_term_iterator& start, 
              const delayed_term_iterator& finish ): 
    m_iter(std::max_element(start, finish)), m_start(start), m_finish(finish) {

  }
  // Default Constructor
  CDegLexIter():  m_iter(), m_start(), m_finish() {}

  /// Constant dereference operator
  reference operator*() const {
    return m_iter.term();
  }

  /// Prefix increment operator
  self& operator++() {
    if (m_iter != m_finish) {
      size_type deg = *m_iter;
      ++m_iter;
      m_iter = std::find(m_iter, m_finish, deg);
      
      if(m_iter == m_finish) {
        m_iter = std::max_element( bounded_iterator(m_start, deg),
                                   bounded_iterator(m_finish, deg) );

      }
    }

    return *this; 
  }

  self operator++(int) {
    self result(*this);
    operator++();
    return result;
  }


  bool_type operator!=(const self& rhs) const {
    return (m_iter != rhs.m_iter);
  }

  bool_type operator==(const self& rhs) const {
    return (m_iter == rhs.m_iter);
  }

private:
  delayed_term_iterator m_iter, m_start, m_finish;
};



END_NAMESPACE_PBORI

#endif
