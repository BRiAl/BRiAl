// -*- c++ -*-
//*****************************************************************************
/** @file pbori_algo.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-07
 *
 * This file includes some templates of simple transformations and similar
 * procedures. 
 *
 * @note This file carries pure template routines. Algorithms, which make
 * explicite use of PolyBoRi classes can be found in pbori_algorithms.h
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
 * Revision 1.29  2007/05/11 11:38:42  dreyer
 * ADD: started pbori_algorithms.h and term_accumulation()
 *
 * Revision 1.28  2007/05/03 16:04:46  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * Revision 1.27  2006/12/14 16:08:19  dreyer
 * FIX: forgotten typename
 *
 * Revision 1.26  2006/12/14 13:48:04  dreyer
 * FIX: Slowdown on sculptor, due to unnecessary return + copy
 *
 * Revision 1.25  2006/12/13 18:07:04  dreyer
 * ADD: poly /= exp
 *
 * Revision 1.24  2006/11/24 16:38:37  dreyer
 * CHANGE: additional fine-tuning multiplesOf
 *
 * Revision 1.23  2006/11/24 15:45:30  dreyer
 * CHANGE: fine-tuning multiplesOf
 *
 * Revision 1.22  2006/11/24 14:49:00  dreyer
 * CHANGE: divisorsOf (less recursions/cache-lookups)
 *
 * Revision 1.21  2006/11/22 15:46:22  dreyer
 * ADD: CacheManager replacing CCacheManagement for external use
 * CHANGE: CacheManager used, where necessary
 * CHANGE: multiplesOf, uses cached recursion
 *
 * Revision 1.20  2006/11/22 10:10:23  dreyer
 * ADD: dd_first_divisors_of
 *
 * Revision 1.19  2006/10/27 15:31:11  dreyer
 * ADD: some maybe useful stuff
 *
 * Revision 1.18  2006/10/06 12:52:01  dreyer
 * ADD easy_equility_property and used in lex_compare
 *
 * Revision 1.17  2006/10/05 12:51:32  dreyer
 * CHANGE: Made lex-based comparisions more generic.
 *
 * Revision 1.16  2006/09/13 15:07:04  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.15  2006/09/12 14:56:19  dreyer
 * ADD Preparations for bidirectional iterator
 *
 * Revision 1.14  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.13  2006/08/22 16:06:22  dreyer
 * + Added highlevel division
 *
 * Revision 1.12  2006/08/15 14:17:29  dreyer
 * ADD minimalElements(), hasTermOfVariables()
 *
 * Revision 1.11  2006/08/09 15:18:58  dreyer
 * FIX: Bug in dd_intersect_some_index (didn't check valid path)
 *
 * Revision 1.10  2006/08/09 12:52:31  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * Revision 1.9  2006/08/03 15:20:20  dreyer
 * ADD: BooleSet::divisorsOf and BooleSet::hasCommonVariables
 *
 * Revision 1.8  2006/07/06 16:01:29  dreyer
 * CHANGE: Functionals ins pbori_func.h made more consistent
 *
 * Revision 1.7  2006/07/04 14:11:03  dreyer
 * ADD: Generic and handy treatment of string literals
 *
 * Revision 1.6  2006/04/21 13:13:30  dreyer
 * ADD PBoRiOutITer for more generic manipulations
 *
 * Revision 1.5  2006/04/14 15:45:30  bricken
 * + removed semicolons
 *
 * Revision 1.4  2006/04/13 07:53:19  dreyer
 * CHANGE BoolePolynomial::print() and deg() produces more useful results
 *
 * Revision 1.3  2006/04/12 16:23:54  dreyer
 * ADD template class CIDXPath<>
 *
 * Revision 1.2  2006/04/10 07:36:27  dreyer
 * ADD pbori_func.h to carry PolyBoRi related functionals
 *
 * Revision 1.1  2006/04/07 16:32:08  dreyer
 * ADD dd_transform and resp. examples
 *
 * @endverbatim
**/
//*****************************************************************************

// include polybori's definitions
#include "pbori_defs.h"

// get polybori's functionals
#include "pbori_func.h"

// temporarily
#include "cudd.h"

