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
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_cache_CCacheManagement_h_
#define polybori_cache_CCacheManagement_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// get DD navigation
#include <polybori/iterators/CCuddNavigator.h>

#include <polybori/ring/CCuddCore.h>
#include <boost/intrusive_ptr.hpp>
// get standard functionality
#include <functional>

BEGIN_NAMESPACE_PBORI

class CCacheTypes {

public:
  struct no_cache_tag { enum { nargs = 0 }; };
  struct unary_cache_tag { enum { nargs = 1 }; };
  struct binary_cache_tag { enum { nargs = 2 }; };
  struct ternary_cache_tag { enum { nargs = 3 }; };

  template <class TagType>
  struct lead_tag: public binary_cache_tag {};

  // user functions
  struct no_cache: public no_cache_tag { };
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
  struct zeros: public binary_cache_tag { };
  struct interpolate_smallest_lex: public binary_cache_tag { };
  
  struct include_divisors: public unary_cache_tag { };
  
  //struct mod_deg2_set: public binary_cache_tag { };
  typedef mod_varset mod_deg2_set;
  typedef mod_varset mod_mon_set;
  
  struct contained_deg2: public unary_cache_tag { };
  struct contained_variables: public unary_cache_tag { };

  struct map_every_x_to_x_plus_one: public unary_cache_tag { };

  struct lex_lead: public unary_cache_tag {};
  typedef lead_tag<dlex_tag> dlex_lead;
  typedef lead_tag<dp_asc_tag> dp_asc_lead;

  typedef lead_tag<block_dlex_tag> block_dlex_lead;
  typedef lead_tag<block_dp_asc_tag> block_dp_asc_lead;

  struct divisorsof_fixedpath: public ternary_cache_tag { };
  struct testwise_ternary: public ternary_cache_tag { };

  struct used_variables: public unary_cache_tag { };

  struct block_degree: public binary_cache_tag { };

 
  struct has_factor_x_plus_y: public ternary_cache_tag { };
  struct left_equals_right_x_branch_and_r_has_fac_x:
    public ternary_cache_tag { };

  struct graded_part: public binary_cache_tag { };
  struct mapping: public binary_cache_tag { };
  
  struct is_rewriteable: public binary_cache_tag{};
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

/** @class CCuddLikeMgrStorage
 *
 * This template defines how to used the Cudd-like decision diagram managers
 * like Cudd and CCuddInterface.
 **/
template <class MgrType>
class CCuddLikeMgrStorage {
public:
  /// Set  manager type
  typedef MgrType manager_type;

  /// Set type of Cudd's internal manager type
  typedef DdManager* internal_manager_type;

  /// Set type of Cudd's nodes
  typedef DdNode* node_type;

  /// Type of navigators
  typedef CCuddNavigator navigator;


  //  typedef CTypes::dd_base dd_base;
  //  typedef typename manager_type::mgrcore_ptr mgrcore_ptr;

  /// Type of Boolean rings
  typedef BoolePolyRing ring_type;

  /// Get high-level decision diagram type
  typedef typename ring_type::dd_type dd_type;

  /// Constructor
  CCuddLikeMgrStorage(const manager_type& mgr): 
    m_mgr(mgr) {}

//   CCuddLikeMgrStorage(const mgrcore_ptr& mgr): 
//     m_mgr(mgr) {}

  /// Accessing manager
  manager_type manager() const { return m_mgr; }

  /// Re-generate valid decision diagram from navigator
  dd_type generate(navigator navi) const {
    return dd_type(m_mgr, navi);
  }

  /// Get constant one
  dd_type one() const {
    return ring_type(m_mgr).one();//dd_type(m_mgr, DD_ONE(m_mgr->manager()));//manager().zddOne();
  }
  /// Get constant zero
  dd_type zero() const {
    return ring_type(m_mgr).zero();//dd_base(m_mgr, PBORI_PREFIX(Cudd_ReadZero)(m_mgr->manager()));//manager().zddZero();
  }

  ring_type ring() const { return ring_type(manager()); }
protected:
  /// Accessing Cudd-internal decision diagram manager
  internal_manager_type internalManager() const { 
    return m_mgr.getManager(); 
    //  return manager().getManager(); 
  }

private:
  /// Store (pointer) to internal manager
  manager_type m_mgr;
  //  typename manager_type::mgrcore_ptr  m_mgr;
};

/** @class CCacheManBase
 *
 * @brief This template forms the base for CCacheManagement. It implements
 * routines for finding and inserting results into the cache.
 *
 * Template arguments: ManagerType, CacheType, nArgs
 *
 * @note We have template specializations for nArgs = 1, 2, 3 corresponding to
 * unary, binary and ternary funtions to be cached. In addition, nArgs = 0 is
 * used, when no cache is used.
 **/
template <class ManagerType, class CacheType, unsigned ArgumentLength>
class CCacheManBase;

// Fixing base type for Cudd-Like type CCuddInterface
template <class CacheType, unsigned ArgumentLength>
struct pbori_base<CCacheManBase<CCuddInterface, CacheType, ArgumentLength> > {

