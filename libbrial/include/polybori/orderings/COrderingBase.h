// -*- c++ -*-
//*****************************************************************************
/** @file COrderingBase.h
 *
 * @author Alexander Dreyer
 * @date 2006-05-23
 *
 * Defines base class for ordering management for polynomial rings.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_orderings_COrderingBase_h_
#define polybori_orderings_COrderingBase_h_

// include basic definitions
#include <polybori/pbori_defs.h>

#include <polybori/BoolePolynomial.h>
#include <polybori/BooleMonomial.h>
#include <polybori/BooleExponent.h>
#include <polybori/iterators/COrderedIter.h>
#include <polybori/except/PBoRiError.h>
#include <polybori/common/CCheckedIdx.h>

#include <vector>

BEGIN_NAMESPACE_PBORI

class BoolePolynomial;
class BooleMonomial;
class BooleExponent;
class CCuddNavigator;

/** @class COrderingBase
 * This class is the base of all PolyBoRi orderings.
 *
 * It can be used as an abstract base for runtime-selectable orderings.
 **/
class COrderingBase:
  public CTypes::auxtypes_type { 

  /// Type of *this
  typedef COrderingBase self;

public:
  /// @name Adopt polynomial type definitions
  //@{
  typedef BoolePolynomial poly_type;
  typedef BooleMonomial monom_type;
  typedef CCuddNavigator navigator;
  typedef BooleExponent exp_type;

  typedef COrderedIter<navigator, monom_type> ordered_iterator;
  typedef COrderedIter<navigator, exp_type> ordered_exp_iterator;
  //@}

  /// Check index on input
  typedef CCheckedIdx checked_idx_type;

  /// Type for block indices
  typedef std::vector<idx_type> block_idx_type;

  /// Type for block iterators
  typedef block_idx_type::const_iterator block_iterator;

  /// Type of Boolean sets
  typedef BooleSet set_type;

  /// Default constructor
  COrderingBase() { }

  // Destructor
  virtual ~COrderingBase() = 0;

  /// Comparison of monomials
  virtual comp_type compare(idx_type, idx_type) const = 0;

  virtual comp_type compare(const monom_type&, const monom_type&) const = 0;

  virtual comp_type compare(const exp_type&, const exp_type&) const = 0;

  /// Get leading term
  virtual monom_type lead(const poly_type&) const = 0;

  /// Get leading term (using upper bound)
  virtual monom_type lead(const poly_type&, deg_type) const = 0;

  /// Get leading exponent
  virtual exp_type leadExp(const poly_type&) const = 0;

  /// Get leading exponent (using upper bound)
  virtual exp_type leadExp(const poly_type&, deg_type) const = 0;

  /// Generates polynomial with leading term first (other terms may be skipped)
  virtual poly_type leadFirst(const poly_type&) const = 0;

  /// Check whether ring is lexicographical 
  virtual bool_type isLexicographical() const = 0;

  /// Test whether iterators respect order
  virtual bool_type orderedStandardIteration() const = 0;

  /// Test whether variable pertubation do not change the order
  virtual bool_type isSymmetric() const = 0;

  /// Test whether we deal with a degree-ordering
  virtual bool_type isDegreeOrder() const = 0;

  /// Test whether we deal with a block-ordering
  virtual bool_type isBlockOrder() const = 0;

  /// Test whether we deal with a total degree-ordering
  virtual bool_type isTotalDegreeOrder() const = 0;

  /// Test whether variables are in ascending order
  virtual bool_type ascendingVariables() const = 0;

  /// Test whether variables are in descending order
  virtual bool_type descendingVariables() const = 0;

  /// Test whether ordering is deg-rev-lex ordering
  virtual bool_type isDegreeReverseLexicographical() const = 0;

  /// Initialize iterator corresponding to leading term
  virtual ordered_iterator leadIteratorBegin(const poly_type&) const = 0;

  /// End marker for iterator corresponding to leading term
  virtual ordered_iterator leadIteratorEnd(const poly_type&) const = 0;

  /// Initialize exponent iterator corresponding to leading term
  virtual ordered_exp_iterator leadExpIteratorBegin(const poly_type&) const = 0;

  /// End marker for exponent iterator corresponding to leading term
  virtual ordered_exp_iterator leadExpIteratorEnd(const poly_type&) const = 0;

  /// Get numerical code for ordering
  virtual ordercode_type getOrderCode() const = 0;

  /// Get numerical code for base ordering (the same for non-block orderings)
  virtual ordercode_type getBaseOrderCode() const = 0 ;

  /// @name interface for block orderings
  //@{
  virtual block_iterator blockBegin() const { return block_iterator(); }
  virtual block_iterator blockEnd() const { return block_iterator(); }
  virtual void appendBlock(checked_idx_type) {}
  virtual void clearBlocks() {}
  //@}

  /// Check, whether two indices are in the same block 
  /// (true for nonblock orderings)
  virtual bool_type lieInSameBlock(idx_type, idx_type) const = 0;

  /// Generic procedure to get index, where last block starts
  virtual idx_type lastBlockStart() const = 0;

protected:
  /// Get monomial from set of subsets of Boolean variables (internal use only)
  monom_type monom(const set_type& rhs) const { 
    if PBORI_UNLIKELY(rhs.isZero())
      throw PBoRiGenericError<CTypes::illegal_on_zero>();
    return monom_type(rhs); 
  }
};

inline
COrderingBase::~COrderingBase() { }

END_NAMESPACE_PBORI

#endif
