// -*- c++ -*-
//*****************************************************************************
/** @file prefix.h 
 *
 * @author Alexander Dreyer
 * @date 2012-02-15
 *
 * This file includes some macro definitions
 *
 * @note It is intended to be included by the header files of the CUDD fork
 * coming with PolyBoRi. It defines macros, which can be used to CUDD's symbols
 * with @c pbori_ to avoid name clashing and errorous linking of third party
 * code to @c libpolybori  (instead of @c libcudd).
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_cudd_prefix_h_
#define polybori_cudd_prefix_h_

#ifdef PBORI_USE_ORIGINAL_CUDD
#define PBORI_PREFIX(name) name
#else
#define PBORI_PREFIX(name) pbori_ ## name
#endif

#endif
