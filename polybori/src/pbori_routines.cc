// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines.cc
 *
 * @author Alexander Dreyer
 * @date 2006-10-26
 *
 * This file includes some compiled routines.
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
 * Revision 1.2  2006/10/30 13:30:33  dreyer
 * FIX: library compiles for PBORI_ADD_BY_* switches, not using *XOR
 *
 * Revision 1.1  2006/10/26 12:58:25  dreyer
 * ADD: lowlevel routine for union-xor (easy cudd-style variant)
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

#include "cudd.h"
#include "cuddInt.h"

BEGIN_NAMESPACE_PBORI

#ifdef PBORI_LOWLEVEL_XOR 
/* The following should be made more generic */
extern "C" {
DdNode *
pboriCuddZddUnionXor(
  DdManager * zdd,
  DdNode * P,
  DdNode * Q)
{
    int		p_top, q_top;
    DdNode	*empty = DD_ZERO(zdd), *t, *e, *res;
    DdManager	*table = zdd;

    statLine(zdd);

    if (P == empty)
	return(Q); 
    if (Q == empty)
	return(P);
    if (P == Q)
      return(empty);

    /* Check cache */
    res = cuddCacheLookup2Zdd(table, pboriCuddZddUnionXor, P, Q);
    if (res != NULL)
	return(res);

    if (cuddIsConstant(P))
	p_top = P->index;
    else
      p_top = P->index;/* zdd->permZ[P->index]; */
    if (cuddIsConstant(Q))
	q_top = Q->index;
    else
      q_top = Q->index; /* zdd->permZ[Q->index]; */
    if (p_top < q_top) {
	e = pboriCuddZddUnionXor(zdd, cuddE(P), Q);
	if (e == NULL) return (NULL);
	cuddRef(e);
	res = cuddZddGetNode(zdd, P->index, cuddT(P), e);
	if (res == NULL) {
	    Cudd_RecursiveDerefZdd(table, e);
	    return(NULL);
	}
	cuddDeref(e);
    } else if (p_top > q_top) {
	e = pboriCuddZddUnionXor(zdd, P, cuddE(Q));
	if (e == NULL) return(NULL);
	cuddRef(e);
	res = cuddZddGetNode(zdd, Q->index, cuddT(Q), e);
	if (res == NULL) {
	    Cudd_RecursiveDerefZdd(table, e);
	    return(NULL);
	}
	cuddDeref(e);
    } else {
	t = pboriCuddZddUnionXor(zdd, cuddT(P), cuddT(Q));
	if (t == NULL) return(NULL);
	cuddRef(t);
	e = pboriCuddZddUnionXor(zdd, cuddE(P), cuddE(Q));
	if (e == NULL) {
	    Cudd_RecursiveDerefZdd(table, t);
	    return(NULL);
	}
	cuddRef(e);
	res = cuddZddGetNode(zdd, P->index, t, e);
	if (res == NULL) {
	    Cudd_RecursiveDerefZdd(table, t);
	    Cudd_RecursiveDerefZdd(table, e);
	    return(NULL);
	}
	cuddDeref(t);
	cuddDeref(e);
    }

    cuddCacheInsert2(table, pboriCuddZddUnionXor, P, Q, res);

    return(res);

} /* end of pboriCuddZddUnionXor */

DdNode *
pboriCudd_zddUnionXor(
  DdManager * dd,
  DdNode * P,
  DdNode * Q)
{
    DdNode *res;

    do {
	dd->reordered = 0;
	res = pboriCuddZddUnionXor(dd, P, Q);
    } while (dd->reordered == 1);
    return(res);

} /* end of Cudd_zddIntersect */

} /* end of extern "C" */

#endif
END_NAMESPACE_PBORI
