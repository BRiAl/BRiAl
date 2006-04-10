// -*- c++ -*-
//*****************************************************************************
/** @file pbori_func.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-07
 *
 * This file includes some functional templates templates. 
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
 * Revision 1.1  2006/04/10 07:36:27  dreyer
 * ADD pbori_func.h to carry PolyBoRi related functionals
 *
 * @endverbatim
**/
//*****************************************************************************

#include "pbori_defs.h"

#ifndef pbori_func_h_
#define pbori_func_h_

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


/// @class project_ith
/// @brief Accessing ith of n arguments 
/// (ITH = 0 returns default value of first type)
///
/// @note The argument NLEN is not necessary, but is may be used to indicate the
/// argument length for documentation reasons.

template <unsigned int ITH, unsigned int NLEN = ITH>
class project_ith;

/// @class project_ith<0, NLEN>
/// @brief Generates default value for first given value type
template <unsigned int NLEN>
class project_ith<0, NLEN> {

public:
  /// Functional operator template
  template <class ValueType>
  ValueType operator() (const ValueType&, ...) const {
    return ValueType();
  } 
};

/// @class project_ith<1, NLEN>
/// @brief Is the identity functional or projection to first argument
template <unsigned int NLEN>
class project_ith<1, NLEN> {

public:
  /// Functional operator template
  template <class ValueType>
  const ValueType& operator() (const ValueType& value, ...) const {
    return value;
  } 

  /// Constant functional operator template
  template <class ValueType>
  ValueType& operator() (ValueType& value, ...) const {
    return value;
  } 
};


/// @class project_ith<2, NLEN>
/// @brief Project to the second of all given arguments
template <unsigned int NLEN>
class project_ith<2, NLEN> {

public:
  /// Functional operator template
  template <class FirstType, class ValueType>
  const ValueType& 
  operator() (const FirstType&, const ValueType& value, ...) const {
    return value;
  } 

  /// Constant functional operator template
  template <class FirstType, class ValueType>
  ValueType& operator() (const FirstType&, ValueType& value, ...) const {
    return value;
  } 
};


/// @class project_ith<3, NLEN>
/// @brief Project to the third of all given arguments
template <unsigned int NLEN>
class project_ith<3, NLEN> {

public:
  /// Functional operator template
  template <class FirstType, class SecondType, class ValueType>
  const ValueType& 
  operator() (const FirstType&, const SecondType&, 
              const ValueType& value, ...) const {
    return value;
  } 

  /// Constant functional operator template
  template <class FirstType, class SecondType, class ValueType>
  ValueType& operator() (const FirstType&, const SecondType&, 
                         ValueType& value, ...) const {
    return value;
  } 
};

END_NAMESPACE_PBORI

#endif
