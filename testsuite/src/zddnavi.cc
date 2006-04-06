// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for getting thwe leading term
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
 * Revision 1.2  2006/04/06 15:54:50  dreyer
 * CHANGE testsuite revised
 *
 * Revision 1.1  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.3  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.2  2006/03/30 08:52:57  dreyer
 * CHANGE: testsuite revised
 *
 * Revision 1.1  2006/03/28 07:34:03  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************


// load standard iostream capapilities
#include <iostream>
#include <list>

// load polybori header file
# include "polybori.h"

USING_NAMESPACE_PBORI


template <class NaviType, class TermType, class ListType>
void
find_paths(NaviType navi, const TermType& currentPath, ListType& theList ) {

  if (navi.isConstant()) {      // Reached end of path
    if (navi.terminalValue())   // Case of a valid path
      theList.push_back(currentPath);
  }
  else {
    find_paths(navi.thenBranch(), currentPath.change(*navi), theList);
    find_paths(navi.elseBranch(), currentPath, theList);
  }
}

template <class IteratorType>
void
print_test(IteratorType start, IteratorType end) {

  while(start != end){
    (*start).print(std::cout);
    ++start;
  }
  
}


int
main(){

  std::cout << "Testing leading terms" <<std::endl;   

  try {
    BoolePolyRing the_ring(5);

    BoolePolynomial x = the_ring.variable(0);
    BoolePolynomial y = the_ring.variable(1);

    BoolePolynomial z = BooleVariable(2);

    BoolePolynomial v = BooleVariable(3);
    BoolePolynomial w = BooleVariable(4);


    BoolePolynomial poly;


    poly =  ( x*z  )    * (z + v*w) + y;


   std::cout << poly <<std::endl;

    std::cout << "poly:  "<<std::endl;
    std::cout << poly <<std::endl;

    std::cout << "leading term: "<<std::endl;
    std::cout << poly.lead() <<std::endl;

    std::cout << "Navigation: "<<std::endl;

    BoolePolynomial::navigator navi = poly.navigation();

    std::list<CDDInterface<ZDD> > theList;

    find_paths(navi, BoolePolyRing::ringOne(),  theList);

    print_test(theList.begin(), theList.end());

    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