#ifndef pbori_algo_h_
#define pbori_algo_h_

BEGIN_NAMESPACE_PBORI


//       dd_backward_transform(p.navigation(), Polynomial(0), 
//                             mapped_new_node<std::vector<idx_type>, Variable,
//                             Polynomial>(table), 
//                             integral_constant<bool, true, Polynomial>());

/// Function templates for transforming decision diagrams 
template< class NaviType, class TermType, 
          class TernaryOperator, 
          class TerminalOperator >
TermType
dd_backward_transform( NaviType navi, TermType init, TernaryOperator newNode,
              TerminalOperator terminate ) {
 
  TermType result = init;

  if (navi.isConstant()) {      // Reached end of path
    if (navi.terminalValue()){   // Case of a valid path
      result = terminate();
    }
  }
  else {
    result = dd_backward_transform(navi.thenBranch(), init, newNode, terminate);
    result = newNode(*navi, result, 
                     dd_backward_transform(navi.elseBranch(), init, newNode,
                                           terminate) );
  }
  return result;
}


/// Function templates for transforming decision diagrams 
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator, class ElseBinaryOperator, 
          class TerminalOperator >
OutIterator
dd_transform( NaviType navi, TermType init, OutIterator result, 
              ThenBinaryOperator then_binop, ElseBinaryOperator else_binop,
              TerminalOperator terminate ) {
 

  if (navi.isConstant()) {      // Reached end of path
    if (navi.terminalValue()){   // Case of a valid path
      *result = terminate(init);
      ++result;
    }
  }
  else {
    result = dd_transform(navi.thenBranch(), then_binop(init, *navi), result,
                 then_binop, else_binop, terminate);
    result = dd_transform(navi.elseBranch(), else_binop(init, *navi), result,
                 then_binop, else_binop, terminate);
  }
  return result;
}

/// Function templates for transforming decision diagrams 
/// with special treatment of the leading term
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator, class ElseBinaryOperator, 
          class TerminalOperator, class FirstTermOp >
OutIterator
dd_transform( NaviType navi, TermType init, OutIterator result, 
              ThenBinaryOperator then_binop, ElseBinaryOperator else_binop,
              TerminalOperator terminate, FirstTermOp terminate_first ) {

  if (navi.isConstant()) {      // Reached end of path
    if (navi.terminalValue()){   // Case of a valid path - here leading term
      *result = terminate_first(init);
      ++result;
    }
  }
  else {
    result = dd_transform(navi.thenBranch(), then_binop(init, *navi), result,
                 then_binop, else_binop, terminate, terminate_first);
    result = dd_transform(navi.elseBranch(), else_binop(init, *navi), result,
                 then_binop, else_binop, terminate);
  }
  return result;
}

/// Function templates for transforming decision diagrams 
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator, class ElseBinaryOperator >
void
dd_transform( const NaviType& navi, const TermType& init, 
              const OutIterator& result, 
              const ThenBinaryOperator& then_binop, 
              const ElseBinaryOperator& else_binop ) {

  dd_transform(navi, init, result, then_binop, else_binop, 
               project_ith<1>() );
}

/// Function templates for transforming decision diagrams 
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator >
void
dd_transform( const NaviType& navi, const TermType& init, 
              const OutIterator& result, 
              const ThenBinaryOperator& then_binop ) {

  dd_transform(navi, init, result, then_binop,
               project_ith<1, 2>(),
               project_ith<1>() );
}


template <class InputIterator, class OutputIterator, 
          class FirstFunction, class UnaryFunction>
OutputIterator 
special_first_transform(InputIterator first, InputIterator last,
                        OutputIterator result, 
                         UnaryFunction op, FirstFunction firstop) {
  InputIterator second(first);
  if (second != last) {
    ++second;
    result = std::transform(first, second, result, firstop);
  }
  return std::transform(second, last, result, op);
}


/// Function templates doing a reversed copy using intermediate storage
template <class InputIterator, class Intermediate, class OutputIterator>
OutputIterator
reversed_inter_copy( InputIterator start, InputIterator finish,
                     Intermediate& inter, OutputIterator output ) {

    std::copy(start, finish, inter.begin());
    // force constant
    return std::copy( const_cast<const Intermediate&>(inter).rbegin(),
                      const_cast<const Intermediate&>(inter).rend(), 
                      output );
}

