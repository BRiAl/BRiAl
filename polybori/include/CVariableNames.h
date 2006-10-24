// -*- c++ -*-
//*****************************************************************************
/** @file CVariableNames.h
 *
 * @author Alexander Dreyer
 * @date 2006-24-10
 *
 * This file defines an array-like type for storing variables names by index.
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
 * Revision 1.1  2006/10/24 08:44:04  dreyer
 * CHANGE: Added CVariableNames; variable names handled by OrderedManager
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// get standard vector functionality
#include <vector>

// get standard string functionalities
#include <string>
#include <sstream>


#ifndef CVariableNames_h_
#define CVariableNames_h_

BEGIN_NAMESPACE_PBORI

class CVariableNames {
public:

  /// @name adopt global type definitions
  //@{
  typedef CTypes::size_type size_type;
  typedef CTypes::idx_type idx_type;
  //@}

  /// Define type for setting/getting names of variables
  typedef CTypes::vartext_type vartext_type;

  /// Define type for storing names of variables
  typedef std::string varname_type;

  /// Define type for storing names of variables
  typedef std::vector<varname_type> storage_type;

  /// Define type for write accessing elements
  typedef storage_type::reference reference;

  /// Define type for outputing variable names
  typedef vartext_type const_reference;

  /// Define type of *this
  typedef CVariableNames self;

  /// Constructor
  CVariableNames(size_type nvars): m_data(nvars) {  reset(); }

  /// Copy Constructor
  CVariableNames(const self& rhs): m_data(rhs.m_data) { }

  /// Set default variable names
  void reset();

  /// Get name of variable with index idx
  const_reference operator[](idx_type idx) const { 
    if (idx >= m_data.size())
      return "UNDEF";
    return m_data[idx].c_str(); 
  }

  /// Get writable reference to name of variable with index idx
  reference operator[](idx_type idx) { 
    return m_data[idx];
  }

private:
  storage_type m_data;
};

inline void CVariableNames::reset() {

  idx_type nlen = m_data.size();
  for (idx_type idx = 0; idx < nlen; ++idx){
    std::ostringstream sstrg; 
    sstrg << "x(" << idx << ')';
    m_data[idx] = sstrg.str();
  }
}

END_NAMESPACE_PBORI

#endif
