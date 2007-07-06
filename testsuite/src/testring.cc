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
// Revision 1.25  2007/07/06 18:46:31  dreyer
// ADD: rewritten C++-Interface for Cudd
//
// Revision 1.24  2007/05/03 16:04:47  dreyer
// CHANGE: new-style CTermIter integrated
//
// Revision 1.23  2007/04/30 15:20:32  dreyer
// CHANGE: Switching from CTermIter to iterators based on CTermStack
//
// Revision 1.22  2007/03/28 12:34:57  dreyer
// ADD: added testsuite testcases for blockordering; Fixed errors in block-order
//
// Revision 1.21  2007/03/21 08:55:10  dreyer
// ADD: first version of block_dlex running
//
// Revision 1.20  2007/01/29 17:03:45  dreyer
// ADD: toy sandbox for block orderings
//
// Revision 1.19  2006/12/19 12:36:47  dreyer
// ADD: BoolePolyRing::clear{Ring}Cache()
//
// Revision 1.18  2006/11/29 16:37:35  dreyer
// CHANGE: updated testsuite
//
// Revision 1.17  2006/10/24 08:44:05  dreyer
// CHANGE: Added CVariableNames; variable names handled by OrderedManager
//
// Revision 1.16  2006/10/05 12:51:32  dreyer
// CHANGE: Made lex-based comparisions more generic.
//
// Revision 1.15  2006/10/04 15:46:50  dreyer
// ADD: divisorsOf(exp_type), orderedExpBegin/End;
// CHANGE: Polynomials printing respects ordering
//
// Revision 1.14  2006/10/04 13:09:57  dreyer
// ADD: added compile-time optimied iterators and genericBegin/genericEnd
//
// Revision 1.13  2006/10/02 09:28:38  dreyer
// ADD BoolePolyRing::changeOrdering and infrastructure
//
// Revision 1.12  2006/09/13 15:07:05  dreyer
// ADD: lead(sugar) and infrastructure
//
// Revision 1.11  2006/09/13 09:05:44  dreyer
// ADD: dp_asc/DegRevLexAscOrder
// ADD: BoolePolynomial::endOfNavigation()
// CHANGE: BoolePolynomial: removed biDegBegin(), biDegEnd(), which can be
//   generated more generically using navigation() and endOfNavigation().
//
// Revision 1.10  2006/09/08 16:15:28  dreyer
// ADD: Added ordering-dependent term iteration
//
// Revision 1.9  2006/09/08 14:31:39  dreyer
// ADD: COrderedIter and infrastructure for order-dependent iterator
//
// Revision 1.8  2006/09/07 16:04:32  dreyer
// ADD: CDegLexIter.h
//
// Revision 1.7  2006/09/05 11:10:45  dreyer
// ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
//
// Revision 1.6  2006/09/05 08:48:34  dreyer
// ADD: BoolePolyRing::is(Total)DegreeOrder()
//
// Revision 1.5  2006/07/20 08:55:49  dreyer
// ADD isOrdered() and  isLexicographical()
//
// Revision 1.4  2006/04/11 09:26:57  dreyer
// ADD  BoolePolyRing::printInfo();
//
// Revision 1.3  2006/03/20 14:51:01  dreyer
// CHANGE: Use CDDInterface temple specializations instead of raw dd_type
//
// Revision 1.2  2006/03/13 12:27:24  dreyer
// CHANGE: consistent function names
//
// Revision 1.1.1.1  2006/03/09 14:35:25  dreyer
// + Project started
//

// load standard iostream capapilities
#include <iostream>

// load polybori header file
# include "polybori.h"

#include "CDegLexIter.h"
  //#include "COrderedIter.h"



template <class Type, class BaseType = void>
class enumerate_tags;

template <class Type, class BaseType>
class enumerate_tags:
  public BaseType {
public:

  enum { numval = BaseType::numval + 1};
  using BaseType::num;
  static unsigned num(Type) { return numval; }
// template<class Klasse>
// struct dummy_enum;
//   using base::dummy_enum;
// template<>
// struct dummy_enum<Type> { enum{ val = numval}; };

};


