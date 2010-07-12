/*
 * PolyBoRi-Singular interface
 */

#include "PsicoBase.h"          // from Singular's psico branch
#include <boost/shared_ptr.hpp>

#include <boost/python.hpp>
#include "polybori.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mod2.h>
#include <tok.h>
#include <structs.h>
#include <ipid.h>
#include <lists.h>

#include <locals.h>
#include <omalloc.h>

BOOLEAN mod_lex_boolean_gb(leftv res, leftv h);


#include "pbwrapper.h"
#include <ideals.h>



//void initPyPolyBoRi();

ideal lex_bgb(ideal s);

class Psico;


void my_test_function( boost::python::object fun_obj){
  fun_obj(17);
  fun_obj("hihi");
}


void callgb(boost::python::object main_module) {
  using namespace boost::python;

  object result = main_module.attr("myprint")(17);

  BoolePolyRing bring(10);

  boost::python::list args;
  //  args.append(1);
  BoolePolynomial bpoly1 = bring.variable(1);
  BoolePolynomial bpoly2 = bring.variable(1)+bring.variable(2);

  args.append(bpoly1);

  args.append(bpoly2);

  main_module.attr("myprint")(result);
  main_module.attr("myprint")(args);
  
  PyRun_SimpleString("def print_gb(args): print 'gb', groebner_basis(args) "); 
  //  object groebner_basis = main_module.attr("print_gb");
  object groebner_basis = main_module.attr("groebner_basis");
  object gbres = groebner_basis(args);
  main_module.attr("myprint")(gbres);
  
  main_module.attr("myprint")(result);
}


class PsicoDummy: 
  public PsicoBase {
public:
  typedef PsicoBase base;
  typedef PsicoDummy self;

  PsicoDummy(const self& rhs): m_int(rhs.m_int)  { }
  PsicoDummy(int val = 17):  m_int(val) {}
  
  PsicoDummy& operator=(const self& rhs){
    this->m_int = rhs.m_int;
    return *this;
  }

  virtual ~PsicoDummy() {  }
  virtual base* copy() const { return (base*) new self(*this); }
  int value() const {return m_int;}
private:
  int m_int;
};


class PsicoInterpreter {
  typedef PsicoInterpreter self;
public:

  /// Standard constructor
  PsicoInterpreter() {
    std::cerr<<"init"<<std::endl;
    Py_Initialize();
    PyRun_SimpleString("from sys import path");
    PyRun_SimpleString("path.append('.')");
    PyRun_SimpleString("path.append('../pyroot')");

    PyRun_SimpleString("path.append('./Singular')");

    PyRun_SimpleString("from polybori import *");
    PyRun_SimpleString("from polybori.gbcore import *");
    PyRun_SimpleString("from polybori.frontend import *");
    //    PyRun_SimpleString("declare_ring([Block('x',10)], globals())");

    //    PyRun_SimpleString("def myprint(args):\n  print(args)\n   return args;");
  }


  /// Destructor
  ~PsicoInterpreter() {
     std::cerr<<"exiting psico"<<std::endl;
     Py_Finalize();
  }

};




