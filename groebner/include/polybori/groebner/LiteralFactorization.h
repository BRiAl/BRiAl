/*
 *  LiteralFactorization.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 29.05.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include "groebner_defs.h"
#ifndef PBORI_GB_LF_H
#define PBORI_GB_LF_H
BEGIN_NAMESPACE_PBORIGB

std::vector<Polynomial> easy_linear_factors(const Polynomial &p);

class LiteralFactorizationIterator;
class LiteralFactorization{

public:
  typedef LiteralFactorizationIterator const_iterator;
  LiteralFactorization(const Polynomial&);

  typedef std::map<idx_type, int> map_type;
  map_type factors;
  Polynomial rest;
  deg_type lmDeg;
  bool occursAsLeadOfFactor(idx_type v) const;
  bool trivial() const;
  bool is11Factorization();
  bool is00Factorization();
  //Theorem: f BoolePolynomial with factor (x+b), b in 0, 1 (considered in the usual Polynomial Ring)
  //then f/(x+b) does not involve the variable x
  //typedef std::pair<idx_type,idx_type> var_pair_type;
  //typedef std::set<var_pair_type> two_var_factors;
  typedef std::map<idx_type, idx_type> var2var_map_type;
  var2var_map_type var2var_map;
  const_iterator begin();
  const_iterator end();
  protected:

};
class LiteralFactorizationIterator{
public:
    LiteralFactorizationIterator(
        LiteralFactorization* literal_factorization,
        LiteralFactorization::map_type::const_iterator
            var2const_iter,
        LiteralFactorization::var2var_map_type::const_iterator
            var2var_iter
            
        ): m_ring(literal_factorization->rest.ring())
      {
        this->literal_factorization=literal_factorization;
        this->var2const_iter=var2const_iter;
        this->var2var_iter=var2var_iter;
    }
    typedef LiteralFactorizationIterator self;
    LiteralFactorization::var2var_map_type::const_iterator var2var_iter;
    LiteralFactorization::map_type::const_iterator var2const_iter;
    self & operator++();
    Polynomial operator*() const;
    bool operator==(const self& other){
           return ((literal_factorization==other.literal_factorization)
           && (var2const_iter==other.var2const_iter) &&
           (var2var_iter==other.var2var_iter)
           );

       }
       bool operator!=(const self& other){
           return !(*this==other);

       }
private: 
    const LiteralFactorization* literal_factorization;
   
    BoolePolyRing m_ring;
};
deg_type common_literal_factors_deg(const LiteralFactorization& a, const LiteralFactorization& b);

Polynomial 
multiply_with_literal_factors(const LiteralFactorization& lf, Polynomial p);

END_NAMESPACE_PBORIGB
#endif
