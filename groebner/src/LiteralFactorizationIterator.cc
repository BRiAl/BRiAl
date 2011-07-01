/*
 *  LiteralFactorizationIterator.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 29.05.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#include <polybori/groebner/LiteralFactorizationIterator.h>
#include <polybori/groebner/LiteralFactorization.h>
#include <stdexcept>

BEGIN_NAMESPACE_PBORIGB


LiteralFactorizationIterator::self& 
LiteralFactorizationIterator::operator++(){
  if (var2const_iter!=literal_factorization->factors.end()){
    ++var2const_iter;
  }
  else if (var2var_iter!=literal_factorization->var2var_map.end()){
    ++var2var_iter;
  }
  return *this;
}


Polynomial
LiteralFactorizationIterator::operator*() const{
  if (var2const_iter!=literal_factorization->factors.end()){
    return Variable(var2const_iter->first, m_ring)
      + Polynomial(var2const_iter->second, m_ring);
  }
  if (var2var_iter!=literal_factorization->var2var_map.end()){
    return Variable(var2var_iter->first, m_ring)
      + Variable(var2var_iter->second,m_ring);
  }
  throw std::runtime_error("Should never reach here!");
  return m_ring.zero();
}


END_NAMESPACE_PBORIGB
