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
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.13  2007/12/19 09:03:16  dreyer
 * CHANGE: make Cudd-related globals static
 *
 * Revision 1.12  2007/12/18 10:20:17  dreyer
 * CHANGE CNamedManager removed, names are in core now
 *
 * Revision 1.11  2007/12/17 16:12:02  dreyer
 * CHANGE: reviewed and optimized merge frim sf.net
 *
 * Revision 1.10  2007/12/13 15:53:48  dreyer
 * CHANGE: Ordering in BoolePolyRing again; BooleEnv manages active ring
 *
 * Revision 1.9  2007/11/06 15:03:34  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.8  2007/10/09 10:30:51  dreyer
 * ADD: poly.gradedPart(deg); FIX: term_accumulate (constant term)
 *
 * Revision 1.7  2007/07/30 15:19:39  dreyer
 * CHANGE: CCuddNavigator does not convert to DdNode* impicitely any more
 *
 * Revision 1.6  2007/04/24 15:23:03  dreyer
 * FIX: minor changes fixing -Wall warnings
 *
 * Revision 1.5  2007/03/21 08:55:08  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.4  2006/11/30 19:42:44  dreyer
 * CHANGE: lead(bound) now uses cached and recursive variant
 *
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

#include "BooleEnv.h"

#include "CCacheManagement.h"

#ifndef CDegreeCache_h_
#define CDegreeCache_h_ 

BEGIN_NAMESPACE_PBORI
//class BoolePolyRing;
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

   /// Type for representing size
  typedef typename base::size_type size_type;

  /// Type of decision diagram manager
  typedef typename CTypes::manager_type manager_type;

  /// Construct from index
  CIndexHandle(idx_type idx): base( toNode(idx) ) {}

  /// Construct from given navigator
  explicit CIndexHandle(navigator navi): base(navi) {}

  /// Dereference to get stored index
  typename base::reference operator*() const {
    if UNLIKELY(base::isConstant())
      return manager().nVariables();
    else 
      return base::operator*();
  }

protected:

  /// Get active manager
  manager_type& manager() const { return BooleEnv::manager(); }

  /// Convert plain number to navigation type
  navigator toNode(idx_type idx) const {

    if LIKELY((size_type)idx < manager().nVariables())
      return  navigator(manager().persistentVariable(idx));

    return  navigator(manager().empty());
  }
};


template <class NaviType>
class CIndexCacheHandle:
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

   /// Type for representing size
  typedef typename base::size_type size_type;

  /// Type of decision diagram manager
  typedef typename CTypes::manager_type manager_type;

  /// Construct from index
  CIndexCacheHandle(idx_type idx): base( toNode(idx) ) {}

  /// Construct from given navigator
  explicit CIndexCacheHandle(navigator navi): base(navi) {}

  /// Dereference to get stored index
  typename base::reference operator*() const {
    if UNLIKELY(base::isConstant())
      return manager().nVariables();
    else 
      return base::operator*();
  }

protected:

  /// Get active manager
  manager_type& manager() const { return BooleEnv::manager(); }

  /// Convert plain number to navigation type
  navigator toNode(idx_type idx) const {

    if LIKELY((size_type)idx < manager().nVariables())
      return  navigator(manager().persistentVariable(idx));

    return  navigator(manager().empty());
  }
};

template <class TagType = typename CCacheTypes::degree,
          class DDType = typename CTypes::dd_type>
class CDegreeCache:
  public CCacheManagement<TagType, 1> {

public:
  /// @name Define generic access to data types
  //@{
  typedef DDType dd_type;
  typedef TagType tag_type;
  typedef CCacheManagement<tag_type, 1> base;
  typedef CDegreeCache<tag_type, dd_type> self;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type input_node_type;
  typedef typename base::manager_type manager_type;
  typedef typename dd_type::size_type size_type;
  typedef typename dd_type::navigator navi_type;
  typedef CIndexHandle<navi_type> node_type;
  typedef CIndexCacheHandle<navi_type> node_cache_type;
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

  node_type find(navi_type navi) const{ 
    return node_type(base::find(navi)); }

  /// Store cached degree wrt. given navigator
  void insert(input_node_type navi, size_type deg) const {
    base::insert(navi, node_cache_type(deg));
  }

  /// Store cached degree wrt. given navigator
  void insert(navi_type navi, size_type deg) const {
    base::insert(navi, node_cache_type(deg));
  }

};




template <class TagType =  typename CCacheTypes::block_degree,
          class DDType = typename CTypes::dd_type>
class CBlockDegreeCache:
  public CCacheManagement<TagType, 2> {

public:
  /// @name Define generic access to data types
  //@{
  typedef DDType dd_type;
  typedef TagType tag_type;
  typedef CCacheManagement<tag_type, 2> base;
  typedef CBlockDegreeCache<tag_type, dd_type> self;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type input_node_type;
  typedef typename base::manager_type manager_type;
  typedef typename dd_type::idx_type idx_type;
  typedef typename dd_type::size_type size_type;
  typedef typename dd_type::navigator navi_type;
  typedef CIndexHandle<navi_type> node_type;
  typedef CIndexCacheHandle<navi_type> node_cache_type;
  //@}

  /// Constructor
  CBlockDegreeCache(const manager_type& mgr): base(mgr) {}

  /// Copy Constructor
  CBlockDegreeCache(const self& rhs): base(rhs) {}

  /// Destructor
  ~CBlockDegreeCache() {}

  /// Find cached degree wrt. given navigator
  node_type find(input_node_type navi, idx_type idx) const{ 
    return node_type(base::find(navi, node_type(idx))); }

  node_type find(navi_type navi, idx_type idx) const{ 
    return node_type(base::find(navi, node_type(idx))); }

  /// Store cached degree wrt. given navigator
  void insert(input_node_type navi, idx_type idx, size_type deg) const {
    base::insert(navi, node_cache_type(idx), node_cache_type(deg));
  }

  /// Store cached degree wrt. given navigator
  void insert(navi_type navi, idx_type idx, size_type deg) const {
    base::insert(navi, node_cache_type(idx), node_cache_type(deg));
  }
};

template <class TagType, 
          class DDType = typename CTypes::dd_type>
class CDegreeArgumentCache:
  public CCacheManagement<TagType, 2> {

public:
  /// @name Define generic access to data types
  //@{
  typedef DDType dd_type;
  typedef TagType tag_type;
  typedef CCacheManagement<tag_type, 2> base;
  typedef CDegreeArgumentCache<tag_type, dd_type> self;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type node_type;
  typedef typename base::manager_type manager_type;
  typedef typename dd_type::size_type size_type;
  typedef typename dd_type::navigator navi_type;
  typedef CIndexCacheHandle<navi_type> degree_node_type;
  //@}

  /// Constructor
  CDegreeArgumentCache(const manager_type& mgr): base(mgr) {}

  /// Copy Constructor
  CDegreeArgumentCache(const self& rhs): base(rhs) {}

  /// Destructor
  ~CDegreeArgumentCache() {}

  /// Find cached degree wrt. given navigator
  navi_type find(navi_type navi, size_type deg) const{ 
    return base::find(navi, degree_node_type(deg)); 
  }

  /// Store cached degree wrt. given navigator
  void insert(navi_type navi, size_type deg, navi_type result) const {
    base::insert(navi, degree_node_type(deg), result);
  }

};


END_NAMESPACE_PBORI

#endif
