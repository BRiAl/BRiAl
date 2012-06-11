#include <polybori/groebner/groebner.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <algorithm>

BEGIN_NAMESPACE_PBORIGB

typedef Polynomial::navigator nav_type;
class Evaluateable {
public:
  typedef std::vector< std::vector<Evaluateable*>  > evec_type;

    Evaluateable(idx_type idx = 0): _index(idx) { }

    virtual ~Evaluateable(){
        
    }
    virtual Polynomial evaluate(){
        
    }
    virtual BoolePolyRing ring(){
        
    }
    const idx_type& index() const {
        return _index;
    }
    virtual void push_back_nodes(evec_type & evec){
        
    }
protected:
    idx_type _index;
};


class IteEvaluation: public Evaluateable {
public:
    
    Polynomial evaluate(){
        if (res.isZero()){
            res = BooleSet(_index, then_branch->evaluate(), else_branch->evaluate());
        }
        return res;
    }
    IteEvaluation(idx_type v, boost::shared_ptr<Evaluateable> then_branch,
                  boost::shared_ptr<Evaluateable> else_branch): Evaluateable(v),
        res(then_branch->ring()), then_branch(then_branch), else_branch(else_branch)
    {
    
    }
    ~IteEvaluation(){
    }
    BoolePolyRing ring(){
        return then_branch -> ring();
    }
    void push_back_nodes(evec_type & evec){
        evec[index()].push_back(this);
        then_branch->push_back_nodes(evec);
        else_branch->push_back_nodes(evec);
    }
    
//     idx_type index() {
//         return _index;
//     }
protected:
    Polynomial res;

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
//     idx_type index() {
//         if (!(data.isConstant())){
//             return *(data.navigation());
//         }
//         else {
//             return ring().nVariables();
//         }
//     }
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
    ExponentData(const Exponent& e, const BoolePolyRing& r):
      Evaluateable(e.deg() > 0? *(data.begin()): r.nVariables()),
      data(e), _ring(r), converted(r){
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


boost::shared_ptr<Evaluateable>
    add_up_lex_sorted_monomials_delayed(const BoolePolyRing& ring,
                            std::vector<nav_type>& vec, int start, int end){
    PBORI_ASSERT(end<=vec.size());
    PBORI_ASSERT(start>=0);
    int d=end-start;
    PBORI_ASSERT(d>=0);
    if (d<=1){
        switch(d){
            case 0:
                return (boost::shared_ptr<Evaluateable>) new PolynomialData(Polynomial(ring));
        case 1:return (boost::shared_ptr<Evaluateable>) new PolynomialData(Polynomial(vec[start], ring));

        }


    }

    //more than two monomial, lex sorted, so if first is  constant, all are constant
    if (vec[start].isConstant()) return (boost::shared_ptr<Evaluateable>) new PolynomialData( Polynomial(end-start, ring));
    PBORI_ASSERT (!(vec[start].isConstant()));
    idx_type idx=*vec[start];
    int limes = end;
    vec[start] = vec[start].thenBranch();
    for(limes=start+1; limes<end; limes++){
        if (PBORI_UNLIKELY((vec[limes].isConstant()) || (*vec[limes] != idx))){
            PBORI_ASSERT((vec[limes].isConstant()) || (*vec[limes]>idx));
            break;
        } else 
            vec[limes]=vec[limes].thenBranch();
            //vec[limes].changeAssign(idx);
    }

    return (boost::shared_ptr<Evaluateable>) new IteEvaluation(idx, add_up_lex_sorted_monomials_delayed(ring, vec,start,limes),
                       add_up_lex_sorted_monomials_delayed(ring, vec,limes,end));
}

inline 
bool compare_evaluateables(Evaluateable* a, Evaluateable* b){
    return a->index()>b->index();
}

std::vector<Polynomial> translate_from_lex_sorted_monomials(std::vector<std::vector<nav_type> >& conversion_vector, BoolePolyRing ring){
    std::vector<boost::shared_ptr<Evaluateable> > delayed_polys;
    Evaluateable::evec_type nodes_collector(ring.nVariables()+1);
    int i;
    for (i=0; i< conversion_vector.size();i++){
        delayed_polys.push_back(add_up_lex_sorted_monomials_delayed(ring, conversion_vector[i], 0, conversion_vector[i].size()));
        delayed_polys.back()->push_back_nodes(nodes_collector);
    }
    //std::sort(nodes_collector.begin(), nodes_collector.end(),
    //compare_evaluateables);
    typedef Evaluateable::evec_type::const_reverse_iterator iter;
    for (iter start(nodes_collector.rbegin()), finish(nodes_collector.rend());
         start != finish; ++start){

      typedef Evaluateable::evec_type::value_type::const_iterator inner_iter;
      for (inner_iter istart(start->begin()), iend(start->end()); istart != iend;
                             ++istart){
        //results are cached, we just traverse the computation graph in correct order
        //should be good for caching
        //in particular parent children dependencies have to be respected
        (*istart)->evaluate();
      }
    }

    std::vector<Polynomial> poly_vec;
    poly_vec.reserve(delayed_polys.size());

    for(i=0;i<delayed_polys.size();i++){
        poly_vec.push_back(delayed_polys[i]->evaluate());
    }
    return poly_vec;
}

END_NAMESPACE_PBORIGB
