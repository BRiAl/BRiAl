// -*- c++ -*-
//*****************************************************************************
/** @file pbori_algorithm.h
 *
 * @author Alexander Dreyer
 * @date 2007-05-11
 *
 * This file contains algorithmic procedures, which can be used together with
 * PolyBoRi classes. 
 * @note This file depends on several PolyBoRi classes, so it should
 * not be included by PolyBoRi headers (excl. polybori.h), because this would
 * lead to curious dependencies. Several templates, which do not depend on
 * PolyBoRi classes can be found in pbori_algo.h.
 *
 * @par Copyright:
 *   (c) 2007 by
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
 * Revision 1.2  2007/05/14 16:09:40  dreyer
 * Fix: removed infinite recursion in specialized case
 *
 * Revision 1.1  2007/05/11 11:38:42  dreyer
 * ADD: started pbori_algorithms.h and term_accumulation()
 *
 * @endverbatim
**/
//*****************************************************************************

// include standard headers
#include <numeric>

// include basic definitions
#include "pbori_defs.h"

// include PolyBoRi algorithm, which do not depend on PolyBoRi classes
#include "pbori_algo.h"

// include PolyBoRi class definitions, which are used here
#include "BoolePolynomial.h"
#include "BooleMonomial.h"
#include "CGenericIter.h"


BEGIN_NAMESPACE_PBORI

/// @func spoly
/// Compute spoly of two polynomials
inline BoolePolynomial 
spoly(const BoolePolynomial& first, const BoolePolynomial& second){

   BooleMonomial lead1(first.lead()), lead2(second.lead());

   BooleMonomial prod = lead1;
   prod *= lead2;

   return ( first * (prod / lead1) ) + ( second * (prod / lead2) );
}

/// @func term_accumulate
/// Routine for adding all terms given by iterators
template <class InputIterator, class ValueType>
ValueType 
term_accumulate(InputIterator first, InputIterator last, ValueType init) {
  return std::accumulate(first, last, init);
}


// Specialized routine for adding all terms given by lexicographic iterators
template <class ValueType>
ValueType 
term_accumulate(BoolePolynomial::const_iterator first, 
                BoolePolynomial::const_iterator last, ValueType init) {
  return std::accumulate(first, last, init);
}



END_NAMESPACE_PBORI
