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
  while((!(strat.pairs.pairSetEmpty())) &&(strat.pairs.queue.top().sugar<=deg) && (strat.pairs.queue.top().wlen<=wlen*f+2)&& (res.size()<n)){
    
    PBORI_ASSERT(strat.pairs.queue.top().sugar==deg);
    res.push_back(strat.nextSpoly());
    strat.pairs.cleanTopByChainCriterion();
  }
  return res;
  
}


// class members
GroebnerStrategy::GroebnerStrategy(const GroebnerStrategy& orig):
  pairs(orig.pairs),
  generators(orig.generators),
  r(orig.r), 
  cache(orig.cache),

  optDrawMatrices(orig.optDrawMatrices),
  optModifiedLinearAlgebra(orig.optModifiedLinearAlgebra), 
  optRedTailInLastBlock(orig.optRedTailInLastBlock),
  optLinearAlgebraInLastBlock(orig.optLinearAlgebraInLastBlock),
  optDelayNonMinimals(orig.optDelayNonMinimals),
  optStepBounded(orig.optStepBounded),
  optAllowRecursion(orig.optAllowRecursion),
  optHFE(orig.optHFE),
  optLazy(orig.optLazy),
  optExchange(orig.optExchange),

  reductionSteps(orig.reductionSteps),
  normalForms(orig.normalForms),
  currentDegree(orig.currentDegree),
  chainCriterions(orig.chainCriterions),
  variableChainCriterions(orig.variableChainCriterions),
  easyProductCriterions(orig.easyProductCriterions),
  extendedProductCriterions(orig.extendedProductCriterions),
  averageLength(orig.averageLength),
  enabledLog(orig.enabledLog),
  reduceByTailReduced(orig.reduceByTailReduced) {

  this->pairs.strat = this;
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

void GroebnerStrategy::propagate_step(entryset_type& others) {

  const PolyEntry& e = *(*others.begin());
  others.erase(others.begin());

  if (should_propagate(e)) {
    for(PolyEntryVector::const_iterator start(generators.begin()),
	  finish(generators.end()); start != finish; ++start)
      if (start->propagatableBy(e))
	exchange(reduce_by_small_entry(start->p, e), *start, others);
  }
}

void
GroebnerStrategy::exchange(const Polynomial& reduced, const PolyEntry& entry,
			   entryset_type& others) {

  if (reduced != entry.p) {
    update_propagated(generators(entry) = reduced);
    others.insert(&entry);
  }
}

void
GroebnerStrategy::update_propagated(const PolyEntry& entry) {

  generators.monomials.update(entry);
  if ( (entry.length == 2) && (entry.ecart() == 0))
    addNonTrivialImplicationsDelayed(entry);
}


void GroebnerStrategy::propagate(const PolyEntry& e){

  entryset_type entries;
  entries.insert(&e);

  while (!entries.empty())
    propagate_step(entries);

}

std::vector<Polynomial> GroebnerStrategy::addHigherImplDelayedUsing4(int s, const LiteralFactorization& literal_factors, bool include_orig){
    if (literal_factors.rest.isOne()){
        if(s>=0)
          generators(s).markVariablePairsCalculated();
        return std::vector<Polynomial>();
    }

    Polynomial p=literal_factors.rest;
    BoolePolyRing current_ring(p.ring());
   
    //Monomial used_variables_m=p.usedVariables();
    Exponent used_variables=p.usedVariablesExp();
    Exponent e=p.leadExp();
    if (e.size()>4) std::cerr<<"too many variables for table"<<std::endl;
    
    std::vector<char> ring_2_0123(current_ring.nVariables());
    std::vector<idx_type> back_2_ring(4);
    set_up_translation_vectors(ring_2_0123, back_2_ring, used_variables);
    unsigned int p_code=p2code_4(p, ring_2_0123);
    int i;
    if ((get_table_entry4(this->r, p_code,0)==p_code) &&
        (get_table_entry4(this->r, p_code,1)==0)){
        if (s>=0)
          generators(s).markVariablePairsCalculated();
        return std::vector<Polynomial>();
    }
    
    //Exponent e_i_high=multiply_with_literal_factor(literal_factors,Polynomial(Monomial(e_i)).lead());
    bool can_add_directly=true;
    std::vector<Polynomial> impl;
    for(i=0;get_table_entry4(this->r, p_code,i)!=0;i++){
      unsigned int impl_code=get_table_entry4(this->r, p_code,i);
        if ((include_orig) ||(p_code!=impl_code)){
          Polynomial p_i=code_2_poly_4(current_ring, impl_code, back_2_ring);
            Exponent e_i=p_i.leadExp();
            if ((include_orig)||(e_i!=e)){
                p_i=
                    multiply_with_literal_factors(
                        literal_factors,
                        p_i);
               impl.push_back(p_i);
                if ((can_add_directly) &&(!(this->generators.minimalLeadingTerms.divisorsOf(p_i.leadExp()).isZero())))
                //e_i is wrong here, have to multiply
                    can_add_directly=false;
            }
        }
    }
    
    
     if (s>=0)
       generators(s).markVariablePairsCalculated();
     
     if (can_add_directly)
       return impl;
     else if (!(include_orig))
       for_each(impl.begin(), impl.end(), *this, &self::addGeneratorDelayed);
     
     return std::vector<Polynomial>();
}

std::vector<Polynomial> GroebnerStrategy::add4ImplDelayed(const Polynomial& p, const Exponent& lm_exp, const Exponent& used_variables,int s, bool include_orig){
    //cout<<"I am here";
    //Polynomial p=generators[s].p;
    //Exponent used_variables=generatusedVariables;

    Exponent e=lm_exp;//generators[s].lmExp;
    BoolePolyRing current_ring(p.ring());

    std::vector<char> ring_2_0123(current_ring.nVariables());
    std::vector<idx_type> back_2_ring(4);
    set_up_translation_vectors(ring_2_0123, back_2_ring, used_variables);
    
    unsigned int p_code=p2code_4(p, ring_2_0123);

    if ((get_table_entry4(this->r,p_code,0)==p_code) && (get_table_entry4(this->r,p_code,1)==0)){
        if(s>=0)
          generators(s).markVariablePairsCalculated();

        return std::vector<Polynomial>();
    }
    
    int i;
    bool can_add_directly=true;
    std::vector<Polynomial> impl;
    for(i=0;get_table_entry4(this->r, p_code,i)!=0;i++){
      unsigned int impl_code=get_table_entry4(this->r,p_code,i);
        if ((include_orig) ||(p_code!=impl_code)){
          Polynomial p_i=code_2_poly_4(current_ring, impl_code, back_2_ring);
            Exponent e_i=p_i.leadExp();

            if ((include_orig) ||(e_i!=e)){
                impl.push_back(p_i);
                if ((can_add_directly)&&(!(this->generators.minimalLeadingTerms.divisorsOf(e_i).isZero())))
                    can_add_directly=false;
                //impl.push_back(p_i);
                //addGeneratorDelayed(p_i);
            }
        }
    }
    
    
    if (s >= 0)
      generators(s).markVariablePairsCalculated();
    
    if (can_add_directly)
        return impl;
    else if (!(include_orig))
      for_each(impl.begin(), impl.end(), *this, &self::addGeneratorDelayed);
    
    return std::vector<Polynomial>();    
}

void
GroebnerStrategy::addVariablePairs(int s) {
  Exponent::const_iterator it=generators[s].leadExp.begin();
  Exponent::const_iterator end=generators[s].leadExp.end();
  while(it != end){
    if ((generators[s].lead.deg() == 1) ||
        generators[s].literal_factors.occursAsLeadOfFactor(*it)) {
      generators(s).vPairCalculated.insert(*it);
    } 
    else
      pairs.introducePair(Pair(s, *it, generators, VARIABLE_PAIR));
    it++;
  } 
}

std::vector<Polynomial>
GroebnerStrategy::treatVariablePairs(int s){

  const PolyEntry& e = generators[s];
  if ((have_ordering_for_tables(this->r))||
      ((have_base_ordering_for_tables(this->r))&&
       (e.p.inSingleBlock()))) { 
    int uv=e.usedVariables.deg();
    if (uv<=4){
      return add4ImplDelayed(e.p, e.leadExp, e.usedVariables, s, false);
    } 
    else {
      int uv_opt = uv-e.literal_factors.factors.size()-2*e.literal_factors.var2var_map.size();
      ////should also be proofable for var2var factors
      PBORI_ASSERT(uv_opt==e.literal_factors.rest.nUsedVariables());//+2*var2var_map.size());
      if (uv_opt<=4){
        return addHigherImplDelayedUsing4(s, e.literal_factors, false);
      }
    }
  }


  std::vector<Polynomial> empty;
  
  Polynomial rest=e.literal_factors.rest;
  
  int rest_lm_deg=rest.leadDeg();
  Monomial rest_uv=rest.usedVariables();
  if ((rest_uv.deg() > 1) && (rest_uv.deg() < 12)){
    MonomialSet my_ones = zeros(rest + 1, rest_uv.divisors());
    if ((my_ones.size() << rest.leadDeg()) == (1 << rest_uv.deg()))
      return empty;
  }

  addVariablePairs(s);
  return empty;
}
void GroebnerStrategy::treatNormalPairs(int s, const PolyEntry& e,MonomialSet leadingTerms, MonomialSet minimal_intersecting_terms,MonomialSet other_terms, MonomialSet ext_prod_terms){
  //   PolyEntry e=generators[s];
    int i;
    
    if(!(Polynomial(other_terms).hasConstantPart()))//.divisorsOf(lm).isZero()))
     {
       BooleMonomial lm=e.lead;

  #ifndef EXP_FOR_PAIRS
          //MonomialSet already_used;
       std::vector<Monomial> mt_vec=minimal_elements_multiplied(minimal_intersecting_terms, lm);
  #else
          std::vector<Exponent> mt_vec;
	  minimal_elements_divided(minimal_intersecting_terms, lm,ext_prod_terms,mt_vec);
  #endif

          int mt_i;
          for(mt_i=mt_vec.size()-1;mt_i>=0;mt_i--){
  #ifndef EXP_FOR_PAIRS
              Monomial t=mt_vec[mt_i];
              Monomial t_divided=t/lm;
              PBORI_ASSERT(t.reducibleBy(lm));
  #else
              Exponent t_divided=mt_vec[mt_i];
              Exponent t=t_divided+e.leadExp;
  #endif
              //MonomialSet lm_d=t_divided.divisors();
              PBORI_ASSERT((other_terms.intersect(t_divided.divisors(lm.ring())).isZero()));
              if (true){
             // #ifndef EXP_FOR_PAIRS
             //     MonomialSet act_l_terms=generators.leadingTerms.intersect(t.divisors());
              //#else
                  MonomialSet act_l_terms = 
                    fixed_path_divisors(leadingTerms, Monomial(t, lm.ring()), 
                                        Monomial(t_divided, lm.ring()));
                  //generators.leadingTerms.divisorsOf(t);
                  
                  
                  //intersect(t.divisors());//divisorsOf((Monomial)t);//.intersect(t.divisors());
              //#endif
                  if (std::find_if(act_l_terms.expBegin(), act_l_terms.expEnd(),HasTRepOrExtendedProductCriterion(*this,s))!=act_l_terms.expEnd()){
                      //at this point we assume minimality of t_divided w.r.t. natural partial order
                      //chosen.unite(act_l_terms.weakDivide(lm.diagram()));
                      continue;
                  }
                  // if (std::find_if(act_l_terms.expBegin(), act_l_terms.expEnd(),CountCriterion(*this,s))!=act_l_terms.expEnd()){
                  //       continue;
                  //   }
                  //chosen=chosen.unite(t_divided.diagram());
                  #ifndef EXP_FOR_PAIRS

                  Monomial min=*(std::min_element(act_l_terms.begin(),act_l_terms.end(), LessWeightedLengthInStrat(this->generators)));
                  #else

                  Exponent min=*(std::min_element(act_l_terms.expBegin(),act_l_terms.expEnd(), LessWeightedLengthInStrat(this->generators)));
                  #endif
                  int chosen_index=generators.index(min);
                  this->pairs.introducePair(Pair(chosen_index,s,generators));
              }
              //if (t.intersect())
          }

     }
}



class PolyFromPolyEntry {
public:
  const Polynomial& operator()(const PolyEntry& entry) const {
    return entry.p;
  }
};

std::vector<Polynomial>
GroebnerStrategy::allGenerators(){
  std::vector<Polynomial> result(generators.size(), r);
  std::transform(generators.begin(), generators.end(), result.begin(),
		 PolyFromPolyEntry());
  pairs.appendHiddenGenerators(result);
  
  return result;
}


int GroebnerStrategy::addGeneratorStep(const BoolePolynomial& p_arg){

  PBORI_ASSERT(p_arg.ring().id() == r.id());
  const MonomialSet empty(p_arg.ring());
  PolyEntry e(p_arg);

  // here we make use of the fact, that the index of the 1 node is 
  // bigger than that of variables
  generators.reducibleUntil = std::max(generators.reducibleUntil,
                                       *p_arg.navigation());

  //do this before adding leading term
  propagate(e);

  MonomialSet other_terms(empty), ext_prod_terms(empty),
    critical_terms_base(empty);
  MonomialSet intersecting_terms = 
    generators.intersecting_leads(e, other_terms,  ext_prod_terms,
				  critical_terms_base);
  MonomialSet minimal_intersection = critical_terms_base.intersect(generators.minimalLeadingTerms);
  MonomialSet old_lead = generators.leadingTerms;

  checkSingletonCriterion(e, intersecting_terms);

  easyProductCriterions += generators.minimalLeadingTerms.length() -
    intersecting_terms.length();

  const int s = generators.size();

  generators.append(e);   

  //!!!!! here we add the lm !!!!
  //we assume that lm is minimal in generators.leadingTerms
  treatNormalPairs(s, e, old_lead, minimal_intersection, other_terms, ext_prod_terms);

  return s;
}

void
GroebnerStrategy::addImplications(const BoolePolynomial& poly,
				  std::vector<int>& indices){
  PBORI_ASSERT(!poly.isZero());
  int result = addGeneratorStep((generators.optRedTail?
                                 red_tail(generators, poly): poly));

  pairs.status.setToHasTRep(indices.begin(), indices.end(), result);
  indices.push_back(result);
  generators(result).markVariablePairsCalculated();
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
GroebnerStrategy::addGenerator(const BoolePolynomial& p_arg) {

  int s = addGeneratorStep(p_arg);
  PolyEntryReference entry = generators(s);

  if (entry.minimal)
    addImplications(treatVariablePairs(s), s);
  else
    entry.markVariablePairsCalculated();
}

class TimesConstantImplication {
public:
  template <class PairType>
  Polynomial operator()(const Polynomial& lhs, const PairType& rhs) const {
    return lhs * (lhs.ring().variable(rhs.first) + bool(rhs.second) );
  }
};

class TimesVariableExchange {
public:
  template <class PairType>
  Polynomial operator()(const Polynomial& lhs, const PairType& rhs) const {
    return lhs * (lhs.ring().variable(rhs.first) +
                  lhs.ring().variable(rhs.second));
  }
};

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
                              mult_by, TimesVariableExchange());

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
  PBORI_ASSERT(p.ring().id() == r.id());
  pairs.introducePair(Pair(p));
}

