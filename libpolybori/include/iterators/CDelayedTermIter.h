// -*- c++ -*-
//*****************************************************************************
/** @file CDelayedTermIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-04
 * 
 * A specialization of CTermIter, which construct terms only if on explicite
 * calls. 
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include CTermIter definitions
#include "CTermIter.h"

#ifndef CDelayedTermIter_h_
#define CDelayedTermIter_h_

BEGIN_NAMESPACE_PBORI

/** @class CDelayedTermIter
 * @brief This class defines extend a given PolyBoRi degree iterator
 *
 **/

template <class TermType, class AppendOp, class TerminalValueOp, class DegIterBase>
class CDelayedTermIter:
  public DegIterBase {

public:
  typedef TermType term_type;
  typedef typename term_type::size_type size_type;
  typedef DegIterBase base;


  typedef CDelayedTermIter<term_type, AppendOp, TerminalValueOp, DegIterBase> self;

  typedef typename base::stack_type stack_type;
  typedef AppendOp appendop_type;
  typedef TerminalValueOp terminalop_type;

  /// Default constructor
  CDelayedTermIter(): base() {}

  /// Copy constructor
  CDelayedTermIter(const self& rhs): base(rhs) {}

  /// Construct from degree iterator type
  CDelayedTermIter(const base& rhs): base(rhs) {}

  /// Destructor
  ~CDelayedTermIter() {}

  term_type term() const {
    stack_type the_stack(base::getStack());

    term_type result;
    result = terminalop_type()(result, !the_stack.empty());

    appendop_type do_append;

    while(!the_stack.empty() && the_stack.top().isValid()) {

      result =  do_append(result, *the_stack.top() );
      the_stack.pop();
    }

    return result;
  }
};


END_NAMESPACE_PBORI

#endif
