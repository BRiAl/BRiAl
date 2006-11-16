from sys import path

from gbrefs import load_data, clean_data, my_import
#from data import stas1 as data
from nf import *

datafilename="data.stas1";

try:
    from custom_profile import filename as datafilename;
except:
    print 'No custom_profile.py file! Using ' + datafilename + '.';

data=load_data(datafilename, base_dir="../testsuite/py/")
I=buchberger_C_based2(data.ideal,optAllowRecursion=False)
del I
clean_data(data)
del data
