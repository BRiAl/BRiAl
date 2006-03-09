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


  BoolePolyRing ring(4);

  int verbosity = 2;
  std::cout << "Variable 0:"<<std::endl;
  ring.variable(0).print(3, verbosity);

  std::cout << "Variable 1:"<<std::endl;
  ring.variable(1).print(3, verbosity);

  std::cout << "Variable 2:"<<std::endl;
  ring.variable(2).print(3, verbosity);

  std::cout << "Variable 3:"<<std::endl;
  ring.variable(3).print(3, verbosity);

  return 0;
}

