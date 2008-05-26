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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.9  2008/05/26 12:06:39  dreyer
 * ADD: isEnd() as end of iteration check, sing iterator_facade
 *
 * Revision 1.8  2007/11/06 15:03:33  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.7  2007/10/25 14:38:00  dreyer
 * ADD: use of CCuddNavigator more secure
 *
 * Revision 1.6  2006/12/13 18:07:04  dreyer
 * ADD: poly /= exp
 *
 * Revision 1.5  2006/12/06 09:20:09  dreyer
 * CHANGE: poly * exp now recursive
 *
 * Revision 1.4  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.3  2006/04/04 15:31:06  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * Revision 1.2  2006/03/30 08:59:42  dreyer
 * FIX: CCuddFirstIter works for empty and zero polynomials now
 *
 * Revision 1.1  2006/03/29 16:26:46  dreyer
 * ADD: Class CCuddFirstIter used for BoolePolynomial::lead()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "CCuddNavigator.h"

// include boost's interator facade
#include <boost/iterator/iterator_facade.hpp>

#ifndef CCuddFirstIter_h_
#define CCuddFirstIter_h_

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
