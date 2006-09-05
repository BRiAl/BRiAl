// -*- c++ -*-
// $Id$
//
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains tests for the class BoolePolyRing.
 *
 *  (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
**/
//*****************************************************************************
//
// Last edit by $Author$ on $Date$
// 
// $Log$
// Revision 1.7  2006/09/05 11:10:45  dreyer
// ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
//
// Revision 1.6  2006/09/05 08:48:34  dreyer
// ADD: BoolePolyRing::is(Total)DegreeOrder()
//
// Revision 1.5  2006/07/20 08:55:49  dreyer
// ADD isOrdered() and  isLexicographical()
//
// Revision 1.4  2006/04/11 09:26:57  dreyer
// ADD  BoolePolyRing::printInfo();
//
// Revision 1.3  2006/03/20 14:51:01  dreyer
// CHANGE: Use CDDInterface temple specializations instead of raw dd_type
//
// Revision 1.2  2006/03/13 12:27:24  dreyer
// CHANGE: consistent function names
//
// Revision 1.1.1.1  2006/03/09 14:35:25  dreyer
// + Project started
//

// load standard iostream capapilities
#include <iostream>

// load polybori header file
# include "polybori.h"

USING_NAMESPACE_PBORI

int
main(){

  try{
    BoolePolyRing::ring();
  }
  catch(PBoRiError err){
    std::cout << "Exception caught sucessfully: "<<err.text() <<std::endl;
  }

  BoolePolyRing ring(4);

  int verbosity = 2;
  std::cout << "Variable 0:"<<std::endl;
  ((ZDD)ring.variable(0)).print(3, verbosity);

  std::cout << "Variable 1:"<<std::endl;
  ((ZDD)ring.variable(1)).print(3, verbosity);

  std::cout << "Variable 2:"<<std::endl;
  ((ZDD)ring.variable(2)).print(3, verbosity);

  std::cout << "Variable 3:"<<std::endl;
  ((ZDD)ring.variable(3)).print(3, verbosity);

  BoolePolyRing::printInfo();

  std::cout << "is lexicographical?" <<std::endl;
  std::cout << BoolePolyRing::isLexicographical() <<std::endl;

  std::cout << "is ordered?" <<std::endl;
  std::cout << BoolePolyRing::orderedStandardIteration() <<std::endl;

  std::cout << "is symmetric?" <<std::endl;
  std::cout << BoolePolyRing::isSymmetric() <<std::endl;  

  std::cout << "is degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isDegreeOrder() <<std::endl;  
  std::cout << "is total degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isTotalDegreeOrder() <<std::endl;  
  std::cout << "has descending variables?" <<std::endl;
  std::cout << BoolePolyRing::descendingVariables() <<std::endl;  
  std::cout << "has ascending variables?" <<std::endl;
  std::cout << BoolePolyRing::ascendingVariables() <<std::endl;  

  try{
    BoolePolyRing::ring();
  }
  catch(PBoRiError err){
    std::cout << "Warning! Accessing global ring failed. ";
    std::cout << "Unexpected exception occured: ";
    std::cout <<err.text() <<std::endl;
  }

  BoolePolyRing ring2(5, CTypes::dlex);
  std::cout << "Testing DegLex ring... "<<std::endl;;

  std::cout << "is lexicographical?" <<std::endl;
  std::cout << BoolePolyRing::isLexicographical() <<std::endl;

  std::cout << "is ordered?" <<std::endl;
  std::cout << BoolePolyRing::orderedStandardIteration() <<std::endl;

  std::cout << "is symmetric?" <<std::endl;
  std::cout << BoolePolyRing::isSymmetric() <<std::endl;  
  std::cout << "is degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isDegreeOrder() <<std::endl;  
  std::cout << "is total degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isTotalDegreeOrder() <<std::endl;  
  std::cout << "has descending variables?" <<std::endl;
  std::cout << BoolePolyRing::descendingVariables() <<std::endl;  
  std::cout << "has ascending variables?" <<std::endl;
  std::cout << BoolePolyRing::ascendingVariables() <<std::endl;  

  BooleMonomial x = BooleVariable(0);
  BooleMonomial y = BooleVariable(1);
  BooleMonomial z = BooleVariable(2);
  BooleMonomial v = BooleVariable(3);
  BooleMonomial w = BooleVariable(4);

  BoolePolynomial poly =  x*y + z + z*v*w;

  std::cout << "poly " << poly <<std::endl;

  std::cout << "lead() " << poly.lead() <<std::endl;
  std::cout << "leadExp() " << poly.leadExp() <<std::endl;
  std::cout << "lmDeg() " << poly.lmDeg() <<std::endl;
  std::cout << "lmDivisors() " << poly.lmDivisors() <<std::endl;

  try{
    BoolePolyRing::ring();
  }
  catch(PBoRiError err){
    std::cout << "Warning! Accessing global ring failed. ";
    std::cout << "Unexpected exception occured: ";
    std::cout <<err.text() <<std::endl;
  }

  return 0;
}

