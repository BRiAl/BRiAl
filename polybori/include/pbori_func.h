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
 * Revision 1.2  2006/04/12 16:23:54  dreyer
 * ADD template class CIDXPath<>
 *
 * Revision 1.1  2006/04/10 07:36:27  dreyer
 * ADD pbori_func.h to carry PolyBoRi related functionals
 *
 * @endverbatim
**/
//*****************************************************************************

// get polybori definitions
#include "pbori_defs.h"

// get standard string and string stream functionality
#include <string>
#include <sstream>

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


/// @class default_varname
/// @brief Generate variable name
template <class IdxType, 
          class OutType = std::string, class StringType = OutType>
class default_varname:
  public std::unary_function<IdxType, OutType>{

public:

  /// Get type of *this
  typedef default_varname<IdxType, OutType> self;

  /// Get base type
  typedef std::unary_function<IdxType, OutType> base;

  /// Default constructor
  default_varname():
    pre_str("x("), post_str(")"), base() {}

  /// Constructor with custom pre and post strings
  default_varname(const StringType& before):
    pre_str(before), post_str(")"), base() {
    pre_str.append("x(");
  }

   /// Constructor with custom before and after strings
  default_varname(const StringType& before, const StringType& after):
    pre_str(before), post_str(")"), base() {
    pre_str.append("x(");
    post_str.append(after);
  }

  /// Constructor with custom before, after, pre and post strings
  default_varname(const StringType& before,
                  const StringType& pre, const StringType& post, 
                  const StringType& after):
    pre_str(before), post_str(post), base() {
    pre_str.append(pre);
    post_str.append(after);
  }

 /// Copy constructor 
  default_varname(const self& rhs):
    pre_str(rhs.pre_str), post_str(rhs.post_str), base() {}

  /// Functional operator 
  OutType operator() (IdxType idx)  {
    std::ostringstream oss;
    oss << pre_str << idx << post_str;

    return oss.str();
  } 

private:
  StringType  pre_str, post_str;
};


class list_separator {

public:
  typedef const char* result_type;

  result_type operator()(){
    return ", ";
  }

};

class times_as_separator {

public:
  typedef const char* result_type;

  result_type operator()(){
    return "*";
  }

};

END_NAMESPACE_PBORI

#endif
