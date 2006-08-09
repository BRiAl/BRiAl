// -*- c++ -*-
//*****************************************************************************
/** @file 
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
 *   (c) 2006 by
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
 * Revision 1.22  2006/08/09 12:52:31  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * Revision 1.21  2006/08/01 11:14:17  dreyer
 * CHANGE: Bug fixed, now Cudd's external ref/deref command (crashed on 64 bit)
 *
 * Revision 1.20  2006/07/31 11:48:53  dreyer
 * ADD: lowlevel implementation for multiples and lmDivisors
 *
 * Revision 1.19  2006/06/08 08:54:33  dreyer
 * FIX FREEs memory
 *
 * Revision 1.18  2006/06/07 11:54:26  dreyer
 * ADD variantes for usedVariables
 *
 * Revision 1.17  2006/06/07 08:37:50  dreyer
 * ADD CCuddLastIter and BooleSet::lastLexicographicalTerm()
 *
 * Revision 1.16  2006/05/05 09:03:44  dreyer
 * ADD different implementation of the addition
 *
 * Revision 1.15  2006/04/20 08:31:21  dreyer
 * ADD BooleMonomial::mulples(...)
 *
 * Revision 1.14  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.13  2006/04/06 13:54:58  dreyer
 * ADD BoolePolynomial::length()
 *
 * Revision 1.12  2006/04/05 15:26:04  dreyer
 * CHANGE: File access of BoolePolynomial::prettyPrint moved to CDDInterface
 *
 * Revision 1.11  2006/04/05 14:56:38  dreyer
 * ADD prettyPrint functions for dot-output to stdout or file
 *
 * Revision 1.10  2006/04/04 15:31:06  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * Revision 1.9  2006/04/04 07:36:35  dreyer
 * ADD: tests isZero(), isOne() and poly == bool, bool == poly
 *
 * Revision 1.8  2006/03/30 12:21:20  dreyer
 * CHANGE: more consistency
 *
 * Revision 1.7  2006/03/30 08:59:42  dreyer
 * FIX: CCuddFirstIter works for empty and zero polynomials now
 *
 * Revision 1.6  2006/03/29 16:26:46  dreyer
 * ADD: Class CCuddFirstIter used for BoolePolynomial::lead()
 *
 * Revision 1.5  2006/03/27 15:02:43  dreyer
 * ADD: BoolePolynomial::operator/=(const self&) and spoly
 *
 * Revision 1.4  2006/03/24 16:15:15  dreyer
 * CHANGE: (n)usedVariables() now uses Cudd-internal commands
 * ADD: CDDInterface<> support() and nSupport() (for above)
 *
 * Revision 1.3  2006/03/24 15:02:44  dreyer
 * ADD: Reference to manager_type can also be used for CDDManager<> -nterface
 * ADD: lead(), (n)usedVariables(), lmDeg() implemented in BoolePolynomial
 *
 * Revision 1.2  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.1  2006/03/20 14:51:00  dreyer
 * CHANGE: Use CDDInterface temple specializations instead of raw dd_type
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef CDDInterface_h_
#define CDDInterface_h_

// load basic definitions
#include "pbori_defs.h"

// Getting iterator type for navigating through Cudd's ZDDs structure
#include "CCuddNavigator.h"

// Getting iterator type for retrieving first term from Cudd's ZDDs
#include "CCuddFirstIter.h"

// Getting iterator type for retrieving last term from Cudd's ZDDs
#include "CCuddLastIter.h"

// Getting functional for generating new Cudd's ZDD nodes
#include "CCuddGetNode.h"

// Cudd's internal definitions
#include "cuddInt.h"

#include "pbori_algo.h"

// Using stl's vector
#include <vector>

BEGIN_NAMESPACE_PBORI

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

  /// Casting operator to interfaced type
  operator interfaced_type&() { return m_interfaced; }

  /// Constant casting operator to interfaced type
    operator const interfaced_type&() const { return m_interfaced; }

 protected:
  interfaced_type m_interfaced;
};

/** @class CDDInterface<ZDD>
 *
 * @brief this specialization of the template class CDDInterface gives an
 * interface to Cudd's ZDD type.
 *
 **/

