// -*- c++ -*-
//*****************************************************************************
/** @file pbori_algorithms.h
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
**/
//*****************************************************************************

#ifndef polybori_routines_pbori_algorithms_h_
#define polybori_routines_pbori_algorithms_h_

// include standard headers
#include <numeric>

// include basic definitions
#include <polybori/pbori_defs.h>

// include PolyBoRi algorithm, which do not depend on PolyBoRi classes
#include "pbori_algo.h"

// include PolyBoRi class definitions, which are used here
#include <polybori/BoolePolynomial.h>
#include <polybori/BooleMonomial.h>
#include <polybori/iterators/CGenericIter.h>


BEGIN_NAMESPACE_PBORI

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
  PBORI_ASSERT(init.isZero());
  /// @todo Maybe recursive caching is efficient here.
  if (lstart == lfinish){
    return init;
  }
  
  if (navi.isConstant())
    return (navi.terminalValue()? (ValueType)init.ring().one(): init);
  
  PBORI_ASSERT(*lstart >= *navi);

  ValueType result(init.ring());
  if (*lstart > *navi) {

    ValueType reselse = 
      lower_term_accumulate(navi.elseBranch(), lstart, lfinish, init);

//     if(reselse.isZero())
//       return BooleSet(navi.thenBranch()).change(*navi);

    // Note: result == BooleSet(navi) holds only in trivial cases, so testing
    // reselse.navigation() == navi.elseBranch() is almost always false
    // Hence, checking reselse.navigation() == navi.elseBranch() for returning
    // navi, instead of result yields too much overhead.
    result = BooleSet(*navi, navi.thenBranch(), reselse.navigation(), 
                      init.ring());
  }
  else  {
    PBORI_ASSERT(*lstart == *navi);
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
    return init.ring().one();
  
  while (*navi < *ustart)
    navi.incrementElse();
  ++ustart;
  NaviType navithen = navi.thenBranch();
  ValueType resthen = upper_term_accumulate(ustart, ufinish, navithen, init);

  // The following condition holds quite often, so computation time may be saved
  if (navithen == resthen.navigation())
    return BooleSet(navi, init.ring());

  return BooleSet(*navi, resthen.navigation(), navi.elseBranch(), init.ring());
}

// assuming lstart .. lfinish *not* marking the term one
template <class UpperIterator, class NaviType, class LowerIterator, 
          class ValueType>
ValueType 
term_accumulate(UpperIterator ustart, UpperIterator ufinish, NaviType navi, 
                LowerIterator lstart, LowerIterator lfinish, ValueType init) {


  if (lstart == lfinish)
    return upper_term_accumulate(ustart, ufinish, navi, init);

  if (ustart == ufinish)
    return init.ring().one();

  while (*navi < *ustart)
    navi.incrementElse();
  ++ustart;

  
  if (navi.isConstant())
    return BooleSet(navi, init.ring());

  PBORI_ASSERT(*lstart >= *navi);

  ValueType result(init.ring());
  if (*lstart > *navi) {
    ValueType resthen = 
      upper_term_accumulate(ustart, ufinish, navi.thenBranch(), init);
    ValueType reselse = 
      lower_term_accumulate(navi.elseBranch(), lstart, lfinish, init);

    result = BooleSet(*navi, resthen.navigation(), reselse.navigation(),
                      init.ring());
  }
  else  {
    PBORI_ASSERT(*lstart == *navi);
    ++lstart;
     BooleSet resthen = term_accumulate(ustart, ufinish,  navi.thenBranch(),
                                        lstart, lfinish, init).diagram();
 
    result = resthen.change(*navi);
  }

  return result;
}




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

  // test for non-lex more complicated (see testsuite)
  PBORI_ASSERT((init.ring().ordering().isLexicographical()?
          result == std::accumulate(first, last, init):
          true) ); 


  return result;

#else

  /// @note: This line always uses the active manager!
  /// @todo: check correct manager
  if(first.isZero())
    return typename ValueType::dd_type(init.ring(),
                                       first.navigation());

  ValueType result = upper_term_accumulate(first.begin(), first.end(), 
                                           first.navigation(), init);
  if(!last.isZero())
    result += upper_term_accumulate(last.begin(), last.end(), 
                                    last.navigation(), init);

  // test for non-lex more complicated (see testsuite)
  PBORI_ASSERT((init.ring().ordering().isLexicographical()?
          result == std::accumulate(first, last, init):
          true) ); 
  
  return result;
#endif
}


// determine the part of a polynomials of a given degree
template <class CacheType, class NaviType, class SetType>
SetType
dd_mapping(const CacheType& cache, NaviType navi, NaviType map, SetType init) {

  if (navi.isConstant())
    return cache.generate(navi);

  while (*map < *navi) {
    PBORI_ASSERT(!map.isConstant());
    map.incrementThen();
  }

  PBORI_ASSERT(*navi == *map);

  NaviType cached = cache.find(navi, map);

  // look whether computation was done before
  if (cached.isValid())
    return SetType(cached, cache.ring());

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

  CCacheManagement<typename PolyType::ring_type, typename CCacheTypes::mapping> 
    cache(poly.ring());

  return dd_mapping(cache, poly.navigation(), map.navigation(), 
                    typename PolyType::set_type(poly.ring())); 
}


template <class MonomType, class PolyType>
PolyType
generate_mapping(MonomType& fromVars, MonomType& toVars, PolyType init) {

  if(fromVars.isConstant()) {
    PBORI_ASSERT(fromVars.isOne() && toVars.isOne());
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

  return apply_mapping(poly, generate_mapping(fromVars, toVars, PolyType(poly.ring())) );
}



END_NAMESPACE_PBORI

#endif // pbori_algorithms_h_
