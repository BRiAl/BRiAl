from polybori.PyPolyBoRi import if_then_else, Polynomial, global_ring
from polybori.gbcore import groebner_basis

def to_fast_pickable(f):
    """
    to_fast_pickable(f) converts a polynomial a builtin Python value, which is fast pickable and compact.
    It is converted to a list of conversions of nodes. The nodes are sorted, that
    n occurs before n.else_branch(), n.then_branch()
    Nodes are only listed, if they are not constant (except for the root node, which might also become constant).
    A node is converted in this way:
        0 -> 0
        1 -> 1
        if_then_else(v,t,e) -> (v, index of then branch +2, index of else branch +2)
        The shift of +2 is for the constant values implicitly contained in the list.
    >>> from polybori.PyPolyBoRi import Ring, Variable
    >>> r=Ring(1000)
    >>> x=Variable
    >>> to_fast_pickable(Polynomial(1))
    [1]
    >>> to_fast_pickable(Polynomial(0))
    [0]
    >>> to_fast_pickable(x(0))
    [(0, 1, 0)]
    >>> to_fast_pickable(x(1))
    [(1, 1, 0)]
    >>> to_fast_pickable(x(0)+1)
    [(0, 1, 1)]
    >>> to_fast_pickable(x(0)*x(1))
    [(0, 3, 0), (1, 1, 0)]
    >>> to_fast_pickable(x(0)*x(1)+x(1))
    [(0, 3, 3), (1, 1, 0)]
    >>> to_fast_pickable(x(0)*x(1)+x(2))
    [(0, 3, 4), (1, 1, 0), (2, 1, 0)]
    """
    f=f.set()
    r=f.ring()
    one=r.one().navigation()
    zero=r.zero().navigation()
    f_nav=f.navigation()
    if f_nav==one:
        return [1]
    if f_nav==zero:
        return [0]
    nodes=set()
    nodes_sorted=list()
    def find_navs(nav):
        if not nav in nodes and not nav.constant():
            nodes.add(nav)
            nodes_sorted.append(nav)
            find_navs(nav.then_branch())
            find_navs(nav.else_branch())
    find_navs(f_nav)
    nodes2i={one:1,zero:0}
    for (i,n) in enumerate(nodes_sorted):
        nodes2i[n]=i+2
    for i in xrange(len(nodes_sorted)):
        n=nodes_sorted[i]
        t=nodes2i[n.then_branch()]
        e=nodes2i[n.else_branch()]
        nodes_sorted[i]=(n.value(),t,e)
    return nodes_sorted

def from_fast_pickable(f,r=None):
    """from_fast_pickable(f) undoes the operation to_fast_pickable. The first argument is an object created by to_fast_pickable.
    For the specified format, see the documentation of to_fast_pickable.
    The second argument is ring, in which this polynomial should be created.
    >>> from polybori.PyPolyBoRi import Ring, Variable
    >>> r=Ring(1000)
    >>> x=Variable
    >>> from_fast_pickable([1])
    1
    >>> from_fast_pickable([0])
    0
    >>> from_fast_pickable([(0, 1, 0)])
    x(0)
    >>> from_fast_pickable([(1, 1, 0)])
    x(1)
    >>> from_fast_pickable([(0, 1, 1)])
    x(0) + 1
    >>> from_fast_pickable([(0, 3, 0), (1, 1, 0)])
    x(0)*x(1)
    >>> from_fast_pickable([(0, 3, 3), (1, 1, 0)])
    x(0)*x(1) + x(1)
    >>> from_fast_pickable([(0, 3, 4), (1, 1, 0), (2, 1, 0)])
    x(0)*x(1) + x(2)
    """
    if r is None:
        r=global_ring()
    i2poly={0:r.zero(),1:r.one()}
    for i in reversed(xrange(1,len(f))):
        (v,t,e)=f[i]
        t=i2poly[t]
        e=i2poly[e]
        f[i]=if_then_else(v,t,e)
        i2poly[i+2]=f[i]
    if isinstance(f[0],int):
        return i2poly[f[0]]
    else:
        (v,t,e)=f[0]
        t=i2poly[t]
        e=i2poly[e]
        return Polynomial(if_then_else(v,t,e))

def f(x):
    (I,kwds_as_single_arg)=x
    I=[from_fast_pickable(p) for p in I]
    res=groebner_basis(I,**kwds_as_single_arg)
    res=[to_fast_pickable(p) for p in res]
    return res
def groebner_basis_first_finished(I, *l):
    """l is a list of keyword dictionaries, which will be keyword arguments to groebner_basis.
    First finished computes in parallel in just returns the result of the first finished process. Arguments are supposed to pickable.

    >>> from polybori.PyPolyBoRi import Ring, Variable
    >>> r=Ring(1000)
    >>> x=Variable
    >>> groebner_basis_first_finished([x(1)*x(2)+x(2)+x(1)],dict(heuristic=True), dict(heuristic=False))
    [x(1), x(2)]
    """
    from processing import Pool


    pool = Pool(processes=len(l))            
    I=[to_fast_pickable(Polynomial(p)) for p in I]
    it = pool.imap_unordered(f, [(I,kwds) for kwds in l])  
   
    res=it.next() 
    pool.terminate()
    return [from_fast_pickable(p) for p in res]
def _test():
    import doctest
    doctest.testmod()

if __name__ == "__main__":
    _test()