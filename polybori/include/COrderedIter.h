// -*- c++ -*-
//*****************************************************************************
/** @file COrderedIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-06
 *
 * This file defines an iterator, which respects the current ordering.
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
 * Revision 1.15  2007/05/02 14:27:43  dreyer
 * FIX: typo in docu
 *
 * Revision 1.14  2007/04/30 15:20:31  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.13  2007/04/27 21:20:04  dreyer
 * CHANGE: testing exponent iterator
 *
 * Revision 1.12  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.11  2007/04/12 09:12:16  dreyer
 * FIX: 1-polynomial now handles corretly by block ordering
 *
 * Revision 1.10  2007/04/05 15:38:33  dreyer
 * CHANGE: experimenting with shared pointers
 *
 * Revision 1.9  2007/04/05 12:31:22  dreyer
 * CHANGE: ordered iterator uses deep copy only if necessary
 *
 * Revision 1.8  2007/04/05 10:49:52  dreyer
 * FIX: ordered iterator does deep copies now
 *
 * Revision 1.7  2007/03/21 08:55:08  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.6  2007/03/19 16:49:39  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.5  2007/03/16 16:59:20  dreyer
 * CHANGE: started to rewrite CGenericIter using boost:iterator_facade
 *
 * Revision 1.4  2006/10/04 15:46:49  dreyer
 * ADD: divisorsOf(exp_type), orderedExpBegin/End;
 * CHANGE: Polynomials printing respects ordering
 *
 * Revision 1.3  2006/09/12 14:56:55  dreyer
 * ADD bidirectional term iterator template
 *
 * Revision 1.2  2006/09/08 16:15:27  dreyer
 * ADD: Added ordering-dependent term iteration
 *
 * Revision 1.1  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * @endverbatim
**/
//*****************************************************************************


// include basic definitions
#include "pbori_defs.h"
#include "pbori_algo.h"

 
#include "BoolePolynomial.h"
#include "BoolePolyRing.h"
#include "OrderedManager.h"
#include "CDelayedTermIter.h"
#include "CBidirectTermIter.h"
#include <algorithm>

#include "CGenericIter.h"
  //#include "CIndirectIter.h"

#ifndef COrderedIter_h_
#define COrderedIter_h_

BEGIN_NAMESPACE_PBORI


template <class MonomType>
class CAbstractTermIter {

public:

  /// Fix type for monomials
  typedef MonomType monom_type;

  /// Extract plain Boolean type
  typedef typename monom_type::bool_type bool_type;

  /// @name Interface types for standard iterator access
  //@{
  typedef monom_type value_type;
  typedef std::forward_iterator_tag iterator_category;
  //  typedef typename iterator::difference_type difference_type;
  typedef void pointer;
  typedef value_type reference;
  //@}

  /// Generic access to type of *this
  typedef CAbstractTermIter<monom_type> self;

  // Default Constructor
  CAbstractTermIter() {}

  // Destructor
  ~CAbstractTermIter() {}

  /// Constant dereference operator
  virtual reference operator*() const = 0;

  /// Prefix increment operator
  virtual self& operator++() = 0;

  /// Postfix increment operator
  virtual self operator++(int) = 0;

  /// Equality test
  virtual bool_type operator!=(const self&) const = 0;

  /// Inequality test
  virtual bool_type operator==(const self&) const = 0;
};


//////////////////////////////////////
/// HIER!
/////////////////////////////////////


// template <class NavigatorType, class MonomType>
// class CIndirectIter:
//   public boost::iterator_facade<
//   CIndirectIter<NavigatorType, MonomType>,
//   MonomType, boost::forward_traversal_tag, MonomType
//   > {

// public:

//   typedef CIndirectIter<NavigatorType, MonomType> self;
//   typedef CAbstractIterCore<NavigatorType, MonomType> iterator_core;

