#!/usr/bin/env python
# encoding: utf-8
"""
cluster.py

Created by Michael Brickenstein on 2011-08-05.
Copyright 2011 The PolyBoRi Team. See LICENSE file.
"""

import sys
import os
from statistics import used_vars, used_vars_set


def main():
    pass

class ClusterAlgorithmFailed(Exception):
    pass

class ClusterAlgorithm(object):
    def __init__(self, ideal, determination_modifier=1):
        if len(ideal) == 0:
            raise ValueError, 'ideal generators list should be non empty'
        
        self.ideal = ideal
        
        self.determination_modifier = determination_modifier
        
        self.used_variables_ideal = used_vars_set(ideal)
        
        self.number_of_used_variables_in_ideal = len(self.used_variables_ideal)
        self.used_variables_of_polynomial = dict(
            [(p, set(p.vars_as_monomial().variables()))])
        self.variables_introduction_mapping = dict()
        
        def varset_to_tuple(s):
            return tuple(sorted(s, key = Variable.index))
        
        self.cluster = []
        self.used_variables_cluster = set()
        self.build_variables_usage()
        self.initialize_variables_introduction_mapping()

    def build_variables_usage(self):
        self.variables_usage=dict()
        for (p, variables) in self.used_variables_of_polynomial:
            for v in variables:
                self.variables_usage.setdefault(v, []).append(p)
                
    def initialize_variables_introduction_mapping(self):
        self._build_variables_introduction_mapping()
        
    def _build_variables_introduction_mapping(self):
        self.variables_introduction_mapping.clear()
        for (p, var_set) in self.used_variables_of_polynomial:
            as_tuple = varset_to_tuple(var_set.difference(self.used_variables_cluster))
            self.variables_introduction_mapping.setdefault(
                as_tuple, []).append(p)
    def adjust_variables_introduction_mapping(self, introduced_variables):
        self._build_variables_introduction_mapping()

    def determined_enough(self):
        return self.number_of_used_variables_in_cluster>=self.determination_modifier+len(self.cluster)

    def find_cluster(self):
        p = self.initial_choice()
        self.cluster = []
        self.add_polynomial_to_cluster(p)
    
    def add_polynomial_to_cluster(self, p):
        self.cluster.append(p)
        self.used_variables_cluster = used_vars_set(cluster)
        self.number_of_used_variables_in_cluster = len(self.used_variables_cluster)
        self.adjust_variables_introduction_mapping(self.used_variables_of_polynomial[p])
        
    def initial_choice(self):
        def max_key(entry):
            (entry_variable, entry_polynomials)=entry
            return len(entry_polynomials)
        (variable, polynomials)=max(self.variables_usage, key=max_key)
        def min_key(p):
            return len(self.used_variables_of_polynomial[p])
        return min(polynomials, key=min_key)
    def increase_cluster(self):
        introduced_variables_possibilities=self.variables_introduction_mapping.keys()
        introduced_variables=min(introduced_variables_possibilities, key=len)
        polynomials=variables_introduction_mapping[introduced_variables]
        assert len(polynomials)>0
        for p in polynomials:
            self.add_polynomial_to_cluster(p)

def find_cluster(ideal):
    def initial_choice(ideal):
        return ideal[0]

    def increase_cluster():

    
    if len(ideal) == 0:
        return None
    ring = ideal[0].ring()

    p = initial_choice(ideal)
    cluster = [p]
    while not determined_enough():
        increase_cluster()
    

if __name__ == '__main__':
    main()

