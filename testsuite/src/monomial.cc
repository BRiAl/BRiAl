// -*- c++ -*-
//*****************************************************************************
/** @file monomial.cc 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for the Boolean Variables.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************


// load standard capapilities
#include <iostream>
#include <algorithm>
#include <iterator>

// load polybori header file
# include "polybori.h"

#include "cuddInt.h"
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
    std::cout << (monom = monom.change(2));
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
     // For monomials we throw exception in this case now
     std::cout << (BoolePolynomial(monom) /= denom) <<std::endl;

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


    monom1 = x*y;
    std::cout << "monom1"<<std::endl;
    std::cout << monom1<<std::endl;
    std::cout << "print"<<std::endl;
    std::copy(monom1.begin(), monom1.end(),
            std::ostream_iterator<int>(std::cout, "\n")  );
    
    std::cout <<std::endl<< "Testing lowlevel divisors"<<std::endl;

    BoolePolynomial poly;
    poly = x*z +y +1;

    std::cout <<poly.diagram().firstDivisors() <<std::endl;

    /// ... a second time
    poly.diagram().firstDivisors();

    std::cout << "Testing lowlevel multiples"<<std::endl;
    std::vector<BoolePolynomial::idx_type> multipliers(3);
    multipliers[0]= 1;
    multipliers[1]= 2;
    multipliers[2]= 4;
   
    std::cout <<poly.diagram().firstMultiples(multipliers) <<std::endl;
    poly.diagram().firstMultiples(multipliers);

    monom1 = x*w;
    monom2 = x;

    std::cout <<monom1 <<" < " << monom2 <<std::endl;

    std::cout << (monom1 < monom2 ) <<std::endl;
    std::cout <<monom1 <<" > " << monom2 <<std::endl;

    std::cout << (monom1 > monom2 ) <<std::endl;

    std::cout <<monom1 <<" < " << monom2 <<std::endl;

    std::cout << (monom1.exp() < monom2.exp() ) <<std::endl;
    std::cout <<monom1 <<" > " << monom2 <<std::endl;

    std::cout << (monom1.exp() > monom2.exp() ) <<std::endl;

    std::cout << "Testing popFirst."<<std::endl;

    BooleExponent exp1(monom1.exp());
    std::cout << (monom1) <<std::endl;
    monom1.popFirst();

    std::cout << (monom1) <<std::endl;

    std::cout << "Testing BooleExponent.popFirst()"<<std::endl;

    std::cout << (exp1) <<std::endl;
    exp1.popFirst();

    std::cout << (exp1) <<std::endl;
    monom1 = BooleVariable(1)*BooleVariable(2)*BooleVariable(4);
    std::cout << "Testing BooleMonomial.variableBegin() of "<< 
      monom1<<std::endl;

    BooleMonomial::variable_iterator start(monom1.variableBegin()),
      finish(monom1.variableEnd());

    while (start != finish) {
      std::cout << *start <<" ";
      ++start;
    }
    std::cout << std::endl;

    std::cout << "Finished."<<std::endl;


 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
