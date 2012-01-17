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
#include "groebner_defs.h"
#include "pairs.h"
#include "cache_manager.h"

#include "PairManager.h"
#include "ReductionStrategy.h"

#include <vector>
#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE_PBORIGB

class PolyEntryPtrLmLess {
public:
  bool operator()(const PolyEntry* lhs, const PolyEntry* rhs) const {
    return lhs->lead < rhs->lead;
  }
};


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

  void addGenerator(const BoolePolynomial& p);
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
  std::vector<Polynomial> treatVariablePairs(int s);
  void treatNormalPairs(int s,MonomialSet intersecting_terms,
                        MonomialSet other_terms, MonomialSet ext_prod_terms);
  void addVariablePairs(int s);
  std::vector<Polynomial>
  add4ImplDelayed(const Polynomial& p, const Exponent& lm_exp,
                  const Exponent& used_variables, int s, bool include_orig);
  std::vector<Polynomial>
  addHigherImplDelayedUsing4(int s,const LiteralFactorization& literal_factors,
                             bool include_orig);

private:

  int addGeneratorStep(const BoolePolynomial& p);

  void addImplications(const BoolePolynomial& p, std::vector<int>& indices);

  template <class Iterator>
  void addImplications(Iterator, Iterator, std::vector<int>& indices);
  void addImplications(const std::vector<Polynomial>& impl, int s);

  typedef std::set<const PolyEntry*, PolyEntryPtrLmLess> entryset_type;

  void propagate_step(entryset_type& others);
  void exchange(const Polynomial&, const PolyEntry&, entryset_type&);
  void update_propagated(const PolyEntry& entry);


  template <class Iterator>
  void check_len1_crit(const int s, Iterator is_it, Iterator is_end,
		       bool e_len_not_1) {
    pairs.status.prolong(PairStatusSet::HAS_T_REP);
    
    while(is_it != is_end) {
      int index = generators.index(*is_it);
      PBORI_ASSERT(index != s);
      //product criterion doesn't hold
      //try length 1 crit
      if (generators[index].length != 1 || e_len_not_1)
	pairs.status.setToUncalculated(index, s);
      else
	++extendedProductCriterions;

      ++is_it;
    }
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
