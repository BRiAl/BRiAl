/**CFile***********************************************************************

  FileName    [cuddObj.cc]

  PackageName [cuddObj]

  Synopsis    [Functions for the C++ object-oriented encapsulation of CUDD.]

  Description []

  SeeAlso     []

  Author      [Fabio Somenzi]

  Copyright   [Copyright (c) 1995-2004, Regents of the University of Colorado

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  Neither the name of the University of Colorado nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.]

******************************************************************************/

#include "cuddObj.hh"

// ---------------------------------------------------------------------------
// Variable declarations
// ---------------------------------------------------------------------------

#ifndef lint
static char rcsid[] UTIL_UNUSED = "$Id$";
#endif

// ---------------------------------------------------------------------------
// Members of class DD
// ---------------------------------------------------------------------------


DD::DD(Cudd *ddManager, DdNode *ddNode) {
    ddMgr = ddManager;
    node = ddNode;
    if (node != 0) Cudd_Ref(node);
    if (ddMgr->isVerbose()) {
        cout << "Standard DD constructor for node " << hex << long(node) <<
	    " ref = " << Cudd_Regular(node)->ref << "\n";
    }

} // DD::DD


DD::DD() {
    ddMgr = 0;
    node = 0;

} // DD::DD


DD::DD(const DD &from) {
    ddMgr = from.ddMgr;
    node = from.node;
    if (node != 0) {
	Cudd_Ref(node);
	if (ddMgr->isVerbose()) {
	    cout << "Copy DD constructor for node " << hex << long(node) <<
		" ref = " << Cudd_Regular(node)->ref << "\n";
	}
    }

} // DD::DD


inline DdManager *
DD::checkSameManager(
  const DD &other) const
{
    DdManager *mgr = ddMgr->p->manager;
    if (mgr != other.ddMgr->p->manager) {
	ddMgr->p->errorHandler("Operands come from different manager.");
    }
    return mgr;

} // DD::checkSameManager


inline void
DD::checkReturnValue(
  const DdNode *result) const
{
    if (result == 0) {
	DdManager *mgr = ddMgr->p->manager;
	Cudd_ErrorType errType = Cudd_ReadErrorCode(mgr);
	switch (errType) {
	case CUDD_MEMORY_OUT:
	    ddMgr->p->errorHandler("Out of memory.");
	    break;
	case CUDD_TOO_MANY_NODES:
	    break;
	case CUDD_MAX_MEM_EXCEEDED:
	    ddMgr->p->errorHandler("Maximum memory exceeded.");
	    break;
	case CUDD_INVALID_ARG:
	    ddMgr->p->errorHandler("Invalid argument.");
	    break;
	case CUDD_INTERNAL_ERROR:
	    ddMgr->p->errorHandler("Internal error.");
	    break;
	case CUDD_NO_ERROR:
	default:
	    ddMgr->p->errorHandler("Unexpected error.");
	    break;
	}
    }

} // DD::checkReturnValue


inline void
DD::checkReturnValue(
  const int result,
  const int expected) const
{
    if (result != expected) {
	DdManager *mgr = ddMgr->p->manager;
	Cudd_ErrorType errType = Cudd_ReadErrorCode(mgr);
	switch (errType) {
	case CUDD_MEMORY_OUT:
	    ddMgr->p->errorHandler("Out of memory.");
	    break;
	case CUDD_TOO_MANY_NODES:
	    break;
	case CUDD_MAX_MEM_EXCEEDED:
	    ddMgr->p->errorHandler("Maximum memory exceeded.");
	    break;
	case CUDD_INVALID_ARG:
	    ddMgr->p->errorHandler("Invalid argument.");
	    break;
	case CUDD_INTERNAL_ERROR:
	    ddMgr->p->errorHandler("Internal error.");
	    break;
	case CUDD_NO_ERROR:
	default:
	    ddMgr->p->errorHandler("Unexpected error.");
	    break;
	}
    }

} // DD::checkReturnValue


Cudd *
DD::manager() const
{
    return ddMgr;

} // DD::manager

// moved to header file
/*
inline DdNode *
DD::getNode() const
{
    return node;

} // DD::getNode
*/

int
DD::nodeCount() const
{
    return Cudd_DagSize(node);

} // DD::nodeCount


// ---------------------------------------------------------------------------
// Members of class ABDD
// ---------------------------------------------------------------------------


ABDD::ABDD(Cudd *bddManager, DdNode *bddNode) : DD(bddManager,bddNode) {}
ABDD::ABDD() : DD() {}
ABDD::ABDD(const ABDD &from) : DD(from) {}


ABDD::~ABDD() {
    if (node != 0) {
	Cudd_RecursiveDeref(ddMgr->p->manager,node);
	if (ddMgr->isVerbose()) {
	    cout << "ADD/BDD destructor called for node " << hex <<
		long(node) << " ref = " << Cudd_Regular(node)->ref << "\n";
	}
    }

} // ABDD::~ABDD


int
ABDD::operator==(
  const ABDD& other) const
{
    this->checkSameManager(other);
    return node == other.node;

} // ABDD::operator==


int
ABDD::operator!=(
  const ABDD& other) const
{
    this->checkSameManager(other);
    return node != other.node;

} // ABDD::operator!=


void
ABDD::print(
  int nvars,
  int verbosity) const
{
    cout.flush();
    int retval = Cudd_PrintDebug(ddMgr->p->manager,node,nvars,verbosity);
    if (retval == 0) ddMgr->p->errorHandler("print failed");

} // ABDD::print


// ---------------------------------------------------------------------------
// Members of class BDD
// ---------------------------------------------------------------------------

BDD::BDD(Cudd *bddManager, DdNode *bddNode) : ABDD(bddManager,bddNode) {}
BDD::BDD() : ABDD() {}
BDD::BDD(const BDD &from) : ABDD(from) {}


int
BDD::operator==(
  const BDD& other) const
{
    this->checkSameManager(other);
    return node == other.node;

} // BDD::operator==


int
BDD::operator!=(
  const BDD& other) const
{
    this->checkSameManager(other);
    return node != other.node;

} // BDD::operator!=


BDD
BDD::operator=(
  const BDD& right)
{
    if (this == &right) return *this;
    if (right.node != 0) Cudd_Ref(right.node);
    if (node != 0) {
	Cudd_RecursiveDeref(ddMgr->p->manager,node);
	if (ddMgr->isVerbose()) {
	    cout << "BDD dereferencing for node " << hex << long(node) <<
		" ref = " << Cudd_Regular(node)->ref << "\n";
	}
    }
    node = right.node;
    ddMgr = right.ddMgr;
    if (node != 0 && ddMgr->isVerbose()) {
	cout << "BDD assignment for node " << hex << long(node) <<
	    " ref = " << Cudd_Regular(node)->ref << "\n";
    }
    return *this;

} // BDD::operator=


int
BDD::operator<=(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return Cudd_bddLeq(mgr,node,other.node);

} // BDD::operator<=


int
BDD::operator>=(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return Cudd_bddLeq(mgr,other.node,node);

} // BDD::operator>=


int
BDD::operator<(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return node != other.node && Cudd_bddLeq(mgr,node,other.node);

} // BDD::operator<


int
BDD::operator>(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return node != other.node && Cudd_bddLeq(mgr,other.node,node);

} // BDD::operator>


BDD
BDD::operator!() const
{
    return BDD(ddMgr, Cudd_Not(node));

} // BDD::operator!


BDD
BDD::operator~() const
{
    return BDD(ddMgr, Cudd_Not(node));

} // BDD::operator~


