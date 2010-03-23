// -*- c++ -*-
//*****************************************************************************
/** @file BoolePolynomial.h
 *
 * @author Alexander Dreyer
 * @date 2006-03-10
 *
 * This file carries the definition of class @c BoolePolynomial, which can be
 * used to access the boolean polynomials with respect to the polynomial ring,
 * which was active on initialization time.
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
 * Revision 1.98  2009/03/31 08:30:49  dreyer
 * CHANGE: started consistent use of lead* instea of lm*
 *
 * Revision 1.97  2008/09/21 22:21:02  dreyer
 * Change: deg_type replaces size_type for deg(), etc.
 *
 * Revision 1.96  2008/09/17 08:17:17  bricken
 * != bug
 *
 * Revision 1.95  2008/07/18 22:37:50  dreyer
 * Fix: doxygen clean-up (removed inclusion loop)
 *
 * Revision 1.94  2008/03/20 08:54:25  bricken
 * + fixed evil bug in comparison with constants
 *
 * Revision 1.93  2008/03/10 16:48:07  dreyer
 * Fix: exception for division by 0 and invalid monomial-zero
 *
 * Revision 1.92  2008/03/05 16:23:37  dreyer
 * CHANGE: BooleMonomial::variableBegin()|End(); monom/monom = 0 throws
 *
 * Revision 1.91  2008/03/03 12:44:31  dreyer
 * Change: More inlining, and safer constructors
 *
 * Revision 1.90  2008/03/02 23:45:33  dreyer
 * CHANGED: added contructors for given ring
 *
 * Revision 1.89  2008/03/02 23:24:37  dreyer
 * CHANGE: ring elements like polynomials, monomials, and variables have ring()
 *
 * Revision 1.88  2008/03/01 01:11:25  dreyer
 * Fix: working around bug in addition
 *
 * Revision 1.87  2008/02/28 17:05:47  dreyer
 * Fix: treating constants (0, 1) accordingly
 *
 * Revision 1.86  2008/02/27 16:35:12  dreyer
 * Fix: Polynomial(0|1) removed, where possible
 *
 * Revision 1.85  2008/01/29 16:05:02  dreyer
 * Fix: cast from navigator to BooleSet or BoolePolynomial made explicit
 *
 * Revision 1.84  2008/01/11 16:58:56  dreyer
 * CHANGE: Experimenting with iterators and correct rings
 *
 * Revision 1.83  2007/12/14 11:50:30  dreyer
 * Fix: merged from bugfix at sf.net
 *
 * Revision 1.82  2007/12/13 15:53:48  dreyer
 * CHANGE: Ordering in BoolePolyRing again; BooleEnv manages active ring
 *
 * Revision 1.81  2007/11/30 09:33:19  dreyer
 * CHANGE: more dd-like stableHash()
 *
 * Revision 1.80  2007/11/29 16:28:31  dreyer
 * ADD: fast hash(), where applicable; + stableHashes() anywhere
 *
 * Revision 1.79  2007/11/15 13:08:00  dreyer
 * CHANGE: removed dd_type from PyPolyBoRi => .diagram()->.set()
 *
 * Revision 1.78  2007/11/06 15:03:33  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.77  2007/10/22 15:16:55  dreyer
 * ADD: lexLmDeg(), lexLead()
 *
 * Revision 1.76  2007/10/09 15:07:27  dreyer
 * ADD: mapping;   CHANGE: shared python modules to pyroot
 *
 * Revision 1.75  2007/10/09 10:30:51  dreyer
 * ADD: poly.gradedPart(deg); FIX: term_accumulate (constant term)
 *
 * Revision 1.74  2007/07/31 07:43:49  dreyer
 * ADD: getBaseOrderCode(), lieInSameBlock(...), isSingleton(), isPair()...
 *
 * Revision 1.73  2007/07/30 15:19:38  dreyer
 * CHANGE: CCuddNavigator does not convert to DdNode* impicitely any more
 *
 * Revision 1.72  2007/07/18 07:17:26  dreyer
 * CHANGE: some clean-ups
 *
 * Revision 1.71  2007/05/25 11:37:59  dreyer
 * ADD: cast int -> BoolePolynomial, which obeyes %2
 *
 * Revision 1.70  2007/05/14 08:10:59  dreyer
 * ADD: added poly / poly and poly % poly
 *
 * Revision 1.69  2007/05/11 11:38:42  dreyer
 * ADD: started pbori_algorithms.h and term_accumulation()
 *
 * Revision 1.68  2007/05/03 16:04:45  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * Revision 1.67  2007/04/30 15:20:30  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.66  2007/04/27 21:20:04  dreyer
 * CHANGE: testing exponent iterator
 *
 * Revision 1.65  2007/04/20 12:30:18  dreyer
 * FIX: cast navigator -> BoolePolynomial now corrent
 *
 * Revision 1.64  2007/04/18 15:37:28  dreyer
 * ADD: dp_asc now active
 *
 * Revision 1.63  2007/04/13 13:55:52  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.62  2007/03/21 09:33:07  dreyer
 * ADD: generic{Exp|}{Begin|End}(block_dlex_tag)
 *
 * Revision 1.61  2007/03/21 08:55:08  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.60  2007/03/19 16:49:38  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.59  2007/03/16 16:59:20  dreyer
 * CHANGE: started to rewrite CGenericIter using boost:iterator_facade
 *
 * Revision 1.58  2006/12/13 18:07:03  dreyer
 * ADD: poly /= exp
 *
 * Revision 1.57  2006/11/21 16:06:40  dreyer
 * CHANGE: inlining added
 *
 * Revision 1.56  2006/11/21 12:33:33  dreyer
 * ADD: BooleSet::ownsOne, BooleSet(idx, navi, navi); divisorsof
 *
 * Revision 1.55  2006/11/21 09:52:05  dreyer
 * CHANGE: some simple functions in BoolePolynomial inlined
 * ADD: caching of ternary operations
 * ADD: commandline switch PBORI_FAST_MULTIPLICATION (dense multiplication)
 *
 * Revision 1.54  2006/10/16 15:13:10  bricken
 * + new map type
 *
 * Revision 1.53  2006/10/16 10:54:29  dreyer
 * ADD: idx_map_type to BoolePolynomial
 *
 * Revision 1.52  2006/10/06 12:52:00  dreyer
 * ADD easy_equility_property and used in lex_compare
 *
 * Revision 1.51  2006/10/05 09:03:34  bricken
 * + uncommented duplicate typedef
 *
 * Revision 1.50  2006/10/05 07:33:58  dreyer
 * ADD: BoolePolynomial::genericExpBegin()/End()
 *
 * Revision 1.49  2006/10/04 15:46:49  dreyer
 * ADD: divisorsOf(exp_type), orderedExpBegin/End;
 * CHANGE: Polynomials printing respects ordering
 *
 * Revision 1.48  2006/10/04 13:09:56  dreyer
 * ADD: added compile-time optimied iterators and genericBegin/genericEnd
 *
 * Revision 1.47  2006/09/20 07:06:39  dreyer
 * ADD BoolePolynomial/CDDInterface::isConstant(), used it in deg()
 *
 * Revision 1.46  2006/09/19 09:46:05  dreyer
 * CHANGE: binary_compose to binary_composition
 * CHANGE: BooleMonomial::idx_map_type
 * ADD: symmetric_composition<>, navigates<>
 *
 * Revision 1.45  2006/09/14 10:57:25  dreyer
 * ADD: usedVariablesExp()
 *
 * Revision 1.44  2006/09/13 15:07:04  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.43  2006/09/13 09:05:43  dreyer
 * ADD: dp_asc/DegRevLexAscOrder
 * ADD: BoolePolynomial::endOfNavigation()
 * CHANGE: BoolePolynomial: removed biDegBegin(), biDegEnd(), which can be
 *   generated more generically using navigation() and endOfNavigation().
 *
 * Revision 1.42  2006/09/12 14:56:55  dreyer
 * ADD bidirectional term iterator template
 *
 * Revision 1.41  2006/09/08 16:15:27  dreyer
 * ADD: Added ordering-dependent term iteration
 *
 * Revision 1.40  2006/09/04 11:33:57  dreyer
 * CHANGE: lm*() use leadFirst()
 *
 * Revision 1.39  2006/09/01 10:35:25  dreyer
 * ADD: Multiplication poly * poly, poly * exponent
 *
 * Revision 1.38  2006/08/29 10:37:56  dreyer
 * CHANGE: non-const version of diagram() now internalDiagram()
 *
 * Revision 1.37  2006/08/29 09:02:36  dreyer
 * ADD: leadExp()
 *
 * Revision 1.36  2006/08/28 07:25:07  dreyer
 * CHANGE: BooleExponent nomenclatur
 *
 * Revision 1.35  2006/08/24 14:47:49  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.34  2006/07/17 15:32:08  dreyer
 * ADD: BoolePolynomial::hasConstantPart, BooleMonomial::isOne, isZero
 *
 * Revision 1.33  2006/07/06 16:01:29  dreyer
 * CHANGE: Functionals ins pbori_func.h made more consistent
 *
 * Revision 1.32  2006/05/24 08:01:01  dreyer
 * ADD operator % and %=
 *
 * Revision 1.31  2006/05/24 06:38:14  bricken
 * + corrected type for usedVariables
 *
 * Revision 1.30  2006/05/23 09:40:39  dreyer
 * ADD operators - and -=
 *
 * Revision 1.29  2006/05/03 10:37:03  bricken
 * + work on pseudo parallel reductions
 *
 * Revision 1.28  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.27  2006/04/24 14:45:35  dreyer
 * FIX CTermIter; ADD BoolePolynomial uses CTermIter
 *
 * Revision 1.26  2006/04/24 10:23:22  dreyer
 * ADD BoolePolynomial::begin() and end()
 * FIX type reference in CCuddNavigator
 *
 * Revision 1.25  2006/04/20 16:59:47  dreyer
 * routine
 *
 * Revision 1.24  2006/04/20 08:31:21  dreyer
 * ADD BooleMonomial::mulples(...)
 *
 * Revision 1.23  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.22  2006/04/14 15:45:30  bricken
 * + removed semicolons
 *
 * Revision 1.21  2006/04/12 10:33:01  bricken
 * *bricken: elimination length
 *
 * Revision 1.20  2006/04/06 14:10:58  dreyer
 * ADD hash_type and CCuddNavigator::hash()
 *
 * Revision 1.19  2006/04/06 13:54:58  dreyer
 * ADD BoolePolynomial::length()
 *
 * Revision 1.18  2006/04/05 15:26:03  dreyer
 * CHANGE: File access of BoolePolynomial::prettyPrint moved to CDDInterface
 *
 * Revision 1.17  2006/04/05 14:56:38  dreyer
 * ADD prettyPrint functions for dot-output to stdout or file
 *
 * Revision 1.16  2006/04/04 15:31:06  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * Revision 1.15  2006/04/04 13:01:28  bricken
 * + copyDiagram
 *
 * Revision 1.14  2006/04/04 12:07:37  dreyer
 * ADD BoolePolynomial::reducibleby(), and firstBegin(), firstEnd()
 *
 * Revision 1.13  2006/04/04 11:59:54  bricken
 * + hash function
 *
 * Revision 1.12  2006/04/04 11:21:22  dreyer
 * ADD lmDivisors() added
 *
 * Revision 1.11  2006/04/04 07:36:35  dreyer
 * ADD: tests isZero(), isOne() and poly == bool, bool == poly
 *
 * Revision 1.10  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.9  2006/03/27 15:02:43  dreyer
 * ADD: BoolePolynomial::operator/=(const self&) and spoly
 *
 * Revision 1.8  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.7  2006/03/24 15:02:44  dreyer
 * ADD: Reference to manager_type can also be used for CDDManager<> -nterface
 * ADD: lead(), (n)usedVariables(), lmDeg() implemented in BoolePolynomial
 *
 * Revision 1.6  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.5  2006/03/20 14:51:00  dreyer
 * CHANGE: Use CDDInterface temple specializations instead of raw dd_type
 *
 * Revision 1.4  2006/03/20 12:11:57  dreyer
 * CHANGE: Revised *deg*() functions.
 *
 * Revision 1.3  2006/03/16 17:09:12  dreyer
 * ADD BoolePolynial functionality started
 *
 * Revision 1.2  2006/03/13 12:27:24  dreyer
 * CHANGE: consistent function names
 *
 * Revision 1.1  2006/03/10 15:14:24  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef BoolePolynomial_h_
#define BoolePolynomial_h_

// include standard definitions
#include <vector>

// get standard map functionality
#include <map>

// get standard algorithmic functionalites
#include <algorithm>

#include "BooleRing.h"
// include basic definitions and decision diagram interface
#include "CDDInterface.h"

// include definition of sets of Boolean variables
#include "CTermIter.h"
#include "CBidirectTermIter.h"

#include "pbori_func.h"
#include "pbori_tags.h"
#include "BooleSet.h"

#include "CTermIter.h"
#include "BooleConstant.h"

BEGIN_NAMESPACE_PBORI


// forward declarations
class LexOrder;
class DegLexOrder;
class DegRevLexAscOrder;
class BlockDegLexOrder;
class BlockDegRevLexAscOrder;

class BooleMonomial;
class BooleVariable;
class BooleExponent;


template <class IteratorType, class MonomType>
class CIndirectIter;

template <class IteratorType, class MonomType>
class COrderedIter;


//template<class, class, class, class> class CGenericIter;
template<class, class, class, class> class CDelayedTermIter;

template<class OrderType, class NavigatorType, class MonomType>
class CGenericIter;

template<class NavigatorType, class ExpType>
class CExpIter;


/** @class BoolePolynomial
 * @brief This class wraps the underlying decicion diagram type and defines the
 * necessary operations.
 *
 **/
