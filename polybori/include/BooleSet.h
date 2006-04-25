// -*- c++ -*-
//*****************************************************************************
/** @file BooleSet.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-20
 *
 * This file defines the class BooleSet, which is currently just a typedef.
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
 * Revision 1.5  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.4  2006/04/21 09:31:44  dreyer
 * CHANGE reducing redundancy
 *
 * Revision 1.3  2006/04/21 07:11:49  dreyer
 * ADD operator=
 *
 * Revision 1.2  2006/04/20 16:59:47  dreyer
 * routine
 *
 * Revision 1.1  2006/04/20 08:31:21  dreyer
 * ADD BooleMonomial::mulples(...)
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include definitions of decision diagram interfaces
#include "CDDInterface.h"

// include polybori functionals
#include "pbori_func.h"
// include polybori iterator
#include "CTermIter.h"


#ifndef BooleSet_h_
#define BooleSet_h_

BEGIN_NAMESPACE_PBORI

/// Forward declaration of monomial type
class BooleMonomial;

class BooleSet:
  public CTypes::dd_type {

public:
  /// Generic access to type of *this
  typedef BooleSet self;

  /// Generic access to base type
  typedef CTypes::dd_type base;

  /// Type of terms
  typedef BooleMonomial term_type;

  /// Iterator type for iterating all monomials
  typedef CTermIter<term_type, navigator, 
                    change_assign<>,
                    change_assign<> >
  const_iterator;

  /// Default constructor
  BooleSet();

  /// Copy constructor
  BooleSet(const self&);

  /// Copy constructor
  BooleSet(const base&);

  /// Destructor
  ~BooleSet();

  /// Start of iteration over terms
  const_iterator begin() const;

  /// Finish of iteration over terms
  const_iterator end() const;

  /// Assignment operator
  self& operator=(const self&);

  /// Use assignment operator of base element
  using base::operator=;

  /// Add given monomial to sets and assign
  self& addAssign(const term_type&);

  /// Add given monomial to sets 
  self add(const term_type&) const;

  /// Check whether rhs is includes in *this
  bool_type owns(const term_type&) const;



};


END_NAMESPACE_PBORI

#endif
