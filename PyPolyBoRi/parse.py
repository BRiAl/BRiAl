from pyparsing import Literal,CaselessLiteral,Word,Combine,Group,Optional,\
    ZeroOrMore,Forward,nums,alphas, Or
#from itertools import enumerate
#import math
import operator
from PyPolyBoRi import *
try:
    set
except NameError:
    from sets import Set as set
#variables=["x","y","z"]
#def pushFirst( strg, loc, toks ):
#    exprStack.append( toks[0] )
factors=set()
terms=set()
polys=set()

def set_variable(str, log, toks):
    global current_variable
    current_variable=toks[0]
    factors.add(toks[0])

def add_term(str,log, toks):
    global factors, terms
    tuple_form=tuple(sorted(list(factors)))
    if tuple_form in terms:
        terms.remove(tuple_form)
    else:
        terms.add(tuple_form)
    factors=set()
def add_poly(str,log,toks):
    global terms
    polys.add(tuple(sorted(list(terms))))
    terms=set()

def generate_ideal_bnf(variables):
    """
    expop   :: '^'
    multop  :: '*' | '/'
    addop   :: '+' | '-'
    integer :: ['+' | '-'] '0'..'9'+
    atom    :: PI | E | real | fn '(' expr ')' | '(' expr ')'
    factor  :: atom [ expop factor ]*
    term    :: factor [ multop factor ]*
    expr    :: term [ addop term ]*
    """

    point = Literal( "." )
    e     = CaselessLiteral( "E" )
    exponent=Word(nums)
    fnumber = Combine( Word( "+-"+nums, nums ) + 
                       Optional( point + Optional( Word( nums ) ) ) +
                       Optional( e + Word( "+-"+nums, nums ) ) )

 
    plus  = Literal( "+" )
    minus = Literal( "-" )
    mult  = Literal( "*" )
    div   = Literal( "/" )
    seperator=Literal(",")
    lpar  = Literal( "(" ).suppress()
    rpar  = Literal( ")" ).suppress()
    addop  = plus | minus
    multop = mult | div
    expop = Literal( "^" )
    one=Literal("1")
    
    poly = Forward()
    variable = Or([Literal(v) for v in variables]).setParseAction(set_variable)
    factor = Forward()
    factor << Or([one,variable + ZeroOrMore( ( expop + exponent))])
    
    term = Forward()
    term << factor + ZeroOrMore( ( multop + factor ))
    term.setParseAction(add_term)
    poly << term + ZeroOrMore( ( addop + term ) ).setParseAction(add_poly)
    ideal=Forward()
    
    ideal << poly+ ZeroOrMore( ( seperator + poly ) )+Literal(";")
    return ideal
    
def parse_ideal(i, variables):
    global terms, factors, polys
    terms=set()
    factors=set()
    polys=set()
    bnf=generate_ideal_bnf(variables)
    bnf.parseString(i)
    return polys
def translate_ideal(s,vars):
    r=Ring(len(vars))
    var_cache=[Variable(i) for i in range(r.nVars())]
    trans=dict([(v,i) for (i,v) in enumerate(vars)])
    raw=parse_ideal(s,vars)
    res=[]
    for p in raw:
        myp=Polynomial()
        for t in p:
            #myterm=r.one()
            if len(t)>0:
                vars=[var_cache[trans[v]] for v in t]
                
                myterm=Polynomial(vars[0])           
                for v in vars[1:]:
                    myterm*=v
                # for v in t:
    #                 
    #                 print v
    #                 print trans[v]
    #                 myterm.change(trans[v])
                #print "nodes per term", myterm.nNodes()
            else:
                myterm=r.one()
            myp+=Polynomial(myterm)
                
        res.append(myp)
    return (r,res)

if __name__=='__main__':
    print parse_ideal("x*y*z+x, x*z+y*z,y*z+x", ["x","y","z"])
    i=translate_ideal("x*y*z+x, x*z+y*z,y*z+x", ["x","y","z"])
    for p in i:
        print p.nNodes()



