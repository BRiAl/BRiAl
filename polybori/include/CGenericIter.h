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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.21  2008/11/22 23:47:31  dreyer
 * ADD: BooleSet::rbegin(),end()
 *
 * Revision 1.20  2008/01/16 17:10:18  dreyer
 * CHANGE: term-iterators use correct manager now
 *
 * Revision 1.19  2008/01/11 16:58:57  dreyer
 * CHANGE: Experimenting with iterators and correct rings
 *
 * Revision 1.18  2007/11/06 15:03:34  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.17  2007/05/03 16:04:45  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * Revision 1.16  2007/04/30 15:20:30  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.15  2007/04/27 21:20:04  dreyer
 * CHANGE: testing exponent iterator
 *
 * Revision 1.14  2007/04/24 13:25:27  bricken
 * + virtual destructor
 *
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
