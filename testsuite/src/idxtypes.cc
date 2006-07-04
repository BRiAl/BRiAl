// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains tests for the Boolean Variables.
 *
 * @par Copyright:
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.2  2006/07/04 14:11:03  dreyer
 * ADD: Generic and handy treatment of string literals
 *
 * Revision 1.1  2006/04/12 16:23:54  dreyer
 * ADD template class CIDXPath<>
 *
 * Revision 1.4  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.3  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; ring uses shared_ptr now
 *
 * Revision 1.2  2006/03/17 16:53:37  dreyer
 * ADD added nNodes(), operator*= to BoolePolynomial
 *
 * Revision 1.1  2006/03/16 17:09:13  dreyer
 * ADD BoolePolynial functionality started
 *
 * @endverbatim
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
