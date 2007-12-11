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
 *   (c) 2007 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.17  2007/12/11 13:18:28  bricken
 * + > -> <
 *
 * Revision 1.16  2007/11/30 12:55:57  dreyer
 * ADD: count_index
 *
 * Revision 1.15  2007/11/06 15:03:36  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.14  2007/10/11 09:30:47  dreyer
 * Fix: Workaround function template pointer problem
 *
 * Revision 1.13  2007/10/09 15:07:27  dreyer
 * ADD: mapping;   CHANGE: shared python modules to pyroot
 *
 * Revision 1.12  2007/10/09 12:16:49  dreyer
 * ADD: apply_mapping
 *
 * Revision 1.11  2007/10/09 10:30:52  dreyer
 * ADD: poly.gradedPart(deg); FIX: term_accumulate (constant term)
 *
 * Revision 1.10  2007/07/27 15:15:21  dreyer
 * CHANGE: using alternative for term_accumulate (uses add-cache)
 *
 * Revision 1.9  2007/05/20 16:03:24  dreyer
 * CHANGE: more fine tuning
 *
 * Revision 1.8  2007/05/20 15:22:55  dreyer
 * CHANGE fine tuning
 *
 * Revision 1.7  2007/05/20 09:44:40  dreyer
 * ADD: BooleSet(true/false)
 *
 * Revision 1.6  2007/05/19 08:43:03  dreyer
 * CHANGE: remove unnecessary casts
 *
 * Revision 1.5  2007/05/18 16:10:27  dreyer
 * CHANGE: term_accumulate optimized more
 *
 * Revision 1.4  2007/05/18 14:28:06  dreyer
 * CHANGE: some optimizations
 *
 * Revision 1.3  2007/05/18 11:48:39  dreyer
 * ADD: sophisticated term_accumulate
 *
 * Revision 1.2  2007/05/14 16:09:40  dreyer
 * Fix: removed infinite recursion in specialized case
 *
 * Revision 1.1  2007/05/11 11:38:42  dreyer
 * ADD: started pbori_algorithms.h and term_accumulation()
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef pbori_algorithms_h_
#define pbori_algorithms_h_

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

template <class NaviType, class LowerIterator, class ValueType>
ValueType 
lower_term_accumulate(NaviType navi, 
                      LowerIterator lstart, LowerIterator lfinish, 
                      ValueType init) {

  /// @todo Maybe recursive caching is efficient here.
  if (lstart == lfinish){
    return false;
  }
  
  if (navi.isConstant())
    return BooleSet(navi);
  
  assert(*lstart >= *navi);

  ValueType result;
  if (*lstart > *navi) {

    ValueType reselse = 
      lower_term_accumulate(navi.elseBranch(), lstart, lfinish, init);

//     if(reselse.isZero())
//       return BooleSet(navi.thenBranch()).change(*navi);

    // Note: result == BooleSet(navi) holds only in trivial cases, so testing
    // reselse.navigation() == navi.elseBranch() is almost always false
    // Hence, checking reselse.navigation() == navi.elseBranch() for returning
    // navi, instead of result yields too much overhead.
    result = BooleSet(*navi, navi.thenBranch(), reselse.navigation());
  }
  else  {
    assert(*lstart == *navi);
    ++lstart;
    BooleSet resthen = 
      lower_term_accumulate(navi.thenBranch(), lstart, lfinish, init).diagram();

    result = resthen.change(*navi);
  }

  return  result;
}


template <class UpperIterator, class NaviType, class ValueType>
ValueType 
upper_term_accumulate(UpperIterator ustart, UpperIterator ufinish,
                      NaviType navi, ValueType init) {

  // Note: Recursive caching, wrt. a navigator representing the term
  // corresponding to ustart .. ufinish cannot be efficient here, because
  // dereferencing the term is as expensive as this procedure in whole. (Maybe
  // the generation of the BooleSet in the final line could be cached somehow.)

  // assuming (ustart .. ufinish) never means zero
  if (ustart == ufinish)
     return true;
  
  while (*navi < *ustart)
    navi.incrementElse();
  ++ustart;
  NaviType navithen = navi.thenBranch();
  ValueType resthen = upper_term_accumulate(ustart, ufinish, navithen, init);

  // The following condition holds quite often, so computation time may be saved
  if (navithen == resthen.navigation())
    return BooleSet(navi);

  return BooleSet(*navi, resthen.navigation(), navi.elseBranch());
}

///@note: assuming lstart .. lfinish *not* marking the term one
template <class UpperIterator, class NaviType, class LowerIterator, 
          class ValueType>
