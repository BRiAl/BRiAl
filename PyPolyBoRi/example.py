#log# Automatic Logger file. *** THIS MUST BE THE FIRST LINE ***
#log# DO NOT CHANGE THIS LINE OR THE TWO BELOW
#log# opts = Struct({'logfile': 'ipython.log', 'log': 1})
#log# args = []
#log# It is safe to make manual edits below here.
#log#-----------------------------------------------------------------------
import PyPolyBoRi
r=PyPolyBoRi.Ring()

vars=[r.var(i) for i in range(r.nVars())]
vars
vars[1].printMinterm()
x1=vars[1]
x2=vars[2]
x3=vars[3]
x1*x2
o=x1*x2
o.printMinterm()
o=x1*x2+x1
o.printMinterm()
o.printToStdout(100,1)
print "subset0"
def ring_var(i):
    res=vars[i]
    for j in range(0,r.nVars()):
        if i!=j:
            res=res.subset0(j)
    return res


#x1.subset0(x2).printMinterm()
#print "subset0"
#x1.subset1(x2).printMinterm()
#del vars
del o
#del x1
#del x2
#del x3
v=[ring_var(i) for i in range(r.nVars())]
for xi in v:
    xi.printMinterm()
    xi.printToStdout(100,1)

def myprint(f):
    f.printMinterm()
    #f.printCover()
    f.printToStdout(r.nVars(),1)
zdd_false=v[1] & v[2]

print "generated variables"
def poly_add(a,b):
    return a.diff(b) +b.diff(a)
def poly_add2(a,b):
    return a.union(b).diff(a.intersect(b))
myprint(zdd_false)
#myprint(zdd_false-v[1])
#myprint(v[1])
myprint(poly_add(v[1],v[2]))
myprint(poly_add(poly_add(v[1],v[2]),v[1]))
myprint(poly_add2(v[1],v[2]))
myprint(poly_add2(poly_add2(v[1],v[2]),v[1]))
#myprint(v[1].diffConst(v[2]))
#myprint((v[1]+v[2]).diffConst(v[2]))

print dir()
del v
#get segfault on destruction of r, if still objects exist
print "hi"