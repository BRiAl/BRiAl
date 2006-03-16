// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file implements the class BoolePolymials, where carries the definition
 * of variables over Booleans.
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
 * Revision 1.1  2006/03/16 17:09:13  dreyer
 * ADD BoolePolynial functionality started
 *
 * @endverbatim
**/
//*****************************************************************************


// load header file
# include "BooleVariable.h"

// get ring definitions
# include "BoolePolyRing.h"

BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------

// Default constructor
BooleVariable::BooleVariable(idx_type idx):

  base( BoolePolyRing::ringVariable(idx) )  {

  PBORI_TRACE_FUNC( "BooleVariable(idx_type)" );

  size_type nlen = BoolePolyRing::nRingVars();

  dd_type& dd(diagram());
  for(size_type i = 0; i< nlen; ++i)
    if (i != idx)
      dd = dd.Subset0(i);

}

// Copy constructor
BooleVariable::BooleVariable(const self& rhs) :
  base(rhs) {
  
  PBORI_TRACE_FUNC( "BooleVariable(const self&)" );
}

END_NAMESPACE_PBORI
