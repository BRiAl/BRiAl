# Emacs edit mode for this file is -*- python -*-
#$Id$
opts = Options('custom.py')

# Some hard-coded settings
pboriname = 'PolyBoRi'
pboriversion = "0.4"
pborirelease = "0"
libraryversion = "0.0.0"
debname = "polybori-" + pboriversion

import tarfile

import sys
from os import sep, path
from glob import glob

m4ri=["grayflex.c", "packedmatrix.c","watch.c","misc.c",
"brilliantrussian.c"]
m4ri=[path.join("M4RI", m) for m in m4ri]

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

DebPath = PathJoiner('pkgs/debian')
DebInstPath = PathJoiner('debian')

RPMPath = PathJoiner('pkgs/rpm')
SpecsPath = PathJoiner(RPMPath('SPECS'))

   
# Split lists separated by colons and whitespaces
def SplitColonSep(arg):
    result = []
    for element in Split(arg):
        result += element.split(':')
    return result

def shell_output(*args):
    return os.popen(' '.join(args)).read().strip()

pyroot="pyroot/"
ipbroot = 'ipbori'
cudd_name = 'pboriCudd'

[PyRootPath, IPBPath] = [PathJoiner(fdir) for fdir in [pyroot, ipbroot] ]


try:
	import SCons.Tool.applelink as applelink
except:
	pass
import os

def FinalizePermissions(targets, perm):
    for src in targets:
        env.AddPostAction(src, Chmod(str(src), perm))
    return targets

def FinalizeExecs(targets):
    return FinalizePermissions(targets, 0755)

def FinalizeNonExecs(targets):
    return FinalizePermissions(targets, 0644)

distribute = 'distribute' in COMMAND_LINE_TARGETS

prepare_deb = 'prepare-debian' in COMMAND_LINE_TARGETS
generate_deb = 'deb' in COMMAND_LINE_TARGETS
deb_generation = prepare_deb or generate_deb
generate_rpm = 'rpm' in COMMAND_LINE_TARGETS
generate_srpm = 'srpm' in COMMAND_LINE_TARGETS
rpm_generation = generate_rpm or generate_srpm

DefaultBuild = Default
if distribute or rpm_generation or deb_generation:
    def DefaultBuild(arg):
        return arg

defaultenv = Environment()

# Define option handle, may be changed from command line or custom.py
opts.Add('CXX', 'C++ Compiler', "g++")
opts.Add('CC', 'C Compiler', "gcc")
opts.Add('PYTHON', 'Python executable', "python")

opts.Add('LIBPATH', 'list of library paths (colon or whitespace separated)',
         [], converter = SplitColonSep)
opts.Add('CPPPATH', 'list of include paths (colon or whitespace separated)',
         [], converter = SplitColonSep)
opts.Add('CPPDEFINES', 'list of preprocessor defines (whitespace separated)',
         ['NDEBUG'], converter = Split)

opts.Add('CCFLAGS', "C compiler flags", "-O3 -ansi", converter = Split)
opts.Add('CXXFLAGS', "C++ compiler flags", "$CCFLAGS -ftemplate-depth-100",
         converter = Split)
opts.Add('LINKFLAGS', "Linker flags", ['-s'], converter = Split)
opts.Add('LIBS', 'custom libraries needed for build', [], converter = Split)

opts.Add('PREFIX', 'installation prefix directory', '/usr/local')
opts.Add('EPREFIX','executables installation prefix directory', '$PREFIX/bin')

opts.Add('INSTALLDIR', 'end user installation directory',
         '$PREFIX/share/polybori')
opts.Add('DOCDIR', 'documentation installation directory',
         '$INSTALLDIR/doc')
opts.Add('MANDIR', 'Man-pages installation directory',
         '$PREFIX/man')
opts.Add('PYINSTALLPREFIX',
         'python modules directory (default is built-in site)', '$PYTHONSITE')

opts.Add('DEVEL_PREFIX',
         'development version installation directory','$PREFIX' )

opts.Add('SINGULAR_HOME', 'directory of Singular development version', '')
         
opts.Add(BoolOption('HAVE_DOXYGEN',
                    'Generate doxygen-based documentation, if available', True))
opts.Add(BoolOption('HAVE_PYTHON_EXTENSION',
                    'Build python extension, if possible', True))
opts.Add(BoolOption('BOOST_WORKS',
                    'Skip check for Boost libraries', False))

opts.Add(BoolOption('RELATIVE_SYMLINK',
                    'Use relative symbolic links on install', True))

opts.Add(BoolOption('HAVE_L2H', 'Switch latex2html on/off', True))
opts.Add(BoolOption('HAVE_HEVEA', 'Switch hevea on/off (if latex2html is not available)', True))
opts.Add(BoolOption('HAVE_TEX4HT', 'Switch tex4ht on/off (if latex2html and hevea are not available) ', True))


