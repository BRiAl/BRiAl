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
**/
//*****************************************************************************

#ifndef polybori_routines_pbori_algo_h_
#define polybori_routines_pbori_algo_h_

// include polybori's definitions
#include <polybori/pbori_defs.h>

// get polybori's functionals
#include "pbori_func.h"
#include <polybori/common/traits.h>

// temporarily
#include <polybori/cudd/cudd.h>
#include <polybori/ring/CCuddInterface.h>

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
      int(PBORI_PREFIX(Cudd_Regular)((DdNode*)navi)->ref) << std::endl;

    dd_print(navi.thenBranch());
    dd_print(navi.elseBranch());

  }
  else {
    std::cout << "const isvalid? "<<navi.isValid()<<" addr "
              <<((DdNode*)navi)<<" ref "<<
      int(PBORI_PREFIX(Cudd_Regular)((DdNode*)navi)->ref)<<std::endl;

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
  SetType result(cache_mgr.zero());
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
    PBORI_ASSERT(*rhs > index);
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
  SetType result(cache_mgr.zero());
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


///@todo merge with extract_manager
// inline CCuddInterface::mgrcore_ptr
// get_mgr_core(const CCuddInterface& mgr) {
//   return mgr.managerCore();
// }

/// temporarily (needs to be more generic) (similar fct in CCuddDDFacade.h)
template<class ManagerType, class ReverseIterator, class MultReverseIterator,
class DDBase>
inline DDBase
cudd_generate_multiples(const ManagerType& mgr, 
                        ReverseIterator start, ReverseIterator finish,
                        MultReverseIterator multStart, 
                        MultReverseIterator multFinish, type_tag<DDBase> ) {

    DdNode* prev( (mgr.getManager())->one );

    DdNode* zeroNode( (mgr.getManager())->zero ); 

    PBORI_PREFIX(Cudd_Ref)(prev);
    while(start != finish) {

      while((multStart != multFinish) && (*start < *multStart)) {

        DdNode* result = PBORI_PREFIX(cuddUniqueInterZdd)( mgr.getManager(), *multStart,
                                             prev, prev );

        PBORI_PREFIX(Cudd_Ref)(result);
        PBORI_PREFIX(Cudd_RecursiveDerefZdd)(mgr.getManager(), prev);

        prev = result;
        ++multStart;

      };

      DdNode* result = PBORI_PREFIX(cuddUniqueInterZdd)( mgr.getManager(), *start,
                                           prev, zeroNode );

      PBORI_PREFIX(Cudd_Ref)(result);
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(mgr.getManager(), prev);

      prev = result;


      if((multStart != multFinish) && (*start == *multStart))
        ++multStart;


      ++start;
    }

    while(multStart != multFinish) {

      DdNode* result = PBORI_PREFIX(cuddUniqueInterZdd)( mgr.getManager(), *multStart,
                                           prev, prev );

      PBORI_PREFIX(Cudd_Ref)(result);
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(mgr.getManager(), prev);

      prev = result;
      ++multStart;

    };

    PBORI_PREFIX(Cudd_Deref)(prev);

    typedef DDBase dd_base;
    return dd_base(mgr, prev);
  }



/// temporarily (needs to be more generic) (similar fct in CCuddDDFacade.h)
template<class ManagerType, class ReverseIterator, class DDBase>
DDBase
cudd_generate_divisors(const ManagerType& mgr, 
                       ReverseIterator start, ReverseIterator finish, type_tag<DDBase>) {


    DdNode* prev= (mgr.getManager())->one;

    PBORI_PREFIX(Cudd_Ref)(prev);
    while(start != finish) {
 
      DdNode* result = PBORI_PREFIX(cuddUniqueInterZdd)( mgr.getManager(), *start,
                                           prev, prev);

      PBORI_PREFIX(Cudd_Ref)(result);
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(mgr.getManager(), prev);
 
      prev = result;
      ++start;
    }

    PBORI_PREFIX(Cudd_Deref)(prev);
    ///@todo Next line needs generalization 
      return DDBase(mgr, prev);

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

// dummy for PBORI_PREFIX(cuddcache) (implemented in pbori_routines.cc)
DdNode* 
pboriCuddZddUnionXor__(DdManager *, DdNode *, DdNode *);


/// The following should be made more generic 
/// @todo This is still Cudd-like style, should be rewritten with PolyBoRi's
/// cache wrapper, which would the dependency on PBORI_PREFIX(cuddInt.h)
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
  res = PBORI_PREFIX(cuddCacheLookup2Zdd)(table, pboriCuddZddUnionXor__, P, Q);
  if (res != NULL)
    return(res);
  
  if (PBORI_PREFIX(cuddIsConstant)(P))
    p_top = P->index;
  else
    p_top = P->index;/* zdd->permZ[P->index]; */
  if (PBORI_PREFIX(cuddIsConstant)(Q))
    q_top = Q->index;
  else
    q_top = Q->index; /* zdd->permZ[Q->index]; */
  if (p_top < q_top) {
    e = pboriCuddZddUnionXor(zdd, PBORI_PREFIX(cuddE)(P), Q);
    if (e == NULL) return (NULL);
    PBORI_PREFIX(Cudd_Ref)(e);
    res = PBORI_PREFIX(cuddZddGetNode)(zdd, P->index, PBORI_PREFIX(cuddT)(P), e);
    if (res == NULL) {
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(table, e);
      return(NULL);
    }
    PBORI_PREFIX(Cudd_Deref)(e);
  } else if (p_top > q_top) {
    e = pboriCuddZddUnionXor(zdd, P, PBORI_PREFIX(cuddE)(Q));
    if (e == NULL) return(NULL);
    PBORI_PREFIX(Cudd_Ref)(e);
    res = PBORI_PREFIX(cuddZddGetNode)(zdd, Q->index, PBORI_PREFIX(cuddT)(Q), e);
    if (res == NULL) {
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(table, e);
      return(NULL);
    }
    PBORI_PREFIX(Cudd_Deref)(e);
  } else {
    t = pboriCuddZddUnionXor(zdd, PBORI_PREFIX(cuddT)(P), PBORI_PREFIX(cuddT)(Q));
    if (t == NULL) return(NULL);
    PBORI_PREFIX(Cudd_Ref)(t);
    e = pboriCuddZddUnionXor(zdd, PBORI_PREFIX(cuddE)(P), PBORI_PREFIX(cuddE)(Q));
    if (e == NULL) {
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(table, t);
      return(NULL);
    }
    PBORI_PREFIX(Cudd_Ref)(e);
    res = PBORI_PREFIX(cuddZddGetNode)(zdd, P->index, t, e);
    if (res == NULL) {
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(table, t);
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(table, e);
      return(NULL);
    }
    PBORI_PREFIX(Cudd_Deref)(t);
    PBORI_PREFIX(Cudd_Deref)(e);
  }
  
  PBORI_PREFIX(cuddCacheInsert2)(table, pboriCuddZddUnionXor__, P, Q, res);
  
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

  return count_index(size, idx, bset.navigation(), SetType(bset.ring()));
}


template <class InIter, class OutIter, class Object, class MemberFuncPtr>
inline OutIter
transform(InIter start, InIter finish, OutIter result, Object& obj,
	  MemberFuncPtr func) {
  for(; start != finish; ++start, ++result)
    *result = (obj .* func)(*start);
}

template <class InIter, class Object, class MemberFuncPtr>
inline void
for_each(InIter start, InIter finish, Object& obj, MemberFuncPtr func) {
  for(; start != finish; ++start)
    (obj .* func)(*start);
}

template <class InIter, class Object, class MemberFuncPtr>
inline void
for_each(InIter start, InIter finish, const Object& obj, MemberFuncPtr func) {
  for(; start != finish; ++start)
    (obj .* func)(*start);
}
template <class Type, class Type1>
const Type&
which(bool condition, const Type1& value1, const Type& value) {
  if (condition)
    return value1;
  return  value;
}

template <class Type, class Type1, class Type2>
const Type&
which(bool cond1, const Type1& value1, 
      bool cond2, const Type2& value2, const Type& value) {
  return which(cond1, value1, which(cond2, value2, value) );
}

template <class Type, class Type1, class Type2, class Type3>
const Type&
which(bool cond1, const Type1& value1, 
      bool cond2, const Type2& value2, 
      bool cond3, const Type3& value3, const Type& value ) {
  return which(cond1, value1, cond2, value2, which(cond3, value3, value) );
}




template <class IDType, class Iterator>
bool same_rings(const IDType& id, Iterator start, Iterator finish) {
  
  while ((start != finish) && (start->ring().id() == id)) {  ++start;  }
  return (start == finish);
}

template <class Iterator>
bool same_rings(Iterator start, Iterator finish) {

  return (start == finish) || same_rings(start->ring().id(), start, finish);
}

END_NAMESPACE_PBORI

#endif
