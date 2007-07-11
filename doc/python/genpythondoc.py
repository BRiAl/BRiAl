import sys
import pydoc
import os
from os import sep,chdir
from os.path import abspath

pyroot=sys.argv[1]
pyroot=abspath(pyroot)
chdir("doc/python")
sys.path.append(pyroot)
pydoc.writedocs(pyroot)

