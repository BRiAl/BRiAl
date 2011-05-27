if __name__=='__main__':
    from sys import path as search_path
    from os import path as file_path   
    search_path.append(file_path.join(file_path.dirname(__file__), '..'))


from polybori.PyPolyBoRi import Ring, VariableFactory, MonomialFactory
from polybori.PyPolyBoRi import Variable as VariableType
import polybori

class RingContext(object):
    """
    >>> r = Ring(1000)
    >>> from polybori import Variable
    >>> print Variable
    <class 'polybori.dynamic.PyPolyBoRi.Variable'>
    >>> with RingContext(r) as rc:
    ...     print polybori.Variable(17)
    x(17)
    >>> try:
    ...     print polybori.Variable(17)
    ... except:
    ...     print "caught expected exception"
    caught expected exception
    """
    def __init__(self, ring):
        self.context = (VariableFactory(ring), MonomialFactory(ring))


    def __enter__(self):
        self.old_context = (polybori.Variable, polybori.Monomial)
        (polybori.Variable, polybori.Monomial) = self.context
        return self

    def __exit__(self, type, value, traceback):
        (polybori.Variable, polybori.Monomial) = self.old_context
        return False

if __name__=='__main__':
    import doctest
    doctest.testmod()

