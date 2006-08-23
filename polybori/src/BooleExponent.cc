// -*- c++ -*-
//*****************************************************************************
/** @file BooleExponent.cc
 *
 * @author Alexander Dreyer
 * @date 2006-04-19
 *
 * This file implements the class BoolePolymials, where carries the definition
 * of monomials over Booleans.
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
 * Revision 1.1  2006/08/23 17:00:02  dreyer
 * ADD: initial version
 *
 * @endverbatim
**/
//*****************************************************************************


// load header files
# include "BooleExponent.h"
# include "BooleVariable.h"

// get ring definitions
# include "BoolePolyRing.h"
# include "OrderedManager.h"
# include "PBoRiError.h"

BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------

// Default constructor
BooleExponent::BooleExponent():
  m_data()  {

  PBORI_TRACE_FUNC( "BooleExponent()" );

}

// Copy constructor
BooleExponent::BooleExponent(const self& rhs) :
  m_data(rhs.m_data) {
  
  PBORI_TRACE_FUNC( "BooleExponent(const self&)" );
}

// //  Construct from Boolean variable
// BooleExponent::BooleExponent(const var_type& rhs) :
//   m_data(rhs) {
  
//   PBORI_TRACE_FUNC( "BooleExponent(const var_type&)" );
// }

// //  Construct from Boolean constant
// BooleExponent::BooleExponent(bool_type val) :
//   m_data(val) {
  
//   PBORI_TRACE_FUNC( "BooleExponent(bool_type)" );
// }

// Destructor
BooleExponent::~BooleExponent() {

  PBORI_TRACE_FUNC( "~BooleExponent()" );
}


// Reducibility test
BooleExponent::bool_type
BooleExponent::reducibleBy(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::reducibleBy(const self&) const" );

  return std::includes(begin(), end(), rhs.m_data.begin(), rhs.m_data.end());
}


// // Casting operator
// BooleExponent::operator const BoolePolynomial&() const {

//   PBORI_TRACE_FUNC( "BooleExponent::operator const BoolePolynomial&() const" );

//   return m_data;
// }

//  Substitute variable with index idx by its complement and assign
BooleExponent&
BooleExponent::changeAssign(idx_type idx) {

  PBORI_TRACE_FUNC( "BooleExponent::changeAssign(idx_type)" );

  PBORI_NOT_IMPLEMENTED;

  return *this;
}

//  Substitute variable with index idx by its complement
BooleExponent
BooleExponent::change(idx_type idx) const {

  PBORI_TRACE_FUNC( "BooleExponent::change(idx_type) const " );

  PBORI_NOT_IMPLEMENTED;

  return self();
}

//  Division
BooleExponent
BooleExponent::divide(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::divide(const self&) const " );

  if (reducibleBy(rhs)) {

    self result;
    std::back_insert_iterator<data_type> outiter(result.m_data);
  
    result.m_data.reserve(size());
    std::set_difference(begin(), end(), rhs.begin(), rhs.end(), outiter);

    return result;
  }
  else
    return self();
}


// //  Multiplication with assignment
// BooleExponent&
// BooleExponent::operator*=(const self& rhs) {

//   PBORI_TRACE_FUNC( "BooleExponent::operator*=(const self&)" );

//   m_data.diagram().unateProductAssign(rhs.m_data.diagram());

//   return *this;
// }

// //  Division with assignment
// BooleExponent&
// BooleExponent::operator/=(const self& rhs) {

//   PBORI_TRACE_FUNC( "BooleExponent::operator*=(const self&)" );

//   m_data /= rhs;//.m_data;

//   return *this;
// }

// //  Multiplication with assignment (with variable)
// BooleExponent&
// BooleExponent::operator*=(const var_type& rhs) {

//   PBORI_TRACE_FUNC( "BooleExponent::operator*=(const var_type&)" );

//   if (!reducibleBy(rhs))
//     changeAssign(rhs.index());

//   return *this;
// }

// // Division with assignment (with variable)
// BooleExponent&
// BooleExponent::operator/=(const var_type& rhs) {

//   PBORI_TRACE_FUNC( "BooleExponent::operator/=(const var_type&)" );

//   m_data.diagram().subset1Assign(rhs.index());

