from polybori import BooleSet

class PartialFunction(object):
    """docstring for PartialFunction"""
    def __init__(self, zeros,ones):
        super(PartialFunction, self).__init__()
        self.zeros = zeros
        self.ones = ones
    def __str__(self):
        return "PartialFunction(zeros="+str(self.zeros)+", ones="+str(self.ones)+")"
    def definedOn(self):
        return self.zeros.union(self.ones)
    def __add__(self,other):
        domain=self.definedOn().intersect(other.definedOn())
        zeros=self.zeros.intersect(other.zeros).union(self.ones.intersect(other.ones))
        ones=self.zeros.intersect(other.ones).union(self.ones.intersect(other.zeros))
        assert zeros.diff(domain).empty()
        assert ones.diff(domain).empty()
        return PartialFunction(zeros,ones)
    def __repr__(self):
        return str(self)