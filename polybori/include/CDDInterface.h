// -*- c++ -*-
//*****************************************************************************
/** @file CDDInterface.h
 *
 * @author Alexander Dreyer
 * @date  2006-03-20
 *
 * This file define specializations for the CDDInterface<> template class, which
 * allow unified access to various binary decision diagram implementations.
 *
 * @note Currently only interface to Cudd's ZDDs are available.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef CDDInterface_h_
#define CDDInterface_h_

#include "extrafwd.h"
// load basic definitions
#include "pbori_defs.h"



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

// Cudd's internal definitions
#include "cuddInt.h"

#include "pbori_algo.h"

#include "pbori_tags.h"
#include "pbori_routines_hash.h"

// Using stl's vector
#include <vector>
#include <numeric>

  //#include "CCuddInterface.h"
#include "pbori_traits.h"

#include "util.h"//FREE

BEGIN_NAMESPACE_PBORI



// inline CCuddInterface::mgrcore_ptr
// extract_manager(const CCuddInterface& mgr) {
//   return mgr.managerCore();
// }

template <class MgrType>
inline const MgrType&
extract_manager(const MgrType& mgr) {
  return mgr;
}


template <class MgrType>
inline const MgrType&
get_manager(const MgrType& mgr) {
  return mgr;
}
/** @class CDDInterfaceBase
 *
 * @brief This is the common base for the specialized template class
 * CDDInterface.
 *
 **/

template<class DDType>
class CDDInterfaceBase {

 public:

  /// The interfaced type
  typedef DDType interfaced_type;

  /// Generic access to type of *this
  typedef CDDInterfaceBase<interfaced_type> self;

  /// Default constructor
  CDDInterfaceBase() :
    m_interfaced() {}

  /// Construct instance from interfaced type
  CDDInterfaceBase(const interfaced_type& interfaced) :
    m_interfaced(interfaced) {}

  /// Copy constructor
  CDDInterfaceBase(const self& rhs) :
    m_interfaced(rhs.m_interfaced) {}

  /// Destructor
  ~CDDInterfaceBase() {}

  /// Constant casting operator to interfaced type
  operator const interfaced_type&() const { return m_interfaced; }

 protected:
  interfaced_type m_interfaced;
};

