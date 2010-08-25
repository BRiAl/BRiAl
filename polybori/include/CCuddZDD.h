// -*- c++ -*-
//*****************************************************************************
/** @file CCuddZDD.h
 *
 * @author Alexander Dreyer
 * @date 2007-07-16
 *
 * This files defines a replacement for the decision diagrams of CUDD's
 * C++ interface. 
 *
 * @par Copyright:
 *   (c) 2007-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef CCuddZDD_h
#define CCuddZDD_h

// include basic definitions
#include "pbori_defs.h"

#include "pbori_func.h"
#include "pbori_traits.h"
#include "CCuddCore.h"

#include "BooleRing.h"
#include "PBoRiError.h"
#include "CExtrusivePtr.h"

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>

#include <boost/weak_ptr.hpp>


#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/stringize.hpp>




BEGIN_NAMESPACE_PBORI

/// Define code for verbosity 
#define PB_DD_VERBOSE(text) if (ddMgr->verbose) \
  std::cout << text << " for node " << node <<  \
  " ref = " << refCount() << std::endl;


/// Releasing here
template <class DataType>
inline void
extrusive_ptr_release(const DataType& data, DdNode* ptr) {
   if (ptr != NULL) {
     Cudd_RecursiveDerefZdd(data.getManager(), ptr);
   }
}

/// Incrememt reference count
template <class DataType>
inline void 
extrusive_ptr_add_ref(const DataType&, DdNode* ptr) {
  if (ptr) Cudd_Ref(ptr);
}

/** @class CCuddDDBase
 * @brief This template class defines a C++ interface to @c CUDD's decision
 * diagram structure.
 *
 * The purpose of this wrapper is just to provide an efficient and save way of
 * handling the decision diagrams. It corrects some short-comings of
 * CUDD's built-in interface.
 *
 * @attention This template class is intented for internal use only, e.g. as
 * base class for CCuddZDD.
 **/

template <class RingType, class DiagramType, class NodeType>
class CCuddDDBase: 
  public CAuxTypes {

  /// Name type of *this
  typedef CCuddDDBase self;
public:

  typedef DiagramType diagram_type;

  typedef RingType ring_type;

  typedef NodeType node_type; 
  typedef node_type* node_ptr; 
  typedef typename ring_type::mgr_type mgr_type;
  typedef node_ptr (*unary_int_function)(mgr_type*, int);
  typedef node_ptr (*void_function)(mgr_type*);

  typedef node_ptr (*unary_function)(mgr_type*, node_ptr);
  typedef node_ptr (*binary_function)(mgr_type*, node_ptr, node_ptr);
  typedef node_ptr (*binary_int_function)(mgr_type*, node_ptr, int);
  typedef node_ptr (*ternary_function)(mgr_type*, node_ptr, node_ptr, node_ptr);
 

  typedef int (*int_unary_function)(mgr_type*, node_ptr);

  /// Construct diagram from raw CUDD elements
  CCuddDDBase(const ring_type& ring, node_ptr ddNode): 
    p_node(ring, ddNode) { }

  /// Copy constructor
  CCuddDDBase(const self& from): 
    p_node(from.p_node) { } 

  /// Default constructor
  CCuddDDBase(): p_node(NULL, NULL) {}

  /// Destructor
  ~CCuddDDBase() { }

  /// Assignment operator
  diagram_type& operator=(const diagram_type& rhs) {
    if UNLIKELY(this == &rhs) return *this;
    p_node = static_cast<const self&>(rhs).p_node;
    return static_cast<diagram_type&>(*this);
  }
  /// Get reference to ring
  const ring_type& ring() const { return p_node.data(); }

  /// Get raw node structure
  node_ptr getNode() const { return p_node.get(); }

  /// Get raw decision diagram manager
  mgr_type* getManager() const { return p_node.data().getManager(); }

protected:
  /// Test, whether both operands 
  void checkSameManager(const diagram_type& other) const {
    if UNLIKELY(getManager() != other.getManager()) {
      throw std::runtime_error("Operands come from different manager.");
    }
  }

   /// @name Apply C-style procedures to nodes
  //@{
  diagram_type apply(unary_function func) const {
    return diagram_type(ring(), func(getManager(), getNode()));
  }

  diagram_type apply(binary_function func, const diagram_type& rhs) const {
    checkSameManager(rhs);
    return  diagram_type(ring(), func(getManager(), getNode(), rhs.getNode()));
  }

  diagram_type apply(binary_int_function func, idx_type idx) const {
    return  diagram_type(ring(), func(getManager(), getNode(), idx) );
  }

  diagram_type apply(ternary_function func, 
                     const diagram_type& first, const diagram_type& second) const {
    checkSameManager(first);
    checkSameManager(second);
    return diagram_type(ring(), func(getManager(), getNode(), 
                                     first.getNode(), second.getNode()) );
  }

  /// Unary functions return other types
  template <class ResultType>
  ResultType apply(ResultType (*func)(mgr_type *, node_ptr)) const {
    return func(getManager(), getNode());
  }
  // @}

protected:

  CExtrusivePtr<ring_type, node_type> p_node;
}; // CCuddDD


