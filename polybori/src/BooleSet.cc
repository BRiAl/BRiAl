// -*- c++ -*-
//*****************************************************************************
/** @file BooleSet.cc
 *
 * @author Alexander Dreyer
 * @date 2006-04-19
 *
 * This file implements the class BooleSet, where carries the definition
 * of sets of Boolean variables.
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
 * Revision 1.16  2006/08/29 12:09:27  dreyer
 * using CDDOperations for generic dd functions
 *
 * Revision 1.15  2006/08/29 08:02:08  dreyer
 * ADD: BooleSet::expBegin() and expEnd()
 *
 * Revision 1.14  2006/08/23 14:24:54  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * Revision 1.13  2006/08/22 16:06:23  dreyer
 * + Added highlevel division
 *
 * Revision 1.12  2006/08/18 19:47:15  dreyer
 * change finished low-level variant of minimal_elements also for lnegth()==2
 *
 * Revision 1.11  2006/08/17 15:35:30  dreyer
 * ADD: extended and activated low-level version of dd_minimal_elements
 *
 * Revision 1.10  2006/08/15 14:17:29  dreyer
 * ADD minimalElements(), hasTermOfVariables()
 *
 * Revision 1.9  2006/08/09 12:52:32  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * Revision 1.8  2006/08/03 15:20:20  dreyer
 * ADD: BooleSet::divisorsOf and BooleSet::hasCommonVariables
 *
 * Revision 1.7  2006/06/07 08:37:50  dreyer
 * ADD CCuddLastIter and BooleSet::lastLexicographicalTerm()
 *
 * Revision 1.6  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.5  2006/04/21 09:31:44  dreyer
 * CHANGE reducing redundancy
 *
 * Revision 1.4  2006/04/21 07:11:50  dreyer
 * ADD operator=
 *
 * Revision 1.3  2006/04/21 06:35:45  dreyer
 * CHANGE Default constructor starts with empty set
 *
 * Revision 1.2  2006/04/20 17:31:05  dreyer
 * FIX removed casting operator, which caused ambigeous overloads
 *
 * Revision 1.1  2006/04/20 16:59:47  dreyer
 * routine
 *
 * @endverbatim
**/
//*****************************************************************************


// load header files
# include "BooleSet.h"
# include "BooleMonomial.h"
# include "BooleExponent.h"
# include "BoolePolyRing.h"
# include "CIdxPath.h"
# include "PBoRiOutIter.h"
# include <iterator>
# include "pbori_algo.h"


#include "pbori_algo_int.h"


// get internal routines
# include "pbori_routines.h"
# include "CDDOperations.h"

BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------

// Default constructor
BooleSet::BooleSet():
  base( BoolePolyRing::ringZero() )  {

  PBORI_TRACE_FUNC( "BooleSet()" );

}

// Copy constructor
BooleSet::BooleSet(const self& rhs) :
  base(rhs) {
  
  PBORI_TRACE_FUNC( "BooleSet(const self&)" );
}

// Copy constructor
BooleSet::BooleSet(const base& rhs) :
  base(rhs) {
  
  PBORI_TRACE_FUNC( "BooleSet(const base&)" );
}


// Destructor
BooleSet::~BooleSet() {

  PBORI_TRACE_FUNC( "~BooleSet()" );
}

// Assignment
BooleSet&
BooleSet::operator=(const self& rhs) {

  PBORI_TRACE_FUNC( "BooleSet::operator=(const self&) const" );

  base::operator=(rhs);
  return *this;
}

// Add given monomial to sets and assign
BooleSet&
BooleSet::addAssign(const term_type& rhs) {

  PBORI_TRACE_FUNC( "BooleSet::addAssign(const term_type&) const" );

  uniteAssign(rhs.diagram());
  return *this;
}

// Add given monomial to sets
BooleSet
BooleSet::add(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::addAssign(const term_type&) const" );

  return self(*this).addAssign(rhs);
}

// Check whether rhs is includes in *this
BooleSet::bool_type
BooleSet::owns(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::owns(const term_type&) const" );

  return !(intersect(rhs.diagram()).emptiness());
}


