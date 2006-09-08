// -*- c++ -*-
//*****************************************************************************
/** @file CRestrictedIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-06
 *
 * This file defines restricted iterator
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
 * Revision 1.1  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * @endverbatim
**/
//*****************************************************************************


// include basic definitions
#include "pbori_defs.h"


#include "BoolePolynomial.h"
#include "CDelayedTermIter.h"

#include <algorithm>

#ifndef CRestrictedIter_h_
#define CRestrictedIter_h_

BEGIN_NAMESPACE_PBORI


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



END_NAMESPACE_PBORI

#endif