  typedef CCuddLikeMgrStorage<CCuddInterface> type;
};


// Fixing base type for Cudd-Like type CCuddInterface
template <class CacheType, unsigned ArgumentLength>
struct pbori_base<CCacheManBase<BoolePolyRing, CacheType, ArgumentLength> > {

  typedef CCuddLikeMgrStorage<BoolePolyRing> type;
};

// Fixing base type for Cudd-Like type CCuddInterface
template <class CacheType, unsigned ArgumentLength>
struct pbori_base<CCacheManBase<boost::intrusive_ptr<CCuddCore>, CacheType, ArgumentLength> > {

  typedef CCuddLikeMgrStorage<boost::intrusive_ptr<CCuddCore> > type;
};
// Dummy variant for generating empty cache managers, e.g. for using generate()
template <class ManagerType, class CacheType>
class CCacheManBase<ManagerType, CacheType, 0> :
  public pbori_base<CCacheManBase<ManagerType, CacheType, 0> >::type {

public:
  /// Set this type
  typedef CCacheManBase<ManagerType, CacheType, 0> self;

  /// Set base type
  typedef typename pbori_base<self>::type base;

  /// @name Extracting inherited types
  //@{
  typedef typename base::node_type node_type;
  typedef typename base::navigator navigator;
  typedef typename base::manager_type manager_type;
  //@}

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// @name Dummy functions
  //@{
  navigator find(navigator, ...) const { return navigator(); }
  node_type find(node_type, ...) const { return NULL; }
  void insert(...) const {}
  //@}
};


// Variant for unary functions
template <class ManagerType, class CacheType>
class CCacheManBase<ManagerType, CacheType, 1> :
  public pbori_base<CCacheManBase<ManagerType, CacheType, 1> >::type {

public:
  /// Set this type
  typedef CCacheManBase<ManagerType, CacheType, 1> self;

  /// Set base type
  typedef typename pbori_base<self>::type base;

  /// @name Extracting inherited types
  //@{
  typedef typename base::node_type node_type;
  typedef typename base::navigator navigator;
  typedef typename base::manager_type manager_type;
  //@}

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type node) const {
    return PBORI_PREFIX(cuddCacheLookup1Zdd)(internalManager(), cache_dummy, node);
  }

  /// Find cached value wrt. given node (for navigator type)
  navigator find(navigator node) const { 
    return explicit_navigator_cast(find(node.getNode())); 
  }

  /// Store cached value wrt. given node  
  void insert(node_type node, node_type result) const {
    PBORI_PREFIX(Cudd_Ref)(result);
    PBORI_PREFIX(cuddCacheInsert1)(internalManager(), cache_dummy, node, result);
    PBORI_PREFIX(Cudd_Deref)(result);
  }

  /// Store cached value wrt. given node  
  void insert(navigator node, navigator result) const {
    insert(node.getNode(), result.getNode());
  }

protected:
  /// Access manager used for caching
  using base::internalManager;

private:
  /// Define unique static function, as marker for Cudd cache
  static node_type cache_dummy(typename base::internal_manager_type,node_type){ // LCOV_EXCL_LINE
    return NULL; // LCOV_EXCL_LINE
  }
};

// Variant for binary functions
template <class ManagerType, class CacheType>
class CCacheManBase<ManagerType, CacheType, 2> :
  public pbori_base<CCacheManBase<ManagerType, CacheType, 2> >::type {

public:
  /// Set this type
  typedef CCacheManBase<ManagerType, CacheType, 2> self;

  /// Set base type
  typedef typename pbori_base<self>::type base;

  /// @name Extracting inherited types
  //@{
  typedef typename base::node_type node_type;
  typedef typename base::navigator navigator;
  typedef typename base::manager_type manager_type;
  //@}

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type first, node_type second) const {
    return PBORI_PREFIX(cuddCacheLookup2Zdd)(internalManager(), cache_dummy, first, second);
  }
  /// Find cached value wrt. given node (for navigator type)
  navigator find(navigator first, navigator second) const { 
    return explicit_navigator_cast(find(first.getNode(), second.getNode()));
  }

  /// Store cached value wrt. given node  
  void insert(node_type first, node_type second, node_type result) const {
    PBORI_PREFIX(Cudd_Ref)(result);
    PBORI_PREFIX(cuddCacheInsert2)(internalManager(), cache_dummy, first, second, result);
    PBORI_PREFIX(Cudd_Deref)(result);
  }

  /// Store cached value wrt. given node  
  void insert(navigator first, navigator second, navigator result) const {
    insert(first.getNode(), second.getNode(), result.getNode());
  }