/// essentially boost::python::object
class Psico: 
  public PsicoBase {

  typedef Psico self; 
public:
  typedef PsicoBase base;
  typedef boost::shared_ptr<PsicoInterpreter> interpreter_ptr;
  // copy constructor without NULL checking, for efficiency. 
  Psico(const  Psico& rhs): m_ptr(rhs.m_ptr)  {  Py_XINCREF(m_ptr); }
  Psico(PyObject* ptr):  m_ptr(ptr) {}
  
  Psico& operator=(Psico const& rhs){
    Py_XINCREF(rhs.m_ptr);
    Py_XDECREF(this->m_ptr);
    this->m_ptr = rhs.m_ptr;
    return *this;
  }

  virtual ~Psico() { Py_XDECREF(m_ptr); }
  virtual base* copy() const {
    base* result = new self(*this); 

    return result;
  }
//  self attr(const char* name) const {
//    return attr(PsicoString(name));
//  }

  self attr(const self& key) const {
    return PyObject_GetAttr(ptr(), key.ptr());
  }

  // Underlying object access -- returns a borrowed reference
  PyObject* ptr() const {  return m_ptr; }


  /// Virtual addition operation
  base* radd(base* rhs) const {

    self pstr(PyString_FromString("__radd__"));
    self result(PyObject_CallMethodObjArgs(((self*)rhs)->ptr(),pstr.ptr(), ptr(), 
                                           NULL));


    assert (result.ptr() != Py_NotImplemented);
    return result.copy();
  }

  /// Virtual addition operation
  virtual base* add(base* rhs) const {

    self pstr(PyString_FromString("__add__"));
    self result(PyObject_CallMethodObjArgs(ptr(), pstr.ptr(), ((self*)rhs)->ptr(),
                                           NULL));

    if (result.ptr() == Py_NotImplemented) {
      return radd(rhs);
    }
    return result.copy();
  }

  /// Virtual multiplication operation
  virtual base* times(base* rhs) const {
    self pstr(PyString_FromString("__mul__"));
    self result(PyObject_CallMethodObjArgs(ptr(), pstr.ptr(), ((self*)rhs)->ptr(),
                                           NULL)); 

    if (result.ptr() == Py_NotImplemented) {
      return rtimes(rhs);
    }
    return result.copy();
  }

  /// Virtual multiplication operation
  virtual base* rtimes(base* rhs) const {
    self pstr(PyString_FromString("__rmul__"));
    return self(PyObject_CallMethodObjArgs(((self*)rhs)->ptr(), pstr.ptr(), ptr(),
                                           NULL)).copy(); 
  }

  /// Printing
  virtual void print(void (*printer)(const char*) ) const { 
    printer(c_str()); 
  }
  /// Get cstring
  virtual void cstring(char*& output) const {

    self tmp(PyObject_Str(ptr()));
    char* str = PyString_AsString(tmp.ptr()); 
    output = new char[strlen(str)];
    strcpy(output, str); 
  }
  /// call operation
  virtual base* operator()() const { 

    return new self(PyObject_CallObject(ptr(), NULL)); 
  }

  /// call operation
  virtual base* operator()(const base* args) const { 

    return new self(PyObject_CallObject(ptr(), ((self*)args)->ptr())); 
  }
  virtual base* operator()(void* args) const; // inlined below

  /// call operation
  self call(const self& args) const { 

    return self(PyObject_CallObject(ptr(), args.ptr())); 
  }
  self operator[](Py_ssize_t idx) const {

    return self(PyList_GetItem(ptr(), idx));
  }
  Py_ssize_t size() const { return PyList_Size(ptr()); }

  //protected:
  virtual const char* c_str() const { 
    self tmp(PyObject_Str(ptr()));
    return PyString_AsString(tmp.ptr()); 
  }

protected:
  /// Get tuple with @i nelts elements
  virtual base* clip(unsigned nelts) const { 
    return new self(PyTuple_New(nelts)); 
  }

  /// set nth element
  virtual void set_element(unsigned nth, const base* element) const {
    PyTuple_SetItem(ptr(), nth, ((self*)element)->ptr());
 }

  //private:
  PyObject* m_ptr;

  static interpreter_ptr m_interpreter;
};

/// Initializing Interpreter
Psico::interpreter_ptr Psico::m_interpreter(new PsicoInterpreter);

class PsicoString:
  public Psico {
public:
  typedef Psico base;

  PsicoString(const char* str): base(PyString_FromString(str)) {}
};
/// Factory for globals
class PsicoGlobals:
  public Psico {
  typedef PsicoGlobals self;

public:
  typedef Psico base;

  PsicoGlobals(): base(get()) { }
protected:
  PyObject* get() const {
    PyObject* globals = PyEval_GetGlobals();
    return (globals? globals: PyDict_New());
  }
};



class PsicoObject:
  public Psico {
public:
  typedef Psico base;
  typedef PsicoObject self;
  /// handle with care...
  PsicoObject(const base& rhs): base(rhs) {}
  PsicoObject(const self& rhs): base(rhs) {}

  self attr(const char* name) const {
    return attr(PsicoString(name));
  }

  self attr(const base& key) const {
    return base(PyObject_GetAttr(ptr(), key.ptr()));
  }


  self operator()(const self& args) const {
    return base(PyObject_CallObject(ptr(), args.ptr()));
  }



};

class PsicoImport__:
  public PsicoObject {

protected:
  PsicoImport__(const PsicoString& name): 
    PsicoObject(PyImport_Import(name.ptr())), m_name(name) { }

private:
  PsicoString m_name;  
};

class PsicoImport:
  public PsicoImport__ {
public:
  PsicoImport(const char* name): PsicoImport__(PsicoString(name)) { }

  Psico dict() const { return Psico(PyModule_GetDict(ptr()));}
};

/// Factory for python simple string
class PsicoEval:
  public Psico {
  typedef PsicoEval self;

public:
  typedef Psico base;

  PsicoEval(const char* rhs): base(get(rhs)) { }

protected:
  PyObject* get(const char* rhs) const {
    //PsicoGlobals globals;
    PsicoImport main ("__main__");
    Psico  globals(main.attr("__dict__"));


    PyObject* result= PyRun_String(rhs, Py_eval_input, globals.ptr(),
                                   globals.ptr());
    //    std::cerr<<rhs <<"-> " <<result <<std::endl;
    return result;
  }

};