/// Function templates for checking whether a given decision diagram node is on
/// path 
template <class NaviType>
bool
dd_on_path(NaviType navi) {
 
  if (navi.isConstant()) 
    return navi.terminalValue();

  return dd_on_path(navi.thenBranch()) || dd_on_path(navi.elseBranch());
}

/// Function templates for checking whether a given decision diagram contains
/// paths with indices only in range [start, finish)
template <class NaviType, class OrderedIterator>
bool
dd_owns_term_of_indices(NaviType navi, 
                          OrderedIterator start, OrderedIterator finish) {
 
  if (!navi.isConstant()) {     // Not at end of path
    bool not_at_end;

    while( (not_at_end = (start != finish)) && (*start < *navi) )
      ++start;

    NaviType elsenode = navi.elseBranch();

    if (elsenode.isConstant() && elsenode.terminalValue())
      return true;
      

    if (not_at_end){
      
      if ( (*start == *navi) && 
           dd_owns_term_of_indices(navi.thenBranch(), start, finish))
        return true;
      else  
        return dd_owns_term_of_indices(elsenode, start, finish);
    }
    else {

      while(!elsenode.isConstant()) 
        elsenode.incrementElse();
      return elsenode.terminalValue();
    }
   
  }
  return navi.terminalValue();
}

/// Function templates extracting the terms of a given decision diagram contain
/// which contains only indices from the range [start, finish)
/// Note: Returns incremented node
template <class NaviType, class OrderedIterator, class NodeOperation>
NaviType
dd_intersect_some_index(NaviType navi, 
                        OrderedIterator start, OrderedIterator finish,
                        NodeOperation newNode ) {
 
  if (!navi.isConstant()) {     // Not at end of path
    bool not_at_end;
    while( (not_at_end = (start != finish)) && (*start < *navi) )
      ++start;

    if (not_at_end) {
      NaviType elseNode = 
        dd_intersect_some_index(navi.elseBranch(), start, finish, 
                                newNode);
  
      if (*start == *navi) {

        NaviType thenNode = 
          dd_intersect_some_index(navi.thenBranch(), start, finish, 
                                  newNode);

        return newNode(*start, navi, thenNode, elseNode); 
      }
      else
        return elseNode;
    }
    else {                      // if the start == finish, we only check 
                                // validity of the else-only branch 
      while(!navi.isConstant()) 
        navi = navi.elseBranch();
      return newNode(navi);
    }

  }

  return newNode(navi);
}



/// Function templates for debugging, prints dd indices and reference counts
template <class NaviType>
void
dd_print(NaviType navi) {
 
  if (!navi.isConstant()) {     // Not at end of path
 
    std::cout << std::endl<< "idx " << *navi <<" addr "<<
      ((DdNode*)navi)<<" ref "<<
      int(Cudd_Regular((DdNode*)navi)->ref) << std::endl;

    dd_print(navi.thenBranch());
    dd_print(navi.elseBranch());

  }
  else {
    std::cout << "const isvalid? "<<navi.isValid()<<" addr "
              <<((DdNode*)navi)<<" ref "<<
      int(Cudd_Regular((DdNode*)navi)->ref)<<std::endl;

  }
}

// Determinine the minimum of the distance between two iterators and limit
template <class IteratorType, class SizeType>
SizeType
limited_distance(IteratorType start, IteratorType finish, SizeType limit) {

  SizeType result = 0;

  while ((result < limit) && (start != finish)) {
    ++start, ++result;
  }

  return result;
}

#if 0
// Forward declaration of CTermIter template
template <class, class, class, class, class, class> class CTermIter;

// Determinine the minimum of the number of terms and limit
template <class NaviType, class SizeType>
SizeType
limited_length(NaviType navi, SizeType limit) {


  typedef CTermIter<dummy_iterator, NaviType, 
                    project_ith<1>, project_ith<1>, project_ith<1, 2>, 
    project_ith<1> >
  iterator;

  return limited_distance(iterator(navi), iterator(), limit);
}
#endif