class VariableHasValue {
public:
  VariableHasValue(idx_type value): m_value(value) {}

  bool operator()(const PolyEntry& rhs) const {
    return (rhs.usedVariables.deg() == 1) && 
      (*(rhs.usedVariables.begin()) == m_value);
  }

private:
  idx_type m_value;
};


bool GroebnerStrategy::variableHasValue(idx_type v){
  return std::find_if(generators.begin(), generators.end(),
                      VariableHasValue(v)) != generators.end();
}

class RedTailNth {
public:
  RedTailNth(ReductionStrategy& strat): m_strat(strat) {}

  template <class KeyType>
  const Polynomial& operator()(const KeyType& key) {
    return get(m_strat(key)).p;
  }

private:
  const PolyEntry& get(PolyEntryReference entry) {
    return entry = red_tail(m_strat, entry.get().p);
  }

  ReductionStrategy& m_strat;
};

class GetNthPoly {
public:
  GetNthPoly(ReductionStrategy& strat): m_strat(strat) {}

  template <class KeyType>
  const Polynomial& operator()(const KeyType& key) {
    return m_strat[key].p;
  }

  ReductionStrategy& m_strat;
};


std::vector<Polynomial>
GroebnerStrategy::minimalizeAndTailReduce(){

    bool tail_growth_bak = generators.optRedTailDegGrowth;
    generators.optRedTailDegGrowth = true;

    MonomialSet minelts = minimal_elements(generators.minimalLeadingTerms);
    std::vector<Polynomial> result(minelts.size(), r);
    std::transform(minelts.rExpBegin(), minelts.rExpEnd(), result.rbegin(), 
                   RedTailNth(generators));

    generators.optRedTailDegGrowth = tail_growth_bak;
    return result;
}

