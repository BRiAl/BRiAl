#import PyPolyBoRi

from sys import path
#print path

from polybori.gbrefs import load_data, clean_data, my_import
from polybori.gbcore import groebner_basis

#from data import stas1 as data
from polybori.nf import *
from polybori.PyPolyBoRi import *

datafilename="data.stas1"
ordername="lp"
prot=True

try:
    from custom_profile import filename as datafilename
except:
    print 'No filename in custom_profile.py file! Using ' + datafilename + '.'
try:
    from custom_profile import ordering as ordername
except:
    print 'No ordering in custom_profile.py file! Using ' + ordername + '.'
try:
    from custom_profile import protocol as prot
except:
    print 'No protocol in custom_profile.py file! Using ' + prot + '.'
    
data=load_data(datafilename, base_dir="../../polybori-testsuite/")
ring = data.ideal[0].ring().clone(ordering=getattr(OrderCode,ordername))

I=groebner_basis([ring(poly) for poly in data.ideal], prot=prot)


## from_ring=global_ring()
## change_ordering(lp)
## to_ring=global_ring()
## vec=BoolePolynomialVector()
## for p in I:
##     vec.append(p)
## from time import sleep
## sleep(30)
## res=FGLMStrategy(from_ring,to_ring, vec).main()
