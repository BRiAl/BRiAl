 // -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests
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
 * Revision 1.3  2007/02/09 17:07:00  dreyer
 * CHANGE: experimenting once more
 *
 * Revision 1.2  2007/02/07 11:01:51  dreyer
 * CHANGE: routine
 *
 * Revision 1.1  2007/01/29 17:03:44  dreyer
 * ADD: toy sandbox for block orderings
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


template <class Iterator>
void dummy_print2(Iterator start, Iterator finish) {
  std::cout << "[";
  while (start != finish) {
    std::cout << **start <<std::endl;
    ++start;
  }
  std::cout << "]"<<std::endl;;
}
template <class StackType, class Iterator>

void dummy_append(StackType& stack, Iterator start, Iterator finish) {

  while (start != finish) {
    stack.push(*start);
    ++start;
  }
}

template <class StackType, class NaviType, class IdxType>
class deg_next_term {
public:

  deg_next_term(StackType& thestack, IdxType mini, IdxType maxi,
                const CBlockDegreeCache<>& deg_cache):
    m_stack(thestack), min_idx(mini), max_idx(maxi), m_deg_cache(deg_cache) {

    assert(mini < maxi);
  }



  CBlockDegreeCache<> m_deg_cache;

  bool at_end(const NaviType& navi) const {
    
    return navi.isConstant()  || (*navi >= max_idx);
  }

  bool on_path(const NaviType& navi) const {

    return (navi.isConstant() && navi.terminalValue()) ||
      (!navi.isConstant()&&(*navi >= max_idx));
  }

  IdxType find_deg(IdxType upperbound) const {

    IdxType deg = 0, max_deg = 0;
    //empty?
    assert(!m_stack.empty());
  
    std::vector<NaviType> max_path(0);
    max_path.reserve(upperbound);

    bool notFound = true;

    // goto begin of next term

    NaviType current = m_stack.top();
    //    std::cout <<"hihi "<<*current <<" "<<upperbound <<std::endl;
    current.incrementThen();//deg++;
    //   std::cout <<"huhu "<<*current <<std::endl;
    while (notFound) {


      bool notdone = (*current >= min_idx);
      
      while ( (deg < upperbound) && !at_end(current) ) {
        m_stack.push(current);
        deg++;
        current.incrementThen();
      }

      if (!on_path(current) ) {
        current = current.elseBranch();
        // assert(deg > 0);
        //  deg--;
        //   std::cout <<"!on "<<*current <<" "<<deg <<std::endl;
      }

      if (on_path(current) ) {
        if (deg == upperbound) {
          //std::cout <<"size deg " << deg <<std::endl;
          //   std::cout << "current3 "<<*current<<std::endl;
          //          std::cout << "top()"<< *m_stack.top()  <<std::endl;
          return deg;
        }

        if (deg > max_deg) {
          max_deg = deg;
          max_path.clear();
        }
      }
      
      current = m_stack.top();
      NaviType next=m_stack.top(); 
       notdone = (*current >= min_idx);
      if(!notdone) {
        dummy_append(m_stack, max_path.rbegin(), max_path.rend());
        //   std::cout <<"size " << max_path.size() <<":"<<deg <<std::endl;
        //  std::cout << "max_path."<< *max_path.front()  <<std::endl;
        return deg;
      }
      
      while(notdone) {
        m_stack.pop();
        deg--;
        
        if (max_path.empty() || *current < (*max_path.back()) )
          max_path.push_back(current);
        
        current.incrementElse();
        
        next = current;
        current = m_stack.top();
        notdone = at_end(current) && (*current >= min_idx); 
      }
      current = next;
      notFound = !( at_end(current) );
    }
    //    std::cout << "max_path."<< *max_path.front()  <<std::endl;
    dummy_append(m_stack, max_path.rbegin(), max_path.rend());

    //    std::cout <<"size " << max_path.size()<<std::endl;
    return deg;
  }


