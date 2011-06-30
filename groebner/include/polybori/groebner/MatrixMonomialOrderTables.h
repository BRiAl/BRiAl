// -*- c++ -*-
//*****************************************************************************
/** @file MatrixMonomialOrderTables.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c MatrixMonomialOrderTables.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_MatrixMonomialOrderTables_h_
#define polybori_groebner_MatrixMonomialOrderTables_h_

// include basic definitions
#include "groebner_defs.h"
#include <vector>

BEGIN_NAMESPACE_PBORIGB

/** @class MatrixMonomialOrderTables
 * @brief This class defines MatrixMonomialOrderTables.
 *
 **/
class MatrixMonomialOrderTables{
        
public:
  typedef Exponent::idx_map_type from_term_map_type;

  MatrixMonomialOrderTables(MonomialSet input_terms):
    terms(input_terms) {
                  setup_order_tables();
        }
  std::vector<Exponent> terms_as_exp;
  std::vector<Exponent> terms_as_exp_lex;
  std::vector<int> ring_order2lex;
  std::vector<int> lex_order2ring;
  from_term_map_type from_term_map;
  MonomialSet terms;

private:
  void setup_order_tables();
  
};

END_NAMESPACE_PBORIGB

#endif /* polybori_MatrixMonomialOrderTables_h_ */