class PsicoExec:
  public Psico {
  typedef PsicoExec self;

public:
  typedef Psico base;

  PsicoExec(const char* rhs): base(get(rhs)) { }

protected:
  PyObject* get(const char* rhs) const {
    //PsicoGlobals globals;
    PsicoImport main ("__main__");
    Psico  globals(main.attr("__dict__"));
    PyObject* result= PyRun_String(rhs, Py_file_input, globals.ptr(),
                                   globals.ptr());
    //    std::cerr<<rhs <<"-> " <<result <<std::endl;
    return result;
  }

};

BOOLEAN psico_eval(leftv __res, leftv __v) {
  if (__v == NULL) {
    Werror("expected psico_eval('string')");
    return TRUE;
  }

  int __tok =  __v->Typ();

  if (__tok != STRING_CMD) {
    Werror("expected psico_eval('string')");
    return TRUE;
  }
  // std::string args((char *)__v->Data());
  __res->data =(void*) new PsicoEval((char *)__v->Data());

  __res->rtyp = PSICO_CMD;

  return FALSE;
}

BOOLEAN psico_exec(leftv __res, leftv __v) {
  if (__v == NULL) {
    Werror("expected psico_exec('string')");
    return TRUE;
  }

  int __tok =  __v->Typ();

  if (__tok != STRING_CMD) {
    Werror("expected psico_exec('string')");
    return TRUE;
  }
  // std::string args((char *)__v->Data());
  __res->data =NULL;//(void*) new PsicoExec((char *)__v->Data());

  __res->rtyp = NONE;//PSICO_CMD;

  return FALSE;
}
class PsicoTuple:
  public Psico {
public:
  typedef Psico base;
  typedef PsicoTuple self;

  /// Singleton
  PsicoTuple(const base& rhs): base(get_tuple(rhs)) {}


protected:

  base get_tuple(const base& arg) const {
    PyObject *pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, arg.ptr());
    return pArgs;
  }
};

class PsicoTupleN:
  public Psico {
public:
  typedef Psico base;
  typedef PsicoTupleN self;

  PsicoTupleN(long len): base(PyTuple_New(len)) {}

  template <class Iterator>
  PsicoTupleN(Iterator start, Iterator finish):
    base(PyTuple_New(std::distance(start, finish))) {

    long idx = 0;
    while (start != finish) {
      set_item(idx, *start);
      ++start; 
      ++idx;
    }

  }
  int set_item(Py_ssize_t pos, const base& rhs) {
    Py_INCREF(rhs.ptr());       // set_item steals reference
    return PyTuple_SetItem(ptr(), pos, rhs.ptr());
  }
  Py_ssize_t size() const { return PyTuple_Size(ptr()); }

  int resize(Py_ssize_t newsize) {
    return _PyTuple_Resize(&m_ptr, newsize);
  }
};

class PsicoFromSingular;

class PsicoList:
  public Psico {
public:
  typedef Psico base;
  typedef PsicoList self;

  PsicoList(long len = 0):
    base(PyList_New(len)) {
  }

//   PsicoList(void* singlist):
//     base(PyList_New(((lists)singlist)->nr)) {
   
//     for(long idx = 0; idx < ((lists)singlist)->nr; ++idx) {
//       PsicoFromSingular tmp(((lists)singlist)->m[idx]);
//       PyList_SetItem(ptr(), idx, tmp.ptr());
//     }
//   } 

  void append(const base& elt) {
        PyList_Append(base::ptr(), elt.ptr());

    //    base::attr(PsicoString("append"))(&elt);
  }

};


class PsicoInt:
  public Psico {
  typedef PsicoInt self;
public:
  typedef Psico base;
  PsicoInt(long rhs): base(PyInt_FromLong(rhs)) {}


protected:
  base get_tuple(const base& arg) const {
    PyObject *pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, arg.ptr());
    return pArgs;
  }
};

