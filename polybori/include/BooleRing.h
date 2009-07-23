// -*- c++ -*-
//*****************************************************************************
/** @file BooleRing.h 
 *
 * @author Alexander Dreyer
 * @date 2008-03-02
 *
 * This file the class BooleRing, where carries the definition of a
 * polynomial ring over Booleans.
 *
 * @par Copyright:
 *   (c) 2008 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.4  2009/07/23 19:41:06  dreyer
 * ADD: BooleRing::hash
 *
 * Revision 1.3  2009/06/22 07:58:42  dreyer
 * ADD: cloning of rings
 *
 * Revision 1.2  2008/03/03 18:07:19  dreyer
 * Fix: missing things in Python-interface
 *
 * Revision 1.1  2008/03/02 23:45:34  dreyer
 * CHANGED: added contructors for given ring
 *
 * @endverbatim
**/
//*****************************************************************************

// load PolyBoRi settings
# include "pbori_defs.h"

// include basic decision diagram manager interface 
#include "CDDManager.h"


#ifndef BooleRing_h_
#define BooleRing_h_

BEGIN_NAMESPACE_PBORI

/** @class BooleRing
 * @brief This class is just a wrapper for reinterpreting decicion diagram
 * managers as Boolean polynomial rings.
 *
 **/
class BooleRing: 
  public CTypes::orderenums_type, public CTypes::compenums_type, 
  public CTypes::auxtypes_type {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// generic access to current type
  typedef BooleRing self;

  /// generic access to base type
  typedef CTypes::orderenums_type base;

  /// @name adopt global type definitions
  //@{
  typedef CTypes::ordercode_type ordercode_type;
  typedef CTypes::manager_type manager_type;
  typedef CTypes::manager_reference manager_reference;
  typedef CTypes::manager_ptr manager_ptr;
  typedef CTypes::dd_type dd_type;
  typedef CTypes::vartext_type vartext_type;
  //@}

  /// Explicitely mention ordercodes' enumeration
  using base::ordercodes;

  /// Constructor for @em nvars variables
  BooleRing(size_type nvars = 100):
    m_mgr(nvars) {}

  BooleRing(const manager_type& mgr):
    m_mgr(mgr) {}

  /// destructor
  ~BooleRing() {}

  /// Access to decision diagram manager
  manager_type& manager() {  return m_mgr; }

  /// Constant access to decision diagram manager
  const manager_type& manager() const {  return m_mgr; }

  /// Access nvar-th variable of decision diagram manager
  dd_type ddVariable(idx_type nvar) const { return m_mgr.ddVariable(nvar); }

  /// Access nvar-th ring variable
  dd_type variable(idx_type nvar) const { return m_mgr.variable(nvar); }

  /// Access nvar-th ring variable
  dd_type persistentVariable(idx_type nvar) const { 
    return m_mgr.persistentVariable(nvar); 
  }

  /// Get empty decision diagram 
  dd_type zero() const { return m_mgr.empty(); }

  /// Get decision diagram with all variables negated
  dd_type one() const { return m_mgr.blank(); }

  /// Get number of ring variables
  size_type nVariables() const { return m_mgr.nVariables(); }

  /// Get name of variable with index idx
  vartext_type getVariableName(idx_type idx){
    return m_mgr.getVariableName(idx);
  }

  /// Set name of variable with index idx
  void setVariableName(idx_type idx, vartext_type varname) {
    m_mgr.setVariableName(idx, varname);
  }

  /// Clears the function cache
  void clearCache() { cuddCacheFlush(m_mgr.manager().getManager()); }

  /// Print out statistics and settings for current ring
  void printInfo() {  return m_mgr.printInfo(); }

  /// Generate ring based on the same manager
  self clone() const {
    return self( (manager_type)CCuddCore::mgrcore_ptr(new
      CCuddCore(*m_mgr.manager().managerCore()))  );
  }

  /// Get unique identifier for *this
  hash_type hash() const { 
    return static_cast<hash_type>(reinterpret_cast<std::ptrdiff_t
                                  >(m_mgr.manager().getManager())); 
  }
protected: 
  /// Interprete @c m_mgr as structure of Boolean polynomial ring
  manager_type m_mgr;
};


END_NAMESPACE_PBORI
#endif
