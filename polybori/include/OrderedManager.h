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
  typedef BooleMonomial monom_type;
  typedef BoolePolynomial poly_type;
  typedef BooleExponent exp_type;
  //@}

  /// Construct new decision diagramm manager
  OrderedManagerBase(size_type nvars = 0): 
    base(nvars) { }



  /// Construct new decision diagramm manager
  OrderedManagerBase(const self& rhs): 
    base(rhs) { }

  // Destructor
  virtual ~OrderedManagerBase() { }

  /// Comparison of monomials
  virtual comp_type compare(idx_type, idx_type) const = 0;

  virtual comp_type compare(const monom_type&, const monom_type&) const = 0;

  virtual comp_type compare(const exp_type&, const exp_type&) const = 0;

  /// Get leading term
  virtual monom_type lead(const poly_type&) const = 0;

  /// Get leading exponent
  virtual exp_type leadExp(const poly_type&) const = 0;

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

  /// Test whether we deal with a total degree-ordering
  virtual bool_type isTotalDegreeOrder() const = 0;

  /// Test whether variables are in ascending order
  virtual bool_type ascendingVariables() const = 0;

  /// Test whether variables are in descending order
  virtual bool_type descendingVariables() const = 0;
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
  //@}

  /// Construct new decision diagramm manager
  OrderedManager(size_type nvars = 0, 
                 const order_type& theOrder = order_type() ): 
    base(nvars), ordering(theOrder) { }

  /// Construct new decision diagramm manager
  OrderedManager(const self& rhs): 
    base(rhs), ordering(rhs.ordering) { }

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

  /// Get leading exponent
  exp_type leadExp(const poly_type& rhs) const {

    return ordering.leadExp(rhs);
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

  /// Test whether we deal with a total degree-ordering
  bool_type isTotalDegreeOrder() const {
    return properties_type().isTotalDegreeOrder();
  }

  /// Test whether variables are in ascending order
  bool_type ascendingVariables() const {
    return properties_type().ascendingVariables();
  }

  /// Test whether variables are in descending order
  bool_type descendingVariables() const {
    return properties_type().descendingVariables();
  }


protected:
  order_type ordering;
};

END_NAMESPACE_PBORI