/// A first version
/// Function templates extracting minimal elements of dd wrt. inclusion
/// Assumption, navi is navigator of dd
template <class NaviType, class DDType>
DDType
dd_minimal_elements(NaviType navi, DDType dd, DDType& multiples) {


  if (!navi.isConstant()) {     // Not at end of path

    DDType elsedd = dd.subset0(*navi);


    DDType elseMultiples;
    elsedd = dd_minimal_elements(navi.elseBranch(), elsedd, elseMultiples);

    // short cut if else and then branche equal or else contains 1
    if((navi.elseBranch() == navi.thenBranch()) || elsedd.blankness()){
      multiples = elseMultiples;
      return elsedd;
    }

    NaviType elseNavi = elseMultiples.navigation();

    int nmax;
    if (elseNavi.isConstant()){
      if (elseNavi.terminalValue())
        nmax = dd.nVariables();
      else
        nmax = *navi;
    }
    else
      nmax = *elseNavi;


    for(int i = nmax-1; i > *navi; --i){
      elseMultiples.uniteAssign(elseMultiples.change(i)); 
    }


    DDType thendd = dd.subset1(*navi);
    thendd = thendd.diff(elseMultiples);

    DDType thenMultiples;
    thendd = dd_minimal_elements(navi.thenBranch(), thendd, thenMultiples); 
 
    NaviType thenNavi = thenMultiples.navigation();


    if (thenNavi.isConstant()){
      if (thenNavi.terminalValue())
        nmax =  dd.nVariables();
      else
        nmax = *navi;
    }
    else
      nmax = *thenNavi;


    for(int i = nmax-1; i > *navi; --i){
      thenMultiples.uniteAssign(thenMultiples.change(i)); 
    }


    thenMultiples = thenMultiples.unite(elseMultiples);
    thenMultiples = thenMultiples.change(*navi);


    multiples = thenMultiples.unite(elseMultiples);
    thendd = thendd.change(*navi);

    DDType result =  thendd.unite(elsedd);

    return result;

  }

  multiples = dd;
  return dd;
}

/// temporarily (needs to be more generic)
template<class ManagerType, class ReverseIterator, class MultReverseIterator>
ZDD
cudd_generate_multiples(ManagerType& mgr, 
                        ReverseIterator start, ReverseIterator finish,
                        MultReverseIterator multStart, 
                        MultReverseIterator multFinish) {

    DdNode* prev( (mgr.getManager())->one );

    DdNode* zeroNode( (mgr.getManager())->zero ); 

    Cudd_Ref(prev);
    while(start != finish) {

      while((multStart != multFinish) && (*start < *multStart)) {

        DdNode* result = cuddUniqueInterZdd( mgr.getManager(), *multStart,
                                             prev, prev );

        Cudd_Ref(result);
        Cudd_RecursiveDerefZdd(mgr.getManager(), prev);

        prev = result;
        ++multStart;

      };

      DdNode* result = cuddUniqueInterZdd( mgr.getManager(), *start,
                                           prev, zeroNode );

      Cudd_Ref(result);
      Cudd_RecursiveDerefZdd(mgr.getManager(), prev);

      prev = result;


      if((multStart != multFinish) && (*start == *multStart))
        ++multStart;


      ++start;
    }

    while(multStart != multFinish) {

      DdNode* result = cuddUniqueInterZdd( mgr.getManager(), *multStart,
                                           prev, prev );

      Cudd_Ref(result);
      Cudd_RecursiveDerefZdd(mgr.getManager(), prev);

      prev = result;
      ++multStart;

    };

    Cudd_Deref(prev);

    return ZDD(&mgr, prev);
  }

/// temporarily (needs to be more generic)
template<class ManagerType, class ReverseIterator>
ZDD
cudd_generate_divisors(ManagerType& mgr, 
                       ReverseIterator start, ReverseIterator finish) {


    DdNode* prev= (mgr.getManager())->one;

    Cudd_Ref(prev);
    while(start != finish) {
 
      DdNode* result = cuddUniqueInterZdd( mgr.getManager(), *start,
                                           prev, prev);

      Cudd_Ref(result);
      Cudd_RecursiveDerefZdd(mgr.getManager(), prev);
 
      prev = result;
      ++start;
    }

    Cudd_Deref(prev);
 
    return ZDD(&mgr, prev);

}


