// -*- c++ -*-
//*****************************************************************************
/** @file booleset.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for the Boolean sets.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.8  2009/03/31 08:30:49  dreyer
 * CHANGE: started consistent use of lead* instea of lm*
 *
 * Revision 1.7  2008/11/22 23:47:31  dreyer
 * ADD: BooleSet::rbegin(),end()
 *
 * Revision 1.6  2008/02/28 17:05:48  dreyer
 * Fix: treating constants (0, 1) accordingly
 *
 * Revision 1.5  2007/11/06 15:03:42  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.4  2006/11/29 16:37:35  dreyer
 * CHANGE: updated testsuite
 *
 * Revision 1.3  2006/08/24 14:47:51  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.2  2006/08/15 14:17:29  dreyer
 * ADD minimalElements(), hasTermOfVariables()
 *
 * Revision 1.1  2006/08/09 12:52:32  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * @endverbatim
**/
//*****************************************************************************


// load standard iostream capapilities
#include <iostream>

// load polybori header file
# include "polybori.h"
#include "CacheManager.h"

USING_NAMESPACE_PBORI


int
main(){

  std::cout << "Testing Boolean polynomials" <<std::endl;   

  try {
    BoolePolyRing the_ring(5);

    BooleMonomial x = BooleVariable(0);
    BooleMonomial y = BooleVariable(1);

    BooleMonomial z = BooleVariable(2);

    BooleMonomial v = BooleVariable(3);
    BooleMonomial w = BooleVariable(4);

    BoolePolynomial poly;

    std::cout <<std::endl<<  "BooleSet divisorsOf test"<<std::endl;

    poly = x*y*z;

    BooleSet bset = poly.leadDivisors();

    BooleSet bset2 (bset.divisorsOf(x*y*w)); 
     std::cout <<  bset2 << std::endl;

    std::cout <<   bset.divisorsOf((x*y*w) ) << std::endl;
    
    std::cout <<   bset.hasTermOfVariables((x*y*w) ) << std::endl;

    std::cout <<   "next" << std::endl;

    std::cout <<   bset.hasTermOfVariables((v*w) ) << std::endl;

    std::cout <<   bset.divisorsOf(v*w) << std::endl;

    std::cout <<   bset.intersect((v*w).divisors()) << std::endl;

    std::cout <<   bset.intersect((v*w).divisors()).emptiness() << std::endl;
   std::cout <<   "next" << std::endl;

   std::cout <<   bset.hasTermOfVariables((z) ) << std::endl;


    bset = BoolePolynomial(x*z*w).leadDivisors();

    std::cout <<   bset.divisorsOf((x*y*w) ) << std::endl;
    std::cout <<   bset.divisorsOf((x*y*w) ) << std::endl;

    std::cout <<   "testing minimal elements" << std::endl;
//     poly = x*y + x*y*z +y*v + y*v*w;

//     bset = poly.diagram();

//     std::cout <<   "bset "<<  bset << std::endl;
//     std::cout <<   bset.minimalElements() << std::endl;
//     std::cout <<   bset.minimalElements() << std::endl;

    std::cout <<   "testing next minimal elements" << std::endl;
     poly = y + x*y*z +y*v + y*v*w;

   bset = poly.diagram();
   std::cout <<  "bset "<<  bset << std::endl;
//     std::cout <<   bset.minimalElements() << std::endl;
//     std::cout <<   "second" << std::endl;

    std::cout <<   bset.minimalElements() << std::endl;
     poly = y*v + x*y*z*v*w;

   bset = poly.diagram();
   std::cout <<  "bset "<<  bset << std::endl<< std::endl<< std::endl;;
//     std::cout <<   bset.minimalElements() << std::endl;
//     std::cout <<   "second" << std::endl;

    std::cout <<   bset.minimalElements() << std::endl;
//      std::cout <<   "testing next minimal elements" << std::endl;
//      poly =  x*y*z*v*w + y*w*z*v;

//    bset = poly.diagram();
//     std::cout <<   "bset "<<  bset << std::endl;
//      std::cout <<   bset.minimalElements() << std::endl;
//      std::cout <<   "second" << std::endl;

//     std::cout <<   bset.minimalElements() << std::endl;
//     std::cout <<std::endl<<  "Finished."<<std::endl;

    BooleMonomial rhs;

    poly =  x*y + y*z*v + z*w + y*z*w;
    std::cout <<  "Test exponents of " << poly <<std::endl;

    copy(poly.expBegin(), poly.expEnd(), 
         std::ostream_iterator<BooleExponent>(std::cout, ", "));
 std::cout << std::endl;
    poly =  y*z*v;
    rhs = z;

    std::cout  << "divide " << (poly / rhs) <<" == " <<std::endl;

    std::cout  << (poly.lead().exp().divide(rhs) )<<" ? " <<std::endl;

    std::cout  <<(poly.lead().exp().divide(rhs.exp())) <<" ? "<<std::endl;

    std::cout  << "multiply " << (poly * rhs) <<" == " <<std::endl;

    std::cout  << (poly.lead().exp().multiply(rhs)) <<" ? " <<std::endl;

    std::cout <<(poly.lead().exp().multiply(rhs.exp()) )<<" ? "<<std::endl;
    rhs = w;
    std::cout  << "multiply " << (poly * rhs) <<" == " <<std::endl;

    std::cout  << (poly.lead().exp().multiply(rhs) )<<" ? " <<std::endl;
    
    std::cout <<(poly.lead().exp().multiply(rhs.exp()) )<<" ? "<<std::endl; 
    
    poly =  z*v;
    rhs = x*w;
    
    std::cout  << "multiples " << poly <<" wrt. "<< rhs<<std::endl;
    
    std::cout <<(poly.lead().exp().multiples(rhs.exp()) )<<std::endl; 
    
       
    std::cout <<(poly.lead().multiples(rhs) )<<std::endl; 
    
    std::cout  << "divisors " << poly <<std::endl;
      

    std::cout <<(poly.lead().exp().divisors() )<<std::endl; 

    poly = x * y + y * z + w;

    bset = poly.diagram();

    BooleMonomial monom = x * w* v;

    std::cout  << "multiplesOf " << bset <<"|"<<monom <<std::endl;
    monom = y;

    std::cout  <<bset.multiplesOf(monom) <<std::endl;


    std::cout  << "multiplesOf " << bset <<"|"<<monom <<std::endl;

    std::cout  <<bset.multiplesOf(monom) <<std::endl;

    poly = x * y+x * y*z + y * z*v + w;
    bset = poly.diagram();
    monom = y*z;
    std::cout  << "multiplesOf " << bset <<"|"<<monom <<std::endl;

    std::cout  <<bset.multiplesOf(monom) <<std::endl;
    /*
    monom = BoolePolynomial(0).lead();
    std::cout  << "multiplesOf " << bset <<"|"<<monom <<std::endl;
    */
    std::cout  <<bset.multiplesOf(monom) <<std::endl;
 
    monom = BoolePolynomial(1).lead();;
    std::cout  << "multiplesOf " << bset <<"|"<<monom <<std::endl;

    std::cout << std::endl;

    poly = x * y+x * y*z + y * z*v + w;
    bset = poly.diagram();
    std::cout  << "reversed iterator testing of " <<bset<<std::endl;
    std::cout.flush();
    BooleSet::const_reverse_iterator riter(bset.rbegin());

    std::cout  << *riter <<std::endl;
    ++riter;
    std::cout  << *riter <<std::endl;
    --riter;
    std::cout  << *riter <<std::endl;
    --riter;
    std::cout  << "at end? "<<riter.isZero() <<std::endl;

    ++riter;
    std::cout  << "at end? "<<riter.isZero() <<std::endl;
    std::cout  << *riter <<std::endl; 
 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