class BoolePolynomial;
BoolePolynomial 
operator+(const BoolePolynomial& lhs, const BoolePolynomial& rhs);

class BoolePolynomial {

public:

  /// Let BooleMonomial access protected and private members
  friend class BooleMonomial;

  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// Generic access to current type
  typedef BoolePolynomial self;

  /// @name Adopt global type definitions
  //@{
  typedef CTypes::manager_type manager_type;
  typedef CTypes::manager_reference manager_reference;
  typedef CTypes::manager_ptr manager_ptr;
  typedef CTypes::dd_type dd_type;
  typedef CTypes::size_type size_type;
  typedef CTypes::deg_type deg_type;
  typedef CTypes::idx_type idx_type;
  typedef CTypes::bool_type bool_type;
  typedef CTypes::ostream_type ostream_type;
  typedef CTypes::hash_type hash_type;
  //@}

  /// Iterator type for iterating over indices of the leading term
  typedef dd_type::first_iterator first_iterator;

  /// Iterator-like type for navigating through diagram structure
  typedef dd_type::navigator navigator;

  /// Type for output of pretty print
  typedef dd_type::pretty_out_type pretty_out_type;

  /// Type for naming file for pretty print
  typedef dd_type::filename_type filename_type;

  /// @todo A more sophisticated treatment for monomials is needed.

