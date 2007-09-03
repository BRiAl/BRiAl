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
 * Revision 1.33  2007/09/03 13:43:09  bricken
 * + changed implementation for opposite_logic
 *
 * Revision 1.32  2007/08/27 12:44:02  bricken
 * + new factorization/disabled at the moment
 *
 * Revision 1.31  2007/08/27 11:26:24  bricken
 * + used counters
 *
 * Revision 1.30  2007/08/27 11:07:14  bricken
 * + more funs with cache
 *
 * Revision 1.29  2007/08/27 10:31:51  bricken
 * + more funs with cache
 *
 * Revision 1.28  2007/07/30 15:19:38  dreyer
 * CHANGE: CCuddNavigator does not convert to DdNode* impicitely any more
 *
 * Revision 1.27  2007/07/06 14:04:21  dreyer
 * ADD: newly written C++_interface for Cudd
 *
 * Revision 1.26  2007/07/06 08:24:44  bricken
 * + include_divsors
 *
 * Revision 1.25  2007/07/03 09:58:06  bricken
 * + new tag
 *
 * Revision 1.24  2007/07/02 14:15:08  bricken
 * +smallest lex
 *
 * Revision 1.23  2007/07/02 11:36:30  bricken
 * + interpolate
 *
 * Revision 1.22  2007/06/05 05:43:54  bricken
 * + plug_1
 *
 * Revision 1.21  2007/05/24 14:01:30  dreyer
 * CHANGE: Recursive routine for usedVariables()
 *
 * Revision 1.20  2007/05/10 07:29:32  bricken
 * + new minimal elements
 *
 * Revision 1.19  2007/04/24 15:23:03  dreyer
 * FIX: minor changes fixing -Wall warnings
 *
 * Revision 1.18  2007/04/16 12:38:05  bricken
 * + ll_red_nf
 *
 * Revision 1.17  2007/03/21 08:55:08  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.16  2006/12/09 10:46:18  dreyer
 * CHANGE added and used recursively cache variant of /=
 *
 * Revision 1.15  2006/12/07 08:22:52  dreyer
 * ADD/CHANGE: Lowlevel variant of existAbstract
 *
 * Revision 1.14  2006/11/30 19:42:43  dreyer
 * CHANGE: lead(bound) now uses cached and recursive variant
 *
 * Revision 1.13  2006/11/27 16:25:13  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.12  2006/11/27 08:00:56  bricken
 * + mod_deg2
 *
 * Revision 1.11  2006/11/23 12:46:44  bricken
 * + new minimal elements
 *
 * Revision 1.10  2006/11/22 15:46:22  dreyer
 * ADD: CacheManager replacing CCacheManagement for external use
 * CHANGE: CacheManager used, where necessary
 * CHANGE: multiplesOf, uses cached recursion
 *
 * Revision 1.9  2006/11/21 12:33:34  dreyer
 * ADD: BooleSet::ownsOne, BooleSet(idx, navi, navi); divisorsof
 *
 * Revision 1.8  2006/11/21 09:52:05  dreyer
 * CHANGE: some simple functions in BoolePolynomial inlined
 * ADD: caching of ternary operations
 * ADD: commandline switch PBORI_FAST_MULTIPLICATION (dense multiplication)
 *
 * Revision 1.7  2006/11/20 14:56:46  dreyer
 * CHANGE CCacheType names, operator*=, CDDInterface node Constructor
 *
 * Revision 1.6  2006/11/20 12:40:57  bricken
 * + new minimal elements, fails tests
 *
 * Revision 1.5  2006/11/13 11:39:52  dreyer
 * ADD: changed order of template arguments
 *
 * Revision 1.4  2006/11/13 11:31:15  dreyer
 * ADD: Arity may be changed/given explicitely as template argument
 *
 * Revision 1.3  2006/11/10 16:19:49  dreyer
 * ADD: Commutative caching
 *
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

// get polynomial ring
//#include "BoolePolyRing.h"

// get standard functionality
#include <functional>

