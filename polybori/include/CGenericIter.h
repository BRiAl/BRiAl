// -*- c++ -*-
//*****************************************************************************
/** @file CGenericIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-06
 *
 * This file defines a degree lexicographic iterator.
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
 * Revision 1.10  2007/04/12 09:12:15  dreyer
 * FIX: 1-polynomial now handles corretly by block ordering
 *
 * Revision 1.9  2007/04/05 15:38:32  dreyer
 * CHANGE: experimenting with shared pointers
 *
 * Revision 1.8  2007/04/05 10:49:52  dreyer
 * FIX: ordered iterator does deep copies now
 *
 * Revision 1.7  2007/03/28 12:34:57  dreyer
 * ADD: added testsuite testcases for blockordering; Fixed errors in block-order
 *
 * Revision 1.6  2007/03/21 08:55:08  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.5  2007/03/19 16:49:38  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.4  2007/03/16 16:59:20  dreyer
 * CHANGE: started to rewrite CGenericIter using boost:iterator_facade
 *
 * Revision 1.3  2006/10/05 07:33:58  dreyer
 * ADD: BoolePolynomial::genericExpBegin()/End()
 *
 * Revision 1.2  2006/10/04 15:46:49  dreyer
 * ADD: divisorsOf(exp_type), orderedExpBegin/End;
 * CHANGE: Polynomials printing respects ordering
 *
 * Revision 1.1  2006/10/04 13:09:56  dreyer
 * ADD: added compile-time optimied iterators and genericBegin/genericEnd
 *
 * @endverbatim
**/
//*****************************************************************************


// include basic definitions
#include "pbori_defs.h"
#include "pbori_routines.h"

#include "BoolePolynomial.h"


#include "CRestrictedIter.h"
#include "CDegreeCache.h"
#include <boost/iterator/iterator_facade.hpp>

#ifndef CGenericIter_h_
#define CGenericIter_h_

BEGIN_NAMESPACE_PBORI
// class block_degree;

template <class, class, class>
class CBlockDegreeCache;

template<class NaviType, class DescendingProperty>
class bounded_restricted_term;

// Empty class dummy
class CIterCore { };

// Abstract interface
template <class IteratorType, class ReferenceType>
class CAbstractIterCore {

public:
  typedef CAbstractIterCore<IteratorType, ReferenceType> iterator_core;
  typedef PBORI_SHARED_PTR(iterator_core) core_pointer;


  CAbstractIterCore(const IteratorType& iter): m_iter(iter) {}
  CAbstractIterCore(): m_iter() {}

  /// Equility check for compatible types
  //template <class RhsType>
  virtual bool equality(const IteratorType& rhs) const = 0;

  /// Abstract function, inherited classes must provide incrementation
  virtual void increment() = 0;

  /// Dereferencing operation
  //virtual 
  ReferenceType dereference() const {
    typedef CDelayedTermIter<ReferenceType, 
      change_assign<>, project_ith<2>, 
      IteratorType> iter_type;

    return iter_type(m_iter).term(); 
  }
  virtual core_pointer copy() const = 0;
  /// Equility check for compatible types

  bool equal(const CAbstractIterCore& rhs) const { return (m_iter == rhs.m_iter); }

  /// Store unterlying iterator temparily
  IteratorType m_iter;

};

template<class StackType>
void print_stack(StackType thestack) {
    std::cout <<"(";
    while (!thestack.empty()){
      if (thestack.top().isValid())
        std::cout << *(thestack.top()) << ", ";
      else 
        std::cout << "one";
      std::cout.flush();
      thestack.pop();
    }

    std::cout <<")";
}

template <class IteratorType, class ReferenceType, 
          class RHSIterator = IteratorType, 
          class IsAbstract = valid_tag>
