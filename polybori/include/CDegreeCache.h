// -*- c++ -*-
//*****************************************************************************
/** @file CDegreeCache.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-21
 *
 * This file contains declaration and template specilizations for the class
 * template CDegreeCache.
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
 * Revision 1.3  2006/11/27 16:25:14  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.2  2006/09/21 16:09:59  dreyer
 * ADD: caching mechanism for BoolePolynomial::deg()
 *
 * Revision 1.1  2006/09/21 15:47:14  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

#include "BoolePolyRing.h"

#include "CCacheManagement.h"

#ifndef CDegreeCache_h_
#define CDegreeCache_h_ 

BEGIN_NAMESPACE_PBORI

/** @class CIndexHandle
 * @brief This class defines an uses an navigator-like type for storing integer
 * values from 0 upto nmax (number of currently active ring variables).
 **/


template <class NaviType>
class CIndexHandle:
  public NaviType {
public:

  /// Set actual storage type
  typedef NaviType navigator;

  /// Define type base
  typedef navigator base;

  /// Plain Boolean type
  typedef typename navigator::bool_type bool_type;

  /// Type for representing indices
  typedef typename base::value_type idx_type;

  /// Type of decision diagram manager
  typedef typename BoolePolyRing::manager_type manager_type;

  /// Construct from index
  CIndexHandle(idx_type idx): base( toNode(idx) ) {}

  /// Construct from given navigator
  explicit CIndexHandle(navigator navi): base(navi) {}

  /// Dereference to get stored index
  typename base::reference operator*() const {
    if (base::isConstant())
      return manager().nVariables();
    else 
      return base::operator*();
  }

protected:

  /// Get active manager
  manager_type& manager() const { return BoolePolyRing::activeManager(); }

  /// Convert plain number to navigation type
  navigator toNode(idx_type idx) const {

    if (idx < manager().nVariables())
      return  navigator(manager().persistentVariable(idx));
    else
      return  navigator(manager().empty());
  }
};

template <class DDType = typename CTypes::dd_type,
          class ManagerType = typename CTypes::manager_base>
class CDegreeCache:
  public CCacheManagement<CCacheTypes::degree, 1, ManagerType> {

public:
  /// @name Define generic access to data types
  //@{
  typedef ManagerType manager_type;
  typedef DDType dd_type;
  typedef CCacheManagement<CCacheTypes::degree, 1, manager_type> base;
  typedef CDegreeCache<dd_type, manager_type> self;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type input_node_type;
  typedef typename dd_type::size_type size_type;
  typedef typename dd_type::navigator navi_type;
  typedef CIndexHandle<navi_type> node_type;
  //@}

  /// Constructor
  CDegreeCache(const manager_type& mgr): base(mgr) {}

  /// Copy Constructor
  CDegreeCache(const self& rhs): base(rhs) {}

  /// Destructor
  ~CDegreeCache() {}

  /// Find cached degree wrt. given navigator
  node_type find(input_node_type navi) const{ 
    return node_type(base::find(navi)); }

  /// Store cached degree wrt. given navigator
  void insert(input_node_type navi, size_type deg) const {
    base::insert(navi, node_type(deg));
  }
};


END_NAMESPACE_PBORI

#endif
