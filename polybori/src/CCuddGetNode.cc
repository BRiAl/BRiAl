// -*- c++ -*-
//*****************************************************************************
/** @file CCuddGetNode.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file generates a new decision diagram node wrt. a given indes and two
 * nodes.
 *
 * @note: For Cudd's ZDDs only.
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
 * Revision 1.1  2006/08/09 12:52:32  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "CCuddGetNode.h"

// get cudd's internal commands
#include "cuddInt.h"

BEGIN_NAMESPACE_PBORI

// constructor
CCuddGetNode::CCuddGetNode(const manager_type& man):
  mgr(man) {

  PBORI_TRACE_FUNC( "CCuddGetNode::CCuddGetNode(const manager_type&)" );
}

// copy constructor
CCuddGetNode::CCuddGetNode(const self& rhs):
  mgr(rhs.mgr) {

  PBORI_TRACE_FUNC( "CCuddGetNode::CCuddGetNode(const self&)" );
}


// destructor
CCuddGetNode::~CCuddGetNode() {

  PBORI_TRACE_FUNC( "CCuddGetNode::~CCuddGetNode()" );
  // does nothing, because everything is managed outside the class
}

//  Variant, which reuses oldnode, if possible
CCuddGetNode::node_type
CCuddGetNode::operator()(idx_type idx, const node_type& oldnode,
                         const node_type& thennode, 
                         const node_type& elsenode) const {

  PBORI_TRACE_FUNC( "CCuddGetNode::operator()(idx_type, const node_type&, "
                    "const node_type&, const node_type&) const" );

  // assuming *oldnode == idx
  if( (thennode == oldnode.thenBranch()) && 
      (elsenode == oldnode.elseBranch()) ) {

    // Reuse oldnode again -> increment reference count
    Cudd_Ref(oldnode); 

    // elsenode and thennode are not used any more -> decrease reference count 
    Cudd_Deref(elsenode); 
    Cudd_Deref(thennode);
    return oldnode;
  }

  return (*this)(idx, thennode, elsenode);
}

// The get node operation
CCuddGetNode::node_type
CCuddGetNode::operator()(idx_type idx, 
                         const node_type& thennode, 
                         const node_type& elsenode) const {

  PBORI_TRACE_FUNC( "CCuddGetNode::operator()("
                    "idx_type, const node_type&, const node_type&) const" );

  // Note: assuming thennode.isValid() and elsenode.isValid(), and reference
  // count was increased for application of this operator
  node_type result =  cuddZddGetNode( mgr.getManager(), 
                                      idx, thennode, elsenode );

  if(!result.isValid()){
    Cudd_RecursiveDerefZdd(mgr.getManager(), thennode);
    Cudd_RecursiveDerefZdd(mgr.getManager(), elsenode);
    return result;
  }

  // Mark as new node
  Cudd_Ref(result); 

  // elsenode and thennode are not used any more -> decrease reference count
  Cudd_Deref(elsenode); 
  Cudd_Deref(thennode); 

  return result;
}

// constant pointer access operator
CCuddGetNode::node_type
CCuddGetNode::operator()(const node_type& oldnode) const {

  PBORI_TRACE_FUNC( "CCuddGetNode::operator()(const node_type&) const" );

  // Mark as new node
  Cudd_Ref(oldnode); 

  return oldnode;
}


END_NAMESPACE_PBORI