class CDelayedCore:
  public CAbstractIterCore<RHSIterator, ReferenceType> {
  //  on_same_type<IsAbstract, invalid_tag, CIterCore, 
  //               CAbstractIterCore<RHSIterator, ReferenceType> >::type {

public:
  /// Constructor
  typedef CAbstractIterCore<RHSIterator, ReferenceType> base;
  CDelayedCore(const IteratorType& iter): base(iter) { }

  /// Default Constructor
  CDelayedCore(): base() {}

public:
  friend class boost::iterator_core_access;

  bool equality(const RHSIterator& rhs) const {
    return (base::m_iter == rhs); 
  };

  /// Equility check for compatible types
  //  template <class RhsType>
  //bool equal(const RhsType& rhs) const { return equality(rhs.base::m_iter); }

  /// Abstract function, inherited classes must provide incrementation
  //  virtual void increment() = 0;


  
protected:
  /// Store unterlying iterator
  // IteratorType m_iter;
};


template <class OrderType, class PolyType, class IteratorType, class
ReferenceType, class RHSIterator>
class CGenericCore;


template <class BlockProperty, class DegreeProperty, class DescendingProperty, 
          class PolyType, class IteratorType,
          class ReferenceType, class RHSIterator>
class CGenericDegreeCore;

template <class DescendingProperty, 
          class PolyType, class IteratorType, class ReferenceType, class RHSIterator>
class CGenericDegreeCore<invalid_tag, valid_tag, DescendingProperty, 
                          PolyType,  IteratorType, 
                         ReferenceType, RHSIterator>:

  public CDelayedCore <IteratorType, ReferenceType, RHSIterator> {

  /// Base type 
  typedef CDelayedCore <IteratorType, ReferenceType, RHSIterator> base;

public:

  typedef typename 
  on_same_type<DescendingProperty, valid_tag, 
               std::less<unsigned>, std::less_equal<unsigned> >::type comp;


  /// Constructor
  CGenericDegreeCore(const PolyType& poly): 
    base(), 
    m_start((IteratorType)poly.navigation()),
    m_finish((IteratorType)poly.endOfNavigation()) {
    base::m_iter = std::max_element(m_start, m_finish, comp() );
  }

  // Default Constructor
  CGenericDegreeCore(): base(), m_start(), m_finish() {}

  /// Incrementation operation
  void increment(){

    if (base::m_iter != m_finish) {
      typename std::iterator_traits<IteratorType>::value_type deg =
        *base::m_iter;


      find_next(deg, DescendingProperty());

      typedef CRestrictedIter<IteratorType> bounded_iterator;  
      
      if(base::m_iter == m_finish) {
        base::m_iter = std::max_element( bounded_iterator(m_start, deg),
                                         bounded_iterator(m_finish, deg), 
                                         comp() );
      }
    } 
  }

  typedef CAbstractIterCore<IteratorType, ReferenceType> iterator_core;
  typedef PBORI_SHARED_PTR(iterator_core) core_pointer;
  core_pointer copy() const {

    return core_pointer(new CGenericDegreeCore<invalid_tag, valid_tag,
                        DescendingProperty,  
                        PolyType,  IteratorType, 
                        ReferenceType, RHSIterator>(*this));
  }
private:
  void find_next(unsigned deg, valid_tag) {
    ++base::m_iter;
    base::m_iter = std::find(base::m_iter, m_finish, deg);
  }
  void find_next(unsigned deg, invalid_tag) {

    --base::m_iter;
    typedef reversed_iteration_adaptor<IteratorType> rev_iter;
    base::m_iter = std::find(rev_iter(base::m_iter), rev_iter(m_finish), 
                             deg).get();
  }

  /// Store begin and end of underlying iteration
  IteratorType m_start, m_finish;
};



template <class OrderType, 
          class PolyType, class IteratorType, class ReferenceType,
          class RHSIterator>
class CGenericCore:
  public CGenericDegreeCore<typename OrderType::blockorder_property,
                            typename OrderType::degorder_property,
                            typename OrderType::descending_property,
                            PolyType, 
                            IteratorType, ReferenceType, RHSIterator> {

  /// Base type 
  typedef CGenericDegreeCore<typename OrderType::blockorder_property,
                             typename OrderType::degorder_property,
                             typename OrderType::descending_property,
                             PolyType, 
                             IteratorType, ReferenceType, RHSIterator> base;

public:
  /// Constructor
  CGenericCore(const PolyType& poly): base(poly) {}
  CGenericCore(): base() {}

};


