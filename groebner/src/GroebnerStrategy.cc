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

#include <polybori/groebner/GroebnerStrategy.h>



BEGIN_NAMESPACE_PBORIGB


bool polynomial_in_one_block(const Polynomial p){
    if (p.isConstant()) return true;
    Monomial vars=p.usedVariables();
    
    return p.ring().ordering().lieInSameBlock(*vars.begin(),*std::max_element(vars.begin(),vars.end()));
}




#ifndef DANGEROUS_FIXED_PATH
  typedef PBORI::CacheManager<CCacheTypes::divisorsof_fixedpath>
#else
  typedef PBORI::CacheManager<CCacheTypes::divisorsof>
#endif
  fixed_divisors_cache_type;

// Variant navigator
static MonomialSet
do_fixed_path_divisors(const fixed_divisors_cache_type & cache_mgr, 
                       MonomialSet::navigator a_nav,
                       MonomialSet::navigator m_nav,
                       MonomialSet::navigator n_nav){

  if (n_nav.isTerminated()) return 
    MonomialSet(cache_mgr.generate(a_nav)).firstDivisorsOf(cache_mgr.generate(m_nav));
  assert(!(n_nav.isConstant()&&(!(n_nav.terminalValue()))));

  if (a_nav.isConstant()) return cache_mgr.generate(a_nav);

  assert(!(n_nav.isConstant()));
  assert(!(m_nav.isConstant()));
  int m_index=*m_nav;
  int n_index=*n_nav;
  int a_index=*a_nav;
   
  assert(m_index<=n_index);
  
  
  //here we rely on the fact, that in Cudd deref of constant nav. gives a bigger index than allow allowed real indices
  while((a_index=*a_nav)!=(m_index=*m_nav)){
     if (a_index<m_index) a_nav.incrementElse();
     else{
       n_index=*n_nav;
       assert(n_index>=m_index);
       if (m_index==n_index){
         n_nav.incrementThen();
         m_nav.incrementThen();
       } else {
         m_nav.incrementThen();
       }
     }
   
  }
  n_index=*n_nav;
 
  if (a_nav.isConstant()){
    return cache_mgr.generate(a_nav);
  }
  assert((*a_nav)==(*m_nav));

  MonomialSet::navigator cached;
  #ifndef DANGEROUS_FIXED_PATH
  cached =
    cache_mgr.find(a_nav, m_nav,n_nav);
    if (cached.isValid() ){
      return  cache_mgr.generate(cached);
    }
  #else
  //MonomialSet::navigator cached =
    //cache_mgr.find(a_nav, m_nav);
  #endif

    
  // here it is theoretically possible to get answers which don't contain the
  // fixed path n, but that doesn't matter in practice, 
  // as it is optimization anyway
  typedef PBORI::CacheManager<CCacheTypes::divisorsof>
    cache_mgr_type2;

  cache_mgr_type2 cache_mgr2(cache_mgr.manager());
  
  cached =
    cache_mgr2.find(a_nav, m_nav);
  
  if (cached.isValid()){
    return cache_mgr2.generate(cached);
  }
    
  assert(a_index==m_index);
  int index=m_index;
  MonomialSet result(cache_mgr.zero());
  if (m_index==n_index){
    result=do_fixed_path_divisors(cache_mgr, a_nav.thenBranch(), 
                                  m_nav.thenBranch(), n_nav.thenBranch());
    if (!(result.isZero())) 
      result = MonomialSet(index, result, cache_mgr.zero());
  } else {
    MonomialSet
      then_path=do_fixed_path_divisors(cache_mgr, a_nav.thenBranch(),
                                       m_nav.thenBranch(), n_nav);
    MonomialSet
      else_path=do_fixed_path_divisors(cache_mgr, a_nav.elseBranch(),
                                       m_nav.thenBranch(), n_nav);
    if (then_path.isZero()){
      result=else_path;
    } else {
      result=MonomialSet(index,then_path,else_path);
    }
  }
#ifndef DANGEROUS_FIXED_PATH
  cache_mgr.insert(a_nav,m_nav,n_nav,result.navigation());
#else
 cache_mgr2.insert(a_nav,m_nav,result.navigation());
#endif
  return result;
}
// end of variant for navigator

