// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_dd.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which define function templates related to
 * decision diagrams. It should be loaded from pbori_routines.h only
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_routines_pbori_routines_dd_h_
#define polybori_routines_pbori_routines_dd_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// get addition definitions
#include <polybori/iterators/CTermIter.h>
#include <polybori/iterators/PBoRiOutIter.h>
#include <set>
#include <vector>

BEGIN_NAMESPACE_PBORI

/// Get last term (wrt. lexicographical order).
template<class DDType, class OutputType>
OutputType
dd_last_lexicographical_term(const DDType& dd, type_tag<OutputType>) {

  typedef typename DDType::idx_type idx_type;
  typedef typename DDType::size_type size_type;
  typedef OutputType term_type;

  term_type result(dd.ring());

  PBORI_ASSERT(!dd.isZero());

  size_type nlen = std::distance(dd.lastBegin(), dd.lastEnd());

  // store indices in list
  std::vector<idx_type> indices(nlen);
  
  // iterator, which uses changeAssign to insert variable
  // wrt. given indices to a monomial
  PBoRiOutIter<term_type, idx_type, change_assign<term_type> >  
    outiter(result);
  
  // insert backward (for efficiency reasons)
  reversed_inter_copy(dd.lastBegin(), dd.lastEnd(), indices, outiter);

  return result;
}


END_NAMESPACE_PBORI

#endif