//   return *this;
// }
// Comparision
BooleExponent::comp_type
BooleExponent::compare(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::compare(const self& rhs) const" );

  PBORI_NOT_IMPLEMENTED;

  return 0;
}

// Degree of the lcm
BooleExponent::size_type
BooleExponent::LCMDeg(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::LCMDeg(const self&) const" );

  PBORI_NOT_IMPLEMENTED;

  return 0;
//   if ( m_data.isZero() || ( (const BoolePolynomial&)rhs).isZero() )
//     return 0;

//   if ( (*this == rhs) || ( (const BoolePolynomial&)rhs).isOne() )
//     return deg();
  
//   if ( m_data.isOne() )
//     return rhs.deg();

//   size_type result = 0;
//   const_iterator start(begin()), finish(end()),
//     rhs_start(rhs.begin()), rhs_finish(rhs.end());

//   while ( (start != finish) && (rhs_start != rhs_finish) ) {

//     idx_type last_idx(*start);

//     if (last_idx <= *rhs_start) 
//       ++start;

//     if (last_idx >= *rhs_start) 
//       ++rhs_start;

//     ++result;
//   }

//   while (start != finish) {
//     ++start;
//     ++result;
//   }

//   while (rhs_start != rhs_finish) {
//     ++rhs_start;
//     ++result;
//   }

//   return result;
}

// gcd
BooleExponent
BooleExponent::GCD(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::GCD(const self&) const" );

  self result;
  std::back_insert_iterator<data_type> outiter(result.m_data);

  result.m_data.reserve(std::min(size(), rhs.size()));
  std::set_intersection(begin(), end(), rhs.begin(), rhs.end(), outiter);

  return result;
}

// gcd with assignment
// BooleExponent&
// BooleExponent::GCDAssign(const self& rhs) {

//   PBORI_TRACE_FUNC( "BooleExponent::GCDAssign(const self&)" );
//   PBORI_NOT_IMPLEMENTED;

//   return *this;
//   if ((*this == rhs) || m_data.isZero() || m_data.isOne() )
//     return *this;

//   if ( ( (const BoolePolynomial&)rhs).isOne() || 
//        ( (const BoolePolynomial&)rhs).isZero() )
//     return (*this = rhs);

//   const_iterator start(begin()), finish(end()),
//     rhs_start(rhs.begin()), rhs_finish(rhs.end());

//   while ( (start != finish) && (rhs_start != rhs_finish) ) {

//     // delete variable not on rhs
//     if (*start < *rhs_start) {
//       changeAssign(*start);
//       ++start;
//     }
//     else {
//       // keep common variables
//       if (*start == *rhs_start) 
//         ++start;

//       // proceed with next rhs
//       ++rhs_start;
//     }
//   }

//   // delete remaining variables in *this
//   while (start != finish) {
//     changeAssign(*start);
//     ++start;
//   }

//   return *this;
//  }

// lcm
BooleExponent
BooleExponent::LCM(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::GCD(const self&) const" );

  return multiply(rhs);
}

// multiplication
BooleExponent
BooleExponent::multiply(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::multiply(const self&) const" );

  self result;
  std::back_insert_iterator<data_type> outiter(result.m_data);

  result.m_data.reserve(size() + rhs.size());
  std::set_union(begin(), end(), rhs.begin(), rhs.end(), outiter);

  return result;
}

// lcm with assignment
// BooleExponent&
// BooleExponent::LCMAssign(const self& rhs) {

//   PBORI_TRACE_FUNC( "BooleExponent::GCDAssign(const self&)" );

//   PBORI_NOT_IMPLEMENTED;

//   return *this;
// }

// Multiples wrt. given monom
BooleExponent::set_type
BooleExponent::multiples(const self& monom) const {

  PBORI_TRACE_FUNC( "BooleExponent::multiples(const self&) const" );

  PBORI_NOT_IMPLEMENTED;

  return set_type();
}

// /// @function greater_variable
// /// @brief  
// BooleExponent::bool_type
// greater_variable(BooleExponent::idx_type lhs, BooleExponent::idx_type rhs){

//   PBORI_TRACE_FUNC( "greater_variable(idx_type, idx_type)" );

//   return
//     (BoolePolyRing::activeManager().compare(lhs, rhs)==CTypes::greater_than);
// }

END_NAMESPACE_PBORI
