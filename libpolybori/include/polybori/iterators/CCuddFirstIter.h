// -*- c++ -*-
//*****************************************************************************
/** @file CCuddFirstIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file defines an stl-like iterator for accessing the nonzero indices of
 * the leading (first) minimal term of Cudd decision diagrams.
 *
 * @note: Tested for Cudd's ZDDs only.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_iterators_CCuddFirstIter_h_
#define polybori_iterators_CCuddFirstIter_h_

// include basic definitions
#include "CCuddNavigator.h"

// include boost's interator facade
#include <boost/iterator/iterator_facade.hpp>

BEGIN_NAMESPACE_PBORI

/** @class CCuddFirstIter
 * @brief This class defines an iterator over the first minimal term of a given
 * ZDD node.
 *
 **/

class CCuddFirstIter : 
  public boost::iterator_facade<
  CCuddFirstIter,
  CCuddNavigator::value_type,
  std::forward_iterator_tag,
  CCuddNavigator::reference
  > {

public:

  /// Interface type for standard iterator access
  typedef std::forward_iterator_tag iterator_category;

  /// Get type of *this
  typedef CCuddFirstIter self;

  /// Get navigator type 
  typedef CCuddNavigator navigator;

  /// Type for constantly accessing node pointer
  typedef navigator::const_access_type const_access_type;

  /// Type for boolean values
  typedef navigator::bool_type bool_type;

  /// Default constructor
  CCuddFirstIter(): m_navi() {}

  /// Construct from navigator over nodes
  CCuddFirstIter(navigator rhs): m_navi(rhs) { terminateConstant(); }

  /// Destructor
  ~CCuddFirstIter() {}

  /// Incrementation operation
  void increment() {
    m_navi.incrementThen();
    terminateConstant();
  }

  /// Equality test
  bool_type equal (const self& rhs) const { return (m_navi == rhs.m_navi); }

  /// Dereferencing of the iterator
  navigator::reference dereference() const { return *m_navi; }

  /// Check, whether end of iteration is reached
  bool_type isEnd() const { return !m_navi.isValid(); }

protected:
  /// Constant nodes are marked as the end of a path
  void terminateConstant() {
    if (m_navi.isConstant()) 
      *this = self();           // mark end of path reached

  }

  /// Use navigator to access diagram nodes
  navigator m_navi;
};

END_NAMESPACE_PBORI

#endif
