// -*- c++ -*-
//*****************************************************************************
/** @file CExtrusivePtr.h
 *
 * @author Alexander Dreyer
 * @date 2010-08-24
 *
 * This is essentially a reimplementation of the interface from
 * boost:intrusive_ptr, but with an additional field for storing data, which
 * might be used .
 *
 * @par Copyright:
 *   (c) 2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_common_CExtrusivePtr_h_
#define polybori_common_CExtrusivePtr_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <algorithm>            // std::swap

BEGIN_NAMESPACE_PBORI

/** @class CExtrusivePtr
 * @brief This template class defines a reimplementation of the interface from
 * boost:intrusive_ptr, but with an additional data field.
 *
 * The data field can be used as a helper for the incrementing and decrementing
 * reference counts. Very much like in boost::intrusive_ptr, the following
 * functions have to be defined:
 * 
 * @code
 void extrusive_ptr_release(const DataType&, ValueType*);
 void extrusive_ptr_add_ref(const DataType&, ValueType*)
 * @endcode
 * If @c DataType is ignored, this is essentially
 * @c boost::intrusive_ptr, while something like DataType = int* could be used
 * to implement something like @c boost::shared_ptr.
 **/
template <class DataType, class ValueType>
class CExtrusivePtr {

  /// Name type of *this
  typedef CExtrusivePtr self;

public:

  /// Type for additional data storange
  typedef DataType data_type;

  /// Value type
  typedef ValueType value_type;

  /// Construct managed pointer with additional data
  CExtrusivePtr(const data_type& data, value_type* ptr): 
    m_data(data), p_ptr(ptr) { lock(); }

  /// Copy constructor
  CExtrusivePtr(const self& rhs): 
    m_data(rhs.m_data), p_ptr(rhs.p_ptr) { lock(); } 

  CExtrusivePtr(): 
    m_data(), p_ptr(NULL) { } 

  /// Destructor
  ~CExtrusivePtr() { release(); }

  /// Assignment
  self& operator=(const self& rhs) {
    self(rhs).swap(*this);
    return *this;
  }

  /// Accessing helpter data
  const data_type& data() const { return m_data; }

  /// Get actual pointer
  value_type* get() const {
    return p_ptr;
  }

  /// Constant dereferencing
  const value_type & operator*() const {
    PBORI_ASSERT(p_ptr != NULL);
    return *p_ptr;
  }

  /// Nonconstant dereference
  value_type & operator*() {
    PBORI_ASSERT(p_ptr != NULL);
    return *p_ptr;
  }
  
  /// Pointer operator
  value_type* operator->() const {
    PBORI_ASSERT(p_ptr != NULL);
    return p_ptr;
  }

  /// Swap
  void swap(self & rhs) {
    std::swap(m_data, rhs.m_data);
    std::swap(p_ptr, rhs.p_ptr);
  }

protected:
  void lock() {
    extrusive_ptr_add_ref(data(), get());
  }
  void release() {
    extrusive_ptr_release(data(), get());
  }

  /// Store helper data
  data_type m_data;

  /// Store actual pointer
  value_type* p_ptr;
};

/// Equality check
template <class Data1, class Type1, class Data2, class Type2> 
inline bool
operator==(const CExtrusivePtr<Data1, Type1> & lhs, 
           const CExtrusivePtr<Data2, Type2> & rhs) {
  return lhs.get() == rhs.get();
}

/// Nonequality check
template <class Data1, class Type1, class Data2, class Type2>
inline bool
operator!=(const CExtrusivePtr<Data1, Type1> & lhs,
           const CExtrusivePtr<Data2, Type2> & rhs) {
  return lhs.get() != rhs.get();
}

/// Equality check wrt. pointer
template <class Data1, class Type1, class Type2>
inline bool
operator==(const CExtrusivePtr<Data1, Type1> & lhs, Type2 * rhs) {
  return lhs.get() == rhs;
}

/// Nonequality check wrt. pointer
template <class Data1, class Type1, class Type2>
inline bool 
operator!=(const CExtrusivePtr<Data1, Type1> & lhs, Type2* rhs) {
  return lhs.get() != rhs;
}

/// Equality check from a pointer
template <class Type1, class Data2, class Type2>
inline bool
operator==(Type1* lhs, const CExtrusivePtr<Data2, Type2> & rhs) {
  return lhs == rhs.get();
}

/// Nonequality check from a pointer
template <class Type1, class Data2, class Type2>
inline bool
operator!=(Type1* lhs, const CExtrusivePtr<Data2, Type2> & rhs) {
  return lhs != rhs.get();
}

END_NAMESPACE_PBORI

#endif
