#include <polybori/groebner/groebner.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <algorithm>

BEGIN_NAMESPACE_PBORIGB

class Evaluateable {
public:
    typedef std::vector<Evaluateable*> evec_type;
    
    virtual ~Evaluateable(){
        
    }
    virtual Polynomial evaluate(){
        
    }
    virtual BoolePolyRing ring(){
        
    }
    virtual idx_type index(){
        
    }
    virtual void push_back_nodes(evec_type & evec){
        
    }
};


class IteEvaluation: public Evaluateable {
public:
    
    Polynomial evaluate(){
        if (res.isZero()){
            res = BooleSet(_index, then_branch->evaluate(), else_branch->evaluate());
        }
        return res;
    }
    IteEvaluation(idx_type v, boost::shared_ptr<Evaluateable> then_branch, boost::shared_ptr<Evaluateable> else_branch):
        res(then_branch->ring()), then_branch(then_branch), else_branch(else_branch), _index(v)
    {
    
    }
    ~IteEvaluation(){
    }
    BoolePolyRing ring(){
        return then_branch -> ring();
    }
    void push_back_nodes(evec_type & evec){
        evec.push_back(this);
        then_branch->push_back_nodes(evec);
        else_branch->push_back_nodes(evec);
    }
    
    idx_type index() {
        return _index;
    }
protected:
    Polynomial res;
    idx_type _index;
    boost::shared_ptr<Evaluateable> then_branch;
    boost::shared_ptr<Evaluateable> else_branch;
    
};
class PolynomialData: public Evaluateable {
public:
    Polynomial evaluate(){
        return data;
    }
    BoolePolyRing ring(){
        return data.ring();
    }
    PolynomialData(Polynomial p):data(p){
    }
    idx_type index() {
        if (!(data.isConstant())){
            return *(data.navigation());
        }
        else {
            return ring().nVariables();
        }
    }
protected:
    Polynomial data;
    
};

class ExponentData: public Evaluateable {
public:
    Polynomial evaluate(){
        if (converted.isZero())
            converted=Monomial(data, _ring);
        return converted;
    }
    BoolePolyRing ring(){
        return _ring;
    }
    ExponentData(Exponent e, BoolePolyRing r):data(e), _ring(r), converted(r){
    }
    idx_type index(){
        if (data.deg()>0)
            return *(data.begin());
        else
            return _ring.nVariables();
    }
protected:
    Exponent data;
    Polynomial converted;
    BoolePolyRing _ring;
};

boost::shared_ptr<Evaluateable>
    add_up_lex_sorted_exponents_delayed(const BoolePolyRing& ring,
                            std::vector<Exponent>& vec, int start, int end){
    PBORI_ASSERT(end<=vec.size());
    PBORI_ASSERT(start>=0);
    int d=end-start;
    PBORI_ASSERT(d>=0);
    if (d<=1){
        switch(d){
            case 0:
                return (boost::shared_ptr<Evaluateable>) new PolynomialData(Polynomial(ring));
        case 1:return (boost::shared_ptr<Evaluateable>) new PolynomialData(Monomial(vec[start], ring));

        }

    
    }
    
    //more than two monomial, lex sorted, so if first is  constant, all are constant
    if (vec[start].deg()==0) return (boost::shared_ptr<Evaluateable>) new PolynomialData( Polynomial(end-start, ring));
    PBORI_ASSERT (!(vec[start].deg()==0));
    idx_type idx=*vec[start].begin();
    int limes=end;
    vec[start].popFirst();
    for(limes=start+1;limes<end;limes++){
        if (PBORI_UNLIKELY((vec[limes].deg()==0)||(*vec[limes].begin()!=idx))){
            PBORI_ASSERT((vec[limes].deg()==0)||(*vec[limes].begin()>idx));
            break;
        } else 
           vec[limes].popFirst();
            //vec[limes].changeAssign(idx);
    }
    
    return (boost::shared_ptr<Evaluateable>) new IteEvaluation(idx, add_up_lex_sorted_exponents_delayed(ring, vec,start,limes),
                       add_up_lex_sorted_exponents_delayed(ring, vec,limes,end));
}

bool compare_evaluateables(Evaluateable* a, Evaluateable* b){
    return a->index()>b->index();
}

std::vector<Polynomial> translate_from_lex_sorted_exponent_vectors(std::vector<std::vector<Exponent> >& conversion_vector, BoolePolyRing ring){
    std::vector<boost::shared_ptr<Evaluateable> > delayed_polys;
    Evaluateable::evec_type nodes_collector;
    int i;
    for (i=0; i< conversion_vector.size();i++){
        delayed_polys.push_back(add_up_lex_sorted_exponents_delayed(ring, conversion_vector[i], 0, conversion_vector[i].size()));
        delayed_polys.back()->push_back_nodes(nodes_collector);
    }
    std::sort(nodes_collector.begin(), nodes_collector.end(), compare_evaluateables);
    for (i=0; i < nodes_collector.size();i++){
        // std::cout<<nodes_collector[i]->index()<<std::endl;
        //results are cached, we just traverse the computation graph in correct order
        //should be good for caching
        //in particular parent children dependencies have to be respected
        nodes_collector[i]->evaluate();
    }
    std::vector<Polynomial> poly_vec;
    poly_vec.reserve(delayed_polys.size());
    for(i=0;i<delayed_polys.size();i++){
        poly_vec.push_back(delayed_polys[i]->evaluate());
    }
    return poly_vec;
}
END_NAMESPACE_PBORIGB
