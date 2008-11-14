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
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.42  2008/11/14 15:06:49  dreyer
 * Fix: optimized routine for BooleSet.minimalElements
 *
 * Revision 1.41  2008/07/13 22:49:35  dreyer
 * Fix: Doxygen clean-up
 *
 * Revision 1.40  2008/03/03 12:44:32  dreyer
 * Change: More inlining, and safer constructors
 *
 * Revision 1.39  2008/02/27 16:35:14  dreyer
 * Fix: Polynomial(0|1) removed, where possible
 *
 * Revision 1.38  2007/12/14 15:05:59  dreyer
 * Fix: merged bug fixes from sf.net
 *
 * Revision 1.37  2007/12/11 14:21:08  dreyer
 * ADD: count terms containing given index
 *
 * Revision 1.36  2007/11/06 15:03:36  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.35  2007/07/31 07:43:50  dreyer
 * ADD: getBaseOrderCode(), lieInSameBlock(...), isSingleton(), isPair()...
 *
 * Revision 1.34  2007/07/27 15:15:21  dreyer
 * CHANGE: using alternative for term_accumulate (uses add-cache)
 *
 * Revision 1.33  2007/07/27 14:38:40  dreyer
 * CHANGE: Addition internally inlined
 *
 * Revision 1.32  2007/07/18 07:17:27  dreyer
 * CHANGE: some clean-ups
 *
 * Revision 1.31  2007/07/17 15:57:00  dreyer
 * ADD: header file for CCuddZDD; clean-up
 *
 * Revision 1.30  2007/07/06 14:04:22  dreyer
 * ADD: newly written C++_interface for Cudd
 *
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
#include "pbori_traits.h"

// temporarily
#include "cudd.h"
#include "cuddInt.h"
#include "CCuddInterface.h"

#ifndef pbori_algo_h_
#define pbori_algo_h_

BEGIN_NAMESPACE_PBORI


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

/// Test whether the empty set is included
template <class NaviType>
bool owns_one(NaviType navi) {
  while (!navi.isConstant() )
    navi.incrementElse();
  
  return navi.terminalValue();
}

template <class CacheMgr, class NaviType, class SetType>
SetType
dd_modulo_monomials(const CacheMgr& cache_mgr,  
                 NaviType navi, NaviType rhs, const SetType& init){

  // Managing trivial cases
  if (owns_one(rhs)) return cache_mgr.zero();

  if (navi.isConstant())
    return cache_mgr.generate(navi);

  typename SetType::idx_type index = *navi;
  while(*rhs < index )
    rhs.incrementElse();

  if (rhs.isConstant())
    return cache_mgr.generate(navi);

  if (rhs == navi)
    return cache_mgr.zero(); 

  // Cache look-up
  NaviType cached = cache_mgr.find(navi, rhs);
  if (cached.isValid()) 
    return cache_mgr.generate(cached);

  // Actual computations
  SetType result;
  if (index == *rhs){

    NaviType rhselse = rhs.elseBranch();
    SetType thenres =
      dd_modulo_monomials(cache_mgr, navi.thenBranch(), rhs.thenBranch(), init);

    result = 
      SetType(index,
              dd_modulo_monomials(cache_mgr, 
                                  thenres.navigation(), rhselse, init),
              dd_modulo_monomials(cache_mgr, 
                                  navi.elseBranch(), rhselse, init)
              );
    
  } 
  else {
    assert(*rhs > index);
    result = 
      SetType(index,
              dd_modulo_monomials(cache_mgr, navi.thenBranch(), rhs, init),
              dd_modulo_monomials(cache_mgr, navi.elseBranch(), rhs, init)
              );
  }
  cache_mgr.insert(navi, rhs, result.navigation());
  return result;
}

