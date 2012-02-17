// -*- c++ -*-
//*****************************************************************************
/** @file CCuddDDFacade.h 
 *
 * @author Alexander Dreyer
 * @date 2010-08-25
 *
 * This file defines a facade for decision diagrams
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_diagram_CCuddDDFacade_h
#define polybori_diagram_CCuddDDFacade_h

// include basic definitions
#include <polybori/pbori_defs.h>

#include <polybori/cudd/cudd.h>
#include <polybori/cudd/prefix.h>
#include "CApplyNodeFacade.h"
#include "CNodeCounter.h"

#include <polybori/routines/pbori_routines_cuddext.h>
#include <polybori/common/CExtrusivePtr.h>

// Getting iterator type for navigating through Cudd's ZDDs structure
#include <polybori/iterators/CCuddNavigator.h>

// Getting iterator type for retrieving first term from Cudd's ZDDs
#include <polybori/iterators/CCuddFirstIter.h>

// Getting iterator type for retrieving last term from Cudd's ZDDs
#include <polybori/iterators/CCuddLastIter.h>

// Getting output iterator functionality
#include <polybori/iterators/PBoRiOutIter.h>

// Getting error coe functionality
#include <polybori/except/PBoRiGenericError.h>

// test input idx_type
#include <polybori/common/CCheckedIdx.h>

#include <polybori/routines/pbori_algo.h>
#include <polybori/common/tags.h>
#include <polybori/routines/pbori_routines_hash.h>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <stdexcept>
#include <algorithm>
#include <numeric>

BEGIN_NAMESPACE_PBORI


/// Releasing raw pointers to decision diagrams here
template <class DataType>
inline void
extrusive_ptr_release(const DataType& data, DdNode* ptr) {
   if (ptr != NULL) {
     PBORI_PREFIX(Cudd_RecursiveDerefZdd)(data.getManager(), ptr);
   }
}

/// Incrememting reference counts to raw pointers to decision diagrams 
template <class DataType>
inline void 
extrusive_ptr_add_ref(const DataType&, DdNode* ptr) {
  if (ptr) PBORI_PREFIX(Cudd_Ref)(ptr);
}

/** @class CCuddDDFacade
 * @brief This template class defines a facade for decision diagrams.
 *
 **/

#define PBORI_NAME_Product product
#define PBORI_NAME_UnateProduct unateProduct
#define PBORI_NAME_WeakDiv weakDivide
#define PBORI_NAME_Divide divide
#define PBORI_NAME_WeakDivF weakDivF
#define PBORI_NAME_DivideF divideF
#define PBORI_NAME_Union unite
#define PBORI_NAME_Intersect intersect
#define PBORI_NAME_Diff diff
#define PBORI_NAME_Subset1 subset1
#define PBORI_NAME_Subset0 subset0
#define PBORI_NAME_Change change

#define PB_ZDD_APPLY(count, data, funcname) \
  diagram_type BOOST_PP_CAT(PBORI_NAME_, funcname)(data rhs) const {    \
    return apply(BOOST_PP_CAT(PBORI_PREFIX(Cudd_zdd), funcname),        \
                              rhs); }