  /// Fix type for treatment of monomials
  typedef BooleMonomial monom_type; 

  /// Fix type for treatment of monomials
  typedef BooleVariable var_type; 

  /// Fix type for treatment of exponent vectors
  typedef BooleExponent exp_type; 

  /// Type for wrapping integer and bool values
  typedef BooleConstant constant_type;

  /// Type for Boolean polynomial rings (without ordering)
  typedef BooleRing ring_type;

  /// Type for result of polynomial comparisons
  typedef CTypes::comp_type comp_type;

  /// Incrementation functional type
  typedef 
  binary_composition< std::plus<size_type>, 
                      project_ith<1>, integral_constant<size_type, 1> > 
  increment_type;

  /// Decrementation functional type
  typedef 
  binary_composition< std::minus<size_type>, 
                      project_ith<1>, integral_constant<size_type, 1> > 
  decrement_type;



  /// Iterator type for iterating over all exponents in ordering order
  //  typedef COrderedIter<exp_type> ordered_exp_iterator;
  typedef COrderedIter<navigator, exp_type> ordered_exp_iterator;

  /// Iterator type for iterating over all monomials in ordering order
  //  typedef COrderedIter<monom_type> ordered_iterator;
  typedef COrderedIter<navigator, monom_type> ordered_iterator;

