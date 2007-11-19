# Emacs edit mode for this file is -*- python -*-
#$Id$
opts = Options('custom.py')
import tarfile
BOOST_WORKS=False
HAVE_DOXYGEN=True
HAVE_PYTHON_EXTENSION = True

import sys
from os import sep, path
from glob import glob
USER_CPPPATH=ARGUMENTS.get("CPPPATH","").split(":")
USER_LIBPATH=ARGUMENTS.get("LIBPATH","").split(":")

m4ri=["grayflex.cc", "packedmatrix.cc","watch.cc",
"brilliantrussian.cc", "matrix.cc"]
m4ri=[path.join("M4RI", m) for m in m4ri]
USERLIBS=[]
PYPREFIX="/sw"
SINGULAR_HOME=None
PBP="python"

# Fix some paths and names
class PathJoiner(object):
    """Generates a valid path from lists of strings, with custom prefix (set at
    initialization). It also changes '/' to correct path separator.""" 
    def __init__(self, *parent):
        self.parent = path.join(*self.validpath(*parent))
    def __call__(self, *args):
        return path.join(self.parent, *self.validpath(*args))
    def validpath(self, *args):
        return [str(elt).replace('/', sep) for elt in args]

[TestsPath, PyPBPath, CuddPath, GBPath, PBPath, DocPath] = [ PathJoiner(fdir)
    for fdir in Split("""testsuite PyPolyBoRi Cudd groebner polybori doc""") ]

DataPath = PathJoiner(TestsPath('py/data'))

pyroot="pyroot/"
ipbroot = 'ipbori'
cudd_name = 'pboriCudd'

[PyRootPath, IPBPath] = [PathJoiner(fdir) for fdir in [pyroot, ipbroot] ]

 
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
    if "HAVE_PYTHON_EXTENSION" in dir(custom):
        HAVE_PYTHON_EXTENSION = custom.HAVE_PYTHON_EXTENSION
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

opts.Add('PREFIX', 'binary installation prefix directory', '/usr/local')
opts.Add('INSTALLDIR', 'end user installation directory',
         '$PREFIX/share/polybori')
opts.Add('DEVEL_PREFIX', 'development version installation directory','$PREFIX')


pbori_cache_macros=["PBORI_UNIQUE_SLOTS","PBORI_CACHE_SLOTS","PBORI_MAX_MEMORY"]
for m in pbori_cache_macros:
    opts.Add(m, 'PolyBoRi Cache macro value: '+m, None)

#opts.Add('USERLIBS', 'additional libs', [])

tools =  ["default", "disttar"]

if HAVE_DOXYGEN:
    tools += ["doxygen"]

env=Environment(options=opts,tools = tools, toolpath = '.')
#print env.Dump()
cache_opts_file=open(PBPath('include', 'cacheopts.h'), "w")
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



class PythonConfig(object):
    def __init__(self, version="2.4", prefix="/usr", libdir=None, incdir=None, libname=None):
        self.version=version
        if libdir:
            self.libdir=libdir
        else:
            self.libdir = path.join(prefix, 'lib')
        self.prefix=prefix
        if libname:
            self.libname=libname
        else:
            self.libname="python"+self.version
        if incdir:
            self.incdir=incdir
        else:
            self.incdir = path.join(self.prefix,'include','python'+self.version)
        self.staticlibdir = path.join(self.libdir, 'python' + version, 'config')

PYTHONSEARCH=[\
    PythonConfig(version="2.5", prefix=PYPREFIX),\
    PythonConfig(version="2.4", prefix=PYPREFIX),\
    PythonConfig(version="2.5"),
    PythonConfig(version="2.4"),\
    PythonConfig(version="2.3"),]

conf = Configure(env)
env.Append(CPPPATH=USER_CPPPATH)
env.Append(LIBPATH=USER_LIBPATH)
env.Append(CPPPATH=[PBPath('include')])
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
    env.Append(CCFLAGS=Split("-O3 -ansi"))

try:
    env.Append(CXXFLAGS=Split(custom.CXXFLAGS))
except:
    env.Append(CXXFLAGS=Split("-ftemplate-depth-100"))
    
try:
    env.Append(LINKFLAGS=Split(custom.LINKFLAGS))
except:
    pass




for c in PYTHONSEARCH:
    if conf.CheckCHeader(path.join(c.incdir, "Python.h")):
        PYTHON_CONFIG=c
        print "Python.h found in " + c.incdir
        env.Append(CPPPATH=[c.incdir])
        env.Append(LIBPATH=[c.staticlibdir])
        #pop it?
        break

