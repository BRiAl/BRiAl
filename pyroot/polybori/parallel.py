# -*- python -*-
# coding=utf-8

#  parallel.py
#  PolyBoRi
#  
#  Created by Michael Brickenstein on 2008-10-31.
#  Copyright 2008 The PolyBoRi Team
# 

from polybori.PyPolyBoRi import if_then_else, Polynomial, Ring, CCuddNavigator
from polybori.gbcore import groebner_basis
from zlib import compress, decompress
    
def to_fast_pickable(l):
    """
    to_fast_pickable(l) converts a list of polynomials into a builtin Python value, which is fast pickable and compact.
    INPUT:
        - a list of Boolean polynomials
    OUTPUT:
        It is converted to a tuple consisting of
        - codes referring to the polynomials
        - list of conversions of nodes. 
            The nodes are sorted, that
            n occurs before n.else_branch(), n.then_branch()
            Nodes are only listed, if they are not constant.
    
        A node is converted in this way:
            0 -> 0
            1 -> 1
            if_then_else(v,t,e) -> (v, index of then branch +2, index of else branch +2)
            The shift of +2 is for the constant values implicitly contained in the list.
        Each code c refers to the c-2-th position in the conversion list, if c >=2, else to
        the corresponding Boolean constant if c in {0, 1}
    EXAMPLES:
        >>> from polybori.PyPolyBoRi import Ring
        >>> r=Ring(1000)
        >>> x=Variable = VariableFactory(r)
        >>> to_fast_pickable([Polynomial(1, r)])
        [[1], []]
        >>> to_fast_pickable([Polynomial(0, r)])
        [[0], []]
        >>> to_fast_pickable([x(0)])
        [[2], [(0, 1, 0)]]
        >>> to_fast_pickable([x(0)*x(1)+x(1)])
        [[2], [(0, 3, 3), (1, 1, 0)]]
        >>> to_fast_pickable([x(1)])
        [[2], [(1, 1, 0)]]
        >>> to_fast_pickable([x(0)+1])
        [[2], [(0, 1, 1)]]
        >>> to_fast_pickable([x(0)*x(1)])
        [[2], [(0, 3, 0), (1, 1, 0)]]
        >>> to_fast_pickable([x(0)*x(1)+x(1)])
        [[2], [(0, 3, 3), (1, 1, 0)]]
        >>> to_fast_pickable([x(0)*x(1)+x(2)])
        [[2], [(0, 3, 4), (1, 1, 0), (2, 1, 0)]]
        >>> p=x(5)*x(23) + x(5)*x(24)*x(59) + x(5) + x(6)*x(23)*x(89) + x(6)*x(60)*x(89) + x(23) + x(24)*x(89) + x(24) + x(60)*x(89) + x(89) + 1
        >>> from_fast_pickable(to_fast_pickable([p]))==[p]
        True
        >>> to_fast_pickable([x(0)*x(1), Polynomial(0, r), Polynomial(1, r), x(3)])
        [[2, 0, 1, 4], [(0, 3, 0), (1, 1, 0), (3, 1, 0)]]
    """
    if len(l)==0:
        return [[], []]
    
    f=l[0]
    f=f.set()
    r=f.ring()
    one=r.one().navigation()
    zero=r.zero().navigation()
    nodes=set()
    
    def find_navs(nav):
        if not nav in nodes and not nav.constant():
            nodes.add(nav)
            find_navs(nav.then_branch())
            find_navs(nav.else_branch())
    for f in l:
        f_nav=f.set().navigation()
        find_navs(f_nav)

    nodes_sorted=sorted(nodes, key=CCuddNavigator.value)
    nodes2i={one:1,zero:0}
    for (i,n) in enumerate(nodes_sorted):
        nodes2i[n]=i+2

    for i in xrange(len(nodes_sorted)):
        n=nodes_sorted[i]
        t=nodes2i[n.then_branch()]
        e=nodes2i[n.else_branch()]
        nodes_sorted[i]=(n.value(),t,e)

    return [[nodes2i[f.set().navigation()] for f in  l], nodes_sorted]


