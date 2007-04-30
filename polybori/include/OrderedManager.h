// -*- c++ -*-
//*****************************************************************************
/** @file OrderedManager.h
 *
 * @author Alexander Dreyer
 * @date 2006-05-23
 *
 * Adds ordering management to decision diagram variables managers.
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
 * Revision 1.23  2007/04/30 15:20:31  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.22  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.21  2007/03/21 08:55:09  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.20  2007/03/19 16:49:39  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.19  2007/03/16 16:59:20  dreyer
 * CHANGE: started to rewrite CGenericIter using boost:iterator_facade
 *
 * Revision 1.18  2006/10/24 08:44:04  dreyer
 * CHANGE: Added CVariableNames; variable names handled by OrderedManager
 *
 * Revision 1.17  2006/10/04 12:22:32  dreyer
 * ADD: getOrderCode()
 *
 * Revision 1.16  2006/10/04 11:48:29  dreyer
 * ADD: isDegreeReverseLexicograpical()
 *
 * Revision 1.15  2006/10/02 09:28:38  dreyer
 * ADD BoolePolyRing::changeOrdering and infrastructure
 *
 * Revision 1.14  2006/09/13 15:07:04  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.13  2006/09/12 14:56:55  dreyer
 * ADD bidirectional term iterator template
 *
 * Revision 1.12  2006/09/08 16:15:27  dreyer
 * ADD: Added ordering-dependent term iteration
 *
 * Revision 1.11  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * Revision 1.10  2006/09/05 11:10:44  dreyer
 * ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
 *
 * Revision 1.9  2006/09/05 08:48:32  dreyer
 * ADD: BoolePolyRing::is(Total)DegreeOrder()
 *
 * Revision 1.8  2006/09/01 11:02:48  dreyer
 * ADD: OrderedManager::isSymmetric()
 *
 * Revision 1.7  2006/08/29 08:49:03  dreyer
 * FIX: compare(const exp_type&, const exp_type&) missing in OrderedManagerBase
 *
 * Revision 1.6  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.5  2006/08/09 12:49:55  dreyer
 * ADD preparing order edpendent operations (leadfirst())
 *
 * Revision 1.4  2006/07/20 08:55:49  dreyer
 * ADD isOrdered() and  isLexicographical()
 *
 * Revision 1.3  2006/07/14 09:02:49  dreyer
 * ADD: greater_variable()
 *
 * Revision 1.2  2006/05/24 07:44:57  bricken
 * + made destructor virtual
 *
 * Revision 1.1  2006/05/23 15:26:25  dreyer
 * CHANGE BoolePolyRing  can handle different orderings (only lex yet)
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// get decision diagram manager interface
#include "CDDManager.h"

#include "BoolePolynomial.h"

#include "BooleMonomial.h"

#include "BooleExponent.h"

#include "COrderProperties.h"
#include "CVariableNames.h"

#include "CGenericIter.h"

  //#include "CIndirectIter.h"

#include <vector>
#ifndef OrderedManager_h_
#define OrderedManager_h_

BEGIN_NAMESPACE_PBORI

/** @class OrderedManagerBase
 * @brief This class adds an interface for orderings to CDDManager<>.
 *
 *
 **/