template<class Type>
class enumerate_tags<Type, void> {
  public:
  enum { numval = 0};
  static unsigned num(Type) { return numval; }


// template<class Klasse>
// struct dummy_enum;

// template<>
// struct dummy_enum<Type> { enum{ val = numval}; };
};


struct dummy1_type{};
struct dummy2_type{};
struct dummy3_type{};



USING_NAMESPACE_PBORI


void test_ordered(CTypes::ordercode_type order_marker) {

  BoolePolyRing ring(5, order_marker);

  std::cout << "order code?" <<std::endl;
  std::cout << BoolePolyRing::getOrderCode();
  std::cout<<" (should be: "<< order_marker<<")" <<std::endl;


  std::cout << "is lexicographical?" <<std::endl;
  std::cout << BoolePolyRing::isLexicographical() <<std::endl;

  std::cout << "is ordered?" <<std::endl;
  std::cout << BoolePolyRing::orderedStandardIteration() <<std::endl;

  std::cout << "is symmetric?" <<std::endl;
  std::cout << BoolePolyRing::isSymmetric() <<std::endl;  
  std::cout << "is degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isDegreeOrder() <<std::endl;  
  std::cout << "is total degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isTotalDegreeOrder() <<std::endl;  
  std::cout << "has descending variables?" <<std::endl;
  std::cout << BoolePolyRing::descendingVariables() <<std::endl;  
  std::cout << "has ascending variables?" <<std::endl;
  std::cout << BoolePolyRing::ascendingVariables() <<std::endl;  

  BooleMonomial x = BooleVariable(0);
  BooleMonomial y = BooleVariable(1);
  BooleMonomial z = BooleVariable(2);
  BooleMonomial v = BooleVariable(3);
  BooleMonomial w = BooleVariable(4);

  BoolePolynomial poly =  x*y + z + z*v*w+ y*v*w+ w +1;

  std::cout << "poly " << poly <<std::endl;

  std::cout << "lead() " << poly.lead() <<std::endl;


  std::cout << "boundedLead(deg()) " << poly.boundedLead(poly.deg())
            <<std::endl;
  std::cout << "leadExp() " << poly.leadExp() <<std::endl;
  std::cout << "boundedLeadExp(deg()) " << poly.boundedLeadExp(poly.deg())
            <<std::endl;

  std::cout << "deg() " << poly.deg() <<std::endl;
  std::cout << "lmDeg() " << poly.lmDeg() <<std::endl;
  std::cout << "lmDivisors() " << poly.lmDivisors() <<std::endl;


  BoolePolynomial::ordered_iterator ordStart(poly.orderedBegin());
  BoolePolynomial::ordered_iterator ordFinish(poly.orderedEnd());

  std::cout << "ordered iteration... "<< std::endl;

  while ((ordStart != ordFinish)){
    std::cout << *ordStart << ", ";
    ++ordStart;
  }
  std::cout << std::endl;

  std::cout << "Comparision "<< (y*w) << "< " << (x*z) <<std::endl;
  std::cout << " "<< ((y*w)  < (x*z)) <<std::endl;

  std::cout << "Comparision "<< (y*z) << "< " << (x*w) <<std::endl;
  std::cout << " "<< ((y*z)  < (x*w)) <<std::endl;

  std::cout << "Comparision "<< (y) << "< " << (x*w) <<std::endl;
  std::cout << " "<< ((y)  < (x*w)) <<std::endl;

}

