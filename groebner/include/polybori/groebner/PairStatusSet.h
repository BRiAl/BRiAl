// -*- c++ -*-
//*****************************************************************************
/** @file PairStatusSet.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c PairStatusSet.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairStatusSet_h_
#define polybori_groebner_PairStatusSet_h_

// include basic definitions
#include "groebner_defs.h"

#include <boost/dynamic_bitset.hpp>

BEGIN_NAMESPACE_PBORIGB

/** @class PairStatusSet
 * @brief This class defines PairStatusSet.
 *
 **/
class PairStatusSet{
public:
  typedef boost::dynamic_bitset<> bitvector_type;
  bool hasTRep(int ia, int ja) const {
    int i,j;
    i=std::min(ia,ja);
    j=std::max(ia,ja);
    return table[j][i]==HAS_T_REP;
  }
  void setToHasTRep(int ia, int ja){
    int i,j;
    i=std::min(ia,ja);
    j=std::max(ia,ja);
    table[j][i]=HAS_T_REP;
  }

  template <class Iterator>
  void setToHasTRep(Iterator start, Iterator finish, int ja){
    for (; start != finish; ++start)
      setToHasTRep(*start, ja);
  }
  void setToUncalculated(int ia, int ja){
    int i,j;
    i=std::min(ia,ja);
    j=std::max(ia,ja);
    table[j][i]=UNCALCULATED;
  }

  template <class Iterator>
  void setToUncalculated(Iterator start, Iterator finish, int ja){
    for (; start != finish; ++start)
      setToUncalculated(*start, ja);
  }

  int prolong(bool value=UNCALCULATED){
    int s=table.size();
    table.push_back(bitvector_type(s, value));
    return s;
  }
  PairStatusSet(int size=0){
    int s=0;
    for(s=0;s<size;s++){
      prolong();
    }
  }
  static const bool HAS_T_REP=true;
  static const bool UNCALCULATED=false;

protected:
std::vector<bitvector_type> table;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PairStatusSet_h_ */
