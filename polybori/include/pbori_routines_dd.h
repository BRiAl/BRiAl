// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_cuddext.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which define function templates related to
 * decision diagrams. It should be loaded from pbori_routines.h only
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
 * Revision 1.1  2006/08/23 14:24:54  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// get addition definitions
#include "CTermIter.h"
#include "PBoRiOutIter.h"
#include <set>
#include <vector>


BEGIN_NAMESPACE_PBORI

/// @func dd_last_lexicographical_term
/// @brief Get last term (wrt. lexicographical order).
template<class DDType, class OutputType>
OutputType
dd_last_lexicographical_term(const DDType& dd, type_tag<OutputType>) {

  typedef typename DDType::idx_type idx_type;
  typedef typename DDType::size_type size_type;
  typedef OutputType term_type;

  term_type result(true);

  if (dd.emptiness())
    result = false;
  else {

    size_type nlen = std::distance(dd.lastBegin(), dd.lastEnd());

    // store indices in list
    std::vector<idx_type> indices(nlen);

    // iterator, which uses changeAssign to insert variable
    // wrt. given indices to a monomial
    PBoRiOutIter<term_type, idx_type, change_assign<term_type> >  
      outiter(result);
    
    // insert backward (for efficiency reasons)
    reversed_inter_copy(dd.lastBegin(), dd.lastEnd(), indices, outiter);
  }

  return result;
}

/// @func dd_used_variables
/// @brief Extract used variables from a decision diagram.
template<class DDType, class OutputType>
OutputType
dd_used_variables(const DDType& dd, type_tag<OutputType>){

  // get type definitions from DDType
  typedef typename DDType::idx_type idx_type;
  typedef typename DDType::navigator navigator;
  typedef OutputType monom_type;


#ifdef PBORI_USEDVARS_BY_SUPPORT

  return dd_last_lexicographical_term(dd.support(), type_tag<OutputType>());

#else

  // default value is the one monomial
  monom_type result(true);

# ifdef PBORI_USEDVARS_HIGHLEVEL

  // define iterator type for storing used variables (on forward branches)
  typedef CTermIter< std::set<idx_type>, navigator, 
    inserts< std::set<idx_type> >, project_ith<1>, project_ith<1> >
  the_iterator;

  // initialize iteration
  the_iterator start(dd.navigation());

  // collect all indices during iteration
  while( !start.empty() ){ ++start; }

  typename the_iterator::reference indices(*start);

# elif defined(PBORI_USEDVARS_BY_TRANSFORM) // variant of highlevel

  typedef std::set<idx_type> path_type;
  path_type indices;

  dd_transform( dd.navigation(), dummy_iterator(),
                dummy_iterator(),
                insert_second_to_list<path_type,
                   dummy_iterator,idx_type>(indices),
                project_ith<1, 2>(),  
                project_ith<1>()
                );

# elif defined(PBORI_USEDVARS_BY_IDX) // using internal variant

  // Get indices of used variables
  std::vector<idx_type> indices(dd.nSupport());
  dd.usedIndices(indices);

# endif

  // generate monomial from indices
  PBoRiOutIter<monom_type, idx_type, change_assign<monom_type> >  
    outiter(result);
  copy(indices.rbegin(), indices.rend(), outiter);

  return result;
#endif
}


END_NAMESPACE_PBORI