  /// @name Generic iterators for various orderings
  //@{
  typedef CGenericIter<LexOrder, navigator, monom_type> lex_iterator;
  ////  typedef CGenericIter<LexOrder, navigator, monom_type> lex_iterator;
  typedef CGenericIter<DegLexOrder, navigator, monom_type> dlex_iterator;
  typedef CGenericIter<DegRevLexAscOrder, navigator, monom_type> 
  dp_asc_iterator;

  typedef CGenericIter<BlockDegLexOrder,  navigator, monom_type> 
  block_dlex_iterator;
  typedef CGenericIter<BlockDegRevLexAscOrder,  navigator, monom_type> 
  block_dp_asc_iterator;

  typedef CGenericIter<LexOrder, navigator, exp_type> lex_exp_iterator;
  typedef CGenericIter<DegLexOrder,  navigator, exp_type> dlex_exp_iterator;
  typedef CGenericIter<DegRevLexAscOrder,  navigator, exp_type> 
  dp_asc_exp_iterator;
  typedef CGenericIter<BlockDegLexOrder, navigator, exp_type> 
  block_dlex_exp_iterator;
  typedef CGenericIter<BlockDegRevLexAscOrder, navigator, exp_type> 
  block_dp_asc_exp_iterator;
  //@}

  /// Iterator type for iterating over all monomials
  typedef lex_iterator const_iterator;

