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
 * Revision 1.1  2006/04/07 16:32:08  dreyer
 * ADD dd_transform and resp. examples
 *
 * @endverbatim
**/
//*****************************************************************************

#include "pbori_defs.h"

#ifndef pbori_algo_h_
#define pbori_algo_h_

BEGIN_NAMESPACE_PBORI

/// @class push_back 
/// @brief Accessing .push_back()
template <class ListType, class ValueType = typename ListType::value_type >
class push_back {
public:

  ListType
  operator()(ListType theList, const ValueType& elt) const {
    theList.push_back(elt);
    return theList;
  }
};

/// @class change
/// @brief Accessing .change()
template <class RhsType, class LhsType = typename RhsType::idx_type >
class change {
public:

  RhsType operator() (const RhsType& rhs, const LhsType& lhs) const {
    return (rhs.change(lhs));
  } 

};


/// @class project_nth
/// @brief Accessing nth argument (NTH = 0also works as identity)
template <class FirstType, unsigned int NTH>
class project_nth;

// project to first and identity
template <class FirstType>
class project_nth<FirstType, 0> {

public:

  const FirstType& 
  operator() (const FirstType& value) const {
    return value;
  } 

  FirstType& operator() (FirstType& value) const {
    return value;
  } 

  template <class SecondType>
  const FirstType& 
  operator() (const FirstType& value, const SecondType&) const {
    return value;
  } 

  template <class SecondType>
  FirstType& operator() (FirstType& value, const SecondType&) const {
    return value;
  } 
};

// project to second
template <class SecondType>
class project_nth<SecondType, 1> {

public:

  template <class FirstType>
  const SecondType& 
  operator() (const FirstType&, const SecondType& value) const {
    return value;
  } 

  template <class FirstType>
  SecondType& operator() (FirstType&, const SecondType& value) const {
    return value;
  } 
};


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
               project_nth<TermType, 0>() );
}

/// Function templates for transforming decision diagrams 
template< class NaviType, class TermType, class OutIterator,
          class ThenBinaryOperator >
void
dd_transform( const NaviType& navi, const TermType& init, 
              const OutIterator& result, 
              const ThenBinaryOperator& then_binop ) {

  dd_transform(navi, init, result, then_binop,
               project_nth<TermType, 0>(),
               project_nth<TermType, 0>() );
}

END_NAMESPACE_PBORI

#endif
