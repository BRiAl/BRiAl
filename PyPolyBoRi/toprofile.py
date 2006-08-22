import unittest
from sys import path
path.append("../../PyPolyBoRi")
import nf
nf.PROT=False
from nf import *
nf.PROT=False
from time import clock
import sys
sys.argv=["profiled"]
def add_unsat_test_case(test_class, data_file_name):
    def test_method(self):
        t=clock()
        mydata=__import__("data."+data_file_name)
        #myattr=sub(r"data\.","",in_file)
        mydata=getattr(mydata,data_file_name)
        I=buchberger_C_based2(mydata.ideal)
        self.assert_(I.containsOne(),"GB is not One, although ideal whole ring")
        #for free
        del I
        del mydata.ideal
        del mydata.r
        t2=clock()
        print t2-t,"s"
    test_method.__name__="testT"+data_file_name
    setattr(test_class,"testT"+data_file_name, test_method)
def add_sat_test_case(test_class, data_file_name):
    def test_method(self):
        t=clock()
        mydata=__import__("data."+data_file_name)
        #myattr=sub(r"data\.","",in_file)
        mydata=getattr(mydata,data_file_name)
        I=buchberger_C_based2(mydata.ideal)
        self.assert_(not I.containsOne(),"GB is One, although ideal not whole ring")
        #for free

        del I

        del mydata.ideal

        del mydata.r
        t2=clock()
        print t2-t,"s"
        #print dir(mydata)
    test_method.__name__="testT"+data_file_name
    setattr(test_class,"testT"+data_file_name, test_method)
class SatTestCase(unittest.TestCase):
    pass
class UnSatTestCase(unittest.TestCase):
    pass

for i in xrange(1,30):
    add_sat_test_case(SatTestCase,"uf20_04"+str(i))
for i in xrange(1,8):
    add_unsat_test_case(UnSatTestCase,"uuf50_0"+str(i))

suite = unittest.makeSuite(UnSatTestCase,'test')
runner = unittest.TextTestRunner()
runner.run(suite)
unittest.main()
