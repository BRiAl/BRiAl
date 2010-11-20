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
