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
 * Revision 1.5  2006/08/18 19:47:15  dreyer
 * change finished low-level variant of minimal_elements also for lnegth()==2
 *
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
#include "CCuddNavigator.h"
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

  typedef typename NaviType::value_type idx_type;
  typedef typename std::vector<idx_type> vector_type;
  typedef typename vector_type::iterator iterator;
  typedef typename vector_type::const_reverse_iterator const_reverse_iterator;

  vector_type indices(apply.nSupport(navi));
  iterator iter(indices.begin());

  NaviType multiples = navi;

      while(!multiples.isConstant()) {
        *iter = *multiples;
        multiples.incrementThen();
        ++iter;
      }

      const_reverse_iterator start(indices.rbegin()),
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

template<class NaviType, class ReverseIterator, class DDOperations>
NaviType
minimal_of_two_terms(NaviType navi, NaviType& multiples,
                       ReverseIterator idxStart, ReverseIterator idxFinish,
                       const DDOperations& apply){

  typedef typename NaviType::value_type idx_type;
  typedef typename std::vector<idx_type> vector_type;
  typedef typename vector_type::iterator iterator;
  typedef typename vector_type::size_type size_type;
  typedef typename vector_type::const_reverse_iterator const_reverse_iterator;

  //  std::cout <<"2s ";


  size_type nmax = apply.nSupport(navi);
  vector_type thenIdx(nmax), elseIdx(nmax);

  thenIdx.resize(0);
  elseIdx.resize(0);

  NaviType thenNavi = navi;
  NaviType elseNavi = navi;

  // See CCuddLastIterator
  NaviType tmp(elseNavi);
  elseNavi.incrementElse();

  while(!elseNavi.isConstant()){
    tmp = elseNavi;
    elseNavi.incrementElse();
  }
  if(elseNavi.isEmpty())
    elseNavi = tmp;

  //    std::cout <<"TH "<<*thenNavi <<" "<<*elseNavi<< ":";

  bool isReducible = true;
  while (isReducible && !thenNavi.isConstant() && !elseNavi.isConstant()){

    while( !thenNavi.isConstant() && (*thenNavi < *elseNavi)) {
      //     std::cout <<"th "<<*thenNavi <<" "<<*elseNavi<< ":";

//   apply.print(thenNavi);
//   apply.print(elseNavi);
      thenIdx.push_back(*thenNavi);
      thenNavi.incrementThen();
    }

    if(!thenNavi.isConstant() && (*thenNavi == *elseNavi) ){
      thenIdx.push_back(*thenNavi);
      thenNavi.incrementThen();
    }
    else 
      isReducible = false;
    //   std::cout <<""<<isReducible << thenNavi.isConstant()<<std::endl;

    elseIdx.push_back(*elseNavi);

    // next on last path
    elseNavi.incrementThen();
    if( !elseNavi.isConstant() ) {       // if still in interior of a path
      
      tmp = elseNavi;         // store copy of *this
      elseNavi.incrementElse();   // go in direction of last term, if possible

      // restore previous value, of else branch was invalid
      if( elseNavi.isEmpty() )
        elseNavi = tmp;

    }
  }


  NaviType elseTail, elseMult;
  apply.assign(elseTail, elseNavi);/// !!!!!!!!!!!!
   ///  elseTail = elseNavi;

    // int initref = ((DdNode*)elseNavi)->ref;
    //    std::cout << ((DdNode*)elseNavi)->ref <<std::endl;
  if (!elseNavi.isConstant()) {
        isReducible = false;
    elseMult = 
      indexed_term_multiples(elseTail, idxStart, idxFinish, apply);
//       if(elseMult==elseTail)
//         Cudd_Deref(elseMult);
  }
  else {
    ///   Cudd_Ref(elseTail);
    ///!1  elseMult =  elseTail;
       apply.assign(elseMult, elseTail);///
  }

NaviType tmp2 = prepend_multiples_wrt_indices(elseMult, *navi, 
                                           idxStart, idxFinish, apply);

Cudd_Ref(tmp2);
Cudd_Deref(elseMult);
 elseMult = tmp2;
    // std::cerr<< "h1"<<std::endl;

  NaviType thenTail, thenMult;

  if(!isReducible){

//     if(!thenNavi.isConstant())
//       std::cout << "   "<<(*thenNavi)<< " "<< *idxStart<<std::endl;
    apply.assign(thenTail, thenNavi);
    ///!2    thenTail = thenNavi;

    if (!thenNavi.isConstant()){

     thenMult = 
        indexed_term_multiples(thenTail, idxStart, idxFinish, apply);
//       if(thenMult==thenTail)
//         Cudd_Deref(thenMult);
//Cudd_Deref(thenTail);///
      ////////   Cudd_Ref(thenMult);///
    }
    else{
      ///!1 thenMult= thenTail;
          apply.assign(thenMult, thenTail);////
    }
  }
    // std::cerr<< "h2"<<std::endl;
  // generating elsePath and multiples
  ReverseIterator idxIter = idxStart;
  const_reverse_iterator start(elseIdx.rbegin()), finish(elseIdx.rend());
  
  //  Cudd_Ref(elseMult);
  // std::cout << "isRed"<< isReducible <<std::endl;

  if(!elseMult.isConstant())
    while((idxIter != idxFinish) && (*idxIter >= *elseMult) ) 
      ++idxIter;

  while(start != finish){
    
    while((idxIter != idxFinish) && (*idxIter > *start) ) {
      
      apply.multiplesAssign(elseMult, *idxIter);
      ++idxIter;
    }
    apply.productAssign(elseMult, *start);

    if (isReducible)
      apply.productAssign(elseTail, *start); 
      
    if(idxIter != idxFinish)
      ++idxIter;
    ++start;
  }
    // std::cerr<< "h3"<<std::endl;
  if (isReducible){
    multiples = elseMult;


    ///////   Cudd_Ref(multiples); ///////
    //  Cudd_Ref(elseTail); 
    //Cudd_Deref(thenTail); 
    //Cudd_Deref(thenMult); 
 
    // std::cerr<< "h4"<<std::endl;
    return elseTail;
  }
  else {

    // std::cerr<< "h5"<<std::endl;
    const_reverse_iterator start2(thenIdx.rbegin()), finish2(thenIdx.rend());
    ReverseIterator idxIter = idxStart;

    //  Cudd_Ref(thenMult);
//     NaviType printer = thenMult;    std::cout<< "thenMult"<<std::endl;
//     while(!printer.isConstant()){
//       std::cout<< *printer <<" & ";
//       printer.incrementThen();
//     }
    if(!thenMult.isConstant())
      while((idxIter != idxFinish) && (*idxIter >= *thenMult) ) 
        ++idxIter;


    // std::cerr<< "h6"<<std::endl;


    while(start2 != finish2){
         
      while((idxIter != idxFinish) && (*idxIter > *start2) ) {
        //   std::cout<< *idxIter <<" ? ";
        apply.multiplesAssign(thenMult, *idxIter);
        ++idxIter;
      }
      apply.productAssign(thenMult, *start2);
      //     apply.productAssign(thenTail, *start);   
      if(idxIter != idxFinish)
        ++idxIter;
      ++start2;
    }


    apply.replacingUnite(multiples, elseMult, thenMult);



    // std::cerr<< "h7"<<std::endl;
//     printer = multiples;    std::cout<< "mu"<<std::endl;
//     while(!printer.isConstant()){
//       //   std::cout<< *printer <<" & ";
//       printer.incrementThen();
//     }
    //  std::cout<< std::endl;
    //////////  Cudd_Ref(multiples);
    // return apply.newNode(navi);
    //  std::cout << " "<<((DdNode*)thenTail)->ref<<std::endl;
    // std::cerr<< "h8"<<std::endl;

   apply.kill(elseTail);
   apply.kill(thenTail);


    return apply.newNode(navi);
  }



//   // remainder of first term
//   while (!thenNavi.isConstant() ){
//     thenIdx.push_back(*thenNavi);
//     thenIdx.incrementThen();
//   }

//   // remainder of last term
//   while (!elseNavi.isConstant()){
//     elseIdx.push_back(*elseNavi);

//     elseIdx.incrementThen();
//     if( !elseIdx.isConstant() ) {       // if still in interior of a path

//       tmp = elseNavi;         // store copy of *this
//       elseNavi.incrementElse();   // go in direction of last term, if possible

//       // restore previous value, of else branch was invalid
//       if( elseNavi.isEmpty() )
//         elseNavi = tmp;
//     }
//     isReducible = false;
//   }



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
void apply_replacing_cudd_function(FunctionType func, ManagerType& mgr,
                                   NodeType& newNode,
                                   NodeType& first, NodeType& second) {

    newNode = func(mgr, first, second);
    Cudd_Ref(newNode);
    Cudd_RecursiveDerefZdd(mgr, first);
    Cudd_RecursiveDerefZdd(mgr, second);
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
  void replacingUnite(node_type& newNode,
                      node_type& first, node_type& second) const {

    apply_replacing_cudd_function(Cudd_zddUnion, mgr,
                                  newNode, first, second);
  }

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
                     node_type& thenNode, const node_type& elseNode) const {
    node_type newNode = cuddZddGetNode(mgr, idx, thenNode, elseNode);
    Cudd_Ref(newNode);
    //Cudd_Deref(thenNode);   
    Cudd_RecursiveDerefZdd(mgr, thenNode);
    thenNode = newNode;
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

  void kill(node_type& node) const {
    Cudd_RecursiveDerefZdd(mgr, node);
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

    if(false&&(nlen == 2)) {

      //      std::cerr << "hier"<<std::endl;
      navi = minimal_of_two_terms(navi, multiples,idxStart, idxEnd, apply);
  
      //     std::cerr << "danach"<<std::endl;
      return navi;

#if 0
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
#endif
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
