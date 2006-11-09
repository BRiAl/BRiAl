// -*- c++ -*-
//*****************************************************************************
/** @file CCacheManagement.h
 *
 * @author Alexander Dreyer
 * @date 2006-10-23
 *
 * This file 
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
 * Revision 1.2  2006/11/09 12:48:33  dreyer
 * + added cachemanager to multiply_recursively2|3
 *
 * Revision 1.1  2006/10/25 14:17:40  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// get DD navigation
#include "CCuddNavigator.h"

#ifndef CCacheManagement_h_
#define CCacheManagement_h_

BEGIN_NAMESPACE_PBORI


class CCacheTypes {
protected:
  struct unary_cache_type { enum { nargs = 1 }; };
  struct binary_cache_type { enum { nargs = 2 }; };

public:
  struct union_xor: public binary_cache_type { };

  struct multiply_recursive: public binary_cache_type { };

  struct dlex_lead: public unary_cache_type { };
  struct dp_asc_lead: public unary_cache_type { };
};


class cudd_manager_storage {
public:
  /// Set  manager type
  typedef Cudd manager_type;

  /// Set type of Cudd's nodes
  typedef DdNode* node_type;

  /// Constructor
  cudd_manager_storage(const manager_type& mgr): 
    m_mgr(mgr.getManager()) {}

  /// Set type of Cudd's internal manager type
  typedef DdManager* internal_manager_type;

  /// Accessing operator
  internal_manager_type operator()() const { return m_mgr; }

private:
  /// Store (pointer) to internal manager
  internal_manager_type m_mgr;
};

template <class ManagerType, class CacheType, unsigned ArgumentLength>
class CCacheManBase;

template <class CacheType>
class CCacheManBase<Cudd, CacheType, 1> :
  protected cudd_manager_storage {

public:
  /// Set base type
  typedef cudd_manager_storage base;

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type node) const {
    return cuddCacheLookup1Zdd(base::operator()(), cache_dummy, node);
  }

  /// Store cached value wrt. given node  
  void insert(node_type node, node_type result) const {
    Cudd_Ref(result);
    cuddCacheInsert1(base::operator()(), cache_dummy, node, result);
    Cudd_Deref(result);
  }

private:
  /// Define unique static function, as marker for Cudd cache
  static node_type cache_dummy(internal_manager_type, node_type){}
};

template <class CacheType>
class CCacheManBase<Cudd, CacheType, 2> :
  protected cudd_manager_storage {

public:

  /// Set base type
  typedef cudd_manager_storage base;

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type first, node_type second) const {
    return cuddCacheLookup2Zdd(base::operator()(), cache_dummy, first, second);
  }

  /// Store cached value wrt. given node  
  void insert(node_type first, node_type second, node_type result) const {
    Cudd_Ref(result);
    cuddCacheInsert2(base::operator()(), cache_dummy, first, second, result);
    Cudd_Deref(result);
  }

private:
  /// Define unique static function, as marker for Cudd cache
  static node_type cache_dummy(internal_manager_type, node_type, node_type){}
};


template <class CacheType, class ManagerType = typename CTypes::manager_base>
class CCacheManagement: 
  public CCacheManBase<ManagerType, CacheType, CacheType::nargs> {
public:


  typedef ManagerType manager_type;
  typedef CacheType cache_type;
  enum { nargs = cache_type::nargs };

  typedef CCacheManBase<manager_type, cache_type, nargs> base;

  /// Constructor
  CCacheManagement(const manager_type& mgr): base(mgr) {}


  using base::find;
  using base::insert;
};



END_NAMESPACE_PBORI

#endif