template <class ManType>
class OrderedManagerBase:
  public CDDManager<ManType> { 

public:

  /// Variable manager type
  typedef ManType manager_type;

  /// Variable manager interface (base type)
  typedef CDDManager<manager_type> base;

  /// Type of *this
  typedef OrderedManagerBase<manager_type> self;

  /// @name adopt global type definitions
  //@{
  typedef CTypes::bool_type bool_type;
  typedef CTypes::dd_type dd_type;
  typedef CTypes::size_type size_type;
  typedef CTypes::idx_type idx_type;
  typedef CTypes::comp_type comp_type;
  typedef CTypes::ordercode_type ordercode_type;
  typedef BooleMonomial monom_type;
  typedef BoolePolynomial poly_type;
  typedef BoolePolynomial::navigator navigator;
  typedef BooleExponent exp_type;


  typedef COrderedIter<navigator, monom_type> ordered_iterator;
  typedef COrderedIter<navigator, exp_type> ordered_exp_iterator;
  //@}

  /// Type for block indices
  typedef std::vector<idx_type> block_idx_type;

  /// Type for block iterators
  typedef block_idx_type::const_iterator block_iterator;

  /// Define type for storing names of variables
  typedef CVariableNames variable_names_type;

  /// Define type for getting names of variables
  typedef variable_names_type::const_reference const_varname_reference;

  /// Construct new decision diagramm manager
  OrderedManagerBase(size_type nvars = 0): 
    base(nvars), m_names(nvars)  { }

  /// Construct old decision diagramm manager
  OrderedManagerBase(const base& rhs): 
    base(rhs), m_names(rhs.nVariables()) { }


  /// Construct new decision diagramm manager
  OrderedManagerBase(const self& rhs): 
    base(rhs), m_names(rhs.m_names) { }

  // Destructor
  virtual ~OrderedManagerBase() { }

  /// Comparison of monomials
  virtual comp_type compare(idx_type, idx_type) const = 0;

  virtual comp_type compare(const monom_type&, const monom_type&) const = 0;

  virtual comp_type compare(const exp_type&, const exp_type&) const = 0;

  /// Get leading term
  virtual monom_type lead(const poly_type&) const = 0;

  /// Get leading term (using upper bound)
  virtual monom_type lead(const poly_type&, size_type) const = 0;

  /// Get leading exponent
  virtual exp_type leadExp(const poly_type&) const = 0;

  /// Get leading exponent (using upper bound)
  virtual exp_type leadExp(const poly_type&, size_type) const = 0;

  /// Generates polynomial with leading term first (other terms may be skipped)
  virtual poly_type leadFirst(const poly_type&) const = 0;

  /// Check whether ring is lexicographical 
  virtual bool_type isLexicographical() const = 0;

  /// Test whether iterators respect order
  virtual bool_type orderedStandardIteration() const = 0;

  /// Test whether variable pertubation do not change the order
  virtual bool_type isSymmetric() const = 0;

  /// Test whether we deal with a degree-ordering
  virtual bool_type isDegreeOrder() const = 0;

  /// Test whether we deal with a block-ordering
  virtual bool_type isBlockOrder() const = 0;

  /// Test whether we deal with a total degree-ordering
  virtual bool_type isTotalDegreeOrder() const = 0;

  /// Test whether variables are in ascending order
  virtual bool_type ascendingVariables() const = 0;

  /// Test whether variables are in descending order
  virtual bool_type descendingVariables() const = 0;

  /// Test whether ordering is deg-rev-lex ordering
  virtual bool_type isDegreeReverseLexicograpical() const = 0;

  /// Initialize iterator corresponding to leading term
  virtual ordered_iterator leadIteratorBegin(const poly_type&) const = 0;

  virtual ordered_iterator leadIteratorEnd() const = 0;
  virtual ordered_exp_iterator leadExpIteratorBegin(const poly_type&) const = 0;

  virtual ordered_exp_iterator leadExpIteratorEnd() const = 0;

  /// Get numerical code for ordering
  virtual ordercode_type getOrderCode() const = 0;

  /// Set name of variable with index idx
  void setVariableName(idx_type idx, const_varname_reference varname) {
    m_names[idx] = varname;
  }

  /// Get name of variable with index idx
  const_varname_reference getVariableName(idx_type idx) const { 
    return m_names[idx];
  }
  /// @name interface for block orderings
  //@{
  virtual block_iterator blockBegin() const = 0;
  virtual block_iterator blockEnd() const = 0;
  virtual void appendBlock(idx_type) = 0;
  virtual void clearBlocks() = 0;
  //@}

private:
  /// Stores names of variables
  variable_names_type m_names;
};

/** @class OrderedManager
 * @brief This class initialize the interface for orderings of
 * OrderedManagerBase.
 *
 **/

