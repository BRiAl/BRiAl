// -*- c++ -*-
//*****************************************************************************
/** @file CountCriterion.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c CountCriterion.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_CountCriterion_h_
#define polybori_groebner_CountCriterion_h_

// include basic definitions
#include "groebner_defs.h"

#include "GroebnerStrategy.h"
#include "interpolate.h"

BEGIN_NAMESPACE_PBORIGB

/** @class CountCriterion
 * @brief This class defines CountCriterion.
 *
 **/
class CountCriterion{
public:
  GroebnerStrategy* strat;
  int j;
  CountCriterion(GroebnerStrategy& strat, int j){
    this->strat=&strat;
    this->j=j;
  }
  bool operator() (int i){
    const PolyEntry & e1=const_cast<const GroebnerStrategy*>(strat)->generators[i];
      const PolyEntry & e2=const_cast<const GroebnerStrategy*>(strat)->generators[j];
      const int USED_VARIABLES_BOUND=6;
      if ((e1.usedVariables.deg()>USED_VARIABLES_BOUND)||
          (e2.usedVariables.deg()>USED_VARIABLES_BOUND)||
          (e1.usedVariables.LCMDeg(e2.usedVariables) > USED_VARIABLES_BOUND)
          
          
          ||(e1.p.set().nNodes()>30)||(e2.p.set().nNodes()>30))
          return false;

      Exponent uv_exp=e1.usedVariables.LCM(e2.usedVariables);

      MonomialSet space=uv_exp.divisors(e1.p.ring());
      
      Monomial lead_lcm=e1.lead.LCM(e2.lead);
      //I am sure, there exists combinatorial formulas
      MonomialSet common_lead_space=lead_lcm.divisors();
      Monomial gcd=e1.lead.GCD(e2.lead);
      int gcd_deg = gcd_deg;
      int standard_monomials_in_common_lead=
      // common_lead_space.diff(
      //     common_lead_space.multiplesOf(e1.lead)).diff(
      //         common_lead_space.multiplesOf(e2.lead)).size();
      (1<<gcd_deg)*((1<<(e1.leadDeg-gcd_deg))-1+(1<<(e1.leadDeg-gcd_deg)
      )-1)
      +((1<<gcd_deg)-1)*((1<<(e1.leadDeg-gcd_deg))+(1<<(e2.leadDeg-gcd_deg)));

      MonomialSet::size_type standard_monomials =
        (standard_monomials_in_common_lead <<(uv_exp.size()-lead_lcm.deg()));
      
      MonomialSet zeros1=zeros(e1.p, space);
      MonomialSet zeros2=zeros(e2.p, space);
      
      MonomialSet my_zeros=zeros1.intersect(zeros2);
      
      
      // MonomialSet my_zeros=zeros(e1.p, space).intersect(zeros(e2.p, space));
      
      
      if (PBORI_UNLIKELY(standard_monomials==my_zeros.size()))
      {
          strat->pairs.status.setToHasTRep(i,j);
          return true;

      }
      else
          return false;
  }
  bool operator() (const Exponent &m){
    int i = strat->generators.index(m);
    return (*this)(i);
  }


};

END_NAMESPACE_PBORIGB

#endif /* polybori_CountCriterion_h_ */
