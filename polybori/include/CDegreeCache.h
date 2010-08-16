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
 * Revision 1.16  2008/09/21 22:21:02  dreyer
 * Change: deg_type replaces size_type for deg(), etc.
 *
 * Revision 1.15  2008/01/17 15:18:40  dreyer
 * CHANGE: removed several calls of BooleEnv::*
 *
 * Revision 1.14  2008/01/16 17:10:18  dreyer
 * CHANGE: term-iterators use correct manager now
 *
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
class CIndexHandle {
public:

  enum { invalid  = CTypes::max_idx };

  /// Set actual storage type
  typedef NaviType navigator;

  /// Define type base
  typedef navigator base;

  /// Plain Boolean type
  typedef typename navigator::bool_type bool_type;

  /// Type for representing indices
  typedef typename CTypes::idx_type idx_type;

   /// Type for representing size
  typedef typename CTypes::size_type size_type;

  /// Type of decision diagram manager
  typedef BoolePolyRing manager_type;

  /// Construct from index
  CIndexHandle(idx_type idx): m_idx(idx) {}

  /// Construct from given navigator
  explicit CIndexHandle(navigator navi, const manager_type& mgr): 
    m_idx(fromNode(navi, mgr)) {}

  /// Dereference to get stored index
  idx_type operator*() const {
    return m_idx;
  }

  bool isValid() const {
    return (m_idx != invalid);
  }
protected:
  /// Dereference to get stored index
  idx_type fromNode(navigator navi, const manager_type& mgr) const {

    if (!navi.isValid())
      return invalid;

    if UNLIKELY(navi.isConstant())
      return mgr.nVariables();
    else 
      return *navi;
  }



  idx_type m_idx;
};


template <class NaviType>
class CIndexCacheHandle {
public:

  /// Set actual storage type
  typedef NaviType navigator;

  /// Define type base
  //  typedef navigator base;

  /// Plain Boolean type
  typedef typename navigator::bool_type bool_type;

  /// Type for representing indices
  typedef typename navigator::value_type idx_type;

  /// Type for representing size
  typedef typename navigator::size_type size_type;

  /// Type of decision diagram manager
  typedef BoolePolyRing manager_type;

  /// Construct from index
  CIndexCacheHandle(idx_type idx, const manager_type& mgr):
    m_navi( toNode(idx, mgr) ) {}

  /// Construct from given navigator
  explicit CIndexCacheHandle(navigator navi): 
    m_navi(navi) {}

  operator navigator() const { return m_navi; }

protected:
  /// Convert plain number to navigation type
  navigator toNode(idx_type idx, const manager_type& mgr) const {

    if LIKELY((size_type)idx < mgr.nVariables())
      return  mgr.variable(idx).navigation();

    return  mgr.zero().navigation();
  }

  /// Actual navigator, which is stored
  navigator m_navi;
};

template <class TagType = typename CCacheTypes::degree,
          class DDType = typename CTypes::dd_type>
class CDegreeCache:
  public CCacheManagement<BoolePolyRing, TagType, 1> {

public:
  /// @name Define generic access to data types
  //@{
  typedef DDType dd_type;
  typedef TagType tag_type;
  typedef CCacheManagement<BoolePolyRing, tag_type, 1> base;
  typedef CDegreeCache<tag_type, dd_type> self;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type input_node_type;
  typedef typename base::manager_type manager_type;
  typedef typename dd_type::size_type size_type;
  typedef typename dd_type::deg_type deg_type;
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
    return node_type(base::find(navi), base::manager()); }

  node_type find(navi_type navi) const{ 
    return node_type(base::find(navi), base::manager()); }

  /// Store cached degree wrt. given navigator
  void insert(input_node_type navi, deg_type deg) const {
    base::insert(navi, node_cache_type(deg, base::manager()));
  }

  /// Store cached degree wrt. given navigator
  void insert(navi_type navi, deg_type deg) const {
    base::insert(navi, node_cache_type(deg, base::manager()));
  }

};




template <class TagType =  typename CCacheTypes::block_degree,
          class DDType = typename CTypes::dd_type>
class CBlockDegreeCache:
  public CCacheManagement<BoolePolyRing, TagType, 2> {

public:
  /// @name Define generic access to data types
  //@{
  typedef DDType dd_type;
  typedef TagType tag_type;
  typedef CCacheManagement<BoolePolyRing, tag_type, 2> base;
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
    return node_type(base::find(navi, node_cache_type(idx, base::manager())), 
                     base::manager()); }

  node_type find(navi_type navi, idx_type idx) const{ 
    return node_type(base::find(navi, node_cache_type(idx, base::manager())), 
                     base::manager()); }

  /// Store cached degree wrt. given navigator
  void insert(input_node_type navi, idx_type idx, size_type deg) const {
    base::insert(navi, node_cache_type(idx, base::manager()), 
                 node_cache_type(deg, base::manager()));
  }

  /// Store cached degree wrt. given navigator
  void insert(navi_type navi, idx_type idx, size_type deg) const {
    base::insert(navi, node_cache_type(idx, base::manager()), 
                 node_cache_type(deg, base::manager()));
  }
};

template <class TagType, 
          class DDType = typename CTypes::dd_type>
class CDegreeArgumentCache:
  public CCacheManagement<BoolePolyRing, TagType, 2> {

public:
  /// @name Define generic access to data types
  //@{
  typedef DDType dd_type;
  typedef TagType tag_type;
  typedef CCacheManagement<BoolePolyRing, tag_type, 2> base;
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
    return base::find(navi, degree_node_type(deg, base::manager())); 
  }

  /// Store cached degree wrt. given navigator
  void insert(navi_type navi, size_type deg, navi_type result) const {
    base::insert(navi, degree_node_type(deg, base::manager()), result);
  }

};


END_NAMESPACE_PBORI

#endif
