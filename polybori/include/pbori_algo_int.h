// -*- c++ -*-
//*****************************************************************************
/** @file pbori_algo.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-07
 *
 * This file includes some templates of simple transformations and similar
 * procedures. 
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
 * Revision 1.4  2006/08/17 15:35:30  dreyer
 * ADD: extended and activated low-level version of dd_minimal_elements
 *
 * Revision 1.3  2006/08/17 09:51:33  dreyer
 * routine
 *
 * Revision 1.2  2006/08/16 16:11:17  dreyer
 * + first lowlevel variant running
 *
 * Revision 1.1  2006/08/15 16:03:58  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include polybori's definitions
#include "pbori_defs.h"

// get polybori's functionals
#include "pbori_func.h"

#ifndef pbori_algo_int_h_
#define pbori_algo_int_h_

BEGIN_NAMESPACE_PBORI

// template<class NaviType, class SizeType, class ManagerType>
// NaviType
// multiples_of_one_term(NaviType navi, SizeType nmax, ManagerType& man){


//   std::vector<int> indices (Cudd_SupportSize(man,navi));
//   std::vector<int>::iterator iter(indices.begin());

//       NaviType multiples = navi;

//       while(!multiples.isConstant()) {
//         *iter = *multiples;
//         multiples.incrementThen();
//         ++iter;
//       }

//       std::vector<int>::const_reverse_iterator start(indices.rbegin()),
//         finish(indices.rend());

//       // int nmax = dd.nVariables();

//       Cudd_Ref(multiples);
//       NaviType emptyset = navi.elseBranch();

//       NaviType tmp;
//       SizeType i = nmax-1;

//       while(start != finish){

//         while ((idxStart != idxFinish) && (*idxStart > *start))
//           //  while(i > *start) {

//           tmp = cuddZddGetNode(man, i, multiples, multiples);
//           Cudd_Ref(tmp);
//           Cudd_Deref(multiples);
//           multiples = tmp;
//           --i;
//         }
//         tmp = cuddZddGetNode(man, i, multiples, emptyset);
//         Cudd_Ref(tmp);
//         Cudd_Deref(multiples);
//         multiples = tmp;
//         --i;
//         ++start;
// }

// return multiples;
// }


template<class NaviType, class ReverseIterator, class DDOperations>
NaviType
indexed_term_multiples(NaviType navi, 
                       ReverseIterator idxStart, ReverseIterator idxFinish,
                       const DDOperations& apply){

  std::vector<int> indices (apply.nSupport(navi));

  std::vector<int>::iterator iter(indices.begin());

  NaviType multiples = navi;

      while(!multiples.isConstant()) {
        *iter = *multiples;
        multiples.incrementThen();
        ++iter;
      }

      std::vector<int>::const_reverse_iterator start(indices.rbegin()),
        finish(indices.rend());


      Cudd_Ref(multiples);

      while(start != finish){
        
        while( (idxStart != idxFinish) && (*idxStart > *start) ) {
          apply.multiplesAssign(multiples, *idxStart);
          ++idxStart;
        }
        apply.productAssign(multiples, *start);
        if(idxStart != idxFinish) 
          ++idxStart;

        ++start;
      }

      return multiples;
}


template <class NaviType>
bool 
is_reducible_by(NaviType first, NaviType second){

  while(!second.isConstant()){
    while( (!first.isConstant()) && (*first < *second))
      first.incrementThen();
    if(*first != *second)
      return false;
    second.incrementThen();
  }
  return true;
}

template <class NaviType, class SizeType, class ReverseIterator, 
          class DDOperations>
NaviType
prepend_multiples_wrt_indices(NaviType navi, SizeType minIdx,
                              ReverseIterator start, ReverseIterator finish,
                              const DDOperations& apply) {

  if (navi.isConstant()){
    if (!navi.terminalValue())
      return navi;
  }
  else 
    while ( (start != finish) && (*start >= *navi) )
      ++start;

  while( (start != finish) && (*start > minIdx) ){
    apply.multiplesAssign(navi, *start);
    ++start;
  }
  return navi;
}


template<class FunctionType, class ManagerType, class NodeType>
void apply_assign_cudd_function(FunctionType func, ManagerType& mgr,
                                NodeType& first, const NodeType& second) {

    NodeType newNode;
    newNode = func(mgr, first, second);
    Cudd_Ref(newNode);
    Cudd_RecursiveDerefZdd(mgr, first);
    first = newNode;
}

template<class FunctionType, class ManagerType, class NodeType>
NodeType apply_cudd_function(FunctionType func, ManagerType& mgr,
                             const NodeType& first, const NodeType& second) {

    NodeType newNode;
    newNode = func(mgr, first, second);
    Cudd_Ref(newNode);
    return newNode;
}

template <class  DDType>
class dd_operations;

template<>
class dd_operations<CDDInterface<ZDD>::navigator> {
public:
  typedef DdManager* manager_type;
  typedef CDDInterface<ZDD>::navigator node_type;
  typedef CDDInterface<ZDD>::idx_type idx_type;
  typedef CDDInterface<ZDD>::size_type size_type;

  dd_operations(manager_type man): mgr(man) {}

  void uniteAssign(node_type& first, const node_type& second) const {
    apply_assign_cudd_function(Cudd_zddUnion, mgr, first, second);
  }
  void diffAssign(node_type& first, const node_type& second) const {
    apply_assign_cudd_function(Cudd_zddDiff, mgr, first, second);
  }
  node_type diff(const node_type& first, const node_type& second) const {
    return apply_cudd_function(Cudd_zddDiff, mgr, first, second);
  }
  void replacingNode(node_type& newNode, idx_type idx, 
                     node_type& first, node_type& second) const {

    newNode = cuddZddGetNode(mgr, idx, first, second);
    Cudd_Ref(newNode);
    Cudd_RecursiveDerefZdd(mgr, first);
    Cudd_RecursiveDerefZdd(mgr, second);
  }
 
  void newNodeAssign(idx_type idx, 
                     node_type& elseNode, const node_type& thenNode) const {
    node_type newNode = cuddZddGetNode(mgr, idx, elseNode, thenNode);
    Cudd_Ref(newNode);
    Cudd_Deref(elseNode);
    elseNode = newNode;
  }

  void multiplesAssign(node_type& node, idx_type idx) const {
    newNodeAssign(idx, node, node);
  }

  void productAssign(node_type& node, idx_type idx) const {
    node_type emptyset = Cudd_ReadZero(mgr);
    newNodeAssign(idx, node, emptyset);
  }

  void assign(node_type& first, const node_type& second) const {

    first = second;
    Cudd_Ref(first);
  }
  void replace(node_type& first, const node_type& second) const {
    Cudd_RecursiveDerefZdd(mgr, first);
    first = second;
  }

  size_type nSupport(const node_type& node) const {
    return Cudd_SupportSize(mgr, node);
  }
  size_type length(const node_type& node) const {
    return Cudd_zddCount(mgr, node);
  }

  node_type& newNode(node_type& node) const {
    Cudd_Ref(node);
    return node;
  }
protected:
  manager_type mgr;
};

/// A first version
/// Function templates extracting minimal elements of dd wrt. inclusion
/// Assumption, navi is navigator of dd
template <class NaviType,  class DDType2, class ReverseIterator,
          class DDOperations>
//DDType
NaviType
dd_minimal_elements(NaviType navi,DDType2& multiples,
                    ReverseIterator idxStart, ReverseIterator idxEnd, 
                    const DDOperations& apply) {



  if (!navi.isConstant()) {     // Not at end of path

    int nlen = apply.length(navi);

    if((nlen == 2)) {
      multiples = navi;


      std::vector<int> indices (apply.nSupport(navi));
      std::vector<int>::iterator iter(indices.begin()), iend(indices.end());
      bool is_reducible = true;
      bool reducible_tested = false;

      int used = 0;
      NaviType thenBr;
      NaviType elseBr;
      while( is_reducible&&(!multiples.isConstant())) {
        *iter = *multiples;
        used++;
          
        thenBr = multiples.thenBranch();
        elseBr = multiples.elseBranch();

        if((elseBr.isConstant() && elseBr.terminalValue())) {
          --iter;
          --used;
          multiples = elseBr;
        }
        else if (elseBr.isConstant() && !elseBr.terminalValue()) 
          multiples = thenBr;
        else {
          if (!reducible_tested){
            reducible_tested == true;
            is_reducible = is_reducible_by(thenBr, elseBr);
          }
          if(is_reducible){
            --iter;
            --used;
          }

          multiples = elseBr;
        }
        
          
          ++iter;
 
      }



      indices.resize(used);

      if (is_reducible) {

        std::vector<int>::const_reverse_iterator start(indices.rbegin()),
          finish(indices.rend());
        
        // int nmax = dd.nVariables();
        
        Cudd_Ref(multiples);
        

        NaviType tmp,tmpnavi;

        apply.assign(tmpnavi, multiples);
        
        ReverseIterator idxIter = idxStart;
        while(start != finish){
         
          while((idxIter != idxEnd) && (*idxIter > *start) ) {

            apply.multiplesAssign(multiples, *idxIter);
            ++idxIter;
          }
          apply.productAssign(multiples, *start);
          apply.productAssign(tmpnavi, *start);      
          if(idxIter != idxEnd)
            ++idxIter;
          ++start;
        }

        navi = tmpnavi;
        return navi;
      }
//       else {                    // Subcase: two proper terms

//         thenBr = indexed_term_multiples(thenBr, idxStart, idxEnd, apply);
//         elseBr = indexed_term_multiples(elseBr, idxStart, idxEnd, apply);

//       }
    }



    if(nlen == 1) {             // Special case of only one term
      //      Cudd_Ref(navi);
      multiples = indexed_term_multiples(navi, idxStart, idxEnd, apply);
      return apply.newNode(navi);
    }


    // treat else branch
    NaviType elseMult;
    NaviType elsedd = dd_minimal_elements(navi.elseBranch(),  
                                          elseMult, idxStart, idxEnd, apply);
    elseMult = prepend_multiples_wrt_indices(elseMult, *navi, 
                                             idxStart, idxEnd, apply);

    // short cut for obvious inclusion
    if( (navi.elseBranch() == navi.thenBranch()) ||
        (elsedd.isConstant() && elsedd.terminalValue()) ){
      multiples = elseMult;      
      return elsedd;
    }
 
    // remove already treated branches
    NaviType thenNavi(apply.diff(navi.thenBranch(), elseMult));

    // treat remaining parts of then branch
    NaviType thenMult;
    apply.replace(thenNavi, dd_minimal_elements(thenNavi,  thenMult, 
                                                idxStart, idxEnd, apply)); 
    thenMult = prepend_multiples_wrt_indices(thenMult, *navi, 
                                             idxStart, idxEnd, apply);

    // generate node consisting of all multiples
    apply.uniteAssign(thenMult, elseMult);
    apply.replacingNode(multiples, *navi, thenMult, elseMult);

    // generate result from minimal elements of then and else brach
    NaviType result;
    apply.replacingNode(result, *navi, thenNavi, elsedd);

    return result;

  }

  apply.assign(multiples, navi);

  return apply.newNode(navi);
}

END_NAMESPACE_PBORI

#endif
