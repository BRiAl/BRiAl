// -*- c++ -*-
//*****************************************************************************
/** @file RelatedTermsBase.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-27
 *
 * This file includes the definition of the class @c RelatedTermsBase.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_RelatedTermsBase_h_
#define polybori_groebner_RelatedTermsBase_h_

#include "minimal_elements.h"
#include <polybori/ring/CMemberFunctionTraits.h>
#include "BoundedDivisorsOf.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class RelatedTermsBase
 * @brief This class deals as a base of @c RelatedTerns.
 **/

class RelatedTermsBase {

public:
  /// Empty set of terms
  RelatedTermsBase(const BoolePolyRing& ring):
    m_lead(ring), m_ignorable(ring), m_ext_prod_terms(ring),
    m_related(ring), m_divisors_of(ring, ring) { }

  /// Nontrivial constructor
  RelatedTermsBase(const Monomial& lead, const MonomialSet& terms,
                   const MonomialSet& ignorable, const MonomialSet& significant):
    m_lead(lead), m_ignorable(ignorable),  m_ext_prod_terms(lead.ring()),
    m_related(lead.ring()), m_divisors_of(lead, significant)  {

    init(significant, unrelated_terms(significant));
  }

  /// finally get the related terms
  const MonomialSet& related_terms() const { return m_related; }

  /// Get parameterized functional
  const BoundedDivisorsOf& divisors_of() const { return  m_divisors_of; }

  /// Minimal related elements from @c terms, with @c lead's variables set to 1
  MonomialSet factors(const MonomialSet& terms) const {
    return minimal_elements_divided(critical_terms_base(terms), m_lead,
                                    m_ext_prod_terms);
  }

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
  /// Find related elements from significant, store them, return irrelevant ones
  void init(const MonomialSet& significant, const MonomialSet& unrelated) {
    PBORI_ASSERT(!m_ignorable.owns(m_lead) && !significant.ownsOne());
    m_related = significant.diff(unrelated);
    m_ext_prod_terms = m_ignorable.existAbstract(m_lead).diff(unrelated);
  }

  Monomial m_lead;
  MonomialSet m_ignorable;
  MonomialSet m_ext_prod_terms; // Contains terms of @c m_ignorable related to
                                // @c m_lead with its variables substituted by 1
  MonomialSet m_related;
  BoundedDivisorsOf m_divisors_of;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_RelatedTermsBase_h_ */
