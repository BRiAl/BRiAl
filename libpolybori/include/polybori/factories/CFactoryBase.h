// -*- c++ -*-
//*****************************************************************************
/** @file CFactoryBase.h 
 *
 * @author Alexander Dreyer
 * @date 2011-05-05
 *
 * This file defines the base class CFactoryBase which is used for @em currying
 * other constructors with a BoolePolyRing.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_factories_CFactoryBase_h_
#define polybori_factories_CFactoryBase_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/BoolePolyRing.h>


BEGIN_NAMESPACE_PBORI

/** @class CFactoryBase
 * @brief This base class CFactoryBase is used for @em currying
 * other constructors with a BoolePolyRing.
 *
 * We generate pseudo constructors that a ring to be provided, because the
 * latter is stored in the factory.
 **/

class CFactoryBase {
  /// Type of *this
  typedef CFactoryBase self;

public:
  /// Type of parent
  typedef BoolePolyRing parent_type;

  /// Default constructor
  CFactoryBase(const parent_type& parent): m_parent(parent) {}

  /// Copy constructor
  CFactoryBase(const self& rhs): m_parent(rhs.m_parent)  {}

  /// Destructor
  ~CFactoryBase() {}

  /// Access delayed data
  const parent_type& parent() const { return m_parent; }

private:
  parent_type m_parent;
};

END_NAMESPACE_PBORI

#endif /* polybori_factories_CFactoryBase_h_ */
