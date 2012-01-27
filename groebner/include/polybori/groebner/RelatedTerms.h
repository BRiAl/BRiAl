// -*- c++ -*-
//*****************************************************************************
/** @file RelatedTerms.h 
 *
 * @author Alexander Dreyer
 * @date 2012-01-27
 *
 * This file includes the definition of the class @c RelatedTerms.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_RelatedTerms_h_
#define polybori_groebner_RelatedTerms_h_

#include "minimal_elements.h"
#include <polybori/ring/CMemberFunctionTraits.h>

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB


/** @class RelatedTerms
 * @brief This class finds those terms in a given set which are related to
 * given terms
 *
 *  Get interesting leading terms (active ones owning some of @c lead's variables)
 **/
class RelatedTerms {
public:


  RelatedTerms(const Monomial& lead, const MonomialSet& terms):
    m_lead(lead), m_ignorable(lead.ring()),  m_ext_prod_terms(lead.ring()),
    m_related(lead.ring()) { init(terms); }

  /// Constructor   
  RelatedTerms(const Monomial& lead, const MonomialSet& terms,
               const MonomialSet& ignorable):
    m_lead(lead), m_ignorable(ignorable),  m_ext_prod_terms(lead.ring()),
    m_related(lead.ring()) { init(terms.diff(ignorable)); }
  
  /// Access actual monomial set
  operator const MonomialSet&() const { return m_related; }

  /// Minimal related elements from @c terms, with @c lead's variables set to 1
  MonomialSet factors(const MonomialSet& terms) const {
    return minimal_elements_divided(critical_terms_base(terms), m_lead,
                                    m_ext_prod_terms);
  }

protected:
  /// Related elements in @c terms modulo the ignorable elements
  MonomialSet critical_terms_base(const MonomialSet& terms) const {
    return mod_mon_set(m_related.intersect(terms), m_ignorable);
  }

  /// Extract terms not owning variables of current generator's leading term
  MonomialSet unrelated_terms(const MonomialSet& terms) const {
    return std::accumulate(m_lead.begin(), m_lead.end(), terms,
                           member_function_operator(&MonomialSet::subset0) );
  }

private:

  /// Find related elements from terms, store them and return the irrelevant ones
  void init(const MonomialSet& terms) {
    PBORI_ASSERT(!m_ignorable.owns(m_lead) && !terms.ownsOne());
    return init(terms, unrelated_terms(terms));
  }
  
  void init(const MonomialSet& terms, const MonomialSet& unrelated) {
    m_related = terms.diff(unrelated);
    m_ext_prod_terms = m_ignorable.existAbstract(m_lead).diff(unrelated);
  }

  const Monomial& m_lead;

  MonomialSet m_ignorable;
  MonomialSet m_ext_prod_terms; // Contains terms of @c m_ignorable related to
                                // @c m_lead with its variables substituted by one
  MonomialSet m_related;
};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_RelatedTerms_h_ */
