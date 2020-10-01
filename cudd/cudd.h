/**CHeaderFile*****************************************************************

  FileName    [cudd.h]

  PackageName [cudd]

  Synopsis    [The University of Colorado decision diagram package.]

  Description [External functions and data strucures of the CUDD package.
  <ul>
  <li> To turn on the gathering of statistics, define DD_STATS.
  <li> To link with mis, define DD_MIS.
  </ul>
  Modified by Abelardo Pardo to interface it to VIS.
  ]

  SeeAlso     []

  Author      [Fabio Somenzi]

  Copyright   [Copyright (c) 1995-2012, Regents of the University of Colorado

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  Neither the name of the University of Colorado nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.]

  Revision    [$Id: cudd.h,v 1.180 2012/02/05 01:07:18 fabio Exp $]

******************************************************************************/

#ifndef _CUDD
#define _CUDD

/*---------------------------------------------------------------------------*/
/* Nested includes                                                           */
/*---------------------------------------------------------------------------*/

#ifdef PBORI_FORCE_ORIGINAL_CUDD
#include "mtr.h"
#include "epd.h"
#ifndef CUDD_ASSERT
#define CUDD_ASSERT assert
#define PBORI_PREFIX(name) name

#endif

#else
#include <polybori/config.h>
#include <polybori/cudd/prefix.h>

#ifndef CUDD_ASSERT

#if defined(PBORI_NDEBUG)
#define CUDD_ASSERT(arg) ((void)(0))
#else
#define CUDD_ASSERT(arg) assert(arg)
#endif

#endif

#endif

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/

#define CUDD_VERSION "2.5.0"

#ifndef SIZEOF_VOID_P
#define SIZEOF_VOID_P 4
#endif
#ifndef SIZEOF_INT
#define SIZEOF_INT 4
#endif
#ifndef SIZEOF_LONG
#define SIZEOF_LONG 4
#endif

#define CUDD_TRUE 1
#define CUDD_FALSE 0

#define CUDD_VALUE_TYPE		double
#define CUDD_OUT_OF_MEM		-1
/* The sizes of the subtables and the cache must be powers of two. */
#define CUDD_UNIQUE_SLOTS	256	/* initial size of subtables */
#define CUDD_CACHE_SLOTS	262144	/* default size of the cache */

/* Constants for residue functions. */
#define CUDD_RESIDUE_DEFAULT	0
#define CUDD_RESIDUE_MSB	1
#define CUDD_RESIDUE_TC		2

/* CUDD_MAXINDEX is defined in such a way that on 32-bit and 64-bit
** machines one can cast an index to (int) without generating a negative
** number.
*/
#if SIZEOF_VOID_P == 8 && SIZEOF_INT == 4
#define CUDD_MAXINDEX		(((DdHalfWord) ~0) >> 1)
#else
#define CUDD_MAXINDEX		((DdHalfWord) ~0)
#endif

/* CUDD_CONST_INDEX is the index of constant nodes.  Currently this
** is a synonim for CUDD_MAXINDEX. */
#define CUDD_CONST_INDEX	CUDD_MAXINDEX

/* These constants define the digits used in the representation of
** arbitrary precision integers.  The configurations tested use 8, 16,
** and 32 bits for each digit.  The typedefs should be in agreement
** with these definitions.
*/
#if SIZEOF_LONG == 8
#define DD_APA_BITS	32
#define DD_APA_BASE	(1L << DD_APA_BITS)
#define DD_APA_HEXPRINT	"%08x"
#else
#define DD_APA_BITS	16
#define DD_APA_BASE	(1 << DD_APA_BITS)
#define DD_APA_HEXPRINT	"%04x"
#endif
#define DD_APA_MASK	(DD_APA_BASE - 1)

/*---------------------------------------------------------------------------*/
/* Stucture declarations                                                     */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Type declarations                                                         */
/*---------------------------------------------------------------------------*/

/**Enum************************************************************************

  Synopsis    [Type of reordering algorithm.]

  Description [Type of reordering algorithm.]

******************************************************************************/
typedef enum {
    CUDD_REORDER_SAME,
    CUDD_REORDER_NONE,
    CUDD_REORDER_RANDOM,
    CUDD_REORDER_RANDOM_PIVOT,
    CUDD_REORDER_SIFT,
    CUDD_REORDER_SIFT_CONVERGE,
    CUDD_REORDER_SYMM_SIFT,
    CUDD_REORDER_SYMM_SIFT_CONV,
    CUDD_REORDER_WINDOW2,
    CUDD_REORDER_WINDOW3,
    CUDD_REORDER_WINDOW4,
    CUDD_REORDER_WINDOW2_CONV,
    CUDD_REORDER_WINDOW3_CONV,
    CUDD_REORDER_WINDOW4_CONV,
    CUDD_REORDER_GROUP_SIFT,
    CUDD_REORDER_GROUP_SIFT_CONV,
    CUDD_REORDER_ANNEALING,
    CUDD_REORDER_GENETIC,
    CUDD_REORDER_LINEAR,
    CUDD_REORDER_LINEAR_CONVERGE,
    CUDD_REORDER_LAZY_SIFT,
    CUDD_REORDER_EXACT
} Cudd_ReorderingType;


/**Enum************************************************************************

  Synopsis    [Type of aggregation methods.]

  Description [Type of aggregation methods.]

******************************************************************************/
typedef enum {
    CUDD_NO_CHECK,
    CUDD_GROUP_CHECK,
    CUDD_GROUP_CHECK2,
    CUDD_GROUP_CHECK3,
    CUDD_GROUP_CHECK4,
    CUDD_GROUP_CHECK5,
    CUDD_GROUP_CHECK6,
    CUDD_GROUP_CHECK7,
    CUDD_GROUP_CHECK8,
    CUDD_GROUP_CHECK9
} Cudd_AggregationType;


/**Enum************************************************************************

  Synopsis    [Type of hooks.]

  Description [Type of hooks.]

******************************************************************************/
typedef enum {
    CUDD_PRE_GC_HOOK,
    CUDD_POST_GC_HOOK,
    CUDD_PRE_REORDERING_HOOK,
    CUDD_POST_REORDERING_HOOK
} Cudd_HookType;


/**Enum************************************************************************

  Synopsis    [Type of error codes.]

  Description [Type of  error codes.]

******************************************************************************/
typedef enum {
    CUDD_NO_ERROR,
    CUDD_MEMORY_OUT,
    CUDD_TOO_MANY_NODES,
    CUDD_MAX_MEM_EXCEEDED,
    CUDD_TIMEOUT_EXPIRED,
    CUDD_INVALID_ARG,
    CUDD_INTERNAL_ERROR
} Cudd_ErrorType;


/**Enum************************************************************************

  Synopsis    [Group type for lazy sifting.]

  Description [Group type for lazy sifting.]

******************************************************************************/
typedef enum {
    CUDD_LAZY_NONE,
    CUDD_LAZY_SOFT_GROUP,
    CUDD_LAZY_HARD_GROUP,
    CUDD_LAZY_UNGROUP
} Cudd_LazyGroupType;


/**Enum************************************************************************

  Synopsis    [Variable type.]

  Description [Variable type. Currently used only in lazy sifting.]

******************************************************************************/
typedef enum {
    CUDD_VAR_PRIMARY_INPUT,
    CUDD_VAR_PRESENT_STATE,
    CUDD_VAR_NEXT_STATE
} Cudd_VariableType;


#if SIZEOF_VOID_P == 8 && SIZEOF_INT == 4
typedef unsigned int   DdHalfWord;
#else
typedef unsigned short DdHalfWord;
#endif

typedef struct DdNode DdNode;

typedef struct DdChildren {
    struct DdNode *T;
    struct DdNode *E;
} DdChildren;

/* The DdNode structure is the only one exported out of the package */
struct DdNode {
    DdHalfWord index;
    DdHalfWord ref;		/* reference count */
    DdNode *next;		/* next pointer for unique table */
    union {
	CUDD_VALUE_TYPE value;	/* for constant nodes */
	DdChildren kids;	/* for internal nodes */
    } type;
};

typedef struct PBORI_PREFIX(DdManager) PBORI_PREFIX(DdManager);

typedef struct DdGen DdGen;

