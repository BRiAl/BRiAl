// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_order.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which defines ordering-related function templates.
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
 * Revision 1.1  2006/09/04 15:58:43  dreyer
 * ADD: DegLexOrder and preparations
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"


BEGIN_NAMESPACE_PBORI

/// @func lex_compare_indices
/// @brief defines lexicographic comparison for variable indices
template<class IdxType>
CTypes::comp_type
lex_compare_indices(IdxType lhs, IdxType rhs) {

  if (lhs == rhs)
    return CTypes::equality;

  return (lhs < rhs?  CTypes::greater_than : CTypes::less_than);
}

/// @func lex_compare
/// @brief defines lexicographic comparison for ranges (defined by iterators)
template<class FirstIterator, class SecondIterator>
CTypes::comp_type
lex_compare(FirstIterator start, FirstIterator finish, 
            SecondIterator rhs_start, SecondIterator rhs_finish) {

  while ( (start != finish) && (rhs_start != rhs_finish) && 
          (*start == *rhs_start) ) {
    ++start; ++rhs_start;
  }

  if (start == finish)
    return CTypes::less_than;

  if (rhs_start == rhs_finish)
    return CTypes::greater_than;

  return lex_compare_indices(*start, *rhs_start);
}

/// @func lex_compare
/// @brief defines lexicographic comparison
template<class LhsType, class RhsType>
CTypes::comp_type
lex_compare(const LhsType& lhs, const RhsType& rhs) {

  if (lhs == rhs)
    return CTypes::equality;

  return lex_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/// @func deg_lex_compare
/// @brief defines degree-lexicographic comparison
template<class LhsType, class RhsType>
CTypes::comp_type
deg_lex_compare(const LhsType& lhs, const RhsType& rhs) {

  if (lhs == rhs)
    return CTypes::equality;

  typename CTypes::size_type llen(lhs.size()), rlen(rhs.size());

  if (llen == rlen)
    return lex_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
  else  
    return (llen > rlen?  CTypes::greater_than : CTypes::less_than);
}

END_NAMESPACE_PBORI
