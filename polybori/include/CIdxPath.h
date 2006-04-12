// -*- c++ -*-
//*****************************************************************************
/** @file CIdxPath.h
 * 
 * @author Alexander Dreyer
 * @date 2006-12-04
 *
 * This file contains the definition of a template for the storage type for
 * indices.
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
 * Revision 1.1  2006/04/12 16:23:54  dreyer
 * ADD template class CIDXPath<>
 *
 * @endverbatim
**/
//*****************************************************************************

// get std::vector functionality
#include <vector>

// get std::iterator functionality
#include <iterator>

// include basic definitions
#include "pbori_defs.h"

// get functionals and algorithms
#include "pbori_func.h"
#include "pbori_algo.h"

#ifndef CIdxPath_h_
#define CIdxPath_h_

/** @class CIdxPath
 * @brief This template class defines a storage type for monomial indices.
 *
 **/
BEGIN_NAMESPACE_PBORI

template <class IdxType = CTypes::idx_type, 
          class SeparatorType = list_separator>
class CIdxPath:
  public std::vector<IdxType> {

public:
  /// @name Adopt global type definitions
  //@{
  typedef IdxType idx_type;
  typedef CTypes::ostream_type ostream_type;
  //@}

  /// Type of base class
  typedef std::vector<idx_type> base;

  /// Type used to generate a string for separating elements
  typedef SeparatorType separator_type;

  /// String-like type for separator
  typedef typename separator_type::result_type sep_value_type ;

  /// Type of *this
  typedef CIdxPath<idx_type, separator_type> self;

  /// Type for sizes
  typedef typename base::size_type size_type;

  /// Default constructor
  CIdxPath(): base() {}

  /// Construct storage for nlen indices
  CIdxPath(size_type nlen): base(nlen) {};

  /// Copy constructor
  CIdxPath(const self& rhs): base(rhs) {};

  /// Destructor
  ~CIdxPath() {};

  /// Print to out-stream
  ostream_type& print(ostream_type& os) const {


    special_first_transform( begin(), end(), 
                             std::ostream_iterator<std::string>(os),
                             default_varname<idx_type, std::string>(),
                             default_varname<idx_type, 
                             std::string>(print_separator()) );

    return os;
  }

  /// Get printing separator
  sep_value_type print_separator() const { return separator_type()(); }
};

/// Stream output operator
template <class IdxType, class SeparatorType>
inline typename CIdxPath<IdxType, SeparatorType>::ostream_type& 
operator<<(typename CIdxPath<IdxType, SeparatorType>::ostream_type& os, 
           const CIdxPath<IdxType, SeparatorType>& storage){

  return storage.print(os);
}

END_NAMESPACE_PBORI

#endif
