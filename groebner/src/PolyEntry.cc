// -*- c++ -*-
//*****************************************************************************
/** @file PolyEntry.cc 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PolyEntry.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************


// include basic definitions
#include <polybori/groebner/PolyEntry.h>

BEGIN_NAMESPACE_PBORIGB

PolyEntry::PolyEntry(const Polynomial &poly):
  literal_factors(poly),
  p(poly), usedVariables(poly.usedVariablesExp()),
  deg(poly.deg()), length(poly.length()), minimal(true),
  // empty/zero default values to be filled below (TODO: use inheritance here)
  lead(poly.ring()), leadExp(), leadDeg(), tail(poly.ring()), 
  weightedLength(), tailVariables(), gcdOfTerms(poly.ring()) {

  this->lead=p.boundedLead(deg);
  this->leadExp=lead.exp();
  this->leadDeg=leadExp.deg();

  if (leadDeg==deg)
    this->weightedLength=this->length;
  else
    this->weightedLength=poly.eliminationLengthWithDegBound(deg);
  
  tail=poly-lead;
  this->tailVariables=tail.usedVariablesExp();
}

void PolyEntry::recomputeInformation(){
  PBORI_ASSERT(this->lead==p.lead());
  if (!(this->p.ring().ordering().isDegreeOrder()))
      this->deg=p.deg();
  //so also lmExp keeps constant
  this->length=p.length();
  if (leadDeg==deg)
    this->weightedLength=this->length;
  else
    this->weightedLength=p.eliminationLengthWithDegBound(deg);
  
  this->usedVariables=p.usedVariablesExp();
  tail=p-lead;
  this->tailVariables=tail.usedVariablesExp();
  this->literal_factors=LiteralFactorization(p);
  //minimal keeps constant
  PBORI_ASSERT(this->leadDeg==p.leadDeg());
}
END_NAMESPACE_PBORIGB
