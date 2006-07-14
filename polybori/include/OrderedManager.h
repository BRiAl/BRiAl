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

  /// Get leading term
  virtual monom_type lead(const poly_type&) const = 0;

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

  /// @name adopt global type definitions
  //@{
  typedef typename base::bool_type bool_type;
  typedef typename base::size_type size_type;
  typedef typename base::idx_type idx_type;
  typedef typename base::comp_type comp_type;
  typedef typename base::monom_type monom_type;
  typedef typename base::poly_type poly_type;
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



  /// Get leading term
  monom_type lead(const poly_type& rhs) const {

    return ordering.lead(rhs);
  }

protected:
  order_type ordering;
};

END_NAMESPACE_PBORI