opts.Add(BoolOption('HAVE_PYDOC', 'Switch python doc generation on/off', True))
opts.Add(BoolOption('EXTERNAL_PYTHON_EXTENSION', 'External python interface',
                    False))

opts.Add(BoolOption('USE_TIMESTAMP', 'Use timestamp on distribution', True))
opts.Add(BoolOption('SHLIBVERSIONING',
                    'Use dlltool-style versionated shared library', True))
opts.Add('SONAMEPREFIX', 'Prefix for compiler soname command.', '-Wl,-soname,')
opts.Add('SONAMESUFFIX','Suffix for compiler soname command.', '')

opts.Add('SHLINKFLAGS',
         'Shared libraries link flags.', defaultenv['SHLINKFLAGS'] +
         ['${_sonamecmd(SONAMEPREFIX, TARGET, SONAMESUFFIX, __env__)}'])

opts.Add('SHLIBVERSIONSUFFIX',
         'Shared libraries suffix for library versioning.',
         '-' + pboriversion +'.' + pborirelease +
         defaultenv['SHLIBSUFFIX'] + '.' + libraryversion)

pbori_cache_macros=["PBORI_UNIQUE_SLOTS","PBORI_CACHE_SLOTS","PBORI_MAX_MEMORY"]
for m in pbori_cache_macros:
    opts.Add(m, 'PolyBoRi Cache macro value: '+m, None)

tools =  ["default"]
if not GetOption('clean'):
    tools +=  ["disttar", "doxygen"]

# Get paths an related things from current environment
# todo: Are these settings sane in any case?
getenv = dict()
for key in ['PATH', 'HOME', 'LD_LIBRARY_PATH'] :
    try:
        getenv[key] = os.environ[key]
    except KeyError:
        pass

env = Environment(ENV = getenv, options = opts, tools = tools, toolpath = '.')

# Extract some option values
HAVE_DOXYGEN = env['HAVE_DOXYGEN'] and ("doxygen" in tools)
HAVE_PYTHON_EXTENSION = env['HAVE_PYTHON_EXTENSION']
BOOST_WORKS = env['BOOST_WORKS']

SINGULAR_HOME = env['SINGULAR_HOME']
USERLIBS = env['LIBS']

# Skipping doxygen-based docu, if no doxygen is found.
if HAVE_DOXYGEN:
    HAVE_DOXYGEN = env.Detect('doxygen')
    if not HAVE_DOXYGEN:
        print "Doxygen not found, skipping C++-documentation generation!"

    
#print env.Dump()

# soname related stuff
def _sonamecmd(prefix, target, suffix, env):
    """Creates soname."""

    target = str(env.subst(target))
    import re
    soPattern = re.compile('(.*)\.[0-9]*\.[0-9]*$', re.I|re.S)
    soname = soPattern.findall(path.basename(target))[0]
    if soname:
        return prefix + soname + suffix    
    else:
        return ''
    
env['_sonamecmd'] = _sonamecmd

cache_opts = PBPath('include/cacheopts.h')
cache_opts_file = open(cache_opts, "w")
for m in pbori_cache_macros:
    if env.get(m,None):
        cache_opts_file.write("#define "+m+" " +str(env[m])+"\n")
cache_opts_file.close()
#USERLIBS=env["USERLIBS"]
#if applelink in dir():
#    applelink.generate(env)


# todo: More generic?
IS_x64 = (2**32).__class__==int

# todo: machtype does not deliver the correct value for rpm and deb
# (only interesting for scons -c rpm|srpm|prepare-debian|deb)
try:
    machtype = os.environ['MACHTYPE']
except KeyError:
    machtype = 'undefined'


class PythonConfig(object):
    def __init__(self, python_executable):
        def querycmd(arg):
            return '"from distutils.sysconfig import *; print ' + arg + '"'
        
        self.python = python_executable
        self.version = shell_output(self.python, "-c",
                                    querycmd("get_python_version()"))
        self.sitedir = shell_output(self.python, "-c",
                                    querycmd("get_python_lib()"))
        self.libdir = shell_output(self.python, "-c",
                                   querycmd("get_config_vars()['LIBDIR']"))
        self.incdir = shell_output(self.python, "-c",
                                   querycmd("get_python_inc()"))
        self.staticlibdir = shell_output(self.python, "-c",
                                         querycmd("get_config_vars()['LIBPL']"))
        self.libs = shell_output(self.python, "-c",
                                 querycmd("get_config_vars()['LIBS']"))
        self.libs = self.libs.replace('-l','').split()
        self.libname = 'python' + str(self.version)

pyconf = PythonConfig(env["PYTHON"])

env.AppendUnique(PYTHONSITE = pyconf.sitedir)

# Generate usage for scons -h here
# (PYTHONSITE is already evaluated, but not python-specific paths)
Help(opts.GenerateHelpText(env))

have_l2h = have_t4h = False

