/*
 *  groebner_defs.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 19.04.06.
 *  Copyright 2006 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */

#include <polybori.h>

#ifndef PBORI_GB_DEFS_H
#define PBORI_GB_DEFS_H
#ifndef PBORI_NO_NAMESPACES
#define BEGIN_NAMESPACE_PBORIGB namespace PBORINAME { namespace groebner{
#define END_NAMESPACE_PBORIGB }} // end of namespace 
#else
#endif


BEGIN_NAMESPACE_PBORIGB

class CTypes{
  typedef polybori::BoolePolynomial poly_type;
  typedef polybori::BooleMonomial monom_type;
};
typedef polybori::BoolePolynomial Polynomial;
typedef polybori::BooleMonomial Monomial;
END_NAMESPACE_PBORIGB
typedef long long wlen_type;
typedef long len_type;
typedef int deg_type;
#endif
