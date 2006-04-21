#$Id$
opts = Options('custom.py')
BOOST_WORKS=False


USER_CPPPATH=ARGUMENTS.get("CPPPATH","").split(":")
USER_LIBPATH=ARGUMENTS.get("LIBPATH","").split(":")
PYPREFIX="/sw"

try:
    import custom
    if "LIBPATH" in dir(custom):
        USER_LIBPATH=custom.LIBPATH+USER_LIBPATH
    if "CPPPATH" in dir(custom):
        USER_CPPPATH=custom.CPPPATH+USER_CPPPATH
    if "BOOST_WORKS" in dir(custom):
        BOOST_WORKS=custom.BOOST_WORKS
    if "PYPREFIX" in dir(custom):
        PYPREFIX=custom.PYPREFIX
except:
    pass


try:
	import SCons.Tool.applelink as applelink
except:
	pass
import os
env=Environment(options=opts)
if (env['PLATFORM']=="darwin"):
    applelink.generate(env)


cudd_libs=Split("obj cudd dddmp mtr st util epd")



HAVE_PYTHON_EXTENSION=1

class PythonConfig(object):
    def __init__(self, version="2.4", prefix="/usr", libdir=None, incdir=None, libname=None):
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
    PythonConfig(version="2.4", prefix=PYPREFIX),\
    PythonConfig(version="2.4"),\
    PythonConfig(version="2.3"),]

conf = Configure(env)
env.Append(CPPPATH=USER_CPPPATH)
env.Append(LIBPATH=USER_LIBPATH)
env.Append(CPPPATH=["./polybori/include"])
env.Append(CPPPATH=["./Cudd/include"])
env.Append(LIBPATH=["polybori"])
env['ENV']['HOME']=os.environ["HOME"]
if env['PLATFORM']=="darwin":
        env.Append(LIBPATH="/sw/lib")
        env.Append(CPPPATH="/sw/include")
#workaround for linux
#env.Append(LIBPATH=".")

env.Append(LIBS=["m"])
try:
    env.Append(CCFLAGS=Split(custom.CCFLAGS))
except:
    env.Append(CCFLAGS=Split("-O3 -ftemplate-depth-100 -ansi --pedantic"))

#env.Append(CCFLAGS=Split("-g -ftemplate-depth-100 -ansi"))
for l in cudd_libs:
    env.Append(LIBPATH=["./Cudd/"+l])
    env.Append(LIBS=[l])
    

HAVE_PYTHON_EXTENSION=0
for c in PYTHONSEARCH:
    if conf.CheckCHeader(c.incdir+"/Python.h"):
        PYTHON_CONFIG=c
        print "Python.h found in " + c.incdir
        env.Append(CPPPATH=[c.incdir])
        break


if BOOST_WORKS or conf.CheckCXXHeader('boost/python.hpp'):
        HAVE_PYTHON_EXTENSION=1
else:
    print 'Warning Boost/python must be installed for python support'
    

env = conf.Finish()


pb_src=Split("BoolePolyRing.cc BoolePolynomial.cc BooleVariable.cc \
CErrorInfo.cc PBoRiError.cc CCuddFirstIter.cc CCuddNavigator.cc \
BooleMonomial.cc BooleSet.cc")
pb_src=["./polybori/src/"+ source for source in pb_src]
libpb=env.StaticLibrary("polybori/polybori", pb_src)
#print "l:", l, dir(l)
#sometimes l seems to be boxed by a list
if isinstance(libpb,list):
    libpb=l[0]
Default(libpb)


gb_src=Split("groebner.cc pairs.cc groebner_alg.cc")
gb_src=["./groebner/src/"+ source for source in gb_src]
gb=env.StaticLibrary("groebner", gb_src+[libpb])
print "gb:", gb, dir(gb)
#sometimes l seems to be boxed by a list
if isinstance(gb,list):
    gb=gb[0]
Default(gb)


tests=["errorcodes","testring", "boolevars", "boolepoly", "cuddinterface", 
  "leadterm", "spoly", "zddnavi", "idxtypes", "monomial" ]

for t in tests:
    Default(env.Program("testsuite/"+t, ["testsuite/src/" + t +".cc"] +[libpb]))

LIBS=env['LIBS']+['boost_python',libpb, gb]
CPPPATH=env['CPPPATH']+['./groebner/src']

if HAVE_PYTHON_EXTENSION:
 
    wrapper_files=["PyPolyBoRi/" + f  for f in ["main_wrapper.cc", "dd_wrapper.cc", "Poly_wrapper.cc", "navigator_wrap.cc", "strategy_wrapper.cc"]]
    if env['PLATFORM']=="darwin":
        env.LoadableModule('PyPolyBori/PyPolyBoRi', wrapper_files,
            LINKFLAGS="-bundle_loader /sw/bin/python",
            LIBS=LIBS,LDMODULESUFFIX=".so",
            CPPPATH=CPPPATH)
    else:
        #print "l:", l
        env.SharedLibrary('PyPolyBoRi/PyPolyBoRi', wrapper_files,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", LIBS=LIBS,
            CPPPATH=CPPPATH)
            #LIBS=env['LIBS']+['boost_python',l])#,LDMODULESUFFIX=".so",\
            #SHLIBPREFIX="")
else:
    print "no python extension"