#ifndef CCacheManagement_h_
#define CCacheManagement_h_

BEGIN_NAMESPACE_PBORI


class CCacheTypes {

public:
  struct unary_cache_tag { enum { nargs = 1 }; };
  struct binary_cache_tag { enum { nargs = 2 }; };
  struct ternary_cache_tag { enum { nargs = 3 }; };

  // user functions
  struct union_xor: public binary_cache_tag { };

  struct multiply_recursive: public binary_cache_tag { };
  struct divide: public binary_cache_tag { };

  struct minimal_mod: public binary_cache_tag { };
  struct minimal_elements: public unary_cache_tag { };

  struct multiplesof: public binary_cache_tag { };
  struct divisorsof: public binary_cache_tag { };
  struct ll_red_nf: public binary_cache_tag { };
  struct plug_1: public binary_cache_tag { };
  struct exist_abstract: public binary_cache_tag { };

  struct degree: public unary_cache_tag { };

  struct has_factor_x: public binary_cache_tag { };
  struct has_factor_x_plus_one: public binary_cache_tag { };

  
  struct mod_varset: public binary_cache_tag { };
  struct interpolate: public binary_cache_tag { };
  struct zeroes: public binary_cache_tag { };
  struct interpolate_smallest_lex: public binary_cache_tag { };
  
  struct include_divisors: public unary_cache_tag { };
  
  //struct mod_deg2_set: public binary_cache_tag { };
  typedef mod_varset mod_deg2_set;
  typedef mod_varset mod_mon_set;
  
  struct contained_deg2: public unary_cache_tag { };
  struct contained_variables: public unary_cache_tag { };

  struct map_every_x_to_x_plus_one: public unary_cache_tag { };

  struct dlex_lead: public unary_cache_tag { };
  struct dp_asc_lead: public unary_cache_tag { };

  struct divisorsof_fixedpath: public ternary_cache_tag { };
  struct testwise_ternary: public ternary_cache_tag { };

  struct used_variables: public unary_cache_tag { };

  struct block_degree: public binary_cache_tag { };
  struct block_dlex_lead: public unary_cache_tag { };
  
  struct has_factor_x_plus_y: public ternary_cache_tag { };
  struct left_equals_right_x_branch_and_r_has_fac_x: public ternary_cache_tag { };
};

// Reserve integer Numbers for Ternary operations (for cudd)
template <class TagType>
struct count_tags;

template<>
struct count_tags<CCacheTypes::divisorsof_fixedpath>{
  enum { value = 0 };
};

template <class BaseTag>
struct increment_count_tags {
  enum{ value = count_tags<BaseTag>::value + 1 };
};

template<>
class count_tags<CCacheTypes::testwise_ternary>:
  public increment_count_tags<CCacheTypes::divisorsof_fixedpath>{ };
template<>
class count_tags<CCacheTypes::left_equals_right_x_branch_and_r_has_fac_x>:
  public increment_count_tags<CCacheTypes::testwise_ternary>{ };
template<>
class count_tags<CCacheTypes::has_factor_x_plus_y>:
  public increment_count_tags<CCacheTypes::left_equals_right_x_branch_and_r_has_fac_x>{ };
// generate tag number (special pattern with 4 usable bits)
// 18 bits are already used
template <unsigned Counted, unsigned Offset = 18>
class cudd_tag_number {
public:
  enum { value = 
         ( ((Counted + Offset) & 0x3 ) << 2) | 
         ( ((Counted + Offset) & 0x1C ) << 3) | 0x2 };
};


class cudd_manager_storage {
public:
  /// Set  manager type
  typedef Cudd manager_type;

  /// Set type of Cudd's nodes
  typedef DdNode* node_type;

  typedef CCuddNavigator navigator;


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

class mycudd_manager_storage {
public:
  /// Set  manager type
  typedef CCuddInterface manager_type;

  /// Set type of Cudd's nodes
  typedef DdNode* node_type;
  typedef CCuddNavigator navigator;
  /// Constructor
  mycudd_manager_storage(const manager_type& mgr): 
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