template<class Iterator, class SizeType>
Iterator
bounded_max_element(Iterator start, Iterator finish, SizeType bound){

  if (*start >= bound)
    return start;

  Iterator result(start);
  if (start != finish)
    ++start;

  while (start != finish) {
    if(*start >= bound)
      return start;
    if(*start > *result)
      result = start;
    ++start;
  }

  return result;
}

/// @func generic_compare_3way
/// @brief defines lexicographic comparison for variable indices
template <class LhsType, class RhsType, class BinaryPredicate>
CTypes::comp_type
generic_compare_3way(const LhsType& lhs, const RhsType& rhs, BinaryPredicate comp) {

  if (lhs == rhs)
    return CTypes::equality;

  return (comp(lhs, rhs)?  CTypes::greater_than: CTypes::less_than);
}

/// @func dd_first_divisors_of
/// Function templates extracting the terms of a given decision diagram contain
/// which contains only indices from first lexicographical path in 
/// Note: Replacement for dd_intersect_some_index
template <class CacheManager, class NaviType, class SetType>
SetType
dd_first_divisors_of(CacheManager cache_mgr, NaviType navi, 
                     NaviType rhsNavi, SetType init ) {

  while( (!navi.isConstant()) && (*rhsNavi != *navi) ) {

    if ( (*rhsNavi < *navi) && (!rhsNavi.isConstant()) ) 
      rhsNavi.incrementThen();
    else 
      navi.incrementElse();  
  }

  if (navi.isConstant())        // At end of path
    return navi;

  // Look up, whether operation was already used
  NaviType result = cache_mgr.find(navi, rhsNavi);
    
  if (result.isValid())       // Cache lookup sucessful
    return  result;
  
  assert(*rhsNavi == *navi);
   
  // Compute new result
  init = SetType(*rhsNavi,  
                 dd_first_divisors_of(cache_mgr, 
                                      navi.thenBranch(), rhsNavi, init),
                 dd_first_divisors_of(cache_mgr, 
                                      navi.elseBranch(), rhsNavi, init) );
  // Insert result to cache
  cache_mgr.insert(navi, rhsNavi, init.navigation());

  return init;
}

template <class CacheType, class NaviType, class SetType>
SetType
dd_first_multiples_of(const CacheType& cache_mgr,
                      NaviType navi, NaviType rhsNavi, SetType init){

  if(rhsNavi.isConstant())
    if(rhsNavi.terminalValue())
      return navi;
    else
      return rhsNavi;

  if (navi.isConstant() || (*navi > *rhsNavi)) 
    return init;

  if (*navi == *rhsNavi)
    return dd_first_multiples_of(cache_mgr, navi.thenBranch(), 
                                 rhsNavi.thenBranch(), init).change(*navi);

  // Look up old result - if any
  NaviType result = cache_mgr.find(navi, rhsNavi);

  if (result.isValid())
    return result;

  // Compute new result
  init = SetType(*navi,
                 dd_first_multiples_of(cache_mgr, navi.thenBranch(), 
                                       rhsNavi, init),
                 dd_first_multiples_of(cache_mgr, navi.elseBranch(), 
                                       rhsNavi, init) );

  // Insert new result in cache
  cache_mgr.insert(navi, rhsNavi, init.navigation());

  return init;
}


template <class IteratorLike, class ForwardIteratorTag>
IteratorLike 
increment_iteratorlike(IteratorLike iter, ForwardIteratorTag) {

  return ++iter;
}

template <class IteratorLike>
IteratorLike 
increment_iteratorlike(IteratorLike iter, navigator_tag) {

  return iter.thenBranch();
}


template <class IteratorLike>
IteratorLike 
increment_iteratorlike(IteratorLike iter) {

  typedef typename std::iterator_traits<IteratorLike>::iterator_category
    iterator_category;

  return increment_iteratorlike(iter, iterator_category());
}

END_NAMESPACE_PBORI

#endif
