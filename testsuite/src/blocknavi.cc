// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for getting thwe leading term
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
 * Revision 1.1  2007/01/29 17:03:44  dreyer
 * ADD: toy sandbox for block orderings
 *
 * Revision 1.5  2006/05/24 11:57:51  dreyer
 * CHANGE alternative modulus operation
 *
 * Revision 1.4  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.3  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.2  2006/03/30 08:52:57  dreyer
 * CHANGE: testsuite revised
 *
 * Revision 1.1  2006/03/28 07:34:03  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************


// load standard iostream capapilities
#include <iostream>

// load polybori header file
# include "polybori.h"

#include "CDegreeCache.h"

USING_NAMESPACE_PBORI

struct block_degree: public CCacheTypes::binary_cache_tag { };
struct block_dlex_lead: public CCacheTypes::unary_cache_tag { };

template <class TagType =  block_degree,
          class DDType = typename CTypes::dd_type,
          class ManagerType = typename CTypes::manager_base>
class CBlockDegreeCache:
  public CCacheManagement<TagType, 2, ManagerType> {

public:
  /// @name Define generic access to data types
  //@{
  typedef ManagerType manager_type;
  typedef DDType dd_type;
  typedef TagType tag_type;
  typedef CCacheManagement<tag_type, 2, manager_type> base;
  typedef CBlockDegreeCache<tag_type, dd_type, manager_type> self;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type input_node_type;
  typedef typename dd_type::idx_type idx_type;
  typedef typename dd_type::size_type size_type;
  typedef typename dd_type::navigator navi_type;
  typedef CIndexHandle<navi_type> node_type;
  //@}

  /// Constructor
  CBlockDegreeCache(const manager_type& mgr): base(mgr) {}

  /// Copy Constructor
  CBlockDegreeCache(const self& rhs): base(rhs) {}

  /// Destructor
  ~CBlockDegreeCache() {}

  /// Find cached degree wrt. given navigator
  node_type find(input_node_type navi, idx_type idx) const{ 
    return node_type(base::find(navi, node_type(idx))); }

  /// Store cached degree wrt. given navigator
  void insert(input_node_type navi, idx_type idx, size_type deg) const {
    base::insert(navi, node_type(idx), node_type(deg));
  }
};


/// Function templates for determining the degree of a decision diagram
/// with the help of cache (e. g. CDegreeCache)
template <class DegreeCacher, class NaviType, class IdxType>
typename NaviType::size_type
dd_cached_block_degree(const DegreeCacher& cache, NaviType navi, 
                       IdxType nextBlock) {

  typedef typename NaviType::size_type size_type;

  if( navi.isConstant() || (*navi >= nextBlock) ) // end of block reached
    return 0;
 
  // Look whether result was cached before
  typename DegreeCacher::node_type result = cache.find(navi, nextBlock);
  if (result.isValid())
    return *result;
  
  // Get degree of then branch (contains at least one valid path)...
  size_type deg = dd_cached_block_degree(cache, navi.thenBranch(), nextBlock) + 1;
 
  // ... combine with degree of else branch
  deg = std::max(deg,  dd_cached_block_degree(cache, navi.elseBranch(), nextBlock) );

  // Write result to cache
  cache.insert(navi, nextBlock, deg);
 
  return deg;
}



template<class DegCacheMgr, class NaviType, class IdxType, class SizeType>
bool max_block_degree_on_then(const DegCacheMgr& deg_mgr, NaviType navi,
                              IdxType next_block,
                              SizeType degree, valid_tag is_descending) {
  navi.incrementThen();
  return ((dd_cached_block_degree(deg_mgr, navi, next_block/*,degree - 1*/) + 1) == degree);
}

template<class DegCacheMgr, class NaviType, class IdxType, class SizeType>
bool max_block_degree_on_then(const DegCacheMgr& deg_mgr, NaviType navi,
                              IdxType next_block,
                              SizeType degree, invalid_tag non_descending) {
  navi.incrementElse();
  return (dd_cached_block_degree(deg_mgr, navi, next_block/*, degree*/) != degree);
}


// with degree bound
template <class CacheType, class DegCacheMgr, class NaviType, 
  class TermType, class Iterator, class SizeType, class DescendingProperty>
TermType
dd_block_degree_lead(const CacheType& cache_mgr, 
                     const DegCacheMgr& deg_mgr,
                     NaviType navi, Iterator block_iter,
                     TermType init, SizeType degree,
                     DescendingProperty prop) {

  if (navi.isConstant())
    return navi;

  while( (*navi >= *block_iter) && (*block_iter != CUDD_MAXINDEX))  {
    ++block_iter;
    degree = dd_cached_block_degree(deg_mgr, navi, *block_iter);
  }


  // Check cache for previous results
  NaviType cached = cache_mgr.find(navi);
  if (cached.isValid())
    return cached;

  // Go to next branch
    if ( max_block_degree_on_then(deg_mgr, navi, *block_iter, degree, prop) ) {
    init = dd_block_degree_lead(cache_mgr, deg_mgr, navi.thenBranch(),
                                block_iter,
                                init, degree - 1, prop).change(*navi);
  }
  else {
    init = dd_block_degree_lead(cache_mgr, deg_mgr, navi.elseBranch(),
                                block_iter,
                                init, degree, prop);
  }

  NaviType resultNavi(init.navigation());
  cache_mgr.insert(navi, resultNavi);
  deg_mgr.insert(resultNavi, *block_iter, degree);

  return init;
}


