// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author 
 * @date 
 *
 * 
 *
 * @par Copyright:
 *   (c) by
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
 * Revision 1.1  2007/07/06 14:04:21  dreyer
 * ADD: newly written C++_interface for Cudd
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef myzdd_h_
#define myzdd_h_

// include basic definitions
#include "pbori_defs.h"

#include "cuddObj.hh"

#include <boost/shared_ptr.hpp>

#include <boost/weak_ptr.hpp>

#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE_PBORI


inline void
mydefaultError(
  string message)
{
    cerr << message << "\n";
    exit(1);

} // defaultError


class mycapsule {
public:
  DdManager *manager;
  PFC errorHandler;
  int verbose;
  long int ref;
  mycapsule(unsigned int numVars = 0,
            unsigned int numVarsZ = 0,
            unsigned int numSlots = CUDD_UNIQUE_SLOTS,
            unsigned int cacheSize = CUDD_CACHE_SLOTS,
            unsigned long maxMemory = 0)
  :    ref(0)
{

    manager = Cudd_Init(numVars,numVarsZ,numSlots,cacheSize,maxMemory);
    errorHandler = mydefaultError;
    verbose = 0;		// initially terse
  }
  ~mycapsule(){
    erase();
//     int retval = Cudd_CheckZeroRef(manager);
//     if (retval != 0) {
//       cerr << retval << " unexpected non-zero reference counts\n";
//     } else if (verbose) {
//       cerr << "All went well\n";
//     }
//     Cudd_Quit(manager);
  }
  void erase() {

    if (--(ref) == 0){
      int retval = Cudd_CheckZeroRef(manager);
      if (retval != 0) {
        cerr << retval << " unexpected non-zero reference counts\n";
      } else if (verbose) {
        cerr << "All went well\n";
      }
      Cudd_Quit(manager);
    }
  }
};

inline
void intrusive_ptr_add_ref(mycapsule* p){
    ++(p->ref);
}

inline
void intrusive_ptr_release(mycapsule * p) {
  p->erase();
}

// New implementation
class CCuddZDD;



// ---------------------------------------------------------------------------
// Class CCuddZDD
// ---------------------------------------------------------------------------

class myDD {
    friend class CCuddZDD;


public: // temporarily

  //  typedef boost::shared_ptr<mycapsule> mgr_ptr;
  typedef boost::intrusive_ptr<mycapsule> mgr_ptr;


  mgr_ptr ddMgr;
  DdNode *node;

    inline DdManager * checkSameManager(const myDD &other) const;
    inline void checkReturnValue(const DdNode *result) const;
    inline void checkReturnValue(const int result, const int expected = 1)
	const;
public:
    myDD(mgr_ptr ddManager, DdNode *ddNode);
    myDD();
    myDD(const myDD &from);

  // temporarily
  //  myDD(const DD &from);

    mgr_ptr manager() const;

  DdManager* getManager() const;
    inline DdNode * getNode() const{  return node; }
    int nodeCount() const;
    unsigned int NodeReadIndex() const;

}; // myDD

class CCuddZDD : public myDD {
    friend class CCuddInterface;
public:
  typedef CCuddZDD self;
  typedef int idx_type;
  typedef myDD base;
  typedef base::mgr_ptr mgr_ptr;

  CCuddZDD(mgr_ptr bddManager, DdNode *bddNode): base(bddManager,bddNode) {}

//   CCuddZDD(boost::shared_ptr<mycapsule> bddManager, DdNode *bddNode): base(bddManager,bddNode) {}

  CCuddZDD(const CCuddInterface& bddManager, DdNode *bddNode);//inlined below
  CCuddZDD(CCuddInterface* bddManager, DdNode *bddNode);//inlined below

  CCuddZDD(): base() {}
  CCuddZDD(const self &from): base(from) {}

  // temporarily
  //  CCuddZDD(const ZDD &from): myDD(from) {}

  ~CCuddZDD();                     // inlined below
  self& operator=(const self& right); // inlined below

  void print(int nvars, int verbosity = 1) const;  // inlined below

  bool operator==(const self& other) const {
    checkSameManager(other);
    return node == other.node;
  }

  bool operator!=(const self& other) const { return !(*this == other); }

