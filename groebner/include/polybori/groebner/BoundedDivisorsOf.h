// -*- c++ -*-
//*****************************************************************************
/** @file BoundedDivisorsOf.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-28
 *
 * This file includes the definition of the class @c BoundedDivisorsOf.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_BoundedDivisorsOf_h_
#define polybori_groebner_BoundedDivisorsOf_h_

#include "fixed_path_divisors.h"

// include basic definitions
#include "groebner_defs.h"

#include <functional>

BEGIN_NAMESPACE_PBORIGB

/** @class BoundedDivisorsOf
 * @brief This class defines defines a function for computing a bounded 
 * variant of @c MononmialSet's @c divisorsOf
 *
   This implements the mapping
   f: terms -> subsets of T (a set of terms), with
      t  :> {y in T :  t divides y, y divides t*t0}

   where t0 and T are a term and a set of terms, respectively,
   given as constructor arguments.
  
 * @note The operator stored references of t0 and T only.
 **/

class BoundedDivisorsOf: 
  public std::unary_function<Monomial, MonomialSet> {

public:
  /// Initialize mapping with given parameters
  BoundedDivisorsOf(const Monomial& term, const MonomialSet& terms):
    m_factor(term), m_terms(terms) {}

  /// The actual call
  MonomialSet operator()(const Monomial& t_divided) const {
    Monomial t = t_divided * m_factor;
    return fixed_path_divisors(m_terms, t, t_divided);
  }

private:
  Monomial m_factor;
  MonomialSet m_terms;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_BoundedDivisorsOf_h_ */