  bool operator()() {

    NaviType current, next;
    IdxType deg = 0;
    bool notFound = true;
    IdxType last_deg = 0;
    // goto begin of next term
    while (notFound) {
      current = m_stack.top();
      assert(!m_stack.empty());

      next=m_stack.top();
      bool atend = true;

      bool notdone = (*current >= min_idx);
      //    std::cout <<"not done" <<*current <<" "<<min_idx <<std::endl;
      if(!notdone) {
        deg = last_deg;//dd_cached_block_degree(m_deg_cache, m_stack.top(), max_idx)-1;
        //        std::cout <<"deg end" <<deg <<std::endl;
        if (deg) {
          bool tmp = (find_deg(deg-1) == 0);
          //          std::cout << "tmp "<<tmp<<std::endl;
          return tmp;
        }
        // std::cout <<"found deg" << *m_stack.top()<<std::endl;

        return true;
      }
      
      bool huhu = false;
      while(notdone) {

        m_stack.pop();
        deg++;

        current.incrementElse();

        next = current;
        if (m_stack.empty())
          return false;
        current = m_stack.top();
        notdone = at_end(current) && (*current >= min_idx); 
      }
  
      current = next;

      if (*current < min_idx) {
          return true;
      }

      last_deg = deg;
      while ( (deg > 0) && !at_end(current) ) {

        m_stack.push(current);
        deg--;
        current.incrementThen();
      }

      notFound = !((deg == 0) && at_end(current) );
    }

    return false;
  }


protected:
  StackType& m_stack;
  IdxType min_idx, max_idx;
};


class CBlockIterator {
public:
  typedef BoolePolynomial::navigator navigator;
  typedef CBlockIterator self;
  CBlockIterator(navigator navi, unsigned* indices, 
                 const CBlockDegreeCache<>& deg_cache):
    m_stack(),  m_indices(indices), m_deg_cache(deg_cache)
    {

    unsigned* idx_iter (m_indices);

    while (!navi.isConstant()  ) {
      incrementBlock(navi, *idx_iter,
                     dd_cached_block_degree(m_deg_cache, navi, *idx_iter) );
      if (*idx_iter !=CUDD_MAXINDEX) ++idx_iter;
      std::cout <<"idx "<<*idx_iter<<std::endl;
    }
  }

  void incrementBlock(navigator& navi, unsigned idx, unsigned deg) {

    while( (deg > 0)/*&&!navi.isConstant()*/) {
      unsigned tmp;
      if ( (tmp = dd_cached_block_degree(m_deg_cache, navi.thenBranch(),
                                   idx) + 1) == deg){
        m_stack.push(navi);
        std::cout<< "pn "<<*navi<<tmp<< deg<<std::endl;
        navi.incrementThen(); 
        --deg;
      }
      else {
        navi.incrementElse();
        m_stack.push(navi);
        std::cout<< "pe "<<*navi<<tmp << deg <<std::endl;
      }
    }

  }

  template <class ListType>
  void findnext(navigator navi, ListType& thelist,
                unsigned deg, unsigned maxidx){
    while( (*navi < maxidx) && (thelist.size() < deg) ){
      thelist.push_back(navi);
      navi.incrementThen();
    }
    std::cout <<"thelist.size "<< thelist.size() << " "<<deg <<std::endl;
   
    while((*navi < maxidx)&&!thelist.empty() && thelist.size() != deg) {
      // std::cout <<"huhuA "<<std::endl;
      while (!thelist.empty() && (*thelist.back().elseBranch() < maxidx  )){
        std::cout <<"huhuAB "<<std::endl;
        thelist.pop_back();
      }
      //std::cout <<"huhuB "<<std::endl;
      while( (*navi < maxidx) && (thelist.size() < deg) ){
        std::cout << "*navi " <<*navi << "maxidx " << maxidx << 
          "thelist.size()  " << thelist.size() <<"deg "<< deg <<std::endl;
        thelist.push_back(navi);
        navi.incrementThen();
      }
    }

    std::cout <<"terminates?"<<deg <<std::endl;
  }


