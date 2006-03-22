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
 * Revision 1.1  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; ring uses shared_ptr now
 *
 * @endverbatim
**/
//*****************************************************************************


// load standard iostream capapilities
#include <iostream>

// load polybori header file
#include "polybori.h"

// cudd interface
#include "CDDManager.h"

USING_NAMESPACE_PBORI

int
main(){

  std::cout << "Testing cudd interface" <<std::endl;   

  try {
    CDDManager<Cudd> man(3);

    CDDInterface<ZDD> dd0 (man.variable(0));
    dd0.print(std::cout);

    CDDInterface<ZDD> dd1 ( man.variable(1));
    dd1.print(std::cout);

    dd0.unateProductAssign(dd1);
    dd0.print(std::cout);

    dd0 = man.variable(0);
    dd0.subset0Assign(1);
    dd0.subset0Assign(2);

    dd0.print(std::cout);

    dd1.subset0Assign(0);
    dd1.subset0Assign(2);

    dd1.print(std::cout);

    dd0.unateProductAssign(dd1);
    dd0.print(std::cout);

  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
