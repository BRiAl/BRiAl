// -*- c++ -*-
//*****************************************************************************
/** @file CDegreeCache.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-21
 *
 * This file contains declaration and template specilizations for the class
 * template CDegreeCache.
 *
 * @par Copyright:
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.2  2006/09/21 16:09:59  dreyer
 * ADD: caching mechanism for BoolePolynomial::deg()
 *
 * Revision 1.1  2006/09/21 15:47:14  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

#ifndef CDegreeCache_h_
#define CDegreeCache_h_ 

BEGIN_NAMESPACE_PBORI

template <class DDType>
class CDegreeCache {

public:
  typedef DDType dd_type;
  typedef typename dd_type::manager_base manager_type;

  typedef typename dd_type::navigator navi_type;
  typedef unsigned size_type;

  /// Constructor
  CDegreeCache(const dd_type& dd): m_mgr(dd.manager()) {}

  navi_type find(const navi_type& navi) const;

  size_type convert(const navi_type& result) const;

  void insert(const navi_type& navi, const size_type& deg) const;
protected:

  const manager_type& m_mgr;
};


// Static function prototype for cudd interface
inline static DdNode * cudd_pbori_zdd_deg(DdManager *ddman, DdNode *zdd){}

template<>
inline CDegreeCache<CDDInterface<ZDD> >::navi_type
CDegreeCache<CDDInterface<ZDD> >::find(const navi_type& navi) const {
  return
    cuddCacheLookup1Zdd(m_mgr.getManager(), cudd_pbori_zdd_deg, navi);
}

template<>
inline CDegreeCache< CDDInterface<ZDD> >::size_type
CDegreeCache<CDDInterface<ZDD> >::convert(const navi_type& result) const { 
 
  //  assuming validity
  if (result.isConstant()) {
    if (result.terminalValue())
      return BoolePolyRing::activeManager().nVariables();
    else 
      return 0;
  }
  else 
    return *result;
}

template<>
void
CDegreeCache<CDDInterface<ZDD> >::insert(const navi_type& navi, 
                                         const size_type& deg) const {
  navi_type result;
  if (deg < BoolePolyRing::activeManager().nVariables()){
    result = 
      BoolePolyRing::activeManager().persistentVariable(deg).getNode();
  }
  else {
    result = 
      BoolePolyRing::activeManager().blank().navigation();
  }
  cuddCacheInsert1(m_mgr.getManager(),  cudd_pbori_zdd_deg, 
                   navi,  result);
}


END_NAMESPACE_PBORI

#endif
