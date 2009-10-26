import pathadjuster
from pyparsing import Literal,CaselessLiteral,Word,Combine,Group,Optional,\
    ZeroOrMore,Forward,nums,alphas, Or, restOfLine,OneOrMore,restOfLine,alphanums

#from polybori.gbrefs import declare_ring
from polybori import *
from polybori.ll import ll_encode
from optparse import OptionParser
from polybori.statistics import used_vars_set
from itertools import chain
from StringIO import StringIO
parser = OptionParser()

gat_max=20000
next_max=2000
output_max=2000
inter_max=20000
input_max=2000
state_max=2000

#declare_ring([Block("x",gat_max),Block("xnext",next_max,reverse=True),Block("xoutput",output_max,reverse=True),Block("xinter",inter_max,reverse=True),Block("xinput",input_max,reverse=True),Block("xstate",state_max,reverse=True)],globals())
#input order is antitopological
#we reverse, when mapping to final variables 
parser.add_option("--forward-iteration",
                  action="store_true", dest="forward", default=True,
                  help="switch between forward and backward iteration")

parser.add_option("--backward-iteration",
                  action="store_false", dest="forward", default=True,
                  help="switch between forward and backward iteration")

parser.add_option("--include-outputs",
                  action="store_true", dest="include_outputs", default=False,
                  help="include circuit outputs")

parser.add_option("--initialize-state",
                  action="store", dest="initialize", default="noinit",type="choice",choices=["noinit", "random", "zero"],
                  help="initialize state variables/only useful with forward iteration")



def parse_identifier(str,log,tokens):
    return int(tokens[0])


def throw_away(str,log,tokens):
    return "None"

assigned=set()
from random import Random

def p2var(p):
    assert p.varsAsMonomial().deg()==1
    return p.lead().variables().next()
def add_negated(str,log,tokens):
    p=tokens[1]
    l=p.lead()
    if not l in assigned and r.randint(0,200)==0:
        assigned.add(l)
        print "NEG",p+1
        return p+1
    else:
        return "NONE"


def fix_symbol_name(str,log,tokens):
    return tokens[0].replace("-","_").replace("]","").replace("[","").replace("/","_")

class VariableManager(object):
    """docstring for VariableManager"""
    def __init__(self,prefix="", initialize="noinit", **kwd):
        super(VariableManager, self).__init__()
        self.output=[]
        self.input=[]
        self.state=[]
        self.next=[]
        self.intermediate=[]
        
        self.next_state_equations=[]
        self.intermediate_equations=[]
        self.__map=dict()
        self.prefix=prefix
        self.initialize = initialize
        for (k, v) in kwd.iteritems():
            setattr(self, k, v)
    def ideals(self):
        ideal_state=[]
        ideal_next_state=self.next_state_equations
        ideal_intermediate=self.intermediate_equations

        if self.initialize!="noinit":
            if self.initialize=="zero":
                initializer=zero_fun
            else:
                initializer=random_bit
            for v in variables:
                if str(v)[:1]=="s":
                    ideal_state.append(v+initializer())

        return [
            self.apply_map(i) for i in 
            (ideal_state,ideal_intermediate, ideal_next_state)]
    def set_variable_name(self, i, name):
        set_variable_name(i, self.prefix+name)
    def parse_output_action(self,str,log,tokens):
        p=Polynomial(tokens[1])
        #self.output.append(p)
        #mapped_to=xoutput(len(self.output)-1)
        mapped_to=self.xoutput(len(self.output))
        self.output.append(mapped_to)
        self.set_variable_name(mapped_to.index(),"out_"+tokens[2])
        if self.include_outputs:
            self.intermediate_equations.append(mapped_to+p)
            return mapped_to+p
        else:
            return "NONE"
    
    
    def parse_buffer_action(self,str,log,tokens):
        return "NONE"
    
    def parse_next_state_action(self,str,log,tokens):
        p=Polynomial(tokens[1])
        #self.next.append(p)
        #mapped_to=xnext(len(self.next)-1)
        mapped_to=self.xnext(len(self.next))
        self.next.append(mapped_to)
        self.set_variable_name(mapped_to.index(),"nstate_"+tokens[2])
        self.next_state_equations.append(mapped_to+p)
        return mapped_to+p
    def parse_state_action(self,str,log,tokens):
        p=self.x(tokens[0])
        #self.state.append(p)
        #mapped_to=xstate(len(self.state)-1)
        mapped_to=self.xstate(len(self.state))
        self.state.append(mapped_to)
        self.set_variable_name(mapped_to.index(),"state_"+tokens[2])
        self.map(p,mapped_to)
        return "NONE"
    def parse_input_action(self,str,log,tokens):
        p=self.x(tokens[0])
        #self.input.append(p)
        #mapped_to=xinput(len(self.input)-1)
        mapped_to=self.xinput(len(self.input))
        self.input.append(mapped_to)
        self.set_variable_name(mapped_to.index(),"in_"+tokens[2])
        self.map(p,mapped_to)
        return "NONE"
    
    def parse_and_action(self,string,log,tokens):
        inter=self.x(tokens[0])
        op1=tokens[2]
        op2=tokens[3]
        #self.intermediate.append(inter)
        #mapped_to=xinter(len(self.intermediate)-1)
        mapped_to=self.xinter(len(self.intermediate))
        self.intermediate.append(mapped_to)
        self.set_variable_name(mapped_to.index(),"y"+str(tokens[0]))
        self.map(inter,mapped_to)
        eq=inter+op1*op2
        self.intermediate_equations.append(eq)
        return eq
    def map(self,from_,to):
        self.__map[from_]=to
    def apply_map(self,eq):
        encoded=ll_encode((k+v for (k,v) in self.__map.items()))
        return [ll_red_nf_noredsb(p,encoded) for p in eq]
    
    
    def parse_sign(self, str,log,tokens):
        if list(tokens)!=[0]:
            return [1]
        else:
            return tokens
    def parse_idenfifier_ref(self, str,log,tokens):
        if tokens[1] in (0,1):
            #from sys import stderr
            #stderr.write("TOKENS:"+repr(tokens))
            #stderr.flush()
            return Polynomial(tokens[0]+tokens[1])
        return tokens[0]+self.x(tokens[1])
    
        
