// -*- c++ -*-
//*****************************************************************************
/** @file BoolePolyRing.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file implements the class BoolePolyRing, where carries the definition of
 * a polynomial ring over Booleans. 
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************


// load header file
#include <polybori/BoolePolyRing.h>
#include <polybori/BooleEnv.h>


// get error types
#include <polybori/except/PBoRiError.h>

#include <polybori/orderings/pbori_order.h>

#include <polybori/BooleExponent.h>
#include <polybori/BooleMonomial.h>


#include <climits> // LINE_MAX
#include <cstdio> // just for tmpfile()

#include <polybori/BooleSet.h>
  //#include <polybori/WeakRingPtr.h>

BEGIN_NAMESPACE_PBORI

 /// Get empty decision diagram 
BoolePolyRing::dd_type
BoolePolyRing::zero() const { return dd_type(*this, p_core->m_mgr.zddZero()); }

  /// Get decision diagram with all variables negated
BoolePolyRing::dd_type
BoolePolyRing::one() const { return dd_type(*this, p_core->m_mgr.zddOne()); }


  /// Get constant one or zero
BoolePolyRing::dd_type
BoolePolyRing::constant(bool is_one) const { return (is_one? one(): zero()); }

BoolePolyRing::dd_type 
BoolePolyRing::variableDiagram(checked_idx_type nvar) const {
  return dd_type(*this, p_core->m_mgr.getVar(nvar)); 
}


/// Constructor for @em nvars variables
  //BooleRing::BooleRing(size_type nvars):
  // p_core(new core_type(nvars, get_ordering(lp)) ) {   }



// interface with cudd's variable management
BoolePolyRing::BoolePolyRing(size_type nvars, ordercode_type order) : 
  p_core(new core_type(nvars, get_ordering(order))) {
  PBORI_TRACE_FUNC( "BoolePolyRing(size_type)" );
}

void
BoolePolyRing::changeOrdering(ordercode_type order) {

  PBORI_TRACE_FUNC( "changeOrdering(ordercode_type)" );
  p_core->change_ordering(get_ordering(order)); 
}


/// Map polynomial to this ring, if possible
BoolePolyRing::poly_type BoolePolyRing::coerce(const poly_type& rhs) const {
  if (getManager() ==
      rhs.ring().getManager()) 
    return poly_type(rhs.navigation(), *this);
  
  throw PBoRiError(CTypes::invalid);
}

/// Map monomial to this ring, if possible
BoolePolyRing::monom_type BoolePolyRing::coerce(const monom_type& rhs) const {
  return CDDOperations<BooleSet, monom_type>().getMonomial(coerce(poly_type(rhs)).set());
}

/// Map variable to this ring
BoolePolyRing::var_type BoolePolyRing::coerce(const var_type& rhs) const {
  return var_type(rhs.index(), *this);
}

BoolePolyRing::var_type 
BoolePolyRing::variable(checked_idx_type nvar) const {
  return var_type(nvar, *this); 
}


#if !defined(__GNUC__) && !defined(fmemopen)
FILE *fmemopen (void *buf, size_t size, const char *opentype)
{
FILE *f;

PBORI_ASSERT(strcmp(opentype, "r") == 0);

f = tmpfile();
fwrite(buf, 1, size, f);
rewind(f);

return f;
}
#endif

BoolePolyRing::ostream_type& 
BoolePolyRing::print(ostream_type& os) const {

  FILE* out = tmpfile();
  bool no_error = false;
  if (out)
    no_error = PBORI_PREFIX(Cudd_PrintInfo)(getManager(), out);

  if (no_error) {
    rewind(out);
    char str_buf[LINE_MAX];
    while( fgets(str_buf, LINE_MAX, out) )
      os <<"## "<< str_buf;
  }
  else
    os << "##  No ring info available" << std::endl;

  fclose(out);

  return os;
}

END_NAMESPACE_PBORI

