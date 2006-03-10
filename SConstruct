#$Id$
opts = Options('custom.py')
env=Environment(options=opts)


cudd_libs=Split("cudd obj dddmp mtr st util epd")

conf = Configure(env)

env.Append(CPPPATH=["./polybori/include"])
env.Append(CPPPATH=["./Cudd/include"])
env.Append(LIBPATH=["polybori"])
#env.Append(CPPDEFINES=["NDEBUG"])
env.Append(LIBS=["m"])
env.Append(CCFLAGS=Split("-O3 -ftemplate-depth-30 -ansi -Wno-long-double"))

for l in cudd_libs:
    env.Append(LIBPATH=["./Cudd/"+l])
    env.Append(LIBS=[l])
env = conf.Finish()


pb_src=Split("BoolePolyRing.cc CErrorInfo.cc PBoRiError.cc")
pb_src=["./polybori/src/"+ source for source in pb_src]
l=env.StaticLibrary("polybori/polybori", pb_src)
tests=["errorcodes","testring"]

for t in tests:
    env.Program("testsuite/"+t, ["testsuite/src/" + t +".cc"] +[l])



