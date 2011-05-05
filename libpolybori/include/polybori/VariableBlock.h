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

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/BooleVariable.h>

BEGIN_NAMESPACE_PBORI

class VariableIndexException{
    
};

/** @class VariableBlock
 * @brief This class defines a VariableBlock.
 * 
 * We break down the two-argumented class @c BooleVariables(idx, ring) to a one argumented
 * call over a functional object @c var(idx) which knows about the ring ("currying").
 **/
template <bool reverse>
class VariableBlock {
  /// Type of *this
  typedef VariableBlock self;

public:
  typedef BooleVariable var_type;
  typedef typename var_type::ring_type ring_type;
  typedef typename var_type::idx_type idx_type;

  /// Construct first Variable of given ring
  VariableBlock(const ring_type& ring):
    m_size(ring.nVariables()), m_start_index(0), m_offset(0), m_ring(ring) { }

  /// Construct block
  VariableBlock(idx_type size, idx_type start_index, idx_type offset,
                const ring_type& ring): 
    m_size(size), m_start_index(start_index), m_offset(offset),
    m_ring(ring) { }

  /// Copy constructor
  VariableBlock(const self& rhs):
    m_size(rhs.m_size),
    m_start_index(rhs.m_start_index), m_offset(rhs.m_offset),
    m_ring(rhs.m_ring) { }

  /// Destructor
  ~VariableBlock() {}

  /// We may easily
  var_type operator()(idx_type i){
    if UNLIKELY((i>=m_start_index+m_size)||(i<m_start_index)){
      throw VariableIndexException();
    }
    return
      var_type((reverse?m_offset+m_start_index+m_size-1-i:i-m_start_index+m_offset),
               m_ring);
  }

protected:
  idx_type m_size;
  idx_type m_start_index;
  idx_type m_offset;

  const ring_type& m_ring;
};

END_NAMESPACE_PBORI