  bool operator<=(const self& other) const {
    return apply(Cudd_zddDiffConst, other).isZero(); 
  }
  bool operator>=(const self& other) const {
    return (other <= *this); 
  }
  bool operator<(const self& rhs) const { 
    return (*this != rhs) && (*this <= rhs);  }
  bool operator>(const self& other) const { 
    return (*this != other) && (*this >= other); }

  bool isZero() const { return node == Cudd_ReadZero(getManager()); }


  self operator*(const self& other) const          { return Intersect(other); }
  self& operator*=(const self& other)       { return *this = (*this * other); }
  self operator&(const self& other) const          { return Intersect(other); }
  self& operator&=(const self& other)       { return *this = (*this & other); }
  self operator+(const self& other) const              { return Union(other); }
  self& operator+=(const self& other)       { return *this = (*this + other); }
  self operator|(const self& other) const              { return Union(other); }
  self& operator|=(const self& other)       { return *this = (*this | other); }
  self operator-(const self& other) const               { return Diff(other); }
  self& operator-=(const self& other)       { return *this = (*this - other); }
  int Count() const                         { return memApply(Cudd_zddCount); }
  double CountDouble() const          { return memApply(Cudd_zddCountDouble); }

  self Product(const self& g) const       { return apply(Cudd_zddProduct, g); }
  self UnateProduct(const self& g) const { 
    return apply(Cudd_zddUnateProduct, g); }
  self WeakDiv(const self& g) const       { return apply(Cudd_zddWeakDiv, g); } 
  self Divide(const self& g) const         { return apply(Cudd_zddDivide, g); }
  self WeakDivF(const self& g) const     { return apply(Cudd_zddWeakDivF, g); } 
  self DivideF(const self& g) const       { return apply(Cudd_zddDivideF, g); } 

  double CountMinterm(int path) const;
  //   BDD PortToBdd() const;
  self Ite(const self& g, const self& h) const { 
    return apply(Cudd_zddIte, g, h); }
  self Union(const self& Q) const           { return apply(Cudd_zddUnion, Q); }
  self Intersect(const self& Q) const   { return apply(Cudd_zddIntersect, Q); }
  self Diff(const self& Q) const             { return apply(Cudd_zddDiff, Q); }
  self DiffConst(const self& Q) const   { return apply(Cudd_zddDiffConst, Q); }

  self Subset1(int var) const           { return apply(Cudd_zddSubset1, var); }
  self Subset0(int var) const           { return apply(Cudd_zddSubset0, var); }
  self Change(int var) const             { return apply(Cudd_zddChange, var); }

  void PrintMinterm() const                    { apply(Cudd_zddPrintMinterm); }

  void PrintCover() const                        { apply(Cudd_zddPrintCover); }

protected:
  typedef DD_CTFP binary_function;
  typedef DdNode * (*binary_int_function)(DdManager *, DdNode *, int);
  typedef DdNode* (*ternary_function)(DdManager*, DdNode*, DdNode*, DdNode*);

  typedef int (*int_unary_function)(DdManager *, DdNode *);

  typedef DdNode* node_type;

  self apply(binary_function func, const self& rhs) const {
    checkSameManager(rhs);
    return checkedResult(func(getManager(), getNode(), rhs.getNode()));
  }

  self apply(binary_int_function func, idx_type idx) const {
    return checkedResult(func(getManager(), getNode(), idx) );
  }

  self apply(ternary_function func, const self& first, const self& second) const {
    checkSameManager(first);
    checkSameManager(second);
    return checkedResult(func(getManager(), getNode(), 
                              first.getNode(), second.getNode()) );
  }

  idx_type apply(int_unary_function func) const {
    return checkedResult(func(getManager(), getNode()) );
  }

  self checkedResult(node_type result) const {
    checkReturnValue(result);
    return self(manager(), result);
  }

  idx_type checkedResult(idx_type result) const {
    checkReturnValue(result);
    return result;
  }

  template <class ResultType>
  ResultType memApply(ResultType (*func)(DdManager *, DdNode *)) const {
    return memChecked(func(getManager(), getNode()) );
  }

  template <class ResultType>
  ResultType memChecked(ResultType result) const {
    checkReturnValue(result != (ResultType) CUDD_OUT_OF_MEM);
    return result;
  }

}; //CCuddZDD





class CCuddInterface {
  friend class MyDD;
  friend class CCuddZDD;

public:
    struct capsule {
	DdManager *manager;
	PFC errorHandler;
	int verbose;
	int ref;
    };
  //  capsule *p;

