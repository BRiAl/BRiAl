// -*- c++ -*-
//*****************************************************************************
/** @file BooleEnv.h 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file the class BooleEnv, where handles global (static) strucutres of
 * PolyBoRi. 
 *
 * @par Copyright:
 *   (c) 2007 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_BooleEnv_h_
#define polybori_BooleEnv_h_

// load PolyBoRi settings
# include <polybori/pbori_defs.h>

#include <polybori/BoolePolyRing.h>
#include <vector>

// Obey stricter dependence of Sun Studio compiler
// todo: resolve dependency 
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
# define PBORI_ENV_RING_NOTINLINED
#endif


BEGIN_NAMESPACE_PBORI


class BoolePolyRing;
//extern BoolePolyRing active_ring;
class COrderingBase;

/** @class BooleEnv
 * @brief This class is just a wrapper accessing global structures.
 *
 *
 **/


class BooleEnv: 
  public CTypes::orderenums_type, public CTypes::compenums_type, 
  public CTypes::auxtypes_type {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// generic access to current type
  typedef BooleEnv self;

  /// generic access to base type
  typedef CTypes::orderenums_type base;

  /// @name adopt global type definitions
  //@{
  typedef CTypes::ordercode_type ordercode_type;
  typedef BoolePolyRing::dd_type dd_type;
  typedef BoolePolyRing::var_type var_type;
  typedef CTypes::vartext_type vartext_type;
  //@}

  /// Type for block indices
  typedef std::vector<idx_type> block_idx_type;

  /// Type for block iterators
  typedef block_idx_type::const_iterator block_iterator;

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------

  /// Explicitely mention ordercodes' enumeration
  using base::ordercodes;

//   /// Access idx-th variable of the active ring
//   static var_type variable(idx_type idx);

//   /// Get numerical code for current ordering
//   static ordercode_type getOrderCode();

//   /// Get numerical code for current base ordering 
//   /// (the same for non-block orderings)
//   static ordercode_type getBaseOrderCode();

//   /// Get empty decision diagram
//   static dd_type zero();

//   /// Get decision diagram with all variables negated
//   static dd_type one();

//   /// Get number of ring variables the of active ring
//   static size_type nVariables();

  typedef BoolePolyRing ring_type;

  typedef COrderingBase order_type;

// private:
// #ifdef PBORI_ENV_RING_NOTINLINED
//   static ring_type& ring();
// #else
//   static ring_type& ring() {
//     static BooleEnv::ring_type active_ring(1000, CTypes::lp, false);

//     return active_ring;
//   }
// #endif
//   //  static manager_type& manager();
//   static order_type& ordering();
// public:

//   /// Set name of variable with index idx
//   static void setVariableName(idx_type idx, vartext_type varname);

//   /// Get name of variable with index idx
//   static vartext_type getVariableName(idx_type idx);

//   /// @name interface for block orderings, if the vcurrent orderins is a block ordering
//   //@{
//   /// Returns iterator over the indices, where the (next) block starts. 
//   /// (The first one implicitely starts at zero.)
//   static block_iterator blockBegin();

//   /// Returns end marker of iteration
//   static block_iterator blockEnd();

//   /// Add index where the next block starts
//   static void appendBlock(idx_type idx);

//   /// Reset block-related data
//   static void clearBlocks();

//   /// Index where the last blocks starts
//   static idx_type lastBlockStart();
//   //@}

//   /// Change order of current ring
//   static void changeOrdering(ordercode_type code);

//   /// Print low-level information
//   static ostream_type& print(ostream_type&);

//   /// Activate given ring
//   static void set(ring_type& theRing);
};

///please use BooleEnv::ring()


/// Extract block data without exposing ordering stuff
BooleEnv::block_iterator 
block_begin(const BooleEnv::ring_type& ring);


/// Extract block data without exposing ordering stuff
BooleEnv::block_iterator 
block_end(const BooleEnv::ring_type& ring);

END_NAMESPACE_PBORI

#endif // of #ifndef polybori_BooleEnv_h_
