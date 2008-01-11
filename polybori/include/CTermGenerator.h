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
 *   (c) 2007 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.11  2008/01/11 16:58:57  dreyer
 * CHANGE: Experimenting with iterators and correct rings
 *
 * Revision 1.10  2007/11/06 15:03:35  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.9  2007/05/18 11:48:39  dreyer
 * ADD: sophisticated term_accumulate
 *
 * Revision 1.8  2007/05/11 11:29:03  dreyer
 * CHANGE: space
 *
 * Revision 1.7  2007/05/10 14:58:30  dreyer
 * CHANGE: removed unnecessary condition
 *
 * Revision 1.6  2007/05/10 13:36:05  dreyer
 * ADD: compile-time switch -D PBORI_NO_TERMS_BY_TAIL
 *
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

#if 0
template <class TermType>
class CTermGeneratorBase<TermType, type_tag<BooleMonomial> >{

public:
  typedef TermType value_type;
  typedef value_type result_type;

  template <class SequenceType>
  result_type operator()(const SequenceType& seq) const{

    value_type result(!seq.isZero());

    typename SequenceType::stack_reverse_iterator 
      start(seq.stackRBegin()), finish(seq.stackREnd());

#ifndef PBORI_NO_TERMS_BY_TAIL
    typename BooleSet::navigator navi(result.diagram().navigation());

    assert((start == finish) || !start->isConstant());
    while((start != finish) && 
          (start->elseBranch().isEmpty()) && (start->thenBranch() == navi)  ) {
      navi = *start;
      ++start;
    }

    result = value_type(BooleSet(navi));
#endif

    while (start != finish){
      result.changeAssign(**start);
      ++start;
    }
    
    return result;
  }
};
#endif //if0

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

/////////
class NoData {};


template <class TermType, class BehaviourTag = type_tag<TermType> >
class MyCTermGeneratorBase;

template <class TermType>
class CTermGeneratorBase<TermType, type_tag<BooleMonomial> >{

public:
  typedef TermType value_type;
  typedef value_type result_type;

  typedef  CTypes::manager_base manager_base;

  typedef  CTypes::dd_type dd_type;
  typedef  dd_type::core_type data_type;

  // typedef  CTypes::manager_base data_type;
  data_type m_data;
  ///CCuddCore * m_data; // non-save variant

  CTermGeneratorBase(const data_type& data): m_data(data) {}

  CTermGeneratorBase(): m_data() {}

  template <class SequenceType>
  result_type operator()(const SequenceType& seq) const{

    assert(m_data != data_type());
    ///@todo: avoid using manager_base here
    value_type result(dd_type(seq.isZero()?  manager_base(m_data).zddZero():
                              manager_base(m_data).zddOne()));


    typename SequenceType::stack_reverse_iterator 
      start(seq.stackRBegin()), finish(seq.stackREnd());

#ifndef PBORI_NO_TERMS_BY_TAIL
    typename BooleSet::navigator navi(result.diagram().navigation());

    assert((start == finish) || !start->isConstant());
    while((start != finish) && 
          (start->elseBranch().isEmpty()) && (start->thenBranch() == navi)  ) {
      navi = *start;
      ++start;
    }

    result = value_type(dd_type(m_data, navi));
#endif

    while (start != finish){
      result.changeAssign(**start);
      ++start;
    }
    
    return result;
  }
};


template <class TermType>
class MyCTermGenerator:
  public MyCTermGeneratorBase<TermType> {
public:
  typedef TermType term_type;
  typedef MyCTermGeneratorBase<term_type> base;
  typedef typename base::data_type data_type;

  MyCTermGenerator(const data_type&  data): base(data) {}
  MyCTermGenerator(): base() {}
};

template <>
class CTermGenerator<BooleMonomial>:
  public CTermGeneratorBase<BooleMonomial> {
public:
  typedef BooleMonomial term_type;
  typedef CTermGeneratorBase<term_type> base;
  typedef base::data_type data_type;

  CTermGenerator(const data_type& data): base(data) {}
  CTermGenerator(const CTermGenerator& rhs): base(rhs) {}
  CTermGenerator(): base() {}
};

END_NAMESPACE_PBORI

#endif