  typedef  myDD::mgr_ptr mgr_ptr;
  mgr_ptr p;

public:
    CCuddInterface(
      unsigned int numVars = 0,
      unsigned int numVarsZ = 0,
      unsigned int numSlots = CUDD_UNIQUE_SLOTS,
      unsigned int cacheSize = CUDD_CACHE_SLOTS,
      unsigned long maxMemory = 0);
    CCuddInterface(const CCuddInterface& x);

  CCuddInterface(mgr_ptr rhs): p(rhs) {};

    ~CCuddInterface();
    PFC setHandler(PFC newHandler);
    PFC getHandler() const;

    DdManager *getManager() const {return p->manager;}


    inline void makeVerbose() {p->verbose = 1;}
    inline void makeTerse() {p->verbose = 0;}
    inline int isVerbose() const {return p->verbose;}
    inline void checkReturnValue(const DdNode *result) const;
    inline void checkReturnValue(const int result) const;
    CCuddInterface& operator=(const CCuddInterface& right);
    void info() const;

    CCuddZDD zddVar(int index) const;
    CCuddZDD zddOne(int i) const;
    CCuddZDD zddZero() const;

    void AutodynEnableZdd(Cudd_ReorderingType method);
    void AutodynDisableZdd();
    int ReorderingStatusZdd(Cudd_ReorderingType * method) const;
    int zddRealignmentEnabled() const;
    void zddRealignEnable();
    void zddRealignDisable();

    unsigned int ReadCacheSlots() const;
    double ReadCacheUsedSlots() const;
    double ReadCacheLookUps() const;
    double ReadCacheHits() const;
    unsigned int ReadMinHit() const;
    void SetMinHit(unsigned int hr);
    unsigned int ReadLooseUpTo() const;
    void SetLooseUpTo(unsigned int lut);
    unsigned int ReadMaxCache() const;
    unsigned int ReadMaxCacheHard() const;
    void SetMaxCacheHard(unsigned int mc);
    int ReadSize() const;
    int ReadZddSize() const;
    unsigned int ReadSlots() const;
    unsigned int ReadKeys() const;
    unsigned int ReadDead() const;
    unsigned int ReadMinDead() const;
    int ReadReorderings() const;
    long ReadReorderingTime() const;
    int ReadGarbageCollections() const;
    long ReadGarbageCollectionTime() const;
    int ReadSiftMaxVar() const;
    void SetSiftMaxVar(int smv);
    int ReadSiftMaxSwap() const;
    void SetSiftMaxSwap(int sms);
    double ReadMaxGrowth() const;
    void SetMaxGrowth(double mg);

    MtrNode * ReadZddTree() const;
    void SetZddTree(MtrNode * tree);
    void FreeZddTree();

    int ReadPermZdd(int i) const;

    int ReadInvPermZdd(int i) const;

    int GarbageCollectionEnabled() const;
    void EnableGarbageCollection();
    void DisableGarbageCollection();
    int DeadAreCounted() const;
    void TurnOnCountDead();
    void TurnOffCountDead();
    int ReadRecomb() const;
    void SetRecomb(int recomb);
    int ReadSymmviolation() const;
    void SetSymmviolation(int symmviolation);
    int ReadArcviolation() const;
    void SetArcviolation(int arcviolation);
    int ReadPopulationSize() const;
    void SetPopulationSize(int populationSize);
    int ReadNumberXovers() const;
    void SetNumberXovers(int numberXovers);
    unsigned long ReadMemoryInUse() const;
    long ReadPeakNodeCount() const;

    long zddReadNodeCount() const;
    void AddHook(DD_HFP f, Cudd_HookType where);
    void RemoveHook(DD_HFP f, Cudd_HookType where);
    int IsInHook(DD_HFP f, Cudd_HookType where) const;
    void EnableReorderingReporting();
    void DisableReorderingReporting();
    int ReorderingReporting();
    int ReadErrorCode() const;
    void ClearErrorCode();
    FILE *ReadStdout() const;
    void SetStdout(FILE *);
    FILE *ReadStderr() const;
    void SetStderr(FILE *);
    unsigned int ReadNextReordering() const;
    double ReadSwapSteps() const;
    unsigned int ReadMaxLive() const;
    void SetMaxLive(unsigned int);
    unsigned long ReadMaxMemory() const;
    void SetMaxMemory(unsigned long);

