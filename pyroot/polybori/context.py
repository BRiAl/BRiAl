from __future__ import with_statement
from polybori.PyPolyBoRi import Ring, global_ring
def current_number_of_variables():
    return global_ring().n_variables()
class RingContext(object):
    def __init__(self, new_ring):
        self.ring=new_ring
    def __enter__(self):
        old_ring=Ring()
        class ContextGuard(object):
            def __exit__(self, type, value, traceback):
                old_ring.set()
                return False
        return ContextGuard()

if __name__=='__main__':
    r=Ring(1000)
    r.set()
    assert current_number_of_variables()==1000, current_number_of_variables()
    r2=Ring(1001)
    assert current_number_of_variables()==1001
    r.set()
    assert current_number_of_variables()==1000
    try:
        with RingContext(r2) as rc:
            assert current_number_of_variables()==1001
            print "succ"
            raise Exception
    except:
        print "cought expected exception"
    assert current_number_of_variables()==1000