import PyPolyBoRi
##import warnings
##warnings.simplefilter('ignore', RuntimeWarning) 

from sys import path
#print path

from polybori.gbrefs import load_data, clean_data, my_import

##warnings.simplefilter('default', RuntimeWarning)

#from data import stas1 as data
from polybori.nf import *
from polybori.PyPolyBoRi import change_ordering, OrderCode

datafilename="data.stas1"
ordername="lp"

try:
    from custom_profile import filename as datafilename
except:
    print 'No filename in custom_profile.py file! Using ' + datafilename + '.'
try:
    from custom_profile import ordering as ordername
except:
    print 'No ordering in custom_profile.py file! Using ' + ordername + '.'

data=load_data(datafilename, base_dir="../testsuite/py/")
change_ordering(getattr(OrderCode,ordername))
I=buchberger_C_based2(data.ideal,optAllowRecursion=False)
del I
clean_data(data)
del data
