// -*- c++ -*-
//*****************************************************************************
/** @file embed.h 
 *
 * @author Alexander Dreyer 
 * @date 2012-06-15
 *
 * This file is a header-only framework for supporting PolyBoRi embedding.
 * It also incorporates Python (via boost_python).
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_embed_h_
#define polybori_embed_h_

// include basic definitions
#include <polybori/polybori.h>

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <set>


#ifndef PBORI_PYTHONPATH 
#define PBORI_PYTHONPATH "."
#endif

BEGIN_NAMESPACE_PBORI

/// objects from namespace boost::python;
using boost::python::str;
using boost::python::import;
using boost::python::handle;
using boost::python::borrowed;
using boost::python::extract;
using boost::python::stl_input_iterator;
using boost::python::error_already_set;


/// convenience aliases
typedef BoolePolyRing Ring;
typedef BoolePolynomial Polynomial;
typedef BooleVariable Variable;
typedef BooleMonomial Monomial;

/// copy enums for convenience
enum order_codes {
    lp = PBORI::COrderEnums::lp,
    dlex = PBORI::COrderEnums::dlex,
    dp_asc = PBORI::COrderEnums::dp_asc,
    block_dlex = PBORI::COrderEnums::block_dlex,
    block_dp_asc = PBORI::COrderEnums::block_dp_asc
};

/** @class Interpreter
 * This class initializes and finalized the python interpreter.
 *
 * It also imports polybori and stores the main context.
 * @note If the polybori python module is not at default location you may set
 * the macro PBORI_PYTHONPATH accordingly (before including this header).
 **/
class Interpreter {
public:

  /// Destructor
  ~Interpreter()  { if(m_owns_python) Py_Finalize();  }

  /// Initialize unique (singleton) python interpreter instance, 
  /// and set Singular type identifier
  static void init() { instance(); }

  /// acces corresponding context
  static boost::python::object& globals() { return instance().m_globals; }

private:
  /// Singleton: Only instance() is allowed to construct an instance
  Interpreter():
    m_owns_python(false), m_globals() {
     if (!Py_IsInitialized()) init_python();
     set_python_defaults();
  }

  /// Static initialization - 
  /// safely takes care of destruction on program termination
  static Interpreter& instance() {
    static Interpreter init_interpreter;
    return init_interpreter;
  }

  /// start the python interpreter, it not done yet
  void init_python() {
    Py_Initialize();
    m_owns_python = true;
  }

  /// @note Always keep this routine in header to allow custom PBORI_PYTHONPATH 
  void set_python_defaults() {
    // Sone python modules needs argc, argv set for some reason
    const char* argv = "";
    PySys_SetArgv(1, (char**)&argv);
    import("sys").attr("path").attr("insert")(0, PBORI_PYTHONPATH);
    PyRun_SimpleString("from polybori.frontend import *");

    m_globals = import("__main__").attr("__dict__");
    boost::python::object start = boost::python::eval("polybori_start", m_globals, m_globals);
    start(m_globals);
  }    

  /// store whether this instance had initialized python
  bool m_owns_python;

  /// the active global context
  boost::python::object m_globals;
};

template <class Type>
class DerefProxy {
  typedef DerefProxy self;
public:

  DerefProxy(const Type& val): m_val(val) {}

  DerefProxy<self> operator*() {
    return *this;
  }
  const Type& get() const{ return  m_val; }

private:
  const Type& m_val;
};

/** @class dict
 * This class extends boost::python::tuple by a Python-style **-operator 
 **/
class dict:
  public boost::python::dict {
  typedef dict self;
  typedef boost::python::dict base;

public:
  /// Unary constructors
  template <class Type>
  dict(const Type& obj): base(obj) {}

  /// Default constructor
  dict(): base() {}

  //// Mimicking Pythons **-operator
  DerefProxy<self> operator*() {
    return *this;
  }
};

/** @class tuple
 * This class extends boost::python::tuple by a Python-style *-operator 
 **/
class tuple:
  public boost::python::tuple {
  typedef tuple self;
  typedef boost::python::tuple base;

public:
  /// Unary constructors
  template <class Type>
  tuple(const Type& obj): base(obj) {}

  /// Default constructor
  tuple(): base() {}

  //// Mimicking Pythons *-operator
  DerefProxy<self> operator*() {
    return *this;
  }
};

/** @class object
 * This class extends boost::python::object by nice conversion operators
 **/
