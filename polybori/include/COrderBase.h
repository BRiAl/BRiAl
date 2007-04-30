// -*- c++ -*-
//*****************************************************************************
/** @file COrderBase.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-05
 *
 * This file defines a base class for orderings.
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
 * Revision 1.14  2007/04/30 15:20:31  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.13  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.12  2007/03/21 08:55:08  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.11  2007/03/19 16:49:39  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.10  2007/03/16 16:59:20  dreyer
 * CHANGE: started to rewrite CGenericIter using boost:iterator_facade
 *
 * Revision 1.9  2006/12/04 12:48:16  dreyer
 * CHANGE: cached and recursive lead() and leadexp() refined, generalized
 *
 * Revision 1.8  2006/11/20 13:08:00  dreyer
 * FIX: Destructor now virtual
 *
 * Revision 1.7  2006/10/06 12:52:00  dreyer
 * ADD easy_equility_property and used in lex_compare
 *
 * Revision 1.6  2006/10/04 11:48:28  dreyer
 * ADD: isDegreeReverseLexicograpical()
 *
 * Revision 1.5  2006/09/13 15:07:04  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.4  2006/09/12 14:56:55  dreyer
 * ADD bidirectional term iterator template
 *
 * Revision 1.3  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * Revision 1.2  2006/09/05 11:10:44  dreyer
 * ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
 *
 * Revision 1.1  2006/09/05 10:21:39  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include basic definitions for orderings
#include "pbori_tags.h"

// include polynomial definitions
#include "BoolePolynomial.h"

// include monomial definitions
#include "BooleMonomial.h"

// include exponent vector definitions
#include "BooleExponent.h"

#include "COrderedIter.h"

#ifndef COrderBase_h_
#define COrderBase_h_

BEGIN_NAMESPACE_PBORI


/** @class COrderBase
 * @brief This class defines ordering related functions.
 *
 *
 **/
class COrderBase:
  public CTypes::auxtypes_type {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// current type should used as base only
  typedef COrderBase base;

  /// Type of Boolean polynomials
  typedef BoolePolynomial poly_type;

  /// Type for sizes
  typedef poly_type::size_type size_type;

  /// Type for indices
  typedef poly_type::idx_type idx_type;

  /// Type of Boolean monomials
  typedef BooleMonomial monom_type;

  typedef BoolePolynomial::navigator navigator;

  typedef COrderedIter<navigator, monom_type> indirect_iterator;

  /// Type of Boolean sets
  typedef BooleSet set_type;

  /// Type of Boolean monomials
  typedef BooleExponent exp_type;

  typedef COrderedIter<navigator, exp_type> indirect_exp_iterator;

  /// Type for block indices
  typedef std::vector<idx_type> block_idx_type;

  /// Type for block iterators
  typedef block_idx_type::const_iterator block_iterator;

  /// @name define generic property markers (default is invalid)
  //@{
  typedef invalid_tag lex_property; 
  typedef invalid_tag ordered_property;
  typedef invalid_tag symmetry_property;
  typedef invalid_tag degorder_property;
  typedef invalid_tag blockorder_property;
  typedef invalid_tag degrevlexorder_property; 
  typedef invalid_tag totaldegorder_property;
  typedef invalid_tag ascending_property;
  typedef invalid_tag descending_property;
  //@}

  /// Default Constructor
  COrderBase() {};

  /// Copy Constructor
  COrderBase(const COrderBase&) {};

  /// Destructor
  virtual ~COrderBase() {};

  /// Comparison of indices corresponding to variables
  virtual comp_type compare(idx_type, idx_type) const = 0;

  /// Comparison of monomials
  virtual comp_type compare(const monom_type&, const monom_type&) const = 0;

  /// Comparison of exponent vectors
  virtual comp_type compare(const exp_type&, const exp_type&) const = 0;

  /// Get leading term
  virtual monom_type lead(const poly_type&) const = 0;

   /// Get leading term (using an upper bound)
  virtual monom_type lead(const poly_type&, size_type) const = 0;

  /// Get leading exponent
  virtual exp_type leadExp(const poly_type&) const = 0;

  /// Get leading exponent (using an upper bound)
  virtual exp_type leadExp(const poly_type&, size_type) const = 0;

  /// Initialize iterator corresponding to leading term
  virtual indirect_iterator leadIteratorBegin(const poly_type&) const = 0;
  virtual indirect_iterator leadIteratorEnd() const = 0;
  virtual indirect_exp_iterator leadExpIteratorBegin(const poly_type&) const = 0;
  virtual indirect_exp_iterator leadExpIteratorEnd() const = 0;

  /// @name interface for block orderings
  //@{
  virtual block_iterator blockBegin() const { return block_iterator(); }
  virtual block_iterator blockEnd() const { return block_iterator(); }
  virtual void appendBlock(idx_type) const { }
  virtual void clearBlocks() const { }
  //@}


protected:
  /// Get monomial from set of subsets of Boolean variables (internal use only)
  monom_type monom(const set_type& rhs) const { return monom_type(rhs); }
};

END_NAMESPACE_PBORI

#endif
