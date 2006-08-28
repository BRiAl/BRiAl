#ifndef EXTRA_FWD_HEADER
#define EXTRA_FWD_HEADER
#include <Cudd.h>

extern "C"{
/* the result of this operation is primes contained in the product of cubes */
extern DdNode *    Extra_zddPrimeProduct( DdManager *dd, DdNode * f, DdNode * g );

/* an alternative implementation of the cover product */
extern DdNode *    Extra_zddProductAlt( DdManager *dd, DdNode * f, DdNode * g );

/* returns the set of cubes pair-wise unate with the given cube */
extern DdNode *    Extra_zddCompatible( DdManager * dd, DdNode * zCover, DdNode * zCube );

/* a wrapper for the call to Extra_zddIsop() */
extern DdNode *    Extra_zddIsopCover( DdManager * dd, DdNode * F1, DdNode * F12 );
/* a wrapper for the call to Extra_zddIsopCover() and Extra_zddPrintCover() */
extern void        Extra_zddIsopPrintCover( DdManager * dd, DdNode * F1, DdNode * F12 );
/* a simple cover computation (not ISOP) */
extern DdNode *    Extra_zddSimplify( DdManager * dd, DdNode * F1, DdNode * F12 );

/* an alternative ISOP cover computation (faster than Extra_zddIsop()) */
extern DdNode *    Extra_zddIsopCoverAlt( DdManager * dd, DdNode * F1, DdNode * F12 );

/* count the number of cubes in the ISOP without building the ISOP as a ZDD */
extern int         Extra_zddIsopCubeNum( DdManager * dd, DdNode * F1, DdNode * F12 );


/* computes the disjoint cube cover produced by the bdd paths */
extern DdNode *    Extra_zddDisjointCover( DdManager * dd, DdNode * F );
/* performs resolution on the set of clauses (S) w.r.t. variables in zdd Vars */
extern DdNode *    Extra_zddResolve( DdManager * dd, DdNode * S, DdNode * Vars );
/* cubes from zC that are not contained by cubes from zD over area bA */
extern DdNode *    Extra_zddNotContainedCubesOverArea( DdManager * dd, DdNode * zC, DdNode * zD, DdNode * bA );
extern DdNode *     extraZddNotContainedCubesOverArea( DdManager * dd, DdNode * zC, DdNode * zD, DdNode * bA );
/* finds cofactors of the cover w.r.t. the top-most variable without creating new nodes */
/* selects one cube from a ZDD representing the cube cover */
extern DdNode *    Extra_zddSelectOneCube( DdManager * dd, DdNode * zS );

/* selects one subset from a ZDD representing the set of subsets */
extern DdNode *    Extra_zddSelectOneSubset( DdManager * dd, DdNode * zS );

/* checks unateness of the cover */
extern int         Extra_zddCheckUnateness( DdManager * dd, DdNode * zCover );
/* computes the BDD of the area covered by the max number of cubes in a ZDD. */
extern DdNode *    Extra_zddGetMostCoveredArea( DdManager * dd, DdNode * zC, int * nOverlaps );


/*=== extraZddExor.c ==============================================================*/

/* computes the Exclusive-OR-type union of two cube sets */
extern DdNode *    Extra_zddUnionExor( DdManager * dd, DdNode * S, DdNode * T );
/* given two sets of cubes, computes the set of pair-wise supercubes */
extern DdNode *    Extra_zddSupercubes( DdManager *dd, DdNode * zA, DdNode * zB );

/* selects cubes from zA that have a distance-1 cube in zB */
extern DdNode *    Extra_zddSelectDist1Cubes( DdManager *dd, DdNode * zA, DdNode * zB );

/* computes the set of fast ESOP covers for the multi-output function */
extern int         Extra_zddFastEsopCoverArray( DdManager * dd, DdNode ** bFs, DdNode ** zCs, int nFs );
/* computes a fast ESOP cover for the single-output function */
//extern DdNode *    Extra_zddFastEsopCover( DdManager * dd, DdNode * bF, st_table * Visited, int * pnCubes );

/*=== extraZddFactor.c ================================================================*/

/* counting the number of literals in the factored form */
extern int         Extra_bddFactoredFormLiterals( DdManager * dd, DdNode * bOnSet, DdNode * bOnDcSet );
extern DdNode *    Extra_zddFactoredFormLiterals( DdManager * dd, DdNode * zCover );
extern DdNode *    Extra_zddLFLiterals( DdManager * dd, DdNode * zCover, DdNode * zCube );
/* computing a quick divisor */
extern DdNode *    Extra_zddQuickDivisor( DdManager * dd, DdNode * zCover );
extern DdNode *    Extra_zddLevel0Kernel( DdManager * dd, DdNode * zCover );
/* division with quotient and remainder */
extern void        Extra_zddDivision( DdManager * dd, DdNode * zCover, DdNode * zDiv, DdNode ** zQuo, DdNode ** zRem );
/* the common cube */
extern DdNode *    Extra_zddCommonCubeFast( DdManager * dd, DdNode * zCover );
/* the cube of literals that occur more than once */
extern DdNode *    Extra_zddMoreThanOnceCubeFast( DdManager * dd, DdNode * zCover );
/* making the cover cube-free */
extern DdNode *    Extra_zddMakeCubeFree( DdManager * dd, DdNode * zCover, int iZVar );
/* testing whether the cover is cube-free */
extern int         Extra_zddTestCubeFree( DdManager * dd, DdNode * zCover );

/* counts the number of literals in the simple cover */
extern int         Extra_zddCountLiteralsSimple( DdManager * dd, DdNode * zCover );
/* tests whether the cover contains more than one cube */
extern int         Extra_zddMoreThanOneCube( DdManager * dd, DdNode * zCover );
/* the cube from levels */
extern DdNode *    Extra_zddCombinationFromLevels( DdManager * dd, int * pLevels, int nVars );
/* determines common literals */
extern int         Extra_zddCommonLiterals( DdManager * dd, DdNode * zCover, int iZVar, int * pLevels, int * pLiterals );
/* determines the set of literals that occur more than once */
extern int         Extra_zddMoreThanOneLiteralSet( DdManager * dd, DdNode * zCover, int StartLevel, int * pVars, int * pCounters );
/* tests whether the given literal literal occurs more than once */
extern int         Extra_zddMoreThanOneLiteral( DdManager * dd, DdNode * zCover, int iZVar );


/*=== extraZddGraph.c ==============================================================*/

/* construct the set of cliques */
extern DdNode *    Extra_zddCliques( DdManager *dd, DdNode * G, int fMaximal ); 
/* construct the set of all maximal cliques */
extern DdNode *    Extra_zddMaxCliques( DdManager *dd, DdNode * G ); 
/* incrementally contruct the set of cliques */
extern DdNode *    Extra_zddIncremCliques( DdManager *dd, DdNode * G, DdNode * C ); 





extern DdNode *    Extra_zddIsopCoverAllVars( DdManager * dd, DdNode * F1, DdNode * F12 );


extern DdNode *    Extra_zddIsopCoverUnateVars( DdManager * dd, DdNode * F1, DdNode * F12 );


/* computes an ISOP cover with a random ordering of variables */
extern DdNode *    Extra_zddIsopCoverRandom( DdManager * dd, DdNode * F1, DdNode * F12 );

extern DdNode *    Extra_zddIsopCoverReduced( DdManager * dd, DdNode * F1, DdNode * F12 );


/*=== extraZddLitCount.c ==============================================================*/

/* count the number of times each variable occurs in the combinations */
extern int *       Extra_zddLitCount( DdManager * dd, DdNode * Set );
/* count the number of literals in one ZDD combination */
extern int         Extra_zddLitCountComb( DdManager * dd, DdNode * zComb );

/*=== extraZddMaxMin.c ==============================================================*/

/* maximal/minimimal */
extern DdNode *    Extra_zddMaximal( DdManager *dd, DdNode * S );

extern DdNode *    Extra_zddMinimal( DdManager *dd, DdNode * S );

/* maximal/minimal of the union of two sets of subsets */
extern DdNode *    Extra_zddMaxUnion( DdManager *dd, DdNode * S, DdNode * T );

extern DdNode *    Extra_zddMinUnion( DdManager *dd, DdNode * S, DdNode * T );

/* dot/cross products */
extern DdNode *    Extra_zddDotProduct( DdManager *dd, DdNode * S, DdNode * T );

extern DdNode *    Extra_zddExorProduct( DdManager *dd, DdNode * S, DdNode * T );

extern DdNode *    Extra_zddCrossProduct( DdManager *dd, DdNode * S, DdNode * T );

extern DdNode *    Extra_zddMaxDotProduct( DdManager *dd, DdNode * S, DdNode * T );


/*=== extraZddMisc.c ==============================================================*/

/* create the combination composed of a single ZDD variable */
extern DdNode *    Extra_zddVariable( DdManager * dd, int iVar );
/* build a ZDD for a combination of variables */
extern DdNode *    Extra_zddCombination( DdManager *dd, int* VarValues, int nVars );

/* the set of all possible combinations of the given set of variables */
extern DdNode *    Extra_zddUniverse( DdManager * dd, DdNode * VarSet );

/* build the set of all tuples of K variables out of N */
extern DdNode *    Extra_zddTuples( DdManager * dd, int K, DdNode * zVarsN );

/* build the set of all tuples of K variables out of N from the BDD cube */
extern DdNode *    Extra_zddTuplesFromBdd( DdManager * dd, int K, DdNode * bVarsN );

/* convert the set of singleton combinations into one combination */
extern DdNode *    Extra_zddSinglesToComb( DdManager * dd, DdNode * Singles );

/* returns the set of combinations containing the max/min number of elements */
extern DdNode *    Extra_zddMaximum( DdManager * dd, DdNode * S, int * nVars );

extern DdNode *    Extra_zddMinimum( DdManager * dd, DdNode * S, int * nVars );

/* returns the random set of k combinations of n elements with average density d */
extern DdNode *    Extra_zddRandomSet( DdManager * dd, int n, int k, double d );
/* other utilities */
extern DdNode *    Extra_zddCoveredByArea( DdManager *dd, DdNode * zC, DdNode * bA );

extern DdNode *    Extra_zddNotCoveredByCover( DdManager *dd, DdNode * zC, DdNode * zD );

extern DdNode *    Extra_zddOverlappingWithArea( DdManager *dd, DdNode * zC, DdNode * bA );

extern DdNode *    Extra_zddConvertToBdd( DdManager *dd, DdNode * zC );

extern DdNode *    Extra_zddConvertToBddUnate( DdManager *dd, DdNode * zC );

extern DdNode *    Extra_zddConvertEsopToBdd( DdManager *dd, DdNode * zC );

extern DdNode *    Extra_zddConvertToBddAndAdd( DdManager *dd, DdNode * zC, DdNode * bA );

extern DdNode *    Extra_zddSingleCoveredArea( DdManager *dd, DdNode * zC );

extern DdNode *    Extra_zddConvertBddCubeIntoZddCube( DdManager *dd, DdNode * bCube );

/*=== extraZddPermute.c ==============================================================*/

/* quantifications */
extern DdNode *    Extra_zddExistAbstract( DdManager *manager, DdNode * F, DdNode * cube );

/* changes the value of several variables in the ZDD */
extern DdNode *    Extra_zddChangeVars( DdManager *manager, DdNode * F, DdNode * cube );

/* permutes variables in ZDD */
extern DdNode *    Extra_zddPermute ( DdManager *dd, DdNode * N, int *permut );
/* computes combinations in F with vars in Cube having the negative polarity */
extern DdNode *    Extra_zddCofactor0( DdManager * dd, DdNode * f, DdNode * cube );

/* computes combinations in F with vars in Cube having the positive polarity */
extern DdNode *    Extra_zddCofactor1( DdManager * dd, DdNode * f, DdNode * cube, int fIncludeVars );


/*=== extraZddSubSup.c ==============================================================*/

/* subset/supset operations */
extern DdNode *    Extra_zddSubSet   ( DdManager *dd, DdNode * X, DdNode * Y );

extern DdNode *    Extra_zddSupSet   ( DdManager *dd, DdNode * X, DdNode * Y );

extern DdNode *    Extra_zddNotSubSet( DdManager *dd, DdNode * X, DdNode * Y );

extern DdNode *    Extra_zddNotSupSet( DdManager *dd, DdNode * X, DdNode * Y );

extern DdNode *    Extra_zddMaxNotSupSet( DdManager *dd, DdNode * X, DdNode * Y );

/* check whether the empty combination belongs to the set of subsets */
extern int         Extra_zddEmptyBelongs ( DdManager *dd, DdNode* zS  );
/* check whether the set consists of one subset only */
extern int         Extra_zddIsOneSubset( DdManager *dd, DdNode* zS  );

}


#endif
