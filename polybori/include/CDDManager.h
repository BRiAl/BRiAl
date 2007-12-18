// -*- c++ -*-
//*****************************************************************************
/** @file CDDManager.h
 *
 * @author Alexander Dreyer
 * @date  2006-03-20
 *
 * This file define specializations for the CDDManager<> template class, which
 * allow unified access to various binary decision diagram implementations.
 *
 * @note Currently only interface to Cudd's manager is available.
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
 * Revision 1.19  2007/12/18 08:28:03  dreyer
 * CHANGE: minor changes
 *
 * Revision 1.18  2007/11/06 15:03:34  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.17  2007/07/18 15:46:14  dreyer
 * CHANGE: added documentation
 *
 * Revision 1.16  2007/07/06 14:04:21  dreyer
 * ADD: newly written C++_interface for Cudd
 *
 * Revision 1.15  2007/05/22 10:33:53  bricken
 * + pbori cache macros to cache opts file
 *
 * Revision 1.14  2007/05/14 08:28:23  dreyer
 * ADD: compiler-switches for overwriting cache defaults
 *
 * Revision 1.13  2006/10/24 08:44:04  dreyer
 * CHANGE: Added CVariableNames; variable names handled by OrderedManager
 *
 * Revision 1.12  2006/10/23 16:05:54  dreyer
 * ADD: BoolePolyRing::set/get(Ring)VariableName()
 *
 * Revision 1.11  2006/10/04 12:49:04  dreyer
 * FIX: Persistent variables need reference of new manager wrapper
 *
 * Revision 1.10  2006/10/02 09:28:37  dreyer
 * ADD BoolePolyRing::changeOrdering and infrastructure
 *
 * Revision 1.9  2006/09/20 14:30:33  dreyer
 * ADD: persistentVariable(idx)
 *
 * Revision 1.8  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.7  2006/08/09 12:52:31  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * Revision 1.6  2006/04/11 09:26:57  dreyer
 * ADD  BoolePolyRing::printInfo();
 *
 * Revision 1.5  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.4  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.3  2006/03/24 15:02:44  dreyer
 * ADD: Reference to manager_type can also be used for CDDManager<> -nterface
 * ADD: lead(), (n)usedVariables(), lmDeg() implemented in BoolePolynomial
 *
 * Revision 1.2  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.1  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; 
 * ring uses shared_ptr now
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef CDDManager_h_
#define CDDManager_h_
#include "cacheopts.h"
// load basic definitions
#include "pbori_defs.h"
#include "pbori_traits.h"

// get decision diagram definitions.
#include "CDDInterface.h"

#include "CCuddInterface.h"

// get standard map functionality
#include <map>


#ifndef PBORI_UNIQUE_SLOTS
#  define PBORI_UNIQUE_SLOTS CUDD_UNIQUE_SLOTS // initial size of subtables
#endif

#ifndef PBORI_CACHE_SLOTS
#  define PBORI_CACHE_SLOTS CUDD_CACHE_SLOTS   // default size of the cache
#endif

#ifndef PBORI_MAX_MEMORY
#  define PBORI_MAX_MEMORY 0    // target maximum memory occupation
                                // if PBORI_MAX_MEMORY == 0 then
                                // guess based on the available memory  
#endif


BEGIN_NAMESPACE_PBORI


inline ZDD
fetch_diagram(const Cudd& mgr, const ZDD& rhs) {
  return ZDD(&const_cast<Cudd&>(mgr), rhs.getNode());
}

template <class MgrType, class DDType>
inline const DDType&
fetch_diagram(const MgrType& mgr, const DDType& rhs) {
  return rhs;
}

inline Cudd&
fetch_manager(const Cudd& mgr) {
  return const_cast<Cudd&>(mgr);
}

template <class MgrType>
inline const MgrType&
fetch_manager(const MgrType& mgr) {
  return mgr;
}


/** @class CDDManagerBase<CuddLikeManType, StorageType>
 *
 * @brief this specialization of the template class CDDManager gives an
 * interface to Cudd-like ZDD manager (StorageType = CuddLikeManType) or a
 * reference of the latter (StorageType = CuddLikeManType&).
 *
 * @note Currently CuddLikeManType may be Cudd or CCuddInterface
 * @todo This may need clean up and/or generalization
 **/
template <class CuddLikeManType, class StorageType>
class CDDManagerBase {
 public:
  
  /// Interfacing Cudd's or CCuddInterces's zero-suppressed decision diagram type
  typedef CuddLikeManType interfaced_type;

  /// Storing interfaced_type or reference of it
  typedef StorageType interfaced_store;
 
  /// Generic access to type of *this
  typedef CDDManagerBase<interfaced_type, interfaced_store> self;

  /// Define size type
  typedef CTypes::size_type size_type;

  /// Define index type
  typedef CTypes::idx_type idx_type;

  /// Define raw type for decision diagrams
  typedef typename manager_traits<interfaced_type>::dd_base dd_base;
  
  /// Define type for decision diagrams
  typedef CDDInterface<dd_base> dd_type;

  /// Define type for caching persistent variables
  typedef std::map<idx_type, dd_base> persistent_cache_type;

  /// Default constructor
  CDDManagerBase(size_type nvars = 0,
                 size_type numSlots = PBORI_UNIQUE_SLOTS,
                 size_type cacheSize = PBORI_CACHE_SLOTS,
                 unsigned long maxMemory = PBORI_MAX_MEMORY): 
    m_interfaced(0, nvars, numSlots, cacheSize, maxMemory),
    m_variables() {  }
  