template <class RingType, class DiagramType>
class CCuddDDFacade: 
  public CApplyNodeFacade<DiagramType, DdNode*>,
  public CAuxTypes {

  /// Type of *this
  typedef CCuddDDFacade self;
public:

  /// Type for output streams
  typedef CTypes::ostream_type ostream_type;

  /// Type for diagrams
  typedef RingType ring_type;

  /// Type for diagrams
  typedef DiagramType diagram_type;

  /// Type for C-style struct
  typedef DdNode node_type;

  /// Pointer type for nodes
  typedef node_type* node_ptr;

  /// Type this is inherited from the following
  typedef CApplyNodeFacade<diagram_type, node_ptr> base;

  /// Iterator type for retrieving first term from Cudd's ZDDs
  typedef CCuddFirstIter first_iterator;

  /// Iterator type for retrieving last term from Cudd's ZDDs
  typedef CCuddLastIter last_iterator;

  /// Iterator type for navigation throught Cudd's ZDDs structure
  typedef CCuddNavigator navigator;

  /// This type has an easy equality check
  typedef valid_tag easy_equality_property;

  /// Raw context
  typedef typename ring_type::mgr_type mgr_type;

  /// Cudd's index type
  typedef int cudd_idx_type;

  /// Cudd's index type
  typedef CCheckedIdx checked_idx_type;

  /// Construct diagram from ring and node
  CCuddDDFacade(const ring_type& ring, node_ptr node): 
    p_node(ring, node) {
    checkAssumption(node != NULL);
  }

  /// Construct from Manager and navigator
  CCuddDDFacade(const ring_type& ring, const navigator& navi): 
    p_node(ring, navi.getNode()) {
    checkAssumption(navi.isValid());
  }
  /// Construct new node from manager, index, and navigators
  CCuddDDFacade(const ring_type& ring, 
               idx_type idx, navigator thenNavi, navigator elseNavi): 
    p_node(ring, getNewNode(ring, idx, thenNavi, elseNavi)) { }

  /// Construct new node from manager, index, and common navigator for then and
  /// else-branches
  CCuddDDFacade(const ring_type& ring, 
               idx_type idx, navigator navi): 
    p_node(ring, getNewNode(ring, idx, navi, navi)) { }

  /// Construct new node
  CCuddDDFacade(idx_type idx, const self& thenDD, const self& elseDD):
    p_node(thenDD.ring(), getNewNode(idx, thenDD, elseDD)) { }

  /// Default constructor
  private: CCuddDDFacade(): p_node()  {}  ///  @todo NULL?
    public:
  /// Copy constructor
  CCuddDDFacade(const self &from): p_node(from.p_node) {}

  /// Destructor
  ~CCuddDDFacade() {}

  /// Assignment operator
  diagram_type& operator=(const diagram_type& rhs) {
    p_node = rhs.p_node;
    return static_cast<diagram_type&>(*this);
  }

  /// @note Preprocessor generated members
  /// @code
  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_APPLY, const diagram_type&, 
    (Product)(UnateProduct)(WeakDiv)(Divide)(WeakDivF)(DivideF)
    (Union)(Intersect)(Diff))

  BOOST_PP_SEQ_FOR_EACH(PB_ZDD_APPLY, int, (Subset1)(Subset0)(Change))
  /** @endcode */

  /// Performs the inclusion test for ZDDs
  bool implies(const self& rhs) const {
    return PBORI_PREFIX(Cudd_zddDiffConst)(getManager(), getNode(), rhs.getNode()) ==
      PBORI_PREFIX(Cudd_ReadZero)(getManager());
  }


  /// @name Functions for print useful information
  //@{
  std::ostream& printIntern(std::ostream& os) const {
    os << getNode() <<": ";

    if (isZero())
      os << "empty";
    else
      os << nNodes() << " nodes " <<  count() << "terms";

    return os;

  }
  void PrintMinterm() const  {
    checkAssumption(apply(PBORI_PREFIX(Cudd_zddPrintMinterm)) == 1);
  }
  //@}

  /// Determine the number of terms
  size_type count() const { return dd_long_count<size_type>(navigation());  }

  /// Appriximate the number of terms
  double countDouble() const { return dd_long_count<double>(navigation()); }

  /// Get index of curent node
  size_type rootIndex() const { return PBORI_PREFIX(Cudd_NodeReadIndex)(getNode()); }

  /// Number of nodes in the current decision diagram
  size_type nNodes() const { return CNodeCounter<navigator>()(navigation()); }

  /// Number of references pointing here
  size_type refCount() const { 
    PBORI_ASSERT(getNode() != NULL);
    return PBORI_PREFIX(Cudd_Regular)(getNode())->ref;
  }

  /// Test whether diagram represents the empty set
  bool isZero() const { return getNode() == PBORI_PREFIX(Cudd_ReadZero)(getManager()); }

  /// Test whether diagram represents the empty set
  bool isOne() const { return getNode() == DD_ONE(getManager()); }

  /// Get reference to ring
  const ring_type& ring() const { return p_node.data(); }

  /// Get raw node structure
  node_ptr getNode() const { return p_node.get(); }

  /// Get raw decision diagram manager
  mgr_type* getManager() const { return p_node.data().getManager(); }