class PsicoFromSingular:
  public Psico {
  typedef PsicoFromSingular self;
public:
  typedef Psico base;
  PsicoFromSingular(leftv pVal): base(get(pVal)) {}

protected:
  static base get(leftv pVal)  {
    PsicoList result;
    void* data = pVal->Data();
    switch (pVal->Typ()) {
    case INT_CMD:
      return PsicoInt((long)data);
    case STRING_CMD:
      return PsicoString((char*)data);    
    case PSICO_CMD:
      return Psico(*((Psico*)data));
     case LIST_CMD:
       //   PsicoList result;
       //     data = pVal->data;
       std::cerr <<"getlist" << ((lists)data)->nr<<std::endl;
       for(long idx = 0; idx <= ((lists)data)->nr; ++idx) {

         //
         //         Psico tmp(get(((lists)data)->m + idx));
         PsicoTuple tmp(*( (Psico*)(( ((lists)data)->m[idx])).Data() ));
         //         Py_XINCREF(tmp.ptr());

         //      PyList_Append(result.ptr(), tmp.ptr());
         tmp.print(PrintS);
         result.append(*(Psico*)tmp.copy());
         //         PyList_Append(result.ptr(), tmp.ptr());
       }
 std::cerr <<"after getlist" << ((lists)data)->nr<<std::endl;
 result.print(PrintS);
 PrintS("afterprint");
 return base(result);
    default:
      std::cerr <<"unknown "<<pVal->Typ() <<std::endl;
      return base(NULL);
    }

  }
};

inline Psico
singular2psico(leftv pVal)  {
 
    void* data = pVal->Data();
    switch (pVal->Typ()) {
    case INT_CMD:
      return PsicoInt((long)data);
    case STRING_CMD:
      return PsicoString((char*)data);    
    case PSICO_CMD:
      return Psico(*((Psico*)data));
    case LIST_CMD:

      lists singlist = (lists)data;
      PsicoList result;

      for (std::size_t i = 0; i <= singlist->nr; ++i) {
        leftv singelt = (singlist->m) + i;
        Psico elt = singular2psico(singelt);
        result.append(elt);
      }
      return result;


    }

}


template <class InputType, class ValueType>
class SingularLikeIterator:
  public std::iterator_traits<InputType> {

  typedef SingularLikeIterator self;
public:
  typedef ValueType value_type;
  typedef InputType input_type;
  typedef value_type reference;
  typedef std::forward_iterator_tag iterator_category;

  /// Constructor
  SingularLikeIterator(const input_type& rhs):
    m_iter(rhs) {}

  /// Copy constructor
  SingularLikeIterator(const self& rhs):
    m_iter(rhs.m_iter) {}

  /// Prefix incrementation operation
  self& operator++() {
    m_iter = m_iter->next;
    return *this;
  }
  /// Postfix incrementation operation
  self operator++(int) {
    self copy(*this);
    operator++();
    return copy;
  }

  /// Dereference operation
  reference operator*() const {
    reference result(singular2psico(m_iter));

    return result;
  }

  /// Equality check
  bool operator==(const self& rhs) const {
    return m_iter == rhs.m_iter;
  }

  /// Nonequality check
  bool operator!=(const self& rhs) const {
    return !operator==(rhs);
  }

private:
  input_type m_iter;
};

inline Psico::base* 
Psico::operator()(void* rhs) const {
  SingularLikeIterator<leftv, Psico> start((leftv)rhs), finish(NULL);

  PsicoTupleN args(start, finish);
  //  print(PrintS);args.print(PrintS);
  self result(PyObject_CallObject(ptr(), args.ptr()));
  
  return result.copy();
}


void test_psico() {
  PyObject *pName, *pModule, *pFunc;
  // Build the name object
//   pName = PyString_FromString("__main__");

//   // Load the module object


//    PsicoString name("__main__");

//    pModule = PyImport_Import(name.ptr());

//    PsicoObject pobj0(pModule);
//    PsicoObject pobj(pobj0);

//   std::cout <<"test_psico 1!"<<std::endl;
//   std::cout.flush();
  // pModule = PyImport_ImportModule("__main__");
  // Psico tmp(pModule);
  //PsicoObject pobj(pModule);

  //  PsicoString name("__main__");
  PsicoImport pobj ("__main__");
  std::cout <<"test_psico 1!"<<std::endl;
  std::cout.flush();

  pobj.attr("myprint")(PsicoTuple(PsicoString("text")));
#if 0
  PsicoObject thedict(pobj.getattr("__dict__"));
  std::cout <<"test_psico 2!"<<std::endl;
  std::cout.flush();

  
  pFunc = PyDict_GetItemString(thedict.ptr(), "myprint");
  //  PsicoObject myprint(thedict.getattr("myprint"));

  PsicoObject myprint(pobj.attr("myprint"));
  std::cout <<"test_psico 3!"<<std::endl;
  std::cout.flush();

  myprint(PsicoTuple(PsicoString("text")));

  std::cout <<"test_psico finished!"<<std::endl;
  std::cout.flush();

  //  Py_DECREF(pModule);
#endif
  //Py_DECREF(pName);
  //  Py_DECREF(pFunc);
}