def from_fast_pickable(l,r):
    """from_fast_pickable(l) undoes the operation to_fast_pickable. The first argument is an object created by to_fast_pickable.
    For the specified format, see the documentation of to_fast_pickable.
    The second argument is ring, in which this polynomial should be created.
    INPUT: 
        see OUTPUT of to_fast_pickable
    OUTPUT:
        a list of Boolean polynomials
    EXAMPLES:
        >>> from polybori.PyPolyBoRi import Ring
        >>> r=Ring(1000)
        >>> x=Variable = VariableFactory(r)
        >>> from_fast_pickable([[1], []])
        [1]
        >>> from_fast_pickable([[0], []])
        [0]
        >>> from_fast_pickable([[2], [(0, 1, 0)]])
        [x(0)]
        >>> from_fast_pickable([[2], [(1, 1, 0)]])
        [x(1)]
        >>> from_fast_pickable([[2], [(0, 1, 1)]])
        [x(0) + 1]
        >>> from_fast_pickable([[2], [(0, 3, 0), (1, 1, 0)]])
        [x(0)*x(1)]
        >>> from_fast_pickable([[2], [(0, 3, 3), (1, 1, 0)]])
        [x(0)*x(1) + x(1)]
        >>> from_fast_pickable([[2], [(0, 3, 4), (1, 1, 0), (2, 1, 0)]])
        [x(0)*x(1) + x(2)]
        >>> from_fast_pickable([[2, 0, 1, 4], [(0, 3, 0), (1, 1, 0), (3, 1, 0)]])
        [x(0)*x(1), 0, 1, x(3)]
    """
    i2poly={0:r.zero(), 1:r.one()}
    (indices, terms)=l

    for i in reversed(xrange(len(terms))):
        (v,t,e)=terms[i]
        t=i2poly[t]
        e=i2poly[e]
        terms[i]=if_then_else(v,t,e)
        i2poly[i+2]=terms[i]
    return [Polynomial(i2poly[i]) for i in indices]

def _calculate_gb_with_keywords(args):
    (I, kwds_as_single_arg) = args
    import traceback
    try:
        return groebner_basis(I, **kwds_as_single_arg)
    except:
        raise ValueError, traceback.format_exc()


def _unpickle_polynomial(self, code):
    self.__init__(from_fast_pickable(code, self.ring())[0])

    
def _pickle_polynomial(poly):
    return to_fast_pickable([poly])

def _initargs_polynomial(self):
    return (self.ring(),)

Polynomial.__safe_for_unpickling__ = True
Polynomial.__getinitargs__ = _initargs_polynomial
Polynomial.__getstate__ = _pickle_polynomial
Polynomial.__setstate__ = _unpickle_polynomial



def _recreate_ring(code):
    import os
    (identifier, data, varnames, blocks) = code

    global _polybori_parallel_rings
    try:
        _polybori_parallel_rings
    except NameError:
        _polybori_parallel_rings = dict()

    if identifier in _polybori_parallel_rings:
        ring = _polybori_parallel_rings[identifier][0]
    else:
        ring = Ring(*data)
        varnames = decompress(varnames).split('\n')
        for (elt, idx) in zip(varnames, xrange(len(varnames))):
            ring.set_variable_name(idx, elt)

        for elt in blocks:
            ring.append_block(elt)

        storage_data = (ring, code)
        _polybori_parallel_rings[identifier] = storage_data
        _polybori_parallel_rings[(ring.id(), os.getpid())] = storage_data

    return ring
    
def _pickle_ring(ring):
    import os
    identifier = (ring.id(), os.getpid())

    global _polybori_parallel_rings
    try:
        _polybori_parallel_rings
    except NameError:
        _polybori_parallel_rings = dict()
        
    if identifier in _polybori_parallel_rings:
        code = _polybori_parallel_rings[identifier][1]
    else:
        nvars = ring.n_variables()
        data = (nvars, ring.get_order_code())
        varnames = '\n'.join([str(ring.variable(idx)) for idx in xrange(nvars)])
        blocks = list(ring.blocks())
        code = (identifier, data, compress(varnames), blocks[:-1])
        _polybori_parallel_rings[identifier] = (ring, code)

    return code

ring_old_init = Ring.__init__

def ring_new_init(self, first, *args):

    try:
        ring_old_init(self, first, *args)
    except:
        ring_old_init(self, first(*args))

Ring.__init__ = ring_new_init

def _initargs_ring(self):
    return (_recreate_ring, _pickle_ring(self))

def _unpickle_ring(self, code):
    pass

Ring.__safe_for_unpickling__ = True
Ring.__getstate__ = _pickle_ring
Ring.__setstate__ = _unpickle_ring
Ring.__getinitargs__ = _initargs_ring


def groebner_basis_first_finished(I, *l):
    """
    INPUT:
        - I ideal
        - l: keyword dictionaries, which will be keyword arguments to groebner_basis.
    OUTPUT:
        - tries to compute groebner_basis(I, **kwd) for kwd in l
        - returns the result of the first terminated computation
    EXAMPLES:    
        >>> from polybori.PyPolyBoRi import Ring
        >>> r=Ring(1000)
        >>> x = VariableFactory(r)
        >>> groebner_basis_first_finished([x(1)*x(2)+x(2)+x(1)],dict(heuristic=True), dict(heuristic=False))
        [x(1), x(2)]
    """
    if not I:
        return []
    try:
        from multiprocessing import Pool
    except:
        from processing import Pool

    pool = Pool(processes=len(l))
    it = pool.imap_unordered(_calculate_gb_with_keywords,
                             [(I, kwds) for kwds in l])
    res=it.next() 

    pool.terminate()

    return res

def _test():
    import doctest
    doctest.testmod()

if __name__ == "__main__":
    _test()
