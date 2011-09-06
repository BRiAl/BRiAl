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
 *   (c) 2007-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_iterators_CTermGenerator_h_
#define polybori_iterators_CTermGenerator_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include polybori functionals
#include <polybori/routines/pbori_func.h>

  //#include <polybori/BooleSet.h>



BEGIN_NAMESPACE_PBORI


template <class TermType, class BehaviourTag>
class CTermGeneratorBase__;


class BooleExponent;
template <class TermType>
class CTermGeneratorBase__<TermType, type_tag<BooleExponent> > {

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
class CTermGeneratorBase__<TermType, type_tag<CTypes::size_type> > {
public:
  typedef TermType value_type;
  typedef value_type result_type;

  template <class SequenceType>
  result_type operator()(const SequenceType& seq) const{
    return seq.deg();
  }
};


template <class TermType>
class CTermGeneratorBase__<TermType, type_tag<CTypes::deg_type> > {
public:
  typedef TermType value_type;
  typedef value_type result_type;

  template <class SequenceType>
  result_type operator()(const SequenceType& seq) const{
    return seq.deg();
  }
};


/////////


template <class TermType>
class CTermGeneratorBase__<TermType, type_tag<BooleMonomial> >{

public:
  typedef TermType value_type;
  typedef value_type result_type;

  //  typedef  CTypes::manager_base manager_base;


  typedef  BoolePolyRing data_type;
  typedef  data_type::dd_type dd_type;

  // typedef  CTypes::manager_base data_type;
  data_type m_data;
  ///CCuddCore * m_data; // non-save variant

  CTermGeneratorBase__(const data_type& data): m_data(data) {}

  CTermGeneratorBase__(): m_data() {}

  template <class SequenceType>
  result_type operator()(const SequenceType& seq) const {
    ///    PBORI_ASSERT(m_data != data_type()); // todo???

    // Do not dereference empty sequence (corresponds to end())
    PBORI_ASSERT(!seq.isZero());

    // @todo: avoid using manager_base here
    typedef typename value_type::ring_type ring_type;
    ///    typedef typename ring_type::manager_type manager_type;
    //    value_type result((ring_type)manager_type(m_data));
    value_type result((ring_type)(m_data));

    typename SequenceType::stack_reverse_iterator 
      start(seq.stackRBegin()), finish(seq.stackREnd());

#ifndef PBORI_NO_TERMS_BY_TAIL
    typename BooleSet::navigator navi(result.diagram().navigation());

    PBORI_ASSERT((start == finish) || !start->isConstant());
    while((start != finish) && 
          (start->elseBranch().isEmpty()) && (start->thenBranch() == navi)  ) {
      navi = *start;
      ++start;
    }

    result = value_type(dd_type(m_data, navi));
#endif

    while (start != finish){
      result = result.change(**start);
      ++start;
    }
    
    return result;
  }
};


template <class TermType>
class CTermGeneratorBase:
  public CTermGeneratorBase__<TermType, type_tag<TermType> > {

};


template <class TermType>
class CTermGenerator:
  public CTermGeneratorBase<TermType> {
public:
  typedef CTermGeneratorBase<TermType> base;

  typedef BoolePolyRing data_type;

  CTermGenerator(const data_type&): base() {}
  CTermGenerator(const CTermGenerator& rhs): base(rhs) {}
  CTermGenerator(): base() {}

};


template <>
class CTermGenerator<BooleMonomial>:
  public CTermGeneratorBase__<BooleMonomial, type_tag<BooleMonomial> > {
public:
  typedef BooleMonomial term_type;
  typedef CTermGeneratorBase__<BooleMonomial, type_tag<BooleMonomial> > base;
  //typedef CTermGeneratorBase<term_type> base;
  typedef base::data_type data_type;

  CTermGenerator(const data_type& data): base(data) {}
  CTermGenerator(const CTermGenerator& rhs): base(rhs) {}
  //  CTermGenerator(): base() {}
};

END_NAMESPACE_PBORI

#endif
