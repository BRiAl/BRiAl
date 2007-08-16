# Emacs edit mode for this file is -*- python -*-
#$Id$
opts = Options('custom.py')
import tarfile
BOOST_WORKS=False
HAVE_DOXYGEN=True
pyroot="pyroot/"
import sys
from os import sep
from glob import glob
USER_CPPPATH=ARGUMENTS.get("CPPPATH","").split(":")
USER_LIBPATH=ARGUMENTS.get("LIBPATH","").split(":")

m4ri=["grayflex.cc", "packedmatrix.cc","watch.cc",
"brilliantrussian.cc", "matrix.cc"]
m4ri=["M4RI/"+m for m in m4ri]
USERLIBS=[]
PYPREFIX="/sw"
SINGULAR_HOME=None
PBP="python"
#TODO: use opts.Add instead of the import of custom.py
#see http://www.scons.org/doc/production/HTML/scons-user/x1445.html
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
    if "LIBS" in dir(custom):
        USERLIBS=custom.LIBS
    if "SINGULAR_HOME" in dir(custom):
        SINGULAR_HOME=custom.SINGULAR_HOME
    if "HAVE_DOXYGEN" in dir(custom):
        HAVE_DOXYGEN=custom.HAVE_DOXYGEN
    
except:
    pass


try:
	import SCons.Tool.applelink as applelink
except:
	pass
import os


#opts.Add("SINGULAR_HOME")
opts.Add('PBP', 'PolyBoRi python', "python")
opts.Add('CXX', 'C++ Compiler', "g++")
pbori_cache_macros=["PBORI_UNIQUE_SLOTS","PBORI_CACHE_SLOTS","PBORI_MAX_MEMORY"]
for m in pbori_cache_macros:
    opts.Add(m, 'PolyBoRi Cache macro value: '+m, None)

#opts.Add('USERLIBS', 'additional libs', [])

tools =  ["default"]

if HAVE_DOXYGEN:
    tools = ["default", "doxygen"]

env=Environment(options=opts,tools = tools, toolpath = '.')

cache_opts_file=open("polybori/include/cacheopts.h","w")
for m in pbori_cache_macros:
    if env.get(m,None):
        cache_opts_file.write("#define "+m+" " +str(env[m])+"\n")
cache_opts_file.close()
#USERLIBS=env["USERLIBS"]
#if applelink in dir():
#    applelink.generate(env)


# todo: More generic?
#machtype =""# os.environ['MACHTYPE']
IS_x64 = (2**32).__class__==int#((machtype == "x86_64") | (machtype == "ia64"))


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
        self.staticlibdir=self.libdir+"/python"+ version+"/config"

PYTHONSEARCH=[\
    PythonConfig(version="2.5", prefix=PYPREFIX),\
    PythonConfig(version="2.4", prefix=PYPREFIX),\
    PythonConfig(version="2.4"),\
    PythonConfig(version="2.3"),]

conf = Configure(env)
env.Append(CPPPATH=USER_CPPPATH)
env.Append(LIBPATH=USER_LIBPATH)
env.Append(CPPPATH=["./polybori/include"])
env.Append(CPPDEFINES=["PACKED","HAVE_M4RI"])
env.Append(LIBPATH=["polybori","groebner"])

##env.Append(RPATH = pyroot+"polybori/")


env['ENV']['HOME']=os.environ["HOME"]
try:
    env['ENV']['LD_LIBRARY_PATH']=os.environ["LD_LIBRARY_PATH"]
except KeyError:
    pass
#if env['PLATFORM']=="darwin":
#        env.Append(LIBPATH="/sw/lib")
#        env.Append(CPPPATH="/sw/include")
#workaround for linux
#env.Append(LIBPATH=".")

env.Append(LIBS=["m"]+USERLIBS)


try:
    env.Append(CCFLAGS=Split(custom.CCFLAGS))
except:
    env.Append(CCFLAGS=Split("-O3 -ansi --pedantic"))

try:
    env.Append(CXXFLAGS=Split(custom.CXXFLAGS))
except:
    env.Append(CXXFLAGS=Split("-ftemplate-depth-100"))
    
try:
    env.Append(LINKFLAGS=Split(custom.LINKFLAGS))