void pyinit() {
  Py_Initialize();
  // initPyPolyBoRi();
  PyRun_SimpleString("from sys import path");
  PyRun_SimpleString("path.append('.')");
  PyRun_SimpleString("path.append('../pyroot')");

  PyRun_SimpleString("from polybori import *");
  PyRun_SimpleString("from polybori.gbcore import *");
}

int pycommand() {
  pyinit();

  PyRun_SimpleString("print 'huhu'"); 
  PyRun_SimpleString("def myprint(args):\n  print(args)\n  return args"); 
  PyRun_SimpleString("def myhelp(args): help(args)"); 

  test_psico();

#if 0
  using namespace boost::python;

  object main_module = import("__main__");
  // object main_namespace = main_module.attr("__dict__");
  callgb(main_module);


  //  object result =  eval("print groebner_basis(arg_ideal)");
  //  PyRun_SimpleString("print groebner_basis(arg_ideal)"); 

  //main_module.attr("myhelp")(main_module.attr("groebner_basis"));
  //  main_module.attr("groebner_basis")(1);

//  boost::python::object printer  = exec("print", main_namespace);


  //def("my_test_function", my_test_function);

  //  printer("huhu");
  //  boost::python::object hihi  = boost::python::eval("'hihi'");

  //  boost::python::call<void>(printer.ptr(), "hihi");
  //  PyObject_CallObject(printer, "hihi");

    //  PyRun_SimpleString("my_test_function(print)"); 




  PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pClass, *pInstance, *pArgs;

  // Build the name object

  pName = PyString_FromString("__main__");

  // Load the module object
  
  pModule = PyImport_Import(pName);

  // pDict is a borrowed reference 
  
  pDict = PyModule_GetDict(pModule);

  // pFunc is also a borrowed reference 
  
  pFunc = PyDict_GetItemString(pDict, "myprint");

//     if (PyCallable_Check(pFunc)) 
//     {
//         PyObject_CallObject(pFunc, NULL);
//     } else 
//     {
//         PyErr_Print();
//     }
  pArgs = PyTuple_New(1);
  PyTuple_SetItem(pArgs, 0, pName);   


  PyObject_CallObject(pFunc, pArgs);
 

    // Clean up

    Py_DECREF(pModule);
    Py_DECREF(pName);
    Py_DECREF(pArgs);

#endif

  Py_Finalize();
  return 0;
}



Psico
call_func_tuple(const char* name, const Psico& rhs){



  PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pClass, *pInstance, *pArgs;

  // Build the name object

  pName = PyString_FromString("__main__");

  // Load the module object
  
  pModule = PyImport_Import(pName);

  // pDict is a borrowed reference 
  
  pDict = PyModule_GetDict(pModule);

  // pFunc is also a borrowed reference 
  
  pFunc = PyDict_GetItemString(pDict, name);

//     if (PyCallable_Check(pFunc)) 
//     {
//         PyObject_CallObject(pFunc, NULL);
//     } else 
//     {
//         PyErr_Print();
//     }

  pInstance = PyObject_CallObject(pFunc,  rhs.ptr());
 

    // Clean up

  Py_DECREF(pModule);
  Py_DECREF(pName);
  Py_DECREF(pFunc);
  Py_DECREF(pDict);
  return pInstance;
}


Psico
call_func(const char* name, const Psico& rhs){



  PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pClass, *pInstance, *pArgs;

  // Build the name object

  pName = PyString_FromString("__main__");

  // Load the module object
  
  pModule = PyImport_Import(pName);

  // pDict is a borrowed reference 
  
  pDict = PyModule_GetDict(pModule);

  // pFunc is also a borrowed reference 
  
  pFunc = PyDict_GetItemString(pDict, name);

//     if (PyCallable_Check(pFunc)) 
//     {
//         PyObject_CallObject(pFunc, NULL);
//     } else 
//     {
//         PyErr_Print();
//     }
  pArgs = PyTuple_New(1);
  PyTuple_SetItem(pArgs, 0, rhs.ptr());   


  pInstance = PyObject_CallObject(pFunc, pArgs);
 

    // Clean up

  Py_DECREF(pModule);
  Py_DECREF(pName);
  return pInstance;
}



