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

/** @class NormalPairsTreatmentBase
 * @brief This class defines the base of @c NormalPairsTreatment.
 *
 **/

#ifdef EXP_FOR_PAIRS
class NormalPairsTreatmentBase {
public:
  typedef Exponent value_type;
  void init(std::vector<value_type>& mt_vec,
            const Monomial& lead, const MonomialSet& minimal_intersecting_terms,
            const MonomialSet& ext_prod_terms) {
    minimal_elements_divided(minimal_intersecting_terms, lead, ext_prod_terms,
                             mt_vec);
  } 

  MonomialSet terms(const Exponent& t_divided_exp, const PolyEntry& entry,
                    const MonomialSet& leading_terms) const {
    Monomial t_divided(t_divided_exp, leading_terms.ring());
    Monomial t(t_divided_exp + entry.leadExp,t_divided.ring());
    return fixed_path_divisors(leading_terms, t, t_divided);
  }
};

#else
class NormalPairsTreatmentBase {
public:
  typedef Monomial value_type;
  void init(std::vector<value_type>& mt_vec, 
            const Monomial& lead, const MonomialSet& minimal_intersecting_terms,
            const MonomialSet& ext_prod_terms) {
    mt_vec = minimal_elements_multiplied(minimal_intersecting_terms, lead);
  }

  MonomialSet terms(const Monomial& t, const PolyEntry& entry,
                    const MonomialSet& leading_terms) const {
    Monomial t_divided = t / entry.lead;
    PBORI_ASSERT(t.reducibleBy(entry.lead));
    return fixed_path_divisors(leading_terms, t, t_divided);
  }
};
#endif

/** @class NormalPairsTreatment
 * @brief This class defines NormalPairsTreatment.
 *
 **/
class NormalPairsTreatment: 
  public NormalPairsTreatmentBase {

public:
  typedef std::vector<MonomialSet>::const_iterator const_iterator;

  /// Standard constructor yields an empty set
  NormalPairsTreatment():  m_active_terms() { }

  /// Construct a bunch of active leading term sets wrt. given data
  NormalPairsTreatment(const PolyEntry& entry, 
                       const MonomialSet& leading_terms,
                       const MonomialSet& minimal_intersecting_terms,
                       const MonomialSet& ext_prod_terms):
    m_active_terms() {

    std::vector<value_type> mt_vec;
    init(mt_vec, entry.lead, minimal_intersecting_terms, ext_prod_terms);
    m_active_terms.resize(mt_vec.size(), leading_terms.ring());
    active_terms(mt_vec.begin(), mt_vec.end(), m_active_terms.begin(),
                 entry, leading_terms);
  }

  /// Iterate result
  //@{
  const_iterator begin() const { return m_active_terms.begin(); }
  const_iterator end() const { return m_active_terms.end(); }
  //@}

private:
  template <class Iterator, class OutIter>
  void active_terms(Iterator start, Iterator finish, OutIter iter,
                    const PolyEntry& entry,
                    const MonomialSet& leading_terms) const {
    for(; start != finish; ++start, ++iter)
      *iter = terms(*start, entry, leading_terms);
  }

  std::vector<MonomialSet> m_active_terms;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_NormalPairsTreatment_h_ */