/*

template <class PolyType, class IteratorType, class ReferenceType,
          class RHSIterator>
class CGenericCore<DegLexOrder, PolyType, IteratorType, ReferenceType, RHSIterator>:
  public CDelayedCore <IteratorType, ReferenceType, RHSIterator> {

  /// Base type 
  typedef CDelayedCore <IteratorType, ReferenceType, RHSIterator> base;

public:
  /// Constructor
  CGenericCore(const PolyType& poly): 
    base(std::max_element(IteratorType(poly.navigation()), 
                          IteratorType(poly.endOfNavigation()))) { }

  // Default Constructor
  CGenericCore(): base() {}

  /// Incrementation operation
  void increment(){
    typedef IteratorType iterator;
    typedef CRestrictedIter<iterator> bounded_iterator;
    
    
    if ((this->m_iter) != m_finish) {
      unsigned deg = *(this->m_iter);
      ++(this->m_iter);
      (this->m_iter) = std::find((this->m_iter), m_finish, deg);
      
      if((this->m_iter) == m_finish) {
        (this->m_iter) = std::max_element( bounded_iterator(m_start, deg),
                                 bounded_iterator(m_finish, deg) );
        
      }
    }
  }
  /// Store begin and end of underlying iteration
  IteratorType m_start, m_finish;
};

template <class PolyType, class IteratorType, class ReferenceType,
          class RHSIterator>
class CGenericCore<DegRevLexAscOrder, PolyType, IteratorType, ReferenceType, RHSIterator>:
  public CDelayedCore <IteratorType, ReferenceType, RHSIterator> {

  /// Base type 
  typedef CDelayedCore <IteratorType, ReferenceType, RHSIterator> base;

public:
  /// Constructor
  CGenericCore(const PolyType& poly): 
    base(std::max_element(IteratorType(poly.navigation()), 
                          IteratorType(poly.endOfNavigation()),
                          std::less_equal<unsigned>())) { }

  // Default Constructor
  CGenericCore(): base() {}

  /// Incrementation operation
  void increment(){
    typedef IteratorType iterator;
    typedef CRestrictedIter<iterator> bounded_iterator;
    
    
    if ((this->m_iter) != m_finish) {
      
      unsigned deg = *(this->m_iter);
      --(this->m_iter);
      (this->m_iter) = std::find(reversed_iteration_adaptor<iterator>((this->m_iter)), 
                               reversed_iteration_adaptor<iterator>(m_finish), deg).get();
      
      if((this->m_iter) == m_finish) {
        (this->m_iter) = std::max_element( bounded_iterator(m_start, deg),
                                         bounded_iterator(m_finish, deg), 
                                         std::less_equal<unsigned>() );
        
      }
    }

  }
  /// Store begin and end of underlying iteration
  IteratorType m_start, m_finish;
};
*/
template <class PolyType, class IteratorType, class ReferenceType,
          class RHSIterator>
class CGenericCore<LexOrder, PolyType, IteratorType, ReferenceType, RHSIterator>:
  public CDelayedCore <IteratorType, ReferenceType, RHSIterator> {

  /// Base type 
  typedef CDelayedCore <IteratorType, ReferenceType, RHSIterator> base;

public:
  /// Constructor
  CGenericCore(const PolyType& poly): 
    base((IteratorType)poly.navigation()) { }

  // Default Constructor
  CGenericCore(): base() {}

  /// Incrementation operation
  void increment(){
    ++base::m_iter;
  }

  typedef CAbstractIterCore<IteratorType, ReferenceType> iterator_core;
  typedef PBORI_SHARED_PTR(iterator_core) core_pointer;
  core_pointer copy() const {

    return core_pointer(new CGenericCore<LexOrder, PolyType, IteratorType, ReferenceType,
                        RHSIterator>(*this));
  }


};


template <class StackType, class NaviType, class IdxType>
class deg_next_term {
public:

  deg_next_term(StackType& thestack, IdxType mini, IdxType maxi,
                NaviType navi):
    m_stack(thestack), min_idx(mini), max_idx(maxi)/*, m_deg_cache(deg_cache)*/ 
    , m_navi(navi)
{

    assert(mini < maxi);
  }