Psico
call_print_iter(const Psico& rhs){



  PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pClass, *pInstance, *pArgs;

  // Build the name object

  pName = PyString_FromString("__main__");

  // Load the module object
  
  pModule = PyImport_Import(pName);

  // pDict is a borrowed reference 
  
  pDict = PyModule_GetDict(pModule);

  // pFunc is also a borrowed reference 
  
  pFunc = PyDict_GetItemString(pDict, "print_iteration_two");

//     if (PyCallable_Check(pFunc)) 
//     {
//         PyObject_CallObject(pFunc, NULL);
//     } else 
//     {
//         PyErr_Print();
//     }
  pArgs = PyTuple_New(1);
  PyTuple_SetItem(pArgs, 0, rhs.ptr());   


  pInstance = PyObject_CallObject(pFunc, pArgs);
 

    // Clean up

  Py_DECREF(pModule);
  Py_DECREF(pName);
  return pInstance;
}

ideal lex_bgb(ideal s){
  pycommand();
  return do_lex_gb(s);
}


BOOLEAN set_psico(leftv __res, leftv __v) {

  //  PsicoDummy pstr(3);

  Psico pstr = PsicoString("das ist der String!");

   std::cerr<< "orig pointer: " <<pstr.ptr()<<std::endl;

   //  __res->data =(void*) pstr.copy();

  __res->data =(void*)new  Psico(pstr);
  std::cerr <<"set_psico " <<__res->data <<std::endl;
   std::cerr<< "set pointer: " <<((Psico*)__res->data)->ptr()<<std::endl;
   std::cerr<< "set pointer: " <<((Psico*)__res->Data())->ptr()<<std::endl;
  __res->rtyp = PSICO_CMD;
  return FALSE;
}


BOOLEAN call_print(leftv __res, leftv __v) {
  if (__v == NULL) {
    Werror("expected call_print('psico')");
    return TRUE;
  }
  int __tok =  __v->Typ();

  if (__tok != PSICO_CMD) {
    Werror("expected call_print('psico')");
    return TRUE;
  }
  PyRun_SimpleString("def myprint(*args):\n  print(args)\n  return args"); 

  //    PsicoObject arg(PsicoString("qQWERQWER"));//*((PsicoObject*)__v->Data()));
  //   std::cerr<< "pointer: " << (__v->Data())<<std::endl;

  //   std::cerr<< "ptr(): " <<((Psico*)__v->Data())->ptr()<<std::endl;

   PsicoObject arg(((Psico*)__v->Data())->ptr());

  //  PsicoImport pobj ("__main__");
  PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pClass, *pInstance, *pArgs;
  pName = PyString_FromString("__main__");

  // Load the module object
  
  pModule = PyImport_Import(pName);

  // pDict is a borrowed reference 
  
  pDict = PyModule_GetDict(pModule);

  // pFunc is also a borrowed reference 
  
  pFunc = PyDict_GetItemString(pDict, "myprint");
  pArgs = PyTuple_New(1);

  PyTuple_SetItem(pArgs, 0, arg.ptr());   


  if( arg.ptr() )
    PyObject_CallObject(pFunc, pArgs);
 

  //  PyRun_SimpleString("print 'fasdfasdfasdf'"); 

  
//   std::cerr<< "pointer: " << ((PsicoDummy*)__v->Data())<<std::endl;
//   std::cerr<< "value: " << ((PsicoDummy*)__v->data)->value()<<std::endl;
//   std::cerr<< "value: " << ((PsicoDummy*)__v->Data())->value()<<std::endl;


//   std::cerr<< "res: " << ((PsicoDummy*)__res)->value()<<std::endl;
  __res->data =(void*) NULL;
  __res->data =(void*) NULL;

  __res->rtyp = NONE;



  return FALSE;
}

BOOLEAN empty_print(leftv __res, leftv __v) {

  //  PsicoImport pobj ("__main__");
  //  pobj.attr("print")(PsicoTuple(PsicoString("asdfasdfasdfa")));
  // convert to Singular objects
  __res->data =(void*) NULL;

  __res->rtyp = ANY_TYPE;
  printf("adasdas");
  return FALSE;
}


BOOLEAN getattr(leftv __res, leftv __v) {

  Psico lhs(singular2psico(__v));
  Psico rhs(__v? singular2psico(__v->next): NULL);

  __res->data =(void*) lhs.attr(rhs).copy();

  __res->rtyp = PSICO_CMD;

  return !__res->data;
}

BOOLEAN simple_string(leftv __res, leftv __v) {
  if ((!__v) || (__v->Typ() != STRING_CMD)) {
    Werror("expected simple_string('string')");
    return TRUE;
  }

  PyRun_SimpleString((char*)__v->Data());
  __res->data =(void*) NULL;
  __res->rtyp = NONE;
  return FALSE;
}