/// Get minimal elements with respect to inclusion
template <class CacheMgr, class ModMonCacheMgr, class NaviType, class SetType>
SetType
dd_minimal_elements(const CacheMgr& cache_mgr, const ModMonCacheMgr& modmon_mgr,
                 NaviType navi, const SetType& init){

  // Trivial Cases
  if (navi.isEmpty()) 
    return cache_mgr.generate(navi);
  
  if (owns_one(navi)) return cache_mgr.one();

  NaviType ms0 = navi.elseBranch();
  NaviType ms1 = navi.thenBranch();

  // Cache look-up
  NaviType cached = cache_mgr.find(navi);
  if (cached.isValid())
    return cache_mgr.generate(cached);
  
  SetType minimal_else = dd_minimal_elements(cache_mgr, modmon_mgr, ms0, init);
  SetType minimal_then = 
    dd_minimal_elements(cache_mgr, modmon_mgr,
                        dd_modulo_monomials(modmon_mgr, ms1,
                                            minimal_else.navigation(),
                                            init).navigation(),
                     init);
  SetType result;
  if ( (minimal_else.navigation() == ms0) 
       && (minimal_then.navigation() == ms1) )
    result = cache_mgr.generate(navi);
  else
    result = SetType(*navi, minimal_then, minimal_else);

  cache_mgr.insert(navi, result.navigation());
  return result;
}


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

template <class MgrType>
inline const MgrType&
get_mgr_core(const MgrType& rhs) { 
  return rhs;
}
inline Cudd*
get_mgr_core(const Cudd& rhs) { 
  return &const_cast<Cudd&>(rhs);
}

///@todo merge with extract_manager
inline CCuddInterface::mgrcore_ptr
get_mgr_core(const CCuddInterface& mgr) {
  return mgr.managerCore();
}

/// temporarily (needs to be more generic)
template<class ManagerType, class ReverseIterator, class MultReverseIterator>
typename manager_traits<ManagerType>::dd_base
cudd_generate_multiples(const ManagerType& mgr, 
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

    typedef typename manager_traits<ManagerType>::dd_base dd_base;
    return dd_base(get_mgr_core(mgr), prev);
  }



