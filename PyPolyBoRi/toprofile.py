from sys import path

from gbrefs import my_import
#from data import stas1 as data
from nf import *
data=my_import("data.uuf75.uuf75_5")
I=buchberger_C_based2(data.ideal)
