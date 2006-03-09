// -*- c++ -*-
// $Id$
//
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains tests for the class PBoRiError.
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
// Revision 1.1  2006/03/09 14:35:25  dreyer
// Initial revision
//

// load standard iostream capapilities
#include <iostream>

// load polybori header file
# include "polybori.h"

USING_NAMESPACE_PBORI

int
main(){

  try {
    std::cout << "Throwing 'alright'"<<std::endl;
    throw PBoRiError(CTypes::alright);
    std::cout << "--------> Should not reach here!'"<<std::endl;
 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  try {
    std::cout << "Throwing 'failed'"<<std::endl;
    throw PBoRiError(CTypes::failed);
    std::cout << "--------> Should not reach here!'"<<std::endl;
 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
  }

  try {
    std::cout << "Throwing 'no_ring'"<<std::endl;
    throw PBoRiError(CTypes::no_ring);
    std::cout << "--------> Should not reach here!'"<<std::endl;
 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
  }


  return 0;
}
