// -*- c++ -*-
//*****************************************************************************
/** @file CDDFacade.h 
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

// include basic definitions
#include "pbori_defs.h"

#ifndef CDDFacade_h
#define CDDFacade_h

#include "cuddInt.h"
#include "CCuddZDD.h"


// Getting iterator type for navigating through Cudd's ZDDs structure
#include "CCuddNavigator.h"

// Getting iterator type for retrieving first term from Cudd's ZDDs
#include "CCuddFirstIter.h"

// Getting iterator type for retrieving last term from Cudd's ZDDs
#include "CCuddLastIter.h"

// Getting output iterator functionality
#include "PBoRiOutIter.h"

// Getting error coe functionality
#include "PBoRiGenericError.h"

#include "pbori_algo.h"
#include "pbori_tags.h"
#include "pbori_routines_hash.h"

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <stdexcept>
#include <algorithm>
#include <numeric>

BEGIN_NAMESPACE_PBORI

/** @class CDDFacade
 * @brief This template class defines a facade for decision diagrams.
 *
 **/


#define PB_ZDD_APPLY(count, data, funcname) \
  self funcname(data rhs) const {    \
    return apply(BOOST_PP_CAT(Cudd_zdd, funcname), rhs); }

template <class RingType, class DiagramType>
class CDDFacade: 
  public CCuddDDBase<RingType, DiagramType, DdNode> {

  /// Type of *this
  typedef CDDFacade self;
public:

  /// Define size type
  typedef CTypes::size_type size_type;

  /// Define degree type
  typedef CTypes::deg_type deg_type;

  /// Define index type
  typedef CTypes::idx_type idx_type;

  /// Type for output streams
  typedef CTypes::ostream_type ostream_type;

  /// Type for comparisons
  typedef CTypes::bool_type bool_type;

  /// Type for hashed
  typedef CTypes::hash_type hash_type;
  /// Type for diagrams
  typedef RingType ring_type;

  /// Type for diagrams
  typedef DiagramType diagram_type;

  /// Type for C-style struct
  typedef DdNode node_type;

  /// Iterator type for retrieving first term from Cudd's ZDDs
  typedef CCuddFirstIter first_iterator;

  /// Iterator type for retrieving last term from Cudd's ZDDs
  typedef CCuddLastIter last_iterator;

  /// Iterator type for navigation throught Cudd's ZDDs structure
  typedef CCuddNavigator navigator;

  /// This type has an easy equality check
  typedef valid_tag easy_equality_property;


  /// Type this is inherited from
  typedef CCuddDDBase<RingType, DiagramType, DdNode> base;

  /// Pointer type for ndoes
  typedef typename  base::node_ptr node_ptr;

  using base::getNode;
  using base::apply;
  using base::getManager;


  typedef typename  base::mgr_type mgr_type;

  /// Construct diagram from ring and node
  CDDFacade(const ring_type& ring, node_ptr node): base(ring, node) {
    checkAssumption(node != NULL);
  }
  /// Construct from Manager and navigator
  CDDFacade(const ring_type& ring, const navigator& navi): 
    base(self::newDiagram(ring, navi)) {}

  /// Construct new node from manager, index, and navigators
  CDDFacade(const ring_type& ring, 
               idx_type idx, navigator thenNavi, navigator elseNavi): 
    base( self::newNodeDiagram(ring, idx, thenNavi, elseNavi) ) {
  }

  /// Construct new node from manager, index, and common navigator for then and
  /// else-branches
  CDDFacade(const ring_type& ring, 
               idx_type idx, navigator navi): 
    base( self::newNodeDiagram(ring, idx, navi, navi) ) {
  }

  /// Construct new node
  CDDFacade(idx_type idx, const self& thenDD, const self& elseDD): 
    base( self::newNodeDiagram(thenDD.ring(), idx, 
                                    thenDD.navigation(), 
                                    elseDD.navigation()) ) {
  }
  /// Default constructor
  CDDFacade(): base() {}

  /// Copy constructor
  CDDFacade(const self &from): base(from) {}

  /// Destructor
  ~CDDFacade() {}

  /// Assignment operator
  // self& operator=(const self& right); // inlined below

  /// @name Logical operations
  //@{
  bool operator==(const self& other) const {
    return base::getNode() == other.getNode();
  }
  bool operator!=(const self& other) const { return !(*this == other); }
  //@}

  /// @note Preprocessor generated members
  /// @code
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
  std::ostream& printIntern(std::ostream& os) const {
    os << getNode() <<": ";

    if (isZero())
      os << "empty";
    else
      os << nNodes() << " nodes " <<  count() << "terms";

    return os;

  }
  void PrintMinterm() const  {
    checkAssumption(apply(Cudd_zddPrintMinterm) == 1);
  }
  //@}

  /// Determine the number of terms
  int count() const          { return memApply(Cudd_zddCount); }

  /// Appriximate the number of terms
  double countDouble() const { return memApply(Cudd_zddCountDouble); }
  /// Get index of curent node
  size_type rootIndex() const { return Cudd_NodeReadIndex(getNode()); }

  /// Number of nodes in the current decision diagram
  size_type nNodes() const { return (size_type)(Cudd_DagSize(getNode())); }

  /// Number of references pointing here
  size_type refCount() const { 
    assert(getNode() != NULL);
    return Cudd_Regular(getNode())->ref;
  }

  /// Test whether diagram represents the empty set
  bool isZero() const { return getNode() == Cudd_ReadZero(getManager()); }

  /// Test whether diagram represents the empty set
  bool isOne() const { return getNode() == DD_ONE(getManager()); }

  /// Get reference to ring
  const ring_type& ring() const { return base::p_node.data(); }

protected:

  template <class ResultType>
  ResultType memApply(ResultType (*func)(mgr_type *, node_ptr)) const {
    ResultType result = apply(func);
    checkAssumption(result != (ResultType) CUDD_OUT_OF_MEM);
    return result;
  }

  /// Check whether previous decision diagram operation for validity
  void checkAssumption(bool isValid) const {
    if UNLIKELY(!isValid)
      throw std::runtime_error(error_text(getManager()));
  }

public:
  /// Get unique hash value (valid only per runtime)
  hash_type hash() const { 
    return static_cast<hash_type>(reinterpret_cast<std::ptrdiff_t>(getNode()));
  }

  /// Get stable hash value, which is reproducible
  hash_type stableHash() const { 
    return stable_hash_range(navigation());
  }

  /// Set union
  diagram_type unite(const diagram_type& rhs) const {
    return Union(rhs);
  };

  /// If-Then-Else operation
  diagram_type ite(const diagram_type& then_dd, const diagram_type& else_dd) const {
    return Ite(then_dd, else_dd);
  };

  /// Set difference
  diagram_type diff(const diagram_type& rhs) const {
    return Diff(rhs);
  };

  /// Set difference
  diagram_type diffConst(const diagram_type& rhs) const {
    return DiffConst(rhs);
  };

  /// Set intersection
  diagram_type intersect(const diagram_type& rhs) const {
    return Intersect(rhs);
  };

  /// Product
  diagram_type product(const diagram_type& rhs) const {
    return Product(rhs);
  };

  /// Unate product
  diagram_type unateProduct(const diagram_type& rhs) const {
    return UnateProduct(rhs);
  };


  diagram_type Xor(const diagram_type& rhs) const {
    if (rhs.isZero())
      return *this;

    return base::apply(pboriCudd_zddUnionXor, rhs);
  }

  /// Generate subset, where decision diagram manager variable idx is false
  diagram_type subset0(idx_type idx) const {
    return Subset0(idx);
  };

  /// Generate subset, where decision diagram manager variable idx is asserted
  diagram_type subset1(idx_type idx) const {
    return Subset1(idx);
  };

  /// Substitute variable with index idx by its complement
  diagram_type change(idx_type idx) const {    

    return Change(idx);
  };

  /// Division
  diagram_type ddDivide(const diagram_type& rhs) const {
    return Divide(rhs);
  };

  /// Weak division
  diagram_type weakDivide(const diagram_type& rhs) const {
    return WeakDiv(rhs);
  };

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
  size_type nSupport() const {
    return base::apply(Cudd_SupportSize);
  }

  /// Get multiples of used variables
  diagram_type support() const {

    DdNode* tmp = Cudd_Support(getManager(), getNode());
    Cudd_Ref(tmp);
 
    diagram_type result(ring(), Cudd_zddPortFromBdd(getManager(), tmp));
    Cudd_RecursiveDeref(getManager(), tmp);        

    return result;
  }

  /// Get used variables (assuming indices of zero length)
  template<class VectorLikeType>
  void usedIndices(VectorLikeType& indices) const {

    int* pIdx =  usedIndices();
    size_type nlen(nVariables());

    indices.reserve(std::accumulate(pIdx, pIdx + nlen, size_type()));

    for(size_type idx = 0; idx < nlen; ++idx)
      if (pIdx[idx] == 1){
        indices.push_back(idx);
      }
    FREE(pIdx);
  }

  /// Get used variables (assuming indices of length nSupport())
  int* usedIndices() const { return apply(Cudd_SupportIndex); }

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

  /// temporarily (needs to be more generic)
  template<class ManagerType, class ReverseIterator, class MultReverseIterator>
  diagram_type
  cudd_generate_multiples(const ManagerType& mgr, 
                          ReverseIterator start, ReverseIterator finish,
                          MultReverseIterator multStart, 
                          MultReverseIterator multFinish) const {

    DdNode* prev( (getManager())->one );
    
    DdNode* zeroNode( (getManager())->zero ); 
    
    Cudd_Ref(prev);
    while(start != finish) {
      
      while((multStart != multFinish) && (*start < *multStart)) {

        DdNode* result = cuddUniqueInterZdd( getManager(), *multStart,
                                             prev, prev );

        Cudd_Ref(result);
        Cudd_RecursiveDerefZdd(getManager(), prev);

        prev = result;
        ++multStart;

      };

      DdNode* result = cuddUniqueInterZdd( getManager(), *start,
                                           prev, zeroNode );

      Cudd_Ref(result);
      Cudd_RecursiveDerefZdd(getManager(), prev);

      prev = result;


      if((multStart != multFinish) && (*start == *multStart))
        ++multStart;


      ++start;
    }

    while(multStart != multFinish) {

      DdNode* result = cuddUniqueInterZdd( getManager(), *multStart,
                                           prev, prev );

      Cudd_Ref(result);
      Cudd_RecursiveDerefZdd(getManager(), prev);

      prev = result;
      ++multStart;

    };

    Cudd_Deref(prev);


    return diagram_type(mgr, prev);
  }
  
  /// Get decison diagram representing the multiples of the first term
  diagram_type firstMultiples(const std::vector<idx_type>& multipliers) const {

    std::vector<idx_type> indices( std::distance(firstBegin(), firstEnd()) );

    std::copy( firstBegin(), firstEnd(), indices.begin() );

    return cudd_generate_multiples( ring(),
                                    indices.rbegin(), indices.rend(),
                                    multipliers.rbegin(),
                                    multipliers.rend() );
  }




  /// temporarily (needs to be more generic)
  template<class ManagerType, class ReverseIterator>
  diagram_type
  cudd_generate_divisors(const ManagerType& mgr, 
                         ReverseIterator start, ReverseIterator finish) const {


    DdNode* prev= (getManager())->one;

    Cudd_Ref(prev);
    while(start != finish) {
 
      DdNode* result = cuddUniqueInterZdd( getManager(), *start,
                                           prev, prev);

      Cudd_Ref(result);
      Cudd_RecursiveDerefZdd(getManager(), prev);
 
      prev = result;
      ++start;
    }

    Cudd_Deref(prev);
    ///@todo Next line needs generalization 
      return diagram_type(mgr, prev);

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

  /// Checks whether the decision diagram is empty
  bool_type emptiness() const {// to be removed
    return (isZero());
  }

  /// Checks whether the decision diagram has every variable negated
  bool_type blankness() const { return isOne(); }  // to be removed

  bool_type isConstant() const {
    return (getNode()) && Cudd_IsConstant(getNode());
  }

  /// Returns number of terms
  size_type size() const {
    return count();
  }

  /// Returns number of terms (deprecated)
  size_type length() const {
    return size();
  }

  /// Returns number of variables in manager
  size_type nVariables() const {
   return Cudd_ReadZddSize(getManager() );
  }


  /// Test whether the empty set is included
  bool_type ownsOne() const { return owns_one(navigation()); }

  /// Approximation of number of terms
  double sizeDouble() const {
    return countDouble();
  }

private:
  navigator newNode(const ring_type& mgr, idx_type idx, 
                    navigator thenNavi, navigator elseNavi) const {
    assert(idx < *thenNavi);
    assert(idx < *elseNavi); 
    return navigator(cuddZddGetNode(mgr.getManager(), idx, 
                                    thenNavi.getNode(), elseNavi.getNode()));
  }

  diagram_type newDiagram(const ring_type& mgr, navigator navi) const { 
    return diagram_type(mgr, navi.getNode());
  }

  diagram_type fromTemporaryNode(const navigator& navi) const { 
    navi.decRef();
    return diagram_type(ring(), navi.getNode());
  }


  diagram_type newNodeDiagram(const ring_type& mgr, idx_type idx, 
                                 navigator thenNavi, 
                                 navigator elseNavi) const {
    if ((idx >= *thenNavi) || (idx >= *elseNavi))
      throw PBoRiGenericError<CTypes::invalid_ite>();

    return newDiagram(mgr, newNode(mgr, idx, thenNavi, elseNavi) );
  }

  diagram_type newNodeDiagram(const ring_type& mgr, 
                                 idx_type idx, navigator navi) const {
    if (idx >= *navi)
      throw PBoRiGenericError<CTypes::invalid_ite>();

    navi.incRef();
    diagram_type result =
      newDiagram(mgr, newNode(mgr, idx, navi, navi) );
    navi.decRef();
    return result;
  }

};


#undef PB_ZDD_APPLY

END_NAMESPACE_PBORI

#endif
