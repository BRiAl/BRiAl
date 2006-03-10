#$Id$
opts = Options('custom.py')
env=Environment(options=opts)

conf = Configure(env)
env.Append(CPPPATH=["./polybori/include"])
env.Append(CPPPATH=["./Cudd/include"])
env.Append(LIBPATH=["./Cudd/cudd","Cudd/obj"])
env.Append(LIBS=["cudd", "obj"])
env = conf.Finish()


pb_src=Split("BoolePolyRing.cc CErrorInfo.cc PBoRiError.cc")
pb_src=["./polybori/src/"+ source for source in pb_src]
tests=["errorcodes","testobj","testring"]
for t in tests:
    env.Program(t, ["testsuite/src/"+t+".cc"]+pb_src)