    void DebugCheck();
    void CheckKeys();
    MtrNode * MakeTreeNode(unsigned int low, unsigned int size, unsigned int type);
    // void Harwell(FILE * fp, ADD* E, ADD** x, ADD** y, ADD** xn, ADD** yn_, int * nx, int * ny, int * m, int * n, int bx, int sx, int by, int sy, int pr);
    void PrintLinear();
    int ReadLinear(int x, int y);

    void ReduceHeap(Cudd_ReorderingType heuristic, int minsize);
    void ShuffleHeap(int * permutation);
    void SymmProfile(int lower, int upper) const;
    unsigned int Prime(unsigned int pr) const;
    int SharingSize(DD* nodes, int n) const;

    int NextNode(DdGen * gen, BDD * nnode);

    void PrintVersion(FILE * fp) const;
    double AverageDistance() const;
    long Random();
    void Srandom(long seed);
    MtrNode * MakeZddTreeNode(unsigned int low, unsigned int size, unsigned int type);
    void zddPrintSubtable() const;
    void zddReduceHeap(Cudd_ReorderingType heuristic, int minsize);
    void zddShuffleHeap(int * permutation);
    void zddSymmProfile(int lower, int upper) const;


}; // CCuddInterface



// ---------------------------------------------------------------------------
// Members of class CCuddInterface
// ---------------------------------------------------------------------------

inline
CCuddInterface::CCuddInterface(
  unsigned int numVars,
  unsigned int numVarsZ,
  unsigned int numSlots,
  unsigned int cacheSize,
  unsigned long maxMemory)  :
  p (new mycapsule(numVars,numVarsZ,numSlots,cacheSize,maxMemory))
{
//   p = new capsule;
//     p->manager = Cudd_Init(numVars,numVarsZ,numSlots,cacheSize,maxMemory);
//     p->errorHandler = mydefaultError;
//     p->verbose = 0;		// initially terse
//     p->ref = 1;

} // CCuddInterface::CCuddInterface

inline
CCuddInterface::CCuddInterface(
               const CCuddInterface& x): p(x.p)
{
//     p = x.p;
//     x.p->ref++;

} // CCuddInterface::CCuddInterface

inline
CCuddInterface::~CCuddInterface()
{
//     if (--p->ref == 0) {
// 	int retval = Cudd_CheckZeroRef(getManager());
// 	if (retval != 0) {
// 	    cerr << retval << " unexpected non-zero reference counts\n";
// 	} else if (p->verbose) {
// 	    cerr << "All went well\n";
// 	}
// 	Cudd_Quit(getManager());
// 	delete p;
//     }

} // CCuddInterface::~CCuddInterface

inline
CCuddInterface&
CCuddInterface::operator=(
  const CCuddInterface& right)
{
  p = right.p;
//     right.p->ref++;
//     if (--p->ref == 0) {	// disconnect self
// 	int retval = Cudd_CheckZeroRef(getManager());
// 	if (retval != 0) {
// 	    cerr << retval << " unexpected non-zero reference counts\n";
// 	}
// 	Cudd_Quit(getManager());
// 	delete p;
//     }
//     p = right.p;
    return *this;

} // CCuddInterface::operator=

inline
PFC
CCuddInterface::setHandler(
  PFC newHandler)
{
    PFC oldHandler = p->errorHandler;
    p->errorHandler = newHandler;
    return oldHandler;

} // Cudd::setHandler

inline
PFC
CCuddInterface::getHandler() const
{
    return p->errorHandler;

} // CCuddInterface::getHandler


inline void
CCuddInterface::checkReturnValue(
  const DdNode *result) const
{
    if (result == 0) {
	if (Cudd_ReadErrorCode(getManager()) == CUDD_MEMORY_OUT) {
	    p->errorHandler("Out of memory.");
	} else {
	    p->errorHandler("Internal error.");
	}
    }

} // Cudd::checkReturnValue


inline void
CCuddInterface::checkReturnValue(
  const int result) const
{
    if (result == 0) {
	if (Cudd_ReadErrorCode(getManager()) == CUDD_MEMORY_OUT) {
	    p->errorHandler("Out of memory.");
	} else {
	    p->errorHandler("Internal error.");
	}
    }

} // Cudd::checkReturnValue


