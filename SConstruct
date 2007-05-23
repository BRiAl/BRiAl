#$Id$
opts = Options('custom.py')
BOOST_WORKS=False
HAVE_DOXYGEN=True

import sys
from glob import glob
USER_CPPPATH=ARGUMENTS.get("CPPPATH","").split(":")
USER_LIBPATH=ARGUMENTS.get("LIBPATH","").split(":")
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
if (env['PLATFORM']=="darwin"):
    applelink.generate(env)


cudd_libs=Split("obj cudd dddmp mtr st cuddutil epd")
#cudd_libs=Split("obj cudd dddmp mtr st util epd")


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
env.Append(CPPPATH=["./Cudd/include","extra"])
env.Append(LIBPATH=["polybori","groebner","extra"])

env['ENV']['HOME']=os.environ["HOME"]
env['ENV']['LD_LIBRARY_PATH']=os.environ["LD_LIBRARY_PATH"]

#if env['PLATFORM']=="darwin":
#        env.Append(LIBPATH="/sw/lib")
#        env.Append(CPPPATH="/sw/include")
#workaround for linux
#env.Append(LIBPATH=".")

env.Append(LIBS=["m","extra"])
try:
    env.Append(CCFLAGS=Split(custom.CCFLAGS))
except:
    env.Append(CCFLAGS=Split("-O3 -ftemplate-depth-100 -ansi --pedantic"))

try:
    env.Append(LINKFLAGS=Split(custom.LINKFLAGS))
except:
    pass

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
        env.Append(LIBPATH=[c.staticlibdir])
        #pop it?
        break


if BOOST_WORKS or conf.CheckCXXHeader('boost/python.hpp'):
        HAVE_PYTHON_EXTENSION=1
else:
    print 'Warning Boost/python must be installed for python support'
    

env = conf.Finish()


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


gb_src=Split("groebner.cc literal_factorization.cc pairs.cc groebner_alg.cc lexbuckets.cc dlex4data.cc dp_asc4data.cc lp4data.cc nf.cc")
gb_src=["./groebner/src/"+ source for source in gb_src]
gb=env.StaticLibrary("groebner/groebner", gb_src+[libpb])
#print "gb:", gb, dir(gb)
#sometimes l seems to be boxed by a list
if isinstance(gb,list):
    gb=gb[0]
Default(gb)


tests_pb=["errorcodes","testring", "boolevars", "boolepoly", "cuddinterface", 
  "leadterm", "spoly", "zddnavi", "idxtypes", "monomial", "stringlit",
  "booleset", "blocknavi", "termaccu" ]
tests_gb=["strategy_initialization"]
CPPPATH=env['CPPPATH']+['./groebner/src']
print env['CCFLAGS']
for t in tests_pb:
    env.Program("testsuite/"+t, 
        ["testsuite/src/" + t +".cc"] +[libpb], 
        CPPPATH=CPPPATH)

for t in tests_gb:
    env.Program("testsuite/"+t, 
        ["testsuite/src/" + t +".cc"] +[libpb, gb], 
        CPPPATH=CPPPATH)

LIBS=env['LIBS']+['boost_python',"polybori", "groebner"]

def add_cnf_dir(env,directory):
  for f in glob(directory+"/*.cnf"):
      env.CNF(f[:-4])
if HAVE_PYTHON_EXTENSION:
 
    wrapper_files=["PyPolyBoRi/" + f  for f in ["test_util.cc","main_wrapper.cc", "dd_wrapper.cc", "Poly_wrapper.cc", "navigator_wrap.cc", "monomial_wrapper.cc", "strategy_wrapper.cc", "set_wrapper.cc", "slimgb_wrapper.cc"]]
    if env['PLATFORM']=="darwin":
        pypb=env.LoadableModule('PyPolyBori/PyPolyBoRi', wrapper_files,
            LINKFLAGS="-bundle_loader " + c.prefix+"/bin/python",
            LIBS=LIBS,LDMODULESUFFIX=".so",
            CPPPATH=CPPPATH)
    else:
        #print "l:", l
        pypb=env.SharedLibrary('PyPolyBoRi/PyPolyBoRi', wrapper_files,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", LIBS=LIBS+USERLIBS,
            CPPPATH=CPPPATH)
            #LIBS=env['LIBS']+['boost_python',l])#,LDMODULESUFFIX=".so",\
            #SHLIBPREFIX="")
    to_append_for_profile=[]
    #to_append_for_profile=File('/lib/libutil.a')
    env.Program('PyPolyBoRi/profiled', wrapper_files+to_append_for_profile,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", 
            LIBS=LIBS+["python"+c.version]+USERLIBS,
            CPPPATH=CPPPATH, CPPDEFINES=["PB_STATIC_PROFILING_VERSION"])
    sys.path.append("testsuite/py")
    from StringIO import StringIO
    from cnf2ideal import gen_clauses, process_input,convert_file_PB
    def cnf2py_build_function(target,source,env):
        #print target, source
        #assert len(source)==1
        #assert len(target)==1
        target=target[0]
        source=source[0]
        #print dir(source)
        #print source.path
        #print dir(source.dir)
        inp=process_input(open(source.path))
        
        clauses=gen_clauses(inp)#StringIO(source.read())))
        
        
        #print target.name, source.name
        out=open(target.path,"w")
        convert_file_PB(clauses,source.name,False, out)
        #out.close()
        
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
     #   for f in glob("testsuite/py/data/gcp_large/*.cnf"):
     #       env.CNF(f[:-4])
    add_cnf_dir(env,"testsuite/py/data/gcp_large")
    add_cnf_dir(env,"testsuite/py/data/bejing")
    #if isinstance(pypb,list):
    #    pypb=pypb[0]
    def pypb_emitter(target,source,env):
        env.Depends(target,pypb)
        return (target, source)

    bld = Builder(action = "$PBP doc/python/genpythondoc.py $SOURCE $TARGET",
                  emitter = pypb_emitter)

    # Add the new Builder to the list of builders
    env['BUILDERS']['PYTHONDOC'] = bld

    # Generate foo.vds from foo.txt using mk_vds
    env.PYTHONDOC(target="doc/python/PyPolyBoRi.html", source='PyPolyBoRi/PyPolyBoRi.so')
    env.PYTHONDOC(target="doc/python/nf.html", source='testsuite/py/nf.py')
    env.PYTHONDOC(target="doc/python/ll.html", source='testsuite/py/ll.py')
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
        env.LoadableModule('Singular/polybori_module', wrapper_files,
            LINKFLAGS="-bundle_loader " + SINGULAR_HOME+"Singular/Singular",
            LIBS=LIBS,LDMODULESUFFIX=".so",
            CPPPATH=SING_INCLUDES+CPPPATH)
    else:
        #print "l:", l
        env.SharedLibrary('Singular/polybori_module', wrapper_files,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", LIBS=LIBS+USERLIBS,
            CPPPATH=SING_INCLUDES+CPPPATH)
            #LIBS=env['LIBS']+['boost_python',l])#,LDMODULESUFFIX=".so",\
            #SHLIBPREFIX="")
  