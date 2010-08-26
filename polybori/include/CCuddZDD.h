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

template <class DataType, class DiagramType, class NodeType>
class CCuddDDBase {

  /// Name type of *this
  typedef CCuddDDBase self;
public:

  /// @name Template arguments
  //@{
  typedef DiagramType diagram_type;
  typedef DataType data_type;
  typedef NodeType node_type; 
  //@}

  /// Raw diagram type
  typedef node_type* node_ptr; 

  /// Global data (like Boolean rings)
  typedef typename data_type::mgr_type mgr_type;

  /// @name Applicable function types
  //@{
  typedef node_ptr (*unary_function)(mgr_type*, node_ptr);
  typedef node_ptr (*binary_function)(mgr_type*, node_ptr, node_ptr);
  typedef node_ptr (*ternary_function)(mgr_type*, node_ptr, node_ptr, node_ptr);
  //@}

  /// Construct diagram from raw CUDD elements
  CCuddDDBase(const data_type& data, node_ptr node): 
    p_node(data, node) { }

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
    return diagram_type(p_node.data(), func(getManager(), getNode()));
  }

  diagram_type apply(binary_function func, const diagram_type& rhs) const {
    checkSameManager(rhs);
    return  diagram_type(p_node.data(), func(getManager(), getNode(), rhs.getNode()));
  }


  diagram_type apply(ternary_function func, 
                     const diagram_type& first, const diagram_type& second) const {
    checkSameManager(first);
    checkSameManager(second);
    return diagram_type(p_node.data(), 
                        func(getManager(),
                             getNode(), first.getNode(), second.getNode()) );
  }

  /// Binary functions with non-diagram right-hand side
  template <class Type>
  diagram_type apply(node_ptr (*func)(mgr_type*, node_ptr, Type),
                     Type value) const {
    return  diagram_type(p_node.data(), func(getManager(), getNode(), value) );
  }

  /// Unary functions with non-diagram right-hand side
  template <class ResultType>
  ResultType apply(ResultType (*func)(mgr_type *, node_ptr)) const {
    return func(getManager(), getNode());
  }
  // @}

protected:

  CExtrusivePtr<data_type, node_type> p_node;
}; // CCuddDD


END_NAMESPACE_PBORI

#endif


