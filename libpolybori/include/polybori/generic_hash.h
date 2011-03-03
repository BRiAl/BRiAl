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
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef generic_hash_h_
#define generic_hash_h_

/// @class pbori_hash
/// @brief For information on hashes, see
/// http://www.partow.net/programming/hashfunctions/
/// We did not use his implementation, because it is CPLed, but all algorithms
/// used here are prior arts (published before Arash Partow). 


class generic_hash_tags {
public:
  struct simple_tag {};
  struct js_tag {};
  struct pjw_tag {};
  struct elf_tag {};
  struct bkdr_tag {};
  struct sdbm_tag {};
  struct djb_tag {};
  struct dek_tag {};
  typedef dek_tag knuth_tag;

  typedef simple_tag default_tag;
}; 

template <class Iterator, class HashType, class UnaryOp>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType sum,
                      generic_hash_tags::simple_tag, UnaryOp op) {

  HashType  vars = 0;
  sum = 0;
 
  while (start != finish){
    vars++;
    sum += ((op(*start))+1) * ((op(*start))+1);
    ++start;
  }
  return sum * vars;
}


template <class Iterator, class HashType, class UnaryOp>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::js_tag, UnaryOp op) {

  hash = 1315423911;
  
  while (start != finish) {
    hash ^= ((hash << 5) + op(*start) + (hash >> 2));
    ++start;
  }
  
  return hash;
}

template <class Iterator, class HashType, class UnaryOp>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::pjw_tag, UnaryOp op) {

  const HashType nBits = (HashType)(sizeof(HashType) * 8);
  const HashType nTimes3Div4 = (HashType)((nBits  * 3) / 4);
  const HashType nDiv8 = (HashType)(nBits / 8);
  const HashType BitMaskHigh = (HashType)(0xFFFFFFFF) << (nBits - nDiv8);
  
  hash = 0;
  HashType test = 0;
  
  while (start != finish) {
    hash = (hash << nDiv8) + op(*start);
    
    if((test = hash & BitMaskHigh)  != 0) {
        hash = (( hash ^ (test >> nTimes3Div4)) & (~BitMaskHigh));
    }
    ++start;
  }
  
  return hash;
}


template <class Iterator, class HashType, class UnaryOp>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::elf_tag, UnaryOp op) {

  hash = 0;
  HashType tmp = 0;

   while (start != finish) {
     hash = (hash << 4) + op(*start);
      if((tmp = hash & 0xF0000000L) != 0) {
        hash ^= (tmp >> 24);
        hash &= ~tmp;
      }
      ++start;
   }
  return hash;
}

template <class Iterator, class HashType, class UnaryOp>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::bkdr_tag, UnaryOp op) {

  HashType magic_number = 131; 
  hash = 0;
  
  while (start != finish) {
    hash = (hash * magic_number) + op(*start);
    ++start;
  }

  return hash;
}
template <class Iterator, class HashType, class UnaryOp>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::sdbm_tag, UnaryOp op) {

  hash = 0;
  
  while (start != finish) {
    hash = op(*start) + (hash << 6) + (hash << 16) - hash;
    ++start;
  }

  return hash;
}

template <class Iterator, class HashType, class UnaryOp>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::djb_tag, UnaryOp op) {

  hash = 5381;
  
  while (start != finish) {
    hash = ((hash << 5) + hash) + op(*start);
    ++start;
  }

  return hash;
}

template <class Iterator, class HashType, class UnaryOp>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType hash,
                      generic_hash_tags::dek_tag, UnaryOp op) {


  hash = static_cast<HashType>(std::distance(start, finish));
  
  while (start != finish) {
     hash = ((hash << 5) ^ (hash >> 27)) ^ op(*start);
    ++start;
  }

  return hash;
}


class simple_identity {
public:
  template <class ValueType>
  ValueType& operator()(ValueType& val) const { return val; }

  template <class ValueType>
  const ValueType& operator()(const ValueType& val) const { return val; }
};

class simple_increment {
public:

  template <class ValueType>
  ValueType operator()(ValueType val) const { return ++val; }
};

template <class Iterator, class HashType, class HashTag>
HashType
generic_hash_function(Iterator start, Iterator finish, HashType init,
                      HashTag tag) {

  return generic_hash_function(start, finish, init, tag, simple_identity());
}


template <class Iterator, class HashType, 
          class AlgTag, HashType BitMask = 0x7FFFFFFF>
class generic_sequence_hash:
  public generic_hash_tags {

public:
  typedef Iterator iterator_type;
  typedef HashType hash_type;
  typedef AlgTag alg_tag;
  enum { mask = BitMask };

  hash_type operator()(iterator_type start, iterator_type finish) const {
    hash_type hash = 0;
    hash = generic_hash_function(start, finish, hash, alg_tag(), 
                                 simple_increment() );
    return (--hash & mask);
  }
};

template <class VectorType, class HashType, 
          class AlgTag, HashType BitMask = 0x7FFFFFFF>
class generic_hash:
  public generic_hash_tags {
public:
  typedef VectorType vector_type;
  typedef typename vector_type::const_iterator const_iterator;
  typedef HashType hash_type;
  typedef AlgTag alg_tag;
  enum { mask = BitMask };

  hash_type operator()(const vector_type& vec) const {
    return hash_op(vec.begin(), vec.end());
  }
protected:
  generic_sequence_hash<const_iterator, hash_type, alg_tag, mask> hash_op;
};

#endif