if not env.GetOption('clean'):
    conf = Configure(env)

    extern_python_ext = env['EXTERNAL_PYTHON_EXTENSION']
    if HAVE_PYTHON_EXTENSION or extern_python_ext:
        env.Append(CPPPATH=[pyconf.incdir])
        env.Append(LIBPATH=[pyconf.libdir, pyconf.staticlibdir])

        env.Append(CPPPATH=[PBPath('include')])
        env.Append(CPPDEFINES=["PACKED","HAVE_M4RI"])
        env.Append(LIBPATH=["polybori","groebner"])
        env.Prepend(LIBS = ["m"])


    from re import search
    for variable in os.environ:
        if search("SAGE",variable):
            env['ENV'][variable]=os.environ[variable]


    if HAVE_PYTHON_EXTENSION:
        if not (BOOST_WORKS or
                conf.CheckCXXHeader(path.join('boost', 'python.hpp')) ):
            HAVE_PYTHON_EXTENSION = False
            print 'Warning Boost/python must be installed for python support'

    have_l2h = env['HAVE_L2H'] and env.Detect('latex2html')

    tex_to_ht = 'hevea'

    if not have_l2h:
        have_t4h = env['HAVE_HEVEA'] and env.Detect('hevea')
        if not have_t4h:
            have_t4h = env['HAVE_TEX4HT'] and env.Detect('htlatex')
            tex_to_ht = 'htlatex'
            if not have_t4h:
                print "Warning: No LaTeX to html converter found,",
                print "Tutorial will not be installed"

    env = conf.Finish()
# end of not cleaning

env.Clean('.', glob('*.pyc') + ['config.log'] )

have_pydoc = env['HAVE_PYDOC']

env['PBVERSION'] = pboriversion
env['PBRELEASE'] = pborirelease

# Resoruces for including anything into the PyPolyBoRi shared library
shared_resources = []

# Builder for symlinks
def build_symlink(target, source, env):
    targetdir = str(target[0].dir)
    target = target[0].path
    source = source[0].path
    if env['RELATIVE_SYMLINK'] :
        source = relpath(targetdir, source)
    
    print "Symlinking from", source, "to", target
    
    try:
        if path.exists(target):
            env.Remove(target)
        os.symlink(source, target)
    except:
        return True
    return None

symlinkbld = Builder(action = build_symlink)

env.Append(BUILDERS={'SymLink' : symlinkbld})
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
visibility_hidden=(env['PLATFORM']=="darwin")
def shared_object(o):
    if not visibility_hidden:
        return env.SharedObject(o)
    else:
        return env.SharedObject(o,CCFLAGS=env["CCFLAGS"]+["-fvisibility=hidden"],CXXFLAGS=env["CXXFLAGS"]+["-fvisibility=hidden"])

for fdir in Split("cudd dddmp mtr st epd"):
    env.Append( CPPPATH=[CuddPath(fdir)] )
    cudd_resources += glob(CuddPath(fdir, fdir + '*.c'))
    cudd_headers += [ CuddPath(fdir, fdir +'.h') ]


# exclude the following files
for fname in ['util/saveimage.c', 'util/test*.c','dddmp/*DdNode*.c']:
    for file in glob(CuddPath(fname)):
        cudd_resources.remove(file)

cudd_shared = shared_object(cudd_resources)

libCudd = env.StaticLibrary(CuddPath(cudd_name), cudd_resources)
DefaultBuild(libCudd)

shared_resources += cudd_shared

def SymlinkReadableLibname(files):
    """ Generate symbolik link with more readable library name."""
    
    suffix = env.subst('$SHLIBVERSIONSUFFIX')
    simplesuffix = env.subst('$SHLIBSUFFIX')
    result = []
    for fname in files:
        simple = str(fname).replace(suffix, simplesuffix)
        if (str(fname) != simple) :
            result += env.SymLink(simple, fname)
    return result

def VersionatedSharedLibrary(*args, **kwds):

    kwds['SHLIBSUFFIX'] = env.subst('$SHLIBVERSIONSUFFIX')
    
    return env.SharedLibrary(*args, **kwds)

slib = env.SharedLibrary
if env['SHLIBVERSIONING']:
    slib = VersionatedSharedLibrary
if env['PLATFORM']=="darwin":
    slib=env.LoadableModule


libCuddShared = slib(CuddPath(cudd_name), list(shared_resources))


######################################################################
# Stuff for building PolyBoRi's C++ part
######################################################################