  /// Iterator type for iterating all exponent vectors 
  typedef CExpIter<navigator, exp_type> exp_iterator;

  /// Iterator type for iterating all monomials (dereferencing to degree)
  typedef CGenericIter<LexOrder, navigator, deg_type> deg_iterator;

  /// Type for lists of terms
  typedef std::vector<monom_type> termlist_type;

  /// The property whether the equality check is easy is inherited from dd_type
  typedef dd_type::easy_equality_property easy_equality_property;

  /// Type for sets of Boolean variables
  typedef BooleSet set_type;

  /// Type for index maps
  typedef std::map<self, idx_type, symmetric_composition<
    std::less<navigator>, navigates<self> > > idx_map_type;
  typedef std::map<self, std::vector<self>, symmetric_composition<
    std::less<navigator>, navigates<self> > > poly_vec_map_type;

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------

  /// Default constructor
  BoolePolynomial();

  /// Construct polynomial from a constant value 0 or 1
  explicit BoolePolynomial(constant_type);

  /// Construct polynomial from a constant value 0 or 1
  BoolePolynomial(constant_type isOne, const ring_type& ring):
    m_dd(isOne? ring.one(): ring.zero() )  { }

  /// Construct polynomial from decision diagram
  BoolePolynomial(const dd_type& rhs): m_dd(rhs) {}

  /// Construct polynomial from a subset of the powerset over all variables
  BoolePolynomial(const set_type& rhs): m_dd(rhs.diagram()) {}

  /// Construct polynomial from exponent vector
  BoolePolynomial(const exp_type&, const ring_type&); 

  /// Construct polynomial from navigator
  BoolePolynomial(const navigator& rhs, const ring_type& ring):
    m_dd(ring.manager().manager(), rhs)  {
    assert(rhs.isValid());
  }

  /// Destructor
  ~BoolePolynomial() {}

  //-------------------------------------------------------------------------
  // operators and member functions
  //-------------------------------------------------------------------------

