// -*- c++ -*-
//*****************************************************************************
/** @file pbori_traits.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-24
 *
 * This file defines properties of built-in an polybori types.
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
 * Revision 1.3  2006/06/06 10:56:59  dreyer
 * CHANGE usedVariables() more efficient now.
 *
 * Revision 1.2  2006/04/24 11:41:56  dreyer
 * FIX only necessary types are traits
 *
 * Revision 1.1  2006/04/24 11:34:05  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

#include <set>
#include <vector>

// include basic definitions
#include "pbori_defs.h"

#ifndef pbori_traits_h_
#define pbori_traits_h_

BEGIN_NAMESPACE_PBORI

/** @class pbori_traits 
 * @brief This classs contains property definitions to be used in polybori 
 * classes
 *
 **/
template <class ValueType>
class pbori_traits {

public:
  //-------------------------------------------------------------------------
  // types for treatment of decision diagrams
  //-------------------------------------------------------------------------
  typedef ValueType value_type;

  /// Type of interface to binary decicion diagrams 
  typedef typename value_type::dd_type dd_type;

//   /// Manage variables to be used by polynomials over Boolean ring
//   typedef typename value_type::manager_type manager_type;

//   /// Reference to decision diagramm manager
//    typedef typename value_type::manager_reference manager_reference;

//   /// Define shared pointer to decision diagram manager
//    typedef typename value_type::manager_ptr manager_ptr;

  //-------------------------------------------------------------------------
  // types for several purposes
  //-------------------------------------------------------------------------

  /// Type for standard true/false statements
  typedef typename value_type::bool_type bool_type;

  /// Type for lengths, dimensions, etc.
  typedef typename value_type::size_type size_type;

  /// Type for integer numbers
  typedef typename value_type::integer_type integer_type;

  /// Type for indices
   typedef typename value_type::idx_type idx_type;

  /// Type for comparisons
  typedef typename value_type::comp_type comp_type;

  /// Type for hashing
  typedef typename value_type::hash_type hash_type;

  /// Type for out-stream
  typedef typename value_type::ostream_type ostream_type;


};

// Built-in types inherit global definitions
template <>
class pbori_traits<void>:
  public CTypes {
};

template <>
class pbori_traits<int>:
  public CTypes {
};

template <>
class pbori_traits<unsigned int>:
  public CTypes {
};

template <>
class pbori_traits<long int>:
  public CTypes {
};


template <class ValueType>
class pbori_traits< std::set<ValueType> >:
  public CTypes {
};

template <class ValueType>
class pbori_traits< std::vector<ValueType> >:
  public CTypes {
};


END_NAMESPACE_PBORI

#endif
