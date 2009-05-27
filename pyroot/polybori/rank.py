def input_signals(p):
    return list((p+p.lex_lead()).vars_as_monomial().variables())
def output_signal(p):
    return p.lead().variables().next()

def rank(data):
    parents=dict()
    res=dict()
    for p in data:
        out=output_signal(p)
        parents.setdefault(out, [])
        for v in input_signals(p):
            parents.setdefault(v, []).append(out)
    def do_rank(v):
        if v in res:
            return res[v]
        my_res = res[v] = max([do_rank(p)+1 for p in parents[v]]+ [0])
        return my_res
    for v in parents.keys():
        do_rank(v)
    return res

