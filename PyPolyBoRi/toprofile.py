from sys import path

from gbrefs import my_import
#from data import stas1 as data
from nf import *
data=my_import("data.stas1")
I=buchberger_C_based2(data.ideal)
