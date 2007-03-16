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


#include <boost/iterator/iterator_facade.hpp>

#ifndef CGenericIter_h_
#define CGenericIter_h_

BEGIN_NAMESPACE_PBORI

template <class IteratorType, class ReferenceType>
class CDelayedCore {

public:
  /// Constructor
  CDelayedCore(const IteratorType& iter): m_iter(iter) { }

  /// Default Constructor
  CDelayedCore(): m_iter() {}

public:
  friend class boost::iterator_core_access;

  /// Equility check for compatible types
  template <class RhsType>
  bool equal(const RhsType& rhs) const { return (m_iter == rhs.m_iter); }

  /// Abstract function, inherited classes must provide incrementation
  virtual void increment() = 0;

  /// Dereferencing operation
  ReferenceType dereference() const { return m_iter.term(); }
  
protected:
  /// Store unterlying iterator
  IteratorType m_iter;
};


template <class OrderType, class PolyType, class IteratorType, class ReferenceType>
class CGenericCore;


template <class BlockProperty, class DegreeProperty, class DescendingProperty, 
          class PolyType, class IteratorType,
          class ReferenceType>
class CGenericDegreeCore;

template <class DescendingProperty, 
          class PolyType, class IteratorType, class ReferenceType>
class CGenericDegreeCore<invalid_tag, valid_tag, DescendingProperty, 
                          PolyType,  IteratorType, 
                          ReferenceType>:

  public CDelayedCore <IteratorType, ReferenceType> {

  /// Base type 
  typedef CDelayedCore <IteratorType, ReferenceType> base;

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

      ++base::m_iter;
      find_next(deg, DescendingProperty());
      
      typedef CRestrictedIter<IteratorType> bounded_iterator;  
      
      if(base::m_iter == m_finish) {
        base::m_iter = std::max_element( bounded_iterator(m_start, deg),
                                         bounded_iterator(m_finish, deg), 
                                         comp() );
      }
    } 
  }

private:
  void find_next(unsigned deg, valid_tag) {
    base::m_iter = std::find(base::m_iter, m_finish, deg);
  }
  void find_next(unsigned deg, invalid_tag) {
    typedef reversed_iteration_adaptor<IteratorType> rev_iter;
    base::m_iter = std::find(rev_iter(base::m_iter), rev_iter(m_finish), 
                             deg).get();
  }

  /// Store begin and end of underlying iteration
  IteratorType m_start, m_finish;
};

/**
template <class PolyType, class IteratorType, class ReferenceType>
class CGenericCore<DegLexOrder, PolyType, IteratorType, ReferenceType>:
  public CGenericDegreeCore<invalid_tag, valid_tag, valid_tag, 
                            PolyType, IteratorType, ReferenceType> {

  /// Base type 
  typedef CGenericDegreeCore<invalid_tag, valid_tag, valid_tag, PolyType, 
                             IteratorType, 
                             ReferenceType> base;

public:
  /// Constructor
  CGenericCore(const PolyType& poly): base(poly) {}
  CGenericCore(): base() {}

};

template <class PolyType, class IteratorType, class ReferenceType>
class CGenericCore<DegRevLexAscOrder, PolyType, IteratorType, ReferenceType>:
  public CGenericDegreeCore<invalid_tag, valid_tag, invalid_tag, PolyType, 
                            IteratorType, ReferenceType> {

  /// Base type 
  typedef CGenericDegreeCore<invalid_tag, valid_tag, invalid_tag, PolyType,
                             IteratorType,  ReferenceType> base;

public:
  /// Constructor
  CGenericCore(const PolyType& poly): base(poly) {}
  CGenericCore(): base() {}

};

**/

template <class OrderType, 
          class PolyType, class IteratorType, class ReferenceType>
class CGenericCore:
  public CGenericDegreeCore<typename OrderType::blockorder_property,
                            typename OrderType::degorder_property,
                            typename OrderType::descending_property,
                            PolyType, 
                            IteratorType, ReferenceType> {

  /// Base type 
  typedef CGenericDegreeCore<typename OrderType::blockorder_property,
                             typename OrderType::degorder_property,
                             typename OrderType::descending_property,
                             PolyType, 
                             IteratorType, ReferenceType> base;

public:
  /// Constructor
  CGenericCore(const PolyType& poly): base(poly) {}
  CGenericCore(): base() {}

};

template <class PolyType, class IteratorType, class ReferenceType>
class CGenericCore<LexOrder, PolyType, IteratorType, ReferenceType>:
  public CDelayedCore <IteratorType, ReferenceType> {

  /// Base type 
  typedef CDelayedCore <IteratorType, ReferenceType> base;

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
  public CGenericCore<OrderType, PolyType, DelayedIterType, MonomType> {

public:

  /// Generic access to type of *this
  typedef CGenericCore<OrderType, PolyType, DelayedIterType, MonomType> base;

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
