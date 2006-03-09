// -*- c++ -*-
// $Id$
//
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file the class BoolePolyRing, where carries the definition of a
 * polynomial ring over Booleans.
 *
 *  (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
**/
//*****************************************************************************
//
// Last edit by $Author$ on $Date$
// 
// $Log$
// Revision 1.1.1.1  2006/03/09 14:34:31  dreyer
// + Project started
//

// load PolyBoRi settings
# include "pbori_defs.h"


#ifndef BoolePolyRing_h_
#define BoolePolyRing_h_

BEGIN_NAMESPACE_PBORI

/** @class BoolePolyRing
 * @brief This class is just a wrapper for using @c cudd's decicion diagram
 * manager as Boolean polynomial rings.
 *
 * The purpose of this wrapper is just to provide a fixed interface to the ring
 * structure, even if switched to a different type of binary decision diagram.
 *
 **/
class BoolePolyRing {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------
  /// generic access to decision diagram manager as base type
  typedef CTypes::manager_type manager_type;

  /// generic access to current type
  typedef BoolePolyRing self;

  /// adopt global decision diagram manager type
  typedef CTypes::dd_type dd_type;

  /// adopt global size type definition 
  typedef CTypes::size_type size_type;

  /// adopt global index type definition 
  typedef CTypes::idx_type idx_type;

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------
  /// constructor for @em nvars variables
  BoolePolyRing(size_type nvars);
  
  /// destructor
  ~BoolePolyRing();

  //-------------------------------------------------------------------------
  // member operators
  //-------------------------------------------------------------------------
  /// cast to base operator
  operator manager_type&();

  //-------------------------------------------------------------------------
  // other member functions
  //-------------------------------------------------------------------------
  /// access to base
  manager_type& manager();

  /// constant access to base
  const manager_type& manager() const;

  /// access nvar-th ring variable
  dd_type variable(idx_type nvar) const;

 protected:

  /// interprete @c mgr as structure of Boolean polynomial ring
  mutable manager_type mgr;
};

END_NAMESPACE_PBORI

#endif // of #ifndef BoolePolyRing_h_
