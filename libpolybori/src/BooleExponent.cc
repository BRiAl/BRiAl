
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
**/
//*****************************************************************************


// load header files
#include <polybori/BooleExponent.h>
#include <polybori/BooleVariable.h>

// get ring definitions
#include <polybori/BoolePolyRing.h>
#include <polybori/orderings/COrderingBase.h>
#include <polybori/except/PBoRiError.h>

#include <polybori/routines/pbori_routines.h>
#include <polybori/routines/pbori_algo.h>
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
// BooleExponent::BooleExponent(bool):
//   m_data()  {

//   PBORI_TRACE_FUNC( "BooleExponent(bool)" );

// }

// Construct from Boolean monomial
BooleExponent&
BooleExponent::get(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BooleExponent::get(const monom_type&)" );

  m_data.resize(rhs.size());  
  std::copy(rhs.begin(), rhs.end(), m_data.begin());

  return *this;
}

// Construct from Boolean monomial
BooleExponent::BooleExponent(const monom_type& rhs): m_data(rhs.size()) {

  PBORI_TRACE_FUNC( "BooleExponent(const monom_type&)" );

  std::copy(rhs.begin(), rhs.end(), m_data.begin());
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

// Degree of the lcm
BooleExponent::deg_type
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
BooleExponent::multiples(const self& multipliers, const ring_type& ring) const {

  PBORI_TRACE_FUNC( "BooleExponent::multiples(const self&, const ring_type&) const" );

  set_type result = cudd_generate_multiples(ring,
                                            m_data.rbegin(), m_data.rend(),
                                            multipliers.rbegin(),
                                            multipliers.rend(),type_tag<set_type>());


   return result;
}
// Multiples wrt. given monom
BooleExponent::set_type
BooleExponent::multiples(const monom_type& mult) const {

  PBORI_TRACE_FUNC( "BooleExponent::multiples(const monom_type&) const" );

  self multipliers(mult);

  set_type result = cudd_generate_multiples(mult.ring(),
                                            m_data.rbegin(), m_data.rend(),
                                            multipliers.rbegin(),
                                            multipliers.rend(),type_tag<set_type>());
  return result;
}

// Divisors
BooleExponent::set_type
BooleExponent::divisors(const ring_type& ring) const {

  PBORI_TRACE_FUNC( "BooleExponent::divisors() const" );

  set_type result = cudd_generate_divisors(ring,
                                           m_data.rbegin(), m_data.rend() ,type_tag<set_type>());
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