  /// Find cached value wrt. given node (for navigator type)
  navigator find(navigator node) const { 
    return find(node.getNode()); 
  }

  /// Store cached value wrt. given node  
  void insert(node_type node, node_type result) const {
    Cudd_Ref(result);
    cuddCacheInsert1(base::operator()(), cache_dummy, node, result);
    Cudd_Deref(result);
  }

  /// Store cached value wrt. given node  
  void insert(navigator node, navigator result) const {
    insert(node.getNode(), result.getNode());
  }

private:
  /// Define unique static function, as marker for Cudd cache
  static node_type cache_dummy(internal_manager_type, node_type){
    return NULL;
  }
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
  /// Find cached value wrt. given node (for navigator type)
  navigator find(navigator first, navigator second) const { 
    return find(first.getNode(), second.getNode());
  }

  /// Store cached value wrt. given node  
  void insert(node_type first, node_type second, node_type result) const {
    Cudd_Ref(result);
    cuddCacheInsert2(base::operator()(), cache_dummy, first, second, result);
    Cudd_Deref(result);
  }

  /// Store cached value wrt. given node  
  void insert(navigator first, navigator second, navigator result) const {
    insert(first.getNode(), second.getNode(), result.getNode());
  }

private:
  /// Define unique static function, as marker for Cudd cache
  static node_type cache_dummy(internal_manager_type, node_type, node_type){
    return NULL;
}
};


template <class CacheType>
class CCacheManBase<Cudd, CacheType, 3> :
  protected cudd_manager_storage {

public:

  /// Set base type
  typedef cudd_manager_storage base;

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type first, node_type second, node_type third) const {
    return cuddCacheLookupZdd(base::operator()(), (ptruint)GENERIC_DD_TAG, 
                              first, second, third);
  }

  /// Find cached value wrt. given node (for navigator type)
  navigator find(navigator first, navigator second, navigator third) const {
    return find(first.getNode(), second.getNode(), third.getNode());
  }

  /// Store cached value wrt. given node  
  void insert(node_type first, node_type second, node_type third, 
              node_type result) const {
    Cudd_Ref(result);
    cuddCacheInsert(base::operator()(), (ptruint)GENERIC_DD_TAG, 
                    first, second, third, result);
    Cudd_Deref(result);
  }
  /// Store cached value wrt. given node  
  void insert(navigator first, navigator second, navigator third, 
              navigator result) const {
    insert(first.getNode(), second.getNode(), third.getNode(), 
           result.getNode());  
  }

private:
  enum { GENERIC_DD_TAG =
         cudd_tag_number<count_tags<CacheType>::value>::value };
};

template <class CacheType, 
          unsigned ArgumentLength = CacheType::nargs,
          class ManagerType = typename CTypes::manager_base>
class CCacheManagement: 
  public CCacheManBase<ManagerType, CacheType, ArgumentLength> {
public:

  /// @name Get template parameters
  //@{
  typedef ManagerType manager_type;
  typedef CacheType cache_type;
  enum { nargs = ArgumentLength };
  //@}

  /// Name base type
  typedef CCacheManBase<manager_type, cache_type, nargs> base;

  /// Constructor and default constructor
  CCacheManagement(const manager_type& mgr):
    base(mgr) {}

  using base::find;
  using base::insert;
};