protected:
  /// Access manager used for caching
  using base::internalManager;

private:
  /// Define unique static function, as marker for Cudd cache
  static node_type cache_dummy(typename base::internal_manager_type,  // LCOV_EXCL_LINE
                               node_type, node_type){ // LCOV_EXCL_LINE
    return NULL;  // LCOV_EXCL_LINE
  }
};

// Variant for ternary functions
template <class ManagerType, class CacheType>
class CCacheManBase<ManagerType, CacheType, 3> :
  public pbori_base<CCacheManBase<ManagerType, CacheType, 3> >::type {

public:
  /// Set this type
  typedef CCacheManBase<ManagerType, CacheType, 3> self;

  /// Set base type
  typedef typename pbori_base<self>::type base;

  /// @name Extracting inherited types
  //@{
  typedef typename base::node_type node_type;
  typedef typename base::navigator navigator;
  typedef typename base::manager_type manager_type;
  //@}

  /// Constructor
  CCacheManBase(const manager_type& mgr): base(mgr) {}

  /// Find cached value wrt. given node
  node_type find(node_type first, node_type second, node_type third) const {
    return PBORI_PREFIX(cuddCacheLookupZdd)(internalManager(), (ptruint)GENERIC_DD_TAG, 
                              first, second, third);
  }

  /// Find cached value wrt. given node (for navigator type)
  navigator find(navigator first, navigator second, navigator third) const {
    return explicit_navigator_cast(find(first.getNode(), second.getNode(),
                                        third.getNode())); 
  }

  /// Store cached value wrt. given node  
  void insert(node_type first, node_type second, node_type third, 
              node_type result) const {
    PBORI_PREFIX(Cudd_Ref)(result);
    PBORI_PREFIX(cuddCacheInsert)(internalManager(), (ptruint)GENERIC_DD_TAG, 
                    first, second, third, result);
    PBORI_PREFIX(Cudd_Deref)(result);
  }
  /// Store cached value wrt. given node  
  void insert(navigator first, navigator second, navigator third, 
              navigator result) const {
    insert(first.getNode(), second.getNode(), third.getNode(), 
           result.getNode());  
  }

protected:
  /// Access manager used for caching
  using base::internalManager;

private:
  enum { GENERIC_DD_TAG =
         cudd_tag_number<count_tags<CacheType>::value>::value };
};

/** @class CCacheManagement
 *
 * @brief This template class forms the base for CCommutativeCacheManagement and
 * CacheManager. It is an interface defining find and insert on decision diagram
 * cache.
 *
 * It it mainly a wrapper for the template specialization of CCacheManBase,
 * which allows a simple call of CCacheManagement<CacheType>, if CacheType is
 * inherited from unary_cache_tag, binary_cache_tag, and ternary_cache_tag.
 * It also fixes the ManagerType to CTypes::manager_base.
 *
 **/
template <class ManagerType, class CacheType, 
          unsigned ArgumentLength = CacheType::nargs>
class CCacheManagement: 
  public CCacheManBase<ManagerType,
                       CacheType, ArgumentLength> {
public:

  /// @name Get template parameters and global types
  //@{
  typedef ManagerType manager_type;
  typedef typename manager_type::deg_type deg_type;
  typedef typename manager_type::size_type size_type;
  typedef typename manager_type::idx_type idx_type;
  typedef CacheType cache_type;
  enum { nargs = ArgumentLength };
  //@}

  /// Name base type
  typedef CCacheManBase<manager_type, cache_type, nargs> base;

  /// Extracting inherited node type
  typedef typename base::node_type node_type;

  /// Constructor and default constructor
  CCacheManagement(const manager_type& mgr):
    base(mgr) {}

  using base::find;
  using base::insert;
};

/** @class CCommutativeCacheManagement
 * This is the variant for cache management of binary commutative functions.
 **/
template <class ManagerType, class CacheType>
class CCommutativeCacheManagement: 
  public CCacheManagement<ManagerType, CacheType, 2> {

public:
  /// @name Get template parameters
  //@{
  typedef ManagerType manager_type;
  typedef CacheType cache_type;
  //@}

  /// Name base type
  typedef CCacheManagement<manager_type, cache_type, 2> base;

  /// Define node type
  typedef typename base::node_type node_type;
  typedef typename base::navigator navigator;

  /// Constructor and default constructor
  CCommutativeCacheManagement(const typename base::manager_type& mgr):
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
    return explicit_navigator_cast(find(first.getNode(), second.getNode()));
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

END_NAMESPACE_PBORI

#endif
