// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for the Boolean Variables.
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
 * Revision 1.2  2006/04/20 08:31:22  dreyer
 * ADD BooleMonomial::mulples(...)
 *
 * Revision 1.1  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.13  2006/04/13 07:53:19  dreyer
 * CHANGE BoolePolynomial::print() and deg() produces more useful results
 *
 * Revision 1.12  2006/04/10 14:38:39  dreyer
 * FIX operator*= works for nontrivial lhs
 *
 * Revision 1.11  2006/04/06 15:54:50  dreyer
 * CHANGE testsuite revised
 *
 * Revision 1.10  2006/04/05 14:56:38  dreyer
 * ADD prettyPrint functions for dot-output to stdout or file
 *
 * Revision 1.9  2006/04/04 12:07:37  dreyer
 * ADD BoolePolynomial::reducibleby(), and firstBegin(), firstEnd()
 *
 * Revision 1.8  2006/04/04 11:21:22  dreyer
 * ADD lmDivisors() added
 *
 * Revision 1.7  2006/04/04 07:36:35  dreyer
 * ADD: tests isZero(), isOne() and poly == bool, bool == poly
 *
 * Revision 1.6  2006/04/03 10:45:57  dreyer
 * ADD: custom python-seach prefix PYPREFIX
 *
 * Revision 1.5  2006/03/27 15:02:43  dreyer
 * ADD: BoolePolynomial::operator/=(const self&) and spoly
 *
 * Revision 1.4  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.3  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.2  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; 
 * ring uses shared_ptr now
 *
 * Revision 1.1  2006/03/20 10:40:22  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************


// load standard capapilities
#include <iostream>
#include <algorithm>
#include <iterator>

// load polybori header file
# include "polybori.h"

USING_NAMESPACE_PBORI

int
main(){

  std::cout << "Testing Boolean monomials" <<std::endl;   

  try {
    BoolePolyRing the_ring(5);

    BooleVariable x = BooleVariable(0);
    BooleVariable y = BooleVariable(1);

    BooleVariable z = BooleVariable(2);

    BooleVariable v = BooleVariable(3);
    BooleVariable w = BooleVariable(4);

    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << z << std::endl;
    std::cout << v << std::endl;
    std::cout << w << std::endl;

    std::cout <<std::endl<<  "Monomial x*y: ";

    BooleMonomial monom = x;
    monom*= (BooleMonomial)y;
    std::cout << monom <<std::endl;

    std::cout <<std::endl<<  "Monomial x*y*z: ";
    std::cout << monom.changeAssign(2);
    std::cout << " degree: "<< monom.deg() <<std::endl;
    std::cout << "(Sum of) Divisors: "<<std::endl<<monom.divisors()<<std::endl;

    std::cout <<std::endl<<  "Indices:"<<std::endl;
    std::copy(monom.begin(), monom.end(), 
              std::ostream_iterator<int>(std::cout, "\n"));

    BooleMonomial  denom = y;
    denom *= (BooleMonomial)z;
    monom *= (BooleMonomial)w;

    std::cout << "x*y*z*w / y*z: ";
    std::cout << (monom /= denom) <<std::endl;
    std::cout <<  "x / x: ";
    monom = x;
    std::cout << (monom /= BooleMonomial(x)) <<std::endl;
    std::cout << "x / y*z: ";
    monom = x;
    std::cout << (monom /= denom) <<std::endl;

    std::cout << "x * y (as BooleVariable): ";
    monom = x;
    std::cout << (monom *= y) <<std::endl;
    monom*=z;
    std::cout << "x*y*z / y (as BooleVariable): ";
    std::cout << (monom /= y) <<std::endl;

    monom = x;
    monom *=y;
    monom *=w;

    BooleMonomial monom1 = x;
    monom1 *=y;
    monom1 *=w;

    BooleMonomial monom2 = x;
    monom2 *=y;
    monom2 *=z;
    std::cout << monom1 <<".compare( " <<monom<<") :";
    std::cout << monom1.compare(monom)<<std::endl;

    std::cout << monom1 <<".compare( " <<monom2<<") :";
    std::cout << monom1.compare(monom2)<<std::endl;

    std::cout << monom2 <<".compare( " <<monom1<<") :";
    std::cout << monom2.compare(monom1)<<std::endl;

    std::cout << monom1 <<" < " <<monom2<<" :";
    std::cout << (monom1 < monom2)<<std::endl;
    std::cout << monom1 <<" > " <<monom2<<" :";
    std::cout << (monom1 > monom2)<<std::endl;
    std::cout << monom1 <<" <= " <<monom2<<" :";
    std::cout << (monom1 <= monom2)<<std::endl;
    std::cout << monom1 <<" >= " <<monom2<<" :";
    std::cout << (monom1 >= monom2)<<std::endl;
    std::cout << monom1 <<" == " <<monom2<<" :";
    std::cout << (monom1 == monom2)<<std::endl;
    std::cout << monom1 <<" < " <<monom<<" :";
    std::cout << (monom1 < monom)<<std::endl;
    std::cout << monom1 <<" > " <<monom<<" :";
    std::cout << (monom1 > monom)<<std::endl;
    std::cout << monom1 <<" <= " <<monom<<" :";
    std::cout << (monom1 <= monom)<<std::endl;
    std::cout << monom1 <<" >= " <<monom<<" :";
    std::cout << (monom1 >= monom)<<std::endl;
    std::cout << monom1 <<" == " <<monom<<" :";
    std::cout << (monom1 == monom)<<std::endl<<std::endl;
    monom1*= v;    std::cout << monom1<<std::endl;
    std::cout << monom1 <<".LCMDeg( " <<monom2<<") :";
    std::cout << monom1.LCMDeg(monom2)<<std::endl;
    std::cout << monom2 <<".LCMDeg( " <<monom1<<") :";
    std::cout << monom2.LCMDeg(monom1)<<std::endl;

    std::cout << "GCD( "<<monom1 <<", "<<monom2<<") :";
    std::cout << GCD(monom1, monom2)<<std::endl;
    monom1 /= x;
    std::cout << "GCD( "<<monom1 <<", "<<monom2<<") :";
    std::cout << GCD(monom1, monom2)<<std::endl;
    monom2 *= w;
    std::cout << "GCD( "<<monom1 <<", "<<monom2<<") :";
    std::cout << GCD(monom1, monom2)<<std::endl;

    monom1 = x*y;
    monom2 = y*z*w;

    std::cout << std::endl;
    std::cout << "Multiples of "<< monom1 << "wrt. "<< monom2<<": "<<std::endl;
    std::cout << monom1.multiples(monom2);

    std::cout <<std::endl << "Finished."<<std::endl;
 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