if HAVE_PYTHON_EXTENSION:
    if not (BOOST_WORKS or
            conf.CheckCXXHeader(path.join('boost', 'python.hpp')) ):
        HAVE_PYTHON_EXTENSION = False
        print 'Warning Boost/python must be installed for python support'
    
env = conf.Finish()

# Resoruces for including anything into the PyPolyBoRi shared library
shared_resources = []

######################################################################
# Stuff for building Cudd library
######################################################################

if IS_x64:
    env.Append(CPPDEFINES=["SIZEOF_VOID_P=8", "SIZEOF_LONG=8"])
env.Append(CPPDEFINES=["HAVE_IEEE_754"])
if not env['PLATFORM']=="darwin":
    env.Append(CPPDEFINES=["BSD"])

env.Append(LIBPATH=[CuddPath()])

cudd_resources = [CuddPath('obj/cuddObj.cc')]
cudd_resources += glob(CuddPath('util/*.c'))

cudd_headers = [ CuddPath(fname) for fname in ['obj/cuddObj.hh', 'util/util.h',
                                               'cudd/cuddInt.h'] ] 

env.Append(CPPPATH = [ CuddPath(fdir) for fdir in ['obj', 'util'] ])

for fdir in Split("cudd dddmp mtr st epd"):
    env.Append( CPPPATH=[CuddPath(fdir)] )
    cudd_resources += glob(CuddPath(fdir, fdir + '*.c'))
    cudd_headers += [ CuddPath(fdir, fdir +'.h') ]


# exclude the following files
for fname in ['util/saveimage.c', 'util/test*.c','dddmp/*DdNode*.c']:
    for file in glob(CuddPath(fname)):
        cudd_resources.remove(file)

cudd_shared = env.SharedObject(cudd_resources)

libCudd = env.StaticLibrary(CuddPath(cudd_name), cudd_resources)
Default(libCudd)

shared_resources += cudd_shared

slib=env.SharedLibrary
if env['PLATFORM']=="darwin":
    slib=env.LoadableModule

libCuddShared = slib(CuddPath(cudd_name), list(shared_resources))
#Default(libCuddShared)

######################################################################
# Stuff for building PolyBoRi's C++ part
######################################################################

pb_src=Split("""BoolePolyRing.cc BoolePolynomial.cc BooleVariable.cc
    CErrorInfo.cc PBoRiError.cc CCuddFirstIter.cc CCuddNavigator.cc
    BooleMonomial.cc BooleSet.cc LexOrder.cc CCuddLastIter.cc 
    CCuddGetNode.cc BooleExponent.cc DegLexOrder.cc DegRevLexAscOrder.cc
    pbori_routines.cc BlockDegLexOrder.cc BlockDegRevLexAscOrder.cc""")
pb_src=[PBPath('src', source) for source in pb_src]
libpb=env.StaticLibrary(PBPath('polybori'), pb_src)
#print "l:", l, dir(l)
#sometimes l seems to be boxed by a list
if isinstance(libpb,list):
    libpb=libpb[0]
Default(libpb)

pb_shared = env.SharedObject(pb_src)
shared_resources += pb_shared

libpbShared = slib(PBPath('polybori'), list(shared_resources))
#Default(libpbShared)


######################################################################
# Stuff for building Groebner library
######################################################################

gb_src=Split("groebner.cc literal_factorization.cc pairs.cc groebner_alg.cc lexbuckets.cc dlex4data.cc dp_asc4data.cc lp4data.cc nf.cc interpolate.cc")
gb_src=[GBPath('src', source) for source in gb_src]+m4ri
gb=env.StaticLibrary(GBPath('groebner'), gb_src+[libpb])

#print "gb:", gb, dir(gb)
#sometimes l seems to be boxed by a list
if isinstance(gb,list):
    gb=gb[0]
Default(gb)

gb_shared = env.SharedObject(gb_src)
shared_resources += gb_shared

libgbShared = slib(GBPath('groebner'), list(shared_resources))
#Default(libgbShared)


tests_pb=["errorcodes","testring", "boolevars", "boolepoly", "cuddinterface", 
  "leadterm", "spoly", "zddnavi", "idxtypes", "monomial", "stringlit",
  "booleset", "blocknavi", "termaccu" ]