// Start of iteration over monomials
BooleSet::const_iterator 
BooleSet::begin() const {

  PBORI_TRACE_FUNC( "BooleSet::begin() const" );
  return navigation();
}

// Finish of iteration over monomials
BooleSet::const_iterator 
BooleSet::end() const {

  PBORI_TRACE_FUNC( "BooleSet::end() const" );
  return const_iterator();
}

// Start of iteration over exponent vectors
BooleSet::exp_iterator 
BooleSet::expBegin() const {

  PBORI_TRACE_FUNC( "BooleSet::exp_begin() const" );
  return base::navigation();
}

// Finish of iteration over monomials
BooleSet::exp_iterator 
BooleSet::expEnd() const {

  PBORI_TRACE_FUNC( "BooleSet::exp_end() const" );
  return exp_iterator();
}


// Get last term (wrt. lexicographical order)
BooleSet::term_type
BooleSet::lastLexicographicalTerm() const {

  PBORI_TRACE_FUNC( "BooleSet::lastTerm() const" );

  return dd_last_lexicographical_term(*this, type_tag<term_type>());

  term_type result(true);

  if (emptiness())
    result = false;
  else {

    size_type nlen = std::distance(base::lastBegin(), base::lastEnd());

    // store indices in list
    CIdxPath<idx_type> indices(nlen);

    // iterator, which uses changeAssign to insert variable
    // wrt. given indices to a monomial
    PBoRiOutIter<term_type, idx_type, change_assign<term_type> >  
      outiter(result) ;
    
    // insert backward (for efficiency reasons)
    reversed_inter_copy(base::lastBegin(), base::lastEnd(), indices, outiter);
  }

  return result;
}

// compute intersection with divisors of rhs
BooleSet
BooleSet::divisorsOf(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::divisorsOf(const term_type&) const" );

#ifdef PBORI_DIVISORSOF_HIGHLEVEL

    return intersect(rhs.divisors());

#else 

    return firstDivisorsOf(rhs.diagram());

#endif 

}

// check whether the intersection with divisors of rhs is non-empty
BooleSet::bool_type
BooleSet::hasTermOfVariables(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::EmptyDivisorsOf(const term_type&) const" );

#ifdef PBORI_HASTERMOFVARIABLES_HIGHLEVEL

  bool_type result =!divisorsOf(rhs).emptiness();

#else

  bool_type result = dd_owns_term_of_indices(navigation(),
                                             rhs.begin(), rhs.end());

//   if (result != !divisorsOf(rhs).emptiness())
//     std::cerr << "Error in hasTermOfVariables!!!" <<std::endl;

#endif
  
  return result;
}





BooleSet
BooleSet::minimalElements() const { 

  //  return base::minimalElements(); 
  // base 

  navigator  resultMultiples;
  std::vector<idx_type> indices(nSupport());

  usedIndices(indices);
  dd_operations<navigator> apply(manager().getManager());
  //  std::cerr<< "b4"<<std::endl;
  navigator result= dd_minimal_elements(navigation(),
  resultMultiples, indices.rbegin(), indices.rend(),  
                                        apply);
  // std::cerr<< "aft"<<std::endl;
base res = ZDD( &manager(), result );


  Cudd_Deref(result);
  Cudd_RecursiveDerefZdd(manager().getManager(), resultMultiples);
  return res;

};


// Division by given term
BooleSet
BooleSet::divide(const term_type& rhs) const {
  return self(base::divideFirst(rhs.diagram()));
};

// Division with assignment by given term
BooleSet& 
BooleSet::divideAssign(const term_type& rhs)  {
  base::divideFirstAssign(rhs.diagram());
  return *this;
};

// Set of variables of the polynomial
BooleSet::term_type
BooleSet::usedVariables() const {

  PBORI_TRACE_FUNC( "BooleSet::usedVariables() const" );


  //  return dd_used_variables(*this, type_tag<term_type>());
  return CDDOperations<self, term_type>().usedVariables(*this);
}

END_NAMESPACE_PBORI
