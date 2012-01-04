// -*- c++ -*-
//*****************************************************************************
/** @file HasTRepOrExtendedProductCriterion.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c HasTRepOrExtendedProductCriterion.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_HasTRepOrExtendedProductCriterion_h_
#define polybori_groebner_HasTRepOrExtendedProductCriterion_h_

// include basic definitions
#include "groebner_defs.h"
#include "GroebnerStrategy.h"

BEGIN_NAMESPACE_PBORIGB


inline bool
extended_product_criterion(const PolyEntry& m, const PolyEntry& m2){
  //BooleMonomial m;
  ///@todo need GCDdeg
  
  bool res=(m.lead.GCD(m2.lead).deg()==common_literal_factors_deg(m.literal_factors, m2.literal_factors));
  //if (res)
  //  cout<<"EXTENDED PRODUCT_CRIT";
  return res;
}


/** @class HasTRepOrExtendedProductCriterion
 * @brief This class defines HasTRepOrExtendedProductCriterion.
 *
 **/
class HasTRepOrExtendedProductCriterion{
public:
  GroebnerStrategy* strat;
  int j;
  HasTRepOrExtendedProductCriterion(GroebnerStrategy& strat, int j){
    this->strat=&strat;
    this->j=j;
  }
  bool operator() (const Monomial &m){
    int i = strat->generators.index(m);
    
    if (strat->pairs.status.hasTRep(i,j))
      return true;
    
    if (extended_product_criterion(const_cast<const GroebnerStrategy*>(strat)->generators[i],const_cast<const GroebnerStrategy*>(strat)->generators[j])){
      strat->pairs.status.setToHasTRep(i,j);
      strat->extendedProductCriterions++;
      return true;
    }
    return false;
  }
  bool operator() (const Exponent &m){
    int i;
    i=strat->generators.index(m);
    
    if (strat->pairs.status.hasTRep(i,j))
      return true;
    
    if (extended_product_criterion(const_cast<const GroebnerStrategy*>(strat)->generators[i],const_cast<const GroebnerStrategy*>(strat)->generators[j])){
      strat->pairs.status.setToHasTRep(i,j);
      strat->extendedProductCriterions++;
      return true;
    }
    return false;
  }

};

END_NAMESPACE_PBORIGB

#endif /* polybori_HasTRepOrExtendedProductCriterion_h_ */
