#include <boost/python.hpp>

#include "misc_wrapper.h"

#include "polybori.h"
#include "pbori_defs.h"
#include "pbori_algorithms.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
#include "Poly_wrapper.h"

void export_misc(){
    def("mapping", mapping<BoolePolynomial,BooleMonomial>);
}