pb_src=Split("""BoolePolyRing.cc BooleEnv.cc BoolePolynomial.cc BooleVariable.cc
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
DefaultBuild(libpb)


pb_shared = shared_object(pb_src)#env.SharedObject(pb_src)
shared_resources += pb_shared

libpbShared = slib(PBPath('polybori'), list(shared_resources))
#DefaultBuild(libpbShared)

env.Clean(libpb + pb_shared, cache_opts)

######################################################################
# Stuff for building Groebner library
######################################################################

gb_src=Split("groebner.cc literal_factorization.cc randomset.cc pairs.cc groebner_alg.cc polynomial_properties.cc lexbuckets.cc dlex4data.cc dp_asc4data.cc lp4data.cc nf.cc interpolate.cc")
gb_src=[GBPath('src', source) for source in gb_src]+m4ri
gb=env.StaticLibrary(GBPath('groebner'), gb_src+[libpb])

#print "gb:", gb, dir(gb)
#sometimes l seems to be boxed by a list
if isinstance(gb,list):
    gb=gb[0]
DefaultBuild(gb)

gb_shared = shared_object(gb_src)#env.SharedObject(gb_src)
shared_resources += gb_shared

libgbShared = slib(GBPath('groebner'), list(shared_resources))
#DefaultBuild(libgbShared)

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
                               specialsets.py aes.py memusage.py
                               heuristics.py gbcore.py interpolate.py coding.py
                               interred.py ncf.py partial.py simplebb.py
                               PyPolyBoRi.py __init__.py dynamic/__init__.py""")
                               ] 


# Currently all python modules are at place
installable_python_modules = []


def add_cnf_dir(env,directory):
  for f in glob(path.join(directory, "*.cnf")):
      env.CNF(f[:-4])

pydocu = []
dynamic_modules = []

python_absolute = shell_output("which", env["PYTHON"])

if HAVE_PYTHON_EXTENSION:
    wrapper_files=[ PyPBPath(f) for f in Split("""test_util.cc main_wrapper.cc
    dd_wrapper.cc Poly_wrapper.cc navigator_wrap.cc variable_block.cc
    monomial_wrapper.cc misc_wrapper.cc strategy_wrapper.cc set_wrapper.cc
    slimgb_wrapper.cc""") ] 
    
    if env['PLATFORM']=="darwin":
        pypb=env.LoadableModule(PyPBPath('PyPolyBoRi'),
            wrapper_files + shared_resources,
            LINKFLAGS="-bundle_loader " + python_absolute,
            LIBS = pyconf.libs + LIBS,LDMODULESUFFIX=".so",
            CPPPATH=CPPPATH,CCFLAGS=env["CCFLAGS"]+["-fvisibility=hidden"],CXXFLAGS=env["CXXFLAGS"]+["-fvisibility=hidden"])
    else:
        #print "l:", l
        pypb=env.SharedLibrary(PyPBPath('PyPolyBoRi'),
            wrapper_files + shared_resources,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", LIBS = LIBS,
            CPPPATH=CPPPATH)
            #LIBS=env['LIBS']+['boost_python',l])#,LDMODULESUFFIX=".so",\
            #SHLIBPREFIX="")
    DefaultBuild(pypb)

    # Define the dynamic python modules in pyroot
    dynamic_modules = env.Install(PyRootPath('polybori/dynamic'), pypb)
    documentable_python_modules += dynamic_modules
   
    DefaultBuild(dynamic_modules)
    
    
    polybori_modules = PyRootPath("polybori")
    #DefaultBuild(env.Install(polybori_modules, pypb))
    for (f,n) in installable_python_modules:
        DefaultBuild(env.Install(polybori_modules, f))


    
    to_append_for_profile = [libpb, gb] + libCudd
    #to_append_for_profile=File('/lib/libutil.a')
    env.Program(PyPBPath('profiled'), wrapper_files+to_append_for_profile,
            LDMODULESUFFIX=".so",SHLIBPREFIX="", 
            LIBS = LIBS + ["python" + str(pyconf.version)] + USERLIBS,
            CPPPATH=CPPPATH, CPPDEFINES=env["CPPDEFINES"]+["PB_STATIC_PROFILING_VERSION"])

    from StringIO import StringIO


    # Converting cnf files to PolyBoRi python format
    def cnf2py_build_function(target,source,env):
        sys.path.append(TestsPath("py"))
        from cnf2ideal import gen_clauses, process_input,convert_file_PB

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

else:
    print "no python extension"

if HAVE_PYTHON_EXTENSION or extern_python_ext:
    # Generating python documentation
    def pypb_emitter(target,source,env):

        TargetPath = PathJoiner(target[0].dir)
        for file in source:
            (fname, fext) = path.splitext(str(file).replace(pyroot,''))
            if fext in ['.so', '.py']:
                fname = TargetPath(fname.replace(sep,'.') + '.html')
                target.append(env.File(fname))

        return (target, source)

    bld = Builder(action = "$PYTHON doc/python/genpythondoc.py " + pyroot,
                  emitter = pypb_emitter)

    # Add the new Builder to the list of builders
    env['BUILDERS']['PYTHONDOC'] = bld

    # Generate foo.vds from foo.txt using mk_vds
    #for f in Split("ll.py nf.py gbrefs.py blocks.py PyPolyBoRi.so specialsets.py"):
    if have_pydoc:
        pydocu = env.PYTHONDOC(target=[DocPath('python/polybori.html'),
                                       DocPath('python/polybori.dynamic.html')],
                               source = documentable_python_modules)

        env.Clean(pydocu, glob(PyRootPath('polybori/*.pyc')) +
                  glob(PyRootPath('polybori/dynamic/*.pyc')))
