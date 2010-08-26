 // -*- c++ -*-
//*****************************************************************************
/** @file blocknavi.cc 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests
 *
 * @par Copyright:
 *   (c) 2007 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.17  2007/12/13 15:53:50  dreyer
 * CHANGE: Ordering in BoolePolyRing again; BooleEnv manages active ring
 *
 * Revision 1.16  2007/11/06 15:03:42  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.15  2007/07/31 07:43:50  dreyer
 * ADD: getBaseOrderCode(), lieInSameBlock(...), isSingleton(), isPair()...
 *
 * Revision 1.14  2007/07/06 18:46:31  dreyer
 * ADD: rewritten C++-Interface for Cudd
 *
 * Revision 1.13  2007/05/03 16:04:46  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * Revision 1.12  2007/04/30 15:20:32  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.11  2007/04/13 13:55:54  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.10  2007/03/28 12:34:57  dreyer
 * ADD: added testsuite testcases for blockordering; Fixed errors in block-order
 *
 * Revision 1.9  2007/03/21 08:55:10  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.8  2007/02/20 09:41:06  dreyer
 * CHANGE: now running prototype for dlex-block iteration
 *
 * Revision 1.7  2007/02/19 17:21:51  dreyer
 * CHANGE: routine check-in
 *
 * Revision 1.6  2007/02/16 16:14:22  dreyer
 * CHANGE: routine check-in
 *
 * Revision 1.5  2007/02/15 17:03:12  dreyer
 * + Routine check-in
 *
 * Revision 1.4  2007/02/14 13:56:10  dreyer
 * CHANGE: finished prototype for block iterator
 *
 * Revision 1.3  2007/02/09 17:07:00  dreyer
 * CHANGE: experimenting once more
 *
 * Revision 1.2  2007/02/07 11:01:51  dreyer
 * CHANGE: routine
 *
 * Revision 1.1  2007/01/29 17:03:44  dreyer
 * ADD: toy sandbox for block orderings
 * @endverbatim
**/
//*****************************************************************************


// load standard iostream capapilities
#include <iostream>
#include <algorithm>

// load polybori header file
# include "polybori.h"
#include "pbori_routines.h"

#include "CDegreeCache.h"
#include "CCacheManagement.h"

USING_NAMESPACE_PBORI



typedef CCacheTypes::block_dlex_lead block_dlex_lead;

template <class Iterator>
void dummy_print2(Iterator start, Iterator finish) {
  std::cout << "[";
  while (start != finish) {
    std::cout << **start <<std::endl;
    ++start;
  }
  std::cout << "]"<<std::endl;;
}


/*
template <class DelayedIterator>
class CBlockIterator:
  public DelayedIterator {

public:

  typedef DelayedIterator base;
  typedef CBlockIterator<base> self;
  typedef typename base::stack_type stack_type;
  typedef typename base::navigator navigator;
  typedef unsigned size_type;
  typedef unsigned idx_type;

  CBlockIterator(navigator navi, unsigned* indices, 
                 const CBlockDegreeCache<>& deg_cache):
    base(),  m_indices(indices), m_deg_cache(deg_cache), 
    m_current_block(indices) {
    findTerminal(navi);   
  }

  size_type currentBlockDegree(const navigator& navi) const {
    return dd_cached_block_degree(m_deg_cache, navi, *m_current_block);
  }
  
  void incrementBlock(navigator& navi) {
    incrementBlock(navi, currentBlockDegree(navi));
  }

  void incrementBlock(navigator& navi, unsigned deg) {

    while(deg > 0) {
      --deg;
      if ( currentBlockDegree(navi.thenBranch()) == deg){
        assert(!navi.isConstant());
        base::m_stack.push(navi);
        navi.incrementThen(); 
      }
      else {
        ++deg;
        navi.incrementElse();
        assert(!navi.isConstant());
        base::m_stack.push(navi);
      }
    }
  }


  idx_type blockMin() const {
    return ( m_current_block == m_indices? 0: *(m_current_block - 1) );
  }

  idx_type blockMax() const {
    return *m_current_block;
  }

  self & operator++() {

    // the zero term
    if (base::empty())
      return *this;

    navigator current = base::m_stack.top(); 
  
    // the term one
    if (!current.isValid()) {
      base::clear();
      return *this;
    }

    while (*current < blockMin())
      --m_current_block;
    ++m_current_block;


    do {
      --m_current_block;
           
      deg_next_term<stack_type, navigator, unsigned>
        nextop(base::m_stack,  blockMin(), blockMax());
  
      current = nextop();

    } while (!base::empty() && current.isEmpty());
 
    findTerminal(current);

    if (base::empty() && current.terminalValue()) {
      base::clear();
      base::m_stack.push(navigator());
    }
 
    return *this;
  }

  bool atBlockEnd(navigator navi) const {
    return navi.isConstant() || (*navi >= blockMax());
  }

  // template <class IdxIterator>
  void findTerminal(navigator navi) {
    if (!navi.isConstant() ) 
      incrementBlock(navi);

    while (!navi.isConstant()  ) {
      assert (blockMax() != CUDD_MAXINDEX);
      ++m_current_block;

      incrementBlock(navi);
    }
  }

  void print() const {
      std::cout << ":";
      std::cout.flush();
    stack_type thestack( base::m_stack);
    while (!thestack.empty()){
      if (thestack.top().isValid())
        std::cout << *(thestack.top()) << ", ";
      else 
        std::cout << "one";
      std::cout.flush();
      thestack.pop();
    }
  }


  // std::stack<navigator> base::m_stack;

  unsigned* m_indices;
  unsigned* m_current_block;

  const CBlockDegreeCache<>& m_deg_cache;
};
*/



