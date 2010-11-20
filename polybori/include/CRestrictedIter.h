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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.3  2007/11/06 15:03:35  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.2  2006/10/04 13:09:56  dreyer
 * ADD: added compile-time optimied iterators and genericBegin/genericEnd
 *
 * Revision 1.1  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * @endverbatim
**/
//*****************************************************************************


// include basic definitions
#include "pbori_defs.h"
#include "pbori_func.h"

#include "BoolePolynomial.h"
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
