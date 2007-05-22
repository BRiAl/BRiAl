// -*- c++ -*-
//*****************************************************************************
/** @file CDDOperations.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-29
 *
 * This file implements an internal template class for geenric CDDOperations;
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
 * Revision 1.5  2007/05/22 11:05:28  dreyer
 * FIX: ambigous overload
 *
 * Revision 1.4  2006/11/27 16:25:14  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.3  2006/09/14 10:57:26  dreyer
 * ADD: usedVariablesExp()
 *
 * Revision 1.2  2006/08/29 12:09:26  dreyer
 * using CDDOperations for generic dd functions
 *
 * Revision 1.1  2006/08/29 10:37:56  dreyer
 * CHANGE: non-const version of diagram() now internalDiagram()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// Get Cudd definitions
#include "cudd.h"
#include "extrafwd.h"

#ifndef CDDOperations_h_
#define CDDOperations_h_

BEGIN_NAMESPACE_PBORI

/// @class CDDOperation
/// @brief Generic class containing decision diagram operations
template <class DDType, class MonomType>
class CDDOperations {
public:

  // This is only a work-around, since a monomial should not be generated from
  // a dd.
  MonomType getMonomial(const DDType& dd) const {
    return MonomType(dd);
  }
  
  MonomType usedVariables(const DDType& dd){

  // get type definitions from DDType
  typedef typename DDType::idx_type idx_type;
  typedef typename DDType::navigator navigator;
  typedef MonomType monom_type;

#ifdef PBORI_USEDVARS_EXTRA

  /// using cudd/extra

    //  monom_type result;
    
    int* pIdx = Cudd_SupportIndex( dd.manager().getManager(), 
                                  ((const ZDD &) dd).getNode() );

    monom_type result( (DDType)ZDD( &dd.manager(),
                Extra_zddCombination( dd.manager().getManager(), 
                                      pIdx, dd.nVariables()) ));


    FREE(pIdx);

    return result;

#else 

#ifdef PBORI_USEDVARS_BY_SUPPORT

  /// @todo: check here
  return dd_last_lexicographical_term(dd.support(), type_tag<MonomType>());

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
  std::vector<idx_type> indices(0);
  dd.usedIndices(indices);

# endif

  // generate monomial from indices
  PBoRiOutIter<monom_type, idx_type, change_assign<monom_type> >  
    outiter(result);
  copy(indices.rbegin(), indices.rend(), outiter);

  return result;
#endif

#endif

}

};



END_NAMESPACE_PBORI

#endif
