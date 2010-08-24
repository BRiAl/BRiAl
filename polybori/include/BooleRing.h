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
**/
//*****************************************************************************

// load PolyBoRi settings
# include "pbori_defs.h"

// include basic decision diagram manager interface 
  //#include "CDDManager.h"
#include "CCuddCore.h"
#include "PBoRiError.h"
#include <boost/intrusive_ptr.hpp>

#ifndef BooleRing_h_
#define BooleRing_h_

BEGIN_NAMESPACE_PBORI

/** @class BooleRing
 * @brief This class is just a wrapper for reinterpreting decicion diagram
 * managers as Boolean polynomial rings.
 *
 **/
template <class DiagramType, class RingType>
class CCuddDDBase;

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

  typedef DdManager mgr_type;
  /// Explicitely mention ordercodes' enumeration
  using CTypes::orderenums_type::ordercodes;

protected:

  template <class, class> class CCuddDDBase;
  /// Support for shallow copy (clone)
  /// @note May generate invalid ring, hence @c protected 
  BooleRing(const core_ptr& rhs):  p_core(rhs) {}

public:
   /// Constructor for @em nvars variables (and lp ordering)
  BooleRing(size_type nvars);

  /// Constructor for @em nvars variables (and given pointer to ordering)
  BooleRing(size_type nvars,
            const order_ptr& order):
    p_core(new core_type(nvars, order)) {}

  /// Copy constructor (cheap)
  BooleRing(const self& rhs):  p_core(rhs.p_core) {}

  /// Destructor
  ~BooleRing() {}


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

  /// Get unique identifier for manager of *this
  hash_type hash() const { 
    return static_cast<hash_type>(reinterpret_cast<std::ptrdiff_t
                                  >(getManager())); 
  }

  /// Access ordering of *this
  order_reference ordering() const { 
    return *(p_core->pOrder); 
  }

  /// Get plain decision diagram manager
  mgr_type* getManager() const {
    return p_core->m_mgr.getManager();
  }

  /// Deep copy of @c *this
  self clone() const {  return self(new core_type(*p_core)); }
protected:

  /// Access to actual data (via ->)
  core_ptr core() const {return p_core;};

  /// Smart pointer to actual data
  core_ptr p_core;
};


END_NAMESPACE_PBORI
#endif
