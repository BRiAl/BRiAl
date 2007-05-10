// -*- c++ -*-
//*****************************************************************************
/** @file CTermGenerator.h
 *
 * @author Alexander Dreyer
 * @date 2007-05-03
 *
 * Defines the template class CTermGenerator, which is a functional for
 * generation of from from a sequence of indices.
 *
 * @par Copyright:
 *   (c) 2007 by
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
 * Revision 1.5  2007/05/10 13:12:56  dreyer
 * CHANGE: using optimized term generation in term-iterator's dereferencing
 *
 * Revision 1.4  2007/05/04 15:52:06  dreyer
 * CHANGE: temporarily deactivated get_tail_term
 *
 * Revision 1.3  2007/05/04 15:26:27  dreyer
 * CHANGE: Optimized version for monomial term generation
 *
 * Revision 1.2  2007/05/04 08:34:54  dreyer
 * Fix: Slowdown removed (due to wrong variable order in CTermGeneratorBase
 *
 * Revision 1.1  2007/05/03 16:04:45  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include polybori functionals
#include "pbori_func.h"

#include "BooleSet.h"

#ifndef CTermGenerator_h_
#define CTermGenerator_h_

BEGIN_NAMESPACE_PBORI


template <class TermType, class BehaviourTag = type_tag<TermType> >
class CTermGeneratorBase;

template <class TermType>
class CTermGeneratorBase<TermType, type_tag<BooleMonomial> >{

public:
  typedef TermType value_type;
  typedef value_type result_type;


  template <class ResultType,  class Iterator>
  void get_tail_term__(ResultType& result, 
                       Iterator start, Iterator finish) const {
    
    
    while (start != finish){
      result.changeAssign(*start);
      ++start;
    }
    
  }
  
  template <class ResultType, class PairType, class Iterator>
  void get_tail_term(ResultType& result, 
                     const PairType& both, Iterator finish) const {
    
    result = BooleSet(both.first);
    get_tail_term__(result, both.second, finish);
    
  }


  template <class SequenceType>
  result_type operator()(const SequenceType& seq) const{

    value_type result(true);

    typename SequenceType::stack_reverse_iterator 
      start(seq.stackRBegin()), finish(seq.stackREnd());

    typename BooleSet::navigator navi(result.diagram().navigation());

    while((start != finish) && (!start->isConstant()) && 
          (start->elseBranch().isEmpty()) && (start->thenBranch() == navi)  ) {
      navi = *start;
      ++start;
    }

    result = value_type(BooleSet(navi));

    while (start != finish){
      result.changeAssign(**start);
      ++start;
    }
    
    return result;
  }
};

class BooleExponent;
template <class TermType>
class CTermGeneratorBase<TermType, type_tag<BooleExponent> > {

public:
  typedef TermType value_type;
  typedef value_type result_type;

  template <class SequenceType>
  result_type operator()(const SequenceType& seq) const{

    value_type result;
    result.reserve(seq.deg());
    typename SequenceType::const_iterator 
      start(seq.begin()), finish(seq.end());

    while (start != finish){
      result.push_back(*start);
      ++start;
    }
    return result;
  }
};

template <class TermType>
class CTermGeneratorBase<TermType, type_tag<CTypes::size_type> > {
public:
  typedef TermType value_type;
  typedef value_type result_type;

  template <class SequenceType>
  result_type operator()(const SequenceType& seq) const{
    return seq.deg();
  }
};



template <class TermType>
class CTermGenerator:
  public CTermGeneratorBase<TermType> {

};


END_NAMESPACE_PBORI

#endif
