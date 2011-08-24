/*  Copyright (c) 2005-2007 by The #+PolyBoRi Team */

#include <boost/python.hpp>
#include <iostream>
#include <polybori.h>
#include <polybori/pbori_defs.h>
#include "main_wrapper.h"
#include "dd_wrapper.h"
#include "Poly_wrapper.h"
#include "navigator_wrap.h"
#include "variable_block.h"
#include "strategy_wrapper.h"
#include "monomial_wrapper.h"
#include "misc_wrapper.h"
#include "set_wrapper.h"
#include "out_helper.h"
#include "test_util.h"
#include "fglm_wrapper.h"
#ifdef HAVE_NTL
#include "ntl_wrapper.h"
#endif
#ifdef HAVE_M4RI
#define PACKED 1
extern "C"{
#include <m4ri/m4ri.h>
}

#endif
using namespace boost::python;
using namespace std;

//EXPORT
BOOST_PYTHON_MODULE(PyPolyBoRi){

  export_main();
  export_strategy();
  export_fglm();
  export_monomial();
  export_bset();
  export_variable_block();
  export_misc();
}

#ifdef PB_STATIC_PROFILING_VERSION
int main(int argc,char* argv[]){
  Py_Initialize();
  initPyPolyBoRi();
  PyRun_SimpleString("from sys import path");
  PyRun_SimpleString("path.append('.')");
  PyRun_SimpleString("path.append('../pyroot')");

  PyRun_SimpleString("import toprofile");
  Py_Finalize();
  return 0;
}
#endif
/*


BDD PortToBdd() const;
*/