#        env.Ignore(pydocu, dynamic_modules)
    #bld=Builder("cd")

    
HAVE_SINGULAR_EXTENSION=True

docutarget = [DocPath('c++', elt) for elt in Split("html latex")]
if HAVE_DOXYGEN:
    cxxdocu = env.Doxygen(source=[DocPath('doxygen.conf')], target = docutarget)
    env.AlwaysBuild(cxxdocu)

env.Clean(DocPath('c++'), docutarget)

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
    DefaultBuild(singpb)
    


# Source distribution archive generation
env.Append(DISTTAR_EXCLUDEEXTS = Split(""".o .os .so .a .dll .cache .pyc
           .cvsignore .dblite .log .sconsign .depend .out .graphViz_temp
           .kprof.html .rpm .spec """),
           DISTTAR_EXCLUDEDIRS = Split("CVS .svn .sconf_temp SOURCES BUILD"),
           DISTTAR_EXCLUDEPATTERN = Split(""".#* #*# *~ profiled cacheopts.h
           coding.py """ + env.subst('*$SHLIBVERSIONSUFFIX')))

if distribute or rpm_generation or deb_generation:
    allsrcs = Split("SConstruct README LICENSE ChangeLog disttar.py doxygen.py")
    for dirname in Split("""Cudd extra groebner ipbori M4RI polybori 
    PyPolyBoRi pyroot Singular pkgs"""):
        allsrcs.append(env.Dir(dirname))

    # Testsuite is not distributed completely
    allsrcs += [TestsPath('execsuite')]
    allsrcs += glob(TestsPath('py/*.py'))

    for exclsrc in Split("""aes_elim.py gbrefs_pair.py red_search.py
    rtpblocks.py runstas1.py rundummy.py specialsets2.py"""):
        for file in glob(TestsPath('py', exclsrc)):
            allsrcs.remove(file)
        
    for dirname in Split("src ref"):
        allsrcs.append(env.Dir(TestsPath(dirname)))

    # doc is not distributed completely
    allsrcs += [ DocPath(dsrc) for dsrc in Split("""doxygen.conf index.html.in
    tutorial/tutorial.tex python/genpythondoc.py man/ipbori.1 """) ]
    allsrcs.append(env.Dir(DocPath('images')))

if distribute:
    presrcdistri = env.DistTar(debname, allsrcs)
    (srcdistrname, srcdistrext1) = path.splitext(str(presrcdistri[0]))
    (srcdistrname, srcdistrext) = path.splitext(srcdistrname)
    srcdistrext += srcdistrext1
    pborisuffix = ""

    if env['USE_TIMESTAMP']:
        from datetime import date
        pborisuffix += "-" + str(date.today())
        
    srcdistri = env.Command(srcdistrname + pborisuffix + srcdistrext,
                            presrcdistri,                            
                            Move("$TARGET", "$SOURCE"))

    env.AlwaysBuild(srcdistri)
    env.Alias('distribute', srcdistri)
    
devellibs = [libpb,gb] + libCudd + libpbShared + libgbShared + libCuddShared
readabledevellibs = SymlinkReadableLibname(devellibs)

# Installation for development purposes
if 'devel-install' in COMMAND_LINE_TARGETS:
    DevelInstPath = PathJoiner(env['DEVEL_PREFIX'])
    
    SymlinkReadableLibname(env.Install(DevelInstPath('lib'), devellibs))
    
    env.Install(DevelInstPath('include/polybori'), glob(PBPath('include/*.h')))
    env.Install(DevelInstPath('include/polybori/groebner'),
                glob(GBPath('src/*.h')))
    env.Install(DevelInstPath('include/cudd'), cudd_headers)
    env.Install(DevelInstPath('include/polybori/M4RI'), glob('M4RI/*.h'))
    env.Alias('devel-install', DevelInstPath())


env.Append(COPYALL_PATTERNS = ['*'])

# Copy glob('*') from one directory to the other
def cp_all(target, source, env):
    source = source[0].path
    target = target[0].path

    if not path.exists(target):
        Execute(Mkdir(target))
    for patt in env['COPYALL_PATTERNS']:
        for file in glob(path.join(source, patt)):
            if not path.isdir(file):
                result = str(path.join(target, path.basename(file)))
                Execute([Copy(result, file), Chmod(result, 0644)])

    return None

