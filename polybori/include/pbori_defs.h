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
 * Revision 1.7  2006/03/16 17:09:13  dreyer
 * ADD BoolePolynial functionality started
 *
 * Revision 1.6  2006/03/16 13:37:41  dreyer
 * ADD: added error codes and PBORI_DEVELOPER marker
 *
 * Revision 1.5  2006/03/13 10:06:29  bricken
 * *bricken: undo last checkin
 *
 * Revision 1.3  2006/03/10 15:10:16  dreyer
 * ADD: Global bool_type added.
 *
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

/// Get out stream type
#ifndef PBORI_NO_STDSTREAMS

# include <iostream>
# define PBORI_OSTREAM std::ostream

#else

/// @struct Dummy structure if std::ostream is not available
struct PBORI_OSTREAM {};

template <class StreamedType>
PBORI_OSTREAM& 
operator<<(PBORI_OSTREAM& dummy, const StreamedType&) {
  return dummy;
};

#endif // of #ifndef PBORI_NO_STDSTREAMS

/// Name the project
#define PBORINAME polybori

/// Define deverloper's mode
#ifndef PBORI_NO_DEVELOPER
#  define PBORI_DEVELOPER
#endif

#ifndef PBORI_NO_NAMESPACES

/// Start project's namespace
#  define BEGIN_NAMESPACE_PBORI namespace PBORINAME {

/// Finish project's namespace
#  define END_NAMESPACE_PBORI } // end of namespace 

/// Use project's namespace
#  define USING_NAMESPACE_PBORI using namespace PBORINAME;

/// Access something in project's namespace using PBORI::
#  define PBORI PBORINAME

/// Using something from project's namespace
#  define USING_PBORI using PBORI

/// Start some namespace
#  define PBORI_BEGIN_NAMESPACE( sub_space ) namespace sub_space {

/// Finish some namespace
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

/// Generate trace function if debugging
#ifdef PBORI_DEBUG_TRACE
# include <iostream>
# define PBORI_TRACE_FUNC(text) std::cerr << text << std::endl;
#else
# define PBORI_TRACE_FUNC(text) 
#endif

BEGIN_NAMESPACE_PBORI

/** @class CTypes
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

  /// Type of underlying binary decicion diagrams 
  typedef ZDD dd_type;

  ///Vector of dd_type
  typedef ZDDvector ddvector_type;

  /// Manage variables to be used by polynomials over Boolean ring
  typedef Cudd manager_type;

  //-------------------------------------------------------------------------
  // types for several purposes
  //-------------------------------------------------------------------------

  /// Type for standard true/false statements
  typedef bool bool_type;

  /// Type for lengths, dimensions, etc.
  typedef unsigned int size_type;

  /// Type for integer numbers
  typedef int integer_type;

  /// Type for indices
  typedef int idx_type;

  /// Error codes enumeration
  enum errorcodes {
    alright = 0,
    failed,
    no_ring,
    invalid,
#ifdef PBORI_DEVELOPER
   not_implemented,
#endif
    last_error
  };

  /// Type used to store error codes
  typedef unsigned int errornum_type;

  /// Type used to verbose error information
  typedef const char* errortext_type;

  /// Type for out-stream
  typedef PBORI_OSTREAM ostream_type;
};

END_NAMESPACE_PBORI

#ifdef PBORI_DEVELOPER
# define PBORI_NOT_IMPLEMENTED \
  throw PBORI::PBoRiError(PBORI::CTypes::not_implemented);
#else
# define PBORI_NOT_IMPLEMENTED 
#endif

#endif // of #ifndef pbori_defs_h_