//   typedef typename iterator_core::const_iterator const_iterator;
//   typedef typename iterator_core::const_reverse_iterator 
//   const_reverse_iterator;
//   typedef typename iterator_core::size_type size_type;
//   typedef typename iterator_core::idx_type idx_type;


//   /// Fix type of direct iterator
//   typedef NavigatorType navigator;
 
//   // Store shared pointer of iterator
//   typedef PBORI_SHARED_PTR(iterator_core) core_pointer;

//   /// Extract plain Boolean type
//   typedef bool bool_type;

//   // Default Constructor

//   CIndirectIter(core_pointer rhs): p_iter(rhs) {}

// //   CIndirectIter(const self& rhs):
// //     p_iter(rhs.p_iter) {}

//   // Destructor
//   ~CIndirectIter() {}

//   bool equal(const CIndirectIter& rhs) const { 
//     return  p_iter->equal(*rhs.p_iter); }

//   /// Incrementation
//   void increment() {
//     if (!p_iter.unique()) {
//       core_pointer tmp(p_iter->copy());
//       p_iter = tmp;
//     }

//     p_iter->increment(); 
//   }

//   /// Dereferencing operation
//   MonomType dereference() const {  return p_iter->dereference(); }

//   const_iterator begin() const { return p_iter->begin(); }
//   const_iterator end() const { return p_iter->end(); }
//   const_reverse_iterator rbegin() const { return p_iter->rbegin(); }
//   const_reverse_iterator rend() const { return p_iter->rend(); }

//   size_type deg() const { return p_iter->deg(); }
//   idx_type firstIndex() const { return p_iter->firstIndex(); }
// protected:
//   core_pointer p_iter;
// };



template <class NavigatorType>
class CAbstractStackBase {
public:
  typedef NavigatorType navigator;

  typedef CAbstractStackBase<NavigatorType> self;
  typedef CTermStackBase<NavigatorType, self> iterator_core;
  typedef PBORI_SHARED_PTR(iterator_core) core_pointer;

  virtual void increment() = 0;
  virtual core_pointer copy() const = 0;

  virtual ~CAbstractStackBase() {}
};



template <class StackType>
class CWrappedStack:
  public StackType {
public:
  typedef StackType base;
  typedef CWrappedStack<StackType> self;

  typedef typename base::navigator navigator;

  typedef typename base::iterator_core iterator_core;
  typedef PBORI_SHARED_PTR(iterator_core) core_pointer;

  CWrappedStack(navigator navi): base(navi) {
    base::init();
  }
  CWrappedStack(): base() {}
  CWrappedStack(const self& rhs): StackType(rhs) {}


  core_pointer copy() const {
    return core_pointer(new self(*this));
  }

};


template<class SequenceType>
void get_term(BooleMonomial& monom, const SequenceType& seq) {

  typename SequenceType::const_reverse_iterator start(seq.rbegin()), 
    finish(seq.rend());

  while (start != finish){
    monom.changeAssign(*start);
    ++start;
  }
}


template<class SequenceType>
void get_term(BooleExponent& termexp, const SequenceType& seq) {

  termexp.reserve(seq.deg());
  typename SequenceType::const_iterator start(seq.begin()), 
    finish(seq.end());

  while (start != finish){
    termexp.push_back(*start);
    ++start;
  }
}


template<class SequenceType>
void get_term(typename CTypes::size_type& termdeg, const SequenceType& seq) {

  termdeg = seq.deg();
}

