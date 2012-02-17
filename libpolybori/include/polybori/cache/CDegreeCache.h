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
**/
//*****************************************************************************

#ifndef polybori_cache_CDegreeCache_h_
#define polybori_cache_CDegreeCache_h_ 

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/BooleEnv.h>

#include "CCacheManagement.h"

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

    if PBORI_UNLIKELY(navi.isConstant())
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

    if PBORI_LIKELY((size_type)idx < mgr.nVariables())
	       return  mgr.variable(idx).set().navigation();

    return  mgr.zero().navigation();
  }

  /// Actual navigator, which is stored
  navigator m_navi;
};

template <class DDType>
class CDegreeCache:
  public CCacheManagement<BoolePolyRing, typename CCacheTypes::degree, 1> {
  typedef CDegreeCache self;
public:
  /// @name Define generic access to data types
  //@{
  typedef DDType dd_type;
  typedef typename CCacheTypes::degree tag_type;
  typedef CCacheManagement<BoolePolyRing, tag_type, 1> base;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type input_node_type;
  typedef typename base::manager_type manager_type;
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
  /// Find cached degree wrt. given navigator (ignoring bound as second argument)
  node_type find(input_node_type navi, deg_type) const { return self::find(navi); }

  node_type find(navi_type navi, deg_type) const{ return self::find(navi); }

  /// Store cached degree wrt. given navigator (ignoring bound as second argument)
  void insert(input_node_type navi, deg_type, deg_type deg) const {
    self::insert(navi, deg);
  }

  /// Store cached degree wrt. given navigator
  void insert(navi_type navi, deg_type, deg_type deg) const {
    self::insert(navi, deg);
  }

};

template <class DDType>
class CBoundedDegreeCache:
  public CCacheManagement<BoolePolyRing, typename CCacheTypes::degree, 2> {
  typedef CBoundedDegreeCache self;
public:
  /// @name Define generic access to data types
  //@{
  typedef DDType dd_type;
  typedef typename CCacheTypes::degree tag_type;
  typedef CCacheManagement<BoolePolyRing, tag_type, 2> base;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type input_node_type;
  typedef typename base::manager_type manager_type;
  typedef typename dd_type::navigator navi_type;
  typedef CIndexHandle<navi_type> node_type;
  typedef CIndexCacheHandle<navi_type> node_cache_type;
  //@}

  /// Constructor
  CBoundedDegreeCache(const manager_type& mgr): base(mgr) {}

  /// Copy Constructor
  CBoundedDegreeCache(const self& rhs): base(rhs) {}

  /// Destructor
  ~CBoundedDegreeCache() {}

  /// Find cached degree wrt. given navigator
  node_type find(input_node_type navi, deg_type upper_bound) const{ 
    return node_type(base::find(navi,  
                                node_cache_type(upper_bound, base::manager())),
                     base::manager()); 
  }

  node_type find(navi_type navi, deg_type upper_bound) const{ 
    return node_type(base::find(navi,  
                                node_cache_type(upper_bound, base::manager())),
                     base::manager()); 
  }

  /// Store cached degree wrt. given navigator
  void insert(input_node_type navi, deg_type upper_bound, deg_type deg) const {
    base::insert(navi, node_cache_type(upper_bound, base::manager()),
                 node_cache_type(deg, base::manager()));
  }

  /// Store cached degree wrt. given navigator
  void insert(navi_type navi, deg_type upper_bound, deg_type deg) const {
    base::insert(navi, node_cache_type(upper_bound, base::manager()),
                 node_cache_type(deg, base::manager()));
  }

};


template <class DDType>
class CBlockDegreeCache:
  public CCacheManagement<BoolePolyRing, 
                          typename CCacheTypes::block_degree, 2> {

public:
  /// @name Define generic access to data types
  //@{
  typedef DDType dd_type;
  typedef typename CCacheTypes::block_degree tag_type;
  typedef CCacheManagement<BoolePolyRing, tag_type, 2> base;
  typedef CBlockDegreeCache<dd_type> self;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type input_node_type;
  typedef typename base::manager_type manager_type;
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
          class DDType>
class CDegreeArgumentCache:
  public CCacheManagement<BoolePolyRing, TagType, 2>,
  public CAuxTypes {

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
