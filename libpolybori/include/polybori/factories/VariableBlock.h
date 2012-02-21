// -*- c++ -*-
//*****************************************************************************
/** @file VariableBlock.h 
 *
 * @author Michael Brickenstein (original PyPolyBoRi class), Alexander Dreyer
 * @date 2011-05-05
 *
 * This file defines the class VariableBlock.
 *
 * @par Copyright:
 *   (c) 2005-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_factories_VariableBlock_h_
#define polybori_factories_VariableBlock_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/factories/VariableFactory.h>

BEGIN_NAMESPACE_PBORI

class VariableIndexException{
    
};

/** @class VariableBlock
 * @brief This class defines a VariableBlock.
 * 
 * We break down the two-argumented class @c BooleVariables(idx, ring) to a one argumented
 * call over a functional object @c var(idx) which knows about the ring ("currying").
 **/
class VariableBlock:
  protected VariableFactory{

  /// Type of *this
  typedef VariableBlock self;

public:
  typedef VariableFactory base;
  typedef base::value_type var_type;

  typedef var_type::ring_type ring_type;
  typedef var_type::idx_type idx_type;

  /// Construct block
  VariableBlock(idx_type size, idx_type start_index, idx_type offset,
                bool reverse, const ring_type& ring):
    base(ring), 
    m_start_index(start_index), 
    m_last (start_index + size - 1), m_offset(offset), m_reverse(reverse) { }

  /// Copy constructor
  VariableBlock(const self& rhs):
    base(rhs),
    m_start_index(rhs.m_start_index),
    m_last(rhs.m_last), m_offset(rhs.m_offset), m_reverse(rhs.m_reverse) { } 

  /// Destructor
  ~VariableBlock() {}

  /// We may easily
  var_type operator()(idx_type i){
    if PBORI_UNLIKELY( (i > m_last) || (i < m_start_index) ){
      throw VariableIndexException();
    }
    return
      base::operator()(m_offset + (m_reverse? m_last - i: i - m_start_index));
  }

protected:
  const idx_type m_start_index;
  const idx_type m_last;
  const idx_type m_offset;
  const bool m_reverse;
};

END_NAMESPACE_PBORI

#endif  /* polybori_factories_VariableBlock_h_*/
