from sys import path
print path
from polybori.gbrefs import load_data, clean_data, my_import
#from data import stas1 as data
from polybori.nf import *
from polybori.PyPolyBoRi import change_ordering, OrderCode
datafilename="data.stas1"
ordername="data.stas1"
try:
    from custom_profile import filename as datafilename
except:
    print 'No custom_profile.py file! Using ' + datafilename + '.'
try:
    from custom_profile import ordername
except:
    print 'No custom_profile.py file! Using ' + ordername + '.'

data=load_data(datafilename, base_dir="../testsuite/py/")
change_ordering(getattr(OrderCode,ordername))
I=buchberger_C_based2(data.ideal,optAllowRecursion=False)
del I
clean_data(data)
del data
