/*
 * PolyBoRi-Singular interface
 */



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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pbwrapper.h"
#include <ideals.h>

ideal lex_bgb(ideal s);




extern "C" {
  int mod_init(SModulFunctions* psModulFunctions) {
    
    psModulFunctions->iiAddCproc(currPack->libname,"lex_boolean_gb",FALSE, mod_lex_boolean_gb);
    return 0;
  }
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




ideal lex_bgb(ideal s){
return do_lex_gb(s);
 
}
