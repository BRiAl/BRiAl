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