/// For Cudd-like ZDDs, like ZDD or CCuddZDD
/// @todo Generalize it
template<class CuddLikeZDD>
class CDDInterface:
 public CDDInterfaceBase<CuddLikeZDD> {
 public:
  
  /// Interfacing Cudd's zero-suppressed decision diagram type
  typedef CuddLikeZDD interfaced_type;
  
  /// Generic access to base type
  typedef CDDInterfaceBase<interfaced_type> base_type;
  typedef base_type base;
  using base::m_interfaced;

  /// Generic access to type of *this
  typedef CDDInterface<interfaced_type> self;

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

  /// Iterator type for retrieving first term from Cudd's ZDDs
  typedef CCuddFirstIter first_iterator;

  /// Iterator type for retrieving last term from Cudd's ZDDs
  typedef CCuddLastIter last_iterator;

  /// Iterator type for navigation throught Cudd's ZDDs structure
  typedef CCuddNavigator navigator;

  /// This type has an easy equality check
  typedef valid_tag easy_equality_property;

  //  typedef typename CuddLikeZDD::core_ptr ring_type;
  typedef typename CuddLikeZDD::ring_type ring_type;
  /// Default constructor
  CDDInterface(): base_type() {}

  /// Copy constructor
  CDDInterface(const self& rhs): base_type(rhs) {}

  /// Construct from interfaced type
  CDDInterface(const interfaced_type& rhs): base_type(rhs) {}

  /// Construct from Manager and navigator
  CDDInterface(const ring_type& ring, const navigator& navi): 
    base_type(self::newDiagram(ring, navi)) {}

  /// Construct new node from manager, index, and navigators
  CDDInterface(const ring_type& ring, 
               idx_type idx, navigator thenNavi, navigator elseNavi): 
    base_type( self::newNodeDiagram(ring, idx, thenNavi, elseNavi) ) {
  }

  /// Construct new node from manager, index, and common navigator for then and
  /// else-branches
  CDDInterface(const ring_type& ring, 
               idx_type idx, navigator navi): 
    base_type( self::newNodeDiagram(ring, idx, navi, navi) ) {
  }

  /// Construct new node
  CDDInterface(idx_type idx, const self& thenDD, const self& elseDD): 
    base_type( self::newNodeDiagram(thenDD.ring(), idx, 
                                    thenDD.navigation(), 
                                    elseDD.navigation()) ) {
  }

  /// Destructor
  ~CDDInterface() {}

  /// Get unique hash value (valid only per runtime)
  hash_type hash() const { 
    return static_cast<hash_type>(reinterpret_cast<std::ptrdiff_t>(m_interfaced
                                                                   .getNode()));
  }

  /// Get stable hash value, which is reproducible
  hash_type stableHash() const { 
    return stable_hash_range(navigation());
  }

  /// Set union
  self unite(const self& rhs) const {
    return self(base_type(m_interfaced.Union(rhs.m_interfaced)));
  };

  /// If-Then-Else operation
  self ite(const self& then_dd, const self& else_dd) const {
    return self(m_interfaced.Ite(then_dd, else_dd));
  };

  /// Set difference
  self diff(const self& rhs) const {
    return m_interfaced.Diff(rhs.m_interfaced);
  };

  /// Set difference
  self diffConst(const self& rhs) const {
    return m_interfaced.DiffConst(rhs.m_interfaced);
  };

  /// Set intersection
  self intersect(const self& rhs) const {
    return m_interfaced.Intersect(rhs.m_interfaced);
  };

  /// Product
  self product(const self& rhs) const {
    return m_interfaced.Product(rhs.m_interfaced);
  };

  /// Unate product
  self unateProduct(const self& rhs) const {
    return m_interfaced.UnateProduct(rhs.m_interfaced);
  };


  self Xor(const self& rhs) const {
    if (rhs.emptiness())
      return *this;

    return interfaced_type(m_interfaced.ring(),
            pboriCudd_zddUnionXor(
                getManager(),
                m_interfaced.getNode(),
                rhs.m_interfaced.getNode()));
  }

  /// Generate subset, where decision diagram manager variable idx is false
  self subset0(idx_type idx) const {
    return m_interfaced.Subset0(idx);
  };

  /// Generate subset, where decision diagram manager variable idx is asserted
  self subset1(idx_type idx) const {
    return m_interfaced.Subset1(idx);
  };

  /// Substitute variable with index idx by its complement
  self change(idx_type idx) const {    

    return m_interfaced.Change(idx);
  };

  /// Division
  self ddDivide(const self& rhs) const {
    return m_interfaced.Divide(rhs);
  };

  /// Weak division
  self weakDivide(const self& rhs) const {
    return m_interfaced.WeakDiv(rhs);
  };

  /// Division with first term of right-hand side
  self divideFirst(const self& rhs) const {

    self result(*this);
    PBoRiOutIter<self, idx_type, subset1_assign<self> >  outiter(result);
    std::copy(rhs.firstBegin(), rhs.firstEnd(), outiter);

    return result;
  }


  /// Get number of nodes in decision diagram
  size_type nNodes() const { return m_interfaced.nNodes(); }

  /// Get number of nodes in decision diagram
  ostream_type& print(ostream_type& os) const {

    m_interfaced.print(os) << std::endl;;
    m_interfaced.PrintMinterm();

    return os;
  }


  /// Equality check
  bool_type operator==(const self& rhs) const {
    return (m_interfaced == rhs.m_interfaced);
  }

  /// Nonequality check
  bool_type operator!=(const self& rhs) const {
    return (m_interfaced != rhs.m_interfaced);
  }
  /// Get reference to ring
  const ring_type& ring() const {
    return m_interfaced.ring();
  }

  /// Get reference to actual decision diagram manager 
  DdManager* getManager() const {
    return ring().getManager();
  }

  /// Get numbers of used variables
  size_type nSupport() const {
    return Cudd_SupportSize(getManager(), m_interfaced.getNode());
  }

  /// Get multiples of used variables
  self support() const {

    DdNode* tmp = Cudd_Support(getManager(), m_interfaced.getNode());
    Cudd_Ref(tmp);
 
    self result = interfaced_type(m_interfaced.ring(),  
      Cudd_zddPortFromBdd(getManager(), tmp));
    Cudd_RecursiveDeref(getManager(), tmp);        

    return result;
  }

  /// Get used variables (assuming indices of zero length)
  template<class VectorLikeType>
  void usedIndices(VectorLikeType& indices) const {

    int* pIdx = Cudd_SupportIndex( getManager(), 
                                   m_interfaced.getNode() );



    size_type nlen(nVariables());

    indices.reserve(std::accumulate(pIdx, pIdx + nlen, size_type()));

    for(size_type idx = 0; idx < nlen; ++idx)
      if (pIdx[idx] == 1){
        indices.push_back(idx);
      }
    FREE(pIdx);
  }

  /// Get used variables (assuming indices of length nSupport())
  int* usedIndices() const {

    return Cudd_SupportIndex( getManager(), 
                                   m_interfaced.getNode() );


  }

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
    return last_iterator(m_interfaced.getNode());
  }

  /// Finish of first term 
  last_iterator lastEnd() const { 
    return last_iterator();
  }

  /// temporarily (needs to be more generic)
  template<class ManagerType, class ReverseIterator, class MultReverseIterator>
  interfaced_type
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


    return interfaced_type(mgr, prev);
  }
  
  /// Get decison diagram representing the multiples of the first term
  self firstMultiples(const std::vector<idx_type>& multipliers) const {

    std::vector<idx_type> indices( std::distance(firstBegin(), firstEnd()) );

    std::copy( firstBegin(), firstEnd(), indices.begin() );

    return cudd_generate_multiples( ring(),
                                    indices.rbegin(), indices.rend(),
                                    multipliers.rbegin(),
                                    multipliers.rend() );
  }




  /// temporarily (needs to be more generic)
  template<class ManagerType, class ReverseIterator>
  interfaced_type
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
      return interfaced_type(mgr, prev);

}

  /// Get decison diagram representing the divisors of the first term
  self firstDivisors() const {

    std::vector<idx_type> indices( std::distance(firstBegin(), firstEnd()) );

    std::copy( firstBegin(), firstEnd(), indices.begin() );

    return cudd_generate_divisors(ring(), indices.rbegin(), indices.rend());
  }

  /// Navigate through ZDD by incrementThen(), incrementElse(), and terminated()
  navigator navigation() const {
    return navigator(m_interfaced.getNode());
  }

  /// Checks whether the decision diagram is empty
  bool_type emptiness() const {
    return ( m_interfaced.isZero());
  }

  /// Checks whether the decision diagram has every variable negated
  bool_type blankness() const { return m_interfaced.isOne(); }

  bool_type isConstant() const {
    return (m_interfaced.getNode()) && Cudd_IsConstant(m_interfaced.getNode());
  }

  /// Returns number of terms
  size_type size() const {
    return m_interfaced.count();
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
    return m_interfaced.countDouble();
  }

