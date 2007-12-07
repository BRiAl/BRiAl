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
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.15  2007/12/07 17:06:19  dreyer
 * CHANGE: First try: ring and order separated
 *
 * Revision 1.14  2007/11/30 09:33:19  dreyer
 * CHANGE: more dd-like stableHash()
 *
 * Revision 1.13  2007/11/29 16:28:32  dreyer
 * ADD: fast hash(), where applicable; + stableHashes() anywhere
 *
 * Revision 1.12  2007/11/06 15:03:37  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.11  2007/07/06 14:04:22  dreyer
 * ADD: newly written C++_interface for Cudd
 *
 * Revision 1.10  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.9  2006/11/29 13:40:03  dreyer
 * CHANGE: leadexp() made recursive and cached
 *
 * Revision 1.8  2006/10/02 15:39:36  dreyer
 * FIX: BooleExponent::push_back() (empty exponent)
 *
 * Revision 1.7  2006/09/20 07:06:39  dreyer
 * ADD BoolePolynomial/CDDInterface::isConstant(), used it in deg()
 *
 * Revision 1.6  2006/09/15 16:21:04  dreyer
 * CHANGE: testing more sophisticated hash fucntion for BooleExponent
 *
 * Revision 1.5  2006/09/14 10:57:26  dreyer
 * ADD: usedVariablesExp()
 *
 * Revision 1.4  2006/08/28 14:31:06  dreyer
 * FIX: using now correct const/nonconst iterators
 *
 * Revision 1.3  2006/08/28 07:25:08  dreyer
 * CHANGE: BooleExponent nomenclatur
 *
 * Revision 1.2  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
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

#include "pbori_routines.h"

#include <functional>

BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------

// Default constructor
BooleExponent::BooleExponent():
  m_data()  {

  PBORI_TRACE_FUNC( "BooleExponent()" );

}

// Default constructor
BooleExponent::BooleExponent(bool):
  m_data()  {

  PBORI_TRACE_FUNC( "BooleExponent(bool)" );

}

// Default constructor
BooleExponent&
BooleExponent::get(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BooleExponent::get(const monom_type&)" );

  m_data.resize(rhs.size());  
  std::copy(rhs.begin(), rhs.end(), m_data.begin());

  return *this;
}

// Copy constructor
BooleExponent::BooleExponent(const self& rhs) :
  m_data(rhs.m_data) {
  
  PBORI_TRACE_FUNC( "BooleExponent(const self&)" );
}

// Destructor
BooleExponent::~BooleExponent() {

  PBORI_TRACE_FUNC( "~BooleExponent()" );
}

// Reducibility test
BooleExponent::bool_type
BooleExponent::reducibleBy(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::reducibleBy(const self&) const" );
  return std::includes(begin(), end(), rhs.begin(), rhs.end());
}
// Reducibility test
BooleExponent::bool_type
BooleExponent::reducibleBy(const monom_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::reducibleBy(const monom_type&) const" );
  return std::includes(begin(), end(), rhs.begin(), rhs.end());
}


// Reducibility test
BooleExponent::bool_type
BooleExponent::reducibleBy(const idx_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::reducibleBy(const idx_type&) const" );

  return (std::find(begin(), end(), rhs) != end());
}

//  Substitute variable with index idx by its complement and assign
BooleExponent&
BooleExponent::changeAssign(idx_type idx) {

  PBORI_TRACE_FUNC( "BooleExponent::changeAssign(idx_type)" );

  const_iterator pos = std::find(begin(), end(), idx);

  if (pos == end())
    insert(idx);
  else
    remove(idx);

  return *this;
}

//  Substitute variable with index idx by its complement
BooleExponent
BooleExponent::change(idx_type idx) const {

  PBORI_TRACE_FUNC( "BooleExponent::change(idx_type) const " );

  const_iterator pos = std::find(begin(), end(), idx);

  return self(*this).changeAssign(idx);
}

//  Insert variable with index idx in exponent vector
BooleExponent
BooleExponent::insertConst(idx_type idx) const {

  PBORI_TRACE_FUNC( "BooleExponent::insertConst(idx_type) const " );

  const_iterator pos =
    std::find_if(begin(), end(), bind2nd(std::greater_equal<idx_type>(), idx));

  self result;
  result.m_data.resize(size() + 1);

  data_type::iterator result_pos = 
    std::copy(begin(), pos, result.m_data.begin());


  if (pos == end())
    *result_pos = idx;
  else if (*pos != idx) {
    *result_pos = idx;
    ++result_pos;
  }
  else
    result.m_data.pop_back();

  std::copy(pos, end(), result_pos);

  return result;
}

//  Insert variable with index idx in exponent vector
BooleExponent&
BooleExponent::insert(idx_type idx) {

  PBORI_TRACE_FUNC( "BooleExponent::insertAssign(idx_type) " );

  iterator pos = 
    std::find_if(internalBegin(), internalEnd(), 
                 bind2nd(std::greater_equal<idx_type>(), idx));

  if (pos == end())
    m_data.push_back(idx);
  else if (*pos != idx)
    m_data.insert(pos, idx);

  return *this;
}
//  Insert variable with index idx in exponent vector (checking end first)
BooleExponent&
BooleExponent::push_back(idx_type idx) {

  PBORI_TRACE_FUNC( "BooleExponent::push_back(idx_type) " );

  if(m_data.empty())
    m_data.push_back(idx);
  else {
    idx_type lastIdx = m_data.back();
    
    if (lastIdx < idx)
      m_data.push_back(idx);
    else if (lastIdx > idx) {
      iterator pos = 
        std::find_if(internalBegin(), internalEnd(), 
                     bind2nd(std::greater_equal<idx_type>(), idx));
      if (*pos != idx)
        m_data.insert(pos, idx);
    }
  }
  return *this;
}

