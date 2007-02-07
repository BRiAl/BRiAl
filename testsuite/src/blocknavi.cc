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
    m_stack(thestack), min_idx(mini), max_idx(maxi), m_deg_cache(deg_cache) {}

  CBlockDegreeCache<> m_deg_cache;

  bool at_end(const NaviType& navi) const {
    
    return navi.isConstant()  || (*navi >= max_idx);
  }

  bool on_path(const NaviType& navi) const {
    //   std::cout <<"onpath "<< navi.isConstant() <<" "<<
    //  navi.terminalValue() <<" "<< (*navi >= max_idx)<<std::endl;
    return (navi.isConstant() && navi.terminalValue()) || (!navi.isConstant()&&(*navi >= max_idx));
  }

  bool find_deg(IdxType upperbound) const {
    std::cout << std::endl;
    // std::cout << "upperbound  "<<upperbound<<std::endl;;
    IdxType deg = 0, max_deg = 0;
    //empty?
    assert(!m_stack.empty());
  
    std::vector<NaviType> max_path(0);
    max_path.reserve(upperbound);

    bool notFound = true;

    // goto begin of next term
 
    NaviType current = m_stack.top();current.incrementThen();deg++;
    while (notFound) {


      bool notdone = (*current >= min_idx);
      //std::cout << "push "<< *current<<" "<< (deg+1) <<std::endl;
      //   if(!notdone)
      //  return false;
      
      while ( (deg <= upperbound) && !at_end(current) ) {
        //    std::cout << "push "<< *current<<" "<< (deg+1) <<std::endl;
        m_stack.push(current);
        deg++;
        current.incrementThen();
      }
      //       std::cout << "after push "<< *current<<" "<< (deg) <<std::endl;
      if (!on_path(current) )
        current = current.elseBranch();
      //      std::cout << "on_path(current) "<< on_path(current) <<std::endl;

      if (on_path(current) ) {
        if (deg == upperbound) {
          //          std::cout << "upperbound reached ";
          return true;
        }
        //        std::cout << "max deg? "<<deg<<std::endl;
        if (deg > max_deg) {
          //          std::cout << "new max_deg "<<std::endl;
          max_deg = deg;
          max_path.clear();
          //  newlyfound = true;
        }
      }
      
      current = m_stack.top();
      NaviType next=m_stack.top(); 
       notdone = (*current >= min_idx);
      if(!notdone) {
        //      std::cout << "done ";
        // dummy_print2(max_path.begin(), max_path.end());
        dummy_append(m_stack, max_path.rbegin(), max_path.rend());
        return false;
      }
      
      while(notdone) {
        //  std::cout << "pop "<< *current<<" "<< (deg-1) <<std::endl; 
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


    //   dummy_print2(max_path.begin(), max_path.end());
    //
    dummy_append(m_stack, max_path.rbegin(), max_path.rend());
    return true;
  }


  void operator()() {

    NaviType current, next;
    IdxType deg = 0;
    bool notFound = true;
    IdxType last_deg = 0;
    // goto begin of next term
    while (notFound) {
      current = m_stack.top();
      next=m_stack.top();
//       m_stack.pop();
//       deg++;
//       current.incrementElse();
//      std::cout << "min_idx "<<min_idx <<std::endl;
      bool atend = true;
#if 0
      while(atend && (*next >= min_idx)) {
        // current = m_stack.top();
//         if (*current < min_idx)
//           return;
        //        std::cout << "a "<<*current <<std::endl;std::cout.flush();
        m_stack.pop();
        deg++;

        current.incrementElse();
        //        std::cout << "A "<<*current <<std::endl; std::cout.flush();

        std::cout << "!!!! "<<  (*current >= min_idx) <<std::endl;
        std::cout.flush();
        atend = at_end(current);
        next = m_stack.top();
        std::cout << current.isValid()<< next.isValid() <<std::endl;std::cout.flush();  std::cout.flush();

      }// while (at_end(current) && (*current >= min_idx) );
#endif


#if 1

      bool notdone = (*current >= min_idx);
      if(!notdone) {
        deg = last_deg;//dd_cached_block_degree(m_deg_cache, m_stack.top(), max_idx)-1;
        std::cout <<"deg end" <<deg <<std::endl;
        if (deg)
          find_deg(deg-1);
        return;
      }
      
      bool huhu = false;
      while(notdone) {
        // current = m_stack.top();
        //huhu =  (*current < min_idx);
        //  if (*current < min_idx)
        //  return;
        //        std::cout << "a "<<*current <<std::endl;std::cout.flush();
        m_stack.pop();
        deg++;

        current.incrementElse();
        //        std::cout << "A "<<*current <<std::endl; std::cout.flush();

        // std::cout << "!!!! "<<  (*current >= min_idx) <<std::endl;
        // std::cout.flush();
        //
        next = current;
        current = m_stack.top();
               notdone = at_end(current) && (*current >= min_idx); 
      }// while (at_end(current) && (*current >= min_idx) );
  

  
      current = next;

#endif

#if 0
      do {
        current = m_stack.top();
        if (*current < min_idx)
          return;
        //        std::cout << "a "<<*current <<std::endl;std::cout.flush();
        m_stack.pop();
        deg++;

        current.incrementElse();
        //        std::cout << "A "<<*current <<std::endl; std::cout.flush();

        std::cout << "!!!! "<<  (*current >= min_idx) <<std::endl; std::cout.flush();
      } while (at_end(current) && (*current >= min_idx) );
#endif

#if 0
      while ( (*current >= min_idx) && at_end(next) ) {
        //  current = m_stack.top();
        //if (*current < min_idx)
        //  return;
        std::cout << "a "<<*current <<" "<<*next<<std::endl;std::cout.flush();
        m_stack.pop();
        deg++;
        //        std::cout << "A "<<*current <<std::endl; std::cout.flush();
        current = m_stack.top();
        next=current.elseBranch();

        std::cout << "!!!! "<<  *current << " "<< min_idx<<" "<<*next <<std::endl; std::cout.flush();
      }
#endif
      //      std::cout << "after "<<*current <<" "<<*next<<std::endl;
      //      std::cout.flush();
      //   return;
      if (*current < min_idx) {
        //        std::cout << "???? "<<std::endl; std::cout.flush();
          return;
      }


      //      std::cout << "ab "<<deg<<" "<<*current<<std::endl;
      last_deg = deg;
      while ( (deg > 0) && !at_end(current) ) {
        //        std::cout << "b "<<deg<<" "<<*current<<std::endl;
        m_stack.push(current);
        deg--;
        current.incrementThen();
        //        std::cout << "B "<<deg<<" "<<*current<<std::endl;
      }

      //      std::cout << "c "<<deg<<" "<<*current<<" "<< max_idx<<std::endl;
      notFound = !((deg == 0) && at_end(current) );

      //    std::cout << "notFound "<<notFound<<std::endl;
    }


#if 0

    IdxType deg = 1;
    NaviType current(m_stack.top());
    m_stack.pop();//?
    // go to active block
    while(  (current.isConstant() || (*current > max_idx)) ){
      m_stack.pop();
      current = m_stack.top();
    }
    
    NaviType next(current);
    next.incrementElse();   

    bool notFound = true;

    std::cout << "b4 notfound" << std::endl;

    while (notFound) { 
      std::cout << "while notfound" << std::endl;
      std::cout << "*current "<<*current << " min_idx "<<min_idx  
                << " *next "<<*next << " max_idx "<<max_idx  << std::endl;
      std::cout <<  (*current >= min_idx)<<(next.isConstant())<<(*next >
      max_idx)<<std::endl;

      while( (*current >= min_idx) && 
             (next.isConstant() || (*next > max_idx)) ){
        std::cout << "back " << *next <<" "<<*current<< std::endl;
        //        m_stack.pop();
        current = m_stack.top();
        m_stack.pop();
        next = current;
        next.incrementElse();
        std::cout << "back2 " << *next <<" "<<*current<< std::endl; 
        deg++;
      }

      std::cout << "*current "<<*current  << std::endl;
      std::cout << "*next "<<*next  << std::endl;
      if (*next >= min_idx) {
        std::cout << "mid " << *next << std::endl;
        m_stack.push(next);
        next.incrementThen();   
        IdxType newdeg = 1; 
        std::cout << "newdeg "<< newdeg << " deg "<<deg
                << " *next "<<*next  << std::endl;
        while( (newdeg < deg) && (next.isConstant() || (*next > max_idx)) ){
          std::cout << "forw " << *next << std::endl;
          m_stack.push(next);
          next.incrementThen();  
          ++newdeg;
        }
        notFound = !((*next >= max_idx) || ( (newdeg < deg) &&
                                            ( next.isConstant() && 
                                              next.terminalValue() )) );
      }
      else {
          std::cout << "Degree not found!"<< std::endl;

        return;
      }
    }
#endif    

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
    //    std::cout << "++i "<< *idx_iter<<std::endl;
    deg_next_term<std::stack<navigator>, navigator, unsigned>
      nextop(m_stack, *(idx_iter-1), *(idx_iter), m_deg_cache);
    nextop();
#if 0
    unsigned deg = 0;
    bool found = false;

    while (!found){
      current = m_stack.top(); 
      m_stack.pop();
      ++deg;
      //= dd_cached_block_degree(m_deg_cache, current, *idx_iter);
      
      std::cout << "current " <<*current<< " "<<deg<<  " "<<*idx_iter << std::endl;
      current.incrementElse();
      
      if (*current > *idx_iter) {
        std::cout << "block end" <<*current<< " "<<deg<<  " "<<*idx_iter << std::endl;
        return *this;
      }
      if (idx_iter!=m_indices &&*current < *(idx_iter-1)) {
        std::cout << "block begin" <<*current<< " "<<deg<<  " "<<*idx_iter << std::endl;
        /*  return *this;*/
        if (deg) --deg;
      }
      std::list<navigator> thelist;
      
      std::cout <<"huhu "<< deg<<" "<<*idx_iter<<std::endl;;
      findnext(current, thelist, deg, *idx_iter);
      
      std::cout <<" FOUND?  "<< thelist.size() <<std::endl;;
      
      
      if (thelist.size() == deg) {
        std::list<navigator>::const_iterator start(thelist.begin()),
          finish(thelist.end());
        while (start != finish){
          m_stack.push(*start);
          ++start;
        }
        found = true;
      }
     
    }
#endif

    return *this;
    //    unsigned deg = m_stack.size();


#if 0



    unsigned updeg = 0;
    navigator current = m_stack.top(); 


    unsigned* idx_iter (m_indices);
    while (*idx_iter <= *current)
      ++idx_iter;

    // if(*current< *m_current_block)
      updeg++;
      //    if (*current< *(m_current_block-1))
      //  m_current_block--;

      //   if (!current.elseBranch().isEmpty()&& *current.elseBranch()< *(idx_iter-1))
      // idx_iter--;

    std::cout << "       " <<*current<<" "<<*current.elseBranch()<<" "<<*idx_iter<<std::endl; std::cout.flush();
    m_stack.pop();
    while(!m_stack.empty()&& !current.elseBranch().isConstant() &&
          (dd_cached_block_degree(m_deg_cache, current.elseBranch(), *idx_iter)<updeg)
          ) { 
      std::cout << "??? " <<*current<<" "<< dd_cached_block_degree(m_deg_cache,
                                                                   current.elseBranch(), *idx_iter)<<"!"<<updeg<<"!"<<*idx_iter<<std::endl; std::cout.flush();
  
      std::cout << "kkk " <<*current<<" "<<*current.elseBranch() <<" "<<*idx_iter<<std::endl; std::cout.flush();

      current = m_stack.top();     m_stack.pop();
      //    if(*current< *idx_iter)
        updeg++;
        if ((idx_iter!=m_indices) && *current< *(idx_iter-1)){
        idx_iter--;
        updeg=0;
      }
    }

    std::cout << "!!! " << m_stack.empty()<<*current<<*current.elseBranch() <<std::endl; std::cout.flush();
    unsigned deg = updeg;
    //   unsigned*& idx_iter (m_current_block);

    navigator navi(current);
    navi.incrementElse();
    //   if (m_stack.empty())  m_stack.push(navi);
    if(!m_stack.empty()||!navi.isEmpty()) {
        std::cout <<"navi"  << *navi<<" "<< deg<<std::endl;
      while (!navi.isConstant() ) {
        std::cout <<"nav"  << *navi<<" "<< deg<<std::endl;
        incrementBlock(navi, *idx_iter, deg);
        ++idx_iter;
      }
    }

    return *this;

#endif
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
      x1*x2*x6*x9  + x1*x2*x7*x9+ x1*x2*x7+ x1*x2*x8*x9; 

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
    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
