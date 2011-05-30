// -*- c++ -*-
//*****************************************************************************
/** @file BooleEnv.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file implements the class BooleEnv, which hold global structures.
 * a polynomial ring over Booleans. 
 *
 * @par Copyright:
 *   (c) 2007 by The PolyBoRi Team
**/
//*****************************************************************************


// load header file
#include <polybori/BooleEnv.h>
#include <polybori/BoolePolyRing.h>
#include <polybori/orderings/COrderingBase.h>

BEGIN_NAMESPACE_PBORI

//BooleEnv::ring_type active_ring(1000, CTypes::lp, false);

// /// @todo needs inlining!!!
// #ifdef PBORI_ENV_RING_NOTINLINED
// BooleEnv::ring_type& BooleEnv::ring() {
//   static BooleEnv::ring_type active_ring(1000, CTypes::lp, false);
//   return active_ring; 
// }
// #endif 

// BooleEnv::block_iterator 
// BooleEnv::blockBegin() {

//   return ordering().blockBegin();
// }

// BooleEnv::block_iterator
// BooleEnv::blockEnd() {

//   return ordering().blockEnd();
// }

// void BooleEnv::appendBlock(idx_type idx) {

//   ordering().appendBlock(idx);
// }

// void BooleEnv::clearBlocks() {

//   ordering().clearBlocks();
// }



// BooleEnv::idx_type
// BooleEnv::lastBlockStart() {
//   return ring().ordering().lastBlockStart();
// }




// // BooleEnv::manager_type& BooleEnv::manager() {
// //   return ring().manager(); }
// BooleEnv::order_type& BooleEnv::ordering() { 
//   return  ring().ordering(); }




//   /// Get empty decision diagram
// BooleEnv::dd_type BooleEnv::zero() { return ring().zero(); }

//   /// Get decision diagram with all variables negated
// BooleEnv::dd_type BooleEnv::one() { return ring().one(); }

//   /// Get number of ring variables the of active ring
// BooleEnv::size_type BooleEnv::nVariables() { 
//   return ring().nVariables(); 
// }





//   /// Set name of variable with index idx
// void 
// BooleEnv::setVariableName(idx_type idx, vartext_type varname) {
//   ring().setVariableName(idx, varname);
// }

//   /// Get name of variable with index idx
// BooleEnv::vartext_type 
// BooleEnv::getVariableName(idx_type idx){
//   return ring().getVariableName(idx);
// }


//   /// Change order of current ring
// void 
// BooleEnv::changeOrdering(ordercode_type code) {
//     ring().changeOrdering(code);
// }



//   /// Get numerical code for current ordering
// BooleEnv::ordercode_type BooleEnv::getOrderCode() { 
//   return ordering().getOrderCode();
// }

//   /// Get numerical code for current base ordering 
//   /// (the same for non-block orderings)
// BooleEnv::ordercode_type BooleEnv::getBaseOrderCode() { 
//   return ordering().getBaseOrderCode(); 
// }

// /// Access idx-th variable of the active ring
// BooleEnv::var_type BooleEnv::variable(idx_type idx) {
//   return ring().variable(idx);
// }


// BooleEnv::ostream_type& 
// BooleEnv::print(ostream_type& os) {

//   return ring().print(os); 
// } 


  //void BooleEnv::set(ring_type& theRing) { ring() = theRing; }

/// Extract block data without exposing ordering stuff
BooleEnv::block_iterator 
block_begin(const BooleEnv::ring_type& ring) {
  return ring.ordering().blockBegin();
}


/// Extract block data without exposing ordering stuff
BooleEnv::block_iterator 
block_end(const BooleEnv::ring_type& ring) {
  return ring.ordering().blockEnd();
}

END_NAMESPACE_PBORI
