// -*- c++ -*-
//*****************************************************************************
/** @file LiteralFactorizationIterator.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30
 *
 * This file includes the definition of the class @c groebner_LiteralFactorizationIterator.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LiteralFactorizationIterator_h_
#define polybori_groebner_LiteralFactorizationIterator_h_

// include basic definitions
#include "groebner_defs.h"
#include "LiteralFactorization.h"

#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <utility>

BEGIN_NAMESPACE_PBORIGB


/** @class LiteralFactorizationIterator
 * @brief This class defines LiteralFactorizationIterator.
 *
 **/
class LiteralFactorizationIterator{
  typedef LiteralFactorizationIterator self;

public:
  typedef LiteralFactorization fac_type;
  typedef fac_type::var2var_map_type var2var_map_type;
  typedef fac_type::map_type map_type;

  LiteralFactorizationIterator(LiteralFactorization* literal_factorization_,
                               map_type::const_iterator var2const_iter_,
                               var2var_map_type::const_iterator
                               var2var_iter_
                               ):
    var2var_iter(var2var_iter_), var2const_iter(var2const_iter_),
    literal_factorization(literal_factorization_),
    m_ring(literal_factorization_->rest.ring())  { }
  

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
  var2var_map_type::const_iterator var2var_iter;
  map_type::const_iterator var2const_iter;

  const LiteralFactorization* literal_factorization;
  BoolePolyRing m_ring;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_LiteralFactorizationIterator_h_ */