class object:
  public boost::python::object {

  typedef object self;
  typedef boost::python::object base;

public:
  /// Unary constructors
  template <class Type>
  object(const Type& obj): base(obj) {}

  /// Default constructor
  object(): base() {}

  /// Generic call operation
  self
  operator()(const DerefProxy<tuple>& args,
             const DerefProxy<DerefProxy<dict> >& kwds) const {
    return object(handle<>(borrowed(PyObject_Call(base::ptr(), args.get().ptr(),
                                                  kwds.get().get().ptr()))));
  }
  /// @name Call with plain arguments
  //@{
  template <class Type>
  self operator()(const Type& arg) const {
    return static_cast<const boost::python::object&>(*this)(arg);
  }
  template <class Type1, class Type2>
  self operator()(const Type1& arg1, const Type2& arg2) const {
    return static_cast<const boost::python::object&>(*this)(arg1, arg2);
  }
  template <class Type1, class Type2, class Type3>
  self operator()(const Type1& arg1, const Type2& arg2, const Type3& arg3) const {
    return static_cast<const boost::python::object&>(*this)(arg1, arg2, arg3);
  }
  template <class Type1, class Type2, class Type3, class Type4>
  self operator()(const Type1& arg1, const Type2& arg2, const Type3& arg3,
                  const Type4& arg4) const {
    return static_cast<const boost::python::object&>(*this)(arg1, arg2, arg3,
                                                            arg4);
  }
  //@}

  /// @name Call with keyword arguments
  //@{
  template <class Type>
  self operator()(const DerefProxy<DerefProxy<dict> >& kwds) const {
    return self::operator()(DerefProxy<tuple>(boost::python::make_tuple()), kwds);
  }
  template <class Type>
  self operator()(const Type& arg,
                  const DerefProxy<DerefProxy<dict> >& kwds) const {
    return self::operator()(DerefProxy<tuple>(boost::python::make_tuple(arg)), kwds);
  }
  template <class Type1, class Type2>
  self operator()(const Type1& arg1, const Type2& arg2,
                  const DerefProxy<DerefProxy<dict> >& kwds) const {
    return self::operator()(DerefProxy<tuple>(boost::python::make_tuple(arg1, arg2)), kwds);
  }
  template <class Type1, class Type2, class Type3>
  self operator()(const Type1& arg1, const Type2& arg2, const Type3& arg3,
                  const DerefProxy<DerefProxy<dict> >& kwds) const {
    return self::operator()(DerefProxy<tuple>(boost::python::make_tuple(arg1, arg2, arg3)), kwds);
  }
  template <class Type1, class Type2, class Type3, class Type4>
  self operator()(const Type1& arg1, const Type2& arg2, const Type3& arg3,
                  const Type4& arg4, const DerefProxy<DerefProxy<dict> >& kwds) const {
    return self::operator()(DerefProxy<tuple>(boost::python::make_tuple(arg1, arg2, arg3,
                                                                        arg4)), kwds);
  }
  //@}

  /// @name Convertig to PolyBoRi types and some built-ins
  //@{
  operator bool() const {
    return extract<bool>(*this);
  }
  operator int() const {
    return extract<int>(*this);
  }
  operator long() const {
    return extract<long>(*this);
  }
  operator std::string() const {
    return extract<std::string>(*this);
  }

  operator const Ring&() const {
    return extract<const Ring&>(*this);
  }
  operator Polynomial() const {
    return extract<Polynomial>(*this);
  }
  operator Monomial() const {
    return extract<Monomial>(*this);
  }
  operator const Variable&() const {
    return extract<const Variable&>(*this);
  }
  operator BooleSet() const {
    return extract<BooleSet>(*this);
  }
  template <class Type>
  operator std::vector<Type>() const {
    stl_input_iterator<self> begin(*this), end;
    return std::vector<Type>(begin, end);
  }
  template <class Type>
  operator std::list<Type>() const {
    stl_input_iterator<self> begin(*this), end;
    return std::list<Type>(begin, end);
  }
  template <class Type>
  operator std::set<Type>() const {
    stl_input_iterator<self> begin(*this), end;
    return std::set<Type>(begin, end);
  }
  //@}
};


inline object
eval(str expression) {

try {
  return boost::python::eval(expression,
                             Interpreter::globals(), 
                             Interpreter::globals());
 }
 catch( error_already_set ) {
   PyErr_Print();
 }
 return object();
}

inline object
exec(str code) {
  try {
    return boost::python::exec(code,
                               Interpreter::globals(), 
                               Interpreter::globals());  
  }
  catch( error_already_set ) {
    PyErr_Print();
  }
  return object();
}

inline object
exec_file(str filename) {
  try {
    return boost::python::exec_file(filename,
                                    Interpreter::globals(), 
                                    Interpreter::globals());  
  }
  catch( error_already_set ) {
    PyErr_Print();
  }
  return object();
}


inline void
run (const char* code) {
  PyRun_SimpleString(code);
}


END_NAMESPACE_PBORI


namespace boost { namespace python { namespace converter {
  template <>
  struct object_manager_traits<PBORI::dict>: 
     object_manager_traits<boost::python::dict> {};

  template <>
  struct object_manager_traits<PBORI::tuple>:
     object_manager_traits<boost::python::tuple> {};

  template <>
  struct object_manager_traits<PBORI::object>:
     object_manager_traits<boost::python::object> {};

}}}

#define BEGIN_PBORI_EMBED() try { USING_NAMESPACE_PBORI; while(0)
#define END_PBORI_EMBED()  } catch(PBORI::error_already_set) { PyErr_Print(); } while(0)

#endif /* polybori_embed_h_ */