BOOLEAN test_list(leftv __res, leftv __v) {

  std::cout<<"list type? "<< (__v->Typ() == LIST_CMD) <<std::endl;

  //  lists singlist = (lists)__v->Data();

  Psico ll = singular2psico(__v);

  /*
  for (int i=0;i<=singlist->nr; ++i) {
    leftv firstsingelt = (singlist->m)+i;

    Psico firstelt = PsicoFromSingular(firstsingelt);

    firstelt.print(PrintS);

    ll.append(firstelt);
  }
  */
  std::cout<<" print list "<< std::endl;

  ll.print(PrintS);



  

  __res->data =(void*)ll.copy();;
  __res->rtyp = PSICO_CMD;
  return FALSE;
}

void set_singular(leftv __res, const Psico& rhs) {
  __res->data =(void*)rhs.copy();
  __res->rtyp = PSICO_CMD;
}

BOOLEAN declare_ring(leftv __res, leftv __v) {
  __res->data =(void*)NULL;
  __res->rtyp = NONE ;
  //  PsicoTupleN all(2);

  if ((__v->Typ() != LIST_CMD) && (__v->Typ() != PSICO_CMD) ) {
    return TRUE;
  }


  Psico thelist(singular2psico(__v));
  //  PrintS("hierEsle");
  //  thelist.print(PrintS);PrintS("hierMitte");


  //  PyRun_SimpleString("from pb_if import *"); 

  //  call_print_iter(thelist);
    //    PsicoEval print_iteration_two("print_iteration_two");
    //  print_iteration_two(&thelist);
  //  PrintS("hierEsleend");

  //  all.set_item(0, thelist);

//   __res->data =(void*)NULL;                     ;

//    __res->rtyp = NONE  ;
//   return FALSE;

  //  PsicoImport main ("__main__");
  //  Psico  globals(main.attr("__dict__"));

  PsicoEval  globals("globals()");
//   PrintS("gloabls");
//   globals.print(PrintS);

//   PrintS("end globals");
//  all.set_item(1,globals);
  // args.set_item(args.size()-1, globals);
  //if (args.ptr())

  //  PsicoEval command("declare_ring");
  //  command(&args);
  // globals.print(PrintS);


//   PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pClass, *pInstance, *pArgs;

//   pArgs = PyTuple_New(2);
//   PyTuple_SetItem(pArgs, 0, thelist.ptr());   
//   PyTuple_SetItem(pArgs, 1, globals.ptr());   

//  Psico all2(pArgs);

  PsicoTupleN all2(2);
  all2.set_item(0, thelist);
  all2.set_item(1, globals);
  PsicoEval None("None");
  //all2.set_item(1, None);
  //  all2.print(PrintS);

  PsicoEval dec_ring("declare_ring");
  //  Psico result = call_func_tuple("declare_ring", all2);
  __res->data =(void*) dec_ring(&all2);
  __res->rtyp = PSICO_CMD ;

  //  set_singular(__res, result);

//   const char* d = "int siebzehn = 17";
//   char * s = (char *)omAlloc(strlen(d) + 13);
//   strcpy( s, (char *)d);
//   strcat( s, "\n;RETURN();\n");
//   newBuffer((char*)s,BT_execute);
//   //  return 
//   yyparse();


  idhdl currHandle =NULL;// = enterid(omStrDup("achtzehn"),0,PSICO_CMD,&IDROOT,FALSE);

  //  IDDATA(currHandle) = (char*)  None.copy();

  //  PyRun_SimpleString("print block_scheme_names(['qwqwe'])");
  
  PsicoEval block_scheme_names("block_scheme_names");
  //  block_scheme_names.print(PrintS);
  Psico newElts(block_scheme_names.call(PsicoTuple(thelist)));
  //  newElts.print(PrintS);

  long len = newElts.size();
  
  for (long idx = 0; idx < len; ++idx) {
    //newElts[idx].print(PrintS);
    const char* identifier = newElts[idx].c_str();
    currHandle = enterid(omStrDup(identifier),0,PSICO_CMD,&IDROOT,FALSE);

    IDDATA(currHandle) = (char*) PsicoEval(identifier).copy();
  }

  //    PyRun_SimpleString("from sys import modules"); 
  //  PsicoEval("modules['polybori.frontend'].__dict__").print(PrintS);
  
  return FALSE;
#if 0

  PsicoBase* result = PsicoEval("just_print")(&all2);

  PsicoBase* result  =NULL;
  __res->data =(void*) result;

   __res->rtyp = PSICO_CMD;

   // PsicoEval printer("myprint");
   // printer(&glob);
  return FALSE;
#endif
}


