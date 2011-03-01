// -*- c++ -*-
//*****************************************************************************
/** @file zddnavi.cc 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for getting thwe leading term
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

// load standard iostream capapilities
#include <iostream>
#include <list>
#include <iterator>
#include <sstream>
#include <vector>

// load polybori header file
# include "polybori.h"

#include "pbori_algo.h"

#include "pbori_func.h"

#include <iterators/CTermIter.h>

#include <iterators/CCuddLastIter.h>

#include "BoolePolynomial.h"

#include "CTermStack.h"

USING_NAMESPACE_PBORI


template <class NaviType, class TermType, class ListType>
void
find_paths(NaviType navi, const TermType& currentPath, ListType& theList ) {

  if (navi.isConstant()) {      // Reached end of path
    if (navi.terminalValue())   // Case of a valid path
      theList.push_back(currentPath);
  }
  else {
    find_paths(navi.thenBranch(), currentPath.change_idx(*navi), theList);
    find_paths(navi.elseBranch(), currentPath, theList);
  }
}


template <class IteratorType>
void print1D(IteratorType first, IteratorType last) {

  while (first != last) {
    std::cout << *first <<", ";
    ++first;
  }
}

template<class IteratorType>
void print2D(IteratorType first, IteratorType last) {

  while (first != last) {
    print1D( (*first).begin(), (*first).end());
    std::cout << std::endl;
    ++first;
  }
}


// dummy type for storing monomial data
class variables_list :
  public std::list<int> {
public:
  
  typedef std::list<int> base;
  variables_list() : base() {}
};

std::ostream& 
operator<<(std::ostream& os, const variables_list& varlist) {

   std::copy(varlist.begin(), varlist.end(), 
             std::ostream_iterator<variables_list::value_type>(os, " ") );

  return os;
}



void testiter(const BoolePolynomial& poly) {
    std::cout << "TEMPORARILY OUT OF ORDER"<<std::endl;
#if 0
  typedef BoolePolynomial::const_iterator const_iterator;

    const_iterator start(poly.navigation()), finish;

    std::cout << "*iter "<< *start<<std::endl;

    ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;

    ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;


    --start;
    std::cout << "-- "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
     --start;
    std::cout << "-- "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
   ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
   ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
   ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
     --start;
    std::cout << "-- "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
     --start;
    std::cout << "-- "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
     --start;
    std::cout << "-- "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
   ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
   ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
   ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
   ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
     --start;
    std::cout << "-- "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;
     --start;
    std::cout << "-- "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;

    ++start;
    std::cout << "++ "<<std::endl;
    std::cout << "*iter "<< *start<<std::endl;

#endif
}





int
main(){
  std::cout << "Temporarily deactivated" <<std::endl;   
#if 0

  std::cout << "Testing navigating through decision diagrams" <<std::endl;   

  try {
    BoolePolyRing the_ring(5);

    BooleMonomial x = BooleVariable(0);
    BooleMonomial y = BooleVariable(1);

    BooleMonomial z = BooleVariable(2);

    BooleMonomial v = BooleVariable(3);
    BooleMonomial w = BooleVariable(4);

    BoolePolynomial poly = (z + v*w) * (x*z) + y;

    std::cout << "poly:  "<<std::endl;
    std::cout << poly <<std::endl;


    std::cout << "Navigation: "<<std::endl;

    BoolePolynomial::navigator navi = poly.navigation();

    std::list<CTypes::dd_type > theList;

    dd_transform( navi, BooleEnv::one(),  
                  std::back_inserter(theList),
                  change_idx<CTypes::dd_type >() );

    std::copy(theList.begin(), theList.end(), 
              std::ostream_iterator<BoolePolynomial>(std::cout, "\n") );


    std::cout <<std::endl<< "Printing via dd_transform: "<<std::endl;
    dd_transform( navi, BooleEnv::one(), 
                  std::ostream_iterator<BoolePolynomial>(std::cout, ""),
                  change_idx<CTypes::dd_type, int> () );

    std::cout <<std::endl<< "Storing via dd_transform: "<<std::endl;

    std::list<std::list<int>  > allLists;

    dd_transform( navi, std::list<int>(),
                  std::back_inserter(allLists),
                  push_back<std::list<int> >() );

    print2D(allLists.begin(), allLists.end());

    std::cout <<std::endl<< "pure print dd_transform: "<<std::endl;

    dd_transform( navi, variables_list(),
                  std::ostream_iterator<variables_list>(std::cout, "\n"),
                  push_back<variables_list>() );
 
    std::vector<variables_list > monomvec( poly.length() );


    dd_transform( navi,  variables_list(),
                  monomvec.begin(),
                  push_back<variables_list>() );

    print1D(monomvec.begin(), monomvec.end());

    std::cout <<std::endl<<std::endl<< "Get terms: "<<std::endl;
    BoolePolynomial::termlist_type theTerms = poly.terms();
    
    print1D(theTerms.begin(), theTerms.end());

    std::cout <<std::endl <<std::endl<<"Test Term iteration "<<std::endl;

    typedef BoolePolynomial::const_iterator term_iterator;
       poly += x*z*w;
    std::cout << poly <<std::endl;
    term_iterator iter(poly.navigation());

    std::cout << *iter <<std::endl;

    ++iter;

    std::cout << *iter <<std::endl;

    ++iter;

    std::cout << *iter <<std::endl;

    ++iter;

    std::cout << *iter <<std::endl;
    ++iter;
    std::cout << "  last: "<< std::endl;
    std::cout << *iter <<std::endl;

    std::cout << "  next: "<< std::endl;
    poly += 1;
    std::cout << poly <<std::endl;
    iter = poly.navigation();
    ++iter;    ++iter;    ++iter;   ++iter;
    std::cout << *iter <<std::endl;
    ++iter;

    std::cout << *iter <<std::endl;

    std::cout << "  next: "<< std::endl;

    poly = 1;
    std::cout << poly <<std::endl;
    iter = poly.navigation();

    std::cout << *iter <<std::endl;
    ++iter;

    std::cout << *iter <<std::endl;

    std::cout << "  next: "<< std::endl;
    poly = 0;
    std::cout << poly <<std::endl;
    iter = poly.navigation();

    std::cout << *iter <<std::endl;
    /* ++iter;

    std::cout << *iter <<std::endl;
    */
    std::cout << std::endl<< "Testing last term" <<std::endl;
    poly = x*y + x*w;

    std::cout << "poly "<<poly <<std::endl;

    CCuddLastIter start(poly.diagram().lastBegin() ), finish;

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));

    std::cout <<std::endl<<  std::endl<< "Testing last term" <<std::endl;
    poly = x*y + y*w;

    std::cout << "poly "<<poly <<std::endl;

     start=(poly.diagram().lastBegin() );

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));

    std::cout << std::endl<< std::endl<< "Testing last term" <<std::endl;
    poly = x*y + z*w;

    std::cout << "poly "<<poly <<std::endl;

     start=(poly.diagram().lastBegin() );

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));
  
    std::cout << std::endl<< std::endl<< "Testing last term" <<std::endl;
    poly = x*y*z*v*w + z*v*w + z*w;

    std::cout << "poly "<<poly <<std::endl;

     start=(poly.diagram().lastBegin() );

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));

  
    std::cout << std::endl<< std::endl<< "Testing last term" <<std::endl;
    poly = x*y*z*v*w + y*z*v*w +z*v*w + z*w;

    std::cout << "poly "<<poly <<std::endl;

     start=(poly.diagram().lastBegin() );

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));

    BooleSet bset(poly.diagram());

    std::cout << "last term via BoolSet: "<< 
      bset.lastLexicographicalTerm()<<std::endl;;

    std::cout <<std::endl;

    std::cout <<" Testing decrement..." <<std::endl;
    std::cout << "poly "<<poly <<std::endl;


    testiter(poly+1);

    std::cout <<" Testing additional properties..." <<std::endl;
    std::cout << "poly "<<poly <<std::endl;

    BoolePolynomial::deg_iterator degiter(poly.degBegin());

    std::cout << "term length: " << degiter.deg()  <<std::endl;

    std::cout << "first index: " << degiter.firstIndex()  <<std::endl;

    

    std::cout<<std::endl <<"Testing CTermStack..." <<std::endl;
    std::cout << " poly "<<poly <<std::endl;

    CTermStack<BoolePolynomial::navigator, std::bidirectional_iterator_tag> term_stack(poly.navigation());

    std::cout << " top() "<< *term_stack.top() <<std::endl;
    term_stack.followElse();
    term_stack.decrementNode();
    std::cout << " top() "<< *term_stack.top() <<std::endl;

    std::copy(term_stack.begin(), term_stack.end(),
    std::ostream_iterator<int>(cout, ", "));
    std::cout << std::endl;
    /*
    term_stack.previousTerm();
    term_stack.followElse();
    term_stack.decrementNode();
    std::cout << " top() "<< *term_stack.top() <<std::endl;

    std::copy(term_stack.begin(), term_stack.end(),
    std::ostream_iterator<int>(cout, ", "));
    std::cout << std::endl;
    */
    /*
    term_stack.incrementTerm();
    std::cout << " top() "<< *term_stack.top() <<std::endl;

    std::copy(term_stack.begin(), term_stack.end(),
    std::ostream_iterator<int>(cout, ", "));
    std::cout << std::endl;
    */

    std::cout<<std::endl <<"Testing CDegTermStack..." <<std::endl;

    poly = x*y*z +y +z*w*v+1;

    typedef CDegTermStack<BoolePolynomial::navigator, invalid_tag>
      deg_term_type;

    deg_term_type deg_term_stack(poly.navigation());

    std::cout << " poly "<<poly <<std::endl;

    deg_term_stack.init();

    while (!deg_term_stack.equal(deg_term_type())) {
      std::copy(deg_term_stack.begin(), deg_term_stack.end(),
                std::ostream_iterator<int>(cout, ", "));
      std::cout  <<std::endl;
      
      deg_term_stack.increment();
    }


//     std::copy(term_stack.rbegin(), term_stack.rend(),
//     std::ostream_iterator<int>(cout, ", "));
    std::cout << "Finished."<<std::endl;

  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }
#endif //if0
  return 0;
}
