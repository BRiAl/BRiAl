from polybori.PyPolyBoRi import *
saver=0
def term_sort_key(t):
    return max([e for e in t]+[-1])
def terms(f):
    res=[]
    while not f.isZero():
        lm=f.lead()
        f=f+lm
        res.append(lm)
        
        
    res.sort(key=term_sort_key)    
    #additionally we sort
    return res
def gen_split_equations(eqs, n, help_var_start):
    global saver
    eqs=[Polynomial(e) for e in eqs]
    hi=help_var_start
    for orig_p in eqs:
        orig_p.toStdOut()
        saver=orig_p
        p_l=terms(orig_p)#[t for t in orig_p]
        start=True
        
        while(len(p_l)>0):
            print hi
            res=Polynomial()
            if not start:
                res=res+Polynomial(Variable(hi))
                hi=hi+1
            if len(p_l)>n:
                res=res+Polynomial(Variable(hi))
            for t in p_l[:n]:
                res=res+t
            p_l=p_l[n:]
            yield(res)
            start=False
    print "ended"
            
def split_equations(eqs,n,help_var_start):
    return list(gen_split_equations(eqs,n, help_var_start))