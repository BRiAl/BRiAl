// -*- c++ -*-
//*****************************************************************************
/** @file CNodeCounter.h 
 *
 * @author Alexander Dreyer
 * @date 2011-04-29
 *
 * This file defines the class CNodeCounter which can be used to determine the 
 * number of nodes in a decision diagrm (represented by a navigator). 
 *
 * @par Copyright:
 *   (c) 2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_diagram_CNodeCounter_h_
#define polybori_diagram_CNodeCounter_h_

// include basic definitions
#include <polybori/pbori_defs.h>

BEGIN_NAMESPACE_PBORI

/** @class CNodeCounter
 * @brief This class defines CNodeCounter.
 *
 **/
template<class NaviType>
class CNodeCounter {
  typedef CNodeCounter self;

public:
  typedef NaviType navigator;
  typedef typename NaviType::size_type size_type;

  CNodeCounter(): m_visited() {}
  CNodeCounter(const self& rhs): m_visited(rhs.m_visited) {}

  ~CNodeCounter() {}

  size_type operator()(navigator navi) {
    if (navi.isConstant() || m_visited.count(navi))
      return 0;

    m_visited.insert(navi);
    return (1 + operator()(navi.thenBranch()) + operator()(navi.elseBranch()));
  } 

private:
  std::set<navigator> m_visited;
};

END_NAMESPACE_PBORI

#endif /* polybori_diagram_CNodeCounter_h_ */
