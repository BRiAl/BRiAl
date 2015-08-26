// -*- c++ -*-
//*****************************************************************************
/** @file CCuddNavigator.h
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file defines an iterator for navigating through then and else branches
 * of Cudd decision diagrams.
 *
 * @note: Tested for Cudd's ZDDs only.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_iterators_CCuddNavigator_h_
#define polybori_iterators_CCuddNavigator_h_

#include <iterator>

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/common/tags.h>
#include <polybori/ring/CCuddInterface.h>

BEGIN_NAMESPACE_PBORI

/** @class CCuddNavigator
 * @brief This class defines an iterator for navigating through then and else branches
 * of ZDDs.
 *
 **/

class CCuddNavigator {

public:
  /// Cudd's node pointer
  typedef DdNode* pointer_type;

  /// Type for constantly accessing node pointer
  typedef const pointer_type const_access_type;

  /// Type for indices
  typedef CTypes::idx_type idx_type;

  /// Type for indices
  typedef CTypes::size_type size_type;

  /// Type for degrees
  typedef CTypes::deg_type deg_type;

  /// Type for hashing
  typedef CTypes::hash_type hash_type;

  /// Type for boolean results
  typedef CTypes::bool_type bool_type;

  /// Return type of dereferencing operator
  typedef idx_type value_type;

  /// Get type of *this
  typedef CCuddNavigator self;

  /// @name Interface types for standard iterator access
  //@{
  typedef navigator_tag iterator_category;
  typedef std::iterator_traits<pointer_type>::difference_type difference_type;
  typedef void pointer;
  typedef value_type reference;
  //@}

  /// Default constructor
  CCuddNavigator(): pNode(NULL) {}

  /// Construct from node pointer
  explicit CCuddNavigator(pointer_type ptr): pNode(ptr) {
    PBORI_ASSERT(isValid());
  }

  /// Copy Constructor
  CCuddNavigator(const self& rhs): pNode(rhs.pNode) {}

  /// Destructor
  ~CCuddNavigator() {}

  /// Increment in @c then direction
  self& incrementThen();        // inlined below

  /// Increment in @c then direction
  self thenBranch() const { return self(*this).incrementThen(); }

  /// Increment in @c else direction
  self& incrementElse();        // inlined below

  /// Increment in @c else direction
  self elseBranch() const { return self(*this).incrementElse(); }

  /// Constant dereference operator
  reference operator*() const;  // inlined below

  /// Constant pointer access operator
  const_access_type operator->() const { return pNode; }

  /// Constant pointer access operator
  const_access_type getNode() const { return pNode; }

  /// Constant pointer access operator
  hash_type hash() const { return reinterpret_cast<hash_type>(pNode); }

  /// Equality test
  bool_type operator==(const self& rhs) const { return (pNode == rhs.pNode); }

  /// Nonequality test
  bool_type operator!=(const self& rhs) const { return (pNode != rhs.pNode); }

  /// Check whether constant node was reached
  bool_type isConstant() const; // inlined below

  /// Check whether terminal node marks end of path
  bool_type terminalValue() const; // inlined below

  /// Check whether *this is not the default iterator self() (NULL pointer)
  bool_type isValid() const { return (pNode != NULL); }

  /// Check whether end of path was reached
  bool_type isTerminated() const { return isConstant() && terminalValue(); }

  /// Check whether dead end was reached
  bool_type isEmpty() const { return isConstant() && !terminalValue(); }

  /// @name Pointer-like comparision operations
  //@{
  bool_type operator<(const self& rhs) const { return (pNode < rhs.pNode); }
  bool_type operator<=(const self& rhs) const { return (pNode <= rhs.pNode); }
  bool_type operator>(const self& rhs) const { return (pNode > rhs.pNode); }
  bool_type operator>=(const self& rhs) const { return (pNode >= rhs.pNode); }
  //@}

  /// Force incrementation of reference count
  void incRef() const {  PBORI_ASSERT(isValid()); PBORI_PREFIX(Cudd_Ref)(pNode); }

  /// Force decrementation of reference count
  void decRef() const {  PBORI_ASSERT(isValid()); PBORI_PREFIX(Cudd_Deref)(pNode); }

  /// Force recursive decrementation of reference count
  template <class MgrType>
  void recursiveDecRef(const MgrType& mgr) const {
    PBORI_ASSERT(isValid());
    PBORI_PREFIX(Cudd_RecursiveDerefZdd)(mgr, pNode); 
  }

private: 
  /// Store node pointer
  pointer_type pNode;
};

// Inlined member functions

// constant pointer access operator
inline CCuddNavigator::value_type
CCuddNavigator::operator*() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator*() const" );
  PBORI_ASSERT(isValid());
  return Cudd_Regular(pNode)->index;
}

// whether constant node was reached
inline CCuddNavigator::bool_type 
CCuddNavigator::isConstant() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::isConstant() const" );
  PBORI_ASSERT(isValid());
  return Cudd_IsConstant(pNode);
}

// constant node value
inline CCuddNavigator::bool_type 
CCuddNavigator::terminalValue() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::terminalValue() const" );
  PBORI_ASSERT(isConstant());
  return Cudd_V(pNode);
}


// increment in then direction
inline CCuddNavigator&
CCuddNavigator::incrementThen() {

  PBORI_TRACE_FUNC( "CCuddNavigator::incrementThen()" );

  PBORI_ASSERT(isValid());
  pNode = Cudd_T(pNode);

  return *this;
}

// increment in else direction
inline CCuddNavigator&
CCuddNavigator::incrementElse() {

  PBORI_TRACE_FUNC( "CCuddNavigator::incrementElse()" );

  PBORI_ASSERT(isValid());
  pNode = Cudd_E(pNode);

  return *this;
}

inline CCuddNavigator
explicit_navigator_cast(CCuddNavigator::pointer_type ptr) {

#ifndef PBORI_NDEBUG
  if (ptr == NULL)
    return CCuddNavigator();
  else
#endif
    return CCuddNavigator(ptr);
}


END_NAMESPACE_PBORI

#endif
