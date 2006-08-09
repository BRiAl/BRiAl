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
 * Revision 1.11  2006/08/09 15:18:58  dreyer
 * FIX: Bug in dd_intersect_some_index (didn't check valid path)
 *
 * Revision 1.10  2006/08/09 12:52:31  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * Revision 1.9  2006/08/03 15:20:20  dreyer
 * ADD: BooleSet::divisorsOf and BooleSet::hasCommonVariables
 *
 * Revision 1.8  2006/07/06 16:01:29  dreyer
 * CHANGE: Functionals ins pbori_func.h made more consistent
 *
 * Revision 1.7  2006/07/04 14:11:03  dreyer
 * ADD: Generic and handy treatment of string literals
 *
 * Revision 1.6  2006/04/21 13:13:30  dreyer
 * ADD PBoRiOutITer for more generic manipulations
 *
 * Revision 1.5  2006/04/14 15:45:30  bricken
 * + removed semicolons
 *
 * Revision 1.4  2006/04/13 07:53:19  dreyer
 * CHANGE BoolePolynomial::print() and deg() produces more useful results
 *
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
OutIterator
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
    result = dd_transform(navi.thenBranch(), then_binop(init, *navi), result,
                 then_binop, else_binop, terminate);
    result = dd_transform(navi.elseBranch(), else_binop(init, *navi), result,
                 then_binop, else_binop, terminate);
  }
  return result;
}

/// Function templates for transforming decision diagrams 
/// with special treatment of the leading term
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator, class ElseBinaryOperator, 
          class TerminalOperator, class FirstTermOp >
OutIterator
dd_transform( NaviType navi, TermType init, OutIterator result, 
              ThenBinaryOperator then_binop, ElseBinaryOperator else_binop,
              TerminalOperator terminate, FirstTermOp terminate_first ) {

  if (navi.isConstant()) {      // Reached end of path
    if (navi.terminalValue()){   // Case of a valid path - here leading term
      *result = terminate_first(init);
      ++result;
    }
  }
  else {
    result = dd_transform(navi.thenBranch(), then_binop(init, *navi), result,
                 then_binop, else_binop, terminate, terminate_first);
    result = dd_transform(navi.elseBranch(), else_binop(init, *navi), result,
                 then_binop, else_binop, terminate);
  }
  return result;
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
                         UnaryFunction op, FirstFunction firstop) {
  InputIterator second(first);
  if (second != last) {
    ++second;
    result = std::transform(first, second, result, firstop);
  }
  return std::transform(second, last, result, op);
}


/// Function templates doing a reversed copy using intermediate storage
template <class InputIterator, class Intermediate, class OutputIterator>
OutputIterator
reversed_inter_copy( InputIterator start, InputIterator finish,
                     Intermediate& inter, OutputIterator output ) {

    std::copy(start, finish, inter.begin());
    // force constant
    return std::copy( const_cast<const Intermediate&>(inter).rbegin(),
                      const_cast<const Intermediate&>(inter).rend(), 
                      output );
}


/// Function templates for checking whether a given decision diagram contain
/// at least one index in range [start, finish)
template <class NaviType, class OrderedIterator>
bool
dd_owns_some_index(NaviType navi, 
                   OrderedIterator start, OrderedIterator finish) {
 
  if (!navi.isConstant()) {     // Not at end of path
    bool not_at_end;
    while( (not_at_end = (start != finish)) && (*start < *navi) )
      ++start;

    if(not_at_end) {
      return (*start == *navi) ||
        dd_owns_some_index(navi.thenBranch(), start, finish) ||
        dd_owns_some_index(navi.elseBranch(), start, finish);
    }
  }
  return false;
}

/// Function templates for checking whether a given decision diagram contain
/// at least one index in range [start, finish)
/// Note: Returns incremented node
template <class NaviType, class OrderedIterator, class NodeOperation>
NaviType
dd_intersect_some_index(NaviType navi, 
                        OrderedIterator start, OrderedIterator finish,
                        NodeOperation newNode ) {
 
  if (!navi.isConstant()) {     // Not at end of path
    bool not_at_end;
    while( (not_at_end = (start != finish)) && (*start < *navi) )
      ++start;

      NaviType elseNode = 
        dd_intersect_some_index(navi.elseBranch(), start, finish, 
                                newNode);
  
      if (not_at_end && (*start == *navi)) {

        NaviType thenNode = 
          dd_intersect_some_index(navi.thenBranch(), start, finish, 
                                  newNode);

        return newNode(*start, navi, thenNode, elseNode); 
      }
      else
        return elseNode;

  }

  return newNode(navi);
}



/// Function templates for debugging, prints dd indices and reference counts
template <class NaviType>
void
dd_print(NaviType navi) {
 
  if (!navi.isConstant()) {     // Not at end of path
 
    std::cout << std::endl<< "idx " << *navi <<" addr "<<
      ((DdNode*)navi)<<" ref "<<
      int(Cudd_Regular((DdNode*)navi)->ref) << std::endl;

    dd_print(navi.thenBranch());
    dd_print(navi.elseBranch());

  }
  else {
    std::cout << "const isvalid? "<<navi.isValid()<<" addr "
              <<((DdNode*)navi)<<" ref "<<
      int(Cudd_Regular((DdNode*)navi)->ref)<<std::endl;

  }
}

END_NAMESPACE_PBORI

#endif
