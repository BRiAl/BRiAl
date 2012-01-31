// -*- c++ -*-
//*****************************************************************************
/** @file GroebnerStrategy.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c GroebnerStrategy.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_GroebnerStrategy_h_
#define polybori_groebner_GroebnerStrategy_h_

// include basic definitions
#include "pairs.h"
#include "cache_manager.h"

#include "PairManager.h"
#include "ReductionStrategy.h"
#include "groebner_defs.h"

#include <vector>
#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE_PBORIGB

class PolyEntryPtrLmLess {
public:
  bool operator()(const PolyEntry* lhs, const PolyEntry* rhs) const {
    return lhs->lead < rhs->lead;
  }
};



template <class InIter, class OutIter, class Object, class MemberFuncPtr>
inline OutIter
transform(InIter start, InIter finish, OutIter result, Object& obj,
	  MemberFuncPtr func) {
  for(; start != finish; ++start, ++result)
    *result = (obj .* func)(*start);
}

template <class InIter, class Object, class MemberFuncPtr>
inline void
for_each(InIter start, InIter finish, Object& obj, MemberFuncPtr func) {
  for(; start != finish; ++start)
    (obj .* func)(*start);
}

template <class InIter, class Object, class MemberFuncPtr>
inline void
for_each(InIter start, InIter finish, const Object& obj, MemberFuncPtr func) {
  for(; start != finish; ++start)
    (obj .* func)(*start);
}


/** @class GroebnerStrategy
 * @brief This class defines GroebnerStrategy.
 *
 **/
class GroebnerStrategy{

  typedef GroebnerStrategy self;
public:
  /// copy constructor
  GroebnerStrategy(const GroebnerStrategy& orig);

  /// Construct from a ring
  GroebnerStrategy(const BoolePolyRing& input_ring):
    generators(input_ring), r(input_ring), pairs(*this, input_ring),
    optRedTailInLastBlock(input_ring.ordering().isBlockOrder()),
    optLazy(!input_ring.ordering().isDegreeOrder()),

    cache(new CacheManager()), matrixPrefix("mat"),
    chainCriterions(0), easyProductCriterions(0), extendedProductCriterions(0),

    optDrawMatrices(false), optModifiedLinearAlgebra(false),
    optDelayNonMinimals(true), enabledLog(false),
    variableChainCriterions(0),  optExchange(true), optHFE(false),
    optStepBounded(false), optAllowRecursion(true),
    optLinearAlgebraInLastBlock(true),
    reduceByTailReduced(false) {

  }

  bool containsOne() const { return generators.leadingTerms.ownsOne(); }
  
  std::vector<Polynomial>  minimalizeAndTailReduce();
  std::vector<Polynomial>  minimalize();

  void addGenerator(const PolyEntry& entry);
  void addGeneratorDelayed(const BoolePolynomial & p);
  void addAsYouWish(const Polynomial& p);  
  void addGeneratorTrySplit(const Polynomial& p, bool is_minimal);

  bool variableHasValue(idx_type i);
  void llReduceAll();

  void treat_m_p_1_case(const PolyEntry& e) {
    generators.monomials_plus_one.update(e);
  }


  Polynomial nextSpoly(){ return pairs.nextSpoly(generators);  }
  void addNonTrivialImplicationsDelayed(const PolyEntry& p);
  void propagate(const PolyEntry& e); 

  void log(const char* c){ if (enabledLog) std::cout<<c<<std::endl; }

  Polynomial redTail(const Polynomial& p);
  std::vector<Polynomial> noroStep(const std::vector<Polynomial>&);
  std::vector<Polynomial> faugereStepDense(const std::vector<Polynomial>&);

  Polynomial nf(Polynomial p) const;
  void symmGB_F2();
  int suggestPluginVariable();
  std::vector<Polynomial> allGenerators();

  template <class Iterator>
  bool sameRing(Iterator start, Iterator finish) const {
  
    while (start != finish) {
      if (r.id() != start->ring().id())
        return false;
      ++start;
    }
    return true;
  }

protected:
  std::vector<Polynomial> treatVariablePairs(PolyEntryReference);
  void normalPairsWithLast(const MonomialSet&);
  void addVariablePairs(PolyEntryReference);
  template <class EntryType>  std::vector<Polynomial>
  add4ImplDelayed(const Polynomial& p, const Exponent& lm_exp,
                  const Exponent& used_variables, EntryType entry, bool include_orig);
  std::vector<Polynomial>
  addHigherImplDelayedUsing4(PolyEntryReference);

  std::vector<Polynomial>
  addHigherImplDelayedUsing4(const LiteralFactorization& literal_factors) const;

  bool
  addHigherImplDelayedUsing4_(const LiteralFactorization& literal_factors,
			      bool include_orig, std::vector<Polynomial>&) const;

private:

  int addGeneratorStep(const PolyEntry&);

  void addImplications(const BoolePolynomial& p, std::vector<int>& indices);

  template <class Iterator>
  void addImplications(Iterator, Iterator, std::vector<int>& indices);
  void addImplications(const std::vector<Polynomial>& impl, int s);

  typedef std::set<const PolyEntry*, PolyEntryPtrLmLess> entryset_type;

  void propagate_step(entryset_type& others);
  void exchange(const Polynomial&, const PolyEntry&, entryset_type&);
  void update_propagated(const PolyEntry& entry);

  // product criterion doesn't hold - try length 1 crit
  void checkSingletonCriterion(const PolyEntry& entry,
			       const MonomialSet& intersection) {

    PBORI_ASSERT(generators.checked_index(entry) == -1);
    pairs.status.prolong(PairStatusSet::HAS_T_REP);

    for_each(intersection.expBegin(), intersection.expEnd(), *this,
             (entry.isSingleton()?  &self::markNextSingleton:
	      &self::markNextUncalculated));
  }

  /// check singleton and product criteria
  void checkCriteria(const PolyEntry& entry, const MonomialSet& terms) {
    checkSingletonCriterion(entry, terms);
    easyProductCriterions += generators.minimalLeadingTerms.length() -
      terms.length();
  }

  void markNextSingleton(const Exponent& key) {
    if (generators[key].isSingleton())
      ++extendedProductCriterions;
    else
      markNextUncalculated(key);
  }

  void markNextUncalculated(const BooleExponent& key) {
    pairs.status.setToUncalculated(generators.index(key), generators.size());
  }

public:
  /// @name public available parameters
  PairManager pairs;
  bool reduceByTailReduced;
  ReductionStrategy generators;
  bool optDrawMatrices;
  std::string matrixPrefix;
  //const char* matrixPrefix;
  boost::shared_ptr<CacheManager> cache;
  BoolePolyRing r;
  bool enabledLog;
   unsigned int reductionSteps;
  int normalForms;
  int currentDegree;
  int chainCriterions;
  int variableChainCriterions;
  int easyProductCriterions;
  int extendedProductCriterions;
  int averageLength;
  
  bool optHFE;
  bool optLazy;
  bool optModifiedLinearAlgebra;
  bool optDelayNonMinimals;
  
  bool optExchange;
  bool optAllowRecursion;
  
  bool optStepBounded;
  bool optLinearAlgebraInLastBlock;
  bool optRedTailInLastBlock;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_GroebnerStrategy_h_ */
