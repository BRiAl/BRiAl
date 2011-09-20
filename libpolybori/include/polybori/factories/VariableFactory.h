// -*- c++ -*-
//*****************************************************************************
/** @file VariableFactory.h 
 *
 * @author Alexander Dreyer
 * @date 2011-05-05
 *
 * This file defines the base class VariableFactory which is used for
 * @em currying the constructors of @c BooleVariable with a BoolePolyRing.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_factories_VariableFactory_h_
#define polybori_factories_VariableFactory_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/factories/CFactoryBase.h>
#include <polybori/BooleVariable.h>

BEGIN_NAMESPACE_PBORI

/** @class VariableFactory
 * @brief This base class VariableFactory is used for @em currying
 * variable constructors with a BoolePolyRing.
 *
 **/

class VariableFactory:
  public CFactoryBase {
  /// Type of *this
  typedef VariableFactory self;

  /// We inherit from the following type
  typedef CFactoryBase base;

public:
  /// Type of parent
  typedef base::parent_type parent_type;

  /// We construct element of this type
  typedef BooleVariable value_type;

  /// Construct from given ring 
  VariableFactory(const parent_type& ring): base(ring) {}

  /// Copy constructor
  VariableFactory(const self& rhs): base(rhs)  {}

  /// Destructor
  ~VariableFactory() {}

  /// Get first variable of the ring
  value_type operator()() const {
    return value_type(0, parent());
  }

  /// Indirect copy constructor
  value_type operator()(const value_type& rhs) const {
    return rhs;
  }

  /// Get first variable of explicit ring
  value_type operator()(const value_type::ring_type& ring) const {
    return value_type(0, ring);
  }

  /// Construct i-th variable for explit ring
  value_type operator()(value_type::idx_type idx,
			const value_type::ring_type& ring) const {
    return value_type(idx, ring);
  }
  /// Construct i-th variable
  value_type operator()(value_type::idx_type idx) const {
    return value_type(idx, parent());
  }
};

END_NAMESPACE_PBORI

#endif /* polybori_factories_VariableFactory_h_ */