except:
    pass




HAVE_PYTHON_EXTENSION=0
for c in PYTHONSEARCH:
    if conf.CheckCHeader(c.incdir+"/Python.h"):
        PYTHON_CONFIG=c
        print "Python.h found in " + c.incdir
        env.Append(CPPPATH=[c.incdir])
        env.Append(LIBPATH=[c.staticlibdir])
        #pop it?
        break


if BOOST_WORKS or conf.CheckCXXHeader('boost/python.hpp'):
        HAVE_PYTHON_EXTENSION=1
else:
    print 'Warning Boost/python must be installed for python support'
    

env = conf.Finish()

# Resoruces for including anything into the PyPolyBoRi shared library
shared_resources = []

######################################################################
# Stuff for building Cudd library
######################################################################

cudd_path = 'Cudd/'
cudd_name = 'pboriCudd'
if IS_x64:
    env.Append(CPPDEFINES=["SIZEOF_VOID_P=8", "SIZEOF_LONG=8"])
env.Append(CPPDEFINES=["HAVE_IEEE_754", "BSD"])

env.Append(LIBPATH=[cudd_path])

cudd_resources = [cudd_path + 'obj/cuddObj.cc'] + glob(cudd_path + 'util/*.c')

env.Append(CPPPATH=[cudd_path + 'obj', cudd_path + 'util'])
for cudddir in Split("cudd dddmp mtr st epd"):
    env.Append(CPPPATH=[cudd_path + cudddir])
    cudd_resources += glob(cudd_path + cudddir + '/' + cudddir + '*.c')

cudd_excludes = [cudd_path + 'util/saveimage.c']
cudd_excludes += glob(cudd_path + 'util/test*.c')
cudd_excludes += glob(cudd_path +'dddmp/*DdNode*.c')

for file in cudd_excludes:
    cudd_resources.remove(file)

cudd_files = env.SharedObject(cudd_resources)

libCudd = env.StaticLibrary(cudd_path + cudd_name, cudd_resources)

Default(libCudd)

shared_resources += cudd_files

print  (libCudd[0]) 
######################################################################
# Stuff for building PolyBoRi's C++ part
######################################################################

pb_src=Split("""BoolePolyRing.cc BoolePolynomial.cc BooleVariable.cc
    CErrorInfo.cc PBoRiError.cc CCuddFirstIter.cc CCuddNavigator.cc
    BooleMonomial.cc BooleSet.cc LexOrder.cc CCuddLastIter.cc 
    CCuddGetNode.cc BooleExponent.cc DegLexOrder.cc DegRevLexAscOrder.cc
    pbori_routines.cc BlockDegLexOrder.cc BlockDegRevLexAscOrder.cc""")
pb_src=["./polybori/src/"+ source for source in pb_src]
libpb=env.StaticLibrary("polybori/polybori", pb_src)
#print "l:", l, dir(l)
#sometimes l seems to be boxed by a list
if isinstance(libpb,list):
    libpb=libpb[0]
Default(libpb)

shared_resources += pb_src

######################################################################
# Stuff for building Groebner library
######################################################################

gb_src=Split("groebner.cc literal_factorization.cc pairs.cc groebner_alg.cc lexbuckets.cc dlex4data.cc dp_asc4data.cc lp4data.cc nf.cc interpolate.cc")
gb_src=["./groebner/src/"+ source for source in gb_src]+m4ri
gb=env.StaticLibrary("groebner/groebner", gb_src+[libpb])
#print "gb:", gb, dir(gb)
#sometimes l seems to be boxed by a list
if isinstance(gb,list):
    gb=gb[0]
Default(gb)

shared_resources += gb_src


tests_pb=["errorcodes","testring", "boolevars", "boolepoly", "cuddinterface", 
  "leadterm", "spoly", "zddnavi", "idxtypes", "monomial", "stringlit",
  "booleset", "blocknavi", "termaccu" ]
tests_gb=["strategy_initialization"]
CPPPATH=env['CPPPATH']+['./groebner/src']
print env['CCFLAGS']
print env['CXXFLAGS']
for t in tests_pb:
    env.Program("testsuite/"+t, 
        ["testsuite/src/" + t +".cc"] + [libpb] + libCudd, 
        CPPPATH=CPPPATH)

