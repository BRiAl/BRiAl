// -*- c++ -*-
//*****************************************************************************
/** @file CIdxVariable.h
 * 
 * @author Alexander Dreyer
 * @date 2006-07-04
 *
 * This file contains the definition of a template for the storage type for
 * one index.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.2  2007/11/06 15:03:34  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.1  2006/07/04 14:11:03  dreyer
 * ADD: Generic and handy treatment of string literals
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// get functionals and algorithms
#include "pbori_func.h"
#include "pbori_algo.h"

#include "CStringLiteral.h"
#include "CPrintOperation.h"



#ifndef CIdxVariable_h_
#define CIdxVariable_h_

/** @class CIdxVariable
 * @brief This template class defines a storage type for one monomial index 
 * and customizable "pretty" printing.
 *
 **/
BEGIN_NAMESPACE_PBORI

template <class IdxType = CTypes::idx_type, 
          class VarNameLit = 
          CStringLiteral<CLiteralCodes::default_variable_name>, 
          class VarHeadLit = 
          CStringLiteral<CLiteralCodes::variable_head>, 
          class VarTailLit = 
          CStringLiteral<CLiteralCodes::variable_tail> >
class CIdxVariable {

public:
  /// @name Adopt global type definitions
  //@{
  typedef IdxType idx_type;
  typedef CTypes::ostream_type ostream_type;
  //@}

  /// Type of *this
  typedef CIdxVariable<idx_type, VarNameLit, VarHeadLit, VarTailLit> self;

  /// Construct storage for nlen indices
  CIdxVariable(idx_type idx_ = 0): idx(idx_) {};

  /// Copy constructor
  CIdxVariable(const self& rhs): idx(rhs.idx) {};

  /// Destructor
  ~CIdxVariable() {};

  /// Print to out-stream
  ostream_type& print(ostream_type& os) const {

    os << VarNameLit()() << VarHeadLit()() << idx << VarTailLit()();

    return os;
  }

protected:
  idx_type idx;
};

/// Stream output operator
template <class IdxType, class VarNameLit, class VarHeadLit, class VarTailLit>
inline typename 
CIdxVariable<IdxType, VarNameLit, VarHeadLit, VarTailLit>::ostream_type& 
operator<<(typename CIdxVariable<IdxType,
           VarNameLit, VarHeadLit, VarTailLit>::ostream_type& os, 
           const CIdxVariable<IdxType,
           VarNameLit, VarHeadLit, VarTailLit>& storage){

  return storage.print(os);
}

END_NAMESPACE_PBORI

#endif
