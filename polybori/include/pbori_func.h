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
 * Revision 1.15  2006/07/20 10:13:59  dreyer
 * CHANGE: Made COrderProperties more generic
 *
 * Revision 1.14  2006/07/06 16:01:29  dreyer
 * CHANGE: Functionals ins pbori_func.h made more consistent
 *
 * Revision 1.13  2006/07/04 14:11:03  dreyer
 * ADD: Generic and handy treatment of string literals
 *
 * Revision 1.12  2006/06/07 11:54:26  dreyer
 * ADD variantes for usedVariables
 *
 * Revision 1.11  2006/06/06 10:56:59  dreyer
 * CHANGE usedVariables() more efficient now.
 *
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

/// @class change_assign
/// @brief Accessing .changeAssign()
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

/// @class unite_assign
/// @brief Accessing .uniteAssign()
template <class RhsType,
          class LhsType = typename pbori_traits<RhsType>::idx_type >
class unite_assign:
  public std::binary_function<RhsType&, const LhsType&, RhsType&> {

public:
  RhsType& operator() (RhsType& rhs, const LhsType& lhs) const {
    return (rhs.uniteAssign(lhs));
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

/*
class print_all {
public:

  print_all(std::ostream& os_):os(os_){}

  template<class Type>
  Type& operator()(Type& val){
    std::copy(val.begin(), val.end(), 
         std::ostream_iterator<typename  Type::value_type>(os, ", "));
    return val;
  }
  std::ostream& os;
};
*/

/// @class dummy_iterator
/// @brief An iterator which virtually does nothing.
class dummy_iterator {
public:

  /// Type of *this
  typedef dummy_iterator self;

  template <class Type>
  const self& operator=(const Type&) const { return *this;}

  const self& operator*() const { return *this;}
  const self& operator++() const { return *this;}
  const self& operator++(int) const { return *this;}
};


/// @class integral_constant
/// @brief integral_constant<int_type, NUM>()() returns NUM of int_type, instead
/// of possibly arguments.
/// integral_constant<int_type, NUM, other_type>()() returns conversion of
/// NUM of int_type to other_type.
template <class IntType, IntType INTCONST, class ResultType = IntType>
struct integral_constant {

  typedef ResultType result_type;
  result_type operator()(...) const { return INTCONST; }
};

/// @class static_compose
/// @brief Compose a binary function with two default constructable unary
/// functions.
template <class BinaryOp, class FirstOp, class SecondOp>
class binary_compose:
  public BinaryOp {

public:

  /// Define types corresponding to template arguments
  //@{
  typedef BinaryOp base;
  typedef FirstOp first_op_type;
  typedef SecondOp second_op_type;
  //@}

  // Constructor
  binary_compose(const base& binop = base(),
                 const first_op_type& unop1 = first_op_type(),
                 const second_op_type& unop2 = second_op_type() ): 
    base(binop), first_op(unop1), second_op(unop2) {}

  /// Getting inherited types
  typedef typename base::result_type result_type;

  /// The composed operation for constant arguments
  template <class FirstType, class SecondType>
  result_type operator()(const FirstType& first, 
                         const SecondType& second) {
    return base::operator()(first_op(first), second_op(second));
  }

  /// The composed operation for constant second argument
  template <class FirstType, class SecondType>
  result_type operator()(FirstType& first, 
                         const SecondType& second) {
    return base::operator()(first_op(first), second_op(second));
  }

  /// The composed operation for constant first argument
  template <class FirstType, class SecondType>
  result_type operator()(const FirstType& first, 
                         SecondType& second) {
    return base::operator()(first_op(first), second_op(second));
  }

protected:
  first_op_type first_op;
  second_op_type second_op;
};

/// @class maximum_iteration
/// @brief Returns maximun of given argument and previous calls
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

/// @class dd_add_assign
/// @brief Defines addition operation with assignment for decision diagrams.
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
  }
};

/// @class times_indexed_var
/// @brief Defines multiplication operation with the idx-th variable
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

/// @class append_indexed_divisor
/// @brief Defines multiplication (with assignment) with the idx-th variable
template <class DDType, class IdxType = typename DDType::idx_type>
class append_indexed_divisor {
public:

  DDType& operator()(DDType& lhs, IdxType idx) const {

    lhs.uniteAssign( lhs.change(idx) );
    return lhs;
  }

};

/// @class inserts
/// @brief Accessing .insert()
template <class RhsType = void,
          class LhsType = typename RhsType::value_type >
class inserts:
  public std::binary_function<RhsType&, const LhsType&, RhsType&> {
public:

  RhsType& operator() (RhsType& rhs, const LhsType& lhs) const {
    rhs.insert(lhs);
    return rhs;
  } 

};

/// @class insert_second_to_list
/// @brief Insert second argument to a given list
template <class ListType, class RhsType, class LhsType>
class insert_second_to_list {
public:

  insert_second_to_list(ListType& theList__):
    theList(theList__) {};

  RhsType& operator() (RhsType& rhs, const LhsType& lhs) const {
    theList.insert(lhs);
    return rhs;
  } 

private:
  ListType& theList;
};


/** @class is_same_type
 * @brief This class tests whether two types equal
 **/
template <class Type1, class Type2>
class is_same_type;

template <class Type>
class is_same_type<Type, Type>:
  public integral_constant<CTypes::bool_type, true> {}; 

template <class Type1, class Type2>
class is_same_type:
  public integral_constant<CTypes::bool_type, false> {};

END_NAMESPACE_PBORI

#endif
