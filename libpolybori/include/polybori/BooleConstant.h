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
 *   (c) 2008 by The PolyBoRi Team
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
 * @note This is mostly equivalent to the built-in type @c bool. Only conversion
 * from ineger values to @c BooleConstant is nontrivial, as it involves the
 * modulo 2 operation.
 **/

class BooleConstant:
  public CTypes {

public:
  /// Default constructor
  BooleConstant(): m_value(false) {}

  /// constructor for bool values
  BooleConstant(bool value): m_value(value) {}

  /// Cosntructor for integer values (nontrivial conversion)
  BooleConstant(int value): m_value(value % 2) {}

  /// Cosntructor for unsigned integer values (nontrivial conversion)
  BooleConstant(unsigned int value): m_value(value % 2) {}

  /// Convert to bool value
  operator bool() const { return m_value; }

  /// Negation operation
  BooleConstant operator!() const { return !m_value; }

  /// Get constant degree
  deg_type deg() const { return (m_value? 0: -1); }

protected:
  /// Boolean value is stored as simple bool
  const bool m_value;
};

/// Stream output operator
inline CTypes::ostream_type& 
operator<<(CTypes::ostream_type& os, const BooleConstant& rhs) {
  return (os << (int) rhs);
}


END_NAMESPACE_PBORI

#endif // polybori_BooleConstant_h_