tests_gb=["strategy_initialization"]
CPPPATH=env['CPPPATH']+[GBPath('src')]
#print env['CCFLAGS']
#print env['CXXFLAGS']
for t in tests_pb:
    env.Program(TestsPath(t), 
        [TestsPath('src', t + ".cc"),  libpb] + libCudd, 
        CPPPATH=CPPPATH)

for t in tests_gb:
    env.Program(TestsPath(t), 
        [TestsPath('src', t + ".cc"), libpb, gb]+ libCudd, 
        CPPPATH=CPPPATH)


LIBS = env['LIBS']+['boost_python']+USERLIBS

LIBS_static = ["polybori", 'groebner', cudd_name] + LIBS
#env["CPPDEFINES"].Append("Packed")


documentable_python_modules = [PyRootPath('polybori', f)
                               for f in Split("""ll.py check_claims.py nf.py
                               gbrefs.py statistics.py randompoly.py blocks.py 
                               specialsets.py aes.py coding.py memusage.py
                               heuristics.py gbcore.py PyPolyBoRi.py""")]   

# Currently all python modules are at place
installable_python_modules = []


def add_cnf_dir(env,directory):
  for f in glob(path.join(directory, "*.cnf")):
      env.CNF(f[:-4])

pydocu = []
dynamic_modules = []
if HAVE_PYTHON_EXTENSION:
    wrapper_files=[ PyPBPath(f) for f in Split("""test_util.cc main_wrapper.cc
    dd_wrapper.cc Poly_wrapper.cc navigator_wrap.cc variable_block.cc
    monomial_wrapper.cc misc_wrapper.cc strategy_wrapper.cc set_wrapper.cc
    slimgb_wrapper.cc""") ] 
    
    if env['PLATFORM']=="darwin":
        pypb=env.LoadableModule(PyPBPath('PyPolyBoRi'),
            wrapper_files + shared_resources,
            LINKFLAGS="-bundle_loader " + c.prefix+"/bin/python",
            LIBS=LIBS,LDMODULESUFFIX=".so",
            CPPPATH=CPPPATH,CCFLAGS=env["CCFLAGS"]+["-fvisibility=hidden"],CXXFLAGS=env["CXXFLAGS"]+["-fvisibility=hidden"])
    else:
        #print "l:", l
        pypb=env.SharedLibrary(PyPBPath('PyPolyBoRi'),
            wrapper_files + shared_resources,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", LIBS=LIBS,
            CPPPATH=CPPPATH)
            #LIBS=env['LIBS']+['boost_python',l])#,LDMODULESUFFIX=".so",\
            #SHLIBPREFIX="")
    Default(pypb)

    # Define the dynamic python modules in pyroot
    dynamic_modules = env.Install(PyRootPath('polybori/dynamic'), pypb)
    documentable_python_modules += dynamic_modules
    
    Default(dynamic_modules)

    
    polybori_modules = PyRootPath("polybori")
    #Default(env.Install(polybori_modules, pypb))
    for (f,n) in installable_python_modules:
        Default(env.Install(polybori_modules, f))


    
    to_append_for_profile = [libpb, gb] + libCudd
    #to_append_for_profile=File('/lib/libutil.a')
    env.Program(PyPBPath('profiled'), wrapper_files+to_append_for_profile,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", 
            LIBS = LIBS + ["python"+c.version] + USERLIBS,
            CPPPATH=CPPPATH, CPPDEFINES=env["CPPDEFINES"]+["PB_STATIC_PROFILING_VERSION"])
    sys.path.append(TestsPath("py"))
    from StringIO import StringIO


    # Converting cnf files to PolyBoRi python format
    from cnf2ideal import gen_clauses, process_input,convert_file_PB
    def cnf2py_build_function(target,source,env):
        target=target[0]
        source=source[0]
        inp=process_input(open(source.path))
        
        clauses=gen_clauses(inp)
        out=open(target.path,"w")
        convert_file_PB(clauses,source.name,False, out)
        return None
    
    cnfbld = Builder(action = cnf2py_build_function,
                     suffix = '.py',
                     src_suffix = '.cnf')
    env.Append(BUILDERS={'CNF' : cnfbld})

    cnffiles =  ["uf20/uf20_" + str(i)         for i in xrange(1,1001)]
    cnffiles += ["flat30_60/flat30_" + str(i)  for i in xrange(1,101)]
    cnffiles += ["uuf50/uuf50_" + str(i)       for i in xrange(1,101)]
    cnffiles += ["uuf75/uuf75_" + str(i)       for i in xrange(1,11)]
    cnffiles += ["phole/hole" + str(i)         for i in xrange(6,13)]
    cnffiles += ["hanoi/hanoi" + str(i)        for i in xrange(4,6)]
    cnffiles += ['uuf100/uuf100_01', 'uuf125/uuf125_1']

    for fname in cnffiles:
        env.CNF(glob(DataPath(fname + ".cnf")))

    for fdir in Split("blocksworld qg gcp_large bejing"):
        add_cnf_dir(env, DataPath(fdir))


    # Generating python documentation
    def pypb_emitter(target,source,env):

        TargetPath = PathJoiner(target[0].dir)
        for file in source:
            (fname, fext) = path.splitext(str(file).replace(pyroot,''))
            if fext in ['.so', '.py']:
                fname = TargetPath(fname.replace(sep,'.') + '.html')
                target.append(env.File(fname))

        return (target, source)

    bld = Builder(action = "$PBP doc/python/genpythondoc.py " + pyroot,
                  emitter = pypb_emitter)

    # Add the new Builder to the list of builders
    env['BUILDERS']['PYTHONDOC'] = bld

    # Generate foo.vds from foo.txt using mk_vds
    #for f in Split("ll.py nf.py gbrefs.py blocks.py PyPolyBoRi.so specialsets.py"):
        
    pydocu = env.PYTHONDOC(target = [DocPath('python/polybori.html'),
                                     DocPath('python/polybori.dynamic.html')],
                           source = documentable_python_modules) 
    #bld=Builder("cd")