  //CBlockDegreeCache<> m_deg_cache;

  bool at_end(const NaviType& navi) const {
    
    return navi.isConstant()  || (*navi >= max_idx);
  }

  bool on_path(const NaviType& navi) const {

    return (navi.isConstant() && navi.terminalValue()) ||
      (!navi.isConstant()&&(*navi >= max_idx));
  }

  void print() const {
      std::cout << ":";
      std::cout.flush();
    StackType thestack( m_stack);
    while (!thestack.empty()){
      if (thestack.top().isValid())
        std::cout << *(thestack.top()) << ", ";
      else 
        std::cout << "one";
      std::cout.flush();
      thestack.pop();
    }
  }


  NaviType operator()() {

    assert(!m_stack.empty());
    unsigned deg = m_stack.size();
    NaviType current;

    if (*m_stack.top() < min_idx)
      return BoolePolynomial(false).navigation();

    
    do {

      assert(!m_stack.empty());
      current = m_stack.top();
      m_stack.pop();

      current.incrementElse();

      while (!current.isConstant() && *current < max_idx) {

        m_stack.push(current);
        current.incrementThen();
      }

    } while ( !m_stack.empty() && (*m_stack.top() >= min_idx) &&
              (current.isEmpty() || (m_stack.size() != deg)) );
  
     if (m_stack.size() == deg)
       return current;

     if (m_stack.empty() &&(deg == 0) )
      return current;

     if (m_stack.empty() )
         current = m_navi;
     else {
       current = m_stack.top();
       assert(!current.isConstant());
       current.incrementThen();
     }


     while (!current.isConstant() && (*current < min_idx))
       current.incrementElse();


     if (*current < min_idx)
       return BoolePolynomial(false).navigation();


     assert(deg > m_stack.size()); 
     bounded_restricted_term<NaviType,  valid_tag>
      bstart(current,  deg - m_stack.size() - 1, max_idx),
      bend;
     bstart = std::max_element(bstart, bend);

    dummy_append(m_stack, bstart.begin(), bstart.end());

    //  std::cout << "bstart.next();"<<*bstart.next()<<std::endl;
    return bstart.next();
  }


protected:
  StackType& m_stack;
  NaviType(m_navi);
  IdxType min_idx, max_idx;
};


template <class PolyType, class IteratorType, class ReferenceType,
          class RHSIterator>
