// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_misc.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which defines miscellaneous function templates.
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
 * Revision 1.3  2007/11/06 15:03:37  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.2  2006/09/22 16:05:17  dreyer
 * ADD: switch PBORI_HASH_TAG
 *
 * Revision 1.1  2006/09/20 07:06:39  dreyer
 * ADD BoolePolynomial/CDDInterface::isConstant(), used it in deg()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"


BEGIN_NAMESPACE_PBORI

// The following may be used without polybori. Hence, we have to load it in the
// namespace here

// Get generic hash functions
#include "generic_hash.h"

#ifndef PBORI_HASH_TAG
#define PBORI_HASH_TAG elf_tag
#endif


typedef generic_hash_tags::PBORI_HASH_TAG pbori_hash_tag;

END_NAMESPACE_PBORI