// variant for MonomialSet
static MonomialSet do_fixed_path_divisors(MonomialSet a, 
                                          MonomialSet m, MonomialSet n){

  //we assume that m is a multiple of n
  MonomialSet::navigator m_nav=m.navigation();
  MonomialSet::navigator n_nav=n.navigation();

  MonomialSet::navigator a_nav=a.navigation();

  typedef fixed_divisors_cache_type cache_mgr_type;
  cache_mgr_type cache_mgr(a.ring());

  return do_fixed_path_divisors(cache_mgr, a_nav, m_nav, n_nav);
}


static MonomialSet fixed_path_divisors(MonomialSet a, Monomial m, Monomial n){
   assert(m.reducibleBy(n));
   return do_fixed_path_divisors(a,m.diagram(),n.diagram());
}


static Polynomial opposite_logic_mapping(Polynomial p){
    return map_every_x_to_x_plus_one(p)+1;
}


// The following should also become memeber functions
static void mark_all_variable_pairs_as_calculated(GroebnerStrategy& strat, int s){
    BooleExponent::const_iterator it=strat.generators[s].leadExp.begin();
    BooleExponent::const_iterator end=strat.generators[s].leadExp.end();
     while(it!=end){
          strat.generators[s].vPairCalculated.insert(*it);
          it++;
    } 
}

Polynomial red_tail_in_last_block(const GroebnerStrategy& strat, Polynomial p){
    Polynomial::navigator nav=p.navigation();
    idx_type last=p.ring().ordering().lastBlockStart();
    if ((*nav)>=last) //includes constant polynomials
        return p;
    while ((*nav)<last){
        nav.incrementElse();
    }
    if (nav.isConstant()){
        //we don't check for strat containing one at this point
        return p;
    }
    Polynomial l1(nav, p.ring());
    Polynomial l2=strat.nf(l1);
    if (!(l2.isZero())) l2=red_tail(strat.generators,l2);
    return p+(l1+l2);
}

static std::vector<Polynomial> small_next_degree_spolys(GroebnerStrategy& strat, double f, int n){
  std::vector<Polynomial> res;
  assert(!(strat.pairs.pairSetEmpty()));
  strat.pairs.cleanTopByChainCriterion();
  deg_type deg=strat.pairs.queue.top().sugar;
  wlen_type wlen=strat.pairs.queue.top().wlen;
  while((!(strat.pairs.pairSetEmpty())) &&(strat.pairs.queue.top().sugar<=deg) && (strat.pairs.queue.top().wlen<=wlen*f+2)&& (res.size()<n)){
    
    assert(strat.pairs.queue.top().sugar==deg);
    res.push_back(strat.nextSpoly());
    strat.pairs.cleanTopByChainCriterion();
  }
  return res;
  
}





// class members


GroebnerStrategy::GroebnerStrategy(const GroebnerStrategy& orig)
:pairs(orig.pairs),
generators(orig.generators),
r(orig.r)


{
 
  optDrawMatrices=orig.optDrawMatrices;
  optModifiedLinearAlgebra=orig.optModifiedLinearAlgebra; 
  optRedTailInLastBlock=orig.optRedTailInLastBlock;
  optLinearAlgebraInLastBlock=orig.optLinearAlgebraInLastBlock;
  optDelayNonMinimals=orig.optDelayNonMinimals;
 
  cache=orig.cache;
  optStepBounded=orig.optStepBounded;

  optAllowRecursion=orig.optAllowRecursion;
  optHFE=orig.optHFE;
  
  optLazy=orig.optLazy;
 
  optExchange=orig.optExchange;
  reductionSteps=orig.reductionSteps;
  normalForms=orig.normalForms;
  currentDegree=orig.currentDegree;
  chainCriterions=orig.chainCriterions;
  variableChainCriterions=orig.variableChainCriterions;
  easyProductCriterions=orig.easyProductCriterions;
  extendedProductCriterions=orig.extendedProductCriterions;
  averageLength=orig.averageLength;
  enabledLog=orig.enabledLog;
  reduceByTailReduced=orig.reduceByTailReduced;
 
  this->pairs.strat=this;
 
}