/// temporarily (needs to be more generic)
template<class ManagerType, class ReverseIterator>
typename manager_traits<ManagerType>::dd_base
cudd_generate_divisors(const ManagerType& mgr, 
                       ReverseIterator start, ReverseIterator finish) {

  typedef typename manager_traits<ManagerType>::dd_base dd_base;
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
    ///@todo Next line needs generalization 
      return dd_base(get_mgr_core(mgr), prev);

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

/// defines lexicographic comparison for variable indices
template <class LhsType, class RhsType, class BinaryPredicate>
CTypes::comp_type
generic_compare_3way(const LhsType& lhs, const RhsType& rhs, BinaryPredicate comp) {

  if (lhs == rhs)
    return CTypes::equality;

  return (comp(lhs, rhs)?  CTypes::greater_than: CTypes::less_than);
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

#ifdef PBORI_LOWLEVEL_XOR 

// dummy for cuddcache (implemented in pbori_routines.cc)
DdNode* 
pboriCuddZddUnionXor__(DdManager *, DdNode *, DdNode *);


/// The following should be made more generic 
/// @todo This is still Cudd-like style, should be rewritten with PolyBoRi's
/// cache wrapper, which would the dependency on cuddInt.h
template <class MgrType, class NodeType>
NodeType
pboriCuddZddUnionXor(MgrType zdd, NodeType P, NodeType Q) {

  int		p_top, q_top;
  NodeType empty = DD_ZERO(zdd), t, e, res;
  MgrType table = zdd;
  
  statLine(zdd);
  
  if (P == empty)
    return(Q); 
  if (Q == empty)
    return(P);
  if (P == Q)
    return(empty);

  /* Check cache */
  res = cuddCacheLookup2Zdd(table, pboriCuddZddUnionXor__, P, Q);
  if (res != NULL)
    return(res);
  
  if (cuddIsConstant(P))
    p_top = P->index;
  else
    p_top = P->index;/* zdd->permZ[P->index]; */
  if (cuddIsConstant(Q))
    q_top = Q->index;
  else
    q_top = Q->index; /* zdd->permZ[Q->index]; */
  if (p_top < q_top) {
    e = pboriCuddZddUnionXor(zdd, cuddE(P), Q);
    if (e == NULL) return (NULL);
    Cudd_Ref(e);
    res = cuddZddGetNode(zdd, P->index, cuddT(P), e);
    if (res == NULL) {
      Cudd_RecursiveDerefZdd(table, e);
      return(NULL);
    }
    Cudd_Deref(e);
  } else if (p_top > q_top) {
    e = pboriCuddZddUnionXor(zdd, P, cuddE(Q));
    if (e == NULL) return(NULL);
    Cudd_Ref(e);
    res = cuddZddGetNode(zdd, Q->index, cuddT(Q), e);
    if (res == NULL) {
      Cudd_RecursiveDerefZdd(table, e);
      return(NULL);
    }
    Cudd_Deref(e);
  } else {
    t = pboriCuddZddUnionXor(zdd, cuddT(P), cuddT(Q));
    if (t == NULL) return(NULL);
    Cudd_Ref(t);
    e = pboriCuddZddUnionXor(zdd, cuddE(P), cuddE(Q));
    if (e == NULL) {
      Cudd_RecursiveDerefZdd(table, t);
      return(NULL);
    }
    Cudd_Ref(e);
    res = cuddZddGetNode(zdd, P->index, t, e);
    if (res == NULL) {
      Cudd_RecursiveDerefZdd(table, t);
      Cudd_RecursiveDerefZdd(table, e);
      return(NULL);
    }
    Cudd_Deref(t);
    Cudd_Deref(e);
  }
  
  cuddCacheInsert2(table, pboriCuddZddUnionXor__, P, Q, res);
  
  return(res);
} /* end of pboriCuddZddUnionXor */

template <class MgrType, class NodeType>
NodeType
pboriCudd_zddUnionXor(MgrType dd, NodeType P, NodeType Q) {

  NodeType res;
  do {
    dd->reordered = 0;
    res = pboriCuddZddUnionXor(dd, P, Q);
    } while (dd->reordered == 1);
  return(res);
}

#endif // PBORI_LOWLEVEL_XOR 


template <class NaviType>
bool
dd_is_singleton(NaviType navi) {

  while(!navi.isConstant()) {
    if (!navi.elseBranch().isEmpty())
      return false;
    navi.incrementThen();
  }
  return true;
}

template <class NaviType, class BooleConstant>
BooleConstant
dd_pair_check(NaviType navi, BooleConstant allowSingleton) {

  while(!navi.isConstant()) {

    if (!navi.elseBranch().isEmpty())
      return dd_is_singleton(navi.elseBranch()) && 
        dd_is_singleton(navi.thenBranch());

    navi.incrementThen();
  }
  return allowSingleton;//();
}


template <class NaviType>
bool
dd_is_singleton_or_pair(NaviType navi) {

  return dd_pair_check(navi, true);
}

template <class NaviType>
bool
dd_is_pair(NaviType navi) {

  return dd_pair_check(navi, false);
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


template <class SizeType, class IdxType, class NaviType, class SetType>
SizeType&
count_index(SizeType& size, IdxType idx, NaviType navi, const SetType& init) {

  if (*navi == idx)
    combine_sizes(SetType(navi.incrementThen(), init.ring()), size);

  if (*navi < idx) {
    count_index(size, idx, navi.thenBranch(), init);
    count_index(size, idx, navi.elseBranch(), init);
  }
  return size;
}


template <class SizeType, class IdxType, class SetType>
SizeType&
count_index(SizeType& size, IdxType idx, const SetType& bset) {

  return count_index(size, idx, bset.navigation(), SetType());
}

END_NAMESPACE_PBORI

#endif