template<>
class CDDInterface<ZDD>:
 public CDDInterfaceBase<ZDD> {
 public:
  
  /// Interfacing Cudd's zero-suppressed decision diagram type
  typedef ZDD interfaced_type;
  
  /// Cudd's decision diagram manager type
  typedef Cudd manager_base;

  /// Interface to Cudd's decision diagram manager type
  typedef CDDManager<Cudd> manager_type;

  /// Generic access to base type
  typedef CDDInterfaceBase<interfaced_type> base_type;

  /// Generic access to type of *this
  typedef CDDInterface<interfaced_type> self;

  /// Define size type
  typedef CTypes::size_type size_type;

  /// Define index type
  typedef CTypes::idx_type idx_type;

  /// Type for output streams
  typedef CTypes::ostream_type ostream_type;

  /// Type for comparisons
  typedef CTypes::bool_type bool_type;

  /// Iterator type for retrieving first term from Cudd's ZDDs
  typedef CCuddFirstIter first_iterator;

  /// Iterator type for retrieving last term from Cudd's ZDDs
  typedef CCuddLastIter last_iterator;

  /// Iterator type for navigation throught Cudd's ZDDs structure
  typedef CCuddNavigator navigator;

  /// Type for output of pretty print
  typedef FILE* pretty_out_type;

  /// Type for file name of pretty print output
  typedef const char* filename_type;

  /// Default constructor
  CDDInterface(): base_type() {}

  /// Copy constructor
  CDDInterface(const self& rhs): base_type(rhs) {}

  /// Construct from interfaced type
  CDDInterface(const interfaced_type& rhs): base_type(rhs) {}

  /// Destructor
  ~CDDInterface() {}
  /// Set union
  self unite(const self& rhs) const {
    return self(base_type(m_interfaced.Union(rhs.m_interfaced)));
  };

  /// Set union with assignment
  self& uniteAssign(const self& rhs) {
    m_interfaced = m_interfaced.Union(rhs.m_interfaced);
    return *this;
  };
  /// If-Then-Else operation
  self ite(const self& then_dd, const self& else_dd) const {
    return self(m_interfaced.Ite(then_dd, else_dd));
  };

  /// If-Then-Else operation with assignment
  self& iteAssign(const self& then_dd, const self& else_dd) {
    m_interfaced = m_interfaced.Ite(then_dd, else_dd);
    return *this;
  };

  /// Set difference
  self diff(const self& rhs) const {
    return m_interfaced.Diff(rhs.m_interfaced);
  };

  /// Set difference with assignment
  self& diffAssign(const self& rhs) {
    m_interfaced = m_interfaced.Diff(rhs.m_interfaced);
    return *this;
  };

  /// Set difference
  self diffConst(const self& rhs) const {
    return m_interfaced.DiffConst(rhs.m_interfaced);
  };

  /// Set difference with assignment
  self& diffConstAssign(const self& rhs) {
    m_interfaced = m_interfaced.DiffConst(rhs.m_interfaced);
    return *this;
  };

  /// Set intersection
  self intersect(const self& rhs) const {
    return m_interfaced.Intersect(rhs.m_interfaced);
  };

  /// Set intersection with assignment
  self& intersectAssign(const self& rhs) {
    m_interfaced = m_interfaced.Intersect(rhs.m_interfaced);
    return *this;
  };

  /// Product
  self product(const self& rhs) const {
    return m_interfaced.Product(rhs.m_interfaced);
  };

  /// Product with assignment
  self& productAssign(const self& rhs) {
    m_interfaced = m_interfaced.Product(rhs.m_interfaced);
    return *this;
  };

  /// Unate product
  self unateProduct(const self& rhs) const {
    return m_interfaced.UnateProduct(rhs.m_interfaced);
  };

  /// Unate product with assignment
  self& unateProductAssign(const self& rhs) {
    m_interfaced = m_interfaced.UnateProduct(rhs.m_interfaced);
    return *this;
  };

  /// Generate subset, where decision diagram manager variable idx is false
  self subset0(idx_type idx) const {
    return m_interfaced.Subset0(idx);
  };

  /// subset0 with assignment
  self& subset0Assign(idx_type idx) {
    m_interfaced = m_interfaced.Subset0(idx);
    return *this;
  };

  /// Generate subset, where decision diagram manager variable idx is asserted
  self subset1(idx_type idx) const {
    return m_interfaced.Subset1(idx);
  };

  /// subset1 with assignment
  self& subset1Assign(idx_type idx) {
    m_interfaced = m_interfaced.Subset1(idx);
    return *this;
  };

  /// Substitute variable with index idx by its complement
  self change(idx_type idx) const {    

    return m_interfaced.Change(idx);
  };

  /// Change with assignment
  self& changeAssign(idx_type idx) {
    m_interfaced = m_interfaced.Change(idx);
    return *this;
  };

  /// Division
  self divide(const self& rhs) const {
    return m_interfaced.Divide(rhs);
  };

  /// Division with assignment
  self& divideAssign(const self& rhs) {
    m_interfaced = m_interfaced.Divide(rhs);
    return *this;
  };
  /// Weak division
  self weakDivide(const self& rhs) const {
    return m_interfaced.WeakDiv(rhs);
  };

  /// Weak division with assignment
  self& weakDivideAssign(const self& rhs) {
    m_interfaced = m_interfaced.WeakDiv(rhs);
    return *this;
  };

  /// Get number of nodes in decision diagram
  size_type nNodes() const {
    return Cudd_zddDagSize(m_interfaced.getNode());
  }

  /// Get number of nodes in decision diagram
  ostream_type& print(ostream_type& os) const {

    FILE* oldstdout = manager().ReadStdout();

    /// Enable ostream cout and cerr (at least)
    if (os == std::cout)
      manager().SetStdout(stdout);
    else if (os == std::cerr)
      manager().SetStdout(stderr);

    m_interfaced.print( Cudd_ReadZddSize(manager().getManager()) );
    m_interfaced.PrintMinterm();

    manager().SetStdout(oldstdout);
    return os;
  }

  /// Print Dot-output to file given by file handle
  void prettyPrint(pretty_out_type filehandle = stdout) const {

    ZDDvector dummyvec(1, &manager());
    dummyvec[0] = m_interfaced;

    dummyvec.DumpDot( NULL, NULL, filehandle );
  };

  /// Print Dot-output to file given by file name
  bool_type prettyPrint(filename_type filename) const {

    FILE* theFile = fopen( filename, "w");
    if (theFile == NULL)
      return true;

    prettyPrint(theFile);
    fclose(theFile);

    return false;
 };

  /// Equality check
  bool_type operator==(const self& rhs) const {
    return (m_interfaced == rhs.m_interfaced);
  }

  /// Nonequality check
  bool_type operator!=(const self& rhs) const {
    return (m_interfaced != rhs.m_interfaced);
  }

  /// Get reference to actual decision diagram manager 
  manager_base& manager() const {
    return *m_interfaced.manager();
  }

  /// Get numbers of used variables
  size_type nSupport() const {
    return Cudd_SupportSize(manager().getManager(), m_interfaced.getNode());
  }

  /// Get multiples of used variables
  self support() const {

    BDD supp( &manager(), 
              Cudd_Support(manager().getManager(), m_interfaced.getNode()) );

    return supp.PortToZdd();
  }

  /// Get used variables (assuming indices of length nSupport())
  void usedIndices(std::vector<idx_type>& indices) const {

    int* pIdx = Cudd_SupportIndex( manager().getManager(), 
                                   m_interfaced.getNode() );

    size_type nlen(nVariables());

    std::vector<idx_type>::iterator iter(indices.begin());
    for(size_type idx = 0; idx < nlen; ++idx)
      if (pIdx[idx] == 1){
        *iter = idx;
        ++iter;
      }
    FREE(pIdx);
  }

  /// Get used variables (assuming indices of length nSupport())
  int* usedIndices() const {

    return Cudd_SupportIndex( manager().getManager(), 
                                   m_interfaced.getNode() );


//     size_type nlen(nVariables());

//     std::vector<idx_type>::iterator iter(indices.begin());
//     for(size_type idx = 0; idx < nlen; ++idx)
//       if (pIdx[idx] == 1){
//         *iter = idx;
//         ++iter;
//       }
//     FREE(pIdx);
  }


  /// Start of first term
  first_iterator firstBegin() const {
    return first_iterator(m_interfaced.getNode());
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

  /// Get decison diagram representing the multiples of the first term
  self firstMultiples(const std::vector<idx_type>& multipliers) const {

    std::vector<idx_type> indices( std::distance(firstBegin(), firstEnd()) );

    std::copy( firstBegin(), firstEnd(), indices.begin() );

    DdNode* prev(DD_ONE(manager().getManager()));

    DdNode* zeroNode(DD_ZERO(manager().getManager())); 


    std::vector<idx_type>::const_reverse_iterator 
      start(indices.rbegin()), finish(indices.rend()),
      multStart(multipliers.rbegin()), multFinish(multipliers.rend());


    Cudd_Ref(prev);
    while(start != finish) {

      while((multStart != multFinish) && (*start < *multStart)) {

        DdNode* result = cuddUniqueInterZdd( manager().getManager(), *multStart,
                                             prev, prev );

        Cudd_Ref(result);
        Cudd_RecursiveDerefZdd(manager().getManager(), prev);

        prev = result;
        ++multStart;

      };

      DdNode* result = cuddUniqueInterZdd( manager().getManager(), *start,
                                           prev, zeroNode );

      Cudd_Ref(result);
      Cudd_RecursiveDerefZdd(manager().getManager(), prev);

      prev = result;


      if((multStart != multFinish) && (*start == *multStart))
        ++multStart;


      ++start;
    }

    Cudd_Deref(prev);

    return interfaced_type(&manager(), prev);
  }

  /// Get decison diagram representing the divisors of the first term
  self firstDivisors() const {

    std::vector<idx_type> indices( std::distance(firstBegin(), firstEnd()) );

    std::copy( firstBegin(), firstEnd(), indices.begin() );

    DdNode* prev= DD_ONE(manager().getManager());


    std::vector<idx_type>::const_reverse_iterator 
      start(indices.rbegin()), finish(indices.rend());

    Cudd_Ref(prev);
    while(start != finish) {
 
      DdNode* result = cuddUniqueInterZdd( manager().getManager(), *start,
                                           prev, prev);

      Cudd_Ref(result);
      Cudd_RecursiveDerefZdd(manager().getManager(), prev);
 
      prev = result;
      ++start;
    }

    Cudd_Deref(prev);
 
    return interfaced_type(&manager(), prev);
  }

  self firstDivisorsOf(const self& rhs) const {

    navigator navi =
      dd_intersect_some_index( navigation(), rhs.firstBegin(), rhs.firstEnd(), 
                               CCuddGetNode(manager()) );

    // Mark navi as unused
    Cudd_Deref(navi);

    // Generate interfaced type, which cares for reference count from now on
    return interfaced_type(&manager(), navi);
  }

  /// Navigate through ZDD by incrementThen(), incrementElse(), and terminated()
  navigator navigation() const {
    return navigator(m_interfaced.getNode());
  }

  /// Checks whether the decision diagram is empty
  bool_type emptiness() const {
    return ( m_interfaced == manager().zddZero() );
  }

  /// Checks whether the decision diagram has every variable negated
  bool_type blankness() const {
    return ( m_interfaced == 
             manager().zddOne( nVariables() ) );
  }

  /// Returns number of terms
  size_type length() const {
    return m_interfaced.Count();
  }
  /// Returns number of variables in manager
  size_type nVariables() const {
   return Cudd_ReadZddSize(manager().getManager() );
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