  self & operator++() {


    unsigned updeg = 0;
    navigator current = m_stack.top(); 

    //    std::cout << "++ "<< *current<<std::endl;
    unsigned* idx_iter (m_indices);

    while (*current >= *idx_iter)
      ++idx_iter;

    bool notfound = true;

    while (notfound) {

      unsigned min_idx(idx_iter == m_indices? 0 : *(idx_iter-1) );
      //      std::cout << "++i "<<   min_idx<< " "<<*idx_iter<<std::endl;

      deg_next_term<std::stack<navigator>, navigator, unsigned>
        nextop(m_stack, min_idx, *(idx_iter), m_deg_cache);
      notfound =  nextop();
      if(notfound)  { 
        if  (idx_iter != m_indices) {
          //  std::cout <<"notfound (++) "<< std::endl;
          --idx_iter;
        }
        else {
          notfound = false; //end here
          std::cout << "already first block"<<std::endl;
        }
      }
    }

    if (m_stack.empty()) return *this;
    navigator navi(m_stack.top());
    idx_iter = (m_indices);
    std::cout << " ???? "<<  navi.isConstant()<<std::endl;std::cout.flush();
    while (*navi >= *idx_iter)
      ++idx_iter;

    while (!navi.isConstant() && (*navi.thenBranch() < *idx_iter) ) {
      navi.incrementElse();
    }
    std::cout << " navi " <<*navi <<" "<< *idx_iter <<std::endl;
    // 
    if (!navi.isConstant() ) navi.incrementThen();
 
    while (!navi.isConstant()  ) {
      incrementBlock(navi, *idx_iter,
                     dd_cached_block_degree(m_deg_cache, navi, *idx_iter) );
      if (*idx_iter !=CUDD_MAXINDEX) ++idx_iter;
//      std::cout <<"idx "<<*idx_iter<<std::endl;
    }
   
    return *this;
    //    unsigned deg = m_stack.size();


  }

  void print() const {
      std::cout << ":";
      std::cout.flush();
    std::stack<navigator> thestack( m_stack);
    while (!thestack.empty()){
      std::cout << *(thestack.top()) << ", ";
      std::cout.flush();
      thestack.pop();
    }
  }


  std::stack<navigator> m_stack;

  unsigned* m_indices;
  // unsigned* m_current_block;

  const CBlockDegreeCache<>& m_deg_cache;
};


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


    BoolePolynomial poly = 
      x1*x2*x6*x9  + x1*x2*x7*x9+ x1*x2*x7+ x1*x2*x8*x9 
      +   x1*x3*x6*x9  + x1*x3*x7*x9+ x1*x3*x7+ x1*x3*x8*x9 ; 

      // x1+ x2*x3+ x1*x2 +  x1*x2*x6  + x1*x2*x7+ x3*x4*x5;
    std::cout << "Polynom: "<< poly <<std::endl;

    CBlockDegreeCache<> blockDegCache(poly.diagram().manager());

    std::cout <<"Block degree wrt. 2: " << 
      dd_cached_block_degree(blockDegCache, poly.navigation(), 2) << std::endl;

    CCacheManagement<block_dlex_lead> cache_mgr(poly.diagram().manager());

    BoolePolynomial::navigator navi(poly.navigation());

    unsigned next_block[3] = {4, 6, CUDD_MAXINDEX};



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

    std::cout << "block iter..."<<std::endl;


    CBlockIterator biter(poly.navigation(), next_block, blockDegCache);

    biter.print();
    ++biter;
    biter.print();
    ++biter;
    biter.print();
    ++biter;
    biter.print();
    ++biter;
    biter.print();
    ++biter;
    biter.print();
    ++biter;
    biter.print(); 
    ++biter;
    biter.print();
    ++biter;
    biter.print();  

    /*  std::cout << "Experimenting with negations..."<<std::endl;

    navi = poly.navigation();
    std::cout << navi.operator->()<<std::endl;
    std::cout <<  Cudd_Not(navi.operator->())<<std::endl;
    std::cout <<  Cudd_Regular(navi.operator->())<<std::endl;
    std::cout <<  Cudd_Regular( Cudd_Not(navi.operator->()))<<std::endl;
    std::cout <<  Cudd_Complement(navi.operator->())<<std::endl;
    */
    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
