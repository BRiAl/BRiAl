/*
 * PolyBoRi-Singular interface
 */

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

#include <locals.h>
#include <omalloc.h>

BOOLEAN mod_lex_boolean_gb(leftv res, leftv h);


#include "pbwrapper.h"
#include <ideals.h>



//void initPyPolyBoRi();

ideal lex_bgb(ideal s);


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

/// essentially boost::python::object
class Psico {
public:
  // copy constructor without NULL checking, for efficiency. 
  Psico(const  Psico& rhs): m_ptr(rhs.m_ptr)  { Py_INCREF(m_ptr); }
  Psico(PyObject* ptr):  m_ptr(ptr) {}
  
  Psico& operator=(Psico const& rhs){
    Py_INCREF(rhs.m_ptr);
    Py_DECREF(this->m_ptr);
    this->m_ptr = rhs.m_ptr;
    return *this;
  }

  ~Psico() {  Py_DECREF(m_ptr); }


  // Underlying object access -- returns a borrowed reference
  PyObject* ptr() const {  return m_ptr; }

private:
  PyObject* m_ptr;
};


class PsicoString:
  public Psico {
public:
  typedef Psico base;

  PsicoString(const char* str): base(PyString_FromString(str)) {}
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

};



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

int pycommand() {
  Py_Initialize();
  // initPyPolyBoRi();
  PyRun_SimpleString("from sys import path");
  PyRun_SimpleString("path.append('.')");
  PyRun_SimpleString("path.append('../pyroot')");

  PyRun_SimpleString("from polybori import *");
  PyRun_SimpleString("from polybori.gbcore import *");
  //  PyRun_SimpleString("help(groebner_basis)");

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


ideal lex_bgb(ideal s){
  pycommand();
  return do_lex_gb(s);
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





extern "C" {
  int mod_init(SModulFunctions* psModulFunctions) {
    
    psModulFunctions->iiAddCproc(currPack->libname,(char*)"lex_boolean_gb",FALSE, mod_lex_boolean_gb);
    return 0;
  }
}
