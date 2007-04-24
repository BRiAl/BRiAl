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
 * Revision 1.13  2007/04/24 11:45:39  dreyer
 * CHANGE: code clean up
 *
 * Revision 1.12  2007/04/18 15:37:28  dreyer
 * ADD: dp_asc now active
 *
 * Revision 1.11  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
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
  //#include "pbori_routines.h"

#include "BoolePolynomial.h"


#include "CRestrictedIter.h"
#include "CDegreeCache.h"
#include "CTermStack.h"
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
template <class NavigatorType, class ReferenceType>
class CAbstractIterCore {

public:
  typedef CAbstractIterCore<NavigatorType, ReferenceType> self;
  typedef PBORI_SHARED_PTR(self) core_pointer;
  typedef const CTermStackBase<NavigatorType>& stack_access_type;


  // CAbstractIterCore(const NavigatorType& navi): base(navi) {}
  CAbstractIterCore() {}


  /// Abstract function, inherited classes must provide incrementation
  virtual void increment() = 0;

   bool equal (const self& rhs) const {
     return operator stack_access_type().equal(rhs);
  }


  /// Dereferencing operation @todo: optimied procedures for exp(monom)
  ReferenceType dereference() const {

    ReferenceType result(true);
    stack_access_type theStack(*this);

    typename CTermStackBase<NavigatorType>::const_iterator 
      start(theStack.begin()), 
      finish(theStack.end());

    while (start != finish) {
      result.changeAssign(*start);
      ++start;
    }
    return result;
  }

  operator stack_access_type() const {
    return getStack();
  }

  virtual core_pointer copy() const = 0;

private:
  virtual stack_access_type getStack() const = 0;
};

template <class OrderType, class NavigatorType> 
struct CGenericCoreStackType;

template <class NavigatorType> 
struct CGenericCoreStackType<LexOrder, NavigatorType> {
  typedef CTermStack<NavigatorType, boost::forward_traversal_tag> type;
};


template <class NavigatorType> 
struct CGenericCoreStackType<DegLexOrder, NavigatorType> {
  typedef CDegTermStack<NavigatorType, 
                        valid_tag> type;
};

template <class NavigatorType> 
struct CGenericCoreStackType<DegRevLexAscOrder, NavigatorType> {
  typedef CDegTermStack<NavigatorType, 
                        invalid_tag> type;
};

template <class NavigatorType> 
struct CGenericCoreStackType<BlockDegLexOrder, NavigatorType> {
  typedef CBlockTermStack<NavigatorType, valid_tag> type;
};

template <class NavigatorType> 
struct CGenericCoreStackType<BlockDegRevLexAscOrder, NavigatorType> {
  typedef CBlockTermStack<NavigatorType, invalid_tag> type;
};

template <class OrderType, class NavigatorType, class ReferenceType>
class CGenericCore:
  public CAbstractIterCore<NavigatorType, ReferenceType>  {
public:

  typedef typename CGenericCoreStackType<OrderType, NavigatorType>::type
  stack_type;

  typedef CAbstractIterCore<NavigatorType, ReferenceType>  base;
 
  typedef CGenericCore<OrderType, NavigatorType, ReferenceType> self;

  typedef typename base::core_pointer core_pointer;

  CGenericCore(const BoolePolynomial& poly):
    base(), m_stack(poly.navigation()) {
    m_stack.init();
  }

  CGenericCore(const self& rhs):
    base(rhs), m_stack(rhs.m_stack) { }

  CGenericCore(NavigatorType navi):
    base(), m_stack(navi) {
    m_stack.init();
  }

  CGenericCore(): base() {}

  void increment() {
    m_stack.increment();
  }

  core_pointer copy() const {
    return core_pointer(new self(*this));
  }

  typename base::stack_access_type getStack() const {
    return m_stack;
  }

  //private:
  stack_type m_stack;
};


template<class OrderType, class NavigatorType, class ReferenceType>
class CGenericIter: 
  public boost::iterator_facade<
  CGenericIter<OrderType, NavigatorType, ReferenceType>,
  ReferenceType, std::forward_iterator_tag, ReferenceType
  >,
  public CGenericCore<OrderType, NavigatorType, ReferenceType> {

public:

  /// Generic access to base type
  typedef CGenericCore<OrderType, NavigatorType, ReferenceType> base;

  // Constructor
  CGenericIter(const BoolePolynomial& poly): 
    base(poly.navigation()) {}

  CGenericIter(NavigatorType navi): 
    base(navi) {}
   
  CGenericIter(): base() {}

};


END_NAMESPACE_PBORI

#endif