void test_ordered_changed(CTypes::ordercode_type order_marker, 
                          CTypes::ordercode_type changed_order) {

  BoolePolyRing ring(5, order_marker);

  std::cout << "is lexicographical?" <<std::endl;
  std::cout << BoolePolyRing::isLexicographical() <<std::endl;

  std::cout << "is ordered?" <<std::endl;
  std::cout << BoolePolyRing::orderedStandardIteration() <<std::endl;

  std::cout << "is symmetric?" <<std::endl;
  std::cout << BoolePolyRing::isSymmetric() <<std::endl;  
  std::cout << "is degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isDegreeOrder() <<std::endl;  
  std::cout << "is total degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isTotalDegreeOrder() <<std::endl;  
  std::cout << "has descending variables?" <<std::endl;
  std::cout << BoolePolyRing::descendingVariables() <<std::endl;  
  std::cout << "has ascending variables?" <<std::endl;
  std::cout << BoolePolyRing::ascendingVariables() <<std::endl;  

  BooleMonomial x = BooleVariable(0);
  BooleMonomial y = BooleVariable(1);
  BooleMonomial z = BooleVariable(2);
  BooleMonomial v = BooleVariable(3);
  BooleMonomial w = BooleVariable(4);

  BoolePolynomial poly =  x*y + z + z*v*w+ y*v*w+ w +1;

  std::cout << "poly " << poly <<std::endl;

  std::cout << "lead() " << poly.lead() <<std::endl;


  std::cout << "boundedLead(deg()) " << poly.boundedLead(poly.deg())
            <<std::endl;
  std::cout << "leadExp() " << poly.leadExp() <<std::endl;
  std::cout << "boundedLeadExp(deg()) " << poly.boundedLeadExp(poly.deg())
            <<std::endl;

  std::cout << "deg() " << poly.deg() <<std::endl;
  std::cout << "lmDeg() " << poly.lmDeg() <<std::endl;
  std::cout << "lmDivisors() " << poly.lmDivisors();


  BoolePolynomial::ordered_iterator ordStart(poly.orderedBegin());
  BoolePolynomial::ordered_iterator ordFinish(poly.orderedEnd());

  std::cout << "ordered iteration... "<< std::endl;

  while ((ordStart != ordFinish)){
    std::cout << *ordStart << ", ";
    ++ordStart;
  }
  std::cout << std::endl;

  std::cout <<std::endl<< "CHANGE ordering!"<<std::endl;

  BoolePolyRing::changeOrdering(changed_order);

 std::cout << "boundedLead(deg()) " << poly.boundedLead(poly.deg())
            <<std::endl;
  std::cout << "leadExp() " << poly.leadExp() <<std::endl;
  std::cout << "boundedLeadExp(deg()) " << poly.boundedLeadExp(poly.deg())
            <<std::endl;

  std::cout << "deg() " << poly.deg() <<std::endl;
  std::cout << "lmDeg() " << poly.lmDeg() <<std::endl;
  std::cout << "lmDivisors() " << poly.lmDivisors();


  BoolePolynomial::ordered_iterator ordStart2(poly.orderedBegin());
  BoolePolynomial::ordered_iterator ordFinish2(poly.orderedEnd());

  std::cout << "ordered iteration... "<< std::endl;

  while ((ordStart2 != ordFinish2)){
    std::cout << *ordStart2 << ", ";
    ++ordStart2;
  }
  std::cout << std::endl;

  BoolePolynomial::ordered_exp_iterator expStart(poly.orderedExpBegin());
  BoolePolynomial::ordered_exp_iterator expFinish(poly.orderedExpEnd());

  std::cout << "ordered exponent iteration... "<< std::endl;

  while ((expStart != expFinish)){
    std::cout << *expStart << ", ";
    ++expStart;
  }
  std::cout << std::endl;

}

template<class Iterator>
void print_iter(Iterator start, Iterator end) {

  int i = 0;
  while ((start != end)&& (i < 10)){
    std::cout << *start << ", ";std::cout.flush();
    ++start;
    ++i;
  }
  std::cout <<  std::endl;
}

