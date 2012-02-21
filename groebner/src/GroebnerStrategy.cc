// -*- c++ -*-
//*****************************************************************************
/** @file GroebnerStrategy.cc 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c GroebnerStrategy.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************


// include definitions

#include <polybori/groebner/HasTRepOrExtendedProductCriterion.h>
#include <polybori/groebner/ShorterEliminationLengthModified.h>
#include <polybori/groebner/tables.h>
#include <polybori/groebner/ExpGreater.h>
#include <polybori/groebner/ChainCriterion.h>

// for extended_product_criterion:
#include <polybori/groebner/HasTRepOrExtendedProductCriterion.h>

  //#include <polybori/BooleSetSequence.h>

#include <polybori/groebner/minimal_elements.h>
#include <polybori/groebner/groebner_alg.h>
#include <polybori/groebner/nf.h>
#include <polybori/groebner/interpolate.h>
#include <polybori/groebner/red_tail.h>
#include <polybori/groebner/ll_red_nf.h>
#include <polybori/groebner/add_up.h>
#include <polybori/groebner/fixed_path_divisors.h>
#include <polybori/groebner/linear_algebra_step.h>
#include <polybori/groebner/GroebnerStrategy.h>
#include <polybori/groebner/RelatedTerms.h>
#include <polybori/groebner/PolyFromPolyEntry.h>
#include <polybori/groebner/TimesConstantImplication.h>
#include <polybori/groebner/TimesVariableSubstitution.h>
#include <polybori/groebner/IsVariableOfIndex.h>
#include <polybori/groebner/RedTailNth.h>
#include <polybori/groebner/GetNthPoly.h>
#include <polybori/groebner/RankingVector.h>
#include <sstream>

BEGIN_NAMESPACE_PBORIGB


static Polynomial opposite_logic_mapping(Polynomial p){
    return map_every_x_to_x_plus_one(p)+1;
}

static std::vector<Polynomial> small_next_degree_spolys(GroebnerStrategy& strat, double f, int n){
  std::vector<Polynomial> res;
  PBORI_ASSERT(!(strat.pairs.pairSetEmpty()));
  strat.pairs.cleanTopByChainCriterion();
  deg_type deg=strat.pairs.queue.top().sugar;
  wlen_type wlen=strat.pairs.queue.top().wlen;
  while((!(strat.pairs.pairSetEmpty())) &&(strat.pairs.queue.top().sugar<=deg) && (strat.pairs.queue.top().wlen<=wlen*f+2)&& (res.size()<(std::size_t)n)){
    
    PBORI_ASSERT(strat.pairs.queue.top().sugar==deg);
    res.push_back(strat.nextSpoly());
    strat.pairs.cleanTopByChainCriterion();
  }
  return res;
  
}

// class members
GroebnerStrategy::GroebnerStrategy(const GroebnerStrategy& orig):
  GroebnerOptions(orig),
  PairManagerFacade<GroebnerStrategy>(orig),
  generators(orig.generators),
  cache(orig.cache),

  reductionSteps(orig.reductionSteps),
  normalForms(orig.normalForms),
  currentDegree(orig.currentDegree),
  averageLength(orig.averageLength),
  chainCriterions(orig.chainCriterions),
  variableChainCriterions(orig.variableChainCriterions),
  easyProductCriterions(orig.easyProductCriterions),
  extendedProductCriterions(orig.extendedProductCriterions) {

  //  this->pairs.strat = this;
}

bool 
GroebnerStrategy::checkChainCriterion(const Exponent& lm, int i, int j) {
    const MonomialSet lms =
      generators.leadingTerms.intersect(lm.divisors(ring()));

    PBORI_ASSERT(lm == generators[i].leadExp.LCM(generators[j].leadExp));
    bool result = std::find_if(lms.expBegin(), lms.expEnd(),
                               ChainCriterion(*this, i, j)) != lms.expEnd();
    if (result)
      ++chainCriterions; 
    return result;
  }

bool
GroebnerStrategy::checkExtendedProductCriterion(int i, int j) {
  bool result = extended_product_criterion(generators[i], generators[j]);
  if (result)
   ++extendedProductCriterions;

  return result;
}


void GroebnerStrategy::llReduceAll(){
  generators.llReduceAll();
}

Polynomial GroebnerStrategy::redTail(const Polynomial & p){
    return red_tail(this->generators,p);
}

inline Polynomial
reduce_by_small_entry(const Polynomial& poly, const PolyEntry& entry) {

  PBORI_ASSERT((entry.length == 1) || (entry.length == 2));

  return (entry.length == 1?
          cancel_monomial_in_tail(poly, entry.lead):
          reduce_by_binom_in_tail(poly, entry.p));
}

void GroebnerStrategy::propagateStep(entryset_type& others) {

  const PolyEntry& entry = *(*others.begin());
  others.erase(others.begin());

  if (should_propagate(entry)) {
    for(PolyEntryVector::const_iterator start(generators.begin()),
	  finish(generators.end()); start != finish; ++start)
      if (start->propagatableBy(entry))
	exchange(reduce_by_small_entry(start->p, entry), *start, others);
  }
}


void
GroebnerStrategy::exchange(const Polynomial& reduced, const PolyEntry& entry,
			   entryset_type& others) {

  if (reduced != entry.p) {
    updatePropagated(generators(entry) = reduced);
    others.insert(&entry);
  }
}

void
GroebnerStrategy::updatePropagated(const PolyEntry& entry) {

  generators.monomials.update(entry);
  if ( (entry.length == 2) && (entry.ecart() == 0))
    addNonTrivialImplicationsDelayed(entry);
}


void GroebnerStrategy::propagate(const PolyEntry& e){

  entryset_type entries;
  entries.insert(&e);

  while (!entries.empty())
    propagateStep(entries);

}

std::vector<Polynomial>
GroebnerStrategy::addHigherImplDelayedUsing4(PolyEntryReference entry) {

  std::vector<Polynomial> impl;
  bool directly = 
    addHigherImplDelayedUsing4(((const PolyEntry&)entry).literal_factors,
                               false, impl);
  entry.markVariablePairsCalculated();
  if (directly)
    return impl;
  
  for_each(impl.begin(), impl.end(), *this, &self::addGeneratorDelayed);
  return std::vector<Polynomial>();
}

std::vector<Polynomial>
GroebnerStrategy::addHigherImplDelayedUsing4(const LiteralFactorization& 
					     literal_factors) const {
  std::vector<Polynomial> impl;
  addHigherImplDelayedUsing4(literal_factors, true, impl);
  return impl;
}

bool
GroebnerStrategy::addHigherImplDelayedUsing4(const LiteralFactorization& 
                                             literal_factors,
                                             bool include_orig,
                                             std::vector<Polynomial>& impl) const {

    if (literal_factors.rest.isOne()){
      // entry.markVariablePairsCalculated();
      return false;
    }

    Polynomial p=literal_factors.rest;
    BoolePolyRing ring(p.ring());
   
    //Monomial used_variables_m=p.usedVariables();
    Exponent used_variables = p.usedVariablesExp();
    Exponent e = p.leadExp();
    if (e.size()>4) std::cerr<<"too many variables for table"<<std::endl;
    
    std::vector<char> ring_2_0123(ring.nVariables());
    std::vector<idx_type> back_2_ring(4);
    set_up_translation_vectors(ring_2_0123, back_2_ring, used_variables);
    unsigned int p_code=p2code_4(p, ring_2_0123);
    if ((get_table_entry4(ring, p_code,0) == p_code) &&
        (get_table_entry4(ring, p_code,1) == 0)){
      return false;
    }
    
    bool can_add_directly=true;
    for(int i=0;get_table_entry4(ring, p_code,i)!=0;i++){
      unsigned int impl_code=get_table_entry4(ring, p_code,i);

        if ((include_orig) ||(p_code!=impl_code)){
          Polynomial p_i=code_2_poly_4(ring, impl_code, back_2_ring);
            Exponent e_i=p_i.leadExp();

            if (include_orig || (e_i != e) ) {
	      p_i = multiply_with_literal_factors(literal_factors, p_i);
	      impl.push_back(p_i);
	      can_add_directly &= generators.minimalLeadingTerms.divisorsOf(p_i.leadExp()).isZero();
                //e_i is wrong here, have to multiply
            }
        }
    }
    
    if (can_add_directly)
      return true;
    else if (include_orig)
      impl.clear();

    return false;
}
std::vector<Polynomial>
GroebnerStrategy::add4ImplDelayed(PolyEntryReference entry) {

  const PolyEntry& e = entry;
  std::vector<Polynomial> impl;

  bool directly = add4ImplDelayed(e.p, e.leadExp, e.usedVariables, false, impl);
  entry.markVariablePairsCalculated();
  if (directly)
    return impl;
 
  for_each(impl.begin(), impl.end(), *this, &self::addGeneratorDelayed);
  return std::vector<Polynomial>();
}

std::vector<Polynomial>
GroebnerStrategy::add4ImplDelayed(const Polynomial& p, const Exponent& lm_exp, 
                                  const Exponent& used_variables) const {
  std::vector<Polynomial> impl;
  add4ImplDelayed(p, lm_exp, used_variables, true, impl);
  return impl;
}

bool
GroebnerStrategy::add4ImplDelayed(const Polynomial& p, const Exponent& lm_exp, 
				  const Exponent& used_variables,
				  bool include_orig, std::vector<Polynomial>& impl) const {
    //cout<<"I am here";
    //Polynomial p=generators[s].p;
    //Exponent used_variables=generatusedVariables;

    Exponent e=lm_exp;//generators[s].lmExp;
    BoolePolyRing ring(p.ring());

    std::vector<char> ring_2_0123(ring.nVariables());
    std::vector<idx_type> back_2_ring(4);
    set_up_translation_vectors(ring_2_0123, back_2_ring, used_variables);
    
    unsigned int p_code=p2code_4(p, ring_2_0123);

    if ((get_table_entry4(ring, p_code, 0)==p_code) &&
        (get_table_entry4(ring, p_code, 1)==0)) {
      return false;
    }
    
    int i;
    bool can_add_directly=true;

    for(i=0;get_table_entry4(ring, p_code,i)!=0;i++){
      unsigned int impl_code=get_table_entry4(ring, p_code, i);
        if ((include_orig) ||(p_code!=impl_code)){
          Polynomial p_i=code_2_poly_4(ring, impl_code, back_2_ring);
            Exponent e_i=p_i.leadExp();

            if ((include_orig) ||(e_i!=e)){
                impl.push_back(p_i);
                can_add_directly &= generators.minimalLeadingTerms.divisorsOf(e_i).isZero();
            }
        }
    }
    
    
    if (can_add_directly)
      return true;
    else if (include_orig)
      impl.clear();

    return false;
}

void
GroebnerStrategy::addVariablePairs(PolyEntryReference entryref) {

  const PolyEntry& entry = entryref;
  Exponent::const_iterator it = entry.leadExp.begin();
  Exponent::const_iterator end = entry.leadExp.end();
  while(it != end){
    if ((entry.lead.deg() == 1) ||
        entry.literal_factors.occursAsLeadOfFactor(*it)) {
      entryref.vPairCalculated.insert(*it);
    } 
    else
      pairs.introducePair(Pair(generators.size() - 1, *it, generators,
			       VARIABLE_PAIR));
    it++;
  } 
}

std::vector<Polynomial>
GroebnerStrategy::treatVariablePairs(PolyEntryReference entry){

  const PolyEntry& e = entry;
  if (have_ordering_for_tables(e.p.ring()) ||
      (have_base_ordering_for_tables(e.p.ring()) && e.p.inSingleBlock())) {
    int uv=e.usedVariables.deg();
    if (uv<=4){
      return add4ImplDelayed(entry);
    } 
    else {
      int uv_opt = uv-e.literal_factors.factors.size()-2*e.literal_factors.var2var_map.size();
      ////should also be proofable for var2var factors
      PBORI_ASSERT(uv_opt==e.literal_factors.rest.nUsedVariables());//+2*var2var_map.size());
      if (uv_opt<=4){
        return addHigherImplDelayedUsing4(entry);
      }
    }
  }


  std::vector<Polynomial> empty;
  
  Polynomial rest=e.literal_factors.rest;
  
  Monomial rest_uv=rest.usedVariables();
  if ((rest_uv.deg() > 1) && (rest_uv.deg() < 12)){
    MonomialSet my_ones = zeros(rest + 1, rest_uv.divisors());
    if ((my_ones.size() << rest.leadDeg()) == (std::size_t(1) << rest_uv.deg()))
      return empty;
  }

  addVariablePairs(entry);
  return empty;
}



void 
GroebnerStrategy::normalPairsWithLast(const MonomialSet& act_l_terms) {

  const int s  = generators.size() - 1;
    
  if (std::find_if(act_l_terms.expBegin(), act_l_terms.expEnd(), 
                   HasTRepOrExtendedProductCriterion(*this, s)) ==
      act_l_terms.expEnd()) {
    
    Exponent min = *std::min_element(act_l_terms.expBegin(),
                                     act_l_terms.expEnd(),
                                     LessWeightedLengthInStrat(generators));
    
    pairs.introducePair(Pair(generators.index(min), s, generators));
  }
}


std::vector<Polynomial>
GroebnerStrategy::allGenerators(){
  if (generators.empty())
    return std::vector<Polynomial>();

  std::vector<Polynomial> result(generators.size(), generators.front().p.ring());
  std::transform(generators.begin(), generators.end(), result.begin(),
		 PolyFromPolyEntry());
  pairs.appendHiddenGenerators(result);
  
  return result;
}


int
GroebnerStrategy::addGeneratorStep(const PolyEntry& entry){

  PBORI_ASSERT(generators.empty() ||
               entry.p.ring().id() == generators[0].p.ring().id());

  // here we make use of the fact, that the index of the 1 node is 
  // bigger than that of variables
  generators.reducibleUntil = std::max(generators.reducibleUntil,
                                       *entry.p.navigation());
  //do this before adding leading term
  propagate(entry);

  RelatedTerms related = generators.related(entry);
  checkCriteria(entry, related.terms());

  RelatedTerms::divisors_sequence_type
    related_divisors(related.divisors(generators.minimalLeadingTerms));

  generators.addGenerator(entry);

  for_each(related_divisors.begin(), related_divisors.end(),
           *this, &GroebnerStrategy::normalPairsWithLast);

  return generators.size() - 1;
}

void
GroebnerStrategy::addImplications(const BoolePolynomial& poly,
				  std::vector<int>& indices){
  PBORI_ASSERT(!poly.isZero());
  int idx = addGeneratorStep((generators.optRedTail?
			      red_tail(generators, poly): poly));

  pairs.status.setToHasTRep(indices.begin(), indices.end(), idx);
  indices.push_back(idx);
  generators.last().markVariablePairsCalculated();
}

template <class Iterator>
void
GroebnerStrategy::addImplications(Iterator start, Iterator finish,
				  std::vector<int>& indices){
  for(; start != finish; ++start)
    addImplications(*start, indices);
}

void
GroebnerStrategy::addImplications(const std::vector<Polynomial>& impl, int s) {
  std::vector<int> indices(1, s);
  addImplications(impl.begin(), impl.end(), indices);
}

void
GroebnerStrategy::addGenerator(const PolyEntry& e_arg) {

  int idx = addGeneratorStep(e_arg);
  PolyEntryReference entry = generators.last();

  if (entry.minimal)
    addImplications(treatVariablePairs(entry), idx);
  else
    entry.markVariablePairsCalculated();
}


void
GroebnerStrategy::addNonTrivialImplicationsDelayed(const PolyEntry& e){
  Polynomial p_opp=opposite_logic_mapping(e.p);
  //cout<<"p_opp"<<p_opp<<endl;
  const Polynomial::ring_type ring(p_opp.ring());
  const Polynomial one_element(ring.one());
  Polynomial mult_by(one_element);
  LiteralFactorization factors_opp(p_opp);
  
  if (factors_opp.trivial()){
    if (e.literal_factors.trivial() || e.literal_factors.rest.isOne()) return;
    
    mult_by = std::accumulate(e.literal_factors.factors.begin(),
                              e.literal_factors.factors.end(),
                              one_element, TimesConstantImplication());

 
    mult_by = std::accumulate(e.literal_factors.var2var_map.begin(),
                              e.literal_factors.var2var_map.end(), 
                              mult_by, TimesVariableSubstitution());

    p_opp=opposite_logic_mapping(e.literal_factors.rest);
    factors_opp=LiteralFactorization(p_opp);
    if (factors_opp.trivial()) return;
  }
  
  
  
   {
    this->log("found new implications");
    //cout<<"implications"<<endl;
    if (!(factors_opp.rest.isOne())){
      addGeneratorDelayed(opposite_logic_mapping(factors_opp.rest));
    }
    LiteralFactorization::map_type::const_iterator itf=factors_opp.factors.begin();
    LiteralFactorization::map_type::const_iterator endf=factors_opp.factors.end();
    while(itf != endf){
      addGeneratorDelayed((ring.variable(itf->first) + bool(itf->second)) *
                          mult_by);
      ++itf;
    }
    
    LiteralFactorization::var2var_map_type::const_iterator itv=factors_opp.var2var_map.begin();
    LiteralFactorization::var2var_map_type::const_iterator endv=factors_opp.var2var_map.end();
    while(itv != endv){
      addGeneratorDelayed(mult_by*(ring.variable(itv->first) + 
                                   ring.variable(itv->second) + 1));
      ++itv; 
    }
  }
}

void GroebnerStrategy::addGeneratorDelayed(const BoolePolynomial& p){
  PBORI_ASSERT(generators.empty() ||
               p.ring().id() == generators[0].p.ring().id());
  pairs.introducePair(Pair(p));
}


bool GroebnerStrategy::variableHasValue(idx_type v){
  return std::find_if(generators.begin(), generators.end(),
                      IsVariableOfIndex(v)) != generators.end();
}

std::vector<Polynomial>
GroebnerStrategy::minimalizeAndTailReduce(){

    bool tail_growth_bak = generators.optRedTailDegGrowth;
    generators.optRedTailDegGrowth = true;

    MonomialSet minelts = minimal_elements(generators.minimalLeadingTerms);
    std::vector<Polynomial> result(minelts.size(), minelts.ring());
    std::transform(minelts.rExpBegin(), minelts.rExpEnd(), result.rbegin(), 
                   RedTailNth(generators));

    generators.optRedTailDegGrowth = tail_growth_bak;
    return result;
}

std::vector<Polynomial>
GroebnerStrategy::minimalize(){

    MonomialSet minelts = minimal_elements(generators.minimalLeadingTerms);
    std::vector<Polynomial> result(minelts.size(), minelts.ring());
    std::transform(minelts.begin(), minelts.end(), result.begin(), 
                   GetNthPoly(generators));

    return result;
    
}
void GroebnerStrategy::addGeneratorTrySplit(const Polynomial & p, bool is_minimal){

  PBORI_ASSERT(generators.empty() || p.ring().id() == generators[0].p.ring().id());
  std::vector<Polynomial> impl;
  int way=0;
  if ((have_ordering_for_tables(p.ring())) ||
      ((have_base_ordering_for_tables(p.ring())) &&
       (p.inSingleBlock()))) { 

    int u_v=p.usedVariablesExp().deg();
    if  (u_v<=4) {
      way = 1;
      impl = add4ImplDelayed(p, p.leadExp(), p.usedVariablesExp());
    } else {
      way=2;
      if (((optAllowRecursion) && (u_v<=15))||(u_v<=10)){
        way=3;
        LiteralFactorization f(p);
        if (f.rest.usedVariablesExp().deg()<=4){
          way = 4;
          impl = addHigherImplDelayedUsing4(f);
        } else {
          deg_type rest_lead_exp_deg=f.rest.leadExp().deg();
          deg_type rest_used_variables_deg=f.rest.usedVariablesExp().deg();
          if ((optAllowRecursion) &&(is_minimal) && 

            (
            (rest_used_variables_deg<=rest_lead_exp_deg+2)||
            ((rest_lead_exp_deg<=6)
            &&(rest_used_variables_deg<=rest_lead_exp_deg+3))||
            ((rest_lead_exp_deg<=4)
            &&(rest_used_variables_deg<=rest_lead_exp_deg+4))||
            ((rest_lead_exp_deg<=3)
            &&(rest_used_variables_deg<=rest_lead_exp_deg+5))||
            ((rest_lead_exp_deg<=2)
            &&(rest_used_variables_deg<=rest_lead_exp_deg+7))))
          {
                        //orig +2
            log("Recursive call");
            impl=full_implication_gb(f.rest,*cache,*this);
            int i;
            int s=impl.size();
            for(i=0;i<s;i++){
              impl[i]=multiply_with_literal_factors(f,impl[i]);
            }
          }
        }

      } else {

      }

    }


  } 
  if (impl.empty())
    addGenerator(p);
  else {
    std::vector<int> implication_indices;

    std::vector<Polynomial>::const_iterator start(impl.begin()),
      finish(impl.end());   
    for(; start != finish; ++start) {
      PBORI_ASSERT(!start->isZero());
      if (generators.minimalLeadingTerms.divisorsOf(start->leadExp()).isZero())
        addImplications(*start, implication_indices);
      else
        addGeneratorDelayed(*start);
    }
    PBORI_ASSERT(!(generators.leadingTerms.divisorsOf(p.leadExp()).isZero()));
  }
}

bool GroebnerStrategy::shorterElimination(const MonomialSet& divisors, wlen_type el,
                                           MonomialSet::deg_type deg) const {
  return std::find_if(divisors.expBegin(),divisors.expEnd(),
                      ShorterEliminationLengthModified(*this, el, deg))
    != divisors.expEnd();
}

void GroebnerStrategy::addAsYouWish(const Polynomial& p){
    Exponent lm_exp = p.leadExp();
    MonomialSet::deg_type deg = lm_exp.deg();
    MonomialSet divisors = generators.leadingTerms.divisorsOf(lm_exp);

    if ( (optDelayNonMinimals && !divisors.isZero()) || 
         divisors.owns(Monomial(lm_exp, p.ring())) ||
         shorterElimination(divisors, p.eliminationLength(), deg) )
      addGeneratorDelayed(p);

    else {
      Polynomial pred = 
        which(generators.optRedTail, red_tail(generators, p),
              optRedTailInLastBlock, red_tail_in_last_block(*this, p), p);
           
      if ((pred != p) && shorterElimination(divisors, pred.eliminationLength(), deg)) 
        addGeneratorDelayed(pred);
      else if (divisors.isZero())
        addGeneratorTrySplit(pred, true);
      else
        addGenerator(pred);
    }
}

int steps(int size) {
  return (size > 100? 19 : (size >10? 30: 100));
}
void GroebnerStrategy::symmGB_F2(){
    const double max_growth = 2.0;
    const int selection_size = 1000;
    const double pair_size_factor = 2.0;

    while(pairs.queue.size() > 0) {
      std::vector<Polynomial> next = 
	small_next_degree_spolys(*this, pair_size_factor, selection_size);
      std::vector<Polynomial> res = 
	parallel_reduce(next, *this, steps(next.size()), max_growth);

      for_each(res.rbegin(), std::find(res.rbegin(), res.rend(), 1),
	       *this, &self::addAsYouWish);
    }
}



int GroebnerStrategy::suggestPluginVariable() {
  RankingVector ranking(generators.minimalLeadingTerms.ring().nVariables());

  for_each(generators.minimalLeadingTerms.expBegin(),
           generators.minimalLeadingTerms.expEnd(), 
           ranking, &RankingVector::rerank );
    
  return ranking.max_index();
}


Polynomial GroebnerStrategy::nf(Polynomial p) const{
    return generators.nf(p);
}

#if  defined(PBORI_HAVE_M4RI) || defined(PBORI_HAVE_NTL)
std::vector<Polynomial> GroebnerStrategy::noroStep(const std::vector<Polynomial>& orig_system){

    if (orig_system.empty())
      return orig_system;

    log("reduction by linear algebra\n");
    static int round=0;
    round++;
    std::vector<Polynomial> polys;
    MonomialSet terms(orig_system[0].ring());
    
    for(std::size_t i=0;i<orig_system.size();i++){
        Polynomial p=orig_system[i];
        if PBORI_LIKELY(!(p.isZero())){
            p=ll_red_nf(p,generators.llReductor);
            if PBORI_LIKELY(!(p.isZero())){
                p=generators.reducedNormalForm(p);
                if PBORI_LIKELY(!(p.isZero())){
                    if (PBORI_UNLIKELY(p.isOne())){
                        polys.clear();
                        polys.push_back(p);
                        return polys;
                    }
                    terms=terms.unite(p.diagram());
                    polys.push_back(p);
                }
            }
        }
    }
    if PBORI_UNLIKELY(polys.size()==0) return std::vector<Polynomial>();
    typedef std::map<int,Exponent> to_term_map_type;
    typedef Exponent::idx_map_type from_term_map_type;
    
    int rows=polys.size();
    int cols=terms.size();
    if PBORI_UNLIKELY(this->enabledLog){
        std::cout<<"ROWS:"<<rows<<"COLUMNS:"<<cols<<std::endl;
    }
    #ifndef PBORI_HAVE_M4RI
    mat_GF2 mat(INIT_SIZE,rows,cols);
    #else
    mzd_t* mat=mzd_init(rows,cols);
    #endif
    std::vector<Exponent> terms_as_exp(terms.size());
    std::copy(terms.expBegin(),terms.expEnd(),terms_as_exp.begin());

    std::sort(terms_as_exp.begin(),terms_as_exp.end(),
	      ExpGreater(polys[0].ring()));


    from_term_map_type from_term_map;
    //to_term_map_type to_term_map;
    for (std::size_t i=0;i<terms_as_exp.size();i++){
        from_term_map[terms_as_exp[i]]=i;
        //to_term_map[i]=terms_as_exp[i]);
    }
    for(std::size_t i=0;i<polys.size();i++){
        Polynomial::exp_iterator it=polys[i].expBegin();//not order dependend
        Polynomial::exp_iterator end=polys[i].expEnd();
        while(it!=end){
            #ifndef PBORI_HAVE_M4RI
            mat[i][from_term_map[*it]]=1;
            #else
            mzd_write_bit(mat,i,from_term_map[*it],1);
            #endif
            it++;
        }
    }
    polys.clear();
    #ifndef PBORI_HAVE_M4RI

    int rank=gauss(mat);
    #else
    {        
       if PBORI_UNLIKELY(optDrawMatrices){
            std::ostringstream matname;   
            matname << matrixPrefix.data() << round << ".png";
            draw_matrix(mat, matname.str().c_str());
        }
    }
    int rank;
    if ((mat->nrows>0) && (mat->ncols>0))
            rank=
            mzd_echelonize_m4ri(mat,TRUE,0);
            //mzd_echelonize_pluq(mat,TRUE);
    else
            rank=0;
    #endif
    for(int i=rank-1;i>=0;i--){
        int j=0;
        std::vector<Exponent> p_t;
        for(;j<cols;j++){
            #ifndef PBORI_HAVE_M4RI
            if (mat[i][j]==1)
            #else
            if (mzd_read_bit(mat,i,j))
            #endif
              {
                p_t.push_back(terms_as_exp[j]);
              }
        }
        PBORI_ASSERT(polys.size()!=0);
        Polynomial from_mat = add_up_exponents(p_t, orig_system[0].ring().zero());
	polys.push_back(from_mat);
        if (PBORI_UNLIKELY(from_mat.isOne())) {
	  polys.erase(polys.begin(), polys.end() - 1);
	  break;
	}
    }
    #ifdef PBORI_HAVE_M4RI
    mzd_free(mat);
    #endif
    return polys;
}
#endif


#if  defined(PBORI_HAVE_NTL) || defined(PBORI_HAVE_M4RI)
std::vector<Polynomial>
GroebnerStrategy::faugereStepDense(const std::vector<Polynomial>& orig_system){

  if (orig_system.empty())
    return orig_system;
  
  std::vector<Polynomial> polys;
  MonomialSet terms(orig_system[0].ring()), leads_from_strat(terms);
  
  PBORI_ASSERT(same_rings(orig_system.begin(), orig_system.end()));
  fix_point_iterate(*this, orig_system, polys, terms, leads_from_strat);
  
  (optModifiedLinearAlgebra? linalg_step_modified:
   linalg_step)(polys, terms, leads_from_strat, enabledLog, optDrawMatrices,
                matrixPrefix.data());
  return polys;
}
 
#endif

END_NAMESPACE_PBORIGB
