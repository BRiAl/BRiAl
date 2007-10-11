#include <boost/python.hpp>

#include "misc_wrapper.h"

#include "polybori.h"
#include "pbori_defs.h"

using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
#include "Poly_wrapper.h"

BoolePolynomial
do_mapping(const BoolePolynomial& poly, const BooleMonomial& fromVars, 
           const BooleMonomial& toVars) {

  return mapping(poly, fromVars, toVars);
}

void export_misc(){
  def("mapping", do_mapping);
}
