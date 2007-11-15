if __name__=='__main__':
    import pathadjuster
from polybori.PyPolyBoRi import BooleSet,Variable,Polynomial,mod_mon_set
def all_monomials_of_degree_d(d,variable_indices):
    if d==0:
        return BooleSet(Polynomial(1).set())
    if len(variable_indices)==0:
        return BooleSet()
    variable_indices=list(reversed(list(set(variable_indices))))

    m=Variable(variable_indices[-1])
    for v in variable_indices[:-1]:
        m=Variable(v)+m
    m=Polynomial(m).set()
    def do_all_monomials(d):
        if d==0:
            return BooleSet(Polynomial(1).set())
        else:
            prev=do_all_monomials(d-1)
            return BooleSet(prev.unateProduct(m).diff(prev))
    return do_all_monomials(d)
def power_set(variable_indices):
    variable_indices=list(reversed(list(set(variable_indices))))
    res=BooleSet(Polynomial(1).set())
    for v in variable_indices:
        res=res.change(v).union(res)
    return BooleSet(res) 
if __name__=='__main__':
    from blocks import declare_ring,Block
    r=declare_ring([Block("x",10000)],globals())
    print list(all_monomials_of_degree_d(0,range(100)))
    print list(all_monomials_of_degree_d(1,range(10)))
    print list(all_monomials_of_degree_d(2,range(4)))
    print list(all_monomials_of_degree_d(3,range(4)))
    print list(all_monomials_of_degree_d(4,range(4)))
    print list(all_monomials_of_degree_d(0,[]))
    print list(all_monomials_of_degree_d(1,[]))
    print list(power_set(range(2)))
    print list(power_set(range(4)))
    print list(power_set([]))
    #every monomial in the first 8 var, which is at most linear in the first 5
    print list(mod_mon_set(power_set(range(8)),all_monomials_of_degree_d(2,range(5))))
    
    #specialized normal form computation
    print Polynomial(
        mod_mon_set(
            BooleSet((x(1)*x(2)+x(1)+1).set()),
            all_monomials_of_degree_d(2,range(1000))))
    print list(mod_mon_set(power_set(range(50)),all_monomials_of_degree_d(2,range(1000))))
