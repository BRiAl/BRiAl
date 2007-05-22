import sys
import pydoc
import os
def writedoc(thing, forceload=0):
    """Write HTML documentation to a file in the current directory."""
    object, name = pydoc.resolve(thing, forceload)
    page = pydoc.html.page(pydoc.describe(object), pydoc.html.document(object, name))
    file = open(sys.argv[2], 'w')
    file.write(page)
    file.close()
    print 'wrote', name + '.html'

sys.path.append("PyPolyBoRi")
sys.path.append("testsuite/py")
from os import sep
todoc=sys.argv[1].split(os.sep)[-1].split(".")[-2]
todoc=__import__(todoc)
writedoc(todoc)


