// -*- c++ -*-
//*****************************************************************************
/** @file CCuddInterface
 *
 * @author Alexander Dreyer
 * @date 2007-07-05
 *
 * 
 *
 * @par Copyright:
 *   (c) 2007 by
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
 * Revision 1.4  2007/07/13 15:05:23  dreyer
 * CHANGE: cleaned up once more
 *
 * Revision 1.3  2007/07/12 15:32:31  dreyer
 * CHANGE: cleanup using preprocessor meta-programming
 *
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

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/stringize.hpp>
BEGIN_NAMESPACE_PBORI

#define PB_BINARY_FUNC_CALL(count, funcname, arg_pair)                        \
  BOOST_PP_EXPAND(funcname(BOOST_PP_SEQ_HEAD(arg_pair),                       \
                           BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(arg_pair))))

template<unsigned ErrorNumber>
struct cudd_error_traits {
  typedef const char* result_type;

  result_type operator()() const;
};


#if 0
template<>
inline const char* cudd_error_traits<CUDD_MEMORY_OUT>::operator()() const {
  return "Out of memory.";
}
#endif


#define PB_CUDD_ERROR_TRAITS(errcode, errstr)                                \
  template<> inline cudd_error_traits<errcode>::result_type                  \
  cudd_error_traits<errcode>::operator()() const {                           \
    return BOOST_PP_STRINGIZE(errstr); }




//PB_CUDD_ERROR_TRAITS(0, 0,  (CUDD_MEMORY_OUT)("Out of memory.")  )

BOOST_PP_SEQ_FOR_EACH( PB_BINARY_FUNC_CALL, PB_CUDD_ERROR_TRAITS, 
                       ((CUDD_MEMORY_OUT)(Out of memory.))
                       ((CUDD_TOO_MANY_NODES)(Too many nodes.))
                       ((CUDD_MAX_MEM_EXCEEDED)(Maximum memory exceeded.))
                       ((CUDD_INVALID_ARG)(Invalid argument.))
                       ((CUDD_INTERNAL_ERROR)(Internal error.))
                       ((CUDD_NO_ERROR)(Unexpected error.))
                       )

#undef PB_CUDD_ERROR_TRAITS

inline void
mydefaultError(
  string message)
{
    cerr << message << "\n";
    exit(1);

} // defaultError


template<unsigned ErrorNumber = CUDD_INTERNAL_ERROR>
struct handle_error {
  static bool found(unsigned err) {
    if UNLIKELY(err == ErrorNumber) {
      mydefaultError(cudd_error_traits<ErrorNumber>()());
      return true;
    }
    return false;        
  }

  void operator()(unsigned err) const {
    if (!found(err))
      handle_error<ErrorNumber-1>()(err);
  }
};

struct handle_cudd_error:
  public  handle_error<> {};

template<>
struct handle_error<0> {
  void operator()(unsigned err) const {
    if LIKELY(err == 0)
      mydefaultError(cudd_error_traits<0>()());
  }
};


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

  void checkReturnValue(const DdNode *result) const {
    checkReturnValue(result != NULL);
  }
  void checkReturnValue(const int result, const int expected = 1) const {
    if UNLIKELY(result != expected)
      handle_cudd_error()(Cudd_ReadErrorCode( getManager() ));
  } 

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

#define PB_ZDD_APPLY(count, data, funcname) \
  self funcname(data rhs) const {    \
    return apply(BOOST_PP_CAT(Cudd_zdd, funcname), rhs); }

#define PB_ZDD_OP_ASSIGN(count, data, op) \
  self& operator BOOST_PP_CAT(op, =)(const self& other) { \
    return *this = (*this op other); }

#define PB_ZDD_OP(count, data, op) \
  self operator op(const self& other) const { return data(other); }


class CCuddZDD : public myDD {
    friend class CCuddInterface;
public:
  typedef CCuddZDD self;
  typedef int idx_type;
  typedef myDD base;
  typedef base::mgr_ptr mgr_ptr;

  CCuddZDD(mgr_ptr bddManager, DdNode *bddNode): base(bddManager,bddNode) {}

  CCuddZDD(const CCuddInterface& bddManager, DdNode *bddNode);//inlined below
  CCuddZDD(CCuddInterface* bddManager, DdNode *bddNode);//inlined below

  CCuddZDD(): base() {}
  CCuddZDD(const self &from): base(from) {}

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
    return (*this != rhs) && (*this <= rhs);  
  }
  bool operator>(const self& other) const { 
    return (*this != other) && (*this >= other); 
  }

  bool isZero() const { return node == Cudd_ReadZero(getManager()); }

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP, Intersect, (*)(&))
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP, Union, (+)(|))
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP, Diff, (-))

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP_ASSIGN, BOOST_PP_NIL, (*)(&)(+)(|)(-))

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_APPLY, const self&, 
    (Product)(UnateProduct)(WeakDiv)(Divide)(WeakDivF)(DivideF)
    (Union)(Intersect)(Diff)(DiffConst)
  )
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_APPLY, int, (Subset1)(Subset0)(Change))

  self Ite(const self& g, const self& h) const { 
    return apply(Cudd_zddIte, g, h); 
  }
  void PrintMinterm() const { apply(Cudd_zddPrintMinterm); }
  void PrintCover() const  { apply(Cudd_zddPrintCover); }
  int Count() const { return memApply(Cudd_zddCount); }
  double CountDouble() const { return memApply(Cudd_zddCountDouble); }

  double CountMinterm(int path) const { 
    return memChecked(Cudd_zddCountMinterm(getManager(), getNode(), path));
  }

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

#undef PB_ZDD_APPLY
#undef PB_ZDD_OP_ASSIGN
#undef PB_ZDD_OP

#define PB_CUDDMGR_READ(count, data, funcname) data funcname() const { \
  return BOOST_PP_CAT(Cudd_, funcname)(getManager()); }

#define PB_CUDDMGR_SWITCH(count, data, funcname) void funcname() { \
    BOOST_PP_CAT(Cudd_, funcname)(getManager()); }

#define PB_CUDDMGR_SET(count, data, funcname)  void funcname(data arg) { \
    BOOST_PP_CAT(Cudd_, funcname)(getManager(), arg); }


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
    p (new mycapsule(numVars, numVarsZ, numSlots, cacheSize, maxMemory)) {
  }

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


  void checkReturnValue(const DdNode *result) const {
    checkReturnValue(result != NULL);
  }
  void checkReturnValue(const int result) const {
    if UNLIKELY(result == 0) {
      typedef handle_error<CUDD_MEMORY_OUT> handle_type;
      if ( !handle_type::found(Cudd_ReadErrorCode(getManager())) )
        mydefaultError(cudd_error_traits<CUDD_INTERNAL_ERROR>()());
    }
 } 


  CCuddInterface& operator=(const CCuddInterface& right) {
    p = right.p;
    return *this;
  }

  void info() const { checkedResult(Cudd_PrintInfo(getManager(),stdout)); }

  CCuddZDD zddVar(idx_type idx) const { return apply(Cudd_zddIthVar, idx); }
  CCuddZDD zddOne(idx_type iMax) const  { return apply(Cudd_ReadZddOne, iMax); }
  CCuddZDD zddZero() const { return apply(Cudd_ReadZero); }

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_SET, unsigned int, 
    (SetMinHit)(SetLooseUpTo)(SetMaxCacheHard)(SetMaxLive) )

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_SET, int, 
    (SetSiftMaxVar)(SetSiftMaxSwap)(SetRecomb)(SetSymmviolation)
    (SetArcviolation)(SetPopulationSize)(SetNumberXovers)
  )

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_SET, FILE*, (SetStdout)(SetStderr))

  PB_CUDDMGR_SET(BOOST_PP_NIL, Cudd_ReorderingType, AutodynEnableZdd)
  PB_CUDDMGR_SET(BOOST_PP_NIL, unsigned long, SetMaxMemory)
  PB_CUDDMGR_SET(BOOST_PP_NIL, double, SetMaxGrowth)
  PB_CUDDMGR_SET(BOOST_PP_NIL, MtrNode*, SetZddTree)

  int ReorderingStatusZdd(Cudd_ReorderingType * method) const {
    return Cudd_ReorderingStatusZdd(getManager(), method);
  }
  PB_CUDDMGR_READ(BOOST_PP_NIL, int, zddRealignmentEnabled);

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_SWITCH, BOOST_PP_NIL, 
    (zddRealignEnable)(zddRealignDisable)
    (AutodynDisableZdd)(FreeZddTree)
    (EnableGarbageCollection)(DisableGarbageCollection)
    (TurnOnCountDead)(TurnOffCountDead)(ClearErrorCode)  
  )

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_READ, double,
    (ReadCacheUsedSlots)(ReadCacheLookUps)(ReadCacheHits) 
    (ReadSwapSteps)(ReadMaxGrowth)(AverageDistance)
  )

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_READ, unsigned int,
    (ReadCacheSlots)(ReadMinHit)(ReadLooseUpTo)(ReadMaxCache)
    (ReadMaxCacheHard)(ReadSlots)(ReadKeys)(ReadDead)(ReadMinDead)
    (ReadNextReordering)(ReadMaxLive)
  )

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_READ, int,
    (ReadZddSize)(ReadReorderings)(ReadSiftMaxVar)
    (ReadSiftMaxSwap)(ReadGarbageCollections)(GarbageCollectionEnabled)
    (DeadAreCounted)(ReadRecomb)
    (ReadPopulationSize)(ReadSymmviolation)(ReadArcviolation)
    (ReadNumberXovers)(ReorderingReporting)(ReadErrorCode)
  )

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_READ, long,
    (ReadReorderingTime)(ReadGarbageCollectionTime)
    (ReadPeakNodeCount)(zddReadNodeCount)
  )

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_READ, unsigned long, 
    (ReadMemoryInUse)(ReadMaxMemory) )

  BOOST_PP_SEQ_FOR_EACH(PB_CUDDMGR_READ, FILE*, (ReadStdout)(ReadStderr))

  PB_CUDDMGR_READ(BOOST_PP_NIL, MtrNode*, ReadZddTree)

  int ReadPermZdd(int i) const { return Cudd_ReadPermZdd(getManager(), i); }

  int ReadInvPermZdd(int i) const { 
    return Cudd_ReadInvPermZdd(getManager(), i); 
  }

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

  void DebugCheck(){ checkedResult(Cudd_DebugCheck(getManager())); }
  void CheckKeys(){ checkedResult(Cudd_CheckKeys(getManager())); }
  void PrintLinear() { checkedResult(Cudd_PrintLinear(getManager())); }

  int ReadLinear(int x, int y) { return Cudd_ReadLinear(getManager(), x, y); }

  unsigned int Prime(unsigned int pr) const { return Cudd_Prime(pr); }

  int SharingSize(DD* nodes, int n) const; // inlined below
  
  void PrintVersion(FILE * fp) const { cout.flush(); Cudd_PrintVersion(fp); }

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


#undef PB_CUDDMGR_READ
#undef PB_CUDDMGR_SWITCH
#undef PB_CUDDMGR_SET

// ---------------------------------------------------------------------------
// Members of class CCuddInterface
// ---------------------------------------------------------------------------



// inline void
// CCuddInterface::checkReturnValue(
//   const DdNode *result) const
// {
//     if (result == 0) {
// 	if (Cudd_ReadErrorCode(getManager()) == CUDD_MEMORY_OUT) {
// 	    p->errorHandler("Out of memory.");
// 	} else {
// 	    p->errorHandler("Internal error.");
// 	}
//     }

// } // Cudd::checkReturnValue


// inline void
// CCuddInterface::checkReturnValue(
//   const int result) const
// {
//     if (result == 0) {
// 	if (Cudd_ReadErrorCode(getManager()) == CUDD_MEMORY_OUT) {
// 	    p->errorHandler("Out of memory.");
// 	} else {
// 	    p->errorHandler("Internal error.");
// 	}
//     }

// } // Cudd::checkReturnValue

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
