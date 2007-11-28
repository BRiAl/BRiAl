import sys
from pydoc import *
import os
from os import sep,chdir
from os.path import abspath

# work around for python 2.5 on SAGE; deactivated, should work now on SAGE
## def writedocs(dir, pkgpath='', done=None):
##     """Write out HTML documentation for all modules in a directory tree."""
##     if done is None: done = {}
##     for file in os.listdir(dir):
##         path = os.path.join(dir, file)
##         if ispackage(path):
##             print path
##             writedocs(path, pkgpath + file + '.', done)
##         elif os.path.isfile(path):
##             modname = inspect.getmodulename(path)
##             if modname:
##                 if modname == '__init__':
##                     modname = pkgpath[:-1] # remove trailing period
##                 else:
##                     modname = pkgpath + modname
##                 if modname not in done:
##                     done[modname] = 1
##                     writedoc(modname)

pyroot=sys.argv[1]
pyroot=abspath(pyroot)
chdir("doc/python")
sys.path.append(pyroot)
writedocs(pyroot)

