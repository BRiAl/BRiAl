import sys
import pydoc
sys.path.append("PyPolyBoRi")
sys.path.append("testsuite/py")
import PyPolyBoRi
import nf
import ll
for m in [PyPolyBoRi,nf,ll]:
    pydoc.writedoc(m)