for t in tests_gb:
    env.Program("testsuite/"+t, 
        ["testsuite/src/" + t +".cc"] + [libpb, gb]+ libCudd, 
        CPPPATH=CPPPATH)


LIBS = env['LIBS']+['boost_python']+USERLIBS

LIBS_static = ["polybori", 'groebner', cudd_name] + LIBS
#env["CPPDEFINES"].Append("Packed")

testsuite_py="testsuite/py/"
installable_python_modules_tp=[(testsuite_py+f,f) for f in Split("ll.py check_claims.py nf.py gbrefs.py statistics.py randompoly.py blocks.py specialsets.py aes.py coding.py")]
installable_python_modules_pypb=[("PyPolyBoRi/PyPolyBoRi.so", "PyPolyBoRi.so")]

installable_python_modules = installable_python_modules_tp + installable_python_modules_pypb


def add_cnf_dir(env,directory):
  for f in glob(directory+"/*.cnf"):
      env.CNF(f[:-4])
if HAVE_PYTHON_EXTENSION:
 
    wrapper_files=["PyPolyBoRi/" + f  for f in ["test_util.cc","main_wrapper.cc", "dd_wrapper.cc", "Poly_wrapper.cc", "navigator_wrap.cc", "variable_block.cc","monomial_wrapper.cc", "strategy_wrapper.cc", "set_wrapper.cc", "slimgb_wrapper.cc"]]
    if env['PLATFORM']=="darwin":
        pypb=env.LoadableModule('PyPolyBori/PyPolyBoRi',
            wrapper_files + shared_resources,
            LINKFLAGS="-bundle_loader " + c.prefix+"/bin/python",
            LIBS=LIBS,LDMODULESUFFIX=".so",
            CPPPATH=CPPPATH)
    else:
        #print "l:", l
        pypb=env.SharedLibrary('PyPolyBoRi/PyPolyBoRi',
            wrapper_files + shared_resources,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", LIBS=LIBS,
            CPPPATH=CPPPATH)
            #LIBS=env['LIBS']+['boost_python',l])#,LDMODULESUFFIX=".so",\
            #SHLIBPREFIX="")
    Default(pypb)
    polybori_modules=pyroot+"polybori/"

    #Default(env.Install(polybori_modules, pypb))
    for (f,n) in installable_python_modules:
        Default(env.Install(polybori_modules, f))
    
    to_append_for_profile=[]
    #to_append_for_profile=File('/lib/libutil.a')
    env.Program('PyPolyBoRi/profiled', wrapper_files+to_append_for_profile,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", 
            LIBS = LIBS_static + ["python"+c.version] + USERLIBS,
            CPPPATH=CPPPATH, CPPDEFINES=["PB_STATIC_PROFILING_VERSION"])
    sys.path.append("testsuite/py")
    from StringIO import StringIO
    from cnf2ideal import gen_clauses, process_input,convert_file_PB
    def cnf2py_build_function(target,source,env):
        target=target[0]
        source=source[0]
        inp=process_input(open(source.path))
        
        clauses=gen_clauses(inp)
        out=open(target.path,"w")
        convert_file_PB(clauses,source.name,False, out)
        return None
    bld = Builder(action = cnf2py_build_function,
                     suffix = '.py',
                     src_suffix = '.cnf')
    env.Append(BUILDERS={'CNF' : bld})
    for i in xrange(1,1001):
        env.CNF("testsuite/py/data/uf20/uf20_"+str(i))
    for i in xrange(1,101):
        env.CNF("testsuite/py/data/flat30_60/flat30_"+str(i))
    for i in xrange(1,101):
        env.CNF("testsuite/py/data/uuf50/uuf50_"+str(i))
    for i in xrange(1,11):
        env.CNF("testsuite/py/data/uuf75/uuf75_"+str(i))
    env.CNF("testsuite/py/data/uuf100/uuf100_01")
    env.CNF("testsuite/py/data/uuf125/uuf125_1")
    for i in xrange(6,11):
        env.CNF("testsuite/py/data/phole/hole"+str(i))
    for i in xrange(4,6):
        env.CNF("testsuite/py/data/hanoi/hanoi"+str(i))

    for f in glob("testsuite/py/data/blocksworld/*.cnf"):
        env.CNF(f[:-4])
        
    for f in glob("testsuite/py/data/qg/*.cnf"):
        env.CNF(f[:-4])
    for f in glob("testsuite/py/data/gcp_large/*.cnf"):
        env.CNF(f[:-4])

    add_cnf_dir(env,"testsuite/py/data/gcp_large")
    add_cnf_dir(env,"testsuite/py/data/bejing")

    def pypb_emitter(target,source,env):
        env.Depends(target,pypb)
        return (target, source)

    bld = Builder(action = "$PBP doc/python/genpythondoc.py " + pyroot,
                  emitter = pypb_emitter)

    # Add the new Builder to the list of builders
    env['BUILDERS']['PYTHONDOC'] = bld

    # Generate foo.vds from foo.txt using mk_vds
    #for f in Split("ll.py nf.py gbrefs.py blocks.py PyPolyBoRi.so specialsets.py"):
        
    env.PYTHONDOC(target="doc/python/polybori.html",source=[polybori_modules+f for (a,f) in installable_python_modules])
    #bld=Builder("cd")