  /// Copy constructor
  CDDManagerBase(const self& rhs): 
    m_interfaced(rhs.m_interfaced), m_variables() {
    
    typename persistent_cache_type::const_iterator start(rhs.m_variables.begin()),
      finish(rhs.m_variables.end());

    // explicit copy, since pointer to manager in dd_base has to be consistent
    while (start != finish) {
      m_variables[start->first] = fetchDiagram(start->second);
      ++start;
    }
  }

  /// Constructor from given ring
  CDDManagerBase(const interfaced_type& rhs): 
    m_interfaced(fetch_manager(rhs)), m_variables() {  }

  /// Extract manager from given decision diagram
  CDDManagerBase(const dd_type& dd): 
    m_interfaced(dd.manager()), m_variables() { }

  /// Destructor
  ~CDDManagerBase() { }

  /// Get decision diagram, from the same manager, but different wrapper 
  dd_base fetchDiagram(const dd_base& rhs) const {
    return fetch_diagram(manager(), rhs);
  }

  /// Access nvar-th managed variable
  dd_base ddVariable(idx_type nvar) const {  
    return manager().zddVar(nvar); 
  }

  /// Access nvar-th managed variable
  dd_base variable(idx_type nvar) const {  
    return blank().change(nvar); 
  }

  /// Access nvar-th managed variable
  dd_base persistentVariable(idx_type nvar) {
    typename persistent_cache_type::iterator iter = m_variables.find(nvar);

    if (iter != m_variables.end())
      return (*iter).second;
    else
      return m_variables[nvar] = variable(nvar);
  }

  /// Get number of managed variables
  size_type nVariables() const { 
    return Cudd_ReadZddSize(manager().getManager()); 
  }

  /// Get empty decision diagram 
  /// @note Corresponds to the empty set (or the zero polynomial)
  dd_type empty() const { return manager().zddZero(); }

  /// Get decision diagram with all variables negated
  /// @note Set containing the empty set only (or the constant polynomial one)
  dd_type blank() const { return manager().zddOne(nVariables()); }

  /// Casting operator to interfaced type
  operator interfaced_type&() { return m_interfaced; }

  /// Constant casting operator to interfaced type
  operator const interfaced_type&() const { return m_interfaced; }

  /// Get interfaced type
  interfaced_type& manager() { return m_interfaced; }

  /// Get interfaced type
  const interfaced_type& manager() const { return m_interfaced; }

  /// Print out statistics and settings for a decision diagram manager
  void printInfo() const { manager().info(); }

private:
  /// Actual decision diagram manager
  mutable interfaced_store m_interfaced;

  /// Store values of Boolean variables
  persistent_cache_type m_variables;
};

/** @class CDDManager<Cudd&>
 *
 * @brief this specialization of the template class CDDManager gives an
 * interface to Cudd's ZDD manager reference.
 *
 **/

template<>
class CDDManager<Cudd&>:
  public CDDManagerBase<Cudd, Cudd&> { 

public:

  typedef Cudd manager_type;
  typedef Cudd& storage_type;
  typedef CDDManagerBase<manager_type, storage_type> base;
  typedef CDDManager<storage_type> self;

  /// Constructor reference of given manager
  CDDManager(manager_type& rhs): 
    base(rhs) { }

  /// Extract manager from given decision diagram
  CDDManager(const dd_type& dd): 
    base(dd) { }

  /// Copy constructor
  CDDManager(const self& rhs): 
    base(rhs) { }

  // Destructor
  ~CDDManager() { }

};

/** @class CDDManager<Cudd>
 *
 * @brief this specialization of the template class CDDManager gives an
 * interface to Cudd's ZDD manager.
 *
 **/


template<>
class CDDManager<Cudd>:
  public CDDManagerBase<Cudd, Cudd> { 

public:

  typedef Cudd manager_type;
  typedef Cudd storage_type;
  typedef CDDManagerBase<manager_type, storage_type> base;
  typedef CDDManager<storage_type> self;

  /// Construct new decision diagramm manager
  CDDManager(size_type nvars = 0): 
    base(nvars) { }

  // Destructor
  ~CDDManager() { }

};


/** @class CDDManager<CCuddInterface&>
 *
 * @brief this specialization of the template class CDDManager gives an
 * interface to CCuddInterface's ZDD manager reference.
 *
 **/

template<>
class CDDManager<CCuddInterface&>:
  public CDDManagerBase<CCuddInterface, const CCuddInterface&> { 

public:

  typedef CCuddInterface manager_type;
  typedef const CCuddInterface& storage_type;
  typedef CDDManagerBase<manager_type, storage_type> base;
  typedef CDDManager<CCuddInterface&> self;

  /// Constructor reference of given manager
  CDDManager(const manager_type& rhs): 
    base(rhs) { }

  /// Extract manager from given decision diagram
  CDDManager(const dd_type& dd): 
    base(dd) { }

  /// Copy constructor
  CDDManager(const self& rhs): 
    base(rhs) { }

  // Destructor
  ~CDDManager() { }

};


/** @class CDDManager<CCuddInterface>
 *
 * @brief this specialization of the template class CDDManager gives an
 * interface to CCuddInterface's ZDD manager.
 *
 **/

template<>
class CDDManager<CCuddInterface>:
  public CDDManagerBase<CCuddInterface, CCuddInterface> { 

public:

  typedef CCuddInterface manager_type;
  typedef CCuddInterface storage_type;
  typedef CDDManagerBase<manager_type, storage_type> base;
  typedef CDDManager<storage_type> self;

  /// Construct new decision diagramm manager
  CDDManager(size_type nvars = 0): 
    base(nvars) { }

  // Destructor
  ~CDDManager() { }

};
END_NAMESPACE_PBORI

#endif // of #ifndef CDDManager_h_
