// -*- c++ -*-
//*****************************************************************************
/** @file BoolePolyRing.h 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file the class BoolePolyRing, where carries the definition of a
 * polynomial ring over Booleans.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
 *
**/
//*****************************************************************************

// load PolyBoRi settings
# include "pbori_defs.h"

// include basic decision diagram manager interface 
  //#include "COrderingBase.h"

#include "BooleRing.h"

  //#include "CDDOperations.h"
  // temporarily for work around


#include <list>

#ifndef BoolePolyRing_h_
#define BoolePolyRing_h_


BEGIN_NAMESPACE_PBORI


class BooleExponent;
class BooleVariable;
class BoolePolynomial;
class BooleMonomial;
class BooleSet;

/** @class BoolePolyRing
 * @brief This class adds functionality to BooleRing.
 *
 *
 **/
class BoolePolyRing: 
  public BooleRing {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// generic access to current type
  typedef BoolePolyRing self;

  /// generic access to base type
  typedef BooleRing base;

  /// define exponent type
  typedef BooleExponent exp_type;

  /// set monomial type
  typedef BooleMonomial monom_type;

  /// set variables type
  typedef BooleVariable var_type;

  /// set decision diagram type
  typedef BooleSet dd_type;

  /// set polynomial type
  typedef BoolePolynomial poly_type;

  /// Type for block indices
  typedef std::vector<idx_type> block_idx_type;

  /// Type for block iterators
  typedef block_idx_type::const_iterator block_iterator;

  /// Type for handling mterm orderings
  typedef COrderingBase order_type;
  
  /// Smart pointer for handling mterm orderings
  typedef boost::shared_ptr<order_type> order_ptr;

  /// Reference for handling mterm orderings
  typedef order_type& order_reference;

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------

  /// Explicitely mention ordercodes' enumeration
  using base::ordercodes;

  /// Default constructor
  BoolePolyRing();//: base(core_ptr()) { }

  /// Constructor for @em nvars variables
  explicit BoolePolyRing(size_type nvars, 
                         ordercode_type order = lp,
                         bool_type make_active = true);

  /// Destructor
  ~BoolePolyRing() {}

  /// Make @c *this global
  void activate();

  /// Change order of current ring
  void changeOrdering(ordercode_type);

  /// Construct ring with similiar properties (deep copy)
  self clone() const { return self(base::clone()); }

  /// Copy constructor (cheap reference-counted copy)
  BoolePolyRing(const self& rhs): base(rhs) { }

  /// Map polynomial to this ring, if possible
  poly_type coerce(const poly_type& rhs) const;

  /// Map monomial to this ring, if possible
  monom_type coerce(const monom_type& rhs) const;

  /// Map variable to this ring
  var_type coerce(const var_type& rhs) const;

  /// Access nvar-th ring variable
  // var_type variable(idx_type nvar) const;  // inlined in BooleVariable.h

  /// Access nvar-th ring variable as diagram
  dd_type variable(idx_type nvar) const;

  /// Get empty decision diagram 
  dd_type zero() const; // inlined below

  /// Get decision diagram with all variables negated
  dd_type one() const; // inlined below

  /// Get constant one or zero
  dd_type constant(bool is_one) const; // inlined below


  explicit BoolePolyRing(const base& rhs): base(rhs) { }
};


// temporarily here!
END_NAMESPACE_PBORI

#include "BooleSet.h"

BEGIN_NAMESPACE_PBORI

 /// Get empty decision diagram 
inline BoolePolyRing::dd_type BoolePolyRing::zero() const { return dd_type(*this, p_core->m_mgr.zddZero()); }

  /// Get decision diagram with all variables negated
  inline  BoolePolyRing::dd_type BoolePolyRing::one() const { return dd_type(*this, p_core->m_mgr.zddOne()); }


  /// Get constant one or zero
inline  BoolePolyRing::dd_type BoolePolyRing::constant(bool is_one) const { return (is_one? one(): zero()); }

inline  BoolePolyRing::dd_type BoolePolyRing::variable(idx_type nvar) const {
    return dd_type(*this, p_core->m_mgr.getVar(nvar)); 
  }

END_NAMESPACE_PBORI

#endif // of #ifndef BoolePolyRing_h_
