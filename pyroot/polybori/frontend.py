from polybori.blocks import declare_ring as orig_declare_ring

def ternary_declare_ring(blocks, context, global_context):
    """This is just a wrapper fpr declare_ring, which selects context, if it is
    not None and global_context otherwise."""
    if context is None:
        context = global_context
        
    return orig_declare_ring(blocks, context)




def block_scheme_names(blocks):
    """Helper for Singular interface."""

    context = dict()
    from polybori.blocks import declare_block_scheme
    declare_block_scheme(blocks, context)

    return context.keys()

def copyright():
    print """Copyright (c) 2007-2011 by The PolyBoRi Team.
  Michael Brickenstein (MFO)  brickenstein@mfo.de
  Alexander Dreyer (ITWM) alexander.dreyer@itwm.fraunhofer.de

The PolyBoRi Team is a joint project of
  Mathematisches Forschungsinstitut Oberwolfach (MFO), Germany
  Department of Mathematics, University of Kaiserslautern, Germany, and
  Fraunhofer Institute for Industrial Mathematics (ITWM), Kaiserslautern, Germany.

PolyBoRi incorporates the following works:
  The CU Decision Diagrams Package  Release 2.4.1 (CUDD) by Fabio Somenzi,
    Copyright (c) 1995-2004, Regents of the University of Colorado. All Rights Reserved.
  The M4RI Library - http://m4ri.sagemath.org
    Copyright (C) 2007-2010, Martin Albrecht, Gregory Bard, and The M4RI Team"""

def license():
    print """ipbori and the PolyBoRi framework are licensed under the terms of
the GNU General Public License (GPL) version 2 or later.
See http://www.gnu.org/licenses/ for details."""
