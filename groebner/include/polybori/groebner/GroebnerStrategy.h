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

#include "PairManagerFacade.h"
#include "ReductionStrategy.h"
#include "groebner_defs.h"
#include "PolyEntryPtrLmLess.h"
#include "GroebnerOptions.h"

#include <vector>
#include <boost/shared_ptr.hpp>

#include <polybori/routines/pbori_algo.h> // member-for_each etc.

BEGIN_NAMESPACE_PBORIGB


/** @class GroebnerStrategy
 * @brief This class defines GroebnerStrategy.
 *
 **/
class GroebnerStrategy:
  public GroebnerOptions, public PairManagerFacade<GroebnerStrategy> {

  typedef GroebnerStrategy self;
public:
  /// copy constructor
  GroebnerStrategy(const GroebnerStrategy& orig);

  /// Construct from a ring
  GroebnerStrategy(const BoolePolyRing& input_ring):
    GroebnerOptions(input_ring.ordering().isBlockOrder(), 
                    !input_ring.ordering().isDegreeOrder()),
    PairManagerFacade<GroebnerStrategy>(input_ring),
    generators(input_ring),

    cache(new CacheManager()),
    chainCriterions(0),  variableChainCriterions(0), 
    easyProductCriterions(0), extendedProductCriterions(0) { }

  const BoolePolyRing& ring() const { return generators.leadingTerms.ring(); }
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

  void log(const char* c) const { if (enabledLog) std::cout<<c<<std::endl; }

  Polynomial redTail(const Polynomial& p);
  std::vector<Polynomial> noroStep(const std::vector<Polynomial>&);
  std::vector<Polynomial> faugereStepDense(const std::vector<Polynomial>&);

  Polynomial nf(Polynomial p) const;
  void symmGB_F2();
  int suggestPluginVariable();
  std::vector<Polynomial> allGenerators();


  bool checkSingletonCriterion(int i, int j) const {
    return generators[i].isSingleton() && generators[j].isSingleton();
  }

  bool checkPairCriteria(const Exponent& lm, int i, int j) {
    return checkSingletonCriterion(i, j) || checkExtendedProductCriterion(i, j)
      || checkChainCriterion(lm, i, j);
  }

  bool checkChainCriterion(const Exponent& lm, int i, int j);
  bool checkExtendedProductCriterion(int i, int j);


  bool checkVariableSingletonCriterion(int idx) const {
    return generators[idx].isSingleton();
  }

  bool checkVariableLeadOfFactorCriterion(int idx, int var) const {
    bool result = generators[idx].literal_factors.occursAsLeadOfFactor(var);
    if (result)
      log("delayed variable linear factor criterion");
    return result;
  }

  bool checkVariableChainCriterion(int idx) {
    bool result = !generators[idx].minimal;
    if (result)
      ++variableChainCriterions;
    return result;
  }
 
  bool checkVariableCriteria(int idx, int var) {
    return PBORI_UNLIKELY(checkVariableSingletonCriterion(idx) || 
		    checkVariableLeadOfFactorCriterion(idx, var)) ||
      checkVariableChainCriterion(idx);
  }
protected:
  std::vector<Polynomial> treatVariablePairs(PolyEntryReference);
  void normalPairsWithLast(const MonomialSet&);
  void addVariablePairs(PolyEntryReference);

  std::vector<Polynomial> add4ImplDelayed(PolyEntryReference);
  std::vector<Polynomial> add4ImplDelayed(const Polynomial& p, const Exponent& lm_exp, 
                                          const Exponent& used_variables) const;


  std::vector<Polynomial> addHigherImplDelayedUsing4(PolyEntryReference);
  std::vector<Polynomial> addHigherImplDelayedUsing4(const LiteralFactorization&) const;


private:

  int addGeneratorStep(const PolyEntry&);

  void addImplications(const BoolePolynomial& p, std::vector<int>& indices);


  bool add4ImplDelayed(const Polynomial& p, const Exponent& lm_exp, 
                       const Exponent& used_variables, bool include_orig,
                       std::vector<Polynomial>& impl) const;

  bool addHigherImplDelayedUsing4(const LiteralFactorization&,
                                  bool include_orig, std::vector<Polynomial>&) const;

  template <class Iterator>
  void addImplications(Iterator, Iterator, std::vector<int>& indices);
  void addImplications(const std::vector<Polynomial>& impl, int s);

  typedef std::set<const PolyEntry*, PolyEntryPtrLmLess> entryset_type;

  void propagateStep(entryset_type& others);
  void exchange(const Polynomial&, const PolyEntry&, entryset_type&);
  void updatePropagated(const PolyEntry& entry);


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

  bool shorterElimination(const MonomialSet& divisors, wlen_type el,
                           MonomialSet::deg_type deg) const;
public:
  /// @name public available parameters
  ReductionStrategy generators;
  boost::shared_ptr<CacheManager> cache;

  unsigned int reductionSteps;
  int normalForms;
  int currentDegree;
  int averageLength;

  int chainCriterions;
  int variableChainCriterions;
  int easyProductCriterions;
  int extendedProductCriterions;

};

END_NAMESPACE_PBORIGB

#endif /* polybori_GroebnerStrategy_h_ */