def generate_gat_bnf(manager):
    identifier=Word(nums).setParseAction(parse_identifier)
    symbolic_name=(Word(alphanums+"_-[]/",alphanums+"_-[]/")).setParseAction(fix_symbol_name)
    meta=ZeroOrMore(
    Or(
    [
    CaselessLiteral("WRITER"),
    CaselessLiteral("DESIGN"),
    CaselessLiteral("ORIGIN")])+restOfLine)
    end=CaselessLiteral("END")
    and_=CaselessLiteral("AND")
    input_=CaselessLiteral("INPUT")
    state=CaselessLiteral("STATE")
    nstate=CaselessLiteral("NSTATE")
    output=CaselessLiteral("OUTPUT")
    buffer_=CaselessLiteral("BUFFER")
    identifier_ref=\
        (Optional(Literal("~"),default=0).setParseAction(
        manager.parse_sign)+identifier).setParseAction(
            manager.parse_idenfifier_ref)

    input_line=(identifier + input_ + symbolic_name).setParseAction(manager.parse_input_action)
    and_line=(identifier + and_ + identifier_ref+identifier_ref).setParseAction(manager.parse_and_action)
    buffer_line=(buffer_ +identifier_ref + symbolic_name).setParseAction(manager.parse_buffer_action)
    output_line=(output+ identifier_ref + symbolic_name).setParseAction(manager.parse_output_action)
    state_line=(identifier + state + symbolic_name).setParseAction(manager.parse_state_action)
    nstate_line=(nstate +identifier_ref + symbolic_name).setParseAction(manager.parse_next_state_action)
    assignment=Or([output_line,and_line,input_line,state_line,nstate_line, buffer_line])
    
    gat=meta+OneOrMore(assignment)+end
    return gat
generator=Random(123)
def parse(f, manager):
    f=open(f)
    content=f.read()
    bnf=generate_gat_bnf(manager)
    parsed=bnf.parseString(content)
    f.close()

def zero_fun():
    return 0

def random_bit():
    return generator.randint(0,1)

def format_grouped(l,group_size=10,indent=0):
    s=StringIO()
    s.write("[")
    last=len(l)-1
    for (i,e) in enumerate(l):
        if i%group_size==0:
            s.write("\n"+indent*" ")
        s.write(e)
        if i!=last:
            s.write(", ")
    s.write("]")
    return s.getvalue()

def generate_three_ideal_output(ideal_state, ideal_intermediate, ideal_next_state, variables):
    print "declare_ring("+format_grouped([repr(str(v)) for v in variables],indent=4) + ")"
    print "block_start_hints="+repr(block_starts)
    print "ideal_intermediate=["
    print ",\n".join((str(p) for p in ideal_intermediate))
    print "]"
    print "ideal_state=["
    print ",\n".join((str(p) for p in ideal_state))
    print "]"
    print "ideal_next_state=["
    print ",\n".join((str(p) for p in ideal_next_state))
    print "]"
    print "ideal=ideal_state+ideal_next_state+ideal_intermediate"

if __name__=='__main__':
    (options, args) = parser.parse_args()
    kwd_args=dict()
    declare_ring([
        "t",
        Block("x",gat_max),
        Block("xnext",next_max,reverse=True),
        Block("xoutput",output_max,reverse=True),
        Block("xinter",inter_max,reverse=True),
        Block("xinput",input_max,reverse=True),
        Block("xstate",state_max,reverse=True)],kwd_args)
    
    manager=VariableManager(include_outputs=options.include_outputs,
        initialize=options.initialize,
        **kwd_args
    )
   
    from sys import argv
    f=args[0]

    parse(f, manager)
    (ideal_state,ideal_intermediate, ideal_next_state)=manager.ideals()
    ideal = ideal_state + ideal_intermediate + ideal_next_state
    
    variables=[]
    used_vars=set(used_vars_set(ideal).variables())
    if not options.forward:
        variables=list(chain(reversed(manager.next),reversed(manager.output),reversed(manager.intermediate),reversed(manager.input),reversed(manager.state)))
    else:
        variables=list(
            chain(reversed(manager.output),
            reversed(manager.intermediate),
            reversed(manager.input),
            reversed(manager.state),
            reversed(manager.next)))
    variables=["t"]+variables
    beginnings=[str(v)[:1] for v in variables]
    block_starts=[]
    last=beginnings[0]
    
    for (i,s) in enumerate(beginnings):
        if s!=last:
            block_starts.append(i)
        last=s
    
    generate_three_ideal_output(ideal_state,ideal_intermediate, ideal_next_state, variables)


