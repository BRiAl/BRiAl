// -*- c++ -*-
//*****************************************************************************
/** @file PBoRiOutIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-21
 *
 * THis file contains the definition of the class PBoRiOutIter
 *
 * @par Copyright:
 *   (c) by
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
 * Revision 1.2  2006/07/04 14:11:03  dreyer
 * ADD: Generic and handy treatment of string literals
 *
 * Revision 1.1  2006/04/21 13:13:30  dreyer
 * ADD PBoRiOutITer for more generic manipulations
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// get BooleMonomial's definition
#include "BooleMonomial.h"

#ifndef PBoRiOutIter_h_
#define PBoRiOutIter_h_

BEGIN_NAMESPACE_PBORI

/** @class PBoRiOutIter 
 * @brief This template class defines an output iterator
 * which interprets assignments of indices as a change of given data wrt. a
 * given binary operation.
 *
 **/

template <class DataType, class RhsType, class BinOp>
class PBoRiOutIter {
public:

  /// Data type
  typedef DataType data_type;

  /// Type of right-hand side
  typedef RhsType rhs_type;

  /// Type of binary operation used to transform data and rhs
  typedef BinOp op_type;

    /// Type of *this
  typedef PBoRiOutIter<data_type, rhs_type, op_type> self;

  /// @name Interface types for standard iterator access
  //@{
  typedef std::output_iterator_tag iterator_category;
  typedef void difference_type;
  typedef void pointer;
  typedef void reference;
  typedef void value_type;
  //@}

  /// Constructor
  PBoRiOutIter(data_type& data_, op_type op_ = op_type()):
    data(data_), op(op_) {}

  /// Copy constructor
  PBoRiOutIter(const self& rhs): 
    data(rhs.data), op(rhs.op) {}

  /// Destructor
  ~PBoRiOutIter() {}

  /// Dereference operator
  /// @note *this is used as proxy reference
  self& operator*() { return *this; }

  /// Assignment
  self& operator=(const self& rhs) {
    data = rhs.data; 
    op = rhs.op;
    return *this;
  }

  /// Assignment of index calls for change of that index in the monomial
  self& operator=(rhs_type rhs){ 
    op(data, rhs); 
    return *this;
  }

  /// Prefix increment operator
  self& operator++() { return *this; }

  /// Postfix increment operator
  self operator++(int) { return *this; }

protected:
  data_type& data;
  op_type op;
};


END_NAMESPACE_PBORI

#endif
