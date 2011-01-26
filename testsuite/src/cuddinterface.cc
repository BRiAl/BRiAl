// -*- c++ -*-
//*****************************************************************************
/** @file cuddinterface.cc
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
#include "polybori.h"

// cudd interface


USING_NAMESPACE_PBORI

int
main(){

  std::cout << "Testing cudd interface" <<std::endl;   


  BoolePolyRing r(17, CTypes::lp);
  BoolePolyRing r2=r;//ooleEnv::ring();


  r2.changeOrdering(CTypes::dlex); 


  
  r.ordering();
  
  return 0;

  BoolePolyRing backup_ring=BooleEnv::ring();

  std::vector<BooleMonomial> vec(1);

  BoolePolyRing q = r.clone();

  BooleEnv::set(q);
  //  q.activate();
  q.changeOrdering(CTypes::lp);

  vec.resize(17);

  std::cerr << vec[0] <<std::endl;
  std::cerr << vec[3] <<std::endl;



  //  assert(false);// test deactivated
#if 0

  try {
    CDDManager<CTypes::manager_base> man(3);

    CTypes::dd_type dd0 (man.ddVariable(0));
    std::cout << dd0;

    CTypes::dd_type dd1 ( man.ddVariable(1));
    std::cout << dd1;

    dd0.unateProductAssign(dd1);
    std::cout << dd0;

    dd0 = man.ddVariable(0);
    dd0.subset0Assign(1);
    dd0.subset0Assign(2);

    std::cout << dd0;
    dd1.subset0Assign(0);
    dd1.subset0Assign(2);

     std::cout << dd1;

    dd0.unateProductAssign(dd1);
    std::cout << dd0;

    dd0.uniteAssign( man.ddVariable(2) );
    std::cout << dd0;

  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }
#endif
  return 0;
}
