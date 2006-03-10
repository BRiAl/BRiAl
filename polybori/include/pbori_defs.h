// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file defines some types, macros and other settings to be used with
 * the polybori library.
 *
 * @note Uses c++ namespace if @c PBORI_NO_NAMESPACES is undefined.
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
 * Revision 1.2  2006/03/10 08:25:54  dreyer
 * + refined header style
 *
 * Revision 1.1.1.1  2006/03/09 14:34:31  dreyer
 * + Project started
 *
 * @endverbatim
**/
//*****************************************************************************


// load cudd's c++ interface
# include <cuddObj.hh>


#ifndef pbori_defs_h_
#define pbori_defs_h_

/// name the project
#define PBORINAME polybori



#ifndef PBORI_NO_NAMESPACES

/// start project's namespace
#  define BEGIN_NAMESPACE_PBORI namespace PBORINAME {

/// finish project's namespace
#  define END_NAMESPACE_PBORI } // end of namespace 

/// use project's namespace
#  define USING_NAMESPACE_PBORI using namespace PBORINAME;

/// access something in project's namespace using PBORI::
#  define PBORI PBORINAME

/// using something from project's namespace
#  define USING_PBORI using PBORI

/// start some namespace
#  define PBORI_BEGIN_NAMESPACE( sub_space ) namespace sub_space {

/// finish some namespace
#  define PBORI_END_NAMESPACE }

#else 

#  define BEGIN_NAMESPACE_PBORI 
#  define END_NAMESPACE_PBORI 
#  define USING_NAMESPACE_PBORI 
#  define PBORI 
#  define USING_PBORI 
#  define PBORI_BEGIN_NAMESPACE( sub_space ) 
#  define PBORI_END_NAMESPACE 

#endif // PBORI_NO_NAMESPACES

/// @def Generate trace function if debugging
#ifdef PBORI_DEBUG_TRACE
# include <iostream>
# define PBORI_TRACE_FUNC(text) std::cerr << test << std::endl;
#else
# define PBORI_TRACE_FUNC(text) 
#endif

BEGIN_NAMESPACE_PBORI

/** @class Types
 * @brief This struct contains type definitions to be used in library classes
 *
 * It mainly consists of definitions for
 * - the type of decicion diagrams (chosen from the @c cudd library),
 * - built-in types to be used consistently all over the library
 * 
 **/
struct CTypes {
  //-------------------------------------------------------------------------
  // types for treatment of decision diagrams
  //-------------------------------------------------------------------------

  /// type of underlying binary decicion diagrams 
  typedef ZDD dd_type;

  /// vector of dd_type
  typedef ZDDvector ddvector_type;

  /// manage variables to be used by polynomials over Boolean ring
  typedef Cudd manager_type;

  //-------------------------------------------------------------------------
  // types for several purposes
  //-------------------------------------------------------------------------

  /// type for lengths, dimensions, etc.
  typedef unsigned int size_type;

  /// type for integer numbers
  typedef int integer_type;

  /// type for indices
  typedef int idx_type;

  /// error codes enumeration
  enum errorcodes {
    alright = 0,
    failed,
    no_ring,
    last_error
  };

  /// type used to store error codes
  typedef unsigned int errornum_type;

  /// type used to verbose error information
  typedef const char* errortext_type;

};

END_NAMESPACE_PBORI

#endif // of #ifndef pbori_defs_h_