/* These typedefs for arbitrary precision arithmetic should agree with
** the corresponding constant definitions above. */
#if SIZEOF_LONG == 8
typedef unsigned int DdApaDigit;
typedef unsigned long int DdApaDoubleDigit;
#else
typedef unsigned short int DdApaDigit;
typedef unsigned int DdApaDoubleDigit;
#endif
typedef DdApaDigit * DdApaNumber;

/* Return type for function computing two-literal clauses. */
typedef struct DdTlcInfo DdTlcInfo;

/* Type of hook function. */
typedef int (*DD_HFP)(PBORI_PREFIX(DdManager) *, const char *, void *);
/* Type of priority function */
typedef DdNode * (*DD_PRFP)(PBORI_PREFIX(DdManager) * , int, DdNode **, DdNode **,
			    DdNode **);
/* Type of apply operator. */
typedef DdNode * (*DD_AOP)(PBORI_PREFIX(DdManager) *, DdNode **, DdNode **);
/* Type of monadic apply operator. */
typedef DdNode * (*DD_MAOP)(PBORI_PREFIX(DdManager) *, DdNode *);
/* Types of cache tag functions. */
typedef DdNode * (*DD_CTFP)(PBORI_PREFIX(DdManager) *, DdNode *, DdNode *);
typedef DdNode * (*DD_CTFP1)(PBORI_PREFIX(DdManager) *, DdNode *);
/* Type of memory-out function. */
typedef void (*DD_OOMFP)(long);
/* Type of comparison function for qsort. */
typedef int (*DD_QSFP)(const void *, const void *);

/*---------------------------------------------------------------------------*/
/* Variable declarations                                                     */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Macro declarations                                                        */
/*---------------------------------------------------------------------------*/


/**Macro***********************************************************************

  Synopsis     [Returns 1 if the node is a constant node.]

  Description  [Returns 1 if the node is a constant node (rather than an
  internal node). All constant nodes have the same index
  (CUDD_CONST_INDEX). The pointer passed to Cudd_IsConstant may be either
  regular or complemented.]

  SideEffects  [none]

  SeeAlso      []

******************************************************************************/
#define Cudd_IsConstant(node) ((Cudd_Regular(node))->index == CUDD_CONST_INDEX)


/**Macro***********************************************************************

  Synopsis     [Complements a DD.]

  Description  [Complements a DD by flipping the complement attribute of
  the pointer (the least significant bit).]

  SideEffects  [none]

  SeeAlso      [Cudd_NotCond]

******************************************************************************/
#define Cudd_Not(node) ((DdNode *)((long)(node) ^ 01))


/**Macro***********************************************************************

  Synopsis     [Complements a DD if a condition is true.]

  Description  [Complements a DD if condition c is true; c should be
  either 0 or 1, because it is used directly (for efficiency). If in
  doubt on the values c may take, use "(c) ? Cudd_Not(node) : node".]

  SideEffects  [none]

  SeeAlso      [Cudd_Not]

******************************************************************************/
#define Cudd_NotCond(node,c) ((DdNode *)((long)(node) ^ (c)))


/**Macro***********************************************************************

  Synopsis     [Returns the regular version of a pointer.]

  Description  []

  SideEffects  [none]

  SeeAlso      [Cudd_Complement Cudd_IsComplement]

******************************************************************************/
#define Cudd_Regular(node) ((DdNode *)((unsigned long)(node) & ~01))


/**Macro***********************************************************************

  Synopsis     [Returns the complemented version of a pointer.]

  Description  []

  SideEffects  [none]

  SeeAlso      [Cudd_Regular Cudd_IsComplement]

******************************************************************************/
#define Cudd_Complement(node) ((DdNode *)((unsigned long)(node) | 01))


/**Macro***********************************************************************

  Synopsis     [Returns 1 if a pointer is complemented.]

  Description  []

  SideEffects  [none]

  SeeAlso      [Cudd_Regular Cudd_Complement]

******************************************************************************/
#define Cudd_IsComplement(node)	((int) ((long) (node) & 01))


/**Macro***********************************************************************

  Synopsis     [Returns the then child of an internal node.]

  Description  [Returns the then child of an internal node. If
  <code>node</code> is a constant node, the result is unpredictable.]

  SideEffects  [none]

  SeeAlso      [Cudd_E Cudd_V]

******************************************************************************/
#define Cudd_T(node) ((Cudd_Regular(node))->type.kids.T)


/**Macro***********************************************************************

  Synopsis     [Returns the else child of an internal node.]

  Description  [Returns the else child of an internal node. If
  <code>node</code> is a constant node, the result is unpredictable.]

  SideEffects  [none]

  SeeAlso      [Cudd_T Cudd_V]

******************************************************************************/
#define Cudd_E(node) ((Cudd_Regular(node))->type.kids.E)


/**Macro***********************************************************************

  Synopsis     [Returns the value of a constant node.]

  Description  [Returns the value of a constant node. If
  <code>node</code> is an internal node, the result is unpredictable.]

  SideEffects  [none]

  SeeAlso      [Cudd_T Cudd_E]

******************************************************************************/
#define Cudd_V(node) ((Cudd_Regular(node))->type.value)


/**Macro***********************************************************************

  Synopsis     [Returns the current position in the order of variable
  index.]

  Description [Returns the current position in the order of variable
  index. This macro is obsolete and is kept for compatibility. New
  applications should use Cudd_ReadPerm instead.]

  SideEffects  [none]

  SeeAlso      [Cudd_ReadPerm]

******************************************************************************/
#define Cudd_ReadIndex(dd,index) (Cudd_ReadPerm(dd,index))


/**Macro***********************************************************************

  Synopsis     [Iterates over the cubes of a decision diagram.]

  Description  [Iterates over the cubes of a decision diagram f.
  <ul>
  <li> PBORI_PREFIX(DdManager) *manager;
  <li> DdNode *f;
  <li> DdGen *gen;
  <li> int *cube;
  <li> CUDD_VALUE_TYPE value;
  </ul>
  Cudd_ForeachCube allocates and frees the generator. Therefore the
  application should not try to do that. Also, the cube is freed at the
  end of Cudd_ForeachCube and hence is not available outside of the loop.<p>
  CAUTION: It is assumed that dynamic reordering will not occur while
  there are open generators. It is the user's responsibility to make sure
  that dynamic reordering does not occur. As long as new nodes are not created
  during generation, and dynamic reordering is not called explicitly,
  dynamic reordering will not occur. Alternatively, it is sufficient to
  disable dynamic reordering. It is a mistake to dispose of a diagram
  on which generation is ongoing.]

  SideEffects  [none]

  SeeAlso      [Cudd_ForeachNode Cudd_FirstCube Cudd_NextCube Cudd_GenFree
  Cudd_IsGenEmpty Cudd_AutodynDisable]

******************************************************************************/
#define Cudd_ForeachCube(manager, f, gen, cube, value)\
    for((gen) = Cudd_FirstCube(manager, f, &cube, &value);\
	Cudd_IsGenEmpty(gen) ? Cudd_GenFree(gen) : CUDD_TRUE;\
	(void) Cudd_NextCube(gen, &cube, &value))


/**Macro***********************************************************************

  Synopsis     [Iterates over the primes of a Boolean function.]

  Description  [Iterates over the primes of a Boolean function producing
  a prime and irredundant cover.
  <ul>
  <li> PBORI_PREFIX(DdManager) *manager;
  <li> DdNode *l;
  <li> DdNode *u;
  <li> DdGen *gen;
  <li> int *cube;
  </ul>
  The Boolean function is described by an upper bound and a lower bound.  If
  the function is completely specified, the two bounds coincide.
  Cudd_ForeachPrime allocates and frees the generator.  Therefore the
  application should not try to do that.  Also, the cube is freed at the
  end of Cudd_ForeachPrime and hence is not available outside of the loop.<p>
  CAUTION: It is a mistake to change a diagram on which generation is ongoing.]

  SideEffects  [none]

  SeeAlso      [Cudd_ForeachCube Cudd_FirstPrime Cudd_NextPrime Cudd_GenFree
  Cudd_IsGenEmpty]

******************************************************************************/
#define Cudd_ForeachPrime(manager, l, u, gen, cube)\
    for((gen) = Cudd_FirstPrime(manager, l, u, &cube);\
	Cudd_IsGenEmpty(gen) ? Cudd_GenFree(gen) : CUDD_TRUE;\
	(void) Cudd_NextPrime(gen, &cube))