private:
  navigator newNode(const ring_type& mgr, idx_type idx, 
                    navigator thenNavi, navigator elseNavi) const {
    assert(idx < *thenNavi);
    assert(idx < *elseNavi); 
    return navigator(cuddZddGetNode(mgr.getManager(), idx, 
                                    thenNavi.getNode(), elseNavi.getNode()));
  }

  interfaced_type newDiagram(const ring_type& mgr, navigator navi) const { 
    return interfaced_type(mgr, navi.getNode());
  }

  self fromTemporaryNode(const navigator& navi) const { 
    navi.decRef();
    return self(ring(), navi.getNode());
  }


  interfaced_type newNodeDiagram(const ring_type& mgr, idx_type idx, 
                                 navigator thenNavi, 
                                 navigator elseNavi) const {
    if ((idx >= *thenNavi) || (idx >= *elseNavi))
      throw PBoRiGenericError<CTypes::invalid_ite>();

    return newDiagram(mgr, newNode(mgr, idx, thenNavi, elseNavi) );
  }

  interfaced_type newNodeDiagram(const ring_type& mgr, 
                                 idx_type idx, navigator navi) const {
    if (idx >= *navi)
      throw PBoRiGenericError<CTypes::invalid_ite>();

    navi.incRef();
    interfaced_type result =
      newDiagram(mgr, newNode(mgr, idx, navi, navi) );
    navi.decRef();
    return result;
  }



};





/// Stream output operator
template <class DDType>
typename CDDInterface<DDType>::ostream_type& 
operator<<( typename CDDInterface<DDType>::ostream_type& os, 
            const CDDInterface<DDType>& dd ) {
  return dd.print(os);
}

END_NAMESPACE_PBORI

#endif // of #ifndef CDDInterface_h_
