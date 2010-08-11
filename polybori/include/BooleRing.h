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
  //#include "CDDManager.h"
#include "CCuddCore.h"

#include <boost/intrusive_ptr.hpp>

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

  /// @name adopt global type definitions
  //@{
  typedef CTypes::ordercode_type ordercode_type;
  typedef CTypes::dd_base dd_base;
  typedef CTypes::dd_type dd_type;
  typedef CTypes::vartext_type vartext_type;
  //@}

  /// Type of actual data
  typedef CCuddCore core_type;
  typedef core_type::const_varname_reference const_varname_reference;
  /// Smart pointer to core
  typedef boost::intrusive_ptr<core_type> core_ptr;

  /// Type for handling mterm orderings
  typedef core_type::order_type order_type;

  /// Smart pointer for handling mterm orderings
  typedef core_type::order_ptr order_ptr;

  /// Reference for handling mterm orderings
  typedef order_type& order_reference;

  /// Explicitely mention ordercodes' enumeration
  using CTypes::orderenums_type::ordercodes;

   /// Constructor
  BooleRing();
  BooleRing(size_type nvars);
   /// Constructor for @em nvars variables
  BooleRing(size_type nvars,
            const order_ptr& order):
    p_core(new core_type(nvars, order)) {}

  /// Copy constructor (cheap)
  BooleRing(const self& rhs):  p_core(rhs.p_core) {}

protected:
public:
  /// Support for shallow copy (clone)
  BooleRing(const core_ptr& rhs):  p_core(rhs) {}

public:
  /// destructor
  ~BooleRing() {}

//   /// Access to decision diagram manager
//   manager_type& manager() {  return m_mgr; }

//   /// Constant access to decision diagram manager
//   const manager_type& manager() const {  return m_mgr; }

  /// Access nvar-th variable of decision diagram manager
  //  dd_type ddVariable(idx_type nvar) const { return m_mgr.ddVariable(nvar); }

  /// Access nvar-th ring variable
  dd_type variable(idx_type nvar) const;// inlined in BooleSet.h

  /// Access nvar-th ring variable
  dd_type persistentVariable(idx_type nvar) const; // inlined in BooleSet.h

  /// Get empty decision diagram 
  dd_type zero() const; // inlined in BooleSet.h

  /// Get decision diagram with all variables negated
  dd_type one() const; // inlined in BooleSet.h


  /// Get constant one or zero
  dd_type constant(bool is_one) const; // inlined in BooleSet.h

  /// Get number of ring variables
  size_type nVariables() const { return p_core->m_mgr.nVariables(); }

  /// Get name of variable with index idx
  vartext_type getVariableName(idx_type idx) const {
    return p_core->m_names[idx];
  }
  /// Get name of variable with index idx
  vartext_type getName(idx_type idx) const{
    return getVariableName(idx);
  }

  /// Set name of variable with index idx
  void setVariableName(idx_type idx, vartext_type varname) {
    p_core->m_names.set(idx, varname);
  }

  /// Clears the function cache
  void clearCache() { p_core->m_mgr.cacheFlush(); }

  /// Print out statistics and settings for current ring
  void printInfo() {  return p_core->m_mgr.info(); }

  /// Generate ring based on the same manager
  self clone() const {
    return self(new core_type(*p_core));
  }

  /// Get unique identifier for manager of *this
  hash_type hash() const { 
    return static_cast<hash_type>(reinterpret_cast<std::ptrdiff_t
                                  >(p_core->m_mgr.getManager())); 
  }

  /// Access ordering of *this
  order_reference ordering() const { 
    return *(p_core->pOrder); 
  }

  /// Access ordering of *this
  //  order_ptr pOrdering() const { return m_mgr.manager().managerCore()->pOrder; }


  core_ptr core() const {return p_core;};
  DdManager* getManager() const {
    return p_core->m_mgr.getManager();
  }

protected: 

  core_ptr p_core;
};


inline boost::intrusive_ptr<CCuddCore>
get_mgr_core(const BooleRing& rhs) { 
  return rhs.core();
}
END_NAMESPACE_PBORI
#endif