template <class CacheType, class DegCacheMgr, class NaviType,  class Iterator,
          class TermType, class DescendingProperty>
TermType
dd_block_degree_lead(const CacheType& cache_mgr, const DegCacheMgr& deg_mgr,
                     NaviType navi, Iterator block_iter, TermType init,
                     DescendingProperty prop){ 

  if (navi.isConstant())
    return navi;
  
  return dd_block_degree_lead(cache_mgr, deg_mgr, navi,block_iter, init,
                              dd_cached_block_degree(deg_mgr, navi,
                              *block_iter), prop);
}



template <class FirstIterator, class SecondIterator, class IdxType, 
          class BinaryPredicate>
CTypes::comp_type
restricted_lex_compare_3way(FirstIterator start, FirstIterator finish, 
                            SecondIterator rhs_start, SecondIterator rhs_finish,
                            IdxType max_index,
                            BinaryPredicate idx_comp) {

  while ( (start != finish) && (*start < max_index) && (rhs_start != rhs_finish)
          && (*rhs_start < max_index) &&  (*start == *rhs_start) ) {
     ++start; ++rhs_start;
   }

  if ( (start == finish) || (*start >= max_index) ) {
    if ( (rhs_start == rhs_finish) || (*rhs_start >= max_index) )
       return CTypes::equality;

     return CTypes::less_than;
   }
   
  if ( (rhs_start == rhs_finish) || (*rhs_start >= max_index) )
     return CTypes::greater_than;

   return (idx_comp(*start, *rhs_start)? 
           CTypes::greater_than: CTypes::less_than);
}




template<class LhsIterator, class RhsIterator, class Iterator,
         class BinaryPredicate>
CTypes::comp_type
block_dlex_compare(LhsIterator lhsStart, LhsIterator lhsFinish,
                   RhsIterator rhsStart, RhsIterator rhsFinish, 
                   Iterator start, Iterator finish,
                   BinaryPredicate idx_comp) {

  // unsigned lhsdeg = 0, rhsdeg = 0;


  CTypes::comp_type result = CTypes::equality;

  while ( (start != finish) && (result == CTypes::equality) ) {
    unsigned lhsdeg = 0, rhsdeg = 0;
    LhsIterator oldLhs(lhsStart); // maybe one can save this and do both
    RhsIterator oldRhs(rhsStart); // comparisons at once.

    // maybe one can save 
    while( (lhsStart != lhsFinish)  &&  (*lhsStart <  *start) ) {
      ++lhsStart; ++lhsdeg;
    }
    while( (rhsStart != rhsFinish)  &&  (*rhsStart <  *start) ) {
      ++rhsStart; ++rhsdeg;
    }
    result = generic_compare_3way(lhsdeg, rhsdeg, 
                                  std::greater<unsigned>() );
  
    if (result == CTypes::equality) {
      result = restricted_lex_compare_3way(oldLhs, lhsFinish,
                                           oldRhs, rhsFinish, *start, idx_comp);
    }
  
    ++start;
  }
    
  return result;
}


int
main(){

  std::cout << "Experimenting with block orderings..." << std::endl;   

  try {
    BoolePolyRing the_ring(10);

    BooleMonomial x0 = BooleVariable(0);
    BooleMonomial x1 = BooleVariable(1);
    BooleMonomial x2 = BooleVariable(2);
    BooleMonomial x3 = BooleVariable(3);
    BooleMonomial x4 = BooleVariable(4);
    BooleMonomial x5 = BooleVariable(5);
    BooleMonomial x6 = BooleVariable(6);
    BooleMonomial x7 = BooleVariable(7);
    BooleMonomial x8 = BooleVariable(8);
    BooleMonomial x9 = BooleVariable(9);


    BoolePolynomial poly = x1*x2 +  x1*x2*x6  + x1*x2*x7+ x3*x4*x5;
    std::cout << "Polynom: "<< poly <<std::endl;

    CBlockDegreeCache<> blockDegCache(poly.diagram().manager());

    std::cout <<"Block degree wrt. 2: " << 
      dd_cached_block_degree(blockDegCache, poly.navigation(), 2) << std::endl;

    CCacheManagement<block_dlex_lead> cache_mgr(poly.diagram().manager());

    BoolePolynomial::navigator navi(poly.navigation());

    unsigned next_block[3] = {3, 5, CUDD_MAXINDEX};



    std::cout << dd_block_degree_lead(cache_mgr, blockDegCache, navi, 
                                      next_block,
                                      BooleSet(), 
                                      valid_tag());


    BooleMonomial monom1 = x1*x2*x6, monom2 = x1*x2*x7;

    std::cout << monom1<< " < " << monom2 << "? "<<
      block_dlex_compare(monom1.begin(), monom1.end(),
                         monom2.begin(), monom2.end(),
                         next_block, next_block+3, std::less<unsigned>())
              << std::endl;

    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