  //  self& operator=(const self& rhs) { 
  //  return m_dd = rhs.m_dd;
  // }

  self& operator=(constant_type rhs) { 
    return (*this) = self(rhs);//rhs.generate(*this); 
  }
  /// @name Arithmetical operations
  //@{
  const self& operator-() const { return *this; }
  self& operator+=(const self&);
  self& operator+=(constant_type rhs) {

    //return *this = (self(*this) + (rhs).generate(*this));
    if (rhs) (*this) = (*this + ring().one());
     return *this;
  }
  template <class RHSType>
  self& operator-=(const RHSType& rhs) { return operator+=(rhs); }
  self& operator*=(const monom_type&);
  self& operator*=(const exp_type&);
  self& operator*=(const self&);
  self& operator*=(constant_type rhs) {
    if (!rhs) *this = ring().zero();
    return *this;
  }
  self& operator/=(const monom_type&);
  self& operator/=(const exp_type&);
  self& operator/=(const self& rhs);
  self& operator/=(constant_type rhs);
  self& operator%=(const monom_type&);
  self& operator%=(const self& rhs) { 
    return (*this) -= (self(rhs) *= (self(*this) /= rhs)); 
  }
  self& operator%=(constant_type rhs) { return (*this) /= (!rhs); }
  //@}

  /// @name Logical operations
  //@{
  bool_type operator==(const self& rhs) const { return (m_dd == rhs.m_dd); }
  bool_type operator!=(const self& rhs) const { return (m_dd != rhs.m_dd); }
  bool_type operator==(constant_type rhs) const { 
    return ( rhs? isOne(): isZero() );
  }
  bool_type operator!=(constant_type rhs) const {
    //return ( rhs? (!(isOne())): (!(isZero())) );
      return (!(*this==rhs));
  }
  //@}

  /// Check whether polynomial is constant zero
  bool_type isZero() const { return m_dd.emptiness(); }

  /// Check whether polynomial is constant one 
  bool_type isOne() const { return m_dd.blankness(); }

  /// Check whether polynomial is zero or one
  bool_type isConstant() const { return m_dd.isConstant(); }

  /// Check whether polynomial has term one  
  bool_type hasConstantPart() const { return m_dd.ownsOne(); }

  /// Tests whether polynomial can be reduced by right-hand side
  bool_type reducibleBy(const self&) const;

  /// Get leading term
  monom_type lead() const;

  /// Get leading term w.r.t. lexicographical order
  monom_type lexLead() const;

  /// Get leading term (using upper bound)
  monom_type boundedLead(size_type bound) const;

  /// Get leading term
  exp_type leadExp() const;

  /// Get leading term (using upper bound)
  exp_type boundedLeadExp(size_type bound) const;

  /// Get all divisors of the leading term
  set_type leadDivisors() const { return leadFirst().firstDivisors(); };
  
  /// Get unique hash value (may change from run to run)
  hash_type hash() const { return m_dd.hash(); }

  /// Get hash value, which is reproducible
  hash_type stableHash() const { return m_dd.stableHash(); } 

  /// Hash value of the leading term
  hash_type leadStableHash() const;
  
  /// Maximal degree of the polynomial
  deg_type deg() const;

  /// Degree of the leading term
  deg_type leadDeg() const;

  /// Degree of the leading term w.r.t. lexicographical ordering
  deg_type lexLeadDeg() const;

  /// Total maximal degree of the polynomial
  deg_type totalDeg() const;

  /// Total degree of the leading term
  deg_type leadTotalDeg() const;

  /// Get part of given degree
  self gradedPart(deg_type deg) const;

  /// Number of nodes in the decision diagram
  size_type nNodes() const;

  /// Number of variables of the polynomial
  size_type nUsedVariables() const;

  /// Set of variables of the polynomial
  monom_type usedVariables() const;

  /// Exponent vector of all of variables of the polynomial
  exp_type usedVariablesExp() const;

  /// Returns number of terms
  size_type length() const;