else:
    print "no python extension"
    
HAVE_SINGULAR_EXTENSION=True


if HAVE_DOXYGEN:
    cxxdocu = env.Doxygen(source=[DocPath('doxygen.conf')])
    env.Clean(cxxdocu, cxxdocu)
    env.AlwaysBuild(cxxdocu)
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
        singpb=env.LoadableModule('Singular/polybori_module', wrapper_files,
            LINKFLAGS="-bundle_loader " + SINGULAR_HOME+"Singular/Singular",
            LIBS=LIBS,LDMODULESUFFIX=".so",
            CPPPATH=SING_INCLUDES+CPPPATH,CCFLAGS=env["CCFLAGS"]+["-fvisibility=hidden"],CXXFLAGS=env["CXXFLAGS"]+["-fvisibility=hidden"])
    else:
        #print "l:", l
        singpb=env.SharedLibrary('Singular/polybori_module', wrapper_files,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", LIBS=LIBS+USERLIBS,
            CPPPATH=SING_INCLUDES+CPPPATH)
            #LIBS=env['LIBS']+['boost_python',l])#,LDMODULESUFFIX=".so",\
            #SHLIBPREFIX="")
    Default(singpb)
    


# Source distribution archive generation
env.Append(DISTTAR_EXCLUDEEXTS = Split(""".o .os .so .a .dll .cache .pyc
           .cvsignore .dblite .log .sconsign .depend .out .graphViz_temp
           .kprof.html"""),
           DISTTAR_EXCLUDEDIRS = Split("CVS .svn .sconf_temp"),
           DISTTAR_EXCLUDEPATTERN = Split(".#* #*# *~ profiled"))

pboriversion = "0.1"
if 'distribute' in COMMAND_LINE_TARGETS:
    
    srcs = Split("SConstruct README LICENSE disttar.py doxygen.py")
    for dirname in Split("""Cudd extra groebner ipbori M4RI polybori 
    PyPolyBoRi pyroot Singular"""):
        srcs.append(env.Dir(dirname))

    # Testsuite is not distributed completely
    srcs += [TestsPath('execsuite')]
    srcs += glob(TestsPath('py/*.py'))

    for exclsrc in Split("""aes_elim.py gbrefs_pair.py red_search.py
    rtpblocks.py rundummy.py specialsets2.py"""):
        srcs.remove(TestsPath('py', exclsrc))
        
    for dirname in Split("src ref"):
        srcs.append(env.Dir(TestsPath(dirname)))

    # doc is not distributed completely
    srcs += [ DocPath(src) for src in ['doxygen.conf', 'tutorial/tutorial.tex',
                                       'python/genpythondoc.py'] ]

    srcdistri = env.DistTar("PolyBoRi-" + pboriversion, srcs) 
    env.Alias('distribute', srcdistri)
    

