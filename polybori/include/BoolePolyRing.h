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
 * Revision 1.30  2006/12/19 12:36:46  dreyer
 * ADD: BoolePolyRing::clear{Ring}Cache()
 *
 * Revision 1.29  2006/11/27 16:25:13  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.28  2006/11/20 15:07:47  dreyer
 * FIX: headed included
 *
 * Revision 1.27  2006/10/24 06:39:38  dreyer
 * CHANGE: changed interface type
 *
 * Revision 1.26  2006/10/23 16:05:54  dreyer
 * ADD: BoolePolyRing::set/get(Ring)VariableName()
 *
 * Revision 1.25  2006/10/04 12:22:32  dreyer
 * ADD: getOrderCode()
 *
 * Revision 1.24  2006/10/04 11:48:28  dreyer
 * ADD: isDegreeReverseLexicograpical()
 *
 * Revision 1.23  2006/10/02 09:28:37  dreyer
 * ADD BoolePolyRing::changeOrdering and infrastructure
 *
 * Revision 1.22  2006/09/05 11:10:44  dreyer
 * ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
 *
 * Revision 1.21  2006/09/05 08:48:32  dreyer
 * ADD: BoolePolyRing::is(Total)DegreeOrder()
 *
 * Revision 1.20  2006/08/24 14:47:49  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.19  2006/07/20 08:55:49  dreyer
 * ADD isOrdered() and  isLexicographical()
 *
 * Revision 1.18  2006/05/23 15:55:24  dreyer
 * CHANGE order of arguments in constructor
 *
 * Revision 1.17  2006/05/23 15:26:25  dreyer
 * CHANGE BoolePolyRing  can handle different orderings (only lex yet)
 *
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
#include "OrderedManager.h"

#ifndef BoolePolyRing_h_
#define BoolePolyRing_h_

BEGIN_NAMESPACE_PBORI

class BooleExponent;
class BooleMonomial;

/** @class BoolePolyRing
 * @brief This class is just a wrapper for using @c cudd's decicion diagram
 * manager as Boolean polynomial rings.
 *
 * The purpose of this wrapper is just to provide a fixed interface to the ring
 * structure, even if switched to a different type of binary decision diagram.
 *
 **/
class BoolePolyRing: 
  public CTypes::orderenums_type, public CTypes::compenums_type, 
  public CTypes::auxtypes_type {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// generic access to current type
  typedef BoolePolyRing self;

  /// generic access to base type
  typedef CTypes::orderenums_type base;

  /// @name adopt global type definitions
  //@{
  typedef CTypes::ordercode_type ordercode_type;
  typedef CTypes::manager_type manager_type;
  typedef CTypes::manager_reference manager_reference;
  typedef CTypes::manager_ptr manager_ptr;
  typedef CTypes::dd_type dd_type;
  typedef CTypes::vartext_type vartext_type;
  //@}

  /// define exponent type
  typedef BooleExponent exp_type;

  /// set monomial type
  typedef BooleMonomial monom_type;

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------

  /// Explicitely mention ordercodes' enumeration
  using base::ordercodes;

  /// Constructor for @em nvars variables
  BoolePolyRing(size_type nvars=100, 
                ordercode_type order = lp,
                bool_type make_active = true);

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

  /// Constant access to decision diagram manager
  static manager_type& activeManager() { return *current_mgr; };

  /// Access nvar-th variable of decision diagram manager
  dd_type ddVariable(idx_type nvar) const;

  /// Access nvar-th variable of the active ring
  static dd_type ringDdVariable(idx_type nvar);

  /// Access nvar-th ring variable
  dd_type variable(idx_type nvar) const;

  /// Access nvar-th variable of the active ring
  static dd_type ringVariable(idx_type nvar);

  /// Access nvar-th ring variable
  dd_type persistentVariable(idx_type nvar) const;

  /// Access nvar-th variable of the active ring
  static dd_type persistentRingVariable(idx_type nvar);

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

  /// Test whether current ring's ordering is lexicographical
  static bool_type isLexicographical();

  /// Test whether current ring's iterators respect the ordering 
  static bool_type orderedStandardIteration();

  ///  Test whether variable pertubation do not change the order
  static bool_type isSymmetric();

  /// Test whether we deal with a degree-ordering
  static bool_type isDegreeOrder();

  /// Test whether we deal with a total degree-ordering
  static bool_type isTotalDegreeOrder();

  /// Test whether ordering is deg-rev-lex ordering
  static bool_type isDegreeReverseLexicograpical();

  /// Test whether variables are in ascending order
  static bool_type ascendingVariables();

  /// Test whether variables are in descending order
  static bool_type descendingVariables();

  ///  Get numerical code for current ordering
  static ordercode_type getOrderCode();

  /// @name Comparison of monomials-like types
  //@{
  static comp_type compare(idx_type, idx_type);
  static comp_type compare(const monom_type&, const monom_type&);
  static comp_type compare(const exp_type&, const exp_type&);
  //@}

  /// Change order of current ring
  static void changeOrdering(ordercode_type);

  /// Set name of variable with index idx
  void setVariableName(idx_type idx, vartext_type varname);

  /// Get name of variable with index idx
  vartext_type getVariableName(idx_type idx);

  /// Set name of variable with index idx
  static void setRingVariableName(idx_type idx, vartext_type varname);

  /// Get name of variable with index idx
  static vartext_type getRingVariableName(idx_type idx);

  /// Clears the function cache
  void clearCache();

  ///  Clears the function cache of the current ring
  static void clearRingCache();

protected: public:
  /// Pointer to current global manager setting
  static manager_ptr current_mgr;

  /// Interprete @c m_mgr as structure of Boolean polynomial ring
  manager_ptr pMgr;
};

END_NAMESPACE_PBORI

#endif // of #ifndef BoolePolyRing_h_
