from sys import path

from gbrefs import my_import
#from data import stas1 as data
from nf import *

datafilename="data.stas1";

try:
    from custom_profile import filename as datafilename;
except:
    print 'No custom_profile.py file! Using ' + datafilename + '.';

data=my_import(datafilename)
I=buchberger_C_based2(data.ideal)
