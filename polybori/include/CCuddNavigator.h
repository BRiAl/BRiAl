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
 * Revision 1.14  2007/02/09 17:08:21  dreyer
 * ADD: assertion
 *
 * Revision 1.13  2006/12/13 18:07:04  dreyer
 * ADD: poly /= exp
 *
 * Revision 1.12  2006/11/27 16:25:13  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.11  2006/11/21 10:33:22  dreyer
 * CHANGE: added inlinable member functions
 *
 * Revision 1.10  2006/09/21 16:09:59  dreyer
 * ADD: caching mechanism for BoolePolynomial::deg()
 *
 * Revision 1.9  2006/09/19 09:46:05  dreyer
 * CHANGE: binary_compose to binary_composition
 * CHANGE: BooleMonomial::idx_map_type
 * ADD: symmetric_composition<>, navigates<>
 *
 * Revision 1.8  2006/08/18 19:47:15  dreyer
 * change finished low-level variant of minimal_elements also for lnegth()==2
 *
 * Revision 1.7  2006/08/09 12:52:31  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * Revision 1.6  2006/04/24 10:23:22  dreyer
 * ADD BoolePolynomial::begin() and end()
 * FIX type reference in CCuddNavigator
 *
 * Revision 1.5  2006/04/21 13:13:29  dreyer
 * ADD PBoRiOutITer for more generic manipulations
 *
 * Revision 1.4  2006/04/13 08:41:34  dreyer
 * CHANGE change() used by BoolePolynomial backward (for efficiency)
 *
 * Revision 1.3  2006/04/06 14:10:58  dreyer
 * ADD hash_type and CCuddNavigator::hash()
 *
 * Revision 1.2  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.1  2006/04/04 15:31:06  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * @endverbatim
**/
//*****************************************************************************

#include <iterator>

// include basic definitions
#include "pbori_defs.h"
#include "pbori_tags.h"

#ifndef CCuddNavigator_h_
#define CCuddNavigator_h_

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

  /// Cudd's node pointer
  typedef CTypes::dd_base dd_base;

  /// Type for accessing node pointer
  typedef const pointer_type access_type;

  /// Type for indices
  typedef CTypes::idx_type idx_type;

  /// Type for indices
  typedef CTypes::size_type size_type;

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

  /// Default constructor and construct from node pointer
  CCuddNavigator(pointer_type ptr = NULL): pNode(ptr) {}

  /// Construct from decision diagram
  explicit CCuddNavigator(const dd_base& rhs): pNode(rhs.getNode()) {}

  /// Copy Constructor
  CCuddNavigator(const self& rhs): pNode(rhs.pNode) {}

  /// Destructor
  ~CCuddNavigator() {}

  /// Increment in @i then direction
  self& incrementThen();

  /// Increment in @i then direction
  self thenBranch() const;

  /// Increment in @i else direction
  self& incrementElse();

  /// Increment in @i else direction
  self elseBranch() const;

  /// Constant dereference operator
  reference operator*() const;

  /// Constant pointer access operator
  access_type operator->() const;

  /// Constant pointer access operator
  operator access_type() const { 
    return operator->(); 
  }

  /// Constant pointer access operator
  hash_type hash() const;

  /// Equality test
  bool_type operator==(const self&) const;

  /// Nonequality test
  bool_type operator!=(const self&) const;

  /// Check whether constant node was reached
  bool_type isConstant() const;

  /// Check whether terminal node marks end of path
  bool_type terminalValue() const;

  /// Check whether *this is not the default iterator self() (NULL pointer)
  bool_type isValid() const;

  /// Check whether end of path was reached
  bool_type isTerminated() const { return isConstant() && terminalValue(); }

  /// Check whether dead end was reached
  bool_type isEmpty() const { return isConstant() && !terminalValue(); }

private:
  pointer_type pNode;
};

// Inlined member functions

// constant pointer access operator
inline CCuddNavigator::value_type
CCuddNavigator::operator*() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator*() const" );
  assert(isValid());
  return Cudd_Regular(pNode)->index;
}

// constant pointer access operator
inline const CCuddNavigator::pointer_type
CCuddNavigator::operator->() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator->() const" );
  return pNode;
}

// constant pointer access operator
inline CCuddNavigator::hash_type
CCuddNavigator::hash() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::hash() const" );
  return reinterpret_cast<long>(pNode);
}

// equality test
inline CCuddNavigator::bool_type 
CCuddNavigator::operator==(const self& rhs) const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator==(const self&) const" );

  return (pNode == rhs.pNode);
}

// nonequality test
inline CCuddNavigator::bool_type 
CCuddNavigator::operator!=(const self& rhs) const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator!=(const self&) const" );
  return (pNode != rhs.pNode);
}

// whether constant node was reached
inline CCuddNavigator::bool_type 
CCuddNavigator::isConstant() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::isConstant() const" );
  return isValid() && Cudd_IsConstant(pNode);
}

// constant node value
inline CCuddNavigator::bool_type 
CCuddNavigator::terminalValue() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::terminalValue() const" );
  return Cudd_V(pNode);
}

// whether iterator is at end
inline CCuddNavigator::bool_type 
CCuddNavigator::isValid() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::isValid()() const" );

  // convention: all non-Null pointers are valid 
  return (pNode != NULL);
}

// increment in then direction
inline CCuddNavigator&
CCuddNavigator::incrementThen() {

  PBORI_TRACE_FUNC( "CCuddNavigator::incrementThen()" );

  assert(isValid());
  pNode = Cudd_T(pNode);

  return *this;
}

// go to then direction
inline CCuddNavigator
CCuddNavigator::thenBranch() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::thenBranch() const" );

  self copy(*this);
  return copy.incrementThen();
}

// increment in else direction
inline CCuddNavigator&
CCuddNavigator::incrementElse() {

  PBORI_TRACE_FUNC( "CCuddNavigator::incrementElse()" );

  assert(isValid());
  pNode = Cudd_E(pNode);

  return *this;
}

// go to else direction
inline CCuddNavigator
CCuddNavigator::elseBranch() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::ielseBranch() const" );

  self copy(*this);
  return copy.incrementElse();
}


END_NAMESPACE_PBORI

#endif
