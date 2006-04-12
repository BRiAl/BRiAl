// -*- c++ -*-
//*****************************************************************************
/** @file pbori_algo.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-07
 *
 * This file includes some templates of simple transformations and similar
 * procedures. 
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
 * Revision 1.3  2006/04/12 16:23:54  dreyer
 * ADD template class CIDXPath<>
 *
 * Revision 1.2  2006/04/10 07:36:27  dreyer
 * ADD pbori_func.h to carry PolyBoRi related functionals
 *
 * Revision 1.1  2006/04/07 16:32:08  dreyer
 * ADD dd_transform and resp. examples
 *
 * @endverbatim
**/
//*****************************************************************************

// include polybori's definitions
#include "pbori_defs.h"

// get polybori's functionals
#include "pbori_func.h"

#ifndef pbori_algo_h_
#define pbori_algo_h_

BEGIN_NAMESPACE_PBORI

/// Function templates for transforming decision diagrams 
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator, class ElseBinaryOperator, 
          class TerminalOperator >
void
dd_transform( NaviType navi, TermType init, OutIterator result, 
              ThenBinaryOperator then_binop, ElseBinaryOperator else_binop,
              TerminalOperator terminate ) {

  if (navi.isConstant()) {      // Reached end of path
    if (navi.terminalValue()){   // Case of a valid path
      *result = terminate(init);
      ++result;
    }
  }
  else {
    dd_transform(navi.thenBranch(), then_binop(init, *navi), result,
                 then_binop, else_binop, terminate);
    dd_transform(navi.elseBranch(), else_binop(init, *navi), result,
                 then_binop, else_binop, terminate);
  }
}

/// Function templates for transforming decision diagrams 
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator, class ElseBinaryOperator >
void
dd_transform( const NaviType& navi, const TermType& init, 
              const OutIterator& result, 
              const ThenBinaryOperator& then_binop, 
              const ElseBinaryOperator& else_binop ) {

  dd_transform(navi, init, result, then_binop, else_binop, 
               project_ith<1>() );
}

/// Function templates for transforming decision diagrams 
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator >
void
dd_transform( const NaviType& navi, const TermType& init, 
              const OutIterator& result, 
              const ThenBinaryOperator& then_binop ) {

  dd_transform(navi, init, result, then_binop,
               project_ith<1, 2>(),
               project_ith<1>() );
}


template <class InputIterator, class OutputIterator, 
          class FirstFunction, class UnaryFunction>
OutputIterator 
special_first_transform(InputIterator first, InputIterator last,
                        OutputIterator result, 
                        FirstFunction firstop, UnaryFunction op) {
  InputIterator second(first);
  if (second != last) {
    ++second;
    result = std::transform(first, second, result, firstop);
  }
  return std::transform(second, last, result, op);
};


/// Function templates for transforming decision diagrams 
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator >
void
transform( const NaviType& navi, const TermType& init, 
              const OutIterator& result, 
              const ThenBinaryOperator& then_binop ) {



}


END_NAMESPACE_PBORI

#endif
