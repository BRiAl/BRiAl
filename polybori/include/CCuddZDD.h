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
 *   (c) 2007 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.8  2009/06/21 22:46:28  dreyer
 * CHANGE: preparing ring-cloning (deep copy)
 *
 * Revision 1.7  2007/11/06 15:03:33  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.6  2007/07/19 11:41:47  dreyer
 * CHANGE: clean-up
 *
 * Revision 1.5  2007/07/18 18:57:00  dreyer
 * Fix: Another mysterious performance issue
 *
 * Revision 1.4  2007/07/18 15:46:14  dreyer
 * CHANGE: added documentation
 *
 * Revision 1.3  2007/07/18 15:11:00  dreyer
 * CHANGE: simplified handle_error
 *
 * Revision 1.2  2007/07/18 07:17:26  dreyer
 * CHANGE: some clean-ups
 *
 * Revision 1.1  2007/07/17 15:56:59  dreyer
 * ADD: header file for CCuddZDD; clean-up
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef CCuddZDD_h
#define CCuddZDD_h

// include basic definitions
#include "pbori_defs.h"

#include <algorithm>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>

#include <boost/weak_ptr.hpp>

#include <boost/intrusive_ptr.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/stringize.hpp>

#include "pbori_func.h"
#include "pbori_traits.h"
#include "CCuddCore.h"

  //#include "BoolePolyRing.h"
  //#include "CCuddInterface.h"

BEGIN_NAMESPACE_PBORI

/// Define code for verbosity 
#define PB_DD_VERBOSE(text) if (ddMgr->verbose) \
  std::cout << text << " for node " << node <<  \
  " ref = " << refCount() << std::endl;

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

template <class DiagramType, class RingType>
class CCuddDDBase: public CTypes {

public:
  /// Name type of *this
  typedef DiagramType diagram_type;
  typedef CCuddDDBase self;

  typedef RingType ring_type;
  PB_DECLARE_CUDD_TYPES(CCuddCore)

  /// Define shared pointer type for handling the decision diagram manager
  //  typedef typename CCuddCore::mgrcore_ptr mgrcore_ptr;



  /// Construct diagram from raw CUDD elements
  CCuddDDBase(const ring_type& ring, node_type ddNode): 
    m_ring(ring), node(ddNode) {
    if (node) Cudd_Ref(node);
    //PB_DD_VERBOSE("Standard DD constructor");
  }

  /// Copy constructor
  CCuddDDBase(const self& from): 
    m_ring(from.m_ring), node(from.node) {
    if (node) {
      Cudd_Ref(node);
      //PB_DD_VERBOSE("Copy DD constructor");
    }
  } 

  /// Default constructor
  CCuddDDBase(): m_ring(), node(NULL) {}

  /// Get (shared) pointer to decision diagram manager
  const ring_type& manager() const { return m_ring; }

  /// Get (shared) pointer to decision diagram manager
  const ring_type& ring() const { return m_ring; }

  /// Get raw decision diagram manager
             //  DdManager* getManager() const { return m_ringddMgr->manager(); }

  /// Get raw node structure
  node_type getNode() const{  return node; }

  /// Get index of curent node
  size_type NodeReadIndex() const { return Cudd_NodeReadIndex(node); }

  /// Number of nodes in the current decision diagram
  size_type nodeCount() const { return (size_type)(Cudd_DagSize(node)); }

  /// Number of references pointing here
  size_type refCount() const { 
    assert(node != NULL);
    return Cudd_Regular(node)->ref;
  }

  /// Test whether diagram represents the empty set
  bool isZero() const { return node == Cudd_ReadZero(getManager()); }

protected:
public:
  /// Get raw decision diagram manager
  DdManager* getManager() const { return m_ring->m_mgr.getManager(); }
protected:
  /// Test, whether both operands 
  void checkSameManager(const diagram_type& other) const {
    if (getManager() != other.getManager()) 
      assert(false);//todo
//       (CCuddInterface::getHandler())("Operands come from different manager.");
  }

  /// Check whether decision diagram operation in computing result was valid
  void checkReturnValue(const node_type result) const {
    checkReturnValue(result != NULL);
  }

  /// Check whether previous decision diagram operation for validity
  void checkReturnValue(const int result, const int expected = 1) const {
    if UNLIKELY(result != expected)
      assert(false);//todo
//       handle_error<>(CCuddInterface::getHandler())(Cudd_ReadErrorCode( getManager() ));
  } 

   /// @name Apply CUDD procedures to nodes
  //@{
  diagram_type apply(binary_function func, const diagram_type& rhs) const {
    checkSameManager(rhs);
    return checkedResult(func(getManager(), getNode(), rhs.getNode()));
  }

  diagram_type apply(binary_int_function func, idx_type idx) const {
    return checkedResult(func(getManager(), getNode(), idx) );
  }

