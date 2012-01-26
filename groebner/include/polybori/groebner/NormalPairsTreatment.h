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

#include "minimal_elements.h"
#include "fixed_path_divisors.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

class ActiveTerms {
public:
  ActiveTerms(const Monomial& term, const MonomialSet& terms):
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
  NormalPairsTreatment(const Monomial& lead, 
                       const MonomialSet& leading_terms,
                       const MonomialSet& minimal_intersecting_terms_divided):
    m_active_termsets(minimal_intersecting_terms_divided.length(),
                      lead.ring()) {

    std::transform(minimal_intersecting_terms_divided.begin(),
		   minimal_intersecting_terms_divided.end(),
		   m_active_termsets.begin(),
		   ActiveTerms(lead, leading_terms));
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

class Subset0Operator {
public:

  MonomialSet operator()(const MonomialSet& rhs,
                         const MonomialSet::idx_type& idx) {
    return rhs.subset0(idx);
  }
};
class Subset1Operator {
public:

  MonomialSet operator()(const MonomialSet& rhs,
                         const MonomialSet::idx_type& idx) {
    return rhs.subset1(idx);
  }
};
class ActiveTermsHelper {
public:
  ActiveTermsHelper(const Monomial& lead, const MonomialSet& leadingTerms,
		    const MonomialSet& ignorable):

    m_lead(lead), m_ignorable(ignorable), m_leading_terms(leadingTerms),
    m_ext_prod_terms(lead.ring()), m_intersection_terms(lead.ring()) {

    PBORI_ASSERT(!ignorable.owns(lead) && !leadingTerms.ownsOne());

    MonomialSet unrelated(unrelated_leading_terms());
    m_intersection_terms = leadingTerms.diff(unrelated).diff(ignorable);

    /// terms of @c ignorable related to @c lead (divided by @c lead's dividers)
    m_ext_prod_terms = ignorable.existAbstract(lead).diff(unrelated);
  }


  /// Get interesting leading terms (those containing variables of @c lead)
  const MonomialSet& intersecting_terms() const { return m_intersection_terms;}

  /// Construct @c NormalPairsTreatment 
  NormalPairsTreatment operator()(const MonomialSet& minLeadTerms) const {
    return NormalPairsTreatment(m_lead, m_leading_terms,
                                relevant_factors(minLeadTerms));
  }

protected:
  MonomialSet critical_terms_base(const MonomialSet& minLeadTerms) const {
    return mod_mon_set(m_intersection_terms.intersect(minLeadTerms), m_ignorable);
  }

  MonomialSet relevant_factors(const MonomialSet& minLeadTerms) const {
    return minimal_elements_divided(critical_terms_base(minLeadTerms), m_lead,
                                    m_ext_prod_terms);
  }

  /// Leading terms not owning variables of current generator's leading term
  MonomialSet unrelated_leading_terms() const {
     return std::accumulate(m_lead.begin(), m_lead.end(), 
                            m_leading_terms.diff(m_ignorable),
                            Subset0Operator());
  }

private:
  const Monomial& m_lead;
  const MonomialSet& m_leading_terms;
  MonomialSet m_ignorable;
  MonomialSet m_ext_prod_terms;
  MonomialSet m_intersection_terms;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_NormalPairsTreatment_h_ */