# Copy python docu from one directory to the other and correct paths to modules
def cp_pydoc(target, source, env):

    source = source[0].path
    target = target[0].path
    import re
    patt = re.compile('(file:|)/[^\" ]*' + pyroot, re.VERBOSE)

    if not path.exists(target):
        Execute(Mkdir(target))
    showpath = relpath(env.Dir(target).abspath,
                       env.Dir(env['PYINSTALLPREFIX']).abspath) +'/'

    for file in glob(path.join(source, '*.html')):
        if not path.isdir(file):
            fcontent = open(file).read()
            fcontent = patt.sub(r''+showpath, fcontent)

            result = str(path.join(target, path.basename(file)))
            open(result, "w").write(fcontent)

    return None


cp_recbld = Builder(action = cp_all)
cp_pydocbld = Builder(action = cp_pydoc)


# Note: target[0] needs to be the path resulting from latex2html
# todo: More generic by moving actual result to given path?
def l2h_emitter(target, source, env):
    target = [env.File(path.join(str(target[0]), 'index.html'))]
    env.Clean(target, target[0].dir)
    return (target, source)

l2h = Builder(action = 'latex2html -html_version 4.0,unicode,utf-8 $SOURCE',
              emitter = l2h_emitter)

def t4h_emitter(target, source, env):
    target = [env.File(path.join(str(target[0]), 
                      path.splitext(path.basename(source[0].name))[0] + '.html'))]
    env.Clean(target, target[0].dir)
    return (target, source)


if have_t4h :
    t4h_str =  tex_to_ht + ' ' + path.join(env.Dir('').abspath, "$SOURCE")
    tex_to_ht_bld = Builder(action = 'cd `dirname $TARGET`;' + t4h_str + ';'
                            + t4h_str, emitter = t4h_emitter)
    env.Append(BUILDERS={'TeXToHt' : tex_to_ht_bld})



def pathsplit(p, rest=[]):
    (h,t) = os.path.split(p)
    if len(h) < 1: return [t]+rest
    if len(t) < 1: return [h]+rest
    return pathsplit(h,[t]+rest)

def commonpath(l1, l2, common=[]):
    if len(l1) < 1: return (common, l1, l2)
    if len(l2) < 1: return (common, l1, l2)
    if l1[0] != l2[0]: return (common, l1, l2)
    return commonpath(l1[1:], l2[1:], common+[l1[0]])

def relpath(p1, p2):
    (common,l1,l2) = commonpath(pathsplit(p1), pathsplit(p2))
    p = []
    if len(l1) > 0:
        p = [ ('..' + sep) * len(l1) ]
    p = p + l2

    if len(p) == 0:
        return ''
    return os.path.join( *p )



# substition function 
def substitute_install(target, source, env,
                       preprocess = None, postprocess = None):
        from string import Template
        page = open(str(source[0]), 'r').read()

        if preprocess:
            page = preprocess(page) 
        page = Template(page).safe_substitute(env)
        if postprocess:
            page = postprocess(page)     
        open(str(target[0]), 'w').write(page)
        return None

substinstbld  = Builder(action = substitute_install)


def docu_master(target, source, env):
    import os, re

    basefiles = ['index.html', 'polybori.html']
    basesfound = []
    
    for item in source:
        if os.path.isdir(str(item)):
            for root, dirs, files in os.walk(str(item)):
                for file in files:
                    if file in basefiles:
                        basesfound.append(os.path.join(root, file))

    if str(target[0]) in basesfound:
        basesfound.remove(str(target[0]))

    linkPattern = re.compile('<title.*?>(.*?)</title>', re.I|re.S) 
    links = ''
    for src in basesfound:
        fhandle = open(src, 'r')
        fcontent = fhandle.read()
        fhandle.close()
        ftitle = linkPattern.search(fcontent).group(1)
        relsrc = relpath(str(target[0].dir), src)
        links +=  '<P><A href="' + relsrc + '">' + ftitle +'</A>\n'


    env['PBDOCLINKS'] = links
    substitute_install(target, source, env)

    return None

def docu_emitter(target, source, env):
    if source[0].dir != target[0].dir :
        import re
        linkPattern = re.compile('<img src *?= *?"([^>]*?)".*?>', re.I|re.S) 
        fhandle = open(str(source[0]), 'r')
        relsrcs = linkPattern.findall(fhandle.read())
        SrcPath = PathJoiner(source[0].dir)
        TargetPath = PathJoiner(target[0].dir)

        for src in relsrcs:
            FinalizeNonExecs(env.InstallAs(env.File(TargetPath(src)),
                                           SrcPath(src)))
        fhandle.close()
        
        
    return (target, source)

    
masterdocubld  = Builder(action = docu_master, emitter = docu_emitter)

env.Append(BUILDERS={'CopyAll': cp_recbld, 'L2H': l2h,
                     'SubstInstallAs': substinstbld,
                     'CopyPyDoc':cp_pydocbld})
env.Append(BUILDERS={'DocuMaster': masterdocubld})