  /// Print current polynomial to output stream
  ostream_type& print(ostream_type&) const;

  /// Pretty print to stdout
  void prettyPrint() const;

  /// Pretty print to filename
  void prettyPrint(filename_type filename) const;

  /// Start of iteration over monomials
  const_iterator begin() const;

  /// Finish of iteration over monomials
  const_iterator end() const;

  /// Start of iteration over exponent vectors
  exp_iterator expBegin() const;

  /// Finish of iteration over exponent vectors
  exp_iterator expEnd() const;

  /// Start of first term
  first_iterator firstBegin() const;

  /// Finish of first term 
  first_iterator firstEnd() const;

  /// Get of first lexicographic term 
  monom_type firstTerm() const;

  /// Start of degrees
  deg_iterator degBegin() const;

  /// Finish of degrees
  deg_iterator degEnd() const;

  /// Start of ordering respecting iterator
  ordered_iterator orderedBegin() const; 

  /// Finish of ordering respecting iterator
  ordered_iterator orderedEnd() const;

   /// Start of ordering respecting exponent iterator
  ordered_exp_iterator orderedExpBegin() const; 

  /// Finish of ordering respecting exponent iterator
  ordered_exp_iterator orderedExpEnd() const;

  /// @name Compile-time access to generic iterators
  //@{
  lex_iterator genericBegin(lex_tag) const;
  lex_iterator genericEnd(lex_tag) const;
  dlex_iterator genericBegin(dlex_tag) const;
  dlex_iterator genericEnd(dlex_tag) const;
  dp_asc_iterator genericBegin(dp_asc_tag) const;
  dp_asc_iterator genericEnd(dp_asc_tag) const;
  block_dlex_iterator genericBegin(block_dlex_tag) const;
  block_dlex_iterator genericEnd(block_dlex_tag) const;
  block_dp_asc_iterator genericBegin(block_dp_asc_tag) const;
  block_dp_asc_iterator genericEnd(block_dp_asc_tag) const;


  lex_exp_iterator genericExpBegin(lex_tag) const;
  lex_exp_iterator genericExpEnd(lex_tag) const;
  dlex_exp_iterator genericExpBegin(dlex_tag) const;
  dlex_exp_iterator genericExpEnd(dlex_tag) const;
  dp_asc_exp_iterator genericExpBegin(dp_asc_tag) const;
  dp_asc_exp_iterator genericExpEnd(dp_asc_tag) const;
  block_dlex_exp_iterator genericExpBegin(block_dlex_tag) const;
  block_dlex_exp_iterator genericExpEnd(block_dlex_tag) const;
  block_dp_asc_exp_iterator genericExpBegin(block_dp_asc_tag) const;
  block_dp_asc_exp_iterator genericExpEnd(block_dp_asc_tag) const;
  //@}

  /// Navigate through structure
  navigator navigation() const { return m_dd.navigation(); }
 
  /// End of navigation marker
  navigator endOfNavigation() const { return navigator(); }
  
  /// gives a copy of the diagram
  dd_type copyDiagram(){   return diagram();  }

  /// Casting operator to Boolean set
  operator set_type() const { return set(); };

  size_type eliminationLength() const;
  size_type eliminationLengthWithDegBound(deg_type garantied_deg_bound) const;
  /// Get list of all terms
  void fetchTerms(termlist_type&) const;

  /// Return of all terms
  termlist_type terms() const;

  /// Read-only access to internal decision diagramm structure
  const dd_type& diagram() const { return m_dd; }

  /// Get corresponding subset of of the powerset over all variables
  set_type set() const { return m_dd; }

  /// Test, whether we have one term only
  bool_type isSingleton() const { return dd_is_singleton(navigation()); }

  /// Test, whether we have one or two terms only
  bool_type isSingletonOrPair() const { 
    return dd_is_singleton_or_pair(navigation()); 
  }

  /// Test, whether we have two terms only
  bool_type isPair() const { return dd_is_pair(navigation()); }

  /// Access ring, where this belongs to
  ring_type ring() const { return ring_type(m_dd.manager()); } 