int
main(){

  std::cout << "Experimenting with block orderings..." << std::endl;   

  try {
    BoolePolyRing the_ring(10, CTypes::block_dlex);

    BooleMonomial x0 = BooleVariable(0);
    BooleMonomial x1 = BooleVariable(1);
    BooleMonomial x2 = BooleVariable(2);
    BooleMonomial x3 = BooleVariable(3);
    BooleMonomial x4 = BooleVariable(4);
    BooleMonomial x5 = BooleVariable(5);
    BooleMonomial x6 = BooleVariable(6);
    BooleMonomial x7 = BooleVariable(7);
    BooleMonomial x8 = BooleVariable(8);
    BooleMonomial x9 = BooleVariable(9);


    BoolePolynomial poly = 
      x1*x2*x6*x9  + x1*x2*x7*x9+ x1*x2*x7+ x1*x2*x8*x9 
      +   x1*x3*x6*x9  + x1*x3*x7*x9+ x1*x3*x7+ x1*x3*x8*x9 + x1*x5+x1 +x2+1; 

      // x1+ x2*x3+ x1*x2 +  x1*x2*x6  + x1*x2*x7+ x3*x4*x5;
    std::cout << "Polynom: "<< poly <<std::endl;

    CBlockDegreeCache<BooleSet> blockDegCache(poly.ring());

    std::cout <<"Block degree wrt. 2: " << 
      dd_cached_block_degree(blockDegCache, poly.navigation(), 2) << std::endl;

    CacheManager<block_dlex_lead> cache_mgr(poly.ring());

    BoolePolynomial::navigator navi(poly.navigation());

    unsigned next_block[3] = {4, 6, CUDD_MAXINDEX};

    BooleEnv::appendBlock(4);
    BooleEnv::appendBlock(6);


    std::cout << dd_block_degree_lead(cache_mgr, blockDegCache, navi, 
                                      next_block,
                                      BooleSet(), 
                                      valid_tag());


    BooleMonomial monom1 = x1*x2*x6, monom2 = x1*x2*x7;

    std::cout << monom1<< " < " << monom2 << "? "<<
      block_dlex_compare(monom1.begin(), monom1.end(),
                         monom2.begin(), monom2.end(),
                         next_block, next_block+3, std::less<unsigned>())
              << std::endl;

    std::cout << "block iter..."<<std::endl;

    typedef unsigned size_type;
    // Incrementation functional type
    typedef 
      binary_composition< std::plus<size_type>, 
      project_ith<1>, integral_constant<size_type, 1> > 
      increment_type;
      
    // Decrementation functional type
    typedef 
      binary_composition< std::minus<size_type>, 
      project_ith<1>, integral_constant<size_type, 1> > 
      decrement_type;
    typedef BoolePolynomial::navigator navigator;
    // Iterator type for iterating all monomials (dereferencing to degree)



    BoolePolynomial::ordered_iterator obegin(poly.orderedBegin()),
    oend(poly.orderedEnd());

    std::cout << "ordered iteration " <<std::endl;
    while (obegin != oend) {
      std::cout << *obegin <<", "<<std::endl; std::cout.flush();
      ++obegin;
    }
    std::cout <<std::endl;

    bounded_restricted_term<navigator> bounded_iter(poly.navigation(), 2, 4);
    bounded_restricted_term<navigator> bounded_end;
    /*
    std::cout << "bounded max"<<std::endl;
    bounded_restricted_term<navigator> bounded_max =
      std::max_element(bounded_iter, bounded_end);

    std::cout << "* " <<  *bounded_max<<std::endl;
    std::cout << "next " <<  *bounded_max.next()<<" " <<
      bounded_max.next().isTerminated()<<std::endl;
    bounded_max.print();   std::cout <<std::endl;std::cout.flush();
    std::cout <<std::endl<< "bounded iter"<<std::endl;

    while ( bounded_iter !=  bounded_end ){
      std::cout << "* " <<  *bounded_iter<<std::endl;
      std::cout << "next " <<  *bounded_iter.next()<< 
        "  "<<bounded_iter.next().isTerminated()<<std::endl; 
      bounded_iter.print();   std::cout <<std::endl;std::cout.flush();
      ++bounded_iter;
    }

      std::cout << "next " <<  *bounded_iter.next()<< "  "<<bounded_iter.next().isTerminated()<<std::endl; 
      bounded_iter.print();

    */
    /*  std::cout << "Experimenting with negations..."<<std::endl;
    navi = poly.navigation();
    std::cout << navi.operator->()<<std::endl;
    std::cout <<  Cudd_Not(navi.operator->())<<std::endl;
    std::cout <<  Cudd_Regular(navi.operator->())<<std::endl;
    std::cout <<  Cudd_Regular( Cudd_Not(navi.operator->()))<<std::endl;
    std::cout <<  Cudd_Complement(navi.operator->())<<std::endl;
    */
    std::cout << "Other polynomials (block-ordered)"<<std::endl;
    //    poly =  x0*BoolePolynomial(x1 + x3) *BoolePolynomial(x2 + 1);
     poly =  x0*BoolePolynomial(x4 + 1)*BoolePolynomial(x5 + 1);
   // std::cout << poly <<std::endl;

    BoolePolynomial::const_iterator lexbegin = (poly.begin()),
    lexend = (poly.end());
    std::cout << "lex iteration " <<std::endl;

    while (lexbegin != lexend) {
      std::cout << *lexbegin <<", "<<std::endl; std::cout.flush();
      ++lexbegin;
    }
    
    obegin = (poly.orderedBegin()),
    oend = (poly.orderedEnd());

    std::cout << "ordered iteration " <<std::endl;
    while (obegin != oend) {
      std::cout << "> "<< *obegin <<" < , "<<std::endl; std::cout.flush();
      ++obegin;
    }

    std::cout << (BoolePolynomial(x4 + 1)*BoolePolynomial(x5 + 1)) <<std::endl;
    poly = (BoolePolynomial(x0 + 1)*BoolePolynomial(x2 + 1))*(BoolePolynomial(x4
    + 1)*BoolePolynomial(x5 + 1)) ;

    lexbegin = (poly.begin()),
    lexend = (poly.end());
    std::cout << "lex iteration " <<std::endl;

    while (lexbegin != lexend) {
      std::cout << *lexbegin <<", "<<std::endl; std::cout.flush();
      ++lexbegin;
    }
    
    obegin = (poly.orderedBegin()),
    oend = (poly.orderedEnd());

    std::cout << "ordered iteration " <<std::endl;
    int i = 0;
    while (obegin != oend) {
      std::cout << "> "<< *obegin <<" < , "<<std::endl; std::cout.flush();
      ++obegin; ++i;
    }

    std::cout <<std::endl<< "next poly" <<std::endl;

      poly =  BoolePolynomial(x0 + x1*x2)* BoolePolynomial(x4 + x5)
      * BoolePolynomial(x6 + x7*x8);

    lexbegin = (poly.begin()),
    lexend = (poly.end());
    std::cout <<std::endl<< "lex iteration " <<std::endl;

    while (lexbegin != lexend) {
      std::cout << *lexbegin <<", "<<std::endl; std::cout.flush();
      ++lexbegin;
    }
    
    obegin = (poly.orderedBegin()),
    oend = (poly.orderedEnd());

    std::cout << "ordered iteration " <<std::endl;

    while (obegin != oend) {
      std::cout << "> "<< *obegin <<" < , "<<std::endl; std::cout.flush();
      ++obegin; ++i;
    }
      poly =  BoolePolynomial(x0 + x1*x2+ x0*x2) * BoolePolynomial(x6 + x7*x8);
    lexbegin = (poly.begin()),
    lexend = (poly.end());
    std::cout <<std::endl<< "lex iteration " <<std::endl;

    while (lexbegin != lexend) {
      std::cout << *lexbegin <<", "<<std::endl; std::cout.flush();
      ++lexbegin;
    }
    
    obegin = (poly.orderedBegin()),
    oend = (poly.orderedEnd());

    std::cout  << "ordered iteration " <<std::endl;

    while (obegin != oend) {
      std::cout << "> "<< *obegin <<" < , "<<std::endl; std::cout.flush();
      ++obegin;
    }

      poly =  BoolePolynomial(x0 ) * BoolePolynomial(x6 + x6*x8*x9 + x7*x8*x9);
    lexbegin = (poly.begin()),
    lexend = (poly.end());
    std::cout <<std::endl << "lex iteration " <<std::endl;

    while (lexbegin != lexend) {
      std::cout << *lexbegin <<", "<<std::endl; std::cout.flush();
      ++lexbegin;
    }
    
    obegin = (poly.orderedBegin()),
    oend = (poly.orderedEnd());

    std::cout << "ordered iteration " <<std::endl;

    while (obegin != oend) {
      std::cout << "> "<< *obegin <<" < , "<<std::endl; std::cout.flush();
      ++obegin;
    }

    poly =  BoolePolynomial(x0 +1) * BoolePolynomial(x1 +1) *
      BoolePolynomial(x2 +1)* BoolePolynomial(x3 +1) * BoolePolynomial(x4 +1) *
      BoolePolynomial(x5 +1)* BoolePolynomial(x6 + 1)* BoolePolynomial(x7 + 1)*
      BoolePolynomial(x8 + 1); 
    lexbegin = (poly.begin()),
    lexend = (poly.end());
    std::cout <<std::endl << "lex iteration " <<std::endl;

    i= 0;
    while (lexbegin != lexend) {
      std::cout << *lexbegin <<", "<<std::endl; std::cout.flush();
      ++lexbegin; ++i;
    }
    
    std::cout << "#terms:  "<<i <<std::endl;

    obegin = (poly.orderedBegin()),
    oend = (poly.orderedEnd());

    std::cout << "ordered iteration " <<std::endl;
    i= 0;
    while (obegin != oend) {
      std::cout << "> "<< *obegin <<" < , "<<std::endl; std::cout.flush();
      ++obegin;; ++i;
    }
    std::cout << "#terms:  "<<i <<std::endl;

    std::cout << "Testing comparison  " <<std::endl;
    monom1 = x1*x2*x6;
    
    monom2 = x1*x2*x7*x8;
    std::cout <<  monom1<< " < " << monom2<<" " ;
    std::cout <<  (monom1.exp() < monom2.exp()) <<std::endl;

    BoolePolynomial p(1);

    std::cout << "Testing != " <<std::endl;    std::cout.flush();

    std::cout <<   (p.orderedBegin()!=p.orderedEnd())<<std::endl; 
    std::cout.flush();
    
 
    std::cout << "Testing== " <<std::endl;    std::cout.flush();
    
    std::cout <<    ( p.orderedBegin()==p.orderedEnd())<<std::endl ;
    std::cout.flush();
    
    std::cout << "last begin: "<<BooleEnv::ordering().lastBlockStart()<<std::endl; 





    BoolePolyRing::block_iterator
      start(BooleEnv::ordering().blockBegin()), 
      finish(BooleEnv::ordering().blockBegin());


    std::cout << "Upper blocks endpints: ";

    while (start != finish) {
      std::cout << *start <<", ";
      ++start;
    }
    std::cout<< std::endl << "lieInSameBlock(0, 3): "<<
      BooleEnv::ordering().lieInSameBlock(0, 3) << std::endl;

    std::cout << "lieInSameBlock(0, 4): "<<
      BooleEnv::ordering().lieInSameBlock(0, 4) << std::endl;

    std::cout << "lieInSameBlock(4,5): "<<
      BooleEnv::ordering().lieInSameBlock(4,5) << std::endl;

    std::cout << "lieInSameBlock(4,6): "<<
      BooleEnv::ordering().lieInSameBlock(4,6) << std::endl;

    std::cout << "lieInSameBlock(3,1000): "<<
      BooleEnv::ordering().lieInSameBlock(3,1000) << std::endl;

    std::cout << "lieInSameBlock(7,1000): "<<
      BooleEnv::ordering().lieInSameBlock(7,1000) << std::endl;

    std::cout << std::endl;



    std::cout << "Switching to lex:  " <<std::endl;
    
    
    BooleEnv::changeOrdering(CTypes::lp);
    
    std::cout <<  monom1<< " < " << monom2<<" " ;
    std::cout <<  (monom1 < monom2) <<std::endl;

    std::cout << "Testing!= " <<std::endl;  
    std::cout <<   (p.orderedBegin()!=p.orderedEnd())<<std::endl;

    std::cout << "Testing== " <<std::endl;  
    std::cout <<    ( p.orderedBegin()==p.orderedEnd())<<std::endl ;

    std::cout << "last begin: "<<BooleEnv::ordering().lastBlockStart()<<std::endl;

    std::cout << "base order code: "<<BooleEnv::ordering().getBaseOrderCode()
              << std::endl;
 

    std::cout << "Finished."<<std::endl;
 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