def spec_builder(target, source, env):

    substitute_install(target, source, env)
    return None

specbld = Builder(action = spec_builder)

def rpmemitter(target, source, env):
    target = [RPMPath('RPMS', machtype, target[0].name + '.' +machtype+ '.rpm')]
    return (target, source)

def srpmemitter(target, source, env):
    target = [RPMPath('SRPMS', target[0].name + '.src.rpm')]
    return (target, source)   
    
def generate_rpmbuilder(rpmopts, emitt = rpmemitter):
    return Builder(action = "rpmbuild " + rpmopts + " --define='_topdir " +
                   Dir(RPMPath()).abspath +  "' $SOURCE", emitter = emitt)

srpmbld  = generate_rpmbuilder('-bs', srpmemitter)
rpmbld  = generate_rpmbuilder('-bb', rpmemitter)

# debbuilder is very experimental, we ignore dependencies currently (-d)
debbld = Builder(action = "dpkg-buildpackage -d -rfakeroot")
    
env.Append(BUILDERS={'SpecBuilder': specbld,
                     'RPMBuilder': rpmbld, 'SRPMBuilder': srpmbld,
                     'DebBuilder': debbld})

if have_l2h:
    tutorial = env.L2H(env.Dir(DocPath('tutorial/tutorial')),
                       DocPath('tutorial/tutorial.tex'))
else:
    if have_t4h :
        tutorial = env.TeXToHt(env.Dir(DocPath('tutorial/tutorial')),
                               DocPath('tutorial/tutorial.tex'))
    
# Clean, even, if L2H/TexToHt are not available anymore
env.Clean(DocPath('tutorial'), DocPath('tutorial/tutorial'))

env.DocuMaster(DocPath('index.html'), [DocPath('index.html.in')] + [
    env.Dir(DocPath(srcs)) for srcs in Split("""tutorial python c++""") ] + [
    env.Dir('Cudd/cudd/doc')])  

pbrpmname = pboriname + '-' + pboriversion + "-" + pborirelease 

if rpm_generation:
    # Some file servers use invalid group-ids, so change to current gid
    def correctgid(target, source, env):
        os.chown(target[0].path, -1, os.getgid())

    rpmsrcs = FinalizeNonExecs(env.DistTar(RPMPath('SOURCES',
                                                   "PolyBoRi-" + pboriversion),
                                           allsrcs))
    env.AddPostAction(rpmsrcs, correctgid)
    
    pbspec = FinalizeNonExecs(env.SpecBuilder(SpecsPath(pbrpmname +'.spec'),
                                              SpecsPath('PolyBoRi.spec.in')))

        
    env.AddPostAction(pbspec, correctgid)

    env.AlwaysBuild(pbspec)
    
    pbsrpm = env.SRPMBuilder(RPMPath('SRPMS', pbrpmname),
                             pbspec + rpmsrcs)
    env.AlwaysBuild(pbsrpm)
    pbrpm = env.RPMBuilder(RPMPath('RPMS', pbrpmname ),
                           pbspec + rpmsrcs)

    def provide_builddir(target, source, env):
        if not path.exists(RPMPath('BUILD')):
            Execute(Mkdir(RPMPath('BUILD')))
    
    env.AddPreAction(pbrpm, provide_builddir)
    env.AlwaysBuild(pbrpm)
    env.Alias('srpm', pbsrpm)
    env.Alias('rpm', pbrpm)



if prepare_deb or generate_deb:
    debsrc = env.SubstInstallAs(DebInstPath('changelog'),
                                DebPath('changelog.in'))


    debsrc += FinalizeNonExecs(env.SubstInstallAs(DebInstPath('control.in'),
                                                  DebPath('control.in')))
    env['cdbs'] = 'cdbs (>= 0.4.23-1.1), debhelper (>= 5), quilt, patchutils (>= 0.2.25), cdbs (>= 0.4.27-1)'

    def preprocessed_substitute(target, source, env):
        def preprocess_at(page):
            import re
            p = re.compile('@([^@\n]*) @', re.VERBOSE)
            return p.sub(r'$\1', page)
        
        substitute_install(target, source, env, preprocess=preprocess_at)
    
    debsrc += FinalizeNonExecs(env.Command([DebInstPath('control')],
                                           DebPath('control.in'),
                                           preprocessed_substitute))
        
    for src in ['rules', 'compat', 'copyright', 
                'libpolybori-dev.install',  'python-polybori.install'] :
        debsrc += env.Install(DebInstPath(), DebPath(src))
        
    debsrc += env.InstallAs(DebInstPath('libpolybori-' + pboriversion +
                                        '.' + pborirelease   +'-0.install'),
                            DebPath('libpolybori0.install'))
        
    env.Alias('prepare-debian', DebInstPath())
    env.Clean(DebInstPath(), DebInstPath())
    
    pbdeb = env.DebBuilder(path.join('..', debname + '-' + pborirelease +
                                     '.' + machtype + '.deb'), debsrc)
    
    env.AlwaysBuild(env.Alias('deb', pbdeb))    
    

