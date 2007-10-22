from sys import modules

# First try, whether PyPolyBoRi is already in modules (e.g. static profiling)
try:
    pb = modules["PyPolyBoRi"]
    for k in dir(pb):
        globals()[k]=getattr(pb,k)   
except:
    from polybori.dynamic.PyPolyBoRi import *

