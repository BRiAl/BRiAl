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
 * Revision 1.2  2007/07/10 14:53:27  dreyer
 * CHANGE: clean-up
 *
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

  self apply(ternary_function func, 
             const self& first, const self& second) const {
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



#define PB_CUDDMGR_READ(funcname) funcname() const { \
  return Cudd_ ## funcname(getManager()); }

#define PB_CUDDMGR_SWITCH(funcname) void funcname()  { \
  Cudd_ ## funcname(getManager()); }

#define PB_CUDDMGR_SET2(funcname, thetype) void funcname(thetype arg)  {  \
    Cudd_ ## funcname(getManager(), arg); }

#define PB_CUDDMGR_SET(funcname) PB_CUDDMGR_SET2(funcname, int)


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
  typedef int idx_type;
  typedef CCuddZDD dd_type;
  typedef DdNode * (*unary_int_function)(DdManager *, int);
  typedef DdNode * (*void_function)(DdManager *);
  typedef DdNode* node_type;
public:
  CCuddInterface(unsigned int numVars = 0,
                 unsigned int numVarsZ = 0,
                 unsigned int numSlots = CUDD_UNIQUE_SLOTS,
                 unsigned int cacheSize = CUDD_CACHE_SLOTS,
                 unsigned long maxMemory = 0):
    p (new mycapsule(numVars, numVarsZ, numSlots, cacheSize, maxMemory)) {}

  CCuddInterface(const CCuddInterface& x):  p(x.p) {}

  CCuddInterface(mgr_ptr rhs): p(rhs) {};

  ~CCuddInterface() {}

  PFC setHandler(PFC newHandler) {
    PFC oldHandler = p->errorHandler;
    p->errorHandler = newHandler;
    return oldHandler;
  }

  PFC getHandler() const {  return p->errorHandler; }

  DdManager *getManager() const {return p->manager;}
  mgr_ptr managerCore() const {return p;}
  
  void makeVerbose() {p->verbose = 1;}
  void makeTerse() {p->verbose = 0;}
  int isVerbose() const {return p->verbose;}
  void checkReturnValue(const DdNode *result) const;
  void checkReturnValue(const int result) const;

  CCuddInterface& operator=(const CCuddInterface& right) {
    p = right.p;
    return *this;
  }

  void info() const { checkedResult(Cudd_PrintInfo(getManager(),stdout)); }

  CCuddZDD zddVar(idx_type idx) const { return apply(Cudd_zddIthVar, idx); }
  CCuddZDD zddOne(idx_type iMax) const  { return apply(Cudd_ReadZddOne, iMax); }
  CCuddZDD zddZero() const { return apply(Cudd_ReadZero); }

  PB_CUDDMGR_SET2(AutodynEnableZdd, Cudd_ReorderingType)

  PB_CUDDMGR_SWITCH(AutodynDisableZdd);

  int ReorderingStatusZdd(Cudd_ReorderingType * method) const {
    return Cudd_ReorderingStatusZdd(getManager(), method);
  }
  int PB_CUDDMGR_READ(zddRealignmentEnabled);

  PB_CUDDMGR_SWITCH(zddRealignEnable)
  PB_CUDDMGR_SWITCH(zddRealignDisable)

  unsigned int PB_CUDDMGR_READ(ReadCacheSlots)
  double PB_CUDDMGR_READ(ReadCacheUsedSlots)
  double PB_CUDDMGR_READ(ReadCacheLookUps)
  double PB_CUDDMGR_READ(ReadCacheHits)
  unsigned int PB_CUDDMGR_READ(ReadMinHit)
  PB_CUDDMGR_SET2(SetMinHit, unsigned int)

  unsigned int PB_CUDDMGR_READ(ReadLooseUpTo)

  PB_CUDDMGR_SET2(SetLooseUpTo, unsigned int)
  unsigned int PB_CUDDMGR_READ(ReadMaxCache)
  unsigned int PB_CUDDMGR_READ(ReadMaxCacheHard)

  PB_CUDDMGR_SET2(SetMaxCacheHard, unsigned int)

  int PB_CUDDMGR_READ(ReadZddSize)
  unsigned int PB_CUDDMGR_READ(ReadSlots)
  unsigned int PB_CUDDMGR_READ(ReadKeys)
  unsigned int PB_CUDDMGR_READ(ReadDead)
  unsigned int PB_CUDDMGR_READ(ReadMinDead)
  int PB_CUDDMGR_READ(ReadReorderings)
  long PB_CUDDMGR_READ(ReadReorderingTime)
  int PB_CUDDMGR_READ(ReadGarbageCollections)
  long PB_CUDDMGR_READ(ReadGarbageCollectionTime)
  int  PB_CUDDMGR_READ(ReadSiftMaxVar)
    
  int PB_CUDDMGR_READ(ReadSiftMaxSwap)
  double PB_CUDDMGR_READ(ReadMaxGrowth)

  PB_CUDDMGR_SET(SetSiftMaxVar)
  PB_CUDDMGR_SET(SetSiftMaxSwap)
  PB_CUDDMGR_SET2(SetMaxGrowth, double)

  MtrNode * PB_CUDDMGR_READ(ReadZddTree)
  PB_CUDDMGR_SET2(SetZddTree, MtrNode*)
  PB_CUDDMGR_SWITCH(FreeZddTree)
  int ReadPermZdd(int i) const { return Cudd_ReadPermZdd(getManager(), i); }

  int ReadInvPermZdd(int i) const { 
    return Cudd_ReadInvPermZdd(getManager(), i); 
  }

  int PB_CUDDMGR_READ(GarbageCollectionEnabled)

  PB_CUDDMGR_SWITCH(EnableGarbageCollection)
  PB_CUDDMGR_SWITCH(DisableGarbageCollection) 
  int PB_CUDDMGR_READ(DeadAreCounted)
  PB_CUDDMGR_SWITCH(TurnOnCountDead)
  PB_CUDDMGR_SWITCH(TurnOffCountDead)
  int PB_CUDDMGR_READ(ReadRecomb)

  PB_CUDDMGR_SET(SetRecomb)
  int PB_CUDDMGR_READ(ReadSymmviolation);
  PB_CUDDMGR_SET(SetSymmviolation)
  int PB_CUDDMGR_READ(ReadArcviolation)
  PB_CUDDMGR_SET(SetArcviolation)
  int PB_CUDDMGR_READ(ReadPopulationSize)

  PB_CUDDMGR_SET(SetPopulationSize)
  int PB_CUDDMGR_READ(ReadNumberXovers)
  PB_CUDDMGR_SET(SetNumberXovers)
  unsigned long PB_CUDDMGR_READ(ReadMemoryInUse)
  long PB_CUDDMGR_READ(ReadPeakNodeCount)

  long PB_CUDDMGR_READ(zddReadNodeCount);

  void AddHook(DD_HFP f, Cudd_HookType where) { 
    checkedResult(Cudd_AddHook(getManager(), f, where));
  }
  void RemoveHook(DD_HFP f, Cudd_HookType where) { 
    checkedResult(Cudd_RemoveHook(getManager(), f, where)); 
  }
  int IsInHook(DD_HFP f, Cudd_HookType where) const { 
    return Cudd_IsInHook(getManager(), f, where); 
  }
  void EnableReorderingReporting() { 
    checkedResult(Cudd_EnableReorderingReporting(getManager())); 
  }
  void DisableReorderingReporting() { 
    checkedResult(Cudd_DisableReorderingReporting(getManager())); 
  }
  int PB_CUDDMGR_READ(ReorderingReporting)
  int PB_CUDDMGR_READ(ReadErrorCode)
  PB_CUDDMGR_SWITCH(ClearErrorCode) 

  FILE* PB_CUDDMGR_READ(ReadStdout)
  PB_CUDDMGR_SET2(SetStdout, FILE*)
  FILE* PB_CUDDMGR_READ(ReadStderr)
  PB_CUDDMGR_SET2(SetStderr, FILE*)

  unsigned int PB_CUDDMGR_READ(ReadNextReordering)

  double PB_CUDDMGR_READ(ReadSwapSteps)
  unsigned int PB_CUDDMGR_READ(ReadMaxLive)
  PB_CUDDMGR_SET2(SetMaxLive, unsigned int)
  unsigned long PB_CUDDMGR_READ(ReadMaxMemory)

  PB_CUDDMGR_SET2(SetMaxMemory, unsigned long)

  void DebugCheck(){ checkedResult(Cudd_DebugCheck(getManager())); }
  void CheckKeys(){ checkedResult(Cudd_CheckKeys(getManager())); }
  void PrintLinear() { checkedResult(Cudd_PrintLinear(getManager())); }

  int ReadLinear(int x, int y) { return Cudd_ReadLinear(getManager(), x, y); }

   unsigned int Prime(unsigned int pr) const { return Cudd_Prime(pr); }

  int SharingSize(DD* nodes, int n) const; // inlined below
  
  void PrintVersion(FILE * fp) const { cout.flush(); Cudd_PrintVersion(fp); }

  double PB_CUDDMGR_READ(AverageDistance)

  MtrNode * MakeZddTreeNode(unsigned int low, 
                            unsigned int size, unsigned int type) {
    return Cudd_MakeZddTreeNode(getManager(), low, size, type);
  }
  void zddPrintSubtable() const{ 
    cout.flush();
    Cudd_zddPrintSubtable(getManager());
  }

  void zddReduceHeap(Cudd_ReorderingType heuristic, int minsize) {
    checkedResult(Cudd_zddReduceHeap(getManager(), heuristic, minsize));
  }
  void zddShuffleHeap(int * permutation) { 
    checkedResult(Cudd_zddShuffleHeap(getManager(), permutation));
  }
  void zddSymmProfile(int lower, int upper) const {
    Cudd_zddSymmProfile(getManager(), lower, upper);
  }

protected:

  dd_type checkedResult(node_type result) const  { 
    checkReturnValue(result);
    return dd_type(managerCore(), result);
  }
  idx_type checkedResult(idx_type result) const  { 
    checkReturnValue(result);
    return result;
  }
  dd_type apply(unary_int_function func, idx_type idx) const  { 
    return checkedResult(func(getManager(), idx) );
  }
  dd_type apply(void_function func) const { 
    return checkedResult(func(getManager()) );
  }
}; // CCuddInterface



