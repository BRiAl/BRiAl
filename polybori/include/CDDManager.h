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
 * Revision 1.2  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.1  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; ring uses shared_ptr now
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


/** @class CDDManager<Cudd>
 *
 * @brief this specialization of the template class CDDManager gives an
 * interface to Cudd's ZDD manager.
 *
 **/

template<>
class CDDManager<Cudd> {
 public:
  
  /// Interfacing Cudd's zero-suppressed decision diagram type
  typedef Cudd interfaced_type;
  
  /// Generic access to type of *this
  typedef CDDManager<interfaced_type> self;

  /// Define size type
  typedef CTypes::size_type size_type;

  /// Define index type
  typedef CTypes::idx_type idx_type;

  /// Define raw type for decision diagrams
  typedef ZDD dd_base;

  /// Default constructor
  CDDManager(size_type nvars_ = 0): 
    m_interfaced(0, nvars_) { }

  /// Copy constructor
  CDDManager(const self& rhs): 
    m_interfaced(rhs.m_interfaced) { }

  /// Constructor from given ring
  CDDManager(interfaced_type& rhs): 
    m_interfaced(rhs) { }

  /// Destructor
  ~CDDManager() { }

  /// Access nvar-th managed variable
  dd_base variable(idx_type nvar) const {  return m_interfaced.zddVar(nvar); }

  /// Get number of managed variables
  size_type nVariables() const { 
    return Cudd_ReadZddSize(m_interfaced.getManager()); 
  }

  /// Get empty decision diagram 
  dd_base zeroDD() const { return m_interfaced.zddZero(); }

  /// Get decision diagram with all variables
  dd_base oneDD() const { return m_interfaced.zddOne(nVariables()); }

  /// Casting operator to interfaced type
  operator interfaced_type&() { return m_interfaced; }

  /// Constant casting operator to interfaced type
  operator const interfaced_type&() const { return m_interfaced; }

private:
  /// Actual decision diagram manager
  mutable interfaced_type m_interfaced;
};


END_NAMESPACE_PBORI

#endif // of #ifndef CDDManager_h_
