// -*- c++ -*-
//*****************************************************************************
/** @file SetFactory.h 
 *
 * @author Alexander Dreyer
 * @date 2011-05-056
 *
 * This file defines the base class SetFactory which is used for
 * @em currying the constructors of @c BooleSet with a BoolePolyRing.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_SetFactory_h_
#define polybori_SetFactory_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/factories/CFactoryBase.h>
#include <polybori/BooleSet.h>
#include <polybori/BooleMonomial.h>
BEGIN_NAMESPACE_PBORI

/** @class SetFactory
 * @brief This base class SetFactory is used for @em currying
 * set constructors with a BoolePolyRing.
 *
 **/

class SetFactory:
  public CFactoryBase {
  /// Type of *this
  typedef SetFactory self;

  /// We inherit from the following type
  typedef CFactoryBase base;

public:
  /// Type of parent
  typedef base::parent_type parent_type;

  /// We construct element of this type
  typedef BooleSet value_type;

  /// Construct from given ring 
  SetFactory(const parent_type& ring): base(ring) {}

  /// Copy constructor
  SetFactory(const self& rhs): base(rhs)  {}

  /// Destructor
  ~SetFactory() {}

  /// Get empty set of the ring
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

  /// Construct new node
  value_type operator()(value_type::idx_type idx, 
			const value_type& first,
			const value_type& second) const {
    return value_type(idx, first, second);
  }

  /// Construct new node (using navigator nodes)
  value_type operator()(value_type::idx_type idx, 
			value_type::navigator first,
			value_type::navigator second) const {
    return value_type(idx, first, second, parent());
  }

  /// Construct new node (using navigator nodes and explicit ring)
  value_type operator()(value_type::idx_type idx, 
			value_type::navigator first,
			value_type::navigator second,
			value_type::ring_type& ring) const {
    return value_type(idx, first, second, ring);
  }

  /// Construct new node from navigator (using explicit ring)
  value_type operator()(value_type::navigator navi,
			value_type::ring_type& ring) const {
    return value_type(navi, ring);
  }

  /// Construct new node from navigator
  value_type operator()(value_type::navigator navi) const {
    return value_type(navi, parent());
  }
};

END_NAMESPACE_PBORI

#endif /* polybori_SetFactory_h_ */
