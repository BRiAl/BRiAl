// -*- c++ -*-
//*****************************************************************************
/** @file BooleConstant.h
 *
 * @author Alexander Dreyer
 * @date 2008-03-05
 *
 * This file defines an intermediate class for handling bool and int values,
 * which were not converted to Boolean polynomials or monomials yet.
 *
 * @par Copyright:
 *   (c) 2008-2011 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_BooleConstant_h_
#define polybori_BooleConstant_h_

// include basic definitions
#include <polybori/pbori_defs.h>

BEGIN_NAMESPACE_PBORI

/** @class BooleConstant
 * @brief This class wraps a bool value, which was not converted to a boolean
 * polynomial or monomial yet.
 *
 * This is mostly equivalent to the built-in type @c bool. Only conversion
 * from integer values to @c BooleConstant is nontrivial, as it involves the
 * modulo 2 operation.
 *
 * The class partially implements the interfaces of @c BoolePolynomial and 
 * @c BooleMonomial.
 *
 * @note Since BooleConstant(1) may be interpreted as monomial as well as
 * polynomial, we skip member functions, that have different meanings for
 * polynomials and monomials (like begin() and end()).
 **/

class BooleConstant:
  public CTypes {

  typedef BooleConstant self;

public:
  /// Define dummy iterator
  typedef const self* const_iterator;

  /// Default constructor
  BooleConstant(): m_value(false) {}

  /// constructor for bool values
  BooleConstant(bool value): m_value(value) {}

  /// Constructor for short integer values (nontrivial conversion)
  BooleConstant(short value): m_value(value % 2) {}

  /// Constructor for unsigned shoer integer values (nontrivial conversion)
  BooleConstant(unsigned short value): m_value(value % 2) {}

  /// Constructor for integer values (nontrivial conversion)
  BooleConstant(int value): m_value(value % 2) {}

  /// Constructor for unsigned integer values (nontrivial conversion)
  BooleConstant(unsigned int value): m_value(value % 2) {}

  /// Constructor for long integer values (nontrivial conversion)
  BooleConstant(long value): m_value(value % 2) {}

  /// Constructor for unsigned long integer values (nontrivial conversion)
  BooleConstant(unsigned long value): m_value(value % 2) {}

  /// Convert to bool value
  operator bool() const { return m_value; }

  /// Negation operation
  BooleConstant operator!() const { return !m_value; }

  /// Get constant degree
  deg_type deg() const { return (m_value? 0: -1); }

  /// Start polynomial-style iterations
  const_iterator orderedBegin() const { return this; }

  /// Finish polynomial-style iterations
  const_iterator orderedEnd() const { return this + (m_value? 1: 0); }

  /// Start empty monomial iteration
  const_iterator variableBegin() const { return this; } 

  /// Finish empty monomial iteration
  const_iterator variableEnd() const { return variableBegin(); }

  /// Mimic constant monomial or polynmomial term
  //@{
  bool isOne() const { return m_value; }
  bool isZero() const { return !m_value; }
  bool isConstant() const { return true; }
  bool hasConstantPart() const { return isOne(); }
  //@}

protected:
  /// Boolean value is stored as simple bool
  const bool m_value;
};

/// Stream output operator
inline CTypes::ostream_type& 
operator<<(CTypes::ostream_type& os, const BooleConstant& rhs) {
  return (os << (int) rhs);
}

inline BooleConstant
operator+(const BooleConstant& lhs, const BooleConstant& rhs) {
  return lhs ^ rhs;
}

inline BooleConstant
operator-(const BooleConstant& lhs, const BooleConstant& rhs) {
  return lhs + rhs;
}

inline BooleConstant
operator*(const BooleConstant& lhs, const BooleConstant& rhs) {
  return lhs && rhs;
}

END_NAMESPACE_PBORI

#endif // polybori_BooleConstant_h_
