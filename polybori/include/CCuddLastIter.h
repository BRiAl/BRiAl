// -*- c++ -*-
//*****************************************************************************
/** @file CCuddLastIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file defines an stl-like iterator for accessing the nonzero indices of
 * the leading (last) minimal term of Cudd decision diagrams.
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
 * Revision 1.4  2007/11/06 16:36:07  dreyer
 * Fix: used correct constructor
 *
 * Revision 1.3  2007/11/06 15:03:33  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.2  2006/12/13 18:07:04  dreyer
 * ADD: poly /= exp
 *
 * Revision 1.1  2006/06/07 08:37:50  dreyer
 * ADD CCuddLastIter and BooleSet::lastLexicographicalTerm()
 *
 * Revision 1.4  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.3  2006/04/04 15:31:06  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * Revision 1.2  2006/03/30 08:59:42  dreyer
 * FIX: CCuddLastIter works for empty and zero polynomials now
 *
 * Revision 1.1  2006/03/29 16:26:46  dreyer
 * ADD: Class CCuddLastIter used for BoolePolynomial::lead()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "CCuddNavigator.h"

#ifndef CCuddLastIter_h_
#define CCuddLastIter_h_

BEGIN_NAMESPACE_PBORI

/** @class CCuddLastIter
 * @brief This class defines an iterator over the last minimal term of a given
 * ZDD node.
 *
 **/

class CCuddLastIter :
  public CCuddNavigator {

public:
  /// Interface type for standard iterator access
  typedef std::forward_iterator_tag iterator_category;

  /// Get type of *this
  typedef CCuddLastIter self;

  /// Get base type 
  typedef CCuddNavigator base;

  /// Default constructor and construct from node pointer
  CCuddLastIter(): base() {}
  CCuddLastIter(pointer_type ptr);

  /// Copy Constructor
  CCuddLastIter(const self&);

  /// Destructor
  ~CCuddLastIter();

  /// Prefix increment operator
  self& operator++();

  /// Postfix increment operator
  self operator++(int);

protected:
  /// Constant nodes are marked as the end of a path
  void terminateConstant();
};


END_NAMESPACE_PBORI

#endif
