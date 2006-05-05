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
 * Revision 1.10  2006/05/05 09:03:44  dreyer
 * ADD different implementation of the addition
 *
 * Revision 1.9  2006/04/25 10:17:39  dreyer
 * FIX recovered template<> preceeding explicit template specialization
 *
 * Revision 1.8  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.7  2006/04/24 14:45:35  dreyer
 * FIX CTermIter; ADD BoolePolynomial uses CTermIter
 *
 * Revision 1.6  2006/04/24 10:23:22  dreyer
 * ADD BoolePolynomial::begin() and end()
 * FIX type reference in CCuddNavigator
 *
 * Revision 1.5  2006/04/21 13:13:30  dreyer
 * ADD PBoRiOutITer for more generic manipulations
 *
 * Revision 1.4  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.3  2006/04/13 07:53:19  dreyer
 * CHANGE BoolePolynomial::print() and deg() produces more useful results
 *
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

// get polybori properties
#include "pbori_traits.h"

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
class change_idx {
public:

  RhsType operator() (const RhsType& rhs, const LhsType& lhs) const {
    return (rhs.change(lhs));
  } 

};

// /// @class change_assign
// /// @brief Accessing .changeAssign()
template <class RhsType = void,
          class LhsType = typename pbori_traits<RhsType>::idx_type >
class change_assign;

/// @class change_assign
/// @brief Accessing .changeAssign()
template <class RhsType, class LhsType>
class change_assign {
public:

  RhsType& operator() (RhsType& rhs, const LhsType& lhs) const {
    return (rhs.changeAssign(lhs));
  } 

};

/// @class changeAssign
/// @brief Accessing .change(); variante using member templates
template<>
class change_assign<void, pbori_traits<void>::idx_type> {
public:

  template <class RhsType, class LhsType>
  RhsType& operator() (RhsType& rhs, const LhsType& lhs) const {
    return (rhs.changeAssign(lhs));
  } 

};

// @class project_ith
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

/// @class set_constant
/// @brief Generates default value for first given value type
template <class ValueType, unsigned int CONSTVAL = 0>
class constant_value:
  public std::unary_function<ValueType, ValueType> {
public:
  /// Functional operator 
  ValueType operator() (const ValueType &) const {
    return CONSTVAL;
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


class print_it {
public:

  print_it(std::ostream& os_):os(os_){}

  template<class Type>
  Type& operator()(Type& val){
    os <<val;
    return val;
  }
  std::ostream& os;
};


class print_it_plus {
public:
  print_it_plus(std::ostream& os_):os(os_){}
  template<class Type>
  Type& operator()(Type& val){
    os <<" + "<<val;
    return val;
  }
  std::ostream& os;
};

class dummy_iterator {
public:
  typedef dummy_iterator self;

  template <class Type>
  self& operator=(const Type&) { return *this;}

  self& operator*() { return *this;}
  self& operator++() { return *this;}
  self& operator++(int) { return *this;}
};

template <class ValueType>
class increment_value {

public:
  ValueType operator()(const ValueType& val, const ValueType&) const {
    return (val + 1);
  }
};

template <class ValueType>
class decrement_value {

public:
  ValueType operator()(const ValueType& val, const ValueType&) const {
    return (val - 1);
  }
};

template<class ValueType>
class maximum_iteration {
public:
  maximum_iteration(ValueType & init) : max(init){}

  ValueType& operator()(const ValueType& val) const {
    return max = std::max(max, val);
  }


private:
  ValueType & max;
};

template <class DDType>
class dd_add_assign {
public:

  DDType& operator()(DDType& lhs, const DDType& rhs) const {
    // several possible implementations
    return 
#ifdef PBORI_ADD_BY_ITE
      lhs.iteAssign(lhs.diff(rhs), rhs);

# elif defined(PBORI_ADD_BY_OR)
      (lhs = (lhs.diff(rhs)).unite(rhs.diff(lhs)));

# elif defined(PBORI_ADD_BY_UNION)
      (lhs = lhs.unite(rhs).diff( lhs.intersect(rhs) ) );

#endif
  };

};

template <class DDType, class IdxType = typename DDType::idx_type>
class times_indexed_var {
public:


  DDType& operator()(DDType& lhs, IdxType idx) const {

  // get all terms not containing the variable with index idx
    DDType tmp( lhs.subset0(idx) );

    // get the complementary terms
    lhs.diffAssign(tmp);

    // construct polynomial terms
    dd_add_assign<DDType>()(lhs, tmp.change(idx));

    return lhs;
  }

};


template <class DDType, class IdxType = typename DDType::idx_type>
class append_indexed_divisor {
public:

  DDType& operator()(DDType& lhs, IdxType idx) const {

    lhs.uniteAssign( lhs.change(idx) );
    return lhs;
  }

};

END_NAMESPACE_PBORI

#endif
