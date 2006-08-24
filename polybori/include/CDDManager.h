// -*- c++ -*-
//*****************************************************************************
/** @file 
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

// load basic definitions
#include "pbori_defs.h"

// get decision diagram definitions.
#include "CDDInterface.h"

BEGIN_NAMESPACE_PBORI


/** @class CDDManagerBase<Cudd, StorageType>
 *
 * @brief this specialization of the template class CDDManager gives an
 * interface to Cudd's ZDD manager (StorageType = Cudd) or a reference of the
 * latter (StorageType = Cudd&).
 *
 **/


template <class ManType, class StorageType>
class CDDManagerBase;

template <class StorageType>
class CDDManagerBase<Cudd, StorageType> {
 public:
  
  /// Interfacing Cudd's zero-suppressed decision diagram type
  typedef Cudd interfaced_type;

  /// Storing interfaced_type or reference of it
  typedef StorageType interfaced_store;
 
  /// Generic access to type of *this
  typedef CDDManagerBase<interfaced_type, interfaced_store> self;

  /// Define size type
  typedef CTypes::size_type size_type;

  /// Define index type
  typedef CTypes::idx_type idx_type;

  /// Define raw type for decision diagrams
  typedef ZDD dd_base;

  /// Define type for decision diagrams
  typedef CDDInterface<dd_base> dd_type;

  /// Default constructor
   CDDManagerBase(size_type nvars = 0): 
     m_interfaced(0, nvars) { }

  /// Copy constructor
  CDDManagerBase(const self& rhs): 
    m_interfaced(rhs.m_interfaced) { }

  /// Constructor from given ring
  CDDManagerBase(interfaced_type& rhs): 
    m_interfaced(rhs) { }

  /// Extract manager from given decision diagram
  CDDManagerBase(const dd_type& dd): 
    m_interfaced(dd.manager()) { }

  /// Destructor
  ~CDDManagerBase() { }

  /// Access nvar-th managed variable
  dd_base ddVariable(idx_type nvar) const {  
    return m_interfaced.zddVar(nvar); 
  }

  /// Access nvar-th managed variable
  dd_base variable(idx_type nvar) const {  
    return blank().change(nvar); 
  }

  /// Get number of managed variables
  size_type nVariables() const { 
    return Cudd_ReadZddSize(m_interfaced.getManager()); 
  }

  /// Get empty decision diagram 
  /// @note Corresponds to the empty set (or the zero polynomial)
  dd_type empty() const { return m_interfaced.zddZero(); }

  /// Get decision diagram with all variables negated
  /// @note Set containing the empty set only (or the constant polynomial one)
  dd_type blank() const { return m_interfaced.zddOne(nVariables()); }

  /// Casting operator to interfaced type
  operator interfaced_type&() { return m_interfaced; }

  /// Constant casting operator to interfaced type
  operator const interfaced_type&() const { return m_interfaced; }

  /// Get interfaced type
  interfaced_type& manager() { return m_interfaced; }

  /// Get interfaced type
  const interfaced_type& manager() const { return m_interfaced; }

  /// Print out statistics and settings for a decision diagram manager
  void printInfo() const { m_interfaced.info(); }

private:
  /// Actual decision diagram manager
  mutable interfaced_store m_interfaced;
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
END_NAMESPACE_PBORI

#endif // of #ifndef CDDManager_h_