  diagram_type apply(ternary_function func, 
             const diagram_type& first, const diagram_type& second) const {
    checkSameManager(first);
    checkSameManager(second);
    return checkedResult(func(getManager(), getNode(), 
                              first.getNode(), second.getNode()) );
  }

  idx_type apply(int_unary_function func) const {
    return checkedResult(func(getManager(), getNode()) );
  }
  //@}

  /// @name Test results from CUDD procedures for validity
  //@{
  diagram_type checkedResult(node_type result) const {
    checkReturnValue(result);
    return diagram_type(m_ring, result);
  }

  idx_type checkedResult(idx_type result) const {
    checkReturnValue(result);
    return result;
  }

  template <class ResultType>
  ResultType memApply(ResultType (*func)(DdManager *, node_type)) const {
    return memChecked(func(getManager(), getNode()) );
  }

  template <class ResultType>
  ResultType memChecked(ResultType result) const {
    checkReturnValue(result != (ResultType) CUDD_OUT_OF_MEM);
    return result;
  }
  // @}

  /// (Smart) pointer to decsion diagram management
  ring_type m_ring;

  /// Raw pointer to decision diagram node
  node_type node;
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
  public CCuddDDBase<CCuddZDD, boost::intrusive_ptr<CCuddCore> > {
    friend class CCuddInterface;

public:
  /// Type of actual data
  typedef CCuddCore core_type;

  /// Smart pointer to core
  typedef boost::intrusive_ptr<core_type> core_ptr;

  /// Name type of *this
  typedef CCuddZDD self;

  /// Name the type, which self is inherited from
  typedef CCuddDDBase<self,  core_ptr> base;
  typedef  base::node_type node_type;
  /// Construct ZDD from manager core and node

  CCuddZDD(const core_ptr& core, node_type bddNode): base(core, bddNode) {}

  /// Default constructor
  CCuddZDD(): base() {}

  /// Copy constructor
  CCuddZDD(const self &from): base(from) {}

  /// Destructor
  ~CCuddZDD() { deref(); }

  /// Assignment operator
  self& operator=(const self& right); // inlined below

  /// @name Logical operations
  //@{
  bool operator==(const self& other) const {
    checkSameManager(other);
    return base::node == other.node;
  }
  bool operator!=(const self& other) const { return !(*this == other); }

  bool operator<=(const self& other) const {
    return apply(Cudd_zddDiffConst, other).isZero(); 
  }
  bool operator>=(const self& other) const {
    return (other <= *this); 
  }
  bool operator<(const self& rhs) const { 
    return (*this != rhs) && (*this <= rhs);  
  }
  bool operator>(const self& other) const { 
    return (*this != other) && (*this >= other); 
  }
  //@}

  /// @note Preprocessor generated members
  /// @code
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP, Intersect, (*)(&))
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP, Union, (+)(|))
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP, Diff, (-))

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP_ASSIGN, BOOST_PP_NIL, (*)(&)(+)(|)(-))

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
  void print(int nvars, int verbosity = 1) const { std::cout.flush(); 
    if UNLIKELY(!Cudd_zddPrintDebug(getManager(), node, nvars, verbosity))
      assert(false); //todo//CCuddInterface::getHandler()("print failed");
  }
  void PrintMinterm() const  { apply(Cudd_zddPrintMinterm); }
  void PrintCover() const    { apply(Cudd_zddPrintCover); }
  //@}

  /// Determine the number of minterms
  int Count() const          { return memApply(Cudd_zddCount); }

  /// Determine the number of minterms
  double CountDouble() const { return memApply(Cudd_zddCountDouble); }

  /// Counts minterms; takes a path specifing variables number in the support
  double CountMinterm(int path) const { 
    return memChecked(Cudd_zddCountMinterm(getManager(), getNode(), path));
  }

protected:


  /// Derefering current diagram node, if unused
  void deref() {

    if (node != 0) {
      Cudd_RecursiveDerefZdd(getManager(), node);
      //    PB_DD_VERBOSE("CCuddZDD dereferencing");
    }
  }
}; //CCuddZDD

#undef PB_ZDD_APPLY
#undef PB_ZDD_OP_ASSIGN
#undef PB_ZDD_OP

// ---------------------------------------------------------------------------
// Members of class CCuddZDD
// ---------------------------------------------------------------------------

inline CCuddZDD& 
CCuddZDD::operator=(const CCuddZDD& right) {

  if UNLIKELY(this == &right) return *this;
  if LIKELY(right.node) Cudd_Ref(right.node);
  deref();
  
  node = right.node;
  m_ring = right.m_ring;
//   if (node)
//     PB_DD_VERBOSE("CCuddZDD assignment");
  
  return *this;
}

#undef PB_DD_VERBOSE

END_NAMESPACE_PBORI

#endif


