// -*- c++ -*-
//*****************************************************************************
/** @file boolepoly.cc 
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


// load standard iostream capapilities
#include <iostream>

// load polybori header file
# include "polybori.h"

#include <cache/CCacheManagement.h>

USING_NAMESPACE_PBORI

# include <routines/pbori_routines.h>
template <class NaviType>
NaviType
test_union_xor(NaviType first, NaviType second) {


}

template <class Type>
int testit(const char[sizeof(&Type::print)]) {
  return 1;
}

template <class Type>
int testit(int) {
  return 0;
}

template <class T>
class any: public T{
public:
template<class T2>
any(const T2&) {}
  typedef any self;
public:
  //  using T::print;
  bool print(const any&) const { return false; }
  any(){}

  self* operator->() const {return this;}
};

template <class T>
class some {
public:

public:
  enum {thesize = sizeof( (any<T>())->print(std::cout))} ;
};


// template<int Size>
// class func_traits_{};





// template<>
// class func_traits_<16>  {
// public:
//   typedef int result_type;
// };



// template<class Type>
// class func_traits:
//   public func_traits_<sizeof(&Type::print)> {
// };


// template <class Type1, class Type2>
// void print_it(const Type1&, const Type2) {
//   std::cout <<"general"<<std::endl;
// }



// template <class Type1, class Type2>
// void print_it(const Type1&, Type2(Type1:: func*)(Type2) = Type1::print) {
//   std::cout <<"special"<<std::endl;
// }


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

    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << z << std::endl;
    std::cout << v << std::endl;
    std::cout << w << std::endl;


    BoolePolynomial poly = x;
    poly *= y;
    std::cout << "x*y : "<<std::endl;
    std::cout << poly <<std::endl;

    poly += z;

    std::cout << "x*y + z: "<<std::endl;
    std::cout << poly <<std::endl;


    poly = x;
    poly += y;

    poly *= z;

    std::cout << "(x + y)* z: "<<std::endl;
    std::cout << poly <<std::endl;

    poly = x;
    poly *= v;

    BoolePolynomial poly2 = y;
    poly2 *= v;

    poly += poly2;

    poly += z;

    BooleMonomial poly3 = y;
    poly3 *= v;


    std::cout << "(x*v + y*v + z ) "<<std::endl;
    std::cout << poly <<std::endl;

    poly *= poly3;
    std::cout << "(x*v + y*v + z )* (v*y): "<<std::endl;
    std::cout << poly <<std::endl;

    std::cout << "Node count: "<< poly.nNodes() <<std::endl;
    std::cout << "N: "<< BooleEnv::nVariables() <<std::endl;

    std::cout << std::endl << "(x*v + y*v + z )* (v*y): " <<std::endl;
    std::cout <<  (x*v + y*v + z )* (v*y) <<std::endl;

    std::cout <<  "Equality? "  <<std::endl;
    std::cout <<  (poly == ((x*v + y*v + z )* (v*y)) ) <<std::endl;

    std::cout << std::endl << "Nonequality? "  <<std::endl;
    std::cout <<  (poly != ((x*v + y*v + z )* (v*y)) ) <<std::endl;

    std::cout << std::endl << "Division ok? "  <<std::endl;
    std::cout <<  ( ( (x*y*z + x*z*w +v) / (x*z) ) == ( y + w ) ) <<std::endl;

    std::cout << std::endl;

    poly = BoolePolynomial();

    std::cout << "Zero polynomial: isZero? ";
    std::cout <<  poly.isZero() <<std::endl;

    std::cout << "Zero polynomial: isOne? ";
    std::cout <<  poly.isOne() <<std::endl;

    std::cout << "Zero polynomial == 0? ";
    std::cout <<  (poly == 0) <<std::endl;

    std::cout << "Zero polynomial == 1? ";
    std::cout <<  (poly == 1) <<std::endl;

    std::cout << "Constant 0 == Zero polynomial? ";
    std::cout <<  (0 == poly) <<std::endl;

    std::cout << "Constant 1 == Zero polynomial? ";
    std::cout <<  (1 == poly) <<std::endl;

    poly = BoolePolynomial(1);

    std::cout << "One polynomial: isZero? ";
    std::cout <<  poly.isZero() <<std::endl;

    std::cout << "One polynomial: isOne? ";
    std::cout <<  poly.isOne() <<std::endl;

    std::cout << "One polynomial == 0? ";
    std::cout <<  (poly == 0) <<std::endl;

    std::cout << "One polynomial == 1? ";
    std::cout <<  (poly == 1) <<std::endl;

    std::cout << "Constant 0 == One polynomial? ";
    std::cout <<  (0 == poly) <<std::endl;

    std::cout << "Constant 1 == One polynomial? ";
    std::cout <<  (1 == poly) <<std::endl;
    
    std::cout <<"Divisors of the leading monomial of  (x*z + y): "<<std::endl;
    std::cout <<   (x*z + y).leadDivisors()  <<std::endl;

    std::cout <<  "(x*z + y).firstReducibleBy(z): ";
    std::cout <<  (x*z + y).firstReducibleBy(z)  <<std::endl;

    std::cout <<  "(x*z*y + y).firstReducibleBy(y*z): ";
    std::cout <<   (x*z*y + y).firstReducibleBy(y*z)  <<std::endl;

    std::cout <<  "(x*z*y + y).firstReducibleBy(w): ";
    std::cout <<   (x*z*y + y).firstReducibleBy(w)  <<std::endl;

    std::cout <<  "(x*y + y).firstReducibleBy(z+w): ";
    std::cout <<   (x*y + y).firstReducibleBy(z+w)  <<std::endl;

    std::cout <<  "Multiplication tests: "<<std::endl;
    std::cout << ( ((x*y + x)*y) == 0) <<std::endl;
    std::cout << ( ((x*y + x + y*z)*y) == y*z) <<std::endl;
    std::cout << ( (x*y*z + y + x*z + v*w +x*v)*(y*z)  == 
                   (y*z + y*z*v*w + x*y*z*v) ) <<std::endl;

    std::cout <<  "Degree: x + y*z "<<std::endl;
    std::cout << "2? " << (x + y*z).deg() <<std::endl;

    
    std::cout <<  "All degrees: x + y*z "<<std::endl;
    poly = x + y*z;

    std::copy(poly.degBegin(), poly.degEnd(), 
              std::ostream_iterator<unsigned>(std::cout, "\n"));

    std::cout <<  "Degree: x + y*z + 1 "<<std::endl;
    std::cout << "2? " << (x + y*z +1).deg() <<std::endl;

    std::cout <<  "All degrees: x + y*z + 1"<<std::endl;
    poly = x + y*z +1;

    std::copy(poly.degBegin(), poly.degEnd(), 
              std::ostream_iterator<unsigned>(std::cout, "\n"));


    std::copy(poly.begin(), poly.end(), 
              std::ostream_iterator<BooleMonomial>(std::cout, "\n"));
  
    poly = 1;
    std::copy(poly.begin(), poly.end(), 
              std::ostream_iterator<BooleMonomial>(std::cout, "\n"));
    poly = 0;
    std::copy(poly.begin(), poly.end(), 
              std::ostream_iterator<BooleMonomial>(std::cout, "\n"));

    poly = x + y*z +1;
    std::cout <<  poly <<" hasConstantTerm() ?"<<std::endl;
    std::cout <<  poly.hasConstantPart() <<std::endl;

    poly = x + y*z;
    std::cout <<  poly <<" hasConstantTerm() ?"<<std::endl; 
    std::cout <<  poly.hasConstantPart() <<std::endl;
 
    poly =  y*z +1;
    std::cout <<  poly <<" hasConstantTerm() ?"<<std::endl;
    std::cout <<  poly.hasConstantPart() <<std::endl;

    poly =  y*z;
    std::cout <<  poly <<" hasConstantTerm() ?"<<std::endl; 
    std::cout <<  poly.hasConstantPart() <<std::endl;

    poly = 1;
    std::cout <<  poly <<" hasConstantTerm() ?"<<std::endl; 
    std::cout <<  poly.hasConstantPart() <<std::endl;

    poly = 0;
    std::cout <<  poly <<" hasConstantTerm() ?"<<std::endl; 
    std::cout <<  poly.hasConstantPart() <<std::endl;

    poly  = x*y + y*z;

    poly2 = z*w +v*w;

    std::cout <<  poly <<" * " << poly2 <<std::endl; 
    std::cout << poly * poly2 <<std::endl;





    std::cout <<std::endl<<  "Going on..."<<std::endl;
#if 0
    CCacheManagement<CCacheTypes::dp_asc_lead> 
      test_marker(BooleEnv::manager().manager());

    CCacheManagement<CCacheTypes::dlex_lead> 
      test_marker2(BooleEnv::manager().manager());

    std::cout <<  (( test_marker.find( poly.navigation() )).getNode()==NULL)<<std::endl;

      test_marker.insert(poly.navigation(), poly.navigation() );
      std::cout <<  (test_marker.find( poly.navigation() ).getNode()!=NULL)<<std::endl;
      std::cout <<  (test_marker2.find( poly.navigation() ).getNode()==NULL)<<std::endl;
      poly = x*y*z +y*z +y*v + y*w;
      poly2 = x*y+y*z +y*w +v*w;
      std::cout << poly<<std::endl;
      std::cout << poly2<<std::endl;


      BoolePolynomial result = poly + poly2;
     

 std::cout <<  "result " <<result<<std::endl;


  std::cout <<  "test newnode."<<std::endl;

  poly = y*z + z + w;

  poly2 =  w +v ;
  
  std::cout << "newnode "<< 0 << " " <<poly << " " << poly2<<std::endl;
  
  CTypes::dd_type ddtest(0, poly.diagram(), poly2.diagram());
  std::cout <<  "result " << BoolePolynomial(ddtest) <<std::endl;
  
  std::cout <<  "additional properties: " <<std::endl;
  std::cout << (x*y + 0).isSingleton() <<std::endl;
  std::cout << (x*y + 1).isSingleton() <<std::endl;
  std::cout << (x*y + 0).isSingletonOrPair() <<std::endl;
  std::cout << (x*y + 1).isSingletonOrPair() <<std::endl;
  std::cout << (x*y + x + 0).isSingletonOrPair() <<std::endl;
  std::cout << (x*y + x + 1).isSingletonOrPair() <<std::endl;
  std::cout << (x*y + 0).isPair() <<std::endl;
  std::cout << (x*y + 1).isPair() <<std::endl;
  std::cout << (x*y + x + 0).isPair() <<std::endl;
  std::cout << (x*y + x + 1).isPair() <<std::endl;


  std::cout <<  "graded part: " <<std::endl;
  std::cout << (x*y + x + 1).gradedPart(1) <<std::endl;
  std::cout << ((x*y + x + 1).gradedPart(1)  == x)<<std::endl;
  std::cout << (x*y + x + 1).gradedPart(2) <<std::endl;
  std::cout << ((x*y + x + 1).gradedPart(2) == x*y)<<std::endl;
  std::cout << (x*y + x + z).gradedPart(1) <<std::endl;
  std::cout << ((x*y + x + z).gradedPart(1) == (x+z))<<std::endl;
  std::cout << (x+ y*z +z).gradedPart(2) <<std::endl;
  std::cout << ((x+ y*z +z).gradedPart(2) == y*z)<<std::endl;


  std::cout <<  "mapping: " <<std::endl;

  BoolePolynomial map = (x* (y + w) + v);
  poly = x*y +x +y;
  std::cout << "map: "<<map << " poly: "<< poly<<std::endl;
  std::cout << apply_mapping(poly, map) <<std::endl;
  std::cout << (apply_mapping(poly, map) == (v*w+v+w) )<<std::endl;
  std::cout << mapping(poly, x*y, v*w) <<std::endl;
  std::cout << (apply_mapping(poly, map) == mapping(poly, x*y, v*w))<<std::endl;

  double counter(0);
  std::cout << "count idx "<<poly <<" "<<x.firstIndex() <<" occurs "
            <<poly.set().countIndex(x.firstIndex())      <<std::endl;


#endif
 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }


  try {
    std::cout << "Testing division by zero  " <<std::endl;  
    BoolePolynomial(1) / 0;
 }
  catch (PBoRiGenericError<CTypes::division_by_zero>& err) {
    std::cout << "  Caught division_by_zero error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }



    std::cout <<std::endl<<  "Finished."<<std::endl;


    std::cout <<std::endl<<  "sizeof poly"
              <<sizeof(&BoolePolynomial::print)<<std::endl;


    std::cout <<std::endl<<  "printable?"
              <<testit<BoolePolynomial>("a")<<std::endl;

   std::cout <<std::endl<<  "printable?"
              <<testit<BoolePolyRing>("a")<<std::endl;

   //std::cout <<sizeof(&BoolePolynomial::print)<<std::endl;

   //   std::cout <<sizeof(&BoolePolyRing::print)<<std::endl;
   //   print_it(BoolePolynomial());
   // print_it(BoolePolyRing(2)); 

   //   ((any)BoolePolyRing(2)).print(std::cout);

   std::cout << "size BoolePolynomial: " <<some<BoolePolynomial>::thesize
   <<std::endl;

   std::cout << "size BoolePolyRing " <<some<BoolePolyRing>::thesize   <<std::endl;
  return 0;
}
