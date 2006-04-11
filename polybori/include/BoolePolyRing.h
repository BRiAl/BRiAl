/// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file the class BoolePolyRing, where carries the definition of a
 * polynomial ring over Booleans.
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
 * Revision 1.16  2006/04/11 09:26:57  dreyer
 * ADD  BoolePolyRing::printInfo();
 *
 * Revision 1.15  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.14  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.13  2006/03/24 15:02:44  dreyer
 * ADD: Reference to manager_type can also be used for CDDManager<> -nterface
 * ADD: lead(), (n)usedVariables(), lmDeg() implemented in BoolePolynomial
 *
 * Revision 1.12  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.11  2006/03/22 16:48:13  dreyer
 * ADD alternative to shared_ptr (if not available)
 *
 * Revision 1.10  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; 
 * ring uses shared_ptr now
 *
 * Revision 1.9  2006/03/20 14:51:00  dreyer
 * CHANGE: Use CDDInterface temple specializations instead of raw dd_type
 *
 * Revision 1.8  2006/03/20 09:52:57  dreyer
 * CHANGE: BooleVariable uses composition; variable generated in BoolePolyRing
 *
 * Revision 1.7  2006/03/17 16:53:37  dreyer
 * ADD added nNodes(), operator*= to BoolePolynomial
 *
 * Revision 1.6  2006/03/16 17:09:12  dreyer
 * ADD BoolePolynial functionality started
 *
 * Revision 1.5  2006/03/13 12:27:24  dreyer
 * CHANGE: consistent function names
 *
 * Revision 1.4  2006/03/11 08:41:07  bricken
 * *bricken: standard/copy constructor
 *
 * Revision 1.3  2006/03/10 15:13:06  dreyer
 * ADD: Added static access to current ring
 *
 * Revision 1.2  2006/03/10 08:25:54  dreyer
 * + refined header style
 *
 * Revision 1.1.1.1  2006/03/09 14:34:31  dreyer
 * + Project started
 *
 * @endverbatim
**/
//*****************************************************************************

// load PolyBoRi settings
# include "pbori_defs.h"

// include basic decision diagram manager interface 
#include "CDDManager.h"

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

  /// generic access to current type
  typedef BoolePolyRing self;

  /// @name adopt global type definitions
  //@{
  typedef CTypes::manager_type manager_type;
  typedef CTypes::manager_reference manager_reference;
  typedef CTypes::manager_ptr manager_ptr;
  typedef CTypes::bool_type bool_type;
  typedef CTypes::dd_type dd_type;
  typedef CTypes::size_type size_type;
  typedef CTypes::idx_type idx_type;
  //@}

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------

  /// Constructor for @em nvars variables
  BoolePolyRing(size_type nvars=100, bool_type make_active = true);

  /// Construct from manager
  BoolePolyRing(const manager_type &);

  /// Construct from pointer to manager
  BoolePolyRing(manager_ptr pManager);

  /// Copy constructor
  BoolePolyRing(const BoolePolyRing &);

  /// destructor
  ~BoolePolyRing();

  //-------------------------------------------------------------------------
  // member operators
  //-------------------------------------------------------------------------
  /// Cast to base operator
  // operator manager_type&();

  //-------------------------------------------------------------------------
  // other member functions
  //-------------------------------------------------------------------------
  /// Access to decision diagram manager
   manager_type& manager();

  /// Constant access to decision diagram manager
   const manager_type& manager() const;

  /// Access nvar-th variable of decision diagram manager
  dd_type ddVariable(idx_type nvar) const;

  /// Access nvar-th variable of the active ring
  static dd_type ringDdVariable(idx_type nvar);

  /// Access nvar-th ring variable
  dd_type variable(idx_type nvar) const;

  /// Access nvar-th variable of the active ring
  static dd_type ringVariable(idx_type nvar);

  /// Get empty decision diagram 
  dd_type zero() const;

  /// Get empty decision diagram of the active ring
  static dd_type ringZero();

  /// Get decision diagram with all variables negated
  dd_type one() const;

  /// Get decision diagram with all variables negated of the active ring
  static dd_type ringOne();

  /// Get number of ring variables
  size_type nVariables() const;

  /// Get number of ring variables the of active ring
  static size_type nRingVariables();

  /// Access current global ring setting
  static self ring();

  /// Make this global ring
  void activate();

  /// Print out statistics and settings for current ring
  static void printInfo();
protected:
  /// Pointer to current global manager setting
  static manager_ptr current_mgr;

  /// Interprete @c m_mgr as structure of Boolean polynomial ring
  manager_ptr pMgr;
};

END_NAMESPACE_PBORI

#endif // of #ifndef BoolePolyRing_h_
