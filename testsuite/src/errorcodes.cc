// -*- c++ -*-
// $Id$
//
//*****************************************************************************
/** @file errornodes.cc 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains tests for the class PBoRiError.
 *
 *  (c) 2006 by The PolyBoRi Team
 *
**/
//*****************************************************************************
//
// Last edit by $Author$ on $Date$
// 
// $Log$
// Revision 1.3  2007/11/06 15:03:42  dreyer
// CHANGE: More generic copyright
//
// Revision 1.2  2006/03/16 13:37:41  dreyer
// ADD: added error codes and PBORI_DEVELOPER marker
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

  std::cout << "Testing important error codes" <<std::endl;   

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

  std::cout <<std::endl << "Testing all error codes " <<std::endl;   

  for(CTypes::errornum_type i_err = 0; i_err < CTypes::last_error; ++i_err) {
    try {
      std::cout << "Throwing error # "<< i_err <<std::endl;
      throw PBoRiError(i_err);
      std::cout << "--------> Should not reach here!'"<<std::endl;
    }
    catch (PBoRiError& err) {
      std::cout << "  Caught error # "<< err.code() <<std::endl;   
      std::cout << "    which says: "<< err.text() <<std::endl;  
    }
  }

  return 0;
}
