// -*- c++ -*-
//*****************************************************************************
/** @file CCuddGetNode.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-08
 *
 * This file generates a new decision diagram node wrt. a given indes and two
 * nodes.
 *
 * @note: For Cudd's ZDDs only.
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
 * Revision 1.2  2007/11/06 15:03:33  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.1  2006/08/09 12:52:31  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * @endverbatim
**/
//*****************************************************************************

#include <iterator>

// include basic definitions
#include "pbori_defs.h"



// get navigator type for cudd zdds
#include "CCuddNavigator.h"
#define CCuddGetNode_h_
#ifndef CCuddGetNode_h_
#define CCuddGetNode_h_

BEGIN_NAMESPACE_PBORI

template <class ManagerType>
class CDDManager;

/** @class CCuddGetNode 
 * @brief This class defines a functional, which generates a new decision *
 * diagram node wrt. a given indes and two nodes.
 *
 **/

class CCuddGetNode {

public:
  /// Cudd's node pointer
  typedef CCuddNavigator node_type;

  /// Cudd's node pointer
  typedef Cudd manager_type;

  /// Type for indices
  typedef CTypes::idx_type idx_type;

  /// Get type of *this
  typedef CCuddGetNode self;

  /// Default constructor and construct from node pointer
  CCuddGetNode(const manager_type&);

  /// Copy Constructor
  CCuddGetNode(const self&);

  /// Destructor
  ~CCuddGetNode();

  /// The get node operation
  node_type operator()(idx_type idx, 
                       const node_type& thennode, 
                       const node_type& elsenode) const;

  /// Variant, which reuses oldnode, if possible
  node_type operator()(idx_type idx, const node_type& oldnode, 
                       const node_type& thennode, 
                       const node_type& elsenode) const;

  /// Increment reference count only
  node_type operator()(const node_type& oldnode) const;

protected:
  const manager_type& mgr;
};

END_NAMESPACE_PBORI

#endif
