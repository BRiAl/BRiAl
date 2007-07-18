// -*- c++ -*-
//*****************************************************************************
/** @file CCuddZDD.h
 *
 * @author Alexander Dreyer
 * @date 2007-07-16
 *
 * This files defines a replacement for the decision diagrams of CUDD's
 * C++ interface. 
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
 * Revision 1.4  2007/07/18 15:46:14  dreyer
 * CHANGE: added documentation
 *
 * Revision 1.3  2007/07/18 15:11:00  dreyer
 * CHANGE: simplified handle_error
 *
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

class CCuddTypes :
  public CAuxTypes {

public:
  typedef unsigned long large_size_type;
  typedef long int refcount_type;

  typedef DdNode* node_type;
  typedef DdManager* mgrcore_type;

  typedef PFC errorfunc_type;
  typedef node_type (*unary_int_function)(mgrcore_type, int);
  typedef node_type (*void_function)(mgrcore_type);

  typedef DD_CTFP binary_function;
  typedef node_type (*binary_int_function)(mgrcore_type, node_type, int);
  typedef 
  node_type (*ternary_function)(mgrcore_type, node_type, node_type, node_type);

  typedef int (*int_unary_function)(mgrcore_type, node_type);
};

template<unsigned ErrorNumber = CUDD_INTERNAL_ERROR>
struct handle_error {
  typedef CCuddTypes::errorfunc_type errorfunc_type;

  handle_error(errorfunc_type errfunc): m_errfunc(errfunc) {}

  bool found(unsigned err) const {
    if UNLIKELY(err == ErrorNumber) {
      m_errfunc(cudd_error_traits<ErrorNumber>()());
      return true;
    }
    return false;        
  }

  void operator()(unsigned err) const {
    if UNLIKELY(err == ErrorNumber) 
      m_errfunc(cudd_error_traits<ErrorNumber>()());
    else
      reinterpret_cast<const handle_error<ErrorNumber - 1>&>(*this)(err);
  }

protected: 
  const errorfunc_type m_errfunc;
};


template<>
struct handle_error<0> {
  typedef CCuddTypes::errorfunc_type errorfunc_type;

  handle_error(errorfunc_type errfunc): m_errfunc(errfunc) {}

  void operator()(unsigned err) const {
    if LIKELY(err == 0)
      m_errfunc(cudd_error_traits<0>()());
  }
protected: 
  errorfunc_type m_errfunc;
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

class CCuddDD:
  public CCuddTypes {

protected:

  /// Test, whether both operands 
  void checkSameManager(const CCuddDD &other) const {
    if (getManager() != other.getManager()) 
      ddMgr->errorHandler("Operands come from different manager.");
  }

  /// Check whether decision diagram operation in computing result was valid
  void checkReturnValue(const node_type result) const {
    checkReturnValue(result != NULL);
  }

  /// Check whether previous decision diagram operation for validity
  void checkReturnValue(const int result, const int expected = 1) const {
    if UNLIKELY(result != expected)
      handle_error<>(ddMgr->errorHandler)(Cudd_ReadErrorCode( getManager() ));
  } 

public:
  /// Define shared pointer type for handling the decision diagram manager
  typedef boost::intrusive_ptr<CCuddCore> mgrcore_ptr;

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

  /// Get (shared) pointer to decision diagram manager
  mgrcore_ptr manager() const { return ddMgr; }

  /// Get raw decision diagram manager
  mgrcore_type getManager() const { return ddMgr->manager; }

  /// Get raw node structure
  node_type getNode() const{  return node; }

  /// Get index of curent node
  size_type NodeReadIndex() const { return Cudd_NodeReadIndex(node); }

  /// Number of nodes in the current decision diagram
  size_type nodeCount() const { return (size_type)(Cudd_DagSize(node)); }

  /// Number of references pointing here
  size_type refCount() const { 
    assert(node != NULL);
    return Cudd_Regular(node)->ref;
  }

protected:
  /// (Shared) pointer to decsion diagram management
  mgrcore_ptr ddMgr;

  /// Raw pointer to decision diagram node
  node_type node;
}; // CCuddDD

#define PB_ZDD_APPLY(count, data, funcname) \
  self funcname(data rhs) const {    \
    return apply(BOOST_PP_CAT(Cudd_zdd, funcname), rhs); }

#define PB_ZDD_OP_ASSIGN(count, data, op) \
  self& operator BOOST_PP_CAT(op, =)(const self& other) { \
    return *this = (*this op other); }

#define PB_ZDD_OP(count, data, op) \
  self operator op(const self& other) const { return data(other); }


/** @class CCuddZDD
 * @brief This class defines a C++ interface to @c CUDD's decicion diagram
 * structure.
 *
 * The purpose of this wrapper is just to provide an efficient and save way of
 * handling the decision diagrams. It corrects some short-comings of
 * CUDD's built-in interface.
 *
 * @attention This class is intented for internal use only. 
 * Use the highlevel classes CDDInterface<CCuddZDD>, BoolePolynomial, BooleSet,
 * or BooleMonomial instead.
 **/

class CCuddZDD : 
  public CCuddDD {
    friend class CCuddInterface;

public:
  /// Name type of *this
  typedef CCuddZDD self;

  /// Name the type, which self is inherited from
  typedef CCuddDD base;
 
  /// Construct ZDD from manager core and node
  CCuddZDD(mgrcore_ptr mgr, node_type bddNode): base(mgr, bddNode) {}

  /// Default constructor
  CCuddZDD(): base() {}

  /// Copy constructor
  CCuddZDD(const self &from): base(from) {}

  /// Destructor
  ~CCuddZDD() { deref(); }

  /// Assignment operator
  self& operator=(const self& right); // inlined below

  /// @name Logical operations
  //@{
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
  //@}

  /// @note Preprocessor generated members
  /// @code
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP, Intersect, (*)(&))
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP, Union, (+)(|))
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP, Diff, (-))

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_OP_ASSIGN, BOOST_PP_NIL, (*)(&)(+)(|)(-))

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_APPLY, const self&, 
    (Product)(UnateProduct)(WeakDiv)(Divide)(WeakDivF)(DivideF)
    (Union)(Intersect)(Diff)(DiffConst))

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_APPLY, int, (Subset1)(Subset0)(Change))
  /** @endcode */

  /// If-Then-Else operation using current diagram as head
  self Ite(const self& g, const self& h) const { 
    return apply(Cudd_zddIte, g, h); 
  }

  /// @name Functions for print useful information
  //@{
  void print(int nvars, int verbosity = 1) const { std::cout.flush(); 
    if UNLIKELY(!Cudd_zddPrintDebug(getManager(), node, nvars, verbosity))
      ddMgr->errorHandler("print failed");
  }
  void PrintMinterm() const  { apply(Cudd_zddPrintMinterm); }
  void PrintCover() const    { apply(Cudd_zddPrintCover); }
  //@}

  /// Determine the number of minterms
  int Count() const          { return memApply(Cudd_zddCount); }

  /// Determine the number of minterms
  double CountDouble() const { return memApply(Cudd_zddCountDouble); }

  /// Counts minterms; takes a path specifing variables number in the support
  double CountMinterm(int path) const { 
    return memChecked(Cudd_zddCountMinterm(getManager(), getNode(), path));
  }

protected:

  /// @name Apply CUDD procedures to nodes
  //@{
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
  //@}

  /// @name Test results from CUDD procedures for validity
  //@{
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
  // @}

  /// Derefering current diagram node, if unused
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


