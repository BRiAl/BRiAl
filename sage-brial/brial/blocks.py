from __future__ import print_function

import sys
if __name__ == '__main__':
    import pathadjuster

from .PyPolyBoRi import Ring, VariableBlock, Polynomial
from .PyPolyBoRi import VariableFactory, MonomialFactory
from itertools import chain, islice
#class BlockEndException(object):
  #pass
  #def __init__(self, arg):
  # self.arg = arg
  # pass


class Block(object):
    """The block class represents a block of variables
        <var_name>(start_index,...,start_index+size-1), it is the preferred
        block type for simple one-dimensional variable sets"""

    def __init__(self, var_name, size, start_index=0, reverse=False):
        indices = range(start_index, start_index + size)
        if reverse:
            indices = reversed(indices)
    #self.index2pos=dict([(v,k) for (k,v) in enumerate(indices)])
        self.names = [var_name + "(" + str(i) + ")" for i in indices]
        self.var_name = var_name
        self.start_index = start_index
        self.reverse = reverse
        self.size = size

    def __iter__(self):
        return iter(self.names)

    def __getitem__(self, i):
        return self.names[i]

    def __len__(self):
        return self.size

    def register(self, start, context):
        #def var_func(i):
        #  return Variable(self.index2pos[i]+start)
        ring_context = context
        while isinstance(ring_context, PrefixedDictProxy):
            ring_context = ring_context.wrapped
        ring = ring_context['r']

        var_func = VariableBlock(self.size, self.start_index, start, self.
            reverse, ring)
        var_func.__name__ = self.var_name
        context[self.var_name] = var_func


class AlternatingBlock(object):
    """The Alternating Block class is used for doing tricky variable
        schemes,where base names vary, e.g.
        a(0),b(0),a(1),b(1),a(2),b(2)"""

    def __init__(self, var_names, size_per_variable, start_index=0,
        reverse=False):
        self.var_names = var_names
        self.size_per_variable = size_per_variable
        self.reverse = reverse
        indices = range(start_index, start_index + size_per_variable)

        if reverse:
            indices = reversed(indices)
        names = []
        for i in indices:
            for n in var_names:
                names.append(n + "(" + str(i) + ")")
        self.indices = indices
        self.index2pos = dict([(v, k) for (k, v) in enumerate(indices)])
        self.names = names

    def __len__(self):
        return self.size_per_variable * len(self.var_names)

    def __iter__(self):
        return iter(self.names)

    def __getitem__(self, i):
        return self.names[i]

    def register(self, start, context):
        def gen_var_func(var_pos):

            class var_factory(object):
                def __init__(self, ring, index2pos, size):
                    self.ring = ring
                    self.index2pos = index2pos
                    self.size = size

                def __call__(self, idx):
                    return self.ring.variable(self.index2pos[idx] * self.size +
                                        var_pos + start)
            ring_context = context
            while isinstance(ring_context, PrefixedDictProxy):
                ring_context = ring_context.wrapped
            ring = ring_context['r']

            return var_factory(ring, self.index2pos, len(self.var_names))

        for (var_pos, n) in enumerate(self.var_names):
            var_func = gen_var_func(var_pos)
            var_func.__name__ = n
            context[n] = var_func


def shift(f, i):
    def g(j):
        return f(i + j)
    g.__name__ = f.__name__
    return g


class AdderBlock(AlternatingBlock):
    def __init__(self, adder_bits, sums="s", carries="c", input1="a",
        input2="b", start_index=0):
        AlternatingBlock.__init__(self, (sums, carries, input1, input2),
            adder_bits, start_index=start_index, reverse=True)
        self.input1 = input1
        self.input2 = input2
        self.sums = sums
        self.carries = carries
        self.start_index = start_index
        self.adder_bits = adder_bits

    def register(self, start, context):
        super(AdderBlock, self).register(start, context)
        a = context[self.input1]
        b = context[self.input2]
        self.s = shift(context[self.sums], self.start_index)
        self.c = shift(context[self.carries], self.start_index)
        a = shift(a, self.start_index)
        b = shift(b, self.start_index)
        carries = [Polynomial(a(0).ring().zero())]
        for i in range(self.adder_bits):
            #print i, ":"
            c = 1 + (1 + a(i) * b(i)) * (1 + carries[-1] * a(i)) * (1 +
                carries[-1] * b(i))
            carries.append(c)

        self.add_results = [a(i) + b(i) + carries[i] for i in range(self.
            adder_bits)]
        self.carries_polys = carries[1:]

    #def s(i):
    #  return self.add_results[i-self.start_index]
    #def c(i):
    #  return self.carries_polys[i-self.start_index]
    #context[self.sums]=s
    #context[self.carries]=c
    def implement(self, equations):
        for i in range(self.adder_bits):
            equations.append(self.s(i) + self.add_results[i])
            equations.append(self.c(i) + self.carries_polys[i])
        pass


