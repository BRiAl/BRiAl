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
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************


// include basic definitions
#include "polybori/pbori_defs.h"
#include <polybori/routines/pbori_func.h>

#include "polybori/BoolePolynomial.h"
#include "CDelayedTermIter.h"

#include <algorithm>

#ifndef CRestrictedIter_h_
#define CRestrictedIter_h_

BEGIN_NAMESPACE_PBORI


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