protected:

  /// Apply C-style function to diagram
  using base::apply;


  template <class ResultType>
  ResultType memApply(ResultType (*func)(mgr_type *, node_ptr)) const {
    ResultType result = apply(func);
    checkAssumption(result != (ResultType) CUDD_OUT_OF_MEM);
    return result;
  }

  /// Check whether previous decision diagram operation for validity
  void checkAssumption(bool isValid) const {
    if PBORI_UNLIKELY(!isValid)
      throw std::runtime_error(error_text(getManager()));
  }

    /// temporarily (needs to be more generic) (similar fct in pbori_algo.h)
  template<class ManagerType, class ReverseIterator, class MultReverseIterator>
  diagram_type
  cudd_generate_multiples(const ManagerType& mgr, 
                          ReverseIterator start, ReverseIterator finish,
                          MultReverseIterator multStart, 
                          MultReverseIterator multFinish) const {

    // signed case
    while ((multStart != multFinish) && (*multStart > CTypes::max_idx))
      ++multStart;
    // unsigned case
    while ((multStart != multFinish) && (*multStart < 0))
      --multFinish;

    DdNode* prev( (getManager())->one );
    
    DdNode* zeroNode( (getManager())->zero ); 
    
    PBORI_PREFIX(Cudd_Ref)(prev);
    while(start != finish) {
      
      while((multStart != multFinish) && (*start < *multStart)) {

        DdNode* result = PBORI_PREFIX(cuddUniqueInterZdd)( getManager(), *multStart,
                                             prev, prev );

        PBORI_PREFIX(Cudd_Ref)(result);
        PBORI_PREFIX(Cudd_RecursiveDerefZdd)(getManager(), prev);

        prev = result;
        ++multStart;

      };

      DdNode* result = PBORI_PREFIX(cuddUniqueInterZdd)( getManager(), *start,
                                           prev, zeroNode );

      PBORI_PREFIX(Cudd_Ref)(result);
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(getManager(), prev);

      prev = result;


      if((multStart != multFinish) && (*start == *multStart))
        ++multStart;


      ++start;
    }

    while(multStart != multFinish) {

      DdNode* result = PBORI_PREFIX(cuddUniqueInterZdd)( getManager(), *multStart,
                                           prev, prev );

      PBORI_PREFIX(Cudd_Ref)(result);
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(getManager(), prev);

      prev = result;
      ++multStart;

    };

    PBORI_PREFIX(Cudd_Deref)(prev);


    return diagram_type(mgr, prev);
  }

    /// temporarily (needs to be more generic) (similar fct in pbori_algo.h)
  template<class ManagerType, class ReverseIterator>
  diagram_type
  cudd_generate_divisors(const ManagerType& mgr, 
                         ReverseIterator start, ReverseIterator finish) const {


    DdNode* prev= (getManager())->one;

    PBORI_PREFIX(Cudd_Ref)(prev);
    while(start != finish) {

      DdNode* result = PBORI_PREFIX(cuddUniqueInterZdd)( getManager(),
                                           *start, prev, prev);

      PBORI_PREFIX(Cudd_Ref)(result);
      PBORI_PREFIX(Cudd_RecursiveDerefZdd)(getManager(), prev);

      prev = result;
      ++start;
    }

    PBORI_PREFIX(Cudd_Deref)(prev);
    ///@todo Next line needs generalization 
      return diagram_type(mgr, prev);

}
public:

  /// Union Xor
  diagram_type Xor(const diagram_type& rhs) const {
    if (rhs.isZero())
      return *this;

    //    return apply(pboriPBORI_PREFIX(Cudd_zddUnionXor), rhs);
    base::checkSameManager(rhs);
    return diagram_type(ring(), pboriCudd_zddUnionXor(getManager(), getNode(), rhs.getNode()) );
  }

  /// Division with first term of right-hand side
  diagram_type divideFirst(const diagram_type& rhs) const {

    diagram_type result(*this);
    PBoRiOutIter<diagram_type, idx_type, subset1_assign<diagram_type> >  outiter(result);
    std::copy(rhs.firstBegin(), rhs.firstEnd(), outiter);

    return result;
  }


  /// Get number of nodes in decision diagram
  ostream_type& print(ostream_type& os) const {

    printIntern(os) << std::endl;;
    PrintMinterm();

    return os;
  }


  /// Get numbers of used variables
  //  size_type nSupport() const { return apply(PBORI_PREFIX(PBORI_PREFIX(Cudd_SupportSize));) }

  /// Start of first term
  first_iterator firstBegin() const {
    return first_iterator(navigation());
  }

  /// Finish of first term 
  first_iterator firstEnd() const { 
    return first_iterator();
  }

  /// Start of first term
  last_iterator lastBegin() const {
    return last_iterator(getNode());
  }

  /// Finish of first term 
  last_iterator lastEnd() const { 
    return last_iterator();
  }
  
  /// Get decison diagram representing the multiples of the first term
  diagram_type firstMultiples(const std::vector<idx_type>& input_multipliers) const {

    std::set<idx_type> multipliers(input_multipliers.begin(), input_multipliers.end());
    std::vector<idx_type> indices( std::distance(firstBegin(), firstEnd()) );

    std::copy( firstBegin(), firstEnd(), indices.begin() );

    return cudd_generate_multiples( ring(),
                                    indices.rbegin(), indices.rend(),
                                    multipliers.rbegin(),
                                    multipliers.rend() );
  }

  /// Get decison diagram representing the divisors of the first term
  diagram_type firstDivisors() const {

    std::vector<idx_type> indices( std::distance(firstBegin(), firstEnd()) );

    std::copy( firstBegin(), firstEnd(), indices.begin() );

    return cudd_generate_divisors(ring(), indices.rbegin(), indices.rend());
  }

  /// Navigate through ZDD by incrementThen(), incrementElse(), and terminated()
  navigator navigation() const {
    return navigator(getNode());
  }

  /// Checks whether the the diagram corresponds to {} or {{}} (polynomial 0 or 1)
  bool_type isConstant() const {
    return (getNode()) && PBORI_PREFIX(Cudd_IsConstant)(getNode());
  }



private:

  /// Save variant for generating fast ite operation (when idx < root index)
  static node_ptr
  getNewNode(const ring_type& ring, checked_idx_type idx, 
             navigator thenNavi, navigator elseNavi) {

    if ((idx >= *thenNavi) || (idx >= *elseNavi))
      throw PBoRiGenericError<CTypes::invalid_ite>();
    
    return PBORI_PREFIX(cuddZddGetNode)(ring.getManager(), idx, 
                          thenNavi.getNode(), elseNavi.getNode());
  }

  /// Convenience version for diagrams
  static node_ptr 
  getNewNode(idx_type idx, const self& thenDD, const self& elseDD) {
    thenDD.checkSameManager(elseDD);
    return getNewNode(thenDD.ring(), 
                      idx, thenDD.navigation(), elseDD.navigation());
  }

private:
  /// Smart pointer, which uses the ring for deallocating the node
  CExtrusivePtr<ring_type, node_type> p_node;
};


#undef PB_ZDD_APPLY

END_NAMESPACE_PBORI

#endif