def GeneratePyc(sources):
    results = []
    for file in sources:
        (fbase, fext) = path.splitext(file.name)
        if (fext == '.py') :
            cmdline = """$PYTHON -c "import py_compile; """
            cmdline +=  """py_compile.compile('""" + str(file) + """')" """ 
            results += env.Command(str(file) +'c', file, cmdline)

        env.Depends(results, sources)
    return results


def expand_repeated(val, env):
    from string import Template
    newval = Template(val).safe_substitute(env)
    while (val != newval):
        val = newval
        newval = Template(val).safe_substitute(env)

    return val
        
# Installation precedure for end users
if 'install' in COMMAND_LINE_TARGETS:
    # Setting umask for newly generated directories
    try:
        umask = os.umask(022)
        print 'setting umask to 022 (was 0%o)' % umask
    except OSError:     # ignore on systems that don't support umask
        pass
    
    InstPath = PathJoiner(env['INSTALLDIR'])
    InstExecPath = PathJoiner(env['EPREFIX'])
    InstDocPath = PathJoiner(env['DOCDIR'])
    InstPyPath = PathJoiner(env['PYINSTALLPREFIX'])
    InstManPath = PathJoiner(env['MANDIR'])
    
    for inst_path in [InstPath(), InstExecPath(), InstDocPath(), InstPyPath(),
                      InstManPath()]:
        env.Alias('install', inst_path)
    env.Install(InstManPath('man1'), DocPath('man/ipbori.1'))
    
    # Executables and shared libraries to be installed
    pyfiles = []
    for instfile in dynamic_modules :
        installedfile = InstPyPath(relpath(pyroot, instfile.path))
        pyfiles += FinalizeExecs(env.InstallAs(installedfile, instfile))

    
    for instfile in [ IPBPath('ipbori') ]:
        FinalizeExecs(env.InstallAs(InstPath(instfile), instfile))

    # Copy c++ documentation
    if HAVE_DOXYGEN:
        cxxdocinst = env.CopyAll(env.Dir(InstDocPath('c++')),
                                 env.Dir(DocPath('c++/html'))) 
        env.Depends(cxxdocinst, cxxdocu)
        env.Clean(cxxdocinst, cxxdocinst)

    htmlpatterns = Split("*.html *.css *.png *gif *.jpg")

    # Copy python documentation
    pydocuinst = env.CopyPyDoc(env.Dir(InstDocPath('python')),
                               env.Dir(DocPath('python')))

    env.Depends(pydocuinst, pydocu)
    env.Clean(pydocuinst, pydocuinst)

    # Copy Cudd documentation
    env.CopyAll(env.Dir(InstDocPath('cudd')),
                env.Dir('Cudd/cudd/doc') ) 
    env.CopyAll(env.Dir(InstDocPath('cudd/icons')),
                env.Dir('Cudd/cudd/doc/icons'))

    # Copy Tutorial
    if have_l2h or have_t4h :
        env.CopyAll(env.Dir(InstDocPath('tutorial')),
                    env.Dir(DocPath('tutorial/tutorial')),
                    COPYALL_PATTERNS = htmlpatterns)

    # Generate html master
    FinalizeNonExecs(env.DocuMaster(InstDocPath('index.html'),
                                    [DocPath('index.html.in')] + [ 
        env.Dir(InstDocPath(srcs)) for srcs in Split("""tutorial python
        c++ cudd""") ] ))

    # Non-executables to be installed
    for instfile in glob(PyRootPath('polybori/*.py')) + [
        PyRootPath('polybori/dynamic/__init__.py') ] :
        targetfile = InstPyPath(relpath(PyRootPath(), instfile))
        pyfiles += FinalizeNonExecs(env.InstallAs(targetfile, instfile))

    if HAVE_PYTHON_EXTENSION or extern_python_ext:
        FinalizeNonExecs(GeneratePyc(pyfiles))

    env['PYINSTALLPREFIX'] = expand_repeated(env['PYINSTALLPREFIX'], env)
    env['RELATIVEPYPREFIX'] = relpath(expand_repeated(InstExecPath(),env),
                                      env['PYINSTALLPREFIX'])
        
    for instfile in [IPBPath('ipythonrc-polybori') ] :
        FinalizeNonExecs(env.SubstInstallAs(InstPath(instfile), instfile))
      

    # Symlink from executable into bin directory
    ipboribin = env.SymLink(InstExecPath('ipbori'),
                            InstPath(IPBPath('ipbori')))
    env.AlwaysBuild(ipboribin)   
    env.Alias('install', ipboribin)

env.Alias('prepare-devel', devellibs + readabledevellibs)
env.Alias('prepare-install', [pyroot, DocPath()])

