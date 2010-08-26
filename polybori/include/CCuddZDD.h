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

#include <stdexcept>


BEGIN_NAMESPACE_PBORI

/** @class CCuddDDBase
 * @brief This template class defines a facade as a C++ interface for applying
 * C-style functions to C-style structs, where represent decision diagrams
 * (given by raw pointers of the nodes and the context).
 *
 * @attention We assume that the @c DiagramType owns member functions @c ring(), @c
 * getNode() and @c getManager().
 *
 * @note This template class is a facade and hence it is intented for
 * internal use only, e.g. as a base class for BooleSet.
 **/

template <class DiagramType, class NodeType>
class CCuddDDBase {

  /// Name type of *this
  typedef CCuddDDBase self;
public:

  /// @name Template arguments
  //@{
  typedef DiagramType diagram_type;
  typedef NodeType node_type; 
  //@}

  /// Raw diagram type
  typedef node_type* node_ptr; 

  /// Default constructor
  CCuddDDBase() {}

  /// Destructor
  ~CCuddDDBase() { }

  /// @name Logical operations
  //@{
  /// Equality
  bool operator==(const diagram_type& rhs) const {
    return operator node_ptr() == rhs.getNode();
  }
  /// Nonequality
  bool operator!=(const diagram_type& rhs) const { return !(*this == rhs); }
  //@}

protected:
  /// Test, whether both operands 
  void checkSameManager(const diagram_type& other) const {
    if UNLIKELY(my().getManager() != other.getManager()) {
      throw std::runtime_error("Operands come from different manager.");
    }
  }

  /// @name Apply C-style procedures to nodes
  //@{
  /// Unary function
  template <class MgrType>
  diagram_type apply(node_ptr (*func)(MgrType, node_ptr)) const {
    return diagram(func(get<MgrType>(), *this));
  }

  /// Binary function (two diagrams)
  template <class MgrType>
  diagram_type apply(node_ptr (*func)(MgrType, node_ptr, node_ptr),
                     const diagram_type& rhs) const {
    checkSameManager(rhs);
    return diagram(func(get<MgrType>(), *this, rhs));
  }

  /// Ternary function (three diagrams)
  template <class MgrType>
  diagram_type apply(node_ptr (*func)(MgrType, node_ptr, node_ptr, node_ptr),
                     const diagram_type& first, const diagram_type& second) const {
    checkSameManager(first);
    checkSameManager(second);
    return diagram(func(get<MgrType>(), *this, first, second));
  }

  /// Binary functions with non-diagram right-hand side
  template <class MgrType, class Type>
  diagram_type apply(node_ptr(*func)(MgrType, node_ptr, Type), Type value) const {
    return diagram(func(get<MgrType>(), *this, value));
  }

  /// Unary functions with non-diagram result value
  template <class MgrType, class ResultType>
  ResultType apply(ResultType(*func)(MgrType, node_ptr)) const {
    return func(get<MgrType>(), *this);
  }
  // @}

  /// Get diagram of the same context
  diagram_type diagram(node_ptr node) const {
    return diagram_type(my().ring(), node);
  }

private:

  /// Get diagram of the same context
  const diagram_type& my() const {
    return static_cast<const diagram_type&>(*this);
  }

  /// Accessing the actuall type, fir which this facade is inteded to be used for
  template<class MgrType>
  MgrType get() const { return my().getManager(); }

  /// This (and only this) class may automatically convert *this to raw pointer
  operator node_ptr() const { return my().getNode(); }
}; // CCuddDD


END_NAMESPACE_PBORI

#endif


