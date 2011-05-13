// -*- c++ -*-
//*****************************************************************************
/** @file PBoRiOutIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-21
 *
 * This file contains the definition of the class PBoRiOutIter
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_iterators_PBoRiOutIter_h_
#define polybori_iterators_PBoRiOutIter_h_

// include basic definitions
#include <polybori/pbori_defs.h>

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