/**Macro***********************************************************************

  Synopsis     [Iterates over the nodes of a decision diagram.]

  Description  [Iterates over the nodes of a decision diagram f.
  <ul>
  <li> PBORI_PREFIX(DdManager) *manager;
  <li> DdNode *f;
  <li> DdGen *gen;
  <li> DdNode *node;
  </ul>
  The nodes are returned in a seemingly random order.
  Cudd_ForeachNode allocates and frees the generator. Therefore the
  application should not try to do that.<p>
  CAUTION: It is assumed that dynamic reordering will not occur while
  there are open generators. It is the user's responsibility to make sure
  that dynamic reordering does not occur. As long as new nodes are not created
  during generation, and dynamic reordering is not called explicitly,
  dynamic reordering will not occur. Alternatively, it is sufficient to
  disable dynamic reordering. It is a mistake to dispose of a diagram
  on which generation is ongoing.]

  SideEffects  [none]

  SeeAlso      [Cudd_ForeachCube Cudd_FirstNode Cudd_NextNode Cudd_GenFree
  Cudd_IsGenEmpty Cudd_AutodynDisable]

******************************************************************************/
#define Cudd_ForeachNode(manager, f, gen, node)\
    for((gen) = Cudd_FirstNode(manager, f, &node);\
	Cudd_IsGenEmpty(gen) ? Cudd_GenFree(gen) : CUDD_TRUE;\
	(void) Cudd_NextNode(gen, &node))


/**Macro***********************************************************************

  Synopsis     [Iterates over the paths of a ZDD.]

  Description  [Iterates over the paths of a ZDD f.
  <ul>
  <li> PBORI_PREFIX(DdManager) *manager;
  <li> DdNode *f;
  <li> DdGen *gen;
  <li> int *path;
  </ul>
  Cudd_zddForeachPath allocates and frees the generator. Therefore the
  application should not try to do that. Also, the path is freed at the
  end of Cudd_zddForeachPath and hence is not available outside of the loop.<p>
  CAUTION: It is assumed that dynamic reordering will not occur while
  there are open generators.  It is the user's responsibility to make sure
  that dynamic reordering does not occur.  As long as new nodes are not created
  during generation, and dynamic reordering is not called explicitly,
  dynamic reordering will not occur.  Alternatively, it is sufficient to
  disable dynamic reordering.  It is a mistake to dispose of a diagram
  on which generation is ongoing.]

  SideEffects  [none]

  SeeAlso      [Cudd_zddFirstPath Cudd_zddNextPath Cudd_GenFree
  Cudd_IsGenEmpty Cudd_AutodynDisable]

******************************************************************************/
#define Cudd_zddForeachPath(manager, f, gen, path)\
    for((gen) = Cudd_zddFirstPath(manager, f, &path);\
	Cudd_IsGenEmpty(gen) ? Cudd_GenFree(gen) : CUDD_TRUE;\
	(void) Cudd_zddNextPath(gen, &path))



/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/