inline void
CCuddInterface::info() const
{
    cout.flush();
    int retval = Cudd_PrintInfo(getManager(),stdout);
    this->checkReturnValue(retval);

} // Cudd::info


inline CCuddZDD
CCuddInterface::zddVar(
  int index) const
{
    DdNode *result = Cudd_zddIthVar(getManager(),index);
    this->checkReturnValue(result);
    return CCuddZDD(p, result);

} // CCuddInterface::zddVar


inline CCuddZDD
CCuddInterface::zddOne(
  int i) const
{
    DdNode *result = Cudd_ReadZddOne(getManager(),i);
    this->checkReturnValue(result);
    return CCuddZDD(p, result);

} // CCuddInterface::zddOne


inline CCuddZDD
CCuddInterface::zddZero() const
{
    DdNode *result = Cudd_ReadZero(getManager());
    this->checkReturnValue(result);
    return CCuddZDD(p, result);

} // CCuddInterface::zddZero



// inline void
// CCuddInterface::zddVarsFromBddVars(
//   int multiplicity)
// {
//     int result = Cudd_zddVarsFromBddVars(getManager(), multiplicity);
//     this->checkReturnValue(result);

// } // Cudd::zddVarsFromBddVars



inline void
CCuddInterface::AutodynEnableZdd(
  Cudd_ReorderingType method)
{
    Cudd_AutodynEnableZdd(getManager(), method);

} // Cudd::AutodynEnableZdd


inline void
CCuddInterface::AutodynDisableZdd()
{
    Cudd_AutodynDisableZdd(getManager());

} // Cudd::AutodynDisableZdd


inline int
CCuddInterface::ReorderingStatusZdd(
  Cudd_ReorderingType * method) const
{
    return Cudd_ReorderingStatusZdd(getManager(), method);

} // Cudd::ReorderingStatusZdd


inline int
CCuddInterface::zddRealignmentEnabled() const
{
    return Cudd_zddRealignmentEnabled(getManager());

} // CCuddInterface::zddRealignmentEnabled


inline void
CCuddInterface::zddRealignEnable()
{
    Cudd_zddRealignEnable(getManager());

} // CCuddInterface::zddRealignEnable


inline void
CCuddInterface::zddRealignDisable()
{
    Cudd_zddRealignDisable(getManager());

} // CCuddInterface::zddRealignDisable


inline FILE *
CCuddInterface::ReadStdout() const
{
    return Cudd_ReadStdout(getManager());

} // Cudd::ReadStdout

inline void
CCuddInterface::SetStdout(FILE *fp)
{
    Cudd_SetStdout(getManager(), fp);

} // Cudd::SetStdout


inline FILE *
CCuddInterface::ReadStderr() const
{
    return Cudd_ReadStderr(getManager());

} // Cudd::ReadStderr


inline void
CCuddInterface::SetStderr(FILE *fp)
{
    Cudd_SetStderr(getManager(), fp);

} // Cudd::SetStderr



// ---------------------------------------------------------------------------
// Members of class myDD
// ---------------------------------------------------------------------------

inline
myDD::myDD(mgr_ptr ddManager, DdNode *ddNode) {
    ddMgr = ddManager;
    node = ddNode;
    if (node != 0) Cudd_Ref(node);
    if (ddMgr->verbose) {
        cout << "Standard DD constructor for node " << hex << long(node) <<
	    " ref = " << Cudd_Regular(node)->ref << "\n";
    }

} // DD::DD

inline
myDD::myDD() {
  ddMgr = mgr_ptr();
    node = 0;

} // DD::DD

inline
myDD::myDD(const myDD &from) {
    ddMgr = from.ddMgr;
    node = from.node;
    if (node != 0) {
	Cudd_Ref(node);
	if (ddMgr->verbose) {
	    cout << "Copy DD constructor for node " << hex << long(node) <<
		" ref = " << Cudd_Regular(node)->ref << "\n";
	}
    }

} // DD::DD

// inline
// myDD::myDD(const MyDD &from) {
//   ddMgr = from.manager();
//   node = from.getNode();
//     if (node != 0) {
// 	Cudd_Ref(node);
// 	if (ddMgr->isVerbose()) {
// 	    cout << "Copy DD constructor for node " << hex << long(node) <<
// 		" ref = " << Cudd_Regular(node)->ref << "\n";
// 	}
//     }