# Installation for development purposes
if 'devel-install' in COMMAND_LINE_TARGETS:
    DevelInstPath = PathJoiner(env['DEVEL_PREFIX'])
    
    devellibs = [libpb,gb] + libCudd + libpbShared + libgbShared + libCuddShared
    env.Install(DevelInstPath('lib'), devellibs)
    env.Install(DevelInstPath('include/polybori'), glob(PBPath('include/*.h')))
    env.Install(DevelInstPath('include/polybori/groebner'),
                glob(GBPath('src/*.h')))
    env.Install(DevelInstPath('include/cudd'), cudd_headers)
    env.Install(DevelInstPath('include/polybori/M4RI'), glob('M4RI/*.h'))
    env.Alias('devel-install', DevelInstPath())


# Builder for symlinks
def build_symlink(target, source, env):
    source = source[0].path
    target = target[0].abspath
    print "Symlinking from", source, "to", target
    
    try:
        if path.exists(target):
            env.Remove(target)
        os.symlink(source, target)
    except:
        return True
    return None


# Copy glob('*') from one directory to the other
def cp_all(target, source, env):
    source = source[0].path
    target = target[0].path

    if not path.exists(target):
        Execute(Mkdir(target))
    for file in glob(path.join(source, '*')):
        result = str(path.join(target, path.basename(file)))
        Execute([Copy(result, file), Chmod(result, 0644)])

    return None

cp_recbld = Builder(action = cp_all)

symlinkbld = Builder(action = build_symlink)
env.Append(BUILDERS={'SymLink' : symlinkbld, 'CopyAll': cp_recbld})

def FinalizePermissions(targets, perm):
    for file in targets:
        env.AddPostAction(file, Chmod(str(file), perm))
    return targets

def FinalizeExecs(targets):
    return FinalizePermissions(targets, 0755)

def FinalizeNonExecs(targets):
    return FinalizePermissions(targets, 0644)


def GeneratePyc(sources):
    results = []
    for file in sources:
        (pypath, pymod) = str(file.dir).split(pyroot)
        pypath = path.join(pypath, pyroot)
        pymod = pymod.replace(sep, '.')
        (fbase, fext) = path.splitext(file.name)
        if (fext == '.py') :
            pymod += '.' + fbase
            cmdline = """$PBP -c "import sys; sys.path.append('"""
            cmdline +=  pypath + """'); import """ + pymod + """ " """
            results += env.Command(str(file) +'c', file, cmdline)

        env.Depends(results, sources)
    return results
            
# Installation precedure for end users
if 'install' in COMMAND_LINE_TARGETS:
    # Setting umask for newly generated directories
    try:
        umask = os.umask(022)
        print 'setting umask to 022 (was 0%o)' % umask
    except OSError:     # ignore on systems that don't support umask
        pass
    
    InstPath = PathJoiner(env['INSTALLDIR'])

    # Executables and shared libraries to be installed
    pyfiles = []
    for instfile in dynamic_modules :
        pyfiles += FinalizeExecs(env.InstallAs(InstPath(instfile), instfile))

    for instfile in [ IPBPath('ipbori') ]:
        FinalizeExecs(env.InstallAs(InstPath(instfile), instfile))
            
    # Copy c++ documentation
    if HAVE_DOXYGEN:
        cxxdocinst = env.CopyAll(env.Dir(InstPath('doc/c++')),
                                 env.Dir(DocPath('c++/html'))) 
        env.Depends(cxxdocinst, cxxdocu)
        env.Clean(cxxdocinst, cxxdocinst)

    # Copy python documentation
    FinalizeNonExecs(env.Install(InstPath('doc/python'), pydocu))

    # Non-executables to be installed
    for instfile in glob(PyRootPath('polybori/*.py')) + [
        PyRootPath('polybori/dynamic/__init__.py') ] :
        pyfiles += FinalizeNonExecs(env.InstallAs(InstPath(instfile), instfile))

    if HAVE_PYTHON_EXTENSION:
        FinalizeNonExecs(GeneratePyc(pyfiles))
        
    for instfile in [IPBPath('ipythonrc-polybori') ] :
        FinalizeNonExecs(env.InstallAs(InstPath(instfile), instfile))
      
    env.Alias('install', InstPath())

    # Symlink from executable into bin directory
    ipboribin = env.SymLink(path.join(env['PREFIX'], 'bin', 'ipbori'),
                            InstPath(IPBPath('ipbori')))
    env.AlwaysBuild(ipboribin)   
    env.Alias('install', ipboribin)