BOOLEAN mod_lex_boolean_gb(leftv __res, leftv __h) {


  leftv __v = __h, __v_save;
  int __tok = NONE, __index = 0;
  sleftv __sa; leftv __za = &__sa;
  ideal a;


  // check arguments
  if(__v==NULL) goto mod_lex_boolean_gb_error;
  __tok = __v->Typ();
  if((__index=iiTestConvert(__tok, IDEAL_CMD))==0)
     goto mod_lex_boolean_gb_error;
  __v_save = __v->next;
  __v->next = NULL;
  if(iiConvert(__tok, IDEAL_CMD, __index, __v, __za))
     goto mod_lex_boolean_gb_error;
  __v = __v_save;
  a = (ideal)__za->Data();
  if(__v!=NULL) { __tok = __v->Typ(); goto mod_lex_boolean_gb_error; }

  goto mod_lex_boolean_gb_ok;
  mod_lex_boolean_gb_error:
    Werror("lex_boolean_gb(`%s`) is not supported", Tok2Cmdname(__tok));
    Werror("expected lex_boolean_gb('ideal')");
    return TRUE;
  mod_lex_boolean_gb_ok:

  // call actual routine
  ideal h;
  h=lex_bgb(a);

  // convert to Singular objects
  __res->data =(void*) h;

  __res->rtyp = IDEAL_CMD;
  return FALSE;

}



BOOLEAN get_list(leftv __res, leftv __v) {

  Psico ll = singular2psico(__v);
  __res->data =(void*)ll.copy();;
  __res->rtyp = PSICO_CMD;
 
 return FALSE;
}

BOOLEAN test_refcnt(leftv __res, leftv __v) {

  __res->data =(void*)NULL;
  __res->rtyp = NONE;
  
  PyObject* tpl = PyTuple_New(1);

  PsicoString elt("HUHU");

  std::cerr << "refcount before " << Py_REFCNT(elt.ptr()) <<std::endl;

  PyTuple_SetItem(tpl, 0, elt.ptr());

  Py_INCREF(elt.ptr());
  std::cerr << "refcount after " << Py_REFCNT(elt.ptr()) <<std::endl;

  Py_DECREF(tpl);

  std::cerr << "refcount afterall " << Py_REFCNT(elt.ptr()) <<std::endl;
 
  return FALSE;
}


BOOLEAN psico_import(leftv __res, leftv __v) {

  __res->data =(void*)NULL;
  __res->rtyp = NONE;


  if ((__v == NULL)) {
    Werror("expected psico_import('string')");
    return TRUE;
  }

  int __tok =  __v->Typ();

  if ((__tok != STRING_CMD)) {
    Werror("expected psico_import('string')");
    return TRUE;
  }

  char* modules_name = ((char *)__v->Data());
  const char*  cmd = "[str for str in dir(modules['%s']) if str[0] != '_']";

  char buffer[strlen(cmd) + strlen(modules_name)];

  sprintf (buffer, "from %s import *", modules_name);
  PyRun_SimpleString(buffer);

  sprintf (buffer, cmd, modules_name);

  PsicoEval newElts(buffer);

  //  newElts.print(PrintS);

  long len = newElts.size();
  
  idhdl currHandle =NULL;
  for (long idx = 0; idx < len; ++idx) {
    //newElts[idx].print(PrintS);
    const char* identifier = newElts[idx].c_str();
    currHandle = enterid(omStrDup(identifier),0,PSICO_CMD,&IDROOT,FALSE);

    IDDATA(currHandle) = (char*) PsicoEval(identifier).copy();
  }


  return FALSE;
}


extern "C" {
  int mod_init(SModulFunctions* psModulFunctions) {

    psModulFunctions->iiAddCproc(currPack->libname,(char*)"lex_boolean_gb",FALSE, mod_lex_boolean_gb);

    psModulFunctions->iiAddCproc(currPack->libname,(char*)"psico_eval",FALSE, psico_eval);

    psModulFunctions->iiAddCproc(currPack->libname,(char*)"psico_exec",FALSE, psico_exec);
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"call_print",FALSE, call_print);
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"set_psico",FALSE,
    set_psico);
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"empty_print",FALSE,
    empty_print);
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"getattr",FALSE,getattr);
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"simple_string",FALSE,
                                 simple_string);
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"declare_ring",FALSE,
                                 declare_ring);
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"test_list",FALSE,
                                 test_list);
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"get_list",FALSE,
                                 get_list);

    psModulFunctions->iiAddCproc(currPack->libname,(char*)"test_refcnt",FALSE,
                                 test_refcnt);
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"psico_import",FALSE,
                                 psico_import);
    
    return 0;
  }
}
