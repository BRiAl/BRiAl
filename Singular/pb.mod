%{
/*
 *  $Id$
 *
 *  Python module
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <Python.h>
#include "pbwrapper.h"
#include <ideals.h>
ideal lex_bgb(ideal s);
%}

// some comments here

category="tests";
package="polybori_module";
version	= "$Id$";
info	="LIBRARY: kernel.lib  PROCEDURES OF GENERAL TYPE WRITEN IN C python(input); eval a string  in python";
//files= wrapper.cc;
%modinitial
  //Py_Initialize();
  //initSingular();
  //init_Singular();

%endinitial

%procedures
/*
 * NAME:     python
 *           procedure without string as parameter and no return val
 * PURPOSE:  interprets the string in python
 */
ideal lex_boolean_gb(ideal a)
{ 
  %declaration;
  %typecheck;
  ideal h;
  h=lex_bgb(a);
  %return=(void*) h;
}




%%
%C

ideal lex_bgb(ideal s){
return do_lex_gb(s);
 


}