ValueType 
term_accumulate(UpperIterator ustart, UpperIterator ufinish, NaviType navi, 
                LowerIterator lstart, LowerIterator lfinish, ValueType init) {


  if (lstart == lfinish)
    return upper_term_accumulate(ustart, ufinish, navi, init);

  if (ustart == ufinish)
    return true;

  while (*navi < *ustart)
    navi.incrementElse();
  ++ustart;

  
  if (navi.isConstant())
    return BooleSet(navi);

  assert(*lstart >= *navi);

  ValueType result;
  if (*lstart > *navi) {
    ValueType resthen = 
      upper_term_accumulate(ustart, ufinish, navi.thenBranch(), init);
    ValueType reselse = 
      lower_term_accumulate(navi.elseBranch(), lstart, lfinish, init);

    result = BooleSet(*navi, resthen.navigation(), reselse.navigation());
  }
  else  {
    assert(*lstart == *navi);
    ++lstart;
     BooleSet resthen = term_accumulate(ustart, ufinish,  navi.thenBranch(),
                                        lstart, lfinish, init).diagram();
 
    result = resthen.change(*navi);
  }

  return result;
}




/// @func term_accumulate
/// Routine for adding all terms given by iterators
template <class InputIterator, class ValueType>
ValueType 
term_accumulate(InputIterator first, InputIterator last, ValueType init) {

#ifdef PBORI_ALT_TERM_ACCUMULATE
  if(last.isOne())
    return upper_term_accumulate(first.begin(), first.end(), 
                                 first.navigation(), init) + ValueType(1);
  
  ValueType result = term_accumulate(first.begin(), first.end(), 
                                     first.navigation(),
                                     last.begin(), last.end(), init);

  
  // alternative
  /*  ValueType result = upper_term_accumulate(first.begin(), first.end(), 
                                           first.navigation(), init);


  result = lower_term_accumulate(result.navigation(),
                                 last.begin(), last.end(), init);

  */

  assert(result == std::accumulate(first, last, init) ); 

  return result;

#else


  if(first.isZero())
    return first.navigation();

  ValueType result = upper_term_accumulate(first.begin(), first.end(), 
                                           first.navigation(), init);
  if(!last.isZero())
    result += upper_term_accumulate(last.begin(), last.end(), 
                                    last.navigation(), init);

  assert(result == std::accumulate(first, last, init) ); 

  return result;
#endif
}


// determine the part of a polynomials of a given degree
template <class CacheType, class NaviType, class SetType>
SetType
dd_mapping(const CacheType& cache, NaviType navi, NaviType map, SetType init) {

  if (navi.isConstant())
    return navi;

  while (*map < *navi) {
    assert(!map.isConstant());
    map.incrementThen();
  }

  assert(*navi == *map);

  NaviType cached = cache.find(navi, map);

  // look whether computation was done before
  if (cached.isValid())
    return SetType(cached);

  SetType result = 
    SetType(*(map.elseBranch()),  
            dd_mapping(cache, navi.thenBranch(), map.thenBranch(), init),
            dd_mapping(cache, navi.elseBranch(), map.thenBranch(), init)
            );


  // store result for later reuse
  cache.insert(navi, map, result.navigation());

  return result;
}


template <class PolyType, class MapType>
PolyType
apply_mapping(const PolyType& poly, const MapType& map) {

  CCacheManagement<typename CCacheTypes::mapping> 
    cache(poly.diagram().manager());

  return dd_mapping(cache, poly.navigation(), map.navigation(), 
                    typename PolyType::set_type()); 
}


template <class MonomType, class PolyType>
PolyType
generate_mapping(MonomType& fromVars, MonomType& toVars, PolyType init) {

  if(fromVars.isConstant()) {
    assert(fromVars.isOne() && toVars.isOne());
    return fromVars;
  }

  MonomType varFrom = fromVars.firstVariable();
  MonomType varTo = toVars.firstVariable();
  fromVars.popFirst();
  toVars.popFirst();
  return (varFrom * generate_mapping(fromVars, toVars, init)) + varTo;
}

template <class PolyType, class MonomType>
PolyType
mapping(PolyType poly, MonomType fromVars, MonomType toVars) {

  return apply_mapping(poly, generate_mapping(fromVars, toVars, PolyType()) );
}


template <class SetType>
void combine_sizes(const SetType& bset, double& init) {
  init += bset.sizeDouble();
}

template <class SetType>
void combine_sizes(const SetType& bset, 
                   typename SetType::size_type& init) {
  init += bset.size();
}


template <class SizeType, class IdxType, class NaviType>
SizeType&
count_index(SizeType& size, IdxType idx, NaviType navi) {

  if (*navi == idx)
    combine_sizes(BooleSet(navi.incrementThen()), size);

  if (*navi < idx) {
    count_index(size, idx, navi.thenBranch());
    count_index(size, idx, navi.elseBranch());
  }
  return size;
}

END_NAMESPACE_PBORI

#endif // pbori_algorithms_h_