BDD
BDD::operator*(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddAnd(mgr,node,other.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::operator*


BDD
BDD::operator*=(
  const BDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddAnd(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // BDD::operator*=


BDD
BDD::operator&(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddAnd(mgr,node,other.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::operator&


BDD
BDD::operator&=(
  const BDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddAnd(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // BDD::operator&=


BDD
BDD::operator+(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddOr(mgr,node,other.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::operator+


BDD
BDD::operator+=(
  const BDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddOr(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // BDD::operator+=


BDD
BDD::operator|(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddOr(mgr,node,other.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::operator|


BDD
BDD::operator|=(
  const BDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddOr(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // BDD::operator|=


BDD
BDD::operator^(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddXor(mgr,node,other.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::operator^


BDD
BDD::operator^=(
  const BDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddXor(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // BDD::operator^=


BDD
BDD::operator-(
  const BDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddAnd(mgr,node,Cudd_Not(other.node));
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::operator-


BDD
BDD::operator-=(
  const BDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_bddAnd(mgr,node,Cudd_Not(other.node));
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // BDD::operator-=


// ---------------------------------------------------------------------------
// Members of class ADD
// ---------------------------------------------------------------------------


ADD::ADD(Cudd *bddManager, DdNode *bddNode) : ABDD(bddManager,bddNode) {}
ADD::ADD() : ABDD() {}
ADD::ADD(const ADD &from) : ABDD(from) {}


int
ADD::operator==(
  const ADD& other) const
{
    this->checkSameManager(other);
    return node == other.node;

} // ADD::operator==


int
ADD::operator!=(
  const ADD& other) const
{
    this->checkSameManager(other);
    return node != other.node;

} // ADD::operator!=


ADD
ADD::operator=(
  const ADD& right)
{
    if (this == &right) return *this;
    if (right.node != 0) Cudd_Ref(right.node);
    if (node != 0) {
	Cudd_RecursiveDeref(ddMgr->p->manager,node);
    }
    node = right.node;
    ddMgr = right.ddMgr;
    return *this;

} // ADD::operator=


int
ADD::operator<=(
  const ADD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return Cudd_addLeq(mgr,node,other.node);

} // ADD::operator<=


int
ADD::operator>=(
  const ADD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return Cudd_addLeq(mgr,other.node,node);

} // ADD::operator>=


int
ADD::operator<(
  const ADD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return node != other.node && Cudd_addLeq(mgr,node,other.node);

} // ADD::operator<


int
ADD::operator>(
  const ADD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return node != other.node && Cudd_addLeq(mgr,other.node,node);

} // ADD::operator>


ADD
ADD::operator-() const
{
    return ADD(ddMgr, Cudd_addNegate(ddMgr->p->manager,node));

} // ADD::operator-


ADD
ADD::operator*(
  const ADD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addTimes,node,other.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::operator*


ADD
ADD::operator*=(
  const ADD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addTimes,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // ADD::operator*=


ADD
ADD::operator+(
  const ADD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addPlus,node,other.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::operator+


ADD
ADD::operator+=(
  const ADD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addPlus,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // ADD::operator+=


ADD
ADD::operator-(
  const ADD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addMinus,node,other.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::operator-


ADD
ADD::operator-=(
  const ADD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addMinus,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // ADD::operator-=


ADD
ADD::operator~() const
{
    return ADD(ddMgr, Cudd_addCmpl(ddMgr->p->manager,node));

} // ADD::operator~


ADD
ADD::operator&(
  const ADD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addTimes,node,other.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::operator&


ADD
ADD::operator&=(
  const ADD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addTimes,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // ADD::operator&=


ADD
ADD::operator|(
  const ADD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addOr,node,other.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::operator|


ADD
ADD::operator|=(
  const ADD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_addApply(mgr,Cudd_addOr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDeref(mgr,node);
    node = result;
    return *this;

} // ADD::operator|=


// ---------------------------------------------------------------------------
// Members of class ZDD
// ---------------------------------------------------------------------------


ZDD::ZDD(Cudd *bddManager, DdNode *bddNode) : DD(bddManager,bddNode) {}
ZDD::ZDD() : DD() {}
ZDD::ZDD(const ZDD &from) : DD(from) {}


ZDD::~ZDD() {
    if (node != 0) {
	Cudd_RecursiveDerefZdd(ddMgr->p->manager,node);
	if (ddMgr->isVerbose()) {
	    cout << "ZDD destructor called for node " << hex << long(node) <<
		" ref = " << Cudd_Regular(node)->ref << "\n";
	}
    }

} // ZDD::~ZDD


ZDD
ZDD::operator=(
  const ZDD& right)
{
    if (this == &right) return *this;
    if (right.node != 0) Cudd_Ref(right.node);
    if (node != 0) {
	Cudd_RecursiveDerefZdd(ddMgr->p->manager,node);
	if (ddMgr->isVerbose()) {
	    cout << "ZDD dereferencing for node " << hex << long(node) <<
		" ref = " << node->ref << "\n";
	}
    }
    node = right.node;
    ddMgr = right.ddMgr;
    if (node != 0 && ddMgr->isVerbose()) {
	cout << "ZDD assignment for node " << hex << long(node) <<
	    " ref = " << node->ref << "\n";
    }
    return *this;

} // ZDD::operator=


int
ZDD::operator==(
  const ZDD& other) const
{
    this->checkSameManager(other);
    return node == other.node;

} // ZDD::operator==


int
ZDD::operator!=(
  const ZDD& other) const
{
    this->checkSameManager(other);
    return node != other.node;

} // ZDD::operator!=


int
ZDD::operator<=(
  const ZDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return Cudd_zddDiffConst(mgr,node,other.node) == Cudd_ReadZero(mgr);

} // ZDD::operator<=


int
ZDD::operator>=(
  const ZDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return Cudd_zddDiffConst(mgr,other.node,node) == Cudd_ReadZero(mgr);

} // ZDD::operator>=


int
ZDD::operator<(
  const ZDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return node != other.node &&
	Cudd_zddDiffConst(mgr,node,other.node) == Cudd_ReadZero(mgr);

} // ZDD::operator<


int
ZDD::operator>(
  const ZDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    return node != other.node &&
	Cudd_zddDiffConst(mgr,other.node,node) == Cudd_ReadZero(mgr);

} // ZDD::operator>


void
ZDD::print(
  int nvars,
  int verbosity) const
{
    cout.flush();
    int retval = Cudd_zddPrintDebug(ddMgr->p->manager,node,nvars,verbosity);
    if (retval == 0) ddMgr->p->errorHandler("print failed");

} // ZDD::print


ZDD
ZDD::operator*(
  const ZDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddIntersect(mgr,node,other.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::operator*


ZDD
ZDD::operator*=(
  const ZDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddIntersect(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDerefZdd(mgr,node);
    node = result;
    return *this;

} // ZDD::operator*=


ZDD
ZDD::operator&(
  const ZDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddIntersect(mgr,node,other.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::operator&


ZDD
ZDD::operator&=(
  const ZDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddIntersect(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDerefZdd(mgr,node);
    node = result;
    return *this;

} // ZDD::operator&=


ZDD
ZDD::operator+(
  const ZDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddUnion(mgr,node,other.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::operator+


ZDD
ZDD::operator+=(
  const ZDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddUnion(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDerefZdd(mgr,node);
    node = result;
    return *this;

} // ZDD::operator+=


ZDD
ZDD::operator|(
  const ZDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddUnion(mgr,node,other.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::operator|


ZDD
ZDD::operator|=(
  const ZDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddUnion(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDerefZdd(mgr,node);
    node = result;
    return *this;

} // ZDD::operator|=


ZDD
ZDD::operator-(
  const ZDD& other) const
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddDiff(mgr,node,other.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::operator-


ZDD
ZDD::operator-=(
  const ZDD& other)
{
    DdManager *mgr = this->checkSameManager(other);
    DdNode *result = Cudd_zddDiff(mgr,node,other.node);
    this->checkReturnValue(result);
    Cudd_Ref(result);
    Cudd_RecursiveDerefZdd(mgr,node);
    node = result;
    return *this;

} // ZDD::operator-=


// ---------------------------------------------------------------------------
// Members of class Cudd
// ---------------------------------------------------------------------------


Cudd::Cudd(
  unsigned int numVars,
  unsigned int numVarsZ,
  unsigned int numSlots,
  unsigned int cacheSize,
  unsigned long maxMemory)
{
    p = new capsule;
    p->manager = Cudd_Init(numVars,numVarsZ,numSlots,cacheSize,maxMemory);
    p->errorHandler = defaultError;
    p->verbose = 0;		// initially terse
    p->ref = 1;

} // Cudd::Cudd


Cudd::Cudd(
  Cudd& x)
{
    p = x.p;
    x.p->ref++;

} // Cudd::Cudd


Cudd::~Cudd()
{
    if (--p->ref == 0) {
	int retval = Cudd_CheckZeroRef(p->manager);
	if (retval != 0) {
	    cerr << retval << " unexpected non-zero reference counts\n";
	} else if (p->verbose) {
	    cerr << "All went well\n";
	}
	Cudd_Quit(p->manager);
	delete p;
    }

} // Cudd::~Cudd


Cudd&
Cudd::operator=(
  const Cudd& right)
{
    right.p->ref++;
    if (--p->ref == 0) {	// disconnect self
	int retval = Cudd_CheckZeroRef(p->manager);
	if (retval != 0) {
	    cerr << retval << " unexpected non-zero reference counts\n";
	}
	Cudd_Quit(p->manager);
	delete p;
    }
    p = right.p;
    return *this;

} // Cudd::operator=


PFC
Cudd::setHandler(
  PFC newHandler)
{
    PFC oldHandler = p->errorHandler;
    p->errorHandler = newHandler;
    return oldHandler;

} // Cudd::setHandler


PFC
Cudd::getHandler() const
{
    return p->errorHandler;

} // Cudd::getHandler


inline void
Cudd::checkReturnValue(
  const DdNode *result) const
{
    if (result == 0) {
	if (Cudd_ReadErrorCode(p->manager) == CUDD_MEMORY_OUT) {
	    p->errorHandler("Out of memory.");
	} else {
	    p->errorHandler("Internal error.");
	}
    }

} // Cudd::checkReturnValue


inline void
Cudd::checkReturnValue(
  const int result) const
{
    if (result == 0) {
	if (Cudd_ReadErrorCode(p->manager) == CUDD_MEMORY_OUT) {
	    p->errorHandler("Out of memory.");
	} else {
	    p->errorHandler("Internal error.");
	}
    }

} // Cudd::checkReturnValue


void
Cudd::info() const
{
    cout.flush();
    int retval = Cudd_PrintInfo(p->manager,stdout);
    this->checkReturnValue(retval);

} // Cudd::info


BDD
Cudd::bddVar()
{
    DdNode *result = Cudd_bddNewVar(p->manager);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::bddVar


BDD
Cudd::bddVar(
  int index)
{
    DdNode *result = Cudd_bddIthVar(p->manager,index);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::bddVar


BDD
Cudd::bddOne()
{
    DdNode *result = Cudd_ReadOne(p->manager);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::bddOne


BDD
Cudd::bddZero()
{
    DdNode *result = Cudd_ReadLogicZero(p->manager);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::bddZero


ADD
Cudd::addVar()
{
    DdNode *result = Cudd_addNewVar(p->manager);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::addVar


ADD
Cudd::addVar(
  int index)
{
    DdNode *result = Cudd_addIthVar(p->manager,index);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::addVar


ADD
Cudd::addOne()
{
    DdNode *result = Cudd_ReadOne(p->manager);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::addOne


ADD
Cudd::addZero()
{
    DdNode *result = Cudd_ReadZero(p->manager);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::addZero


ADD
Cudd::constant(
  CUDD_VALUE_TYPE c)
{
    DdNode *result = Cudd_addConst(p->manager, c);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::constant


ADD
Cudd::plusInfinity()
{
    DdNode *result = Cudd_ReadPlusInfinity(p->manager);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::plusInfinity


ADD
Cudd::minusInfinity()
{
    DdNode *result = Cudd_ReadMinusInfinity(p->manager);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::minusInfinity


ZDD
Cudd::zddVar(
  int index)
{
    DdNode *result = Cudd_zddIthVar(p->manager,index);
    this->checkReturnValue(result);
    return ZDD(this, result);

} // Cudd::zddVar


ZDD
Cudd::zddOne(
  int i)
{
    DdNode *result = Cudd_ReadZddOne(p->manager,i);
    this->checkReturnValue(result);
    return ZDD(this, result);

} // Cudd::zddOne


ZDD
Cudd::zddZero()
{
    DdNode *result = Cudd_ReadZero(p->manager);
    this->checkReturnValue(result);
    return ZDD(this, result);

} // Cudd::zddZero


void
defaultError(
  string message)
{
    cerr << message << "\n";
    exit(1);

} // defaultError


// ---------------------------------------------------------------------------
// Members of class BDDvector
// ---------------------------------------------------------------------------


BDDvector::BDDvector(int size, Cudd *manager, DdNode **nodes)
{
    if (manager == 0 && nodes != 0) defaultError("Nodes with no manager");
    p = new capsule;
    p->size = size;
    p->manager = manager;
    p->vect = new BDD[size];
    p->ref = 1;
    for (int i = 0; i < size; i++) {
	if (nodes == 0) {
	    p->vect[i] = BDD();
	} else {
	    p->vect[i] = BDD(manager, nodes[i]);
	}
    }
    if (manager != 0 && manager->isVerbose()) {
	cout << "Standard BDDvector constructor for vector " << hex << long(p)
	     << "\n";
    }

} // BDDvector::BDDvector


BDDvector::BDDvector(const BDDvector &from)
{
    p = from.p;
    p->ref++;
    if (p->manager != 0 && p->manager->isVerbose()) {
	cout << "Copy BDDvector constructor for vector " << hex << long(p)
	     << "\n";
    }

} // BDDvector::BDDvector


BDDvector::~BDDvector()
{
    if (p->manager != 0 && p->manager->isVerbose()) {
	cout << "BDDvector destructor for vector " << hex << long(p) <<
	    " ref = " << p->ref << "\n";
    }
    if (--p->ref == 0) {
	delete [] p->vect;
	delete p;
    }

} // BDDvector::~BDDvector


BDDvector&
BDDvector::operator=(
  const BDDvector& right)
{
    right.p->ref++;
    if (--p->ref == 0) {	// disconnect self
	for (int i = 1; i < p->size; i++) {
	    delete &(p->vect[i]);
	}
	delete p->vect;
	delete p;
    }
    p = right.p;
    return *this;

} // BDDvector::operator=


BDD&
BDDvector::operator[](int i) const
{
    if (i >= p->size)
	(p->manager->getHandler())("Out-of-bounds access attempted");
    return p->vect[i];

} // BDDvector::operator[]


// ---------------------------------------------------------------------------
// Members of class ADDvector
// ---------------------------------------------------------------------------


ADDvector::ADDvector(int size, Cudd *manager, DdNode **nodes)
{
    if (manager == 0 && nodes != 0) defaultError("Nodes with no manager");
    p = new capsule;
    p->size = size;
    p->manager = manager;
    p->vect = new ADD[size];
    p->ref = 1;
    for (int i = 0; i < size; i++) {
	if (nodes == 0) {
	    p->vect[i] = ADD();
	} else {
	    p->vect[i] = ADD(manager, nodes[i]);
	}
    }
    if (manager != 0 && manager->isVerbose()) {
	cout << "Standard ADDvector constructor for vector " << hex << long(p)
	     << "\n";
    }

} // ADDvector::ADDvector


ADDvector::ADDvector(const ADDvector &from)
{
    p = from.p;
    p->ref++;
    if (p->manager != 0 && p->manager->isVerbose()) {
	cout << "Copy ADDvector constructor for vector " << hex << long(p)
	     << "\n";
    }

} // ADDvector::ADDvector


ADDvector::~ADDvector()
{
    if (p->manager != 0 && p->manager->isVerbose()) {
	cout << "ADDvector destructor for vector " << hex << long(p) <<
	    " ref = " << p->ref << "\n";
    }
    if (--p->ref == 0) {
	delete [] p->vect;
	delete p;
    }

} // ADDvector::~ADDvector


ADDvector&
ADDvector::operator=(
  const ADDvector& right)
{
    right.p->ref++;
    if (--p->ref == 0) {	// disconnect self
	for (int i = 1; i < p->size; i++) {
	    delete &(p->vect[i]);
	}
	delete p->vect;
	delete p;
    }
    p = right.p;
    return *this;

} // ADDvector::operator=


ADD&
ADDvector::operator[](int i) const
{
    if (i >= p->size)
	(p->manager->getHandler())("Out-of-bounds access attempted");
    return p->vect[i];

} // ADDvector::operator[]


// ---------------------------------------------------------------------------
// Members of class ZDDvector
// ---------------------------------------------------------------------------


ZDDvector::ZDDvector(int size, Cudd *manager, DdNode **nodes)
{
    if (manager == 0 && nodes != 0) defaultError("Nodes with no manager");
    p = new capsule;
    p->size = size;
    p->manager = manager;
    p->vect = new ZDD[size];
    p->ref = 1;
    for (int i = 0; i < size; i++) {
	if (nodes == 0) {
	    p->vect[i] = ZDD();
	} else {
	    p->vect[i] = ZDD(manager, nodes[i]);
	}
    }
    if (manager != 0 && manager->isVerbose()) {
	cout << "Standard ZDDvector constructor for vector " << hex << long(p)
	     << "\n";
    }

} // ZDDvector::ZDDvector


ZDDvector::ZDDvector(const ZDDvector &from)
{
    p = from.p;
    p->ref++;
    if (p->manager != 0 && p->manager->isVerbose()) {
	cout << "Copy ZDDvector constructor for vector " << hex << long(p)
	     << "\n";
    }

} // ZDDvector::ZDDvector


ZDDvector::~ZDDvector()
{
    if (p->manager != 0 && p->manager->isVerbose()) {
	cout << "ZDDvector destructor for vector " << hex << long(p) <<
	    " ref = " << p->ref << "\n";
    }
    if (--p->ref == 0) {
	delete [] p->vect;
	delete p;
    }

} // ZDDvector::~ZDDvector


ZDDvector&
ZDDvector::operator=(
  const ZDDvector& right)
{
    right.p->ref++;
    if (--p->ref == 0) {	// disconnect self
	for (int i = 1; i < p->size; i++) {
	    delete &(p->vect[i]);
	}
	delete p->vect;
	delete p;
    }
    p = right.p;
    return *this;

} // ZDDvector::operator=


ZDD&
ZDDvector::operator[](int i) const
{
    if (i >= p->size)
	(p->manager->getHandler())("Out-of-bounds access attempted");
    return p->vect[i];

} // ZDDvector::operator[]


// ---------------------------------------------------------------------------
// All the rest
// ---------------------------------------------------------------------------


ADD
Cudd::addNewVarAtLevel(
  int level)
{
    DdNode *result = Cudd_addNewVarAtLevel(p->manager, level);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::addNewVarAtLevel


BDD
Cudd::bddNewVarAtLevel(
  int level)
{
    DdNode *result = Cudd_bddNewVarAtLevel(p->manager, level);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::bddNewVarAtLevel


void
Cudd::zddVarsFromBddVars(
  int multiplicity)
{
    int result = Cudd_zddVarsFromBddVars(p->manager, multiplicity);
    this->checkReturnValue(result);

} // Cudd::zddVarsFromBddVars


void
Cudd::AutodynEnable(
  Cudd_ReorderingType method)
{
    Cudd_AutodynEnable(p->manager, method);

} // Cudd::AutodynEnable


void
Cudd::AutodynDisable()
{
    Cudd_AutodynDisable(p->manager);

} // Cudd::AutodynDisable


int
Cudd::ReorderingStatus(
  Cudd_ReorderingType * method) const
{
    return Cudd_ReorderingStatus(p->manager, method);

} // Cudd::ReorderingStatus


void
Cudd::AutodynEnableZdd(
  Cudd_ReorderingType method)
{
    Cudd_AutodynEnableZdd(p->manager, method);

} // Cudd::AutodynEnableZdd


void
Cudd::AutodynDisableZdd()
{
    Cudd_AutodynDisableZdd(p->manager);

} // Cudd::AutodynDisableZdd


int
Cudd::ReorderingStatusZdd(
  Cudd_ReorderingType * method) const
{
    return Cudd_ReorderingStatusZdd(p->manager, method);

} // Cudd::ReorderingStatusZdd


int
Cudd::zddRealignmentEnabled() const
{
    return Cudd_zddRealignmentEnabled(p->manager);

} // Cudd::zddRealignmentEnabled


void
Cudd::zddRealignEnable()
{
    Cudd_zddRealignEnable(p->manager);

} // Cudd::zddRealignEnable


void
Cudd::zddRealignDisable()
{
    Cudd_zddRealignDisable(p->manager);

} // Cudd::zddRealignDisable


int
Cudd::bddRealignmentEnabled() const
{
    return Cudd_bddRealignmentEnabled(p->manager);

} // Cudd::bddRealignmentEnabled


void
Cudd::bddRealignEnable()
{
    Cudd_bddRealignEnable(p->manager);

} // Cudd::bddRealignEnable


void
Cudd::bddRealignDisable()
{
    Cudd_bddRealignDisable(p->manager);

} // Cudd::bddRealignDisable


ADD
Cudd::background()
{
    DdNode *result = Cudd_ReadBackground(p->manager);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::background


void
Cudd::SetBackground(
  ADD bg)
{
    DdManager *mgr = p->manager;
    if (mgr != bg.manager()->p->manager) {
	p->errorHandler("Background comes from different manager.");
    }
    Cudd_SetBackground(mgr, bg.getNode());

} // Cudd::SetBackground


unsigned int
Cudd::ReadCacheSlots() const
{
    return Cudd_ReadCacheSlots(p->manager);

} // Cudd::ReadCacheSlots


double
Cudd::ReadCacheLookUps() const
{
    return Cudd_ReadCacheLookUps(p->manager);

} // Cudd::ReadCacheLookUps


double
Cudd::ReadCacheUsedSlots() const
{
    return Cudd_ReadCacheUsedSlots(p->manager);

} // Cudd::ReadCacheUsedSlots


double
Cudd::ReadCacheHits() const
{
    return Cudd_ReadCacheHits(p->manager);

} // Cudd::ReadCacheHits


unsigned int
Cudd::ReadMinHit() const
{
    return Cudd_ReadMinHit(p->manager);

} // Cudd::ReadMinHit


void
Cudd::SetMinHit(
  unsigned int hr)
{
    Cudd_SetMinHit(p->manager, hr);

} // Cudd::SetMinHit


unsigned int
Cudd::ReadLooseUpTo() const
{
    return Cudd_ReadLooseUpTo(p->manager);

} // Cudd::ReadLooseUpTo


void
Cudd::SetLooseUpTo(
  unsigned int lut)
{
    Cudd_SetLooseUpTo(p->manager, lut);

} // Cudd::SetLooseUpTo


unsigned int
Cudd::ReadMaxCache() const
{
    return Cudd_ReadMaxCache(p->manager);

} // Cudd::ReadMaxCache


unsigned int
Cudd::ReadMaxCacheHard() const
{
    return Cudd_ReadMaxCacheHard(p->manager);

} // Cudd::ReadMaxCacheHard


void
Cudd::SetMaxCacheHard(
  unsigned int mc)
{
    Cudd_SetMaxCacheHard(p->manager, mc);

} // Cudd::SetMaxCacheHard


int
Cudd::ReadSize() const
{
    return Cudd_ReadSize(p->manager);

} // Cudd::ReadSize


int
Cudd::ReadZddSize() const
{
    return Cudd_ReadZddSize(p->manager);

} // Cudd::ReadZddSize


unsigned int
Cudd::ReadSlots() const
{
    return Cudd_ReadSlots(p->manager);

} // Cudd::ReadSlots


unsigned int
Cudd::ReadKeys() const
{
    return Cudd_ReadKeys(p->manager);

} // Cudd::ReadKeys


unsigned int
Cudd::ReadDead() const
{
    return Cudd_ReadDead(p->manager);

} // Cudd::ReadDead


unsigned int
Cudd::ReadMinDead() const
{
    return Cudd_ReadMinDead(p->manager);

} // Cudd::ReadMinDead


int
Cudd::ReadReorderings() const
{
    return Cudd_ReadReorderings(p->manager);

} // Cudd::ReadReorderings


long
Cudd::ReadReorderingTime() const
{
    return Cudd_ReadReorderingTime(p->manager);

} // Cudd::ReadReorderingTime


int
Cudd::ReadGarbageCollections() const
{
    return Cudd_ReadGarbageCollections(p->manager);

} // Cudd::ReadGarbageCollections


long
Cudd::ReadGarbageCollectionTime() const
{
    return Cudd_ReadGarbageCollectionTime(p->manager);

} // Cudd::ReadGarbageCollectionTime


int
Cudd::ReadSiftMaxVar() const
{
    return Cudd_ReadSiftMaxVar(p->manager);

} // Cudd::ReadSiftMaxVar


void
Cudd::SetSiftMaxVar(
  int smv)
{
    Cudd_SetSiftMaxVar(p->manager, smv);

} // Cudd::SetSiftMaxVar


int
Cudd::ReadSiftMaxSwap() const
{
    return Cudd_ReadSiftMaxSwap(p->manager);

} // Cudd::ReadSiftMaxSwap


void
Cudd::SetSiftMaxSwap(
  int sms)
{
    Cudd_SetSiftMaxSwap(p->manager, sms);

} // Cudd::SetSiftMaxSwap


double
Cudd::ReadMaxGrowth() const
{
    return Cudd_ReadMaxGrowth(p->manager);

} // Cudd::ReadMaxGrowth


void
Cudd::SetMaxGrowth(
  double mg)
{
    Cudd_SetMaxGrowth(p->manager, mg);

} // Cudd::SetMaxGrowth


MtrNode *
Cudd::ReadTree() const
{
    return Cudd_ReadTree(p->manager);

} // Cudd::ReadTree


void
Cudd::SetTree(
  MtrNode * tree)
{
    Cudd_SetTree(p->manager, tree);

} // Cudd::SetTree


void
Cudd::FreeTree()
{
    Cudd_FreeTree(p->manager);

} // Cudd::FreeTree


MtrNode *
Cudd::ReadZddTree() const
{
    return Cudd_ReadZddTree(p->manager);

} // Cudd::ReadZddTree


void
Cudd::SetZddTree(
  MtrNode * tree)
{
    Cudd_SetZddTree(p->manager, tree);

} // Cudd::SetZddTree


void
Cudd::FreeZddTree()
{
    Cudd_FreeZddTree(p->manager);

} // Cudd::FreeZddTree


unsigned int
DD::NodeReadIndex() const
{
    return Cudd_NodeReadIndex(node);

} // DD::NodeReadIndex


int
Cudd::ReadPerm(
  int i) const
{
    return Cudd_ReadPerm(p->manager, i);

} // Cudd::ReadPerm


int
Cudd::ReadPermZdd(
  int i) const
{
    return Cudd_ReadPermZdd(p->manager, i);

} // Cudd::ReadPermZdd


int
Cudd::ReadInvPerm(
  int i) const
{
    return Cudd_ReadInvPerm(p->manager, i);

} // Cudd::ReadInvPerm


int
Cudd::ReadInvPermZdd(
  int i) const
{
    return Cudd_ReadInvPermZdd(p->manager, i);

} // Cudd::ReadInvPermZdd


BDD
Cudd::ReadVars(
  int i)
{
    DdNode *result = Cudd_ReadVars(p->manager, i);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::ReadVars


CUDD_VALUE_TYPE
Cudd::ReadEpsilon() const
{
    return Cudd_ReadEpsilon(p->manager);

} // Cudd::ReadEpsilon


void
Cudd::SetEpsilon(
  CUDD_VALUE_TYPE ep)
{
    Cudd_SetEpsilon(p->manager, ep);

} // Cudd::SetEpsilon


Cudd_AggregationType
Cudd::ReadGroupcheck() const
{
    return Cudd_ReadGroupcheck(p->manager);

} // Cudd::ReadGroupcheck


void
Cudd::SetGroupcheck(
  Cudd_AggregationType gc)
{
    Cudd_SetGroupcheck(p->manager, gc);

} // Cudd::SetGroupcheck


int
Cudd::GarbageCollectionEnabled() const
{
    return Cudd_GarbageCollectionEnabled(p->manager);

} // Cudd::GarbageCollectionEnabled


void
Cudd::EnableGarbageCollection()
{
    Cudd_EnableGarbageCollection(p->manager);

} // Cudd::EnableGarbageCollection


void
Cudd::DisableGarbageCollection()
{
    Cudd_DisableGarbageCollection(p->manager);

} // Cudd::DisableGarbageCollection


int
Cudd::DeadAreCounted() const
{
    return Cudd_DeadAreCounted(p->manager);

} // Cudd::DeadAreCounted


void
Cudd::TurnOnCountDead()
{
    Cudd_TurnOnCountDead(p->manager);

} // Cudd::TurnOnCountDead


void
Cudd::TurnOffCountDead()
{
    Cudd_TurnOffCountDead(p->manager);

} // Cudd::TurnOffCountDead


int
Cudd::ReadRecomb() const
{
    return Cudd_ReadRecomb(p->manager);

} // Cudd::ReadRecomb


void
Cudd::SetRecomb(
  int recomb)
{
    Cudd_SetRecomb(p->manager, recomb);

} // Cudd::SetRecomb


int
Cudd::ReadSymmviolation() const
{
    return Cudd_ReadSymmviolation(p->manager);

} // Cudd::ReadSymmviolation


void
Cudd::SetSymmviolation(
  int symmviolation)
{
    Cudd_SetSymmviolation(p->manager, symmviolation);

} // Cudd::SetSymmviolation


int
Cudd::ReadArcviolation() const
{
    return Cudd_ReadArcviolation(p->manager);

} // Cudd::ReadArcviolation


void
Cudd::SetArcviolation(
  int arcviolation)
{
    Cudd_SetArcviolation(p->manager, arcviolation);

} // Cudd::SetArcviolation


int
Cudd::ReadPopulationSize() const
{
    return Cudd_ReadPopulationSize(p->manager);

} // Cudd::ReadPopulationSize


void
Cudd::SetPopulationSize(
  int populationSize)
{
    Cudd_SetPopulationSize(p->manager, populationSize);

} // Cudd::SetPopulationSize


int
Cudd::ReadNumberXovers() const
{
    return Cudd_ReadNumberXovers(p->manager);

} // Cudd::ReadNumberXovers


void
Cudd::SetNumberXovers(
  int numberXovers)
{
    Cudd_SetNumberXovers(p->manager, numberXovers);

} // Cudd::SetNumberXovers


unsigned long
Cudd::ReadMemoryInUse() const
{
    return Cudd_ReadMemoryInUse(p->manager);

} // Cudd::ReadMemoryInUse


long
Cudd::ReadPeakNodeCount() const
{
    return Cudd_ReadPeakNodeCount(p->manager);

} // Cudd::ReadPeakNodeCount


long
Cudd::ReadNodeCount() const
{
    return Cudd_ReadNodeCount(p->manager);

} // Cudd::ReadNodeCount


long
Cudd::zddReadNodeCount() const
{
    return Cudd_zddReadNodeCount(p->manager);

} // Cudd::zddReadNodeCount


void
Cudd::AddHook(
  DD_HFP f,
  Cudd_HookType where)
{
    int result = Cudd_AddHook(p->manager, f, where);
    this->checkReturnValue(result);

} // Cudd::AddHook


void
Cudd::RemoveHook(
  DD_HFP f,
  Cudd_HookType where)
{
    int result = Cudd_RemoveHook(p->manager, f, where);
    this->checkReturnValue(result);

} // Cudd::RemoveHook


int
Cudd::IsInHook(
  DD_HFP f,
  Cudd_HookType where) const
{
    return Cudd_IsInHook(p->manager, f, where);

} // Cudd::IsInHook


void
Cudd::EnableReorderingReporting()
{
    int result = Cudd_EnableReorderingReporting(p->manager);
    this->checkReturnValue(result);

} // Cudd::EnableReorderingReporting


void
Cudd::DisableReorderingReporting()
{
    int result = Cudd_DisableReorderingReporting(p->manager);
    this->checkReturnValue(result);

} // Cudd::DisableReorderingReporting


int
Cudd::ReorderingReporting()
{
    return Cudd_ReorderingReporting(p->manager);

} // Cudd::ReorderingReporting


int
Cudd::ReadErrorCode() const
{
    return Cudd_ReadErrorCode(p->manager);

} // Cudd::ReadErrorCode


void
Cudd::ClearErrorCode()
{
    Cudd_ClearErrorCode(p->manager);

} // Cudd::ClearErrorCode


FILE *
Cudd::ReadStdout() const
{
    return Cudd_ReadStdout(p->manager);

} // Cudd::ReadStdout


void
Cudd::SetStdout(FILE *fp)
{
    Cudd_SetStdout(p->manager, fp);

} // Cudd::SetStdout


FILE *
Cudd::ReadStderr() const
{
    return Cudd_ReadStderr(p->manager);

} // Cudd::ReadStderr


void
Cudd::SetStderr(FILE *fp)
{
    Cudd_SetStderr(p->manager, fp);

} // Cudd::SetStderr


unsigned int
Cudd::ReadNextReordering() const
{
    return Cudd_ReadNextReordering(p->manager);

} // Cudd::ReadNextReordering


double
Cudd::ReadSwapSteps() const
{
    return Cudd_ReadSwapSteps(p->manager);

} // Cudd::ReadSwapSteps


unsigned int
Cudd::ReadMaxLive() const
{
    return Cudd_ReadMaxLive(p->manager);

} // Cudd::ReadMaxLive


void
Cudd::SetMaxLive(unsigned int maxLive)
{
    Cudd_SetMaxLive(p->manager, maxLive);

} // Cudd::SetMaxLive


unsigned long
Cudd::ReadMaxMemory() const
{
    return Cudd_ReadMaxMemory(p->manager);

} // Cudd::ReadMaxMemory


void
Cudd::SetMaxMemory(unsigned long maxMem)
{
    Cudd_SetMaxMemory(p->manager, maxMem);

} // Cudd::SetMaxMemory


int
Cudd::bddBindVar(int index)
{
    return Cudd_bddBindVar(p->manager, index);

} // Cudd::bddBindVar


int
Cudd::bddUnbindVar(int index)
{
    return Cudd_bddUnbindVar(p->manager, index);

} // Cudd::bddUnbindVar


int
Cudd::bddVarIsBound(int index) const
{
    return Cudd_bddVarIsBound(p->manager, index);

} // Cudd::bddVarIsBound


ADD
ADD::ExistAbstract(
  const ADD& cube) const
{
    DdManager *mgr = this->checkSameManager(cube);
    DdNode *result = Cudd_addExistAbstract(mgr, node, cube.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::ExistAbstract


ADD
ADD::UnivAbstract(
  const ADD& cube) const
{
    DdManager *mgr = this->checkSameManager(cube);
    DdNode *result = Cudd_addUnivAbstract(mgr, node, cube.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::UnivAbstract


ADD
ADD::OrAbstract(
  const ADD& cube) const
{
    DdManager *mgr = this->checkSameManager(cube);
    DdNode *result = Cudd_addOrAbstract(mgr, node, cube.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::OrAbstract


ADD
ADD::Plus(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addPlus, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Plus


ADD
ADD::Times(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addTimes, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Times


ADD
ADD::Threshold(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addThreshold, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Threshold


ADD
ADD::SetNZ(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addSetNZ, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::SetNZ


ADD
ADD::Divide(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addDivide, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Divide


ADD
ADD::Minus(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addMinus, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Minus


ADD
ADD::Minimum(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addMinimum, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Minimum


ADD
ADD::Maximum(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addMaximum, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Maximum


ADD
ADD::OneZeroMaximum(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addOneZeroMaximum, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::OneZeroMaximum


ADD
ADD::Diff(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addDiff, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Diff


ADD
ADD::Agreement(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addAgreement, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Agreement


ADD
ADD::Or(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addOr, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Or


ADD
ADD::Nand(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addNand, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Nand


ADD
ADD::Nor(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addNor, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Nor


ADD
ADD::Xor(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addXor, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Xor


ADD
ADD::Xnor(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addApply(mgr, Cudd_addXnor, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Xnor


ADD
ADD::Log() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addMonadicApply(mgr, Cudd_addLog, node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Log


ADD
ADD::FindMax() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addFindMax(mgr, node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::FindMax


ADD
ADD::FindMin() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addFindMin(mgr, node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::FindMin


ADD
ADD::IthBit(
  int bit) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addIthBit(mgr, node, bit);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::IthBit


ADD
ADD::ScalarInverse(
  const ADD& epsilon) const
{
    DdManager *mgr = this->checkSameManager(epsilon);
    DdNode *result = Cudd_addScalarInverse(mgr, node, epsilon.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::ScalarInverse


ADD
ADD::Ite(
  const ADD& g,
  const ADD& h) const
{
    DdManager *mgr = this->checkSameManager(g);
    this->checkSameManager(h);
    DdNode *result = Cudd_addIte(mgr, node, g.node, h.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Ite


ADD
ADD::IteConstant(
  const ADD& g,
  const ADD& h) const
{
    DdManager *mgr = this->checkSameManager(g);
    this->checkSameManager(h);
    DdNode *result = Cudd_addIteConstant(mgr, node, g.node, h.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::IteConstant


ADD
ADD::EvalConst(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addEvalConst(mgr, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::EvalConst


int
ADD::Leq(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    return Cudd_addLeq(mgr, node, g.node);

} // ADD::Leq


ADD
ADD::Cmpl() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addCmpl(mgr, node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Cmpl


ADD
ADD::Negate() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addNegate(mgr, node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Negate


ADD
ADD::RoundOff(
  int N) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addRoundOff(mgr, node, N);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::RoundOff


ADD
Cudd::Walsh(
  ADDvector x,
  ADDvector y)
{
    int n = x.count();
    DdNode **X = ALLOC(DdNode *,n);
    DdNode **Y = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	X[i] = x[i].getNode();
	Y[i] = y[i].getNode();
    }
    DdNode *result = Cudd_addWalsh(p->manager, X, Y, n);
    FREE(X);
    FREE(Y);
    this->checkReturnValue(result);
    return ADD(this, result);

} // ADD::Walsh


ADD
Cudd::addResidue(
  int n,
  int m,
  int options,
  int top)
{
    DdNode *result = Cudd_addResidue(p->manager, n, m, options, top);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::addResidue


BDD
BDD::AndAbstract(
  const BDD& g,
  const BDD& cube) const
{
    DdManager *mgr = this->checkSameManager(g);
    this->checkSameManager(cube);
    DdNode *result = Cudd_bddAndAbstract(mgr, node, g.node, cube.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::AndAbstract


BDD
BDD::AndAbstractLimit(
  const BDD& g,
  const BDD& cube,
  unsigned int limit) const
{
    DdManager *mgr = this->checkSameManager(g);
    this->checkSameManager(cube);
    DdNode *result = Cudd_bddAndAbstractLimit(mgr, node, g.node,
					      cube.node, limit);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::AndAbstractLimit


int
Cudd::ApaNumberOfDigits(
  int binaryDigits) const
{
    return Cudd_ApaNumberOfDigits(binaryDigits);

} // Cudd::ApaNumberOfDigits


DdApaNumber
Cudd::NewApaNumber(
  int digits) const
{
    return Cudd_NewApaNumber(digits);

} // Cudd::NewApaNumber


void
Cudd::ApaCopy(
  int digits,
  DdApaNumber source,
  DdApaNumber dest) const
{
    Cudd_ApaCopy(digits, source, dest);

} // Cudd::ApaCopy


DdApaDigit
Cudd::ApaAdd(
  int digits,
  DdApaNumber a,
  DdApaNumber b,
  DdApaNumber sum) const
{
    return Cudd_ApaAdd(digits, a, b, sum);

} // Cudd::ApaAdd


DdApaDigit
Cudd::ApaSubtract(
  int digits,
  DdApaNumber a,
  DdApaNumber b,
  DdApaNumber diff) const
{
    return Cudd_ApaSubtract(digits, a, b, diff);

} // Cudd::ApaSubtract


DdApaDigit
Cudd::ApaShortDivision(
  int digits,
  DdApaNumber dividend,
  DdApaDigit divisor,
  DdApaNumber quotient) const
{
    return Cudd_ApaShortDivision(digits, dividend, divisor, quotient);

} // Cudd::ApaShortDivision


void
Cudd::ApaShiftRight(
  int digits,
  DdApaDigit in,
  DdApaNumber a,
  DdApaNumber b) const
{
    Cudd_ApaShiftRight(digits, in, a, b);

} // Cudd::ApaShiftRight


void
Cudd::ApaSetToLiteral(
  int digits,
  DdApaNumber number,
  DdApaDigit literal) const
{
    Cudd_ApaSetToLiteral(digits, number, literal);

} // Cudd::ApaSetToLiteral


void
Cudd::ApaPowerOfTwo(
  int digits,
  DdApaNumber number,
  int power) const
{
    Cudd_ApaPowerOfTwo(digits, number, power);

} // Cudd::ApaPowerOfTwo


void
Cudd::ApaPrintHex(
  FILE * fp,
  int digits,
  DdApaNumber number) const
{
    cout.flush();
    int result = Cudd_ApaPrintHex(fp, digits, number);
    this->checkReturnValue(result);

} // Cudd::ApaPrintHex


void
Cudd::ApaPrintDecimal(
  FILE * fp,
  int digits,
  DdApaNumber number) const
{
    cout.flush();
    int result = Cudd_ApaPrintDecimal(fp, digits, number);
    this->checkReturnValue(result);

} // Cudd::ApaPrintDecimal


DdApaNumber
ABDD::ApaCountMinterm(
  int nvars,
  int * digits) const
{
    DdManager *mgr = ddMgr->p->manager;
    return Cudd_ApaCountMinterm(mgr, node, nvars, digits);

} // ABDD::ApaCountMinterm


void
ABDD::ApaPrintMinterm(
  int nvars,
  FILE * fp) const
{
    cout.flush();
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_ApaPrintMinterm(fp, mgr, node, nvars);
    this->checkReturnValue(result);

} // ABDD::ApaPrintMinterm


void
ABDD::EpdPrintMinterm(
  int nvars,
  FILE * fp) const
{
    EpDouble count;
    char str[24];
    cout.flush();
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_EpdCountMinterm(mgr, node, nvars, &count);
    this->checkReturnValue(result,0);
    EpdGetString(&count, str);
    fprintf(fp, "%s\n", str);

} // ABDD::ApaPrintMinterm


BDD
BDD::UnderApprox(
  int numVars,
  int threshold,
  int safe,
  double quality) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_UnderApprox(mgr, node, numVars, threshold, safe, quality);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::UnderApprox


BDD
BDD::OverApprox(
  int numVars,
  int threshold,
  int safe,
  double quality) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_OverApprox(mgr, node, numVars, threshold, safe, quality);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::OverApprox


BDD
BDD::RemapUnderApprox(
  int numVars,
  int threshold,
  double quality) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_RemapUnderApprox(mgr, node, numVars, threshold, quality);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::RemapUnderApprox


BDD
BDD::RemapOverApprox(
  int numVars,
  int threshold,
  double quality) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_RemapOverApprox(mgr, node, numVars, threshold, quality);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::RemapOverApprox


BDD
BDD::ExistAbstract(
  const BDD& cube) const
{
    DdManager *mgr = this->checkSameManager(cube);
    DdNode *result = Cudd_bddExistAbstract(mgr, node, cube.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::ExistAbstract


BDD
BDD::XorExistAbstract(
  const BDD& g,
  const BDD& cube) const
{
    DdManager *mgr = this->checkSameManager(g);
    this->checkSameManager(cube);
    DdNode *result = Cudd_bddXorExistAbstract(mgr, node, g.node, cube.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::XorExistAbstract


BDD
BDD::UnivAbstract(
  const BDD& cube) const
{
    DdManager *mgr = this->checkSameManager(cube);
    DdNode *result = Cudd_bddUnivAbstract(mgr, node, cube.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::UnivAbstract


BDD
BDD::BooleanDiff(
  int x) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_bddBooleanDiff(mgr, node, x);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::BooleanDiff


int
BDD::VarIsDependent(
  const BDD& var) const
{
    DdManager *mgr = ddMgr->p->manager;
    return Cudd_bddVarIsDependent(mgr, node, var.node);

} // BDD::VarIsDependent


double
BDD::Correlation(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    return Cudd_bddCorrelation(mgr, node, g.node);

} // BDD::Correlation


double
BDD::CorrelationWeights(
  const BDD& g,
  double * prob) const
{
    DdManager *mgr = this->checkSameManager(g);
    return Cudd_bddCorrelationWeights(mgr, node, g.node, prob);

} // BDD::CorrelationWeights


BDD
BDD::Ite(
  const BDD& g,
  const BDD& h) const
{
    DdManager *mgr = this->checkSameManager(g);
    this->checkSameManager(h);
    DdNode *result = Cudd_bddIte(mgr, node, g.node, h.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Ite


BDD
BDD::IteConstant(
  const BDD& g,
  const BDD& h) const
{
    DdManager *mgr = this->checkSameManager(g);
    this->checkSameManager(h);
    DdNode *result = Cudd_bddIteConstant(mgr, node, g.node, h.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::IteConstant


BDD
BDD::Intersect(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddIntersect(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Intersect


BDD
BDD::And(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddAnd(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::And


BDD
BDD::AndLimit(
  const BDD& g,
  unsigned int limit) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddAndLimit(mgr, node, g.node, limit);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::AndLimit


BDD
BDD::Or(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddOr(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Or


BDD
BDD::Nand(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddNand(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Nand


BDD
BDD::Nor(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddNor(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Nor


BDD
BDD::Xor(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddXor(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Xor


BDD
BDD::Xnor(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddXnor(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Xnor


int
BDD::Leq(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    return Cudd_bddLeq(mgr, node, g.node);

} // BDD::Leq


BDD
ADD::BddThreshold(
  CUDD_VALUE_TYPE value) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addBddThreshold(mgr, node, value);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ADD::BddThreshold


BDD
ADD::BddStrictThreshold(
  CUDD_VALUE_TYPE value) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addBddStrictThreshold(mgr, node, value);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ADD::BddStrictThreshold


BDD
ADD::BddInterval(
  CUDD_VALUE_TYPE lower,
  CUDD_VALUE_TYPE upper) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addBddInterval(mgr, node, lower, upper);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ADD::BddInterval


BDD
ADD::BddIthBit(
  int bit) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addBddIthBit(mgr, node, bit);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ADD::BddIthBit


ADD
BDD::Add() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_BddToAdd(mgr, node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // BDD::Add


BDD
ADD::BddPattern() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addBddPattern(mgr, node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ADD::BddPattern


BDD
BDD::Transfer(
  Cudd& destination) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_bddTransfer(mgr, destination.p->manager, node);
    this->checkReturnValue(result);
    return BDD(&destination, result);

} // BDD::Transfer


void
Cudd::DebugCheck()
{
    int result = Cudd_DebugCheck(p->manager);
    this->checkReturnValue(result == 0);

} // Cudd::DebugCheck


void
Cudd::CheckKeys()
{
    int result = Cudd_CheckKeys(p->manager);
    this->checkReturnValue(result == 0);

} // Cudd::CheckKeys


BDD
BDD::ClippingAnd(
  const BDD& g,
  int maxDepth,
  int direction) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddClippingAnd(mgr, node, g.node, maxDepth,
					 direction);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::ClippingAnd


BDD
BDD::ClippingAndAbstract(
  const BDD& g,
  const BDD& cube,
  int maxDepth,
  int direction) const
{
    DdManager *mgr = this->checkSameManager(g);
    this->checkSameManager(cube);
    DdNode *result = Cudd_bddClippingAndAbstract(mgr, node, g.node, cube.node,
						 maxDepth, direction);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::ClippingAndAbstract


ADD
ADD::Cofactor(
  const ADD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_Cofactor(mgr, node, g.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Cofactor


BDD
BDD::Cofactor(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_Cofactor(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Cofactor


BDD
BDD::Compose(
  const BDD& g,
  int v) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddCompose(mgr, node, g.node, v);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Compose


ADD
ADD::Compose(
  const ADD& g,
  int v) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_addCompose(mgr, node, g.node, v);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Compose


ADD
ADD::Permute(
  int * permut) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_addPermute(mgr, node, permut);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Permute


ADD
ADD::SwapVariables(
  ADDvector x,
  ADDvector y) const
{
    int n = x.count();
    DdManager *mgr = ddMgr->p->manager;
    DdNode **X = ALLOC(DdNode *,n);
    DdNode **Y = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	X[i] = x[i].node;
	Y[i] = y[i].node;
    }
    DdNode *result = Cudd_addSwapVariables(mgr, node, X, Y, n);
    FREE(X);
    FREE(Y);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::SwapVariables


BDD
BDD::Permute(
  int * permut) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_bddPermute(mgr, node, permut);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Permute


BDD
BDD::SwapVariables(
  BDDvector x,
  BDDvector y) const
{
    int n = x.count();
    DdManager *mgr = ddMgr->p->manager;
    DdNode **X = ALLOC(DdNode *,n);
    DdNode **Y = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	X[i] = x[i].node;
	Y[i] = y[i].node;
    }
    DdNode *result = Cudd_bddSwapVariables(mgr, node, X, Y, n);
    FREE(X);
    FREE(Y);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::SwapVariables


BDD
BDD::AdjPermuteX(
  BDDvector x) const
{
    int n = x.count();
    DdManager *mgr = ddMgr->p->manager;
    DdNode **X = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	X[i] = x[i].node;
    }
    DdNode *result = Cudd_bddAdjPermuteX(mgr, node, X, n);
    FREE(X);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::AdjPermuteX


ADD
ADD::VectorCompose(
  ADDvector vector) const
{
    DdManager *mgr = ddMgr->p->manager;
    int n = Cudd_ReadSize(mgr);
    DdNode **X = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	X[i] = vector[i].node;
    }
    DdNode *result = Cudd_addVectorCompose(mgr, node, X);
    FREE(X);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::VectorCompose


ADD
ADD::NonSimCompose(
  ADDvector vector) const
{
    DdManager *mgr = ddMgr->p->manager;
    int n = Cudd_ReadSize(mgr);
    DdNode **X = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	X[i] = vector[i].node;
    }
    DdNode *result = Cudd_addNonSimCompose(mgr, node, X);
    FREE(X);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::NonSimCompose


BDD
BDD::VectorCompose(
  BDDvector vector) const
{
    DdManager *mgr = ddMgr->p->manager;
    int n = Cudd_ReadSize(mgr);
    DdNode **X = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	X[i] = vector[i].node;
    }
    DdNode *result = Cudd_bddVectorCompose(mgr, node, X);
    FREE(X);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::VectorCompose


void
BDD::ApproxConjDecomp(
  BDD* g,
  BDD* h) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **pieces;
    int result = Cudd_bddApproxConjDecomp(mgr, node, &pieces);
    this->checkReturnValue(result == 2);
    *g = BDD(ddMgr, pieces[0]);
    *h = BDD(ddMgr, pieces[1]);
    Cudd_RecursiveDeref(mgr,pieces[0]);
    Cudd_RecursiveDeref(mgr,pieces[1]);
    FREE(pieces);

} // BDD::ApproxConjDecomp


void
BDD::ApproxDisjDecomp(
  BDD* g,
  BDD* h) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **pieces;
    int result = Cudd_bddApproxDisjDecomp(mgr, node, &pieces);
    this->checkReturnValue(result == 2);
    *g = BDD(ddMgr, pieces[0]);
    *h = BDD(ddMgr, pieces[1]);
    Cudd_RecursiveDeref(mgr,pieces[0]);
    Cudd_RecursiveDeref(mgr,pieces[1]);
    FREE(pieces);

} // BDD::ApproxDisjDecomp


void
BDD::IterConjDecomp(
  BDD* g,
  BDD* h) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **pieces;
    int result = Cudd_bddIterConjDecomp(mgr, node, &pieces);
    this->checkReturnValue(result == 2);
    *g = BDD(ddMgr, pieces[0]);
    *h = BDD(ddMgr, pieces[1]);
    Cudd_RecursiveDeref(mgr,pieces[0]);
    Cudd_RecursiveDeref(mgr,pieces[1]);
    FREE(pieces);

} // BDD::IterConjDecomp


void
BDD::IterDisjDecomp(
  BDD* g,
  BDD* h) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **pieces;
    int result = Cudd_bddIterDisjDecomp(mgr, node, &pieces);
    this->checkReturnValue(result == 2);
    *g = BDD(ddMgr, pieces[0]);
    *h = BDD(ddMgr, pieces[1]);
    Cudd_RecursiveDeref(mgr,pieces[0]);
    Cudd_RecursiveDeref(mgr,pieces[1]);
    FREE(pieces);

} // BDD::IterDisjDecomp


void
BDD::GenConjDecomp(
  BDD* g,
  BDD* h) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **pieces;
    int result = Cudd_bddGenConjDecomp(mgr, node, &pieces);
    this->checkReturnValue(result == 2);
    *g = BDD(ddMgr, pieces[0]);
    *h = BDD(ddMgr, pieces[1]);
    Cudd_RecursiveDeref(mgr,pieces[0]);
    Cudd_RecursiveDeref(mgr,pieces[1]);
    FREE(pieces);

} // BDD::GenConjDecomp


void
BDD::GenDisjDecomp(
  BDD* g,
  BDD* h) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **pieces;
    int result = Cudd_bddGenDisjDecomp(mgr, node, &pieces);
    this->checkReturnValue(result == 2);
    *g = BDD(ddMgr, pieces[0]);
    *h = BDD(ddMgr, pieces[1]);
    Cudd_RecursiveDeref(mgr,pieces[0]);
    Cudd_RecursiveDeref(mgr,pieces[1]);
    FREE(pieces);

} // BDD::GenDisjDecomp


void
BDD::VarConjDecomp(
  BDD* g,
  BDD* h) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **pieces;
    int result = Cudd_bddVarConjDecomp(mgr, node, &pieces);
    this->checkReturnValue(result == 2);
    *g = BDD(ddMgr, pieces[0]);
    *h = BDD(ddMgr, pieces[1]);
    Cudd_RecursiveDeref(mgr,pieces[0]);
    Cudd_RecursiveDeref(mgr,pieces[1]);
    FREE(pieces);

} // BDD::VarConjDecomp


void
BDD::VarDisjDecomp(
  BDD* g,
  BDD* h) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **pieces;
    int result = Cudd_bddVarDisjDecomp(mgr, node, &pieces);
    this->checkReturnValue(result == 2);
    *g = BDD(ddMgr, pieces[0]);
    *h = BDD(ddMgr, pieces[1]);
    Cudd_RecursiveDeref(mgr,pieces[0]);
    Cudd_RecursiveDeref(mgr,pieces[1]);
    FREE(pieces);

} // BDD::VarDisjDecomp


BDD
ABDD::FindEssential() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_FindEssential(mgr, node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ABDD::FindEssential


int
BDD::IsVarEssential(
  int id,
  int phase) const
{
    DdManager *mgr = ddMgr->p->manager;
    return Cudd_bddIsVarEssential(mgr, node, id, phase);

} // BDD::IsVarEssential


void
ABDD::PrintTwoLiteralClauses(
  char **names,
  FILE *fp) const
{
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_PrintTwoLiteralClauses(mgr, node, names, fp);
    this->checkReturnValue(result);

} // ABDD::PrintTwoLiteralClauses


void
BDDvector::DumpBlif(
  char ** inames,
  char ** onames,
  char * mname,
  FILE * fp) const
{
    DdManager *mgr = p->manager->getManager();
    int n = p->size;
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i ++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_DumpBlif(mgr, n, F, inames, onames, mname, fp);
    FREE(F);
    p->manager->checkReturnValue(result);

} // BDDvector::DumpBlif


void
BDDvector::DumpDot(
  char ** inames,
  char ** onames,
  FILE * fp) const
{
    DdManager *mgr = p->manager->getManager();
    int n = p->size;
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i ++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_DumpDot(mgr, n, F, inames, onames, fp);
    FREE(F);
    p->manager->checkReturnValue(result);

} // BDDvector::DumpDot


void
ADDvector::DumpDot(
  char ** inames,
  char ** onames,
  FILE * fp) const
{
    DdManager *mgr = p->manager->getManager();
    int n = p->size;
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i ++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_DumpDot(mgr, n, F, inames, onames, fp);
    FREE(F);
    p->manager->checkReturnValue(result);

} // ADDvector::DumpDot


void
BDDvector::DumpDaVinci(
  char ** inames,
  char ** onames,
  FILE * fp) const
{
    DdManager *mgr = p->manager->getManager();
    int n = p->size;
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i ++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_DumpDaVinci(mgr, n, F, inames, onames, fp);
    FREE(F);
    p->manager->checkReturnValue(result);

} // BDDvector::DumpDaVinci


void
ADDvector::DumpDaVinci(
  char ** inames,
  char ** onames,
  FILE * fp) const
{
    DdManager *mgr = p->manager->getManager();
    int n = p->size;
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i ++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_DumpDaVinci(mgr, n, F, inames, onames, fp);
    FREE(F);
    p->manager->checkReturnValue(result);

} // ADDvector::DumpDaVinci


void
BDDvector::DumpDDcal(
  char ** inames,
  char ** onames,
  FILE * fp) const
{
    DdManager *mgr = p->manager->getManager();
    int n = p->size;
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i ++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_DumpDDcal(mgr, n, F, inames, onames, fp);
    FREE(F);
    p->manager->checkReturnValue(result);

} // BDDvector::DumpDDcal


void
BDDvector::DumpFactoredForm(
  char ** inames,
  char ** onames,
  FILE * fp) const
{
    DdManager *mgr = p->manager->getManager();
    int n = p->size;
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i ++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_DumpFactoredForm(mgr, n, F, inames, onames, fp);
    FREE(F);
    p->manager->checkReturnValue(result);

} // BDDvector::DumpFactoredForm


BDD
BDD::Constrain(
  const BDD& c) const
{
    DdManager *mgr = this->checkSameManager(c);
    DdNode *result = Cudd_bddConstrain(mgr, node, c.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Constrain


BDD
BDD::Restrict(
  const BDD& c) const
{
    DdManager *mgr = this->checkSameManager(c);
    DdNode *result = Cudd_bddRestrict(mgr, node, c.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Restrict


BDD
BDD::NPAnd(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddNPAnd(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::NPAnd


ADD
ADD::Constrain(
  const ADD& c) const
{
    DdManager *mgr = this->checkSameManager(c);
    DdNode *result = Cudd_addConstrain(mgr, node, c.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Constrain


BDDvector
BDD::ConstrainDecomp() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **result = Cudd_bddConstrainDecomp(mgr, node);
    this->checkReturnValue((DdNode *)result);
    int size = Cudd_ReadSize(mgr);
    for (int i = 0; i < size; i++) {
	Cudd_Deref(result[i]);
    }
    BDDvector vect(size, ddMgr, result);
    FREE(result);
    return vect;

} // BDD::ConstrainDecomp


ADD
ADD::Restrict(
  const ADD& c) const
{
    DdManager *mgr = this->checkSameManager(c);
    DdNode *result = Cudd_addRestrict(mgr, node, c.node);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Restrict


BDDvector
BDD::CharToVect() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **result = Cudd_bddCharToVect(mgr, node);
    this->checkReturnValue((DdNode *)result);
    int size = Cudd_ReadSize(mgr);
    for (int i = 0; i < size; i++) {
	Cudd_Deref(result[i]);
    }
    BDDvector vect(size, ddMgr, result);
    FREE(result);
    return vect;

} // BDD::CharToVect


BDD
BDD::LICompaction(
  const BDD& c) const
{
    DdManager *mgr = this->checkSameManager(c);
    DdNode *result = Cudd_bddLICompaction(mgr, node, c.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::LICompaction


BDD
BDD::Squeeze(
  const BDD& u) const
{
    DdManager *mgr = this->checkSameManager(u);
    DdNode *result = Cudd_bddSqueeze(mgr, node, u.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Squeeze


BDD
BDD::Minimize(
  const BDD& c) const
{
    DdManager *mgr = this->checkSameManager(c);
    DdNode *result = Cudd_bddMinimize(mgr, node, c.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Minimize


BDD
BDD::SubsetCompress(
  int nvars,
  int threshold) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_SubsetCompress(mgr, node, nvars, threshold);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::SubsetCompress


BDD
BDD::SupersetCompress(
  int nvars,
  int threshold) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_SupersetCompress(mgr, node, nvars, threshold);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::SupersetCompress


MtrNode *
Cudd::MakeTreeNode(
  unsigned int low,
  unsigned int size,
  unsigned int type)
{
    return Cudd_MakeTreeNode(p->manager, low, size, type);

} // Cudd::MakeTreeNode


/* This is incorrect, but we'll wait for this one.
void
Cudd::Harwell(
  FILE * fp,
  DdManager * dd,
  ADD* E,
  ADD** x,
  ADD** y,
  ADD** xn,
  ADD** yn_,
  int * nx,
  int * ny,
  int * m,
  int * n,
  int bx,
  int sx,
  int by,
  int sy,
  int pr)
{
    DdManager *mgr = p->manager;
    int result = Cudd_addHarwell(fp, mgr, E, x, y, xn, yn_, nx, ny, m, n, bx, sx, by, sy, pr);
    this->checkReturnValue(result);

} // Cudd::Harwell
*/


void
Cudd::PrintLinear()
{
    cout.flush();
    int result = Cudd_PrintLinear(p->manager);
    this->checkReturnValue(result);

} // Cudd::PrintLinear


int
Cudd::ReadLinear(
  int x,
  int y)
{
    return Cudd_ReadLinear(p->manager, x, y);

} // Cudd::ReadLinear


BDD
BDD::LiteralSetIntersection(
  const BDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_bddLiteralSetIntersection(mgr, node, g.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::LiteralSetIntersection


ADD
ADD::MatrixMultiply(
  const ADD& B,
  ADDvector z) const
{
    int nz = z.count();
    DdManager *mgr = this->checkSameManager(B);
    DdNode **Z = ALLOC(DdNode *,nz);
    for (int i = 0; i < nz; i++) {
	Z[i] = z[i].node;
    }
    DdNode *result = Cudd_addMatrixMultiply(mgr, node, B.node, Z, nz);
    FREE(Z);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::MatrixMultiply


ADD
ADD::TimesPlus(
  const ADD& B,
  ADDvector z) const
{
    int nz = z.count();
    DdManager *mgr = this->checkSameManager(B);
    DdNode **Z = ALLOC(DdNode *,nz);
    for (int i = 0; i < nz; i++) {
	Z[i] = z[i].node;
    }
    DdNode *result = Cudd_addTimesPlus(mgr, node, B.node, Z, nz);
    FREE(Z);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::TimesPlus


ADD
ADD::Triangle(
  const ADD& g,
  ADDvector z) const
{
    int nz = z.count();
    DdManager *mgr = this->checkSameManager(g);
    DdNode **Z = ALLOC(DdNode *,nz);
    for (int i = 0; i < nz; i++) {
	Z[i] = z[i].node;
    }
    DdNode *result = Cudd_addTriangle(mgr, node, g.node, Z, nz);
    FREE(Z);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Triangle


BDD
BDD::PrioritySelect(
  BDDvector x,
  BDDvector y,
  BDDvector z,
  const BDD& Pi,
  DD_PRFP Pifunc) const
{
    int n = x.count();
    DdManager *mgr = ddMgr->p->manager;
    DdNode **X = ALLOC(DdNode *,n);
    DdNode **Y = ALLOC(DdNode *,n);
    DdNode **Z = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	X[i] = x[i].node;
	Y[i] = y[i].node;
	Z[i] = z[i].node;
    }
    DdNode *result = Cudd_PrioritySelect(mgr, node, X, Y, Z, Pi.node, n, Pifunc);
    FREE(X);
    FREE(Y);
    FREE(Z);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::PrioritySelect


BDD
Cudd::Xgty(
  BDDvector z,
  BDDvector x,
  BDDvector y)
{
    int N = z.count();
    DdManager *mgr = p->manager;
    DdNode **X = ALLOC(DdNode *,N);
    DdNode **Y = ALLOC(DdNode *,N);
    DdNode **Z = ALLOC(DdNode *,N);
    for (int i = 0; i < N; i++) {
	X[i] = x[i].getNode();
	Y[i] = y[i].getNode();
	Z[i] = z[i].getNode();
    }
    DdNode *result = Cudd_Xgty(mgr, N, Z, X, Y);
    FREE(X);
    FREE(Y);
    FREE(Z);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::Xgty


BDD
Cudd::Xeqy(
  BDDvector x,
  BDDvector y)
{
    int N = x.count();
    DdManager *mgr = p->manager;
    DdNode **X = ALLOC(DdNode *,N);
    DdNode **Y = ALLOC(DdNode *,N);
    for (int i = 0; i < N; i++) {
	X[i] = x[i].getNode();
	Y[i] = y[i].getNode();
    }
    DdNode *result = Cudd_Xeqy(mgr, N, X, Y);
    FREE(X);
    FREE(Y);
    this->checkReturnValue(result);
    return BDD(this, result);

} // BDD::Xeqy


ADD
Cudd::Xeqy(
  ADDvector x,
  ADDvector y)
{
    int N = x.count();
    DdManager *mgr = p->manager;
    DdNode **X = ALLOC(DdNode *,N);
    DdNode **Y = ALLOC(DdNode *,N);
    for (int i = 0; i < N; i++) {
	X[i] = x[i].getNode();
	Y[i] = y[i].getNode();
    }
    DdNode *result = Cudd_addXeqy(mgr, N, X, X);
    FREE(X);
    FREE(Y);
    this->checkReturnValue(result);
    return ADD(this, result);

} // ADD::Xeqy


BDD
Cudd::Dxygtdxz(
  BDDvector x,
  BDDvector y,
  BDDvector z)
{
    int N = x.count();
    DdManager *mgr = p->manager;
    DdNode **X = ALLOC(DdNode *,N);
    DdNode **Y = ALLOC(DdNode *,N);
    DdNode **Z = ALLOC(DdNode *,N);
    for (int i = 0; i < N; i++) {
	X[i] = x[i].getNode();
	Y[i] = y[i].getNode();
	Z[i] = z[i].getNode();
    }
    DdNode *result = Cudd_Dxygtdxz(mgr, N, X, Y, Z);
    FREE(X);
    FREE(Y);
    FREE(Z);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::Dxygtdxz


BDD
Cudd::Dxygtdyz(
  BDDvector x,
  BDDvector y,
  BDDvector z)
{
    int N = x.count();
    DdManager *mgr = p->manager;
    DdNode **X = ALLOC(DdNode *,N);
    DdNode **Y = ALLOC(DdNode *,N);
    DdNode **Z = ALLOC(DdNode *,N);
    for (int i = 0; i < N; i++) {
	X[i] = x[i].getNode();
	Y[i] = y[i].getNode();
	Z[i] = z[i].getNode();
    }
    DdNode *result = Cudd_Dxygtdyz(mgr, N, X, Y, Z);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::Dxygtdyz


BDD
BDD::CProjection(
  const BDD& Y) const
{
    DdManager *mgr = this->checkSameManager(Y);
    DdNode *result = Cudd_CProjection(mgr, node, Y.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::CProjection


int
BDD::MinHammingDist(
  int *minterm,
  int upperBound) const
{
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_MinHammingDist(mgr, node, minterm, upperBound);
    return result;

} // BDD::MinHammingDist


ADD
Cudd::Hamming(
  ADDvector xVars,
  ADDvector yVars)
{
    int nVars = xVars.count();
    DdManager *mgr = p->manager;
    DdNode **X = ALLOC(DdNode *,nVars);
    DdNode **Y = ALLOC(DdNode *,nVars);
    for (int i = 0; i < nVars; i++) {
	X[i] = xVars[i].getNode();
	Y[i] = yVars[i].getNode();
    }
    DdNode *result = Cudd_addHamming(mgr, X, Y, nVars);
    FREE(X);
    FREE(Y);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::Hamming


/* We'll leave these two out for the time being.
void
Cudd::Read(
  FILE * fp,
  ADD* E,
  ADD** x,
  ADD** y,
  ADD** xn,
  ADD** yn_,
  int * nx,
  int * ny,
  int * m,
  int * n,
  int bx,
  int sx,
  int by,
  int sy)
{
    DdManager *mgr = p->manager;
    int result = Cudd_addRead(fp, mgr, E, x, y, xn, yn_, nx, ny, m, n, bx, sx, by, sy);
    this->checkReturnValue(result);

} // Cudd::Read


void
Cudd::Read(
  FILE * fp,
  BDD* E,
  BDD** x,
  BDD** y,
  int * nx,
  int * ny,
  int * m,
  int * n,
  int bx,
  int sx,
  int by,
  int sy)
{
    DdManager *mgr = p->manager;
    int result = Cudd_bddRead(fp, mgr, E, x, y, nx, ny, m, n, bx, sx, by, sy);
    this->checkReturnValue(result);

} // Cudd::Read
*/


void
Cudd::ReduceHeap(
  Cudd_ReorderingType heuristic,
  int minsize)
{
    int result = Cudd_ReduceHeap(p->manager, heuristic, minsize);
    this->checkReturnValue(result);

} // Cudd::ReduceHeap


void
Cudd::ShuffleHeap(
  int * permutation)
{
    int result = Cudd_ShuffleHeap(p->manager, permutation);
    this->checkReturnValue(result);

} // Cudd::ShuffleHeap


ADD
ADD::Eval(
  int * inputs) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_Eval(mgr, node, inputs);
    this->checkReturnValue(result);
    return ADD(ddMgr, result);

} // ADD::Eval


BDD
BDD::Eval(
  int * inputs) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_Eval(mgr, node, inputs);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Eval


BDD
ABDD::ShortestPath(
  int * weight,
  int * support,
  int * length) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_ShortestPath(mgr, node, weight, support, length);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ABDD::ShortestPath


BDD
ABDD::LargestCube(
  int * length) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_LargestCube(mgr, node, length);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ABDD::LargestCube


int
ABDD::ShortestLength(
  int * weight) const
{
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_ShortestLength(mgr, node, weight);
    this->checkReturnValue(result != CUDD_OUT_OF_MEM);
    return result;

} // ABDD::ShortestLength


BDD
BDD::Decreasing(
  int i) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_Decreasing(mgr, node, i);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Decreasing


BDD
BDD::Increasing(
  int i) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_Increasing(mgr, node, i);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Increasing


int
ABDD::EquivDC(
  const ABDD& G,
  const ABDD& D) const
{
    DdManager *mgr = this->checkSameManager(G);
    this->checkSameManager(D);
    return Cudd_EquivDC(mgr, node, G.node, D.node);

} // ABDD::EquivDC


int
ADD::EqualSupNorm(
  const ADD& g,
  CUDD_VALUE_TYPE tolerance,
  int pr) const
{
    DdManager *mgr = this->checkSameManager(g);
    return Cudd_EqualSupNorm(mgr, node, g.node, tolerance, pr);

} // ADD::EqualSupNorm


double *
ABDD::CofMinterm() const
{
    DdManager *mgr = ddMgr->p->manager;
    double *result = Cudd_CofMinterm(mgr, node);
    this->checkReturnValue((DdNode *)result);
    return result;

} // ABDD::CofMinterm


BDD
BDD::SolveEqn(
  const BDD& Y,
  BDD* G,
  int ** yIndex,
  int n) const
{
    DdManager *mgr = this->checkSameManager(Y);
    DdNode **g = ALLOC(DdNode *,n);
    DdNode *result = Cudd_SolveEqn(mgr, node, Y.node, g, yIndex, n);
    this->checkReturnValue(result);
    for (int i = 0; i < n; i++) {
	G[i] = BDD(ddMgr, g[i]);
	Cudd_RecursiveDeref(mgr,g[i]);
    }
    FREE(g);
    return BDD(ddMgr, result);

} // BDD::SolveEqn


BDD
BDD::VerifySol(
  BDD* G,
  int * yIndex,
  int n) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode **g = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	g[i] = G[i].node;
    }
    DdNode *result = Cudd_VerifySol(mgr, node, g, yIndex, n);
    FREE(g);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::VerifySol


BDD
BDD::SplitSet(
  BDDvector xVars,
  double m) const
{
    int n = xVars.count();
    DdManager *mgr = ddMgr->p->manager;
    DdNode **X = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	X[i] = xVars[i].node;
    }
    DdNode *result = Cudd_SplitSet(mgr, node, X, n, m);
    FREE(X);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::SplitSet


BDD
BDD::SubsetHeavyBranch(
  int numVars,
  int threshold) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_SubsetHeavyBranch(mgr, node, numVars, threshold);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::SubsetHeavyBranch


BDD
BDD::SupersetHeavyBranch(
  int numVars,
  int threshold) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_SupersetHeavyBranch(mgr, node, numVars, threshold);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::SupersetHeavyBranch


BDD
BDD::SubsetShortPaths(
  int numVars,
  int threshold,
  int hardlimit) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_SubsetShortPaths(mgr, node, numVars, threshold, hardlimit);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::SubsetShortPaths


BDD
BDD::SupersetShortPaths(
  int numVars,
  int threshold,
  int hardlimit) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_SupersetShortPaths(mgr, node, numVars, threshold, hardlimit);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::SupersetShortPaths


void
Cudd::SymmProfile(
  int lower,
  int upper) const
{
    Cudd_SymmProfile(p->manager, lower, upper);

} // Cudd::SymmProfile


unsigned int
Cudd::Prime(
  unsigned int pr) const
{
    return Cudd_Prime(pr);

} // Cudd::Prime


void
ABDD::PrintMinterm() const
{
    cout.flush();
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_PrintMinterm(mgr, node);
    this->checkReturnValue(result);

} // ABDD::PrintMinterm


void
BDD::PrintCover() const
{
    cout.flush();
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_bddPrintCover(mgr, node, node);
    this->checkReturnValue(result);

} // BDD::PrintCover


void
BDD::PrintCover(
  const BDD& u) const
{
    this->checkSameManager(u);
    cout.flush();
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_bddPrintCover(mgr, node, u.node);
    this->checkReturnValue(result);

} // BDD::PrintCover


int
BDD::EstimateCofactor(
  int i,
  int phase) const
{
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_EstimateCofactor(mgr, node, i, phase);
    this->checkReturnValue(result != CUDD_OUT_OF_MEM);
    return result;

} // BDD::EstimateCofactor


int
BDD::EstimateCofactorSimple(
  int i) const
{
    int result = Cudd_EstimateCofactorSimple(node, i);
    return result;

} // BDD::EstimateCofactorSimple


int
Cudd::SharingSize(
  DD* nodes,
  int n) const
{
    DdNode **nodeArray = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	nodeArray[i] = nodes[i].getNode();
    }
    int result = Cudd_SharingSize(nodeArray, n);
    FREE(nodeArray);
    this->checkReturnValue(result > 0);
    return result;

} // Cudd::SharingSize


double
ABDD::CountMinterm(
  int nvars) const
{
    DdManager *mgr = ddMgr->p->manager;
    double result = Cudd_CountMinterm(mgr, node, nvars);
    this->checkReturnValue(result != (double) CUDD_OUT_OF_MEM);
    return result;

} // ABDD::CountMinterm


double
ABDD::CountPath() const
{
    double result = Cudd_CountPath(node);
    this->checkReturnValue(result != (double) CUDD_OUT_OF_MEM);
    return result;

} // ABDD::CountPath


BDD
ABDD::Support() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_Support(mgr, node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ABDD::Support


int
ABDD::SupportSize() const
{
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_SupportSize(mgr, node);
    this->checkReturnValue(result != CUDD_OUT_OF_MEM);
    return result;

} // ABDD::SupportSize


BDD
BDDvector::VectorSupport() const
{
    int n = p->size;
    DdManager *mgr = p->manager->getManager();
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	F[i] = p->vect[i].getNode();
    }
    DdNode *result = Cudd_VectorSupport(mgr, F, n);
    FREE(F);
    p->manager->checkReturnValue(result);
    return BDD(p->manager, result);

} // BDDvector::VectorSupport


int
BDDvector::nodeCount() const
{
    int n = p->size;
    DdNode **nodeArray = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	nodeArray[i] = p->vect[i].getNode();
    }
    int result = Cudd_SharingSize(nodeArray, n);
    FREE(nodeArray);
    p->manager->checkReturnValue(result > 0);
    return result;

} // BDDvector::nodeCount


BDD
ADDvector::VectorSupport() const
{
    int n = p->size;
    DdManager *mgr = p->manager->getManager();
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	F[i] = p->vect[i].getNode();
    }
    DdNode *result = Cudd_VectorSupport(mgr, F, n);
    FREE(F);
    p->manager->checkReturnValue(result);
    return BDD(p->manager, result);

} // ADDvector::VectorSupport


int
BDDvector::VectorSupportSize() const
{
    int n = p->size;
    DdManager *mgr = p->manager->getManager();
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_VectorSupportSize(mgr, F, n);
    FREE(F);
    p->manager->checkReturnValue(result != CUDD_OUT_OF_MEM);
    return result;

} // BDDvector::VectorSupportSize


int
ADDvector::VectorSupportSize() const
{
    int n = p->size;
    DdManager *mgr = p->manager->getManager();
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_VectorSupportSize(mgr, F, n);
    FREE(F);
    p->manager->checkReturnValue(result != CUDD_OUT_OF_MEM);
    return result;

} // ADDvector::VectorSupportSize


void
ABDD::ClassifySupport(
  const ABDD& g,
  BDD* common,
  BDD* onlyF,
  BDD* onlyG) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *C, *F, *G;
    int result = Cudd_ClassifySupport(mgr, node, g.node, &C, &F, &G);
    this->checkReturnValue(result);
    *common = BDD(ddMgr, C);
    *onlyF = BDD(ddMgr, F);
    *onlyG = BDD(ddMgr, G);

} // ABDD::ClassifySupport


int
ABDD::CountLeaves() const
{
    return Cudd_CountLeaves(node);

} // ABDD::CountLeaves


void
BDD::PickOneCube(
  char * string) const
{
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_bddPickOneCube(mgr, node, string);
    this->checkReturnValue(result);

} // BDD::PickOneCube


BDD
BDD::PickOneMinterm(
  BDDvector vars) const
{
    int n = vars.count();
    DdManager *mgr = ddMgr->p->manager;
    DdNode **V = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	V[i] = vars[i].node;
    }
    DdNode *result = Cudd_bddPickOneMinterm(mgr, node, V, n);
    FREE(V);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::PickOneMinterm


DdGen *
ABDD::FirstCube(
  int ** cube,
  CUDD_VALUE_TYPE * value) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdGen *result = Cudd_FirstCube(mgr, node, cube, value);
    this->checkReturnValue((DdNode *)result);
    return result;

} // ABDD::FirstCube


int
NextCube(
  DdGen * gen,
  int ** cube,
  CUDD_VALUE_TYPE * value)
{
    return Cudd_NextCube(gen, cube, value);

} // NextCube


BDD
Cudd::bddComputeCube(
  BDD * vars,
  int * phase,
  int n)
{
    DdManager *mgr = p->manager;
    DdNode **V = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	V[i] = vars[i].getNode();
    }
    DdNode *result = Cudd_bddComputeCube(mgr, V, phase, n);
    FREE(V);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::bddComputeCube


ADD
Cudd::addComputeCube(
  ADD * vars,
  int * phase,
  int n)
{
    DdManager *mgr = p->manager;
    DdNode **V = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i++) {
	V[i] = vars[i].getNode();
    }
    DdNode *result = Cudd_addComputeCube(mgr, V, phase, n);
    FREE(V);
    this->checkReturnValue(result);
    return ADD(this, result);

} // Cudd::addComputeCube


DdGen *
BDD::FirstNode(
  BDD* fnode) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *Fn;
    DdGen *result = Cudd_FirstNode(mgr, node, &Fn);
    this->checkReturnValue((DdNode *)result);
    *fnode = BDD(ddMgr, Fn);
    return result;

} // DD::FirstNode


int
Cudd::NextNode(
  DdGen * gen,
  BDD * nnode)
{
    DdNode *nn;
    int result = Cudd_NextNode(gen, &nn);
    *nnode = BDD(this, nn);
    return result;

} // Cudd::NextNode


int
GenFree(
  DdGen * gen)
{
    return Cudd_GenFree(gen);

} // GenFree


int
IsGenEmpty(
  DdGen * gen)
{
    return Cudd_IsGenEmpty(gen);

} // IsGenEmpty


BDD
Cudd::IndicesToCube(
  int * array,
  int n)
{
    DdNode *result = Cudd_IndicesToCube(p->manager, array, n);
    this->checkReturnValue(result);
    return BDD(this, result);

} // Cudd::IndicesToCube


void
Cudd::PrintVersion(
  FILE * fp) const
{
    cout.flush();
    Cudd_PrintVersion(fp);

} // Cudd::PrintVersion


double
Cudd::AverageDistance() const
{
    return Cudd_AverageDistance(p->manager);

} // Cudd::AverageDistance


long
Cudd::Random()
{
    return Cudd_Random();

} // Cudd::Random


void
Cudd::Srandom(
  long seed)
{
    Cudd_Srandom(seed);

} // Cudd::Srandom


double
ABDD::Density(
  int nvars) const
{
    DdManager *mgr = ddMgr->p->manager;
    double result = Cudd_Density(mgr, node, nvars);
    this->checkReturnValue(result != (double) CUDD_OUT_OF_MEM);
    return result;

} // ABDD::Density


int
ZDD::Count() const
{
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_zddCount(mgr, node);
    this->checkReturnValue(result != CUDD_OUT_OF_MEM);
    return result;

} // ZDD::Count


double
ZDD::CountDouble() const
{
    DdManager *mgr = ddMgr->p->manager;
    double result = Cudd_zddCountDouble(mgr, node);
    this->checkReturnValue(result != (double) CUDD_OUT_OF_MEM);
    return result;

} // ZDD::CountDouble


ZDD
ZDD::Product(
  const ZDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_zddProduct(mgr, node, g.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::Product


ZDD
ZDD::UnateProduct(
  const ZDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_zddUnateProduct(mgr, node, g.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::UnateProduct


ZDD
ZDD::WeakDiv(
  const ZDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_zddWeakDiv(mgr, node, g.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::WeakDiv


ZDD
ZDD::Divide(
  const ZDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_zddDivide(mgr, node, g.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::Divide


ZDD
ZDD::WeakDivF(
  const ZDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_zddWeakDivF(mgr, node, g.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::WeakDivF


ZDD
ZDD::DivideF(
  const ZDD& g) const
{
    DdManager *mgr = this->checkSameManager(g);
    DdNode *result = Cudd_zddDivideF(mgr, node, g.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::DivideF


MtrNode *
Cudd::MakeZddTreeNode(
  unsigned int low,
  unsigned int size,
  unsigned int type)
{
    return Cudd_MakeZddTreeNode(p->manager, low, size, type);

} // Cudd::MakeZddTreeNode


BDD
BDD::zddIsop(
  const BDD& U,
  ZDD* zdd_I) const
{
    DdManager *mgr = this->checkSameManager(U);
    DdNode *Z;
    DdNode *result = Cudd_zddIsop(mgr, node, U.node, &Z);
    this->checkReturnValue(result);
    *zdd_I = ZDD(ddMgr, Z);
    return BDD(ddMgr, result);

} // BDD::Isop


BDD
BDD::Isop(
  const BDD& U) const
{
    DdManager *mgr = this->checkSameManager(U);
    DdNode *result = Cudd_bddIsop(mgr, node, U.node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // BDD::Isop


double
ZDD::CountMinterm(
  int path) const
{
    DdManager *mgr = ddMgr->p->manager;
    double result = Cudd_zddCountMinterm(mgr, node, path);
    this->checkReturnValue(result != (double) CUDD_OUT_OF_MEM);
    return result;

} // ZDD::CountMinterm


void
Cudd::zddPrintSubtable() const
{
    cout.flush();
    Cudd_zddPrintSubtable(p->manager);

} // Cudd::zddPrintSubtable


ZDD
BDD::PortToZdd() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_zddPortFromBdd(mgr, node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // BDD::PortToZdd


BDD
ZDD::PortToBdd() const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_zddPortToBdd(mgr, node);
    this->checkReturnValue(result);
    return BDD(ddMgr, result);

} // ZDD::PortToBdd


void
Cudd::zddReduceHeap(
  Cudd_ReorderingType heuristic,
  int minsize)
{
    int result = Cudd_zddReduceHeap(p->manager, heuristic, minsize);
    this->checkReturnValue(result);

} // Cudd::zddReduceHeap


void
Cudd::zddShuffleHeap(
  int * permutation)
{
    int result = Cudd_zddShuffleHeap(p->manager, permutation);
    this->checkReturnValue(result);

} // Cudd::zddShuffleHeap


ZDD
ZDD::Ite(
  const ZDD& g,
  const ZDD& h) const
{
    DdManager *mgr = this->checkSameManager(g);
    this->checkSameManager(h);
    DdNode *result = Cudd_zddIte(mgr, node, g.node, h.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::Ite


ZDD
ZDD::Union(
  const ZDD& Q) const
{
    DdManager *mgr = this->checkSameManager(Q);
    DdNode *result = Cudd_zddUnion(mgr, node, Q.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::Union


ZDD
ZDD::Intersect(
  const ZDD& Q) const
{
    DdManager *mgr = this->checkSameManager(Q);
    DdNode *result = Cudd_zddIntersect(mgr, node, Q.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::Intersect


ZDD
ZDD::Diff(
  const ZDD& Q) const
{
    DdManager *mgr = this->checkSameManager(Q);
    DdNode *result = Cudd_zddDiff(mgr, node, Q.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::Diff


ZDD
ZDD::DiffConst(
  const ZDD& Q) const
{
    DdManager *mgr = this->checkSameManager(Q);
    DdNode *result = Cudd_zddDiffConst(mgr, node, Q.node);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::DiffConst


ZDD
ZDD::Subset1(
  int var) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_zddSubset1(mgr, node, var);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::Subset1


ZDD
ZDD::Subset0(
  int var) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_zddSubset0(mgr, node, var);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::Subset0


ZDD
ZDD::Change(
  int var) const
{
    DdManager *mgr = ddMgr->p->manager;
    DdNode *result = Cudd_zddChange(mgr, node, var);
    this->checkReturnValue(result);
    return ZDD(ddMgr, result);

} // ZDD::Change


void
Cudd::zddSymmProfile(
  int lower,
  int upper) const
{
    Cudd_zddSymmProfile(p->manager, lower, upper);

} // Cudd::zddSymmProfile


void
ZDD::PrintMinterm() const
{
    cout.flush();
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_zddPrintMinterm(mgr, node);
    this->checkReturnValue(result);

} // ZDD::PrintMinterm


void
ZDD::PrintCover() const
{
    cout.flush();
    DdManager *mgr = ddMgr->p->manager;
    int result = Cudd_zddPrintCover(mgr, node);
    this->checkReturnValue(result);

} // ZDD::PrintCover


void
ZDDvector::DumpDot(
  char ** inames,
  char ** onames,
  FILE * fp) const
{
    DdManager *mgr = p->manager->getManager();
    int n = p->size;
    DdNode **F = ALLOC(DdNode *,n);
    for (int i = 0; i < n; i ++) {
	F[i] = p->vect[i].getNode();
    }
    int result = Cudd_zddDumpDot(mgr, n, F, inames, onames, fp);
    FREE(F);
    p->manager->checkReturnValue(result);

} // ZDDvector::DumpDot
