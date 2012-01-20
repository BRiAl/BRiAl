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
  ActiveTerms(const PolyEntry& entry, const MonomialSet& leading_terms):
    m_exp(entry.leadExp), m_leading_terms(leading_terms) {}

  MonomialSet operator()(const Exponent& t_divided_exp) const {
    Monomial t_divided(t_divided_exp, m_leading_terms.ring());
    Monomial t(t_divided_exp + m_exp, t_divided.ring());
    return fixed_path_divisors(m_leading_terms, t, t_divided);
  }

private:
  const Exponent& m_exp;
  const MonomialSet& m_leading_terms;
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
  NormalPairsTreatment(const PolyEntry& entry, 
                       const MonomialSet& leading_terms,
                       const MonomialSet& minimal_intersecting_terms_divided):
    m_active_termsets(minimal_intersecting_terms_divided.length(),
		   entry.p.ring()) {

    std::transform(minimal_intersecting_terms_divided.expBegin(),
		   minimal_intersecting_terms_divided.expEnd(),
		   m_active_termsets.begin(),
		   ActiveTerms(entry, leading_terms));
  }
  
  /// Vector-like interface
  //@{
  const_iterator begin() const { return m_active_termsets.begin(); }
  const_iterator end() const { return m_active_termsets.end(); }
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
class ActiveTermsHelper {
public:
  ActiveTermsHelper(const PolyEntry& entry, const MonomialSet& leadingTerms,
		    const MonomialSet& ot2):
    m_entry(entry), m_ot2(ot2), 
    m_ext_prod_terms(entry.p.ring()), m_intersection_terms(entry.p.ring()) {
    
    MonomialSet other_terms(others(leadingTerms));
    if (!ot2.isZero())
      m_ext_prod_terms = ot2.existAbstract(lead()).diff(other_terms);
    
    other_terms = other_terms.unite(m_ext_prod_terms);
    m_intersection_terms = leadingTerms.diff(other_terms).diff(ot2);
    p_leading_terms = (other_terms.ownsOne()? NULL: &leadingTerms);
  }

  const Monomial& lead() const { return m_entry.lead; }
  const MonomialSet& intersecting_terms() const { return m_intersection_terms;}

  MonomialSet critical_terms_base(const MonomialSet& minLeadTerms) const {
    return mod_mon_set(m_intersection_terms.intersect(minLeadTerms), m_ot2);
  }

  bool empty() const { return !p_leading_terms; }

  MonomialSet minimal_elements(const MonomialSet& minLeadTerms) const {
    return minimal_elements_divided(critical_terms_base(minLeadTerms), lead(),
                                    m_ext_prod_terms);
  }

  NormalPairsTreatment operator()(const MonomialSet& minLeadTerms) const {
    return (empty()? NormalPairsTreatment():
	    NormalPairsTreatment(m_entry, *p_leading_terms,
                                 minimal_elements(minLeadTerms)));
  }

private:

  MonomialSet others(const MonomialSet& leadingTerms) const {
    return std::accumulate(lead().begin(), lead().end(), 
			   leadingTerms.diff(m_ot2), Subset0Operator());
  }
  const PolyEntry& m_entry;
  const MonomialSet* p_leading_terms;
  MonomialSet m_ot2;
  MonomialSet m_ext_prod_terms;
  MonomialSet m_intersection_terms;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_NormalPairsTreatment_h_ */
