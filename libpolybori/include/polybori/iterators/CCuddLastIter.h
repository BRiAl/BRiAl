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
**/
//*****************************************************************************

#ifndef polybori_iterators_CCuddLastIter_h_
#define polybori_iterators_CCuddLastIter_h_

// include basic definitions
#include "CCuddNavigator.h"

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