#define PB_ZDD_APPLY(count, data, funcname) \
  self funcname(data rhs) const {    \
    return apply(BOOST_PP_CAT(Cudd_zdd, funcname), rhs); }

#define PB_ZDD_OP_ASSIGN(count, data, op) \
  self& operator BOOST_PP_CAT(op, =)(const self& other) { \
    return *this = (*this op other); }

#define PB_ZDD_OP(count, data, op) \
  self operator op(const self& other) const { return data(other); }


/** @class CCuddZDD
 * @brief This class defines a C++ interface to @c CUDD's zero-suppressed
 * decision diagram structure.
 *
 * The purpose of this wrapper is just to provide an efficient and save way of
 * handling the decision diagrams. It extends CCuddDD for handling ZDDs.
 *
 * @attention This class is intented for internal use only. 
 * Use the highlevel classes CDDInterface<CCuddZDD>, BoolePolynomial, BooleSet,
 * or BooleMonomial instead.
 **/

class CCuddZDD : 
  public CCuddDDBase<BooleRing, CCuddZDD, DdNode> {
    friend class CCuddInterface;

  /// Name type of *this
  typedef CCuddZDD self;

public:
  /// Fix ring type
  typedef BooleRing ring_type;

  /// Name the type, which self is inherited from
  typedef CCuddDDBase<BooleRing, CCuddZDD, DdNode> base;
  typedef  base::node_ptr node_ptr;
  /// Construct ZDD from manager core and node

  CCuddZDD(const ring_type& ring, node_ptr node): base(ring, node) {
    checkAssumption(node != NULL);
  }

  /// Default constructor
  CCuddZDD(): base() {}

  /// Copy constructor
  CCuddZDD(const self &from): base(from) {}

  /// Destructor
  ~CCuddZDD() { }

  /// Assignment operator
  // self& operator=(const self& right); // inlined below

  /// @name Logical operations
  //@{
  bool operator==(const self& other) const {
    ///  checkSameManager(other);
    return base::getNode() == other.getNode();
  }
  bool operator!=(const self& other) const { return !(*this == other); }
  //@}

  /// @note Preprocessor generated members
  /// @code
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_APPLY, const self&, 
    (Product)(UnateProduct)(WeakDiv)(Divide)(WeakDivF)(DivideF)
    (Union)(Intersect)(Diff)(DiffConst))

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_APPLY, int, (Subset1)(Subset0)(Change))
  /** @endcode */

  /// If-Then-Else operation using current diagram as head
  self Ite(const self& g, const self& h) const { 
    return apply(Cudd_zddIte, g, h); 
  }

  /// @name Functions for print useful information
  //@{
  std::ostream& print(std::ostream& os) const {
    os << getNode() <<": ";

    if (isZero())
      os << "empty";
    else
      os << nNodes() << " nodes " <<  count() << "terms";

    return os;

  }
  void PrintMinterm() const  {
    checkAssumption(apply(Cudd_zddPrintMinterm) == 1);
  }
  //@}

  /// Determine the number of terms
  int count() const          { return memApply(Cudd_zddCount); }

  /// Appriximate the number of terms
  double countDouble() const { return memApply(Cudd_zddCountDouble); }
  /// Get index of curent node
  size_type rootIndex() const { return Cudd_NodeReadIndex(getNode()); }

  /// Number of nodes in the current decision diagram
  size_type nNodes() const { return (size_type)(Cudd_DagSize(getNode())); }

  /// Number of references pointing here
  size_type refCount() const { 
    assert(getNode() != NULL);
    return Cudd_Regular(getNode())->ref;
  }

  /// Test whether diagram represents the empty set
  bool isZero() const { return getNode() == Cudd_ReadZero(getManager()); }

  /// Test whether diagram represents the empty set
  bool isOne() const { return getNode() == DD_ONE(getManager()); }

protected:

  template <class ResultType>
  ResultType memApply(ResultType (*func)(mgr_type *, node_ptr)) const {
    ResultType result = apply(func);
    checkAssumption(result != (ResultType) CUDD_OUT_OF_MEM);
    return result;
  }

  /// Check whether previous decision diagram operation for validity
  void checkAssumption(bool isValid) const {
    if UNLIKELY(!isValid)
      throw std::runtime_error(error_text(getManager()));
  }

}; //CCuddZDD

// inline CCuddZDD&
// CCuddZDD::operator=(const CCuddZDD& rhs) {

//   //if UNLIKELY(this == &rhs) return *this;

//   p_node = rhs.p_node;

//   return *this;
// }

#undef PB_ZDD_APPLY
#undef PB_ZDD_OP_ASSIGN
#undef PB_ZDD_OP

// ---------------------------------------------------------------------------
// Members of class CCuddZDD
// ---------------------------------------------------------------------------


#undef PB_DD_VERBOSE

END_NAMESPACE_PBORI

#endif


