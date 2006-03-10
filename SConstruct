#$Id$
opts = Options('custom.py')
import os
env=Environment(options=opts)


#cudd_libs=Split("cudd obj dddmp mtr st util epd")
cudd_libs=Split("obj cudd dddmp mtr st util epd")



HAVE_PYTHON_EXTENSION=0

class PythonConfig(object):
    def __init__(self, version="2.3", prefix="/usr", libdir=None, incdir=None, libname=None):
        self.version=version
        if libdir:
            self.libdir=libdir
        else:
            self.libdir=prefix+"/lib"
        self.prefix=prefix
        if libname:
            self.libname=libname
        else:
            self.libname="python"+self.version
        if incdir:
            self.incdir=incdir
        else:
            self.incdir=self.prefix+"/include/python"+self.version




PYTHONSEARCH=[\
    PythonConfig(version="2.3"),\
    PythonConfig(version="2.4"),\
    PythonConfig(version="2.4",\
                    libdir="/Library/Frameworks/Python.framework/Versions/2.4/lib",\
                    libname="python",\
                    incdir="/Library/Frameworks/Python.framework/Versions/2.4/include/python2.4"),\
                    PythonConfig(version="2.3"),\
                    PythonConfig(version="2.4")]





conf = Configure(env)

env.Append(CPPPATH=["./polybori/include"])
env.Append(CPPPATH=["./Cudd/include"])
env.Append(LIBPATH=["polybori"])
env['ENV']['HOME']=os.environ["HOME"]

#env.Append(CPPDEFINES=["NDEBUG"])
env.Append(LIBS=["m"])
#env.Append(CCFLAGS=Split("-O3 -ftemplate-depth-30 -ansi -Wno-long-double"))
env.Append(CCFLAGS=Split("-O3 -ftemplate-depth-30 -ansi"))




for l in cudd_libs:
    env.Append(LIBPATH=["./Cudd/"+l])
    env.Append(LIBS=[l])
    

#configure python extension
if HAVE_PYTHON_EXTENSION:
    for c in PYTHONSEARCH:
        if conf.CheckCHeader(c.incdir+"/Python.h"):
            PYTHON_CONFIG=c
            break
    if not PYTHON_CONFIG:
        print 'Python header not found'
        Exit(1)
    BOOST_PREFIX="/sw"
    env.Append(CPPPATH=[PYTHON_CONFIG.incdir])
    #if platform =="cygwin":
    #  env.Append(CPPPATH="/usr/local/include/boost-1_32")
    #else:
    env.Append(CPPPATH=BOOST_PREFIX+"/include")
    print PYTHON_CONFIG.incdir
    print env['CPPPATH']
    if not conf.CheckCXXHeader('boost/python.hpp'):
        print 'Boost/python must be installed'
        Exit(1)
    
    if not conf.CheckCXXHeader('boost/shared_ptr.hpp'):
        print 'Boost must be installed'
        Exit(1)
    env.Append(LIBPATH=[BOOST_PREFIX+"/lib"])
    env.Append(CPPPATH=[BOOST_PREFIX+"/include"])
    env.Append(LIBS=["boost_python"])



env = conf.Finish()


pb_src=Split("BoolePolyRing.cc CErrorInfo.cc PBoRiError.cc")
pb_src=["./polybori/src/"+ source for source in pb_src]
l=env.StaticLibrary("polybori/polybori", pb_src)
tests=["errorcodes","testring"]

for t in tests:
    env.Program("testsuite/"+t, ["testsuite/src/" + t +".cc"] +[l])




if HAVE_PYTHON_EXTENSION:

    

    BOOST_VERSION = 'boost.cvs'
    BOOST = '/usr/local/src/' + BOOST_VERSION
    BOOSTLIBPATH = BOOST+'/stage/lib'
    
    env.SharedLibrary ('PolyBoRi', ["PyPolyBoRi/dummy.cc"])