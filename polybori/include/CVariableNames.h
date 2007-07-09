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
 * Revision 1.5  2007/07/09 14:15:32  dreyer
 * Fix: removed performance issue
 *
 * Revision 1.4  2007/07/09 11:30:50  dreyer
 * Fix: dynamic extension of variable names
 *
 * Revision 1.3  2007/04/24 15:23:04  dreyer
 * FIX: minor changes fixing -Wall warnings
 *
 * Revision 1.2  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
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
  void reset(idx_type idx = 0);

  /// Get name of variable with index idx
  const_reference operator[](idx_type idx) const { 

    if UNLIKELY(size_type(idx) >= m_data.size())
      return undefName();
    return m_data[idx].c_str(); 
  }

  /// Get writable reference to name of variable with index idx
  void set(idx_type idx, const varname_type& varname) { 

    size_type nlen = m_data.size();

    if UNLIKELY((size_type)idx >= nlen) {
      m_data.resize((size_type)idx + 1);
      reset((idx_type)nlen);
    }

    m_data[idx] = varname;
  }

protected:
  static const_reference undefName() {  return "UNDEF"; }

private:
  storage_type m_data;
};

inline 
void CVariableNames::reset(idx_type idx) {

  idx_type nlen = (idx_type)m_data.size();

  for (; idx < nlen; ++idx){
    std::ostringstream sstrg; 
    sstrg << "x(" << idx << ')';
    m_data[idx] = sstrg.str();
  }
}


END_NAMESPACE_PBORI

#endif
