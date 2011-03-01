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
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************


// include basic definitions
#include "pbori_defs.h"
#include "pbori_algo.h"

// get stuff for term iteration
#include "CStackSelector.h"
#include "CTermIter.h"
#include "CTermGenerator.h"


#ifndef CGenericIter_h_
#define CGenericIter_h_

BEGIN_NAMESPACE_PBORI

template <class OrderType, class NaviType, class RefType>
struct pbori_base<CGenericIter<OrderType, NaviType, RefType> > {

  typedef typename CStackSelector<OrderType, NaviType>::type stack_type;

  typedef CTermIter<stack_type, CTermGenerator<RefType> > type;
};


template <class OrderType, class NaviType, class RefType>
class CGenericIter: 
  public pbori_base<CGenericIter<OrderType, NaviType, RefType> >::type {
  
public:
  typedef typename pbori_base<CGenericIter>::type base;
  typedef typename base::term_generator term_generator;

  template <class MgrType>
  CGenericIter(NaviType navi, const MgrType& mgr): base(navi, mgr) {}
  CGenericIter(): base() {}
};

#if 0
template <class OrderType, class NaviType>
class CGenericIter<OrderType, NaviType, BooleMonomial>: 
  public pbori_base<CGenericIter<OrderType, NaviType, BooleMonomial> >::type {
  
public:
  typedef typename pbori_base<CGenericIter>::type base;
  typedef typename base::term_generator term_generator;

  template <class MgrType>
  CGenericIter(NaviType navi, const MgrType& mgr): base(navi, mgr) {}

  CGenericIter(): base() {}
};
#endif

///////////////
template <class OrderType, class NaviType, class RefType>
struct pbori_base<CReverseIter<OrderType, NaviType, RefType> > {

  typedef CReverseTermStack<NaviType, std::bidirectional_iterator_tag> stack_type;

  typedef CTermIter<stack_type, CTermGenerator<RefType> > type;
};


template <class OrderType, class NaviType, class RefType>
class CReverseIter: 
  public pbori_base<CReverseIter<OrderType, NaviType, RefType> >::type {
  
public:
  typedef typename pbori_base<CReverseIter>::type base;
  typedef typename base::term_generator term_generator;

  template <class MgrType>
  CReverseIter(NaviType navi, const MgrType& mgr): base(navi, mgr) {}

  CReverseIter(): base() {}
};



END_NAMESPACE_PBORI

#endif