template <class CacheType, class ManagerType = typename CTypes::manager_base>
class CCommutativeCacheManagement: 
  public CCacheManagement<CacheType, 2, ManagerType> {
public:

  /// @name Get template parameters
  //@{
  typedef ManagerType manager_type;
  typedef CacheType cache_type;
  //@}

  /// Name base type
  typedef CCacheManagement<cache_type, 2, manager_type> base;

  /// Define node type
  typedef typename base::node_type node_type;
  typedef typename base::navigator navigator;

  /// Constructor and default constructor
  CCommutativeCacheManagement(const manager_type& mgr):
    base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type first, node_type second) const {
    if ( std::less<node_type>()(first, second) )
      return base::find(first, second);
    else
      return base::find(second, first);
  }

  /// Find cached value wrt. given node (for navigator type)
  navigator find(navigator first, navigator second) const {
    return find(first.getNode(), second.getNode());
  }


  /// Store cached value wrt. given node  
  void insert(node_type first, node_type second, node_type result) const {
    if ( std::less<node_type>()(first, second) )
      base::insert(first, second, result);
    else
      base::insert(second, first, result);   
  }

  /// Store cached value wrt. given node (for navigator type)
  void insert(navigator first, navigator second, navigator result) const {
    insert(first.getNode(), second.getNode(), result.getNode());
  }

};



/// temporarily


template <class CacheType>
class CCacheManBase<CCuddInterface, CacheType, 1> :
  protected mycudd_manager_storage {

public:
  /// Set base type
  typedef mycudd_manager_storage base;

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type node) const {
    return cuddCacheLookup1Zdd(base::operator()(), cache_dummy, node);
  }

  /// Find cached value wrt. given node (for navigator type)
  navigator find(navigator node) const { return find(node.getNode()); }

  /// Store cached value wrt. given node  
  void insert(node_type node, node_type result) const {
    Cudd_Ref(result);
    cuddCacheInsert1(base::operator()(), cache_dummy, node, result);
    Cudd_Deref(result);
  }

  /// Store cached value wrt. given node  
  void insert(navigator node, navigator result) const {
    insert(node.getNode(), result.getNode());
  }

private:
  /// Define unique static function, as marker for Cudd cache
  static node_type cache_dummy(internal_manager_type, node_type){
    return NULL;
  }
};




template <class CacheType>
class CCacheManBase<CCuddInterface, CacheType, 2> :
  protected mycudd_manager_storage {

public:

  /// Set base type
  typedef mycudd_manager_storage base;

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type first, node_type second) const {
    return cuddCacheLookup2Zdd(base::operator()(), cache_dummy, first, second);
  }
  navigator find(navigator first, navigator second) const {
    return find(first.getNode(), second.getNode());
  }

  /// Store cached value wrt. given node  
  void insert(node_type first, node_type second, node_type result) const {
    Cudd_Ref(result);
    cuddCacheInsert2(base::operator()(), cache_dummy, first, second, result);
    Cudd_Deref(result);
  }

  /// Store cached value wrt. given node (for navigator type)
  void insert(navigator first, navigator second, navigator result) const {
    insert(first.getNode(), second.getNode(), result.getNode());
  }
private:
  /// Define unique static function, as marker for Cudd cache
  static node_type cache_dummy(internal_manager_type, node_type, node_type){
    return NULL;
}
};


template <class CacheType>
class CCacheManBase<CCuddInterface, CacheType, 3> :
  protected mycudd_manager_storage {

public:

  /// Set base type
  typedef mycudd_manager_storage base;

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type first, node_type second, node_type third) const {
    return cuddCacheLookupZdd(base::operator()(), (ptruint)GENERIC_DD_TAG, 
                              first, second, third);
  }

  navigator find(navigator first, navigator second, navigator third) const {
    return find(first.getNode(), second.getNode(), third.getNode());
  }

  /// Store cached value wrt. given node  
  void insert(node_type first, node_type second, node_type third, 
              node_type result) const {
    Cudd_Ref(result);
    cuddCacheInsert(base::operator()(), (ptruint)GENERIC_DD_TAG, 
                    first, second, third, result);
    Cudd_Deref(result);
  }
  /// Store cached value wrt. given node  
  void insert(navigator first, navigator second, navigator third, 
              navigator result) const {
    insert(first.getNode(), second.getNode(), third.getNode(), 
           result.getNode());
  }

private:
  enum { GENERIC_DD_TAG =
         cudd_tag_number<count_tags<CacheType>::value>::value };
};

END_NAMESPACE_PBORI

#endif
