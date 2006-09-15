// -*- c++ -*-
//*****************************************************************************
/** @file generic_hash.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes function and class templates defining various hash
 * functions. 
 * 
 * The implementation of the algorithms are based on the work of
 * http://www.partow.net/programming/hashfunctions/
 * It's bases on CPL, so this file (only) may also be CPL. 
 * (Therefore do not use internal definitions here.)
 *
 * @todo Check which algorithms are really useful for us, maybe these are public
 * domain anyway. 
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
 * Revision 1.2  2006/09/15 16:21:03  dreyer
 * CHANGE: testing more sophisticated hash fucntion for BooleExponent
 *
 * Revision 1.1  2006/09/14 10:55:03  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef generic_hash_h_
#define generic_hash_h_

/// @class pbori_hash
/// @brief Experimenting with hashes, based on:
/// http://www.partow.net/programming/hashfunctions/
/// Implementations are CPLed, so best in own module
/// But most algorithms are public domain, so this may not be necessary
/// @todo check license

class generic_hash_tags {
public:
  struct simple_tag {};
  struct rs_tag {};
  struct js_tag {};
  struct pjw_tag {};
  struct elf_tag {};
  struct bkdr_tag {};
  struct sdbm_tag {};
  struct djb_tag {};
  struct dek_tag {};
  typedef dek_tag knuth_tag;
  struct ap_tag {};

  typedef simple_tag default_tag;
}; 

template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType sum,
                      generic_hash_tags::simple_tag) {

  HashType  vars = 0;
  sum = 0;
 
  while (start != finish){
    vars++;
    sum += ((*start)+1) * ((*start)+1);
    ++start;
  }
  return sum * vars;
}


template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::rs_tag) {

  typedef HashType hash_type;
  hash_type bval    = 378551;
  hash_type aval    = 63689;
  hash = 0;
  
  while (start != finish) {
    hash = hash * aval + *start;
    aval    = aval * bval;
    ++start;
  }
  
  return hash;
}

template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::js_tag) {

  hash = 1315423911;
  
  while (start != finish) {
    hash ^= ((hash << 5) + *start + (hash >> 2));
    ++start;
  }
  
  return hash;
}

template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::pjw_tag) {

  unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
  unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
  unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
  unsigned int HighBits          = 
    (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
  
  hash = 0;
  HashType test = 0;
  
  while (start != finish) {
    hash = (hash << OneEighth) + *start;
    
    if((test = hash & HighBits)  != 0) {
        hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
    ++start;
  }
  
  return hash;
}


template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::elf_tag) {

  hash = 0;
  HashType x = 0;

   while (start != finish) {
     hash = (hash << 4) + *start;
      if((x = hash & 0xF0000000L) != 0) {
        hash ^= (x >> 24);
        hash &= ~x;
      }
      ++start;
   }
  return hash;
}

template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::bkdr_tag) {

  HashType seed = 131; // 31 131 1313 13131 131313 etc..
  hash = 0;
  
  while (start != finish) {
    hash = (hash * seed) + *start;
    ++start;
  }

  return hash;
}
template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::sdbm_tag) {

  hash = 0;
  
  while (start != finish) {
    hash = *start + (hash << 6) + (hash << 16) - hash;
    ++start;
  }

  return hash;
}

template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::djb_tag) {

  hash = 5381;
  
  while (start != finish) {
    hash = hash = ((hash << 5) + hash) + *start;
    ++start;
  }

  return hash;
}

template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::dek_tag) {


  hash = static_cast<HashType>(std::distance(start, finish));;
  
  while (start != finish) {
     hash = ((hash << 5) ^ (hash >> 27)) ^ *start;
    ++start;
  }

  return hash;
}

template <class Iterator, class HashType>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::ap_tag) {


  hash = 0;
  
  bool isEven = true;
  while (start != finish) {
       hash ^= ( isEven ? (  (hash <<  7) ^ *start ^ (hash >> 3)) :
                               (~((hash << 11) ^ *start ^ (hash >> 5))));

       isEven = !isEven;
    ++start;
  }

  return hash;
}

template <class VectorType, class HashType, 
          class AlgTag, HashType BitMask = 0x7FFFFFFF>
class generic_hash:
  public generic_hash_tags {
public:
  typedef VectorType vector_type;
  typedef HashType hash_type;
  typedef AlgTag alg_tag;
  enum { mask = BitMask };

  hash_type operator()(const vector_type& vec) const {
    hash_type hash;
    hash = generic_hash_function(vec.begin(), vec.end(), hash, alg_tag() );
    return (hash & mask);
  };
};

#endif