// ---------------------------------------------------------------------------
// Members of class CCuddInterface
// ---------------------------------------------------------------------------



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

inline int
CCuddInterface::SharingSize(
  DD* nodes,
  int n) const
{
    DdNode **nodeArray = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	nodeArray[i] = nodes[i].getNode();
    }
    int result = Cudd_SharingSize(nodeArray, n);
    FREE(nodeArray);
    this->checkReturnValue(result > 0);
    return result;

} // Cudd::SharingSize

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
	case CUDD_MEMORY_OUT:
	    ddMgr->errorHandler("Out of memory.");
	    break;
	case CUDD_TOO_MANY_NODES:
	    break;
	case CUDD_MAX_MEM_EXCEEDED:
	    ddMgr->errorHandler("Maximum memory exceeded.");
	    break;
	case CUDD_INVALID_ARG:
	    ddMgr->errorHandler("Invalid argument.");
	    break;
	case CUDD_INTERNAL_ERROR:
	    ddMgr->errorHandler("Internal error.");
	    break;
	case CUDD_NO_ERROR:
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
	case CUDD_MEMORY_OUT:
	    ddMgr->errorHandler("Out of memory.");
	    break;
	case CUDD_TOO_MANY_NODES:
	    break;
	case CUDD_MAX_MEM_EXCEEDED:
	    ddMgr->errorHandler("Maximum memory exceeded.");
	    break;
	case CUDD_INVALID_ARG:
	    ddMgr->errorHandler("Invalid argument.");
	    break;
	case CUDD_INTERNAL_ERROR:
	    ddMgr->errorHandler("Internal error.");
	    break;
	case CUDD_NO_ERROR:
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