class HigherOrderBlock(object):
    """HigherOrderBlocks are multidimensional blocks of variables, for each dimension a seperate start_index and size can be specified

    var_name : variables will be called <var_name>(multiindex), where multiindex is a tuple of the size <size_tuple>
    size_tuple : specifies the sizes of the ranges of each component of the multi-indices
    start_index_tuple : the multi-indices will be of the form start_index_tuple + a, where a is a multi-index with non-negative components

    """

    def __init__(self, var_name, size_tuple, start_index_tuple=None,
        reverse=False):
        if start_index_tuple is None:
            start_index_tuple = len(size_tuple) * (0, )
        cart = [()]
        assert len(size_tuple) == len(start_index_tuple)
        outer_indices = reversed(range(len(size_tuple)))
        for i in outer_indices:
            s_i = start_index_tuple[i]
            s = size_tuple[i]
      #print "cart", cart
            cart = [(j, ) + c for j in range(s_i, s_i + s) for c in cart]
        if reverse:
            cart.reverse()
        self.cart = cart
        self.cart2index = dict([(v, k) for (k, v) in enumerate(cart)])
        self.var_name = var_name
        self.names = [var_name + str(c) for c in cart]
        pass

    def __getitem__(self, i):
        return self.names[i]

    def __iter__(self):
        return iter(self.names)

    def __len__(self):
        return len(self.names)

    def register(self, start, context):
        def var_func(*indices):
            return Variable(self.cart2index[indices] + start)
        var_func.__name__ = self.var_name
        context[self.var_name] = var_func


class InOutBlock(object):
    def __init__(self, out_size, in_size, output="out", input="in",
               in_start_index=0, out_start_index=0,
               out_reverse=False, in_reverse=False):
        self.output = Block(var_name=output, start_index=out_start_index,
                        size=out_size, reverse=out_reverse)
        self.input = Block(var_name=input, start_index=in_start_index,
                       size=in_size, reverse=in_reverse)
        self.out_start_index = out_start_index

        self.in_start_index = in_start_index

    def __iter__(self):
        return chain(self.output, self.input)

    def __getitem__(self, i):
        if (i < len(self.output)):
            return self.output[i]
        else:
            return self.input[i - len(self.output)]

    def __len__(self):
        return len(self.output) + len(self.input)

    def register(self, start, context):
        self.output.register(start, context)
        self.input.register(start + len(self.output), context)
        self.out_vars = shift(context[self.output.var_name], self.
            out_start_index)
        self.in_vars = shift(context[self.input.var_name], self.in_start_index)
        pass


class MultiBlock(object):
    def __init__(self, sizes=[], var_names=["v"], start_indices=[], reverses=[
        ]):

        self.start_indices = start_indices + [0] * (len(var_names) - len(
            start_indices))
        reverses += [False] * (len(var_names) - len(reverses))
        sizes += [1] * (len(var_names) - len(sizes))

        self.blocks = [Block(var_name=var_names[idx], size=sizes[idx],
            start_index=self.start_indices[idx], reverse=reverses[idx]) for
            idx in range(len(var_names))]

    def __iter__(self):
        return chain(*self.blocks)

    def __getitem__(self, i):
        return next(islice(chain(*self.blocks), i, i + 1))
        # sum([bl.names for bl in self.blocks])[i]

    def __len__(self):
        return sum((len(bl) for bl in self.blocks))

    def register(self, start, context):
        offset = 0
        for bl in self.blocks:
            bl.register(start + offset, context)
            offset += len(bl)

        self.vars = [shift(context[self.blocks[idx].var_name], self.
            start_indices[idx]) for idx in range(len(self.blocks))]


class PrefixedDictProxy(object):
    """docstring for PrefixedDictProxy"""

    def __init__(self, wrapped, prefix):
        super(PrefixedDictProxy, self).__init__()
        self.wrapped = wrapped
        self.prefix = prefix

    def __getitem__(self, k):
        try:
            return self.wrapped[self.prefix + k]
        except KeyError:
            print(self.prefix, k, list(self.wrapped))
            raise KeyError

    def __setitem__(self, k, v):
        self.wrapped[self.prefix + k] = v