void GroebnerStrategy::treat_m_p_1_case(const PolyEntry& e){
    if (e.length==2){
        assert(e.p.length()==2);
        Polynomial::const_iterator it=e.p.begin();
        assert(it!=e.p.end());
        it++;
        assert(it!=e.p.end());
        if (it->deg()==0){
            generators.monomials_plus_one=generators.monomials_plus_one.unite(e.lead.diagram());
        }
    }
}
void GroebnerStrategy::llReduceAll(){
    int i;
    Exponent ll_e=*(generators.llReductor.expBegin());
    for(i=0;i<generators.size();i++){
        if ((generators[i].minimal)&&(ll_e.GCD(generators[i].tailVariables).deg()>0)){
            Polynomial tail=generators[i].tail;
            tail=ll_red_nf(tail,generators.llReductor);
            if (tail!=generators[i].tail){
                generators[i].p=tail+generators[i].lead;
                generators[i].recomputeInformation();
                if (generators[i].length==1) generators.monomials=generators.monomials.unite(generators[i].p.diagram());
                //treat_m_p_1_case(generators[i]);

            }
        }
    }
}
Polynomial GroebnerStrategy::redTail(const Polynomial & p){
    return red_tail(this->generators,p);
}
void GroebnerStrategy::propagate_step(const PolyEntry& e, std::set<int> others){
  
  if (should_propagate(e)){
    Monomial lm=e.lead;
    Exponent exp=e.leadExp;
    int i;
    int s=generators.size();
    for(i=0;i<s;i++){      
      if ((this->generators[i].minimal) && (this->generators[i].deg<=2)&& (this->generators[i].length>1) &&(&this->generators[i]!=&e)&&(this->generators[i].tailVariables.reducibleBy(exp))){
      //if ((this->generators[i].minimal) &&(this->generators[i].length>1) &&(&this->generators[i]!=&e)&&(this->generators[i].tailVariables.reducibleBy(exp))){
        Polynomial new_p(lm.ring());
        if (e.length==1){
          new_p=cancel_monomial_in_tail(this->generators[i].p,e.lead);
        } else {
          assert(e.length==2);
          new_p=reduce_by_binom_in_tail(this->generators[i].p,e.p);
        }
        if (generators[i].p!=new_p){
          generators[i].p=new_p;
          generators[i].recomputeInformation();
          if (generators[i].length==1) generators.monomials=generators.monomials.unite(new_p.diagram());
          if ((generators[i].length==2)&&(generators[i].ecart()==0)){
            addNonTrivialImplicationsDelayed(generators[i]);
            //treat_m_p_1_case(generators[i]);

          }
          others.insert(i);
          
        }
      }
    }
  }
  if (!(others.empty()))
  { ///@todo: should take the one with smallest lm
    std::set<int>::iterator ob=others.begin();
    int next=*ob;
    others.erase(ob);
    this->propagate_step(generators[next],others);
  }
}
void GroebnerStrategy::propagate(const PolyEntry& e){
  if (should_propagate(e)){
    propagate_step(e, std::set<int>());
  }
}
std::vector<Polynomial> GroebnerStrategy::addHigherImplDelayedUsing4(int s, const LiteralFactorization& literal_factors, bool include_orig){
    if (literal_factors.rest.isOne()){
        if(s>=0)
            mark_all_variable_pairs_as_calculated(*this, s);
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
            mark_all_variable_pairs_as_calculated(*this, s);
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
         mark_all_variable_pairs_as_calculated(*this, s);
     
      if (can_add_directly){
        return impl;
    } else {
        if (!(include_orig)){
            std::vector<Polynomial>::iterator it=impl.begin();
            std::vector<Polynomial>::iterator end=impl.end();
        
            while(it!=end){
                addGeneratorDelayed(*it);
                it++;

            }
        }
        return std::vector<Polynomial>();
    }
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
            mark_all_variable_pairs_as_calculated(*this, s);

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
    
    
     if (s>=0)
         mark_all_variable_pairs_as_calculated(*this, s);
    
    if (can_add_directly){
        return impl;
    } else {
        std::vector<Polynomial>::iterator it=impl.begin();
        std::vector<Polynomial>::iterator end=impl.end();
        if (!(include_orig)){
            while(it!=end){
             addGeneratorDelayed(*it);
             it++;
            
            }
        }
        return std::vector<Polynomial>();
    }
    
    
}
void GroebnerStrategy::addVariablePairs(int s){
     Exponent::const_iterator it=generators[s].leadExp.begin();
     Exponent::const_iterator end=generators[s].leadExp.end();
     while(it!=end){
         if ((generators[s].lead.deg()==1) ||
            generators[s].literal_factors.occursAsLeadOfFactor(*it))
         {
              //((MonomialSet(p).subset0(*it).isZero())||(MonomialSet(p).subset0(*it)==(MonomialSet(p).subset1(*it))))){
      //cout<<"factorcrit"<<endl;
             generators[s].vPairCalculated.insert(*it);
          } else
            this->pairs.introducePair(Pair(s,*it,generators,VARIABLE_PAIR));
          it++;
    
    } 
}
std::vector<Polynomial> GroebnerStrategy::treatVariablePairs(int s){
  PolyEntry& e=generators[s];
  if ((have_ordering_for_tables(this->r))||
      ((have_base_ordering_for_tables(this->r))&&
       (polynomial_in_one_block(generators[s].p)))) { 
    int uv=e.usedVariables.deg();
    if (uv<=4){
      return add4ImplDelayed(e.p,e.leadExp,e.usedVariables,s,false);
    } else {

      int uv_opt=uv-e.literal_factors.factors.size()-2*e.literal_factors.var2var_map.size();
      ////should also be proofable for var2var factors
      assert(uv_opt==e.literal_factors.rest.nUsedVariables());//+2*var2var_map.size());
      if (uv_opt<=4){
        return addHigherImplDelayedUsing4(s, e.literal_factors,false);
      }
    }
  }


  std::vector<Polynomial> empty;
  
  Polynomial rest=e.literal_factors.rest;
  
  int rest_lm_deg=rest.leadDeg();
  Monomial rest_uv=rest.usedVariables();
  if ((rest_uv.deg()>1) && (rest_uv.deg()<12)){
      MonomialSet my_ones=zeros(rest+1, rest_uv.divisors());
      if ((my_ones.size()<<rest.leadDeg())==(1<<rest_uv.deg()))
          return empty;
  }

  addVariablePairs(s);
  
  

  
  
  return empty;
}
void GroebnerStrategy::treatNormalPairs(int s,MonomialSet intersecting_terms,MonomialSet other_terms, MonomialSet ext_prod_terms){
    PolyEntry e=generators[s];
    int i;
    
    if(!(Polynomial(other_terms).hasConstantPart()))//.divisorsOf(lm).isZero()))
     {
       BooleMonomial lm=e.lead;

  #ifndef EXP_FOR_PAIRS
          //MonomialSet already_used;
          std::vector<Monomial> mt_vec=minimal_elements_multiplied(intersecting_terms.intersect(generators.minimalLeadingTerms), lm);
  #else
          std::vector<Exponent> mt_vec;
	  minimal_elements_divided(intersecting_terms.intersect(generators.minimalLeadingTerms), lm,ext_prod_terms,mt_vec);
  #endif

          int mt_i;
          for(mt_i=mt_vec.size()-1;mt_i>=0;mt_i--){
  #ifndef EXP_FOR_PAIRS
              Monomial t=mt_vec[mt_i];
              Monomial t_divided=t/lm;
              assert(t.reducibleBy(lm));
  #else
              Exponent t_divided=mt_vec[mt_i];
              Exponent t=t_divided+e.leadExp;
  #endif
              //MonomialSet lm_d=t_divided.divisors();
              assert((other_terms.intersect(t_divided.divisors(lm.ring())).isZero()));
              if (true){
             // #ifndef EXP_FOR_PAIRS
             //     MonomialSet act_l_terms=generators.leadingTerms.intersect(t.divisors());
              //#else
                  MonomialSet act_l_terms = 
                    fixed_path_divisors(generators.leadingTerms, Monomial(t, lm.ring()), 
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
                  int chosen_index=generators.exp2Index[min];
                  this->pairs.introducePair(Pair(chosen_index,s,generators));
              }
              //if (t.intersect())
          }

     }
}
std::vector<Polynomial> GroebnerStrategy::allGenerators(){
     int i;
     std::vector<Polynomial> result;
     for (i=0;i<generators.size();i++){
         result.push_back(generators[i].p);
     }
     pairs.appendHiddenGenerators(result);
     return result;
}
int GroebnerStrategy::addGenerator(const BoolePolynomial& p_arg, bool is_impl,std::vector<int>* impl_v){

  Polynomial p=p_arg;
  Polynomial::ring_type ring(p_arg.ring());
  assert(ring.id() == this->r.id());
  MonomialSet ext_prod_terms(ring);
  PolyEntry e(p);
  Monomial lm=e.lead;

  Polynomial lm_as_poly=lm;
  idx_type idx_from_navigation=*lm_as_poly.navigation();
  if (generators.reducibleUntil<idx_from_navigation)
     generators.reducibleUntil=idx_from_navigation;
  //here we make use of the fact, that the index of the 1 node is bigger than that of variables
  this->propagate(e);
  //do this before adding leading term
  Monomial::const_iterator it=e.lead.begin();
  Monomial::const_iterator end=e.lead.end();
  BooleSet other_terms=this->generators.leadingTerms;
  MonomialSet intersecting_terms(ring);
  bool is00=e.literal_factors.is00Factorization();
  bool is11=e.literal_factors.is11Factorization();
  MonomialSet critical_terms_base(ring);
  MonomialSet ot2(ring);
  if (!((is00 && (generators.leadingTerms==generators.leadingTerms00))||(is11 && (generators.leadingTerms==generators.leadingTerms11)))){
    
    if (is11)
        other_terms=other_terms.diff(generators.leadingTerms11);
    if (is00)
        other_terms=other_terms.diff(generators.leadingTerms00);
    while(it!=end){
    
    other_terms=other_terms.subset0(*it);
    it++;
    
    }
    //other_terms=((Polynomial)other_terms%(Polynomial) e.lm).diagram();
    
    
    if ((is11)||(is00)){
        
        if (is00)
            ot2=generators.leadingTerms00;
        else
            ot2=generators.leadingTerms11;
          /// deactivated existAbstract, because sigfaults on SatTestCase, AD
          
        if (!(ot2.isZero())){
            ext_prod_terms=ot2.existAbstract(lm).diff(other_terms);
            other_terms=other_terms.unite(ext_prod_terms);
        }
       
        //other_terms=other_terms.unite(ot2);
    }
    //we assume that no variable of lm does divide a monomial in other_terms
    
    //diff suffices if we start from minimal leads
    intersecting_terms=this->generators.leadingTerms.diff(other_terms);
    intersecting_terms=intersecting_terms.diff(ot2);//mod_mon_set(intersecting_terms,ot2);
    assert (!((!(p.isOne())) && is00 && is11));
  }
          
  critical_terms_base=mod_mon_set(intersecting_terms.intersect(generators.minimalLeadingTerms),ot2);
  this->easyProductCriterions+=this->generators.minimalLeadingTerms.length()-intersecting_terms.length();
  
 
  
  generators.push_back(e);
  pairs.status.prolong(PairStatusSet::HAS_T_REP);
  const int s=generators.size()-1;
  
  
  int i;

  
  Polynomial inter_as_poly=intersecting_terms;
  
  Polynomial::exp_iterator is_it=inter_as_poly.expBegin();
  Polynomial::exp_iterator is_end=inter_as_poly.expEnd();
  while(is_it!=is_end){
    int index =this->generators.exp2Index[*is_it];
    if (index!=s){
      
      //product criterion doesn't hold
      //try length 1 crit
      if (!((generators[index].length==1) &&(generators[s].length==1)))
      {        
        this->pairs.status.setToUncalculated(index,s);
      } else this->extendedProductCriterions++;
    }
    is_it++;
  }
  if (impl_v!=NULL){
    assert(is_impl);
    int impl_v_size=impl_v->size();
    int j;
    for (j=0;j<impl_v_size;j++){
        this->pairs.status.setToHasTRep((*impl_v)[j],s);
    }
  }
  //Monomial crit_vars=Polynomial(intersecting_terms).usedVariables();
  
  
   treatNormalPairs(s,critical_terms_base,other_terms, ext_prod_terms);
    //!!!!! here we add the lm !!!!
    //we assume that lm is minimal in generators.leadingTerms
    
    
    
    
    std::vector<Polynomial> impl;
    
    generators.setupSetsForLastElement();
      if ((generators[s].minimal)&&(!(is_impl))){

      impl=treatVariablePairs(s);
    }
      else {
        mark_all_variable_pairs_as_calculated(*this, s);
      }

    
        int impl_s=impl.size();
        std::vector<int> implication_indices;
        implication_indices.push_back(s);
        for(i=0;i<impl_s;i++){
            Polynomial implication=impl[i];
            if (generators.optRedTail)
                implication=red_tail(this->generators,implication);

            implication_indices.push_back(
                addGenerator(implication,true,&implication_indices));
            //addGeneratorDelayed(implication);
        }
    
    
    return s;
}
void GroebnerStrategy::addNonTrivialImplicationsDelayed(const PolyEntry& e){
  Polynomial p_opp=opposite_logic_mapping(e.p);
  //cout<<"p_opp"<<p_opp<<endl;
  const Polynomial::ring_type ring(p_opp.ring());
  const Polynomial one_element(ring.one());
  Polynomial mult_by(one_element);
  LiteralFactorization factors_opp(p_opp);
  
  if (factors_opp.trivial()){
    if (e.literal_factors.trivial()) return;
    if ( !e.literal_factors.rest.isOne() ){
      
    
      //if (e.literal_factors.trivial())
      mult_by=one_element;
      LiteralFactorization::map_type::const_iterator itf=e.literal_factors.factors.begin();
      LiteralFactorization::map_type::const_iterator endf=e.literal_factors.factors.end();
      while(itf!=endf){
        idx_type var=itf->first;
        int val=itf->second;
        if (val==0){
          mult_by*=(Monomial)Variable(var, ring);
        } else mult_by*=Variable(var, ring)+one_element;
        itf++;
      }

      LiteralFactorization::var2var_map_type::const_iterator itv=e.literal_factors.var2var_map.begin();
      LiteralFactorization::var2var_map_type::const_iterator endv=e.literal_factors.var2var_map.end();
      while(itv!=endv){
        idx_type var=itv->first;
        idx_type var2=itv->second;

          mult_by*=Variable(var, ring)+Variable(var2, ring);
        itv++;

      }
      p_opp=opposite_logic_mapping(e.literal_factors.rest);
      factors_opp=LiteralFactorization(p_opp);
      if (factors_opp.trivial()) return;
    } else return;
    
  }
  
  
  
   {
    this->log("found new implications");
    //cout<<"implications"<<endl;
    if (!(factors_opp.rest.isOne())){
      addGeneratorDelayed(opposite_logic_mapping(factors_opp.rest));
    }
    LiteralFactorization::map_type::const_iterator itf=factors_opp.factors.begin();
    LiteralFactorization::map_type::const_iterator endf=factors_opp.factors.end();
    while(itf!=endf){
      //cout<<"type1"<<endl;
      
      idx_type var=itf->first;
      int val=itf->second;
      if (val==0){
        addGeneratorDelayed(((Monomial) Variable(var, ring))*mult_by);
      } else addGeneratorDelayed(((Monomial) Variable(var, ring)+one_element)*mult_by);
      itf++;
    }
    
    LiteralFactorization::var2var_map_type::const_iterator itv=factors_opp.var2var_map.begin();
    LiteralFactorization::var2var_map_type::const_iterator endv=factors_opp.var2var_map.end();
    while(itv!=endv){
      //cout<<"type2"<<endl;
      idx_type var=itv->first;
      idx_type var2=itv->second;
      
      addGeneratorDelayed(mult_by*(Variable(var, ring)+Variable(var2, ring)+ 1));
     itv++; 
      
    }
  }
}
void GroebnerStrategy::addGeneratorDelayed(const BoolePolynomial& p){
#ifndef NDEBUG
  if (p.ring().id() != this->r.id())
      throw std::runtime_error("addGeneratorDelayed failed");
#endif
  assert(p.ring().id() == this->r.id());
  this->pairs.introducePair(Pair(p));
}
bool GroebnerStrategy::variableHasValue(idx_type v){
  int i;
  int s=this->generators.size();
  //Monomial m=Variable(v);
  for(i=0;i<s;i++){
    if (this->generators[i].usedVariables.deg()==1){
      if ((*(this->generators[i].usedVariables.begin()))==v){
        return true;
      }
    }
  }
  return false;
}
std::vector<Polynomial> GroebnerStrategy::minimalizeAndTailReduce(){
    MonomialSet m=minimal_elements(this->generators.minimalLeadingTerms);
    std::vector<Polynomial> result;
    bool tail_growth_bak=generators.optRedTailDegGrowth;
    generators.optRedTailDegGrowth=true;
    std::vector<Exponent> m_vec;
    m_vec.resize(m.length());
    std::copy(m.expBegin(),m.expEnd(),m_vec.begin());
    int i=m_vec.size()-1;
    while(i>=0){
        //redTail
        int index=generators.exp2Index[m_vec[i]];
        Polynomial reduced=red_tail(this->generators,generators[index].p);
        generators[index].p=reduced;
        generators[index].recomputeInformation();
        //if (generators[index].length==1) strat.monomials=strat.monomials.unite(reduced.diagram());
        result.push_back(reduced);
        i--;
    }
    generators.optRedTailDegGrowth=tail_growth_bak;
    std::vector<Polynomial> result_r(result.size(), result[0].ring());
    std::copy(result.rbegin(),result.rend(),result_r.begin());
    return result_r;
    
}
std::vector<Polynomial> GroebnerStrategy::minimalize(){
    MonomialSet m=minimal_elements(this->generators.minimalLeadingTerms);
    std::vector<Polynomial> result;
    MonomialSet::const_iterator it=m.begin();
    MonomialSet::const_iterator end=m.end();
    while(it!=end){
        //redTail
        result.push_back(generators[generators.lm2Index[*it]].p);
        it++;
    }
    return result;
    
}
void GroebnerStrategy::addGeneratorTrySplit(const Polynomial & p, bool is_minimal){

  assert(p.ring().id() == this->r.id());
  std::vector<Polynomial> impl;
  int way=0;
  if ((have_ordering_for_tables(this->r)) ||
      ((have_base_ordering_for_tables(this->r)) &&
       (polynomial_in_one_block(p)))) { 

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
  if (impl.size()==0)
    addGenerator(p);
  else{
    int s=impl.size();
    int i;
    std::vector<int> implication_indices;
    for(i=0;i<s;i++){
      assert(!(impl[i].isZero()));
      if (generators.minimalLeadingTerms.divisorsOf(impl[i].leadExp()).isZero()){

        
        Polynomial p_impl=impl[i];
        if(generators.optRedTail) p_impl=red_tail(this->generators,p_impl);
        assert(!(p_impl.isZero()));
        implication_indices.push_back(
          addGenerator(p_impl,true,&implication_indices));
      }
      else {
        addGeneratorDelayed(impl[i]);
                //assert(impl[i]!=)
      }

    }
    assert(!(generators.leadingTerms.divisorsOf(p.leadExp()).isZero()));
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
void GroebnerStrategy::symmGB_F2(){
    const double max_growth=2.0;
    const int selection_size=1000;
    const double pair_size_factor=2.0;
    while(pairs.queue.size()>0){
        std::vector<Polynomial> next=small_next_degree_spolys(*this,pair_size_factor,selection_size);
        std::vector<Polynomial> res;
        if (next.size()>100)
           res=parallel_reduce(next,*this,10,max_growth);
        else{
           if (next.size()>10)
              res=parallel_reduce(next,*this,30,max_growth);
           else
              res=parallel_reduce(next,*this,100, max_growth);}
        int s=res.size();
        int i;
        for(i=s-1;i>=0;i--){
            Polynomial p=res[i];
            addAsYouWish(res[i]);
            if (p.isOne())
                return;
        }
        
    }
}
int GroebnerStrategy::suggestPluginVariable(){
    std::vector<int> ranking(this->r.nVariables());
    int s=ranking.size();
    int i;
    for(i=0;i<s;i++){ ranking[i]=0;}
    int result=-1;
    int most_occ=0;
    MonomialSet::exp_iterator it=generators.minimalLeadingTerms.expBegin();
    MonomialSet::exp_iterator end=generators.minimalLeadingTerms.expEnd();
    while(it!=end){
        Exponent curr=*it;
        if (curr.deg()>=2){
        Exponent::const_iterator curr_it=curr.begin();
        Exponent::const_iterator curr_end=curr.end();
        while(curr_it!=curr_end){
            ranking[*curr_it]++;
            curr_it++;
        }}
        it++;
    }
    
    for(i=0;i<ranking.size();i++){
        if (most_occ<ranking[i])
        {
            most_occ=ranking[i];
            result=i;
        }
    }
    return result;
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
            char matname[255];
            snprintf(matname,255, "%s%d.png", matrixPrefix.data(), round);

            drawmatrix(mat,matname);
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
            if (mat[i][j]==1){
            #else
            if (mzd_read_bit(mat,i,j)){
            #endif
                p_t.push_back(terms_as_exp[j]);
            }
        }
        assert(polys.size()!=0);
        Polynomial from_mat=add_up_exponents(p_t,polys[0].ring().zero());
        if (UNLIKELY(from_mat.isOne())){
            polys.clear();
            polys.push_back(from_mat);
            return polys;
        }
        polys.push_back(from_mat);//,0,p_t.size()));
    }
    #ifdef HAVE_M4RI
    mzd_free(mat);
    #endif
    return polys;
}
#endif

END_NAMESPACE_PBORIGB