template <class NavigatorType, class MonomType>
class COrderedIter:
  public boost::iterator_facade<
  COrderedIter<NavigatorType, MonomType>,
  MonomType, std::forward_iterator_tag, MonomType
  > {

public:

  typedef COrderedIter<NavigatorType, MonomType> self;
  typedef CAbstractStackBase<NavigatorType> stack_base;
  typedef CTermStackBase<NavigatorType, stack_base> iterator_core;


  typedef typename iterator_core::const_iterator const_iterator;
  typedef typename iterator_core::const_reverse_iterator 
  const_reverse_iterator;
  typedef typename iterator_core::size_type size_type;
  typedef typename iterator_core::idx_type idx_type;


  /// Fix type of direct iterator
  typedef NavigatorType navigator;
 
  // Store shared pointer of iterator
  typedef PBORI_SHARED_PTR(iterator_core) core_pointer;

  /// Extract plain Boolean type
  typedef bool bool_type;

  // Default Constructor

  COrderedIter(core_pointer rhs): p_iter(rhs) {}

//   CIndirectIter(const self& rhs):
//     p_iter(rhs.p_iter) {}

  // Destructor
  ~COrderedIter() {}

  bool equal(const self& rhs) const { 
    return  p_iter->equal(*rhs.p_iter); }

  /// Incrementation
  void increment() {
    if (!p_iter.unique()) {
      core_pointer tmp(p_iter->copy());
      p_iter = tmp;
    }

    p_iter->increment(); 
  }


  /// Dereferencing operation
  MonomType dereference() const { 
    MonomType result;
    get_term(result, *p_iter);
    return result;
  }

  const_iterator begin() const { return p_iter->begin(); }
  const_iterator end() const { return p_iter->end(); }
  const_reverse_iterator rbegin() const { return p_iter->rbegin(); }
  const_reverse_iterator rend() const { return p_iter->rend(); }

  size_type deg() const { return p_iter->deg(); }
  idx_type firstIndex() const { return *begin(); }
protected:
  core_pointer p_iter;
};


template <class OrderType, class NavigatorType> 
struct CGenericOrderedIterType;

template <class NavigatorType> 
struct CGenericOrderedIterType<LexOrder, NavigatorType> {
  typedef CAbstractStackBase<NavigatorType> stack_base;
  typedef CWrappedStack<
    CTermStack<NavigatorType, std::forward_iterator_tag, stack_base> > type;
};


template <class NavigatorType> 
struct CGenericOrderedIterType<DegLexOrder, NavigatorType> {
  typedef CAbstractStackBase<NavigatorType> stack_base;
  typedef  CWrappedStack<
    CDegTermStack<NavigatorType, valid_tag, 
                        invalid_tag, stack_base> > type;
};

template <class NavigatorType> 
struct CGenericOrderedIterType<DegRevLexAscOrder, NavigatorType> {
  typedef CAbstractStackBase<NavigatorType> stack_base;
  typedef  CWrappedStack<
    CDegTermStack<NavigatorType,  invalid_tag, 
                        invalid_tag, stack_base> > type;
};

template <class NavigatorType> 
struct CGenericOrderedIterType<BlockDegLexOrder, NavigatorType> {
  typedef CAbstractStackBase<NavigatorType> stack_base;
  typedef CWrappedStack< CBlockTermStack<NavigatorType, valid_tag,
  stack_base> > type; 
};

template <class NavigatorType> 
struct CGenericOrderedIterType<BlockDegRevLexAscOrder, NavigatorType> {
  typedef CAbstractStackBase<NavigatorType> stack_base;
  typedef  CWrappedStack<
    CBlockTermStack<NavigatorType, invalid_tag, stack_base> > type;
};

template <class OrderType, class NavigatorType, class MonomType>
class CGenericOrderedIter:
  public COrderedIter<NavigatorType, MonomType> {
public:

  typedef typename CGenericOrderedIterType<OrderType, NavigatorType>::type
  ordered_iter_type;
  typedef COrderedIter<NavigatorType, MonomType> base;
  typedef typename base::iterator_core iterator_core;
  typedef typename base::core_pointer core_pointer;


  CGenericOrderedIter(NavigatorType navi): 
    base( core_pointer(new ordered_iter_type(navi)) ) {}

  CGenericOrderedIter(const CGenericOrderedIter& rhs): base(rhs) {}
  CGenericOrderedIter(): base( core_pointer(new ordered_iter_type()) ) {}


};

END_NAMESPACE_PBORI

#endif