std::vector<Polynomial>
GroebnerStrategy::minimalize(){

    MonomialSet minelts = minimal_elements(generators.minimalLeadingTerms);
    std::vector<Polynomial> result(minelts.size(), r);
    std::transform(minelts.begin(), minelts.end(), result.begin(), 
                   GetNthPoly(generators));

    return result;
    
}
void GroebnerStrategy::addGeneratorTrySplit(const Polynomial & p, bool is_minimal){

  PBORI_ASSERT(p.ring().id() == this->r.id());
  std::vector<Polynomial> impl;
  int way=0;
  if ((have_ordering_for_tables(this->r)) ||
      ((have_base_ordering_for_tables(this->r)) &&
       (p.inSingleBlock()))) { 

    int u_v=p.usedVariablesExp().deg();
    if  (u_v<=4) {
      way=1;
      impl=add4ImplDelayed(p,p.leadExp(),p.usedVariablesExp(),-1,true);
    } else {
      way=2;
      if (((optAllowRecursion) && (u_v<=15))||(u_v<=10)){
        way=3;
        LiteralFactorization f(p);
        if (f.rest.usedVariablesExp().deg()<=4){
          way=4;
          impl=addHigherImplDelayedUsing4(-1,f,true);
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

void GroebnerStrategy::addAsYouWish(const Polynomial& p){
    //if (p.isZero()) return;
    Exponent lm_exp=p.leadExp();
    MonomialSet divisors=this->generators.leadingTerms.divisorsOf(lm_exp);
    #if 0
    if (divisors.isZero())
        addGenerator(p);
    else addGeneratorDelayed(p);
    #else
    if ((optDelayNonMinimals) && (!(divisors.isZero()))){
      addGeneratorDelayed(p);
      return;
    }else {
      if (divisors.owns(Monomial(lm_exp, p.ring()))){
          addGeneratorDelayed(p);
          return;
    }}
    wlen_type el=p.eliminationLength();
    if (std::find_if(
            divisors.expBegin(),
            divisors.expEnd(),
            ShorterEliminationLengthModified(*this,el,lm_exp.deg()))!=divisors.expEnd()){
        this->addGeneratorDelayed(p);
    } else {
      Polynomial pr(p.ring());
        if (generators.optRedTail)
            pr=red_tail(this->generators,p);
        else 
            {   if (optRedTailInLastBlock)
                    pr=red_tail_in_last_block(*this,p);
                else
                    pr=p;
            }
        if (pr!=p){
            el=pr.eliminationLength();
            if (std::find_if(
                    divisors.expBegin(),
                    divisors.expEnd(),
                    ShorterEliminationLengthModified(*this,el,lm_exp.deg()))!=divisors.expEnd()){
                this->addGeneratorDelayed(pr);
            } else {
                if (divisors.isZero())
                    this->addGeneratorTrySplit(pr, true);
                else
                    addGenerator(pr);
            }
        } else
            if (divisors.isZero())
                this->addGeneratorTrySplit(p, true);
            else
                addGenerator(p);
    }
    #endif
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

class RankingVector:
  public std::vector<int> {
  typedef std::vector<value_type> base; 
public:
  RankingVector(size_type len): base(len, 0) {}


  void increment(size_type idx) {
    ++operator[](idx);
  }

  void rerank(const Exponent& exp) {
    if (exp.deg() >= 2)
      for_each(exp.begin(), exp.end(), *this, &RankingVector::increment);
  }

  value_type max_index() const {
    return (empty()? -1: std::max_element(begin(), end()) - begin());
  }

};


int GroebnerStrategy::suggestPluginVariable() {
  RankingVector ranking(this->r.nVariables());

  for_each(generators.minimalLeadingTerms.expBegin(),
           generators.minimalLeadingTerms.expEnd(), 
           ranking, &RankingVector::rerank );
    
  return ranking.max_index();
}


Polynomial GroebnerStrategy::nf(Polynomial p) const{
    return generators.nf(p);
}

#if  defined(HAVE_M4RI) || defined(HAVE_NTL)
std::vector<Polynomial> GroebnerStrategy::noroStep(const std::vector<Polynomial>& orig_system){

    if (orig_system.empty())
      return orig_system;

    log("reduction by linear algebra\n");
    static int round=0;
    round++;
    std::vector<Polynomial> polys;
    int i;
    MonomialSet terms(orig_system[0].ring());
    
    for(i=0;i<orig_system.size();i++){
        Polynomial p=orig_system[i];
        if LIKELY(!(p.isZero())){
            p=ll_red_nf(p,generators.llReductor);
            if LIKELY(!(p.isZero())){
                p=generators.reducedNormalForm(p);
                if LIKELY(!(p.isZero())){
                    if (UNLIKELY(p.isOne())){
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
    if UNLIKELY(polys.size()==0) return std::vector<Polynomial>();
    typedef std::map<int,Exponent> to_term_map_type;
    typedef Exponent::idx_map_type from_term_map_type;
    
    int rows=polys.size();
    int cols=terms.size();
    if UNLIKELY(this->enabledLog){
        std::cout<<"ROWS:"<<rows<<"COLUMNS:"<<cols<<std::endl;
    }
    #ifndef HAVE_M4RI
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
    for (i=0;i<terms_as_exp.size();i++){
        from_term_map[terms_as_exp[i]]=i;
        //to_term_map[i]=terms_as_exp[i]);
    }
    for(i=0;i<polys.size();i++){
        Polynomial::exp_iterator it=polys[i].expBegin();//not order dependend
        Polynomial::exp_iterator end=polys[i].expEnd();
        while(it!=end){
            #ifndef HAVE_M4RI
            mat[i][from_term_map[*it]]=1;
            #else
            mzd_write_bit(mat,i,from_term_map[*it],1);
            #endif
            it++;
        }
    }
    polys.clear();
    #ifndef HAVE_M4RI

    int rank=gauss(mat);
    #else
    {        
       if UNLIKELY(optDrawMatrices){
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
    for(i=rank-1;i>=0;i--){
        int j;
        std::vector<Exponent> p_t;
        for(j=0;j<cols;j++){
            #ifndef HAVE_M4RI
            if (mat[i][j]==1)
            #else
            if (mzd_read_bit(mat,i,j))
            #endif
              {
                p_t.push_back(terms_as_exp[j]);
              }
        }
        PBORI_ASSERT(polys.size()!=0);
        Polynomial from_mat = add_up_exponents(p_t, r.zero());
	polys.push_back(from_mat);
        if (UNLIKELY(from_mat.isOne())) {
	  polys.erase(polys.begin(), polys.end() - 1);
	  break;
	}
    }
    #ifdef HAVE_M4RI
    mzd_free(mat);
    #endif
    return polys;
}
#endif


#if  defined(HAVE_NTL) || defined(HAVE_M4RI)
std::vector<Polynomial>
GroebnerStrategy::faugereStepDense(const std::vector<Polynomial>& orig_system){

  if (orig_system.empty())
    return orig_system;
  
  std::vector<Polynomial> polys;
  MonomialSet terms(orig_system[0].ring()), leads_from_strat(terms);
  
  PBORI_ASSERT(sameRing(orig_system.begin(), orig_system.end()));
  fix_point_iterate(*this, orig_system, polys, terms, leads_from_strat);
  
  (optModifiedLinearAlgebra? linalg_step_modified:
   linalg_step)(polys, terms, leads_from_strat, enabledLog, optDrawMatrices,
                matrixPrefix.data());
  return polys;
}
 
#endif

END_NAMESPACE_PBORIGB
