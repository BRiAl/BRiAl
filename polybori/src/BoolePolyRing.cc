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
# include "BoolePolyRing.h"
#include "BooleEnv.h"


// get error types
# include "PBoRiError.h"

#include <orderings/pbori_order.h>

#include "BooleExponent.h"
#include "BooleMonomial.h"

#include "cuddInt.h"

#include <climits> // LINE_MAX
#include <cstdio> // just for tmpfile()

BEGIN_NAMESPACE_PBORI


/// Constructor for @em nvars variables
  //BooleRing::BooleRing(size_type nvars):
  // p_core(new core_type(nvars, get_ordering(lp)) ) {   }



/// Active Ring
BoolePolyRing::BoolePolyRing() : 
  p_core(BooleEnv::ring().core()) {
  PBORI_TRACE_FUNC( "BoolePolyRing()" );
}

// interface with cudd's variable management
BoolePolyRing::BoolePolyRing(size_type nvars, ordercode_type order,
                             bool_type make_active) : 
  p_core(new core_type(nvars, get_ordering(order))) {
  PBORI_TRACE_FUNC( "BoolePolyRing(size_type)" );

  if(make_active)
    activate();
}

void
BoolePolyRing::changeOrdering(ordercode_type order) {

  PBORI_TRACE_FUNC( "changeOrdering(ordercode_type)" );
  p_core->change_ordering(get_ordering(order)); 
}


//  make this global ring
void
BoolePolyRing::activate() {

  PBORI_TRACE_FUNC( "BoolePolyRing::activate()" );
  BooleEnv::set(*this);
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


#if !defined(__GNUC__) && !defined(fmemopen)
FILE *fmemopen (void *buf, size_t size, const char *opentype)
{
FILE *f;

assert(strcmp(opentype, "r") == 0);

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
    no_error = Cudd_PrintInfo(getManager(), out);

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
