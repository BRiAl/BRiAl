// -*- c++ -*-
//*****************************************************************************
/** @file PolynomialFactory.h 
 *
 * @author Alexander Dreyer
 * @date 2011-05-056
 *
 * This file defines the base class PolynomialFactory which is used for
 * @em currying the constructors of @c BoolePolynomial with a BoolePolyRing.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_factories_PolynomialFactory_h_
#define polybori_factories_PolynomialFactory_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/factories/CFactoryBase.h>
#include <polybori/BoolePolynomial.h>
#include <polybori/BooleMonomial.h>
BEGIN_NAMESPACE_PBORI

/** @class PolynomialFactory
 * @brief This base class PolynomialFactory is used for @em currying
 * polynomial constructors with a BoolePolyRing.
 *
 **/

class PolynomialFactory:
  public CFactoryBase {
  /// Type of *this
  typedef PolynomialFactory self;

  /// We inherit from the following type
  typedef CFactoryBase base;

public:
  /// Type of parent
  typedef base::parent_type parent_type;

  /// We construct element of this type
  typedef BoolePolynomial value_type;

  /// Construct from given ring 
  PolynomialFactory(const parent_type& ring): base(ring) {}

  /// Copy constructor
  PolynomialFactory(const self& rhs): base(rhs)  {}

  /// Destructor
  ~PolynomialFactory() {}

  /// Get zero polynomial of the ring
  value_type operator()() const {
    return value_type(parent());
  }

  /// Indirect copy constructor
  value_type operator()(const value_type& rhs) const {
    return rhs;
  }

  /// Get first variable of explicit ring
  value_type operator()(const value_type::ring_type& ring) const {
    return value_type(ring);
  }

  /// Construct from Boolean variable (does not need ring)
  value_type operator()(const value_type::var_type& var) const {
    return value_type::monom_type(var);
  }

 /// Construct from Boolean monomial (does not need ring)
  value_type operator()(const value_type::monom_type& monom) const {
    return value_type(monom);
  }

  /// Construct from exponent vector
  value_type operator()(const value_type::exp_type& rhs) const {
    return value_type(rhs, parent());
  }

  /// Construct from exponent vector and explicit ring
  value_type operator()(const value_type::exp_type& rhs,
			const value_type::ring_type& ring) const {
    return value_type(rhs, ring);
  }
  /// Construct polynomial from a constant value 0 or 1 and explicit ring
  value_type operator()(value_type::constant_type isOne, 
			const value_type::ring_type& ring) const {
    return value_type(isOne, ring);
  }
 
  /// Construct polynomial from a constant value 0 or 1
  value_type operator()(value_type::constant_type isOne) const {
    return value_type(isOne, parent());
  }

  /// Construct polynomial from decision diagram
  value_type operator()(const value_type::dd_type& rhs) const {
    return value_type(rhs);
  }

  /// Construct polynomial from navigator and explicit ring
  value_type operator()(const value_type::navigator& rhs, 
			const value_type::ring_type& ring) const{
    return value_type(rhs, ring);
  }
  /// Construct polynomial from navigator
  value_type operator()(const value_type::navigator& rhs) const{
    return value_type(rhs, parent());
  }
};

END_NAMESPACE_PBORI

#endif /* polybori_factories_PolynomialFactory_h_ */
