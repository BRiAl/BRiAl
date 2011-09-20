// -*- c++ -*-
//*****************************************************************************
/** @file MonomialFactory.h 
 *
 * @author Alexander Dreyer
 * @date 2011-05-05
 *
 * This file defines the base class MonomialFactory which is used for
 * @em currying the constructors of @c BooleMonomial with a BoolePolyRing.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_factories_MonomialFactory_h_
#define polybori_factories_MonomialFactory_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/factories/CFactoryBase.h>
#include <polybori/BooleMonomial.h>

BEGIN_NAMESPACE_PBORI

/** @class MonomialFactory
 * @brief This base class MonomialFactory is used for @em currying
 * monomial constructors with a BoolePolyRing.
 *
 **/

class MonomialFactory:
  public CFactoryBase {
  /// Type of *this
  typedef MonomialFactory self;

  /// We inherit from the following type
  typedef CFactoryBase base;

public:
  /// Type of parent
  typedef base::parent_type parent_type;

  /// We construct element of this type
  typedef BooleMonomial value_type;

  /// Construct from given ring 
  MonomialFactory(const parent_type& ring): base(ring) {}

  /// Copy constructor
  MonomialFactory(const self& rhs): base(rhs)  {}

  /// Destructor
  ~MonomialFactory() {}

  /// Get first variable of the ring
  value_type operator()() const {
    return value_type(parent());
  }

  /// Get first variable of explicit ring
  value_type operator()(const value_type::ring_type& ring) const {
    return value_type(ring);
  }

  /// Indirect copy constructor
  value_type operator()(const value_type& rhs) const {
    return rhs;
  }

  /// Construct from Boolean variable (does not need ring)
  value_type operator()(value_type::var_type var) const {
    return value_type(var);
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

};

END_NAMESPACE_PBORI

#endif /* polybori_factories_MonomialFactory_h_ */