  /// Compare with right-hand side and return comparision code
  comp_type compare(const self&) const;

protected:

  /// Access to internal decision diagramm structure
  dd_type& internalDiagram() { return m_dd; }

  /// Generate a polynomial, whose first term is the leading term
  self leadFirst() const;

  /// Get all divisors of the first term
  set_type firstDivisors() const;

private:
  /// The actual decision diagramm
  dd_type m_dd;
};


/// Addition operation 
inline BoolePolynomial 
operator+(const BoolePolynomial& lhs, const BoolePolynomial& rhs) {

  return BoolePolynomial(lhs) += rhs;
}
/// Addition operation 
inline BoolePolynomial 
operator+(const BoolePolynomial& lhs, BooleConstant rhs) {
  return BoolePolynomial(lhs) +=  (rhs);
  //return BoolePolynomial(lhs) +=  BoolePolynomial(rhs);
}

/// Addition operation 
inline BoolePolynomial 
operator+(BooleConstant lhs, const BoolePolynomial& rhs) {

  return BoolePolynomial(rhs) += (lhs);
}


/// Subtraction operation 
template <class RHSType>
inline BoolePolynomial 
operator-(const BoolePolynomial& lhs, const RHSType& rhs) {

  return BoolePolynomial(lhs) -= rhs;
}
/// Subtraction operation with constant right-hand-side
inline BoolePolynomial 
operator-(const BooleConstant& lhs, const BoolePolynomial& rhs) {

  return -(BoolePolynomial(rhs) -= lhs);
}


/// Multiplication with other left-hand side type
#define PBORI_RHS_MULT(type) inline BoolePolynomial \
operator*(const BoolePolynomial& lhs, const type& rhs) { \
    return BoolePolynomial(lhs) *= rhs; }

PBORI_RHS_MULT(BoolePolynomial)
PBORI_RHS_MULT(BooleMonomial)
PBORI_RHS_MULT(BooleExponent)
PBORI_RHS_MULT(BooleConstant)


#undef PBORI_RHS_MULT

/// Multiplication with other left-hand side type
#define PBORI_LHS_MULT(type)  inline BoolePolynomial \
operator*(const type& lhs, const BoolePolynomial& rhs) { return rhs * lhs; }

PBORI_LHS_MULT(BooleMonomial)
PBORI_LHS_MULT(BooleExponent)
PBORI_LHS_MULT(BooleConstant)

#undef PBORI_LHS_MULT


/// Division by monomial (skipping remainder)
template <class RHSType>
inline BoolePolynomial
operator/(const BoolePolynomial& lhs, const RHSType& rhs){
  return BoolePolynomial(lhs) /= rhs;
}

/// Modulus monomial (division remainder)
template <class RHSType>
inline BoolePolynomial
operator%(const BoolePolynomial& lhs, const RHSType& rhs){

  return BoolePolynomial(lhs) %= rhs;
}

/// Equality check (with constant lhs)
inline BoolePolynomial::bool_type
operator==(BoolePolynomial::bool_type lhs, const BoolePolynomial& rhs) {

  return (rhs == lhs); 
}

/// Nonquality check (with constant lhs)
inline BoolePolynomial::bool_type
operator!=(BoolePolynomial::bool_type lhs, const BoolePolynomial& rhs) {

  return (rhs != lhs); 
}

/// Stream output operator
BoolePolynomial::ostream_type& 
operator<<(BoolePolynomial::ostream_type&, const BoolePolynomial&);

// tests whether polynomial can be reduced by rhs
inline BoolePolynomial::bool_type
BoolePolynomial::reducibleBy(const self& rhs) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::reducibleBy(const self&) const" );

  if( rhs.isOne() )
    return true;

  if( isZero() )
    return rhs.isZero();

  return std::includes(firstBegin(), firstEnd(), 
                       rhs.firstBegin(), rhs.firstEnd());

}


END_NAMESPACE_PBORI

#endif // of BoolePolynomial_h_