// } // DD::DD
inline DdManager *
myDD::checkSameManager(
  const myDD &other) const
{
  ///@todo assertion?
  DdManager *mgr = getManager();
  if (mgr != other.getManager()) {
	ddMgr->errorHandler("Operands come from different manager.");
    }
  
  return mgr;

} // DD::checkSameManager


inline void
myDD::checkReturnValue(
  const DdNode *result) const
{
    if (result == 0) {
	DdManager *mgr = getManager();
	Cudd_ErrorType errType = Cudd_ReadErrorCode(mgr);
	switch (errType) {
	CUDD_MEMORY_OUT:
	    ddMgr->errorHandler("Out of memory.");
	    break;
	CUDD_TOO_MANY_NODES:
	    break;
	CUDD_MAX_MEM_EXCEEDED:
	    ddMgr->errorHandler("Maximum memory exceeded.");
	    break;
	CUDD_INVALID_ARG:
	    ddMgr->errorHandler("Invalid argument.");
	    break;
	CUDD_INTERNAL_ERROR:
	    ddMgr->errorHandler("Internal error.");
	    break;
	CUDD_NO_ERROR:
	default:
	    ddMgr->errorHandler("Unexpected error.");
	    break;
	}
    }

} // DD::checkReturnValue


inline void
myDD::checkReturnValue(
  const int result,
  const int expected) const
{
    if (result != expected) {
	DdManager *mgr = getManager();
	Cudd_ErrorType errType = Cudd_ReadErrorCode(mgr);
	switch (errType) {
	CUDD_MEMORY_OUT:
	    ddMgr->errorHandler("Out of memory.");
	    break;
	CUDD_TOO_MANY_NODES:
	    break;
	CUDD_MAX_MEM_EXCEEDED:
	    ddMgr->errorHandler("Maximum memory exceeded.");
	    break;
	CUDD_INVALID_ARG:
	    ddMgr->errorHandler("Invalid argument.");
	    break;
	CUDD_INTERNAL_ERROR:
	    ddMgr->errorHandler("Internal error.");
	    break;
	CUDD_NO_ERROR:
	default:
	    ddMgr->errorHandler("Unexpected error.");
	    break;
	}
    }

} // DD::checkReturnValue


inline myDD::mgr_ptr
myDD::manager() const
{
    return ddMgr;

} // DD::manager2


inline int
myDD::nodeCount() const
{
    return Cudd_DagSize(node);

} // DD::nodeCount

inline
DdManager* myDD::getManager() const {return ddMgr->manager; }

// ---------------------------------------------------------------------------
// Members of class CCuddZDD
// ---------------------------------------------------------------------------


inline
CCuddZDD::~CCuddZDD() {
    if (node != 0) {
	Cudd_RecursiveDerefZdd(getManager(),node);
	if (ddMgr->verbose) {
	    cout << "CCuddZDD destructor called for node " << hex << long(node) <<
		" ref = " << Cudd_Regular(node)->ref << "\n";
	}
    }

} // CCuddZDD::~CCuddZDD


inline
CCuddZDD&
CCuddZDD::operator=(
  const CCuddZDD& right)
{
    if (this == &right) return *this;
    if (right.node != 0) Cudd_Ref(right.node);
    if (node != 0) {
	Cudd_RecursiveDerefZdd(getManager(),node);
	if (ddMgr->verbose) {
	    cout << "CCuddZDD dereferencing for node " << hex << long(node) <<
		" ref = " << node->ref << "\n";
	}
    }
    node = right.node;
    ddMgr = right.ddMgr;
    if (node != 0 && ddMgr->verbose) {
	cout << "CCuddZDD assignment for node " << hex << long(node) <<
	    " ref = " << node->ref << "\n";
    }
    return *this;

} // CCuddZDD::operator=



inline void
CCuddZDD::print(
  int nvars,
  int verbosity) const
{
    cout.flush();
    int retval = Cudd_zddPrintDebug(getManager(),node,nvars,verbosity);
    if (retval == 0) ddMgr->errorHandler("print failed");

} // CCuddZDD::print

inline
CCuddZDD::CCuddZDD(const CCuddInterface& bddManager, DdNode *bddNode): 
  base(bddManager.p,bddNode) {}

inline
CCuddZDD::CCuddZDD(CCuddInterface* bddManager, DdNode *bddNode): 
  base(bddManager->p,bddNode) {}

END_NAMESPACE_PBORI


#endif