/* The DdNode structure is the only one exported out of the package */
struct DdNode2 {
    int index;
  //    short ref;		/* reference count */
    DdNode2 *next;		/* next pointer for unique table */
    union {
      //	CUDD_VALUE_TYPE value;	/* for constant nodes */
	DdChildren kids;	/* for internal nodes */
    } type;
};
int
main(){
  std::cout << "sizeof(DdNode2)"<<sizeof(DdNode2) << std::endl;
  std::cout << "sizeof(DdNode)"<<sizeof(DdNode) << std::endl;
  std::cout << "sizeof(void*)"<<sizeof(void*) << std::endl;

  try{
    BoolePolyRing::ring();
  }
  catch(PBoRiError err){
    std::cout << "Exception caught sucessfully: "<<err.text() <<std::endl;
  }

  BoolePolyRing ring(4);

  int verbosity = 2;
  std::cout << "Variable 0:"<<std::endl;
  ((CTypes::dd_base)ring.variable(0)).print(3, verbosity);

  std::cout << "Variable 1:"<<std::endl;
  ((CTypes::dd_base)ring.variable(1)).print(3, verbosity);

  std::cout << "Variable 2:"<<std::endl;
  ((CTypes::dd_base)ring.variable(2)).print(3, verbosity);

  std::cout << "Variable 3:"<<std::endl;
  ((CTypes::dd_base)ring.variable(3)).print(3, verbosity);

  BoolePolyRing::printInfo();

  std::cout << "is lexicographical?" <<std::endl;
  std::cout << BoolePolyRing::isLexicographical() <<std::endl;

  std::cout << "is ordered?" <<std::endl;
  std::cout << BoolePolyRing::orderedStandardIteration() <<std::endl;

  std::cout << "is symmetric?" <<std::endl;
  std::cout << BoolePolyRing::isSymmetric() <<std::endl;  

  std::cout << "is degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isDegreeOrder() <<std::endl;  
  std::cout << "is total degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isTotalDegreeOrder() <<std::endl;  
  std::cout << "has descending variables?" <<std::endl;
  std::cout << BoolePolyRing::descendingVariables() <<std::endl;  
  std::cout << "has ascending variables?" <<std::endl;
  std::cout << BoolePolyRing::ascendingVariables() <<std::endl;  

  try{
    BoolePolyRing::ring();
  }
  catch(PBoRiError err){
    std::cout << "Warning! Accessing global ring failed. ";
    std::cout << "Unexpected exception occured: ";
    std::cout <<err.text() <<std::endl;
  }

  BoolePolyRing ring2(5, CTypes::dlex);
  std::cout << "Testing DegLex ring... "<<std::endl;;

  std::cout << "is lexicographical?" <<std::endl;
  std::cout << BoolePolyRing::isLexicographical() <<std::endl;

  std::cout << "is ordered?" <<std::endl;
  std::cout << BoolePolyRing::orderedStandardIteration() <<std::endl;

  std::cout << "is symmetric?" <<std::endl;
  std::cout << BoolePolyRing::isSymmetric() <<std::endl;  
  std::cout << "is degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isDegreeOrder() <<std::endl;  
  std::cout << "is total degree ordering?" <<std::endl;
  std::cout << BoolePolyRing::isTotalDegreeOrder() <<std::endl;  
  std::cout << "has descending variables?" <<std::endl;
  std::cout << BoolePolyRing::descendingVariables() <<std::endl;  
  std::cout << "has ascending variables?" <<std::endl;
  std::cout << BoolePolyRing::ascendingVariables() <<std::endl;  

  BooleMonomial x = BooleVariable(0);
  BooleMonomial y = BooleVariable(1);
  BooleMonomial z = BooleVariable(2);
  BooleMonomial v = BooleVariable(3);
  BooleMonomial w = BooleVariable(4);

  BoolePolynomial poly =  x*y + z + z*v*w+ w +1;

  std::cout << "poly " << poly <<std::endl;

  std::cout << "lead() " << poly.lead() <<std::endl;
  std::cout << "leadExp() " << poly.leadExp() <<std::endl;
  std::cout << "lmDeg() " << poly.lmDeg() <<std::endl;
  std::cout << "lmDivisors() " << poly.lmDivisors() <<std::endl;

  BoolePolynomial::ordered_iterator iter(poly.orderedBegin()), 
    finish(poly.orderedEnd());


  while (iter != finish){
    std::cout << *iter << ", ";std::cout.flush();
    ++iter;
  }
  std::cout <<  std::endl;

  BoolePolynomial::deg_iterator start(poly.degBegin()), end(poly.degEnd());

  while ((start != end)){
    std::cout << *start << ", ";
    ++start;

  }

  std::cout <<  std::endl;


  std::cout << "max "<<*std::max_element(poly.degBegin(), poly.degEnd()) <<std::endl;
  std::cout << "min "<<*std::max_element(poly.degBegin(), poly.degEnd(), std::greater<unsigned>())
  <<std::endl;

//   COrderedIter<BooleMonomial> oStart(poly), oFinish;

//   std::cout << "ordered Iter"<< std::endl;

//   while ((oStart != oFinish)){
//     std::cout << *oStart << ", ";
//     ++oStart;
//   }



  BoolePolynomial::ordered_iterator ordStart(poly.orderedBegin());
  BoolePolynomial::ordered_iterator ordFinish(poly.orderedEnd());

  std::cout << "ordered_ Iter"<< std::endl;

  while ((ordStart != ordFinish)){
    std::cout << *ordStart << ", ";
    ++ordStart;
  }
  ordStart = (poly.orderedBegin());
  ordFinish = (poly.orderedEnd());

  std::cout << "ordered_ Iter"<< std::endl;

  while ((ordStart != ordFinish)){
    std::cout << *ordStart << ", ";
    ++ordStart;
  }



  BoolePolyRing ring3(5);
//   COrderedIter<BooleMonomial>  oStart2(poly);

//   std::cout << "lex-ordered Iter"<< std::endl;

//   while ((oStart2 != oFinish)){
//     std::cout << *oStart2 << ", ";
//     ++oStart2;
//   }

  std::cout<< std::endl << "Testing dlex"<< std::endl;
  test_ordered(CTypes::dlex); 
  
  std::cout<< std::endl << "Testing dp_asc"<< std::endl;
  test_ordered(CTypes::dp_asc); 


  std::cout<< std::endl << "Testing changing from lex to dlex"<< std::endl;
  test_ordered_changed(CTypes::lp, CTypes::dlex); 

  std::cout<< std::endl << "Testing changing from dp to lex"<< std::endl;
  test_ordered_changed(CTypes::dp_asc, CTypes::lp); 

  std::cout<< std::endl << "Testing hard-coded iteration for " <<poly<<
  std::endl; 
  std::cout<< "lex"<<std::endl;


  print_iter(poly.genericBegin(lex_tag()), poly.genericEnd(lex_tag()) );
  
  std::cout<< "dlex"<<std::endl;
  print_iter(poly.genericBegin(dlex_tag()), poly.genericEnd(dlex_tag()) );

  std::cout<< "dp_asc"<<std::endl;
  print_iter(poly.genericBegin(dp_asc_tag()), poly.genericEnd(dp_asc_tag()) );

  std::cout<< std::endl << "bug?"<< std::endl;

   poly =  x*y*z*v + x*y*v + x*y + x*z +x;

     std::cout<<  "poly"<<poly<< std::endl;


     std::cout<<  "usedVariables"<<poly.usedVariables()<<   std::endl;
    std::cout<<  "usedVariablesExp"<<poly.usedVariablesExp()<<   std::endl;
      std::cout<<  "usedVariables.Exp"<<poly.usedVariables().exp()<<
      std::endl;


      std::cout<<  "Test Variablenames"<< std::endl;

      std::cout <<"x(0)? "<<
        BoolePolyRing::getRingVariableName(0) <<std::endl;
      std::cout.flush();
      BoolePolyRing::setRingVariableName(0, "X");
      std::cout <<"X? "<<
        BoolePolyRing::activeManager().getVariableName(0) <<std::endl;

      std::cout <<"Testing cache flushing..." <<std::endl;
      std::cout.flush();
      BoolePolyRing::clearRingCache();

      std::cout << "Finished." <<std::endl;


  try{
    BoolePolyRing::ring();
  }
  catch(PBoRiError err){
    std::cout << "Warning! Accessing global ring failed. ";
    std::cout << "Unexpected exception occured: ";
    std::cout <<err.text() <<std::endl;
  }

  typedef  enumerate_tags<dummy3_type, enumerate_tags<dummy2_type,
  enumerate_tags<dummy1_type> > > enumit;

  std::cout<< enumit::numval <<std::endl;
  std::cout<< enumit::num(dummy1_type()) <<std::endl;
  std::cout<< enumit::num(dummy2_type()) <<std::endl;
  std::cout<< enumit::num(dummy3_type()) <<std::endl;

  //  std::cout<< integral_constant<int,
  //  count_tags<dummy3_type>::value>()()<<std::endl; 
  std::cout<<(void*) cudd_tag_number<0, 0>::value  <<std::endl; 
  /*
  std::cout<<(void*) cudd_tag_number<1, 0>::value  <<std::endl; 
  std::cout<<(void*)  cudd_tag_number<2, 0>::value   <<std::endl; 
  std::cout<<(void*)  cudd_tag_number<3, 0>::value   <<std::endl; 
  std::cout<<(void*)  cudd_tag_number<4, 0>::value   <<std::endl; 
  std::cout<<(void*)  cudd_tag_number<5, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<6, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<7, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<8, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<9, 0>::value   <<std::endl;
  std::cout<<(void*) cudd_tag_number<10, 0>::value  <<std::endl; 
  std::cout<<(void*) cudd_tag_number<11, 0>::value  <<std::endl; 
  std::cout<<(void*)  cudd_tag_number<12, 0>::value   <<std::endl; 
  std::cout<<(void*)  cudd_tag_number<13, 0>::value   <<std::endl; 
  std::cout<<(void*)  cudd_tag_number<14, 0>::value   <<std::endl; 
  std::cout<<(void*)  cudd_tag_number<15, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<16, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<17, 0>::value   <<std::endl;
   std::cout<<18   <<std::endl;
 std::cout<<(void*)  cudd_tag_number<18, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<19, 0>::value   <<std::endl;
  std::cout<<(void*) cudd_tag_number<20, 0>::value  <<std::endl; 
  std::cout<<(void*) cudd_tag_number<21, 0>::value  <<std::endl; 
  std::cout<<(void*) cudd_tag_number<22, 0>::value  <<std::endl; 
  std::cout<<(void*) cudd_tag_number<23, 0>::value  <<std::endl; 
  std::cout<<(void*) cudd_tag_number<24, 0>::value  <<std::endl;
  std::cout<<(void*) cudd_tag_number<25, 0>::value  <<std::endl; 
  std::cout<<(void*)  cudd_tag_number<26, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<27, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<28, 0>::value   <<std::endl;
  std::cout<<(void*)  cudd_tag_number<29, 0>::value   <<std::endl;
  std::cout<<(void*) cudd_tag_number<30, 0>::value  <<std::endl; 
  std::cout<<(void*) cudd_tag_number<31, 0>::value  <<std::endl; 
  std::cout<<"huhu" <<std::endl; 
  std::cout<<(void*) cudd_tag_number<0, 17>::value  <<std::endl; 
  std::cout<<(void*) cudd_tag_number<0>::value  <<std::endl; 
  std::cout<<(void*) cudd_tag_number<1>::value  <<std::endl; 
*/
 //  std::cout<< enumit::dummy_enum<dummy1_type>::numval <<std::endl;
  std::cout<< "sizeof(int)  " << sizeof(int) <<std::endl;

  std::cout<< "100000" << int(100000) <<std::endl;
  std::cout<< "" << int() <<std::endl;
//   BoolePolyRing ring5(100002);
//   std::cout<< "" << BooleVariable(100000) <<std::endl;

  return 0;
}

