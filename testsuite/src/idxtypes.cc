// -*- c++ -*-
//*****************************************************************************
/** @file idxtypes.cc 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains tests for the Boolean Variables.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************


// load standard iostream capapilities
#include <iostream>

// load polybori header file
# include "polybori.h"
# include "pbori_func.h"

#include "CIdxPath.h"
#include "CStringLiteral.h"

USING_NAMESPACE_PBORI

int
main(){

  std::cout << "Testing CIdxPath, CIdx" <<std::endl;   

  try {

    typedef CStringLiteral<CLiteralCodes::list_separator> list_separator;
    CIdxPath<CIdxVariable<int>, list_separator> storage(5);

    storage[1]=1;
    storage[2]=17;
    storage[3]=5;
    std::cout <<"Print list style: "<<std::endl;

    std::cout <<storage<<std::endl;
    typedef CStringLiteral<CLiteralCodes::times> times_as_separator;

    CIdxPath<CIdxVariable<int>, times_as_separator> storage2(5);

    storage2[1]=1;
    storage2[2]=17;
    storage2[3]=5;
    std::cout <<"Print monomial style: "<<std::endl;
    std::cout <<storage2<<std::endl;
    std::cout <<"Finished. "<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
