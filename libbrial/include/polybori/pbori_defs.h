// -*- c++ -*-
//*****************************************************************************
/** @file pbori_defs.h 
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
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_pbori_defs_h_
#define polybori_pbori_defs_h_

#include <cstddef>

#include <iostream>

/// @note PolyBoRi comes with a special copy of Cudd
#include <polybori/cudd/cudd.h>

#include "config.h"

#ifndef PBORI_UNIQUE_SLOTS
#  define PBORI_UNIQUE_SLOTS CUDD_UNIQUE_SLOTS // initial size of subtables
#endif

#ifndef PBORI_CACHE_SLOTS
#  define PBORI_CACHE_SLOTS CUDD_CACHE_SLOTS   // default size of the cache
#endif

#ifndef PBORI_MAX_MEMORY
#  define PBORI_MAX_MEMORY 0    // target maximum memory occupation
                                // if PBORI_MAX_MEMORY == 0 then
                                // guess based on the available memory  
#endif


/// For optimizing if-branches
#ifdef __GNUC__
#ifndef PBORI_LIKELY
#define PBORI_LIKELY(expression) (__builtin_expect(!!(expression), 1))
#endif
#ifndef PBORI_UNLIKELY
#define PBORI_UNLIKELY(expression) (__builtin_expect(!!(expression), 0))
#endif
#else
#ifndef PBORI_LIKELY
#define PBORI_LIKELY(expression) (expression)
#endif
#ifndef PBORI_UNLIKELY
#define PBORI_UNLIKELY(expression) (expression)
#endif
#endif 

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

// @todo force assertIon of PBORI_DEBUG
#if defined(PBORI_NDEBUG)
#define PBORI_ASSERT(arg) (static_cast<void>(0))
#else
#define PBORI_ASSERT(arg) assert(arg)
#endif

/// Get output stream type
#ifndef PBORI_NO_STDSTREAMS

# include <iostream>
# define PBORI_OSTREAM std::ostream

#else

BEGIN_NAMESPACE_PBORI

/// @struct Dummy structure if std::ostream is not available
struct PBORI_OSTREAM {};

template <class StreamedType>
PBORI_OSTREAM& 
operator<<(PBORI_OSTREAM& dummy, const StreamedType&) {
  return dummy;
};
END_NAMESPACE_PBORI

#endif // of #ifndef PBORI_NO_STDSTREAMS


BEGIN_NAMESPACE_PBORI


/** @class COrderEnums
 * @brief This struct contains type definitions and enumerations to be used for
 * order codes.
 * 
 **/
struct COrderEnums {
  /// Codes For orderings
  enum ordercodes {
    lp,
    dlex,
    dp_asc,
    block_dlex,
    block_dp_asc
  };
};

/** @class CErrorEnums
 * @brief This struct contains type definitions and enumerations to be used for
 * error codes.
 * 
 **/
struct CErrorEnums {
  /// Error codes enumeration
  enum errorcodes {
    alright = 0,
    failed,
    no_ring,
    invalid,
    out_of_bounds,
    io_error,
    monomial_zero,
    illegal_on_zero,
    division_by_zero,
    invalid_ite,
    not_implemented,
    matrix_size_exceeded,

    last_error
  };
};

/** @class CCompareEnums
 * @brief This struct contains type definitions and enumerations to be used for
 * comparisons.
 * 
 **/
struct CCompareEnums {
  /// Comparision codes enumeration
  enum comparecodes {
    less_than = -1,
    equality = 0,
    greater_than = +1,
    less_or_equal_max = 0,
    greater_or_equal_min = 0
  };

  enum { max_idx = CUDD_MAXINDEX };
};

/** @class CAuxTypes
 * @brief This struct contains auxiliary type definitions.
 * 
 **/
struct CAuxTypes {
  //-------------------------------------------------------------------------
  // types for several purposes
  //-------------------------------------------------------------------------

  /// Type for standard true/false statements
  typedef bool bool_type;

  /// Type for lengths, dimensions, etc.
  typedef std::size_t size_type;

  /// Type for polynomial degrees (ranges from -1 to maxint)
  typedef int deg_type;

  /// Type for integer numbers
  typedef int integer_type;

  /// Type for indices 
  typedef int idx_type;

  /// Type for hashing
  typedef std::size_t hash_type;

   /// Type used to store error codes
  typedef unsigned int errornum_type;

  /// Type for comparisons
  typedef short int comp_type;

  /// Type for ordering codes
  typedef int ordercode_type;

  /// Type used to verbose error information
  typedef const char* errortext_type;

  /// Type for out-stream
  typedef PBORI_OSTREAM ostream_type;

  /// Type for setting/getting names of variables
  typedef const char* vartext_type;

  /// large size_type (necessary?)
  typedef unsigned long large_size_type;

  /// Type for counting references
  typedef std::size_t refcount_type;
};

class BooleSet;
class BoolePolyRing;

/** @class CTypes
 * @brief This struct contains type definitions to be used in library classes
 *
 * It mainly consists of definitions for
 * - the type of decicion diagrams (chosen from the @c cudd library),
 * - built-in types to be used consistently all over the library
 * 
 **/
struct CTypes: 
  public COrderEnums, public CErrorEnums, public CCompareEnums,
  public CAuxTypes {
  //-------------------------------------------------------------------------
  // types for treatment of decision diagrams
  //-------------------------------------------------------------------------

  /// @name get enumeration base definitions
  //@{
  typedef COrderEnums orderenums_type;
  typedef CErrorEnums errorenums_type;
  typedef CCompareEnums compenums_type;
  typedef CAuxTypes auxtypes_type;
  //@}


  /// Type of interface to binary decicion diagrams 
  //  typedef BooleSet dd_type;


  /// Fix type and value of maximal index
  static idx_type max_index() { return max_idx; }
};

END_NAMESPACE_PBORI

#ifdef PBORI_DEVELOPER
# define PBORI_NOT_IMPLEMENTED \
  throw PBORI::PBoRiError(PBORI::CTypes::not_implemented);
#else
# define PBORI_NOT_IMPLEMENTED 
#endif

// Set default addition method
#if defined(PBORI_ADD_BY_ITE) || defined(PBORI_ADD_BY_OR) \
    || defined(PBORI_ADD_BY_UNION) || defined(PBORI_ADD_BY_EXTRA_XOR) \
    || defined(PBORI_ADD_BY_XOR)
#else
# define PBORI_ADD_BY_XOR
#endif 


// Set default union-xor method
#ifdef PBORI_ADD_BY_XOR
# define PBORI_LOWLEVEL_XOR 
#endif 

// Set default method for getting all used variables
#if defined(PBORI_USEDVARS_BY_IDX) || defined(PBORI_USEDVARS_BY_TRANSFORM) \
  || defined(PBORI_USEDVARS_HIGHLEVEL)|| defined(PBORI_USEDVARS_BY_SUPPORT)\
  || defined(PBORI_USEDVARS_EXTRA)
#else
# define PBORI_USEDVARS_BY_IDX
//PBORI_USEDVARS_EXTRA
#endif 


#endif // of #ifndef polybori_pbori_defs_h_
