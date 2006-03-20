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
 * Revision 1.2  2006/03/20 09:52:57  dreyer
 * CHANGE: BooleVariable uses composition; variable generated in BoolePolyRing
 *
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
  m_poly( BoolePolyRing::ringVariable(idx) )  {

  PBORI_TRACE_FUNC( "BooleVariable(idx_type)" );

}

// Copy constructor
BooleVariable::BooleVariable(const self& rhs) :
  m_poly(rhs.m_poly) {
  
  PBORI_TRACE_FUNC( "BooleVariable(const self&)" );
}

// Casting operator
BooleVariable::operator const BoolePolynomial&() const {

  PBORI_TRACE_FUNC( "BooleVariable::operator const BoolePolynomial&() const" );

  return m_poly;
}

END_NAMESPACE_PBORI
