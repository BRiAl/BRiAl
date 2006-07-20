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
  std::cout << BoolePolyRing::isOrdered() <<std::endl;
      
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