template <class ManType, class OrderType>
class OrderedManager:
  public OrderedManagerBase<ManType> { 

public:

  /// Variable manager type
  typedef ManType manager_type;

  /// Variable ordering definiton functional type
  typedef OrderType order_type;

  /// Variable manager interface (base type)
  typedef OrderedManagerBase<manager_type> base;

  /// Type of *this
  typedef OrderedManager<manager_type, order_type> self;

  /// Type defining order related properties
  typedef COrderProperties<order_type> properties_type;

  /// @name adopt global type definitions
  //@{
  typedef typename base::bool_type bool_type;
  typedef typename base::size_type size_type;
  typedef typename base::idx_type idx_type;
  typedef typename base::comp_type comp_type;
  typedef typename base::monom_type monom_type;
  typedef typename base::poly_type poly_type;
  typedef typename base::exp_type exp_type;
  typedef typename base::ordered_iterator ordered_iterator;
  typedef typename base::ordered_exp_iterator ordered_exp_iterator;
  typedef typename base::ordercode_type ordercode_type;
  typedef typename base::block_iterator block_iterator;
  //@}

  /// Construct new decision diagramm manager
  OrderedManager(size_type nvars = 0, 
                 const order_type& theOrder = order_type() ): 
    base(nvars), ordering(theOrder) { }

  /// Construct new decision diagramm manager
  OrderedManager(const self& rhs): 
    base(rhs), ordering(rhs.ordering) { }

  /// Construct from given manager
  OrderedManager(const base& rhs, const order_type& theOrder = order_type()  ): 
    base(rhs), ordering(theOrder) { }

  // Destructor
  ~OrderedManager() { }

  /// Comparison of indices
  comp_type compare(idx_type lhs, idx_type rhs) const {
    return ordering.compare(lhs, rhs);
  }

  /// Comparison of monomials
  comp_type compare(const monom_type& lhs, const monom_type& rhs) const {
    return ordering.compare(lhs, rhs);
  }

  comp_type compare(const exp_type& lhs, const exp_type& rhs) const {
    return ordering.compare(lhs, rhs);
  }

  /// Get leading term
  monom_type lead(const poly_type& rhs) const {

    return ordering.lead(rhs);
  }
  /// Get leading term (using upper bound)
  monom_type lead(const poly_type& rhs, size_type bound) const {

    return ordering.lead(rhs, bound);
  }

  /// Get leading exponent
  exp_type leadExp(const poly_type& rhs) const {

    return ordering.leadExp(rhs);
  }

  /// Get leading exponent (using upper bound)
  exp_type leadExp(const poly_type& rhs, size_type bound) const {

    return ordering.leadExp(rhs, bound);
  }

  /// Generates polynomial with leading term first (other terms may be skipped)
  poly_type leadFirst(const poly_type& poly) const {

    if(orderedStandardIteration())
      return poly;
    else 
      return lead(poly);
  }

  /// Check whether ring is lexicographical 
  bool_type isLexicographical() const {
    return properties_type().isLexicographical();
  }

  /// Test whether iterators respect order
  bool_type orderedStandardIteration() const {
    return properties_type().orderedStandardIteration();
  }

  /// Test whether variable pertubation do not change the order
  bool_type isSymmetric() const {
    return properties_type().isSymmetric();
  }

  /// Test whether we deal with a degree-ordering
  bool_type isDegreeOrder() const {
    return properties_type().isDegreeOrder();
  }

  /// Test whether we deal with a degree-ordering
  bool_type isBlockOrder() const {
    return properties_type().isBlockOrder();
  }

  /// Test whether we deal with a total degree-ordering
  bool_type isTotalDegreeOrder() const {
    return properties_type().isTotalDegreeOrder();
  }

  /// Test whether ordering is deg-rev-lex ordering
  bool_type isDegreeReverseLexicograpical() const {
    return properties_type().isDegreeReverseLexicograpical();
  }

  /// Test whether variables are in ascending order
  bool_type ascendingVariables() const {
    return properties_type().ascendingVariables();
  }

  /// Test whether variables are in descending order
  bool_type descendingVariables() const {
    return properties_type().descendingVariables();
  }

  /// Initialize iterator corresponding to leading term
  //  iterator leadIterator(const poly_type& poly) const {
  //    return ordering.leadIterator(poly);
  //  }
   /// Initialize iterator corresponding to leading term
  ordered_iterator leadIteratorBegin(const poly_type& poly) const {
    return ordering.leadIteratorBegin(poly);
  }

  ordered_iterator leadIteratorEnd() const {
    return ordering.leadIteratorEnd();
  }
   /// Initialize iterator corresponding to leading term
  ordered_exp_iterator leadExpIteratorBegin(const poly_type& poly) const {
    return ordering.leadExpIteratorBegin(poly);
  }

  ordered_exp_iterator leadExpIteratorEnd() const {
    return ordering.leadExpIteratorEnd();
  }

  /// Get numerical code for ordering
  ordercode_type getOrderCode() const {
    return order_type::order_code;
  }
  /// @name interface for block orderings
  //@{
  block_iterator blockBegin() const { return ordering.blockBegin(); }
  block_iterator blockEnd() const { return ordering.blockEnd();  }
  void appendBlock(idx_type idx) { ordering.appendBlock(idx); }
  void clearBlocks() { ordering.clearBlocks();  }
  //@}

protected:
  order_type ordering;
};

END_NAMESPACE_PBORI

#endif