class MacroBlock(object):
    def __init__(self, prefix):

        self.prefix = prefix
        self.blocks = []
        self.combinations = []
        self.connections = []

    def declare(self, blocks):
        self.blocks = blocks

    def connect(self, combinations):
        self.combinations = combinations

    def __iter__(self):
        return (self.prefix + "_" + n for n in chain(*self.blocks))

    def __getitem__(self, i):
        return self.prefix + "_" + next(islice(chain(*self.blocks), i, i + 1))
    #for bl in self.blocks:
    #  if i >= len(bl):
    #    i -= len(bl)
    #  else:
    #    return bl[i]

    def __len__(self):
        return sum((len(bl) for bl in self.blocks))

    def resolve(self, localname):
        return self.prefix + "_" + localname

    def register(self, start, context):
        context = PrefixedDictProxy(context, self.prefix + "_")
        offset = 0
        for bl in self.blocks:
            bl.register(start + offset, context)
            offset += len(bl)

        for ((con1, indices1), (con2, indices2)) in self.combinations:
            for idx in range(min(len(indices1), len(indices2))):
                self.connections += [context[con1](indices1[idx]) + context[
                    con2](indices2[idx])]

    def implement(self, equations):
        for bl in self.blocks:
            if hasattr(bl, "implement"):
                bl.implement(equations)

        equations += self.connections


class IfThen(object):
    def __init__(self, ifpart, thenpart, supposed_to_be_valid=True):
        self.ifpart = [Polynomial(p) for p in ifpart]
        self.thenpart = [Polynomial(p) for p in thenpart]
        self.supposedToBeValid = supposed_to_be_valid

    def __str__(self):
        return ("If(AND(" + ", ".join([str(p) + " == 0" for p in self.ifpart])
            + ")), THEN " + ", ".join([str(p) + " == 0" for p in self.thenpart
            ]))


def if_then(i, t, supposed_to_be_valid=True):
    return IfThen(i, t, supposed_to_be_valid)


def declare_ring(blocks, context=None):
    """Declare Ring is the preferred function to create a ring and declare a variable scheme,
  the number of variables is automatically determined,
  usually you pass globals() as context argument to store the ring and the variable mapping.
  Example
  declare_ring([Block("x",10),Block("y",5)],globals())
  gives  a ring with x(0..9),y(0..4) and registers the ring as r,
  and the variable blocks x and y in the context dictionary globals(), which consists of the global variables of the python module
  """
    if context is None:
        context = sys.modules['__main__'].__dict__

    def canonicalize(blocks):
        for elt in blocks:
            if isinstance(elt, str):
                yield elt
            else:
                for subelt in elt:
                    yield subelt

    blocks = list(blocks)
    n = 0

    for b in blocks:
        if isinstance(b, str):
            n = n + 1
        else:
            n = n + len(b)

    r = Ring(n, names=canonicalize(blocks))

    context["internalVariable"] = VariableFactory(r)
#  context["Monomial"] = MonomialFactory(r)
    context["r"] = r
    declare_block_scheme(blocks, context)
    return r


def declare_block_scheme(blocks, context):
    start = 0
    block_starts = []
    ring = context["r"]
    for b in blocks:
        if start != 0:
            block_starts.append(start)
        if isinstance(b, str):
            context[b] = context["internalVariable"](start)
        #_cpp_set_variable_name(ring, start, b)
            start = start + 1
        else:
            b.register(start, context)
        #for (pos,name) in enumerate(b):
        #    _cpp_set_variable_name(ring, start+pos, name)
            start = start + len(b)
    context["block_start_hints"] = block_starts
    context["number_of_declared_vars"] = start


def main():
    r = Ring(1000)
    ablock = AlternatingBlock(["a", "b", "c"], 100)
    declare_block_scheme([ablock], globals())
    for i in range(10):
        print(r.variable(i))

    print(list(ablock))
    declare_block_scheme([
    Block(var_name="x", size=100),
    HigherOrderBlock("y", (3, 4, 11, 2)),
    AlternatingBlock(["a", "b", "c"], 100)],
    globals())
    for i in range(10):
        print(x(i))
    print(y(0, 0, 0, 0))
    print(y(0, 0, 0, 1))
    print(y(0, 0, 1, 0))
    print(y(0, 0, 1, 1))
    print(a(0), a(1), a(2), b(0), b(1), c(0))
    declare_block_scheme([
    Block(var_name="x", size=100, reverse=True),
    HigherOrderBlock("y", (3, 4, 11, 2), reverse=True),
    AlternatingBlock(["a", "b", "c"], 100, reverse=True)],
    globals())
    for i in range(10):
        print(x(i))
    print(y(0, 0, 0, 0))
    print(y(0, 0, 0, 1))
    print(y(0, 0, 1, 0))
    print(y(0, 0, 1, 1))
    print(a(0), a(1), a(2), b(0), b(1), c(0))
    declare_block_scheme(["a", "b", "c"], globals())
    print(a, b, c)
if __name__ == '__main__':
    main()