#ifndef PBORI_FORCE_ORIGINAL_CUDD
extern DdNode * Cudd_addNewVar (PBORI_PREFIX(DdManager) *dd);
extern DdNode * Cudd_addNewVarAtLevel (PBORI_PREFIX(DdManager) *dd, int level);
extern DdNode * Cudd_bddNewVar (PBORI_PREFIX(DdManager) *dd);
extern DdNode * Cudd_bddNewVarAtLevel (PBORI_PREFIX(DdManager) *dd, int level);
extern DdNode * Cudd_addIthVar (PBORI_PREFIX(DdManager) *dd, int i);
extern DdNode * Cudd_bddIthVar (PBORI_PREFIX(DdManager) *dd, int i);
extern DdNode * PBORI_PREFIX(Cudd_zddIthVar) (PBORI_PREFIX(DdManager) *dd, int i);
extern int Cudd_zddVarsFromBddVars (PBORI_PREFIX(DdManager) *dd, int multiplicity);
extern DdNode * Cudd_addConst (PBORI_PREFIX(DdManager) *dd, CUDD_VALUE_TYPE c);
extern int Cudd_IsNonConstant (DdNode *f);
extern unsigned long Cudd_ReadStartTime(PBORI_PREFIX(DdManager) *unique);
extern unsigned long Cudd_ReadElapsedTime(PBORI_PREFIX(DdManager) *unique);
extern void Cudd_SetStartTime(PBORI_PREFIX(DdManager) *unique, unsigned long st);
extern void Cudd_ResetStartTime(PBORI_PREFIX(DdManager) *unique);
extern unsigned long Cudd_ReadTimeLimit(PBORI_PREFIX(DdManager) *unique);
extern void Cudd_SetTimeLimit(PBORI_PREFIX(DdManager) *unique, unsigned long tl);
extern void Cudd_UpdateTimeLimit(PBORI_PREFIX(DdManager) * unique);
extern void Cudd_IncreaseTimeLimit(PBORI_PREFIX(DdManager) * unique, unsigned long increase);
extern void Cudd_UnsetTimeLimit(PBORI_PREFIX(DdManager) *unique);
extern int Cudd_TimeLimited(PBORI_PREFIX(DdManager) *unique);
extern void Cudd_AutodynEnable (PBORI_PREFIX(DdManager) *unique, Cudd_ReorderingType method);
extern void Cudd_AutodynDisable (PBORI_PREFIX(DdManager) *unique);
extern int Cudd_ReorderingStatus (PBORI_PREFIX(DdManager) *unique, Cudd_ReorderingType *method);
extern void Cudd_AutodynEnableZdd (PBORI_PREFIX(DdManager) *unique, Cudd_ReorderingType method);
extern void Cudd_AutodynDisableZdd (PBORI_PREFIX(DdManager) *unique);
extern int Cudd_ReorderingStatusZdd (PBORI_PREFIX(DdManager) *unique, Cudd_ReorderingType *method);
extern int Cudd_zddRealignmentEnabled (PBORI_PREFIX(DdManager) *unique);
extern void Cudd_zddRealignEnable (PBORI_PREFIX(DdManager) *unique);
extern void Cudd_zddRealignDisable (PBORI_PREFIX(DdManager) *unique);
extern int Cudd_bddRealignmentEnabled (PBORI_PREFIX(DdManager) *unique);
extern void Cudd_bddRealignEnable (PBORI_PREFIX(DdManager) *unique);
extern void Cudd_bddRealignDisable (PBORI_PREFIX(DdManager) *unique);
extern DdNode * Cudd_ReadOne (PBORI_PREFIX(DdManager) *dd);
extern DdNode * PBORI_PREFIX(Cudd_ReadZddOne) (PBORI_PREFIX(DdManager) *dd, int i);
extern DdNode * PBORI_PREFIX(Cudd_ReadZero) (PBORI_PREFIX(DdManager) *dd);
extern DdNode * Cudd_ReadLogicZero (PBORI_PREFIX(DdManager) *dd);
extern DdNode * Cudd_ReadPlusInfinity (PBORI_PREFIX(DdManager) *dd);
extern DdNode * Cudd_ReadMinusInfinity (PBORI_PREFIX(DdManager) *dd);
extern DdNode * Cudd_ReadBackground (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetBackground (PBORI_PREFIX(DdManager) *dd, DdNode *bck);
extern unsigned int Cudd_ReadCacheSlots (PBORI_PREFIX(DdManager) *dd);
extern double Cudd_ReadCacheUsedSlots (PBORI_PREFIX(DdManager) * dd);
extern double Cudd_ReadCacheLookUps (PBORI_PREFIX(DdManager) *dd);
extern double Cudd_ReadCacheHits (PBORI_PREFIX(DdManager) *dd);
extern double Cudd_ReadRecursiveCalls (PBORI_PREFIX(DdManager) * dd);
extern unsigned int Cudd_ReadMinHit (PBORI_PREFIX(DdManager) *dd);
extern void PBORI_PREFIX(Cudd_SetMinHit) (PBORI_PREFIX(DdManager) *dd, unsigned int hr);
extern unsigned int Cudd_ReadLooseUpTo (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetLooseUpTo (PBORI_PREFIX(DdManager) *dd, unsigned int lut);
extern unsigned int Cudd_ReadMaxCache (PBORI_PREFIX(DdManager) *dd);
extern unsigned int Cudd_ReadMaxCacheHard (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetMaxCacheHard (PBORI_PREFIX(DdManager) *dd, unsigned int mc);
extern int Cudd_ReadSize (PBORI_PREFIX(DdManager) *dd);
extern int PBORI_PREFIX(Cudd_ReadZddSize) (PBORI_PREFIX(DdManager) *dd);
extern unsigned int Cudd_ReadSlots (PBORI_PREFIX(DdManager) *dd);
extern double Cudd_ReadUsedSlots (PBORI_PREFIX(DdManager) * dd);
extern double Cudd_ExpectedUsedSlots (PBORI_PREFIX(DdManager) * dd);
extern unsigned int Cudd_ReadKeys (PBORI_PREFIX(DdManager) *dd);
extern unsigned int Cudd_ReadDead (PBORI_PREFIX(DdManager) *dd);
extern unsigned int Cudd_ReadMinDead (PBORI_PREFIX(DdManager) *dd);
extern unsigned int Cudd_ReadReorderings (PBORI_PREFIX(DdManager) *dd);
extern unsigned int Cudd_ReadMaxReorderings (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetMaxReorderings (PBORI_PREFIX(DdManager) *dd, unsigned int mr);
extern long Cudd_ReadReorderingTime (PBORI_PREFIX(DdManager) * dd);
extern int Cudd_ReadGarbageCollections (PBORI_PREFIX(DdManager) * dd);
extern long Cudd_ReadGarbageCollectionTime (PBORI_PREFIX(DdManager) * dd);
extern double Cudd_ReadNodesFreed (PBORI_PREFIX(DdManager) * dd);
extern double Cudd_ReadNodesDropped (PBORI_PREFIX(DdManager) * dd);
extern double Cudd_ReadUniqueLookUps (PBORI_PREFIX(DdManager) * dd);
extern double Cudd_ReadUniqueLinks (PBORI_PREFIX(DdManager) * dd);
extern int Cudd_ReadSiftMaxVar (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetSiftMaxVar (PBORI_PREFIX(DdManager) *dd, int smv);
extern int Cudd_ReadSiftMaxSwap (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetSiftMaxSwap (PBORI_PREFIX(DdManager) *dd, int sms);
extern double Cudd_ReadMaxGrowth (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetMaxGrowth (PBORI_PREFIX(DdManager) *dd, double mg);
extern double Cudd_ReadMaxGrowthAlternate (PBORI_PREFIX(DdManager) * dd);
extern void Cudd_SetMaxGrowthAlternate (PBORI_PREFIX(DdManager) * dd, double mg);
extern int Cudd_ReadReorderingCycle (PBORI_PREFIX(DdManager) * dd);
extern void Cudd_SetReorderingCycle (PBORI_PREFIX(DdManager) * dd, int cycle);
#ifdef PBORI_FORCE_ORIGINAL_CUDD
extern MtrNode * Cudd_ReadTree (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetTree (PBORI_PREFIX(DdManager) *dd, MtrNode *tree);
extern void Cudd_FreeTree (PBORI_PREFIX(DdManager) *dd);
extern MtrNode * Cudd_ReadZddTree (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetZddTree (PBORI_PREFIX(DdManager) *dd, MtrNode *tree);
extern void Cudd_FreeZddTree (PBORI_PREFIX(DdManager) *dd);
#endif
extern unsigned int PBORI_PREFIX(Cudd_NodeReadIndex) (DdNode *node);
extern int Cudd_ReadPerm (PBORI_PREFIX(DdManager) *dd, int i);
extern int Cudd_ReadPermZdd (PBORI_PREFIX(DdManager) *dd, int i);
extern int Cudd_ReadInvPerm (PBORI_PREFIX(DdManager) *dd, int i);
extern int Cudd_ReadInvPermZdd (PBORI_PREFIX(DdManager) *dd, int i);
extern DdNode * Cudd_ReadVars (PBORI_PREFIX(DdManager) *dd, int i);
extern CUDD_VALUE_TYPE Cudd_ReadEpsilon (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetEpsilon (PBORI_PREFIX(DdManager) *dd, CUDD_VALUE_TYPE ep);
extern Cudd_AggregationType Cudd_ReadGroupcheck (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetGroupcheck (PBORI_PREFIX(DdManager) *dd, Cudd_AggregationType gc);
extern int Cudd_GarbageCollectionEnabled (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_EnableGarbageCollection (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_DisableGarbageCollection (PBORI_PREFIX(DdManager) *dd);
extern int Cudd_DeadAreCounted (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_TurnOnCountDead (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_TurnOffCountDead (PBORI_PREFIX(DdManager) *dd);
extern int Cudd_ReadRecomb (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetRecomb (PBORI_PREFIX(DdManager) *dd, int recomb);
extern int Cudd_ReadSymmviolation (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetSymmviolation (PBORI_PREFIX(DdManager) *dd, int symmviolation);
extern int Cudd_ReadArcviolation (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetArcviolation (PBORI_PREFIX(DdManager) *dd, int arcviolation);
extern int Cudd_ReadPopulationSize (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetPopulationSize (PBORI_PREFIX(DdManager) *dd, int populationSize);
extern int Cudd_ReadNumberXovers (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetNumberXovers (PBORI_PREFIX(DdManager) *dd, int numberXovers);
extern unsigned int Cudd_ReadOrderRandomization(PBORI_PREFIX(DdManager) * dd);
extern void Cudd_SetOrderRandomization(PBORI_PREFIX(DdManager) * dd, unsigned int factor);
extern unsigned long Cudd_ReadMemoryInUse (PBORI_PREFIX(DdManager) *dd);
extern int PBORI_PREFIX(Cudd_PrintInfo) (PBORI_PREFIX(DdManager) *dd, FILE *fp);
extern long PBORI_PREFIX(Cudd_ReadPeakNodeCount) (PBORI_PREFIX(DdManager) *dd);
extern int PBORI_PREFIX(Cudd_ReadPeakLiveNodeCount) (PBORI_PREFIX(DdManager) * dd);
extern long Cudd_ReadNodeCount (PBORI_PREFIX(DdManager) *dd);
extern long PBORI_PREFIX(Cudd_zddReadNodeCount) (PBORI_PREFIX(DdManager) *dd);
extern int PBORI_PREFIX(Cudd_AddHook) (PBORI_PREFIX(DdManager) *dd, DD_HFP f, Cudd_HookType where);
extern int PBORI_PREFIX(Cudd_RemoveHook) (PBORI_PREFIX(DdManager) *dd, DD_HFP f, Cudd_HookType where);
extern int Cudd_IsInHook (PBORI_PREFIX(DdManager) * dd, DD_HFP f, Cudd_HookType where);
extern int Cudd_StdPreReordHook (PBORI_PREFIX(DdManager) *dd, const char *str, void *data);
extern int Cudd_StdPostReordHook (PBORI_PREFIX(DdManager) *dd, const char *str, void *data);
extern int Cudd_EnableReorderingReporting (PBORI_PREFIX(DdManager) *dd);
extern int Cudd_DisableReorderingReporting (PBORI_PREFIX(DdManager) *dd);
extern int Cudd_ReorderingReporting (PBORI_PREFIX(DdManager) *dd);
extern int Cudd_PrintGroupedOrder(PBORI_PREFIX(DdManager) * dd, const char *str, void *data);
extern int Cudd_EnableOrderingMonitoring(PBORI_PREFIX(DdManager) *dd);
extern int Cudd_DisableOrderingMonitoring(PBORI_PREFIX(DdManager) *dd);
extern int Cudd_OrderingMonitoring(PBORI_PREFIX(DdManager) *dd);
extern Cudd_ErrorType PBORI_PREFIX(Cudd_ReadErrorCode) (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_ClearErrorCode (PBORI_PREFIX(DdManager) *dd);
extern FILE * Cudd_ReadStdout (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetStdout (PBORI_PREFIX(DdManager) *dd, FILE *fp);
extern FILE * Cudd_ReadStderr (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetStderr (PBORI_PREFIX(DdManager) *dd, FILE *fp);
extern unsigned int PBORI_PREFIX(Cudd_ReadNextReordering) (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetNextReordering (PBORI_PREFIX(DdManager) *dd, unsigned int next);
extern double Cudd_ReadSwapSteps (PBORI_PREFIX(DdManager) *dd);
extern unsigned int Cudd_ReadMaxLive (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetMaxLive (PBORI_PREFIX(DdManager) *dd, unsigned int maxLive);
extern unsigned long Cudd_ReadMaxMemory (PBORI_PREFIX(DdManager) *dd);
extern void Cudd_SetMaxMemory (PBORI_PREFIX(DdManager) *dd, unsigned long maxMemory);
extern int Cudd_bddBindVar (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddUnbindVar (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddVarIsBound (PBORI_PREFIX(DdManager) *dd, int index);
extern DdNode * Cudd_addExistAbstract (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *cube);
extern DdNode * Cudd_addUnivAbstract (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *cube);
extern DdNode * Cudd_addOrAbstract (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *cube);
extern DdNode * Cudd_addApply (PBORI_PREFIX(DdManager) *dd, DdNode * (*)(PBORI_PREFIX(DdManager) *, DdNode **, DdNode **), DdNode *f, DdNode *g);
extern DdNode * Cudd_addPlus (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addTimes (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addThreshold (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addSetNZ (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addDivide (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addMinus (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addMinimum (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addMaximum (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addOneZeroMaximum (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addDiff (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addAgreement (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addOr (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addNand (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addNor (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addXor (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addXnor (PBORI_PREFIX(DdManager) *dd, DdNode **f, DdNode **g);
extern DdNode * Cudd_addMonadicApply (PBORI_PREFIX(DdManager) * dd, DdNode * (*op)(PBORI_PREFIX(DdManager) *, DdNode *), DdNode * f);
extern DdNode * Cudd_addLog (PBORI_PREFIX(DdManager) * dd, DdNode * f);
extern DdNode * Cudd_addFindMax (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern DdNode * Cudd_addFindMin (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern DdNode * Cudd_addIthBit (PBORI_PREFIX(DdManager) *dd, DdNode *f, int bit);
extern DdNode * Cudd_addScalarInverse (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *epsilon);
extern DdNode * Cudd_addIte (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode *h);
extern DdNode * Cudd_addIteConstant (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode *h);
extern DdNode * Cudd_addEvalConst (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern int Cudd_addLeq (PBORI_PREFIX(DdManager) * dd, DdNode * f, DdNode * g);
extern DdNode * Cudd_addCmpl (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern DdNode * Cudd_addNegate (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern DdNode * Cudd_addRoundOff (PBORI_PREFIX(DdManager) *dd, DdNode *f, int N);
extern DdNode * Cudd_addWalsh (PBORI_PREFIX(DdManager) *dd, DdNode **x, DdNode **y, int n);
extern DdNode * Cudd_addResidue (PBORI_PREFIX(DdManager) *dd, int n, int m, int options, int top);
extern DdNode * Cudd_bddAndAbstract (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *g, DdNode *cube);
extern DdNode * Cudd_bddAndAbstractLimit (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *g, DdNode *cube, unsigned int limit);
extern int Cudd_ApaNumberOfDigits (int binaryDigits);
extern DdApaNumber Cudd_NewApaNumber (int digits);
extern void Cudd_ApaCopy (int digits, DdApaNumber source, DdApaNumber dest);
extern DdApaDigit Cudd_ApaAdd (int digits, DdApaNumber a, DdApaNumber b, DdApaNumber sum);
extern DdApaDigit Cudd_ApaSubtract (int digits, DdApaNumber a, DdApaNumber b, DdApaNumber diff);
extern DdApaDigit Cudd_ApaShortDivision (int digits, DdApaNumber dividend, DdApaDigit divisor, DdApaNumber quotient);
extern unsigned int Cudd_ApaIntDivision (int  digits, DdApaNumber dividend, unsigned int  divisor, DdApaNumber  quotient);
extern void Cudd_ApaShiftRight (int digits, DdApaDigit in, DdApaNumber a, DdApaNumber b);
extern void Cudd_ApaSetToLiteral (int digits, DdApaNumber number, DdApaDigit literal);
extern void Cudd_ApaPowerOfTwo (int digits, DdApaNumber number, int power);
extern int Cudd_ApaCompare (int digitsFirst, DdApaNumber  first, int digitsSecond, DdApaNumber  second);
extern int Cudd_ApaCompareRatios (int digitsFirst, DdApaNumber firstNum, unsigned int firstDen, int digitsSecond, DdApaNumber secondNum, unsigned int secondDen);
extern int Cudd_ApaPrintHex (FILE *fp, int digits, DdApaNumber number);
extern int Cudd_ApaPrintDecimal (FILE *fp, int digits, DdApaNumber number);
extern int Cudd_ApaPrintExponential (FILE * fp, int  digits, DdApaNumber  number, int precision);
extern DdApaNumber Cudd_ApaCountMinterm (PBORI_PREFIX(DdManager) *manager, DdNode *node, int nvars, int *digits);
extern int Cudd_ApaPrintMinterm (FILE *fp, PBORI_PREFIX(DdManager) *dd, DdNode *node, int nvars);
extern int Cudd_ApaPrintMintermExp (FILE * fp, PBORI_PREFIX(DdManager) * dd, DdNode * node, int  nvars, int precision);
extern int Cudd_ApaPrintDensity (FILE * fp, PBORI_PREFIX(DdManager) * dd, DdNode * node, int  nvars);
extern DdNode * Cudd_UnderApprox (PBORI_PREFIX(DdManager) *dd, DdNode *f, int numVars, int threshold, int safe, double quality);
extern DdNode * Cudd_OverApprox (PBORI_PREFIX(DdManager) *dd, DdNode *f, int numVars, int threshold, int safe, double quality);
extern DdNode * Cudd_RemapUnderApprox (PBORI_PREFIX(DdManager) *dd, DdNode *f, int numVars, int threshold, double quality);
extern DdNode * Cudd_RemapOverApprox (PBORI_PREFIX(DdManager) *dd, DdNode *f, int numVars, int threshold, double quality);
extern DdNode * Cudd_BiasedUnderApprox (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *b, int numVars, int threshold, double quality1, double quality0);
extern DdNode * Cudd_BiasedOverApprox (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *b, int numVars, int threshold, double quality1, double quality0);
extern DdNode * Cudd_bddExistAbstract (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *cube);
extern DdNode * Cudd_bddExistAbstractLimit(PBORI_PREFIX(DdManager) * manager, DdNode * f, DdNode * cube, unsigned int limit);
extern DdNode * Cudd_bddXorExistAbstract (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *g, DdNode *cube);
extern DdNode * Cudd_bddUnivAbstract (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *cube);
extern DdNode * Cudd_bddBooleanDiff (PBORI_PREFIX(DdManager) *manager, DdNode *f, int x);
extern int Cudd_bddVarIsDependent (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *var);
extern double Cudd_bddCorrelation (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *g);
extern double Cudd_bddCorrelationWeights (PBORI_PREFIX(DdManager) *manager, DdNode *f, DdNode *g, double *prob);
extern DdNode * Cudd_bddIte (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode *h);
  extern DdNode * Cudd_bddIteLimit (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode *h, unsigned int limit);
extern DdNode * Cudd_bddIteConstant (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode *h);
extern DdNode * Cudd_bddIntersect (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode * Cudd_bddAnd (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode * Cudd_bddAndLimit (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, unsigned int limit);
extern DdNode * Cudd_bddOr (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode * Cudd_bddOrLimit (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, unsigned int limit);
extern DdNode * Cudd_bddNand (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode * Cudd_bddNor (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode * Cudd_bddXor (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode * Cudd_bddXnor (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode * Cudd_bddXnorLimit (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, unsigned int limit);
extern int Cudd_bddLeq (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode * Cudd_addBddThreshold (PBORI_PREFIX(DdManager) *dd, DdNode *f, CUDD_VALUE_TYPE value);
extern DdNode * Cudd_addBddStrictThreshold (PBORI_PREFIX(DdManager) *dd, DdNode *f, CUDD_VALUE_TYPE value);
extern DdNode * Cudd_addBddInterval (PBORI_PREFIX(DdManager) *dd, DdNode *f, CUDD_VALUE_TYPE lower, CUDD_VALUE_TYPE upper);
extern DdNode * Cudd_addBddIthBit (PBORI_PREFIX(DdManager) *dd, DdNode *f, int bit);
extern DdNode * Cudd_BddToAdd (PBORI_PREFIX(DdManager) *dd, DdNode *B);
extern DdNode * Cudd_addBddPattern (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern DdNode * Cudd_bddTransfer (PBORI_PREFIX(DdManager) *ddSource, PBORI_PREFIX(DdManager) *ddDestination, DdNode *f);
extern int Cudd_DebugCheck (PBORI_PREFIX(DdManager) *table);
extern int Cudd_CheckKeys (PBORI_PREFIX(DdManager) *table);
extern DdNode * Cudd_bddClippingAnd (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, int maxDepth, int direction);
extern DdNode * Cudd_bddClippingAndAbstract (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode *cube, int maxDepth, int direction);
extern DdNode * Cudd_Cofactor (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern int Cudd_CheckCube (PBORI_PREFIX(DdManager) *dd, DdNode *g);
extern DdNode * Cudd_bddCompose (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, int v);
extern DdNode * Cudd_addCompose (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, int v);
extern DdNode * Cudd_addPermute (PBORI_PREFIX(DdManager) *manager, DdNode *node, int *permut);
extern DdNode * Cudd_addSwapVariables (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **x, DdNode **y, int n);
extern DdNode * Cudd_bddPermute (PBORI_PREFIX(DdManager) *manager, DdNode *node, int *permut);
extern DdNode * Cudd_bddVarMap (PBORI_PREFIX(DdManager) *manager, DdNode *f);
extern int Cudd_SetVarMap (PBORI_PREFIX(DdManager) *manager, DdNode **x, DdNode **y, int n);
extern DdNode * Cudd_bddSwapVariables (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **x, DdNode **y, int n);
extern DdNode * Cudd_bddAdjPermuteX (PBORI_PREFIX(DdManager) *dd, DdNode *B, DdNode **x, int n);
extern DdNode * Cudd_addVectorCompose (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **vector);
extern DdNode * Cudd_addGeneralVectorCompose (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **vectorOn, DdNode **vectorOff);
extern DdNode * Cudd_addNonSimCompose (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **vector);
extern DdNode * Cudd_bddVectorCompose (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **vector);
extern int Cudd_bddApproxConjDecomp (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode ***conjuncts);
extern int Cudd_bddApproxDisjDecomp (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode ***disjuncts);
extern int Cudd_bddIterConjDecomp (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode ***conjuncts);
extern int Cudd_bddIterDisjDecomp (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode ***disjuncts);
extern int Cudd_bddGenConjDecomp (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode ***conjuncts);
extern int Cudd_bddGenDisjDecomp (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode ***disjuncts);
extern int Cudd_bddVarConjDecomp (PBORI_PREFIX(DdManager) *dd, DdNode * f, DdNode ***conjuncts);
extern int Cudd_bddVarDisjDecomp (PBORI_PREFIX(DdManager) *dd, DdNode * f, DdNode ***disjuncts);
extern DdNode * Cudd_FindEssential (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern int Cudd_bddIsVarEssential (PBORI_PREFIX(DdManager) *manager, DdNode *f, int id, int phase);
extern DdTlcInfo * Cudd_FindTwoLiteralClauses (PBORI_PREFIX(DdManager) * dd, DdNode * f);
extern int Cudd_PrintTwoLiteralClauses (PBORI_PREFIX(DdManager) * dd, DdNode * f, char **names, FILE *fp);
extern int Cudd_ReadIthClause (DdTlcInfo * tlc, int i, DdHalfWord *var1, DdHalfWord *var2, int *phase1, int *phase2);
extern void Cudd_tlcInfoFree (DdTlcInfo * t);
extern int Cudd_DumpBlif (PBORI_PREFIX(DdManager) *dd, int n, DdNode **f, char **inames, char **onames, char *mname, FILE *fp, int mv);
extern int Cudd_DumpBlifBody (PBORI_PREFIX(DdManager) *dd, int n, DdNode **f, char **inames, char **onames, FILE *fp, int mv);
extern int Cudd_DumpDot (PBORI_PREFIX(DdManager) *dd, int n, DdNode **f, char **inames, char **onames, FILE *fp);
extern int Cudd_DumpDaVinci (PBORI_PREFIX(DdManager) *dd, int n, DdNode **f, char **inames, char **onames, FILE *fp);
extern int Cudd_DumpDDcal (PBORI_PREFIX(DdManager) *dd, int n, DdNode **f, char **inames, char **onames, FILE *fp);
extern int Cudd_DumpFactoredForm (PBORI_PREFIX(DdManager) *dd, int n, DdNode **f, char **inames, char **onames, FILE *fp);
extern DdNode * Cudd_bddConstrain (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *c);
extern DdNode * Cudd_bddRestrict (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *c);
extern DdNode * Cudd_bddNPAnd (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *c);
extern DdNode * Cudd_addConstrain (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *c);
extern DdNode ** Cudd_bddConstrainDecomp (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern DdNode * Cudd_addRestrict (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *c);
extern DdNode ** Cudd_bddCharToVect (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern DdNode * Cudd_bddLICompaction (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *c);
extern DdNode * Cudd_bddSqueeze (PBORI_PREFIX(DdManager) *dd, DdNode *l, DdNode *u);
extern DdNode * Cudd_bddMinimize (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *c);
extern DdNode * Cudd_SubsetCompress (PBORI_PREFIX(DdManager) *dd, DdNode *f, int nvars, int threshold);
extern DdNode * Cudd_SupersetCompress (PBORI_PREFIX(DdManager) *dd, DdNode *f, int nvars, int threshold);
#ifdef PBORI_FORCE_ORIGINAL_CUDD
extern MtrNode * Cudd_MakeTreeNode (PBORI_PREFIX(DdManager) *dd, unsigned int low, unsigned
int size, unsigned int type);
#endif
extern int Cudd_addHarwell (FILE *fp, PBORI_PREFIX(DdManager) *dd, DdNode **E, DdNode ***x, DdNode ***y, DdNode ***xn, DdNode ***yn_, int *nx, int *ny, int *m, int *n, int bx, int sx, int by, int sy, int pr);
extern PBORI_PREFIX(DdManager) *  PBORI_PREFIX(Cudd_Init) (unsigned int numVars, unsigned int numVarsZ, unsigned int numSlots, unsigned int cacheSize, unsigned long maxMemory);
  extern void  PBORI_PREFIX(Cudd_Quit) (PBORI_PREFIX(DdManager) *unique);
extern int Cudd_PrintLinear (PBORI_PREFIX(DdManager) *table);
extern int Cudd_ReadLinear (PBORI_PREFIX(DdManager) *table, int x, int y);
extern DdNode * Cudd_bddLiteralSetIntersection (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode * Cudd_addMatrixMultiply (PBORI_PREFIX(DdManager) *dd, DdNode *A, DdNode *B, DdNode **z, int nz);
extern DdNode * Cudd_addTimesPlus (PBORI_PREFIX(DdManager) *dd, DdNode *A, DdNode *B, DdNode **z, int nz);
extern DdNode * Cudd_addTriangle (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode **z, int nz);
extern DdNode * Cudd_addOuterSum (PBORI_PREFIX(DdManager) *dd, DdNode *M, DdNode *r, DdNode *c);
extern DdNode * Cudd_PrioritySelect (PBORI_PREFIX(DdManager) *dd, DdNode *R, DdNode **x, DdNode **y, DdNode **z, DdNode *Pi, int n, DdNode * (*)(PBORI_PREFIX(DdManager) *, int, DdNode **, DdNode **, DdNode **));
extern DdNode * Cudd_Xgty (PBORI_PREFIX(DdManager) *dd, int N, DdNode **z, DdNode **x, DdNode **y);
extern DdNode * Cudd_Xeqy (PBORI_PREFIX(DdManager) *dd, int N, DdNode **x, DdNode **y);
extern DdNode * Cudd_addXeqy (PBORI_PREFIX(DdManager) *dd, int N, DdNode **x, DdNode **y);
extern DdNode * Cudd_Dxygtdxz (PBORI_PREFIX(DdManager) *dd, int N, DdNode **x, DdNode **y, DdNode **z);
extern DdNode * Cudd_Dxygtdyz (PBORI_PREFIX(DdManager) *dd, int N, DdNode **x, DdNode **y, DdNode **z);
extern DdNode * Cudd_Inequality (PBORI_PREFIX(DdManager) * dd, int  N, int c, DdNode ** x, DdNode ** y);
extern DdNode * Cudd_Disequality (PBORI_PREFIX(DdManager) * dd, int  N, int c, DdNode ** x, DdNode ** y);
extern DdNode * Cudd_bddInterval (PBORI_PREFIX(DdManager) * dd, int  N, DdNode ** x, unsigned int lowerB, unsigned int upperB);
extern DdNode * Cudd_CProjection (PBORI_PREFIX(DdManager) *dd, DdNode *R, DdNode *Y);
extern DdNode * Cudd_addHamming (PBORI_PREFIX(DdManager) *dd, DdNode **xVars, DdNode **yVars, int nVars);
extern int Cudd_MinHammingDist (PBORI_PREFIX(DdManager) *dd, DdNode *f, int *minterm, int upperBound);
extern DdNode * Cudd_bddClosestCube (PBORI_PREFIX(DdManager) *dd, DdNode * f, DdNode *g, int *distance);
extern int Cudd_addRead (FILE *fp, PBORI_PREFIX(DdManager) *dd, DdNode **E, DdNode ***x, DdNode ***y, DdNode ***xn, DdNode ***yn_, int *nx, int *ny, int *m, int *n, int bx, int sx, int by, int sy);
extern int Cudd_bddRead (FILE *fp, PBORI_PREFIX(DdManager) *dd, DdNode **E, DdNode ***x, DdNode ***y, int *nx, int *ny, int *m, int *n, int bx, int sx, int by, int sy);
extern void PBORI_PREFIX(Cudd_Ref) (DdNode *n);
extern void PBORI_PREFIX(Cudd_RecursiveDeref) (PBORI_PREFIX(DdManager) *table, DdNode *n);
extern void Cudd_IterDerefBdd (PBORI_PREFIX(DdManager) *table, DdNode *n);
extern void Cudd_DelayedDerefBdd (PBORI_PREFIX(DdManager) * table, DdNode * n);
extern void PBORI_PREFIX(Cudd_RecursiveDerefZdd) (PBORI_PREFIX(DdManager) *table, DdNode *n);
extern void PBORI_PREFIX(Cudd_Deref (DdNode *node));
extern int PBORI_PREFIX(Cudd_CheckZeroRef) (PBORI_PREFIX(DdManager) *manager);
extern int Cudd_ReduceHeap (PBORI_PREFIX(DdManager) *table, Cudd_ReorderingType heuristic, int minsize);
extern int Cudd_ShuffleHeap (PBORI_PREFIX(DdManager) *table, int *permutation);
extern DdNode * Cudd_Eval (PBORI_PREFIX(DdManager) *dd, DdNode *f, int *inputs);
extern DdNode * Cudd_ShortestPath (PBORI_PREFIX(DdManager) *manager, DdNode *f, int *weight, int *support, int *length);
extern DdNode * Cudd_LargestCube (PBORI_PREFIX(DdManager) *manager, DdNode *f, int *length);
extern int Cudd_ShortestLength (PBORI_PREFIX(DdManager) *manager, DdNode *f, int *weight);
extern DdNode * Cudd_Decreasing (PBORI_PREFIX(DdManager) *dd, DdNode *f, int i);
extern DdNode * Cudd_Increasing (PBORI_PREFIX(DdManager) *dd, DdNode *f, int i);
extern int Cudd_EquivDC (PBORI_PREFIX(DdManager) *dd, DdNode *F, DdNode *G, DdNode *D);
extern int Cudd_bddLeqUnless (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode *D);
extern int Cudd_EqualSupNorm (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, CUDD_VALUE_TYPE tolerance, int pr);
extern DdNode * Cudd_bddMakePrime (PBORI_PREFIX(DdManager) *dd, DdNode *cube, DdNode *f);
extern DdNode * Cudd_bddMaximallyExpand(PBORI_PREFIX(DdManager) *dd, DdNode *lb, DdNode *ub, DdNode *f);
extern DdNode * Cudd_bddLargestPrimeUnate(PBORI_PREFIX(DdManager) *dd , DdNode *f, DdNode *phaseBdd);
extern double * Cudd_CofMinterm (PBORI_PREFIX(DdManager) *dd, DdNode *node);
extern DdNode * Cudd_SolveEqn (PBORI_PREFIX(DdManager) * bdd, DdNode *F, DdNode *Y, DdNode **G, int **yIndex, int n);
extern DdNode * Cudd_VerifySol (PBORI_PREFIX(DdManager) * bdd, DdNode *F, DdNode **G, int *yIndex, int n);
extern DdNode * Cudd_SplitSet (PBORI_PREFIX(DdManager) *manager, DdNode *S, DdNode **xVars, int n, double m);
extern DdNode * Cudd_SubsetHeavyBranch (PBORI_PREFIX(DdManager) *dd, DdNode *f, int numVars, int threshold);
extern DdNode * Cudd_SupersetHeavyBranch (PBORI_PREFIX(DdManager) *dd, DdNode *f, int numVars, int threshold);
extern DdNode * Cudd_SubsetShortPaths (PBORI_PREFIX(DdManager) *dd, DdNode *f, int numVars, int threshold, int hardlimit);
extern DdNode * Cudd_SupersetShortPaths (PBORI_PREFIX(DdManager) *dd, DdNode *f, int numVars, int threshold, int hardlimit);
extern void Cudd_SymmProfile (PBORI_PREFIX(DdManager) *table, int lower, int upper);
extern unsigned int Cudd_Prime (unsigned int p);
extern int Cudd_Reserve(PBORI_PREFIX(DdManager) *manager, int amount);
extern int Cudd_PrintMinterm (PBORI_PREFIX(DdManager) *manager, DdNode *node);
extern int Cudd_bddPrintCover (PBORI_PREFIX(DdManager) *dd, DdNode *l, DdNode *u);
extern int Cudd_PrintDebug (PBORI_PREFIX(DdManager) *dd, DdNode *f, int n, int pr);
extern int Cudd_DagSize (DdNode *node);
extern int Cudd_EstimateCofactor (PBORI_PREFIX(DdManager) *dd, DdNode * node, int i, int phase);
extern int Cudd_EstimateCofactorSimple (DdNode * node, int i);
extern int Cudd_SharingSize (DdNode **nodeArray, int n);
#ifdef PBORI_FORCE_ORIGINAL_CUDD
extern double Cudd_CountMinterm (PBORI_PREFIX(DdManager) *manager, DdNode *node, int nvars);
extern int Cudd_EpdCountMinterm (PBORI_PREFIX(DdManager) *manager, DdNode *node, int nvars, EpDouble *epd);
#endif
extern double Cudd_CountPath (DdNode *node);
extern double Cudd_CountPathsToNonZero (DdNode *node);
extern int Cudd_SupportIndices(PBORI_PREFIX(DdManager) * dd, DdNode * f, int **indices);
extern DdNode * Cudd_Support (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern int * Cudd_SupportIndex (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern int PBORI_PREFIX(Cudd_SupportSize) (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern int Cudd_VectorSupportIndices(PBORI_PREFIX(DdManager) * dd, DdNode ** F, int n, int **indices);
extern DdNode * Cudd_VectorSupport (PBORI_PREFIX(DdManager) *dd, DdNode **F, int n);
extern int * Cudd_VectorSupportIndex (PBORI_PREFIX(DdManager) *dd, DdNode **F, int n);
extern int Cudd_VectorSupportSize (PBORI_PREFIX(DdManager) *dd, DdNode **F, int n);
extern int Cudd_ClassifySupport (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode **common, DdNode **onlyF, DdNode **onlyG);
extern int Cudd_CountLeaves (DdNode *node);
extern int Cudd_bddPickOneCube (PBORI_PREFIX(DdManager) *ddm, DdNode *node, char *string);
extern DdNode * Cudd_bddPickOneMinterm (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **vars, int n);
extern DdNode ** Cudd_bddPickArbitraryMinterms (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **vars, int n, int k);
extern DdNode * Cudd_SubsetWithMaskVars (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **vars, int nvars, DdNode **maskVars, int mvars);
extern DdGen * Cudd_FirstCube (PBORI_PREFIX(DdManager) *dd, DdNode *f, int **cube, CUDD_VALUE_TYPE *value);
extern int Cudd_NextCube (DdGen *gen, int **cube, CUDD_VALUE_TYPE *value);
extern DdGen * Cudd_FirstPrime(PBORI_PREFIX(DdManager) *dd, DdNode *l, DdNode *u, int **cube);
extern int Cudd_NextPrime(DdGen *gen, int **cube);
extern DdNode * Cudd_bddComputeCube (PBORI_PREFIX(DdManager) *dd, DdNode **vars, int *phase, int n);
extern DdNode * Cudd_addComputeCube (PBORI_PREFIX(DdManager) *dd, DdNode **vars, int *phase, int n);
extern DdNode * Cudd_CubeArrayToBdd (PBORI_PREFIX(DdManager) *dd, int *array);
extern int Cudd_BddToCubeArray (PBORI_PREFIX(DdManager) *dd, DdNode *cube, int *array);
extern DdGen * Cudd_FirstNode (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode **node);
extern int Cudd_NextNode (DdGen *gen, DdNode **node);
extern int Cudd_GenFree (DdGen *gen);
extern int Cudd_IsGenEmpty (DdGen *gen);
extern DdNode * Cudd_IndicesToCube (PBORI_PREFIX(DdManager) *dd, int *array, int n);
extern void Cudd_PrintVersion (FILE *fp);
extern double Cudd_AverageDistance (PBORI_PREFIX(DdManager) *dd);
extern long Cudd_Random (void);
extern void Cudd_Srandom (long seed);
extern double Cudd_Density (PBORI_PREFIX(DdManager) *dd, DdNode *f, int nvars);
#endif /* PBORI_FORCE_ORIGINAL_CUDD*/

extern void PBORI_PREFIX(Cudd_OutOfMem) (long size);
extern int PBORI_PREFIX(Cudd_zddCount) (PBORI_PREFIX(DdManager) *zdd, DdNode *P);
extern double PBORI_PREFIX(Cudd_zddCountDouble) (PBORI_PREFIX(DdManager) *zdd, DdNode *P);
extern DdNode	* PBORI_PREFIX(Cudd_zddProduct) (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode	* PBORI_PREFIX(Cudd_zddUnateProduct) (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode	* PBORI_PREFIX(Cudd_zddWeakDiv) (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode	* PBORI_PREFIX(Cudd_zddDivide) (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode	* PBORI_PREFIX(Cudd_zddWeakDivF) (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode	* PBORI_PREFIX(Cudd_zddDivideF) (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g);
extern DdNode	* PBORI_PREFIX(Cudd_zddComplement) (PBORI_PREFIX(DdManager) *dd, DdNode *node);
#ifdef PBORI_FORCE_ORIGINAL_CUDD
extern MtrNode * Cudd_MakeZddTreeNode (PBORI_PREFIX(DdManager) *dd, unsigned int low, unsigned
int size, unsigned int type);
#endif
extern DdNode	* PBORI_PREFIX(Cudd_zddIsop) (PBORI_PREFIX(DdManager) *dd, DdNode *L, DdNode *U, DdNode **zdd_I);
extern DdNode	* PBORI_PREFIX(Cudd_bddIsop) (PBORI_PREFIX(DdManager) *dd, DdNode *L, DdNode *U);
extern DdNode	* PBORI_PREFIX(Cudd_MakeBddFromZddCover) (PBORI_PREFIX(DdManager) *dd, DdNode *node);
extern int PBORI_PREFIX(Cudd_zddDagSize) (DdNode *p_node);
extern double PBORI_PREFIX(Cudd_zddCountMinterm) (PBORI_PREFIX(DdManager) *zdd, DdNode *node, int path);
extern void PBORI_PREFIX(Cudd_zddPrintSubtable) (PBORI_PREFIX(DdManager) *table);
extern DdNode * PBORI_PREFIX(Cudd_zddPortFromBdd) (PBORI_PREFIX(DdManager) *dd, DdNode *B);
extern DdNode * PBORI_PREFIX(Cudd_zddPortToBdd) (PBORI_PREFIX(DdManager) *dd, DdNode *f);
extern int PBORI_PREFIX(Cudd_zddReduceHeap) (PBORI_PREFIX(DdManager) *table, Cudd_ReorderingType heuristic, int minsize);
extern int PBORI_PREFIX(Cudd_zddShuffleHeap) (PBORI_PREFIX(DdManager) *table, int *permutation);
extern DdNode * PBORI_PREFIX(Cudd_zddIte) (PBORI_PREFIX(DdManager) *dd, DdNode *f, DdNode *g, DdNode *h);
extern DdNode * PBORI_PREFIX(Cudd_zddUnion) (PBORI_PREFIX(DdManager) *dd, DdNode *P, DdNode *Q);
extern DdNode * PBORI_PREFIX(Cudd_zddIntersect) (PBORI_PREFIX(DdManager) *dd, DdNode *P, DdNode *Q);
extern DdNode * PBORI_PREFIX(Cudd_zddDiff) (PBORI_PREFIX(DdManager) *dd, DdNode *P, DdNode *Q);
extern DdNode * PBORI_PREFIX(Cudd_zddDiffConst) (PBORI_PREFIX(DdManager) *zdd, DdNode *P, DdNode *Q);
extern DdNode * PBORI_PREFIX(Cudd_zddSubset1) (PBORI_PREFIX(DdManager) *dd, DdNode *P, int var);
extern DdNode * PBORI_PREFIX(Cudd_zddSubset0) (PBORI_PREFIX(DdManager) *dd, DdNode *P, int var);
extern DdNode * PBORI_PREFIX(Cudd_zddChange) (PBORI_PREFIX(DdManager) *dd, DdNode *P, int var);
extern void PBORI_PREFIX(Cudd_zddSymmProfile) (PBORI_PREFIX(DdManager) *table, int lower, int upper);
extern int PBORI_PREFIX(Cudd_zddPrintMinterm) (PBORI_PREFIX(DdManager) *zdd, DdNode *node);
extern int PBORI_PREFIX(Cudd_zddPrintCover) (PBORI_PREFIX(DdManager) *zdd, DdNode *node);
extern int PBORI_PREFIX(Cudd_zddPrintDebug) (PBORI_PREFIX(DdManager) *zdd, DdNode *f, int n, int pr);
extern DdGen * PBORI_PREFIX(Cudd_zddFirstPath) (PBORI_PREFIX(DdManager) *zdd, DdNode *f, int **path);
extern int PBORI_PREFIX(Cudd_zddNextPath) (DdGen *gen, int **path);
extern char * PBORI_PREFIX(Cudd_zddCoverPathToString) (PBORI_PREFIX(DdManager) *zdd, int *path, char *str);
extern DdNode * PBORI_PREFIX(Cudd_zddSupport) (PBORI_PREFIX(DdManager) * dd, DdNode * f);
extern int PBORI_PREFIX(Cudd_zddDumpDot) (PBORI_PREFIX(DdManager) *dd, int n, DdNode **f, char **inames, char **onames, FILE *fp);
#ifndef PBORI_FORCE_ORIGINAL_CUDD
extern int Cudd_bddSetPiVar (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddSetPsVar (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddSetNsVar (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddIsPiVar (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddIsPsVar (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddIsNsVar (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddSetPairIndex (PBORI_PREFIX(DdManager) *dd, int index, int pairIndex);
extern int Cudd_bddReadPairIndex (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddSetVarToBeGrouped (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddSetVarHardGroup (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddResetVarToBeGrouped (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddIsVarToBeGrouped (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddSetVarToBeUngrouped (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddIsVarToBeUngrouped (PBORI_PREFIX(DdManager) *dd, int index);
extern int Cudd_bddIsVarHardGroup (PBORI_PREFIX(DdManager) *dd, int index);
#endif
/**AutomaticEnd***************************************************************/

#ifdef __cplusplus
} /* end of extern "C" */
#endif

/* For consistence: introduce prefixed macros */
#ifndef PBORI_FORCE_ORIGINAL_CUDD
#define pbori_Cudd_IsConstant Cudd_IsConstant
#define pbori_Cudd_Not Cudd_Not
#define pbori_Cudd_NotCond Cudd_NotCond
#define pbori_Cudd_Regular Cudd_Regular
#define pbori_Cudd_Complement Cudd_Complement
#define pbori_Cudd_IsComplement Cudd_IsComplement
#define pbori_Cudd_T Cudd_T
#define pbori_Cudd_E Cudd_E
#define pbori_Cudd_V Cudd_V
#define pbori_Cudd_ReadIndex Cudd_ReadIndex
#define pbori_Cudd_ForeachCube Cudd_ForeachCube
#define pbori_Cudd_ForeachPrime Cudd_ForeachPrime
#define pbori_Cudd_ForeachNode Cudd_ForeachNode
#define pbori_Cudd_zddForeachPath Cudd_zddForeachPath
#endif

#endif /* _CUDD */