//  Remove variable with index idx in exponent vector
BooleExponent
BooleExponent::removeConst(idx_type idx) const {

  PBORI_TRACE_FUNC( "BooleExponent::removeConst(idx_type) const " );

  const_iterator pos = std::find(begin(), end(), idx);

  self result;
  result.m_data.resize(size());

  data_type::iterator result_pos = 
    std::copy(begin(), pos, result.m_data.begin());

  if(pos != end()){
    ++pos;
    std::copy(pos, end(), result_pos);
    result.m_data.pop_back();
  }

  return result;
}

//  Remove variable with index idx in exponent vector
BooleExponent&
BooleExponent::remove(idx_type idx) {

  PBORI_TRACE_FUNC( "BooleExponent::deleteAssign(idx_type) " );

  iterator pos = std::find(internalBegin(), internalEnd(), idx);

  if (pos != end())
    m_data.erase(pos);

  return *this;
}

template<class ExpType, class RhsType, class ResultType>
void
exp_divide(const ExpType& lhs, const RhsType& rhs, ResultType& result) {

  typedef ExpType self; 
  typedef ResultType data_type; 

  if (lhs.reducibleBy(rhs)) {

    result.reserve(lhs.size());
    std::back_insert_iterator<data_type> outiter(result);
  
    std::set_difference(lhs.begin(), lhs.end(), 
                        rhs.begin(), rhs.end(), outiter);


  }
}

//  Division
BooleExponent
BooleExponent::divide(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::divide(const self&) const " );

  self result;
  exp_divide(*this, rhs, result.m_data);

  return result;
}

//  Division
BooleExponent
BooleExponent::divide(const monom_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::divide(const monom_type&) const " );


  self result;
  exp_divide(*this, rhs, result.m_data);

  return result;
}

// Comparision
BooleExponent::comp_type
BooleExponent::compare(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::compare(const self& rhs) const" );

  return BoolePolyRing::activeOrdering().compare(*this, rhs);
}

// Degree of the lcm
BooleExponent::size_type
BooleExponent::LCMDeg(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::LCMDeg(const self&) const" );

  // temporary definition
  return LCM(rhs).size();
}

// gcd
BooleExponent
BooleExponent::GCD(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::GCD(const self&) const" );

  self result;
  result.m_data.reserve(std::min(size(), rhs.size()));
  std::back_insert_iterator<data_type> outiter(result.m_data);


  std::set_intersection(begin(), end(), rhs.begin(), rhs.end(), outiter);

  return result;
}

// lcm
BooleExponent
BooleExponent::LCM(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::GCD(const self&) const" );

  return multiply(rhs);
}

template<class ExpType, class RhsIterator, class SizeType, class ResultType>
void
exp_multiply(const ExpType& lhs, RhsIterator start, RhsIterator finish,
             SizeType nlen, ResultType& result) {

  typedef ExpType self; 
  typedef ResultType data_type; 

  result.reserve(lhs.size() + nlen);
  std::back_insert_iterator<data_type> outiter(result);

  std::set_union(lhs.begin(), lhs.end(), start, finish, outiter);
}

template<class ExpType, class RhsType, class ResultType>
void
exp_multiply(const ExpType& lhs, const RhsType& rhs, ResultType& result) {

  exp_multiply(lhs, rhs.begin(), rhs.end(), rhs.size(), result);
}

// multiplication
BooleExponent
BooleExponent::multiply(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::multiply(const self&) const" );

  self result;
  exp_multiply(*this, rhs, result.m_data);

  return result;
}

// multiplication
BooleExponent
BooleExponent::multiply(const monom_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::multiply(const monom_type&) const" );

  self result;
  exp_multiply(*this, rhs, result.m_data);

  return result;
}


// multiplication
BooleExponent
BooleExponent::multiplyFirst(const set_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleExponent::multiplyFirst(const set_type&) const" );

  self result;

  exp_multiply(*this, rhs.firstBegin(), rhs.firstEnd(),
               std::distance(rhs.firstBegin(), rhs.firstEnd()), result.m_data);

  return result;
}

// Multiples wrt. given monom
BooleExponent::set_type
BooleExponent::multiples(const self& multipliers) const {

  PBORI_TRACE_FUNC( "BooleExponent::multiples(const self&) const" );


  poly_type theZero(false);
  dd_type result = cudd_generate_multiples(theZero.diagram().manager(),
                                            // BoolePolyRing::activeManager().manager() , //theZero.diagram().manager(),
                                            m_data.rbegin(), m_data.rend(),
                                            multipliers.rbegin(),
                                           multipliers.rend());


   return result;
}

// Divisors
BooleExponent::set_type
BooleExponent::divisors() const {

  PBORI_TRACE_FUNC( "BooleExponent::divisors() const" );

  poly_type theZero(false);
  dd_type result = cudd_generate_divisors(theZero.diagram().manager(),
                                           m_data.rbegin(), m_data.rend() );
  return result;
}

// Print current polynomial to output stream
BooleExponent::ostream_type&
BooleExponent::print(ostream_type& os) const {

  PBORI_TRACE_FUNC( "BooleExponent::print(ostream_type&) const" );

  const_iterator start(begin()), finish(end());

  os << '(';
  
  if (start != finish) {
    --finish;
    std::copy(start, finish, std::ostream_iterator<idx_type>(os, ", "));
    os << *finish;
  }
  os <<')';
  return os;
}

END_NAMESPACE_PBORI
