// -*- c++ -*-
//*****************************************************************************
/** @file pbori_algo.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-07
 *
 * This file includes some templates of simple transformations and similar
 * procedures. 
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
 * Revision 1.1  2006/08/15 16:03:58  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include polybori's definitions
#include "pbori_defs.h"

// get polybori's functionals
#include "pbori_func.h"

#ifndef pbori_algo_int_h_
#define pbori_algo_int_h_

BEGIN_NAMESPACE_PBORI


/// A first version
/// Function templates extracting minimal elements of dd wrt. inclusion
/// Assumption, navi is navigator of dd
template <class NaviType, class DDType>
DDType
dd_minimal_elements(NaviType navi, DDType dd, DDType& multiples, int) {


  if (!navi.isConstant()) {     // Not at end of path

    DdManager* man = dd.manager().getManager();

    DDType elsedd = dd.subset0(*navi);

    DDType elseMultiples;
    elsedd = dd_minimal_elements(navi.elseBranch(), elsedd, elseMultiples, int());

    // short cut if else and then branche equal or else contains 1
    if((navi.elseBranch() == navi.thenBranch()) || elsedd.blankness()){
      multiples = elseMultiples;
      return elsedd;
    }

    NaviType elseMult = elseMultiples.navigation();

    int nmax;
    if (elseMult.isConstant()){
      if (elseMult.terminalValue())
        nmax = dd.nVariables();
      else
        nmax = *navi;
    }
    else
      nmax = *elseMult;

    Cudd_Ref(elseMult); 

    for(int i = nmax-1; i > *navi; --i){
      elseMultiples.uniteAssign(elseMultiples.change(i)); 
    }


    DDType thendd = dd.subset1(*navi);
    thendd = thendd.diff(elseMultiples);

    DDType thenMultiples;
    thendd = dd_minimal_elements(navi.thenBranch(), thendd, thenMultiples, int()); 
 
    NaviType thenMult = thenMultiples.navigation();


    if (thenMult.isConstant()){
      if (thenMult.terminalValue())
        nmax =  dd.nVariables();
      else
        nmax = *navi;
    }
    else
      nmax = *thenMult;


    Cudd_Ref(thenMult);
    NaviType tmp;
    for(int i = nmax-1; i > *navi; --i){
      //      std::cout << ((DdNode*)thenMult) << ":" << ((DdNode*)thenMult) ->ref<<std::endl;
      tmp = cuddZddGetNode(man, i, thenMult, thenMult);
      Cudd_Ref(tmp);
      Cudd_Deref(thenMult);
      thenMult = tmp;
    }



    tmp =  Cudd_zddUnion(man, thenMult, elseMult );

    Cudd_Ref(tmp);
    Cudd_RecursiveDerefZdd(man, thenMult);
    //   Cudd_RecursiveDerefZdd(man, elseMult);

    thenMult = tmp;


    tmp = cuddZddGetNode(man, *navi, thenMult, elseMult);

    Cudd_Ref(tmp);
    Cudd_RecursiveDerefZdd(man, thenMult);
    Cudd_RecursiveDerefZdd(man, elseMult);
    thenMult = tmp;

    thenMultiples = DDType(ZDD(&thenMultiples.manager(), thenMult));
    Cudd_Deref(thenMult);

    //   thenMultiples = thenMultiples.unite(elseMultiples);

    //  std::cout << "huhu2"<<std::endl; std::cout.flush();

    //  thenMultiples = thenMultiples.change(*navi);

    //std::cout << "huhu3"<<std::endl; std::cout.flush();

    //    multiples = thenMultiples.unite(elseMultiples);

    multiples = thenMultiples;

    thendd = thendd.change(*navi);

    DDType result =  thendd.unite(elsedd);

    // std::cout << "huhu end" <<((DdNode*)thenMult) ->ref<<std::endl;
    //  std::cout.flush();

    return result;

  }

  multiples = dd;
  return dd;
}

END_NAMESPACE_PBORI

#endif
