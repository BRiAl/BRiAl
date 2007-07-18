// -*- c++ -*-
//*****************************************************************************
/** @file CCuddZDD.h
 *
 * @author Alexander Dreyer
 * @date 2007-07-16
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
 * Revision 1.2  2007/07/18 07:17:26  dreyer
 * CHANGE: some clean-ups
 *
 * Revision 1.1  2007/07/17 15:56:59  dreyer
 * ADD: header file for CCuddZDD; clean-up
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef CCuddZDD_h
#define CCuddZDD_h

// include basic definitions
#include "pbori_defs.h"

#include <algorithm>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>

#include <boost/weak_ptr.hpp>

#include <boost/intrusive_ptr.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/stringize.hpp>

#include "pbori_func.h"
#include "pbori_traits.h"

BEGIN_NAMESPACE_PBORI


template<unsigned ErrorNumber = CUDD_INTERNAL_ERROR>
struct handle_error {
  static bool found(unsigned err) {
    if UNLIKELY(err == ErrorNumber) {
      defaultError(cudd_error_traits<ErrorNumber>()());
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
      defaultError(cudd_error_traits<0>()());
  }
};

class CCuddTypes :
  public CAuxTypes {

public:
  typedef DdNode* node_type;
  typedef unsigned long large_size_type;
  typedef DdManager* mgrcore_type;
  typedef PFC errorfunc_type;
  typedef long int refcount_type;

  typedef node_type (*unary_int_function)(mgrcore_type, int);
  typedef node_type (*void_function)(mgrcore_type);
};

class CCuddCore:
  public CCuddTypes {

public:
  typedef CCuddCore self;
  typedef boost::intrusive_ptr<self> mgrcore_ptr;

  mgrcore_type manager;
  errorfunc_type errorHandler;
  bool verbose;
  refcount_type ref;

  CCuddCore(size_type numVars = 0,
            size_type numVarsZ = 0,
            size_type numSlots = CUDD_UNIQUE_SLOTS,
            size_type cacheSize = CUDD_CACHE_SLOTS,
            large_size_type maxMemory = 0):  ref(0) {
    manager = Cudd_Init(numVars,numVarsZ,numSlots,cacheSize,maxMemory);
    errorHandler = defaultError;
    verbose = 0;		// initially terse
  }

  ~CCuddCore(){ erase(); }

  void erase() {

    if (--(ref) == 0){
      int retval = Cudd_CheckZeroRef(manager);
      if (retval != 0) {
        std::cerr << retval << " unexpected non-zero reference counts\n";
      } else if (verbose) {
        std::cerr << "All went well\n";
      }
      Cudd_Quit(manager);
    }
  }
};

inline
void intrusive_ptr_add_ref(CCuddCore* p){
    ++(p->ref);
}

inline
void intrusive_ptr_release(CCuddCore * p) {
  p->erase();
}



// ---------------------------------------------------------------------------
// Class CCuddDD
// ---------------------------------------------------------------------------
#define PB_DD_VERBOSE(text) if (ddMgr->verbose) \
  std::cout << text << " for node " << node <<  \
  " ref = " << refCount() << std::endl;

class CCuddDD {


public: // temporarily

  typedef boost::intrusive_ptr<CCuddCore> mgrcore_ptr;
  typedef DdNode* node_type;
  typedef CTypes::size_type size_type;
  mgrcore_ptr ddMgr;
  node_type node;

  void checkSameManager(const CCuddDD &other) const {
    if (getManager() != other.getManager()) 
      ddMgr->errorHandler("Operands come from different manager.");
  }

  void checkReturnValue(const DdNode *result) const {
    checkReturnValue(result != NULL);
  }
  void checkReturnValue(const int result, const int expected = 1) const {
    if UNLIKELY(result != expected)
      handle_cudd_error()(Cudd_ReadErrorCode( getManager() ));
  } 

public:

  /// Construct diagram from raw CUDD elements
  CCuddDD(mgrcore_ptr ddManager, node_type ddNode): 
    ddMgr(ddManager), node(ddNode) {
    if (node) Cudd_Ref(node);
    PB_DD_VERBOSE("Standard DD constructor");
  }

  /// Copy constructor
  CCuddDD(const CCuddDD& from): 
    ddMgr(from.ddMgr), node(from.node) {
    if (node) {
      Cudd_Ref(node);
      PB_DD_VERBOSE("Copy DD constructor");
    }
  } 

  /// Default constructor
  CCuddDD(): ddMgr(mgrcore_ptr()), node(NULL) {}

 
  mgrcore_ptr manager() const { return ddMgr; }
  DdManager* getManager() const { return ddMgr->manager; }

  node_type getNode() const{  return node; }
  int nodeCount() const { return Cudd_DagSize(node); }
  size_type NodeReadIndex() const { return Cudd_NodeReadIndex(node); };

  size_type refCount() const { 
    assert(node != NULL);
    return Cudd_Regular(node)->ref;
  }
}; // CCuddDD

#define PB_ZDD_APPLY(count, data, funcname) \
  self funcname(data rhs) const {    \
    return apply(BOOST_PP_CAT(Cudd_zdd, funcname), rhs); }

#define PB_ZDD_OP_ASSIGN(count, data, op) \
  self& operator BOOST_PP_CAT(op, =)(const self& other) { \
    return *this = (*this op other); }

#define PB_ZDD_OP(count, data, op) \
  self operator op(const self& other) const { return data(other); }


class CCuddZDD : public CCuddDD {
    friend class CCuddInterface;
public:
  typedef CCuddZDD self;
  typedef int idx_type;
  typedef CCuddDD base;
  typedef base::mgrcore_ptr mgrcore_ptr;

  CCuddZDD(mgrcore_ptr bddManager, DdNode *bddNode): base(bddManager, bddNode) {}

  CCuddZDD(): base() {}
  CCuddZDD(const self &from): base(from) {}

  ~CCuddZDD() { deref(); }
  self& operator=(const self& right); // inlined below

  void print(int nvars, int verbosity = 1) const { std::cout.flush(); 
    if UNLIKELY(!Cudd_zddPrintDebug(getManager(), node, nvars, verbosity))
      ddMgr->errorHandler("print failed");
  }

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
    (Union)(Intersect)(Diff)(DiffConst))

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_APPLY, int, (Subset1)(Subset0)(Change))

  self Ite(const self& g, const self& h) const { 
    return apply(Cudd_zddIte, g, h); 
  }
  void PrintMinterm() const  { apply(Cudd_zddPrintMinterm); }
  void PrintCover() const    { apply(Cudd_zddPrintCover); }
  int Count() const          { return memApply(Cudd_zddCount); }
  double CountDouble() const { return memApply(Cudd_zddCountDouble); }

  double CountMinterm(int path) const { 
    return memChecked(Cudd_zddCountMinterm(getManager(), getNode(), path));
  }

public:
  typedef DdNode* node_type;

protected:
  typedef DD_CTFP binary_function;
  typedef node_type (*binary_int_function)(DdManager*, node_type, int);
  typedef 
  node_type (*ternary_function)(DdManager*, node_type, node_type, node_type);

  typedef int (*int_unary_function)(DdManager*, node_type);



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
  ResultType memApply(ResultType (*func)(DdManager *, node_type)) const {
    return memChecked(func(getManager(), getNode()) );
  }

  template <class ResultType>
  ResultType memChecked(ResultType result) const {
    checkReturnValue(result != (ResultType) CUDD_OUT_OF_MEM);
    return result;
  }
  void deref() {
    if (node != 0) {
      Cudd_RecursiveDerefZdd(getManager(), node);
      PB_DD_VERBOSE("CCuddZDD dereferencing");
    }
  }
}; //CCuddZDD

#undef PB_ZDD_APPLY
#undef PB_ZDD_OP_ASSIGN
#undef PB_ZDD_OP

// ---------------------------------------------------------------------------
// Members of class CCuddZDD
// ---------------------------------------------------------------------------

inline CCuddZDD& 
CCuddZDD::operator=(const CCuddZDD& right) {

  if UNLIKELY(this == &right) return *this;
  if LIKELY(right.node) Cudd_Ref(right.node);
  deref();
  
  node = right.node;
  ddMgr = right.ddMgr;
  if (node)
    PB_DD_VERBOSE("CCuddZDD assignment");
  
  return *this;
}

#undef PB_DD_VERBOSE

END_NAMESPACE_PBORI

#endif


