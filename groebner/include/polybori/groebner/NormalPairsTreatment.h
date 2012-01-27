// -*- c++ -*-
//*****************************************************************************
/** @file NormalPairsTreatment.h 
 *
 * @author Alexander Dreyer
 * @date 2012-01-19
 *
 * This file includes the definition of the class @c NormalPairsTreatment.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_NormalPairsTreatment_h_
#define polybori_groebner_NormalPairsTreatment_h_

#include "fixed_path_divisors.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

class ActiveTermsOperator {
public:
  ActiveTermsOperator(const Monomial& term, const MonomialSet& terms):
    m_factor(term), m_terms(terms) {}

  MonomialSet operator()(const Monomial& t_divided) const {
    Monomial t = t_divided * m_factor;
    return fixed_path_divisors(m_terms, t, t_divided);
  }

private:
  const Monomial& m_factor;
  const MonomialSet& m_terms;
};

/** @class NormalPairsTreatment
 * @brief This class defines NormalPairsTreatment.
 *
 **/
class NormalPairsTreatment {

public:
  typedef std::vector<MonomialSet> vector_type;
  typedef std::vector<MonomialSet>::const_iterator const_iterator;

  /// Standard constructor yields an empty set
  NormalPairsTreatment():  m_active_termsets() { }

  /// Construct a bunch of active leading term sets wrt. given data
  NormalPairsTreatment(const MonomialSet& minimal_intersecting_terms_divided,
                       const ActiveTermsOperator& func):
    m_active_termsets(minimal_intersecting_terms_divided.length(),
                      minimal_intersecting_terms_divided.ring()) {

    std::transform(minimal_intersecting_terms_divided.begin(),
		   minimal_intersecting_terms_divided.end(),
		   m_active_termsets.begin(), func);
  }
  
  /// Sequence-like interface
  //@{
  const_iterator begin() const { return m_active_termsets.begin(); }
  const_iterator end() const { return m_active_termsets.end(); }
  bool empty() const { return m_active_termsets.empty(); }
  //@}

private:
  vector_type m_active_termsets;
};




END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_NormalPairsTreatment_h_ */
