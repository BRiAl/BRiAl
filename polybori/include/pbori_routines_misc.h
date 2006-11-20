// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_misc.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which defines miscellaneous function templates.
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
 * Revision 1.7  2006/11/20 14:56:46  dreyer
 * CHANGE CCacheType names, operator*=, CDDInterface node Constructor
 *
 * Revision 1.6  2006/10/24 14:21:56  dreyer
 * ADD: variable_name functional
 *
 * Revision 1.5  2006/10/23 16:05:55  dreyer
 * ADD: BoolePolyRing::set/get(Ring)VariableName()
 *
 * Revision 1.4  2006/10/04 15:46:49  dreyer
 * ADD: divisorsOf(exp_type), orderedExpBegin/End;
 * CHANGE: Polynomials printing respects ordering
 *
 * Revision 1.3  2006/09/21 16:09:59  dreyer
 * ADD: caching mechanism for BoolePolynomial::deg()
 *
 * Revision 1.2  2006/09/14 10:57:26  dreyer
 * ADD: usedVariablesExp()
 *
 * Revision 1.1  2006/08/24 14:52:15  dreyer
 * + Initial Version
 *
 * Revision 1.1  2006/08/23 14:24:54  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// temprarily
#include "CIdxVariable.h"
// temprarily
#include "BoolePolyRing.h"

#include "CCacheManagement.h"

BEGIN_NAMESPACE_PBORI

template<class Iterator>
typename Iterator::value_type
index_vector_hash(Iterator start, Iterator finish){

  typedef typename Iterator::value_type value_type;

  value_type vars = 0;
  value_type sum = 0;
 
  while (start != finish){
    vars++;
    sum += ((*start)+1) * ((*start)+1);
    ++start;
  }
  return sum * vars;
}

/// Function templates for determining the degree of a decision diagram
/// with the help of cache (e. g. CDegreeCache)
template <class DegreeCacher, class NaviType>
typename NaviType::size_type
dd_cached_degree(DegreeCacher cache, NaviType navi) {

  typedef typename NaviType::size_type size_type;

  if (navi.isConstant()) // No need for caching of constant nodes' degrees
    return 0;
 
  // Look whether result was cached before
  NaviType result = cache.find(navi);
  if (result.isValid())
    return cache.convert(result);

  // Get degree of then branch (contains at least one valid path)...
  size_type deg = dd_cached_degree(cache, navi.thenBranch()) + 1;
 
  // ... combine with degree of else branch
  deg = std::max(deg,  dd_cached_degree(cache, navi.elseBranch()) );

  // Write result to cache
  cache.insert(navi, deg);
 
  return deg;
}



template <class Iterator, class NameGenerator, 
          class Separator, class EmptySetType, 
          class OStreamType>
void 
dd_print_term(Iterator start, Iterator finish, const NameGenerator& get_name,
              const Separator& sep, const EmptySetType& emptyset, 
              OStreamType& os){

  if (start != finish){
    os << get_name(*start);
    ++start;
  }
  else
    os << emptyset();

  while (start != finish){
    os << sep() << get_name(*start);
    ++start;
  }
}

template <class TermType, class NameGenerator,
          class Separator, class EmptySetType,
          class OStreamType>
void 
dd_print_term(const TermType& term, const NameGenerator& get_name,
              const Separator& sep, const EmptySetType& emptyset, 
              OStreamType& os){
  dd_print_term(term.begin(), term.end(), get_name, sep, emptyset, os);
}


template <class Iterator, class NameGenerator,
          class Separator, class InnerSeparator, 
          class EmptySetType, class OStreamType>
void 
dd_print_terms(Iterator start, Iterator finish, const NameGenerator& get_name,
               const Separator& sep, const InnerSeparator& innersep,
               const EmptySetType& emptyset, OStreamType& os) {

  if (start != finish){
    dd_print_term(*start, get_name, innersep, emptyset, os);
    ++start;
  }

  while (start != finish){
    os << sep(); 
    dd_print_term(*start, get_name, innersep, emptyset, os);
    ++start;
  }

}


template <class PolynomialType>
PolynomialType 
dd_multiply_recursively(PolynomialType a, PolynomialType b){

  if (a.isZero() || b.isZero()) return 0;
  if (a.isOne()) return b;
  if (b.isOne()) return a;
  if (a == b) return a;

  typedef CCommutativeCacheManagement<CCacheTypes::multiply_recursive>
    cache_mgr_type;

  cache_mgr_type cache_mgr;

  typename PolynomialType::navigator cached =
    cache_mgr.find(a.navigation(), b.navigation());

  PolynomialType result;

  if (cached.isValid() ){
    result = (CDDInterface<ZDD>)
      CTypes::dd_base(&PBORI::BoolePolyRing::activeManager().manager(),
                               cached);
  }
  else {
    int indexa=*(a.navigation());
    int indexb=*(b.navigation());
    int index=std::min(indexa,indexb);
    PolynomialType as0=a.diagram().subset0(index);
    PolynomialType as1=a.diagram().subset1(index);
    PolynomialType bs0=b.diagram().subset0(index);
    PolynomialType bs1=b.diagram().subset1(index);

    if (as0 == as1){
      PolynomialType zero(0);
      bs1=zero.diagram();
    } else {
      if (bs0 == bs1){
        PolynomialType zero(0);
        as1 = zero.diagram();
      }
    }

    result = (PolynomialType)( (dd_multiply_recursively(as0, bs1) 
                            + dd_multiply_recursively(as1, bs1)
                            + dd_multiply_recursively(as1, bs0)).diagram().change(index) )
      + dd_multiply_recursively(as0, bs0);
 
    cache_mgr.insert(a.navigation(), b.navigation(), result.navigation());
  }


  return result;
}

END_NAMESPACE_PBORI