class CGenericCore<BlockDegLexOrder, PolyType, IteratorType, ReferenceType, RHSIterator>:
  public CDelayedCore <IteratorType, ReferenceType, RHSIterator> {

  /// Base type 
  typedef CDelayedCore <IteratorType, ReferenceType, RHSIterator> base;

  typedef CGenericCore<BlockDegLexOrder, PolyType, IteratorType, ReferenceType,
                       RHSIterator> self;
public:
  /// Constructor
  CGenericCore(const PolyType& poly): 
    base(/*(IteratorType)poly.navigation()*/), m_indices(BoolePolyRing::blockRingBegin()), 
    m_current_block(BoolePolyRing::blockRingBegin()),
    m_deg_cache(poly.diagram().manager()), m_navi(poly.navigation()) {    

  
     findTerminal(poly.navigation());   


    if ((this->m_iter).empty() && poly.navigation().terminalValue()) {
      *this = self();      ///     base::clear();
      (this->m_iter).m_stack.push(navigator());
    }
  }

  // Default Constructor
  CGenericCore(): base(), m_indices(), m_current_block(),
                  m_deg_cache(PolyType().diagram().manager()),
                  m_navi(){}

  /// Incrementation operation

              //  typedef DelayedIterator base;
              //  typedef CBlockIterator<base> self;
              //  typedef typename base::stack_type stack_type;
  typedef typename PolyType::navigator navigator;
  typedef typename PolyType::size_type size_type;
  typedef typename PolyType::idx_type idx_type;
   /// Type for block indices
  typedef std::vector<idx_type> block_idx_type;

  typedef dstack<navigator> stack_type;

  /// Type for block iterators
  typedef typename block_idx_type::const_iterator block_iterator;


//   CBlockIterator(navigator navi, unsigned* indices, 
//                  const CBlockDegreeCache<>& deg_cache):
//     base(),  m_indices(indices), m_deg_cache(deg_cache), 
//     m_current_block(indices) {
//     findTerminal(navi);   
//   }

  size_type currentBlockDegree(const navigator& navi) const {
    return dd_cached_block_degree(m_deg_cache, navi, *m_current_block);
  }
  
  void incrementBlock(navigator& navi) {
    incrementBlock(navi, currentBlockDegree(navi));
  }

  void incrementBlock(navigator& navi, unsigned deg) {

    while(deg > 0) {
      --deg;
      if ( currentBlockDegree(navi.thenBranch()) == deg){
        assert(!navi.isConstant());
        (this->m_iter).m_stack.push(navi);
        navi.incrementThen(); 
      }
      else {
        ++deg;
        navi.incrementElse();
        assert(!navi.isConstant());
      }
    }
  }


  idx_type blockMin() const {
    return ( m_current_block == m_indices? 0: *(m_current_block - 1) );
  }

  idx_type blockMax() const {
    return *m_current_block;
  }

  void increment() {



    // the zero term
    if ((this->m_iter).empty())
      return;

    navigator current = (this->m_iter).m_stack.top(); 
  
    // the term one
    if (!current.isValid()) {
      *this = self();
      return;
    }

    while (*current < blockMin())
      --m_current_block;

   
    ++m_current_block;

    do {
      --m_current_block;
    
      deg_next_term<stack_type, navigator, unsigned>
        nextop((this->m_iter).m_stack,  blockMin(), blockMax(), m_navi);
  
      current = nextop();

    } while (!(this->m_iter).empty() && current.isEmpty());
    //   std::cout << "empty "<<(this->m_iter).empty() << " "<<current.isEmpty()<<std::endl;


    findTerminal(current);

    if ((this->m_iter).empty() && current.terminalValue()) {
      *this = self();      ///     base::clear();
      (this->m_iter).m_stack.push(navigator());
    }
 
  }

  bool atBlockEnd(navigator navi) const {
    return navi.isConstant() || (*navi >= blockMax());
  }

  // template <class IdxIterator>
  void findTerminal(navigator navi) {
    if (!navi.isConstant() ) 
      incrementBlock(navi);

    while (!navi.isConstant()  ) {
      assert (blockMax() != CUDD_MAXINDEX);
      ++m_current_block;

      incrementBlock(navi);
    }
  }


  // std::stack<navigator> base::m_stack;

  block_iterator m_indices;
  block_iterator m_current_block;

  navigator m_navi;

  CBlockDegreeCache<CCacheTypes::block_degree, CTypes::dd_type,
                          CTypes::manager_base> m_deg_cache; 
  typedef CAbstractIterCore<IteratorType, ReferenceType> iterator_core;
  typedef PBORI_SHARED_PTR(iterator_core) core_pointer;
  core_pointer copy() const {

    return core_pointer(new  CGenericCore<BlockDegLexOrder, PolyType, IteratorType, ReferenceType, RHSIterator>(*this));
  }

};



template<class OrderType, class PolyType, class MonomType, 
         class IteratorType, 
         class DelayedIterType// = CDelayedTermIter<MonomType, change_assign<>, 
         //                  project_ith<2>, IteratorType>
>
class CGenericIter: 
  public boost::iterator_facade<
  CGenericIter<OrderType, PolyType, MonomType, IteratorType, DelayedIterType>,
  MonomType, boost::forward_traversal_tag, MonomType
  >,
  public CGenericCore<OrderType, PolyType, DelayedIterType, MonomType, IteratorType> {

public:

  /// Generic access to type of *this
  typedef CGenericCore<OrderType, PolyType, DelayedIterType, MonomType, IteratorType> base;

  // Constructor
  CGenericIter(const PolyType& poly): 
    base(poly) {}
   
   // (for end-of-path marker)
  //   CGenericIter(const PolyType& poly, int): base() {}
//     m_iter(poly.endOfNavigation()),  m_data(poly) {
//   }

   // Default (for end-of-path marker)
   CGenericIter(): base() {}

};



END_NAMESPACE_PBORI

#endif