else:
    print "no python extension"
    
HAVE_SINGULAR_EXTENSION=True


if HAVE_DOXYGEN:
    env.Doxygen (source=["doc/doxygen.conf"])
#    env.Doxygen (source=["groebner/doc/doxygen.conf"])
#    dy = env.Doxygen (target="docs/gb/index.html", source=["groebner/doc/doxygen.conf"])

import subprocess
#import re
if SINGULAR_HOME:
  HAVE_SINGULAR_EXTENSION=True
else:
  HAVE_SINGULAR_EXTENSION=False
if HAVE_SINGULAR_EXTENSION:
    SING_ARCH= subprocess.Popen(["sh", SINGULAR_HOME+"/singuname.sh"], stdout=subprocess.PIPE).communicate()[0]
    SING_ARCH=SING_ARCH.replace("\n","")
    SING_INCLUDES=[SINGULAR_HOME+"/"+SING_ARCH+"/include",SINGULAR_HOME+"/kernel",SINGULAR_HOME+"/Singular"]
    
    wrapper_files=["Singular/" + f  for f in ["pb.cc"]]
    if env['PLATFORM']=="darwin":
        pypb=env.LoadableModule('Singular/polybori_module', wrapper_files,
            LINKFLAGS="-bundle_loader " + SINGULAR_HOME+"Singular/Singular",
            LIBS=LIBS,LDMODULESUFFIX=".so",
            CPPPATH=SING_INCLUDES+CPPPATH)
    else:
        #print "l:", l
        pypb=env.SharedLibrary('Singular/polybori_module', wrapper_files,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", LIBS=LIBS+USERLIBS,
            CPPPATH=SING_INCLUDES+CPPPATH)
            #LIBS=env['LIBS']+['boost_python',l])#,LDMODULESUFFIX=".so",\
            #SHLIBPREFIX="")
    Default(singpb)
    
    

def build_tgz(version="0.01"):
    prefix="PolyBoRi-"+str(version)
    tar=tarfile.open(prefix+".tgz","w:gz")
    def addfiletotar(tar,name,tarname=None):
        if tarname is None:
            tarname=name
        tar.add(name,prefix+"/"+tarname)
    for (f,n) in installable_python_modules_tp:
        addfiletotar(tar,f)
    addfiletotar(tar,"groebner")
    addfiletotar(tar,"Cudd")
    addfiletotar(tar,"polybori")
    addfiletotar(tar,"PyPolyBoRi")
    addfiletotar(tar,"SConstruct")
    addfiletotar(tar,"README")
    addfiletotar(tar,"doc")
    addfiletotar(tar,"pyroot")
    addfiletotar(tar,"M4RI")
    addfiletotar(tar,"doxygen.py")
    addfiletotar(tar,"testsuite/py/check_claims.py")
    addfiletotar(tar,"testsuite/py/rundata.py")
    addfiletotar(tar,"testsuite/py/cnf2ideal.py")
    tar.close()
#build_tgz()

