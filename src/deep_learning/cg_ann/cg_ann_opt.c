#include "deep_learning/compute_graph2/cg_znode.h"
#include "deep_learning/compute_graph2/cg_debug.h"
#include "cg_ann_opt.h"

static int __dot_fp(cg_znode_t* znode, void*){
    
    return 0;
}
static int __dot_bp(cg_znode_t* znode, void*) 
{
    return 0;
}

static int __plus_fp(cg_znode_t* znode, void*)
{
    return 0;
}

static int __plus_bp(cg_znode_t* znode, void*) 
{
    return 0;
}

static int __relu_fp(cg_znode_t* znode, void*)
{
    return 0;
}

static int __relu_bp(cg_znode_t* znode, void*)
{
    return 0;
}

static int __softmax_fp(cg_znode_t* znode, void*)
{
    return 0;
}

static int __softmax_bp(cg_znode_t* znode, void*)
{
    return 0;
}

static int __cross_fp(cg_znode_t* znode, void*)
{
    return 0;
}

static int __cross_bp(cg_znode_t* znode, void*)
{
    return 0;
}
cg_opt_base_t* dot_opt (void*)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__dot_fp, .bp=__dot_bp, .recycle=NULL};
    return opt;
}

cg_opt_base_t* plus_opt (void*)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__plus_fp, .bp=__plus_bp, .recycle=NULL};
    return opt;
}


cg_opt_base_t* relu_opt (void*)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__relu_fp, .bp=__relu_bp, .recycle=NULL};
    return opt;
}


cg_opt_base_t* softmax_opt (void*)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__softmax_fp, .bp=__softmax_bp, .recycle=NULL};
    return opt;
}

cg_opt_base_t* cross_entroy (void*)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__cross_fp, .bp=__cross_bp, .recycle=NULL};
    return opt;
}

cg_opt_base_t* mse_opt (void*)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__cross_fp, .bp=__cross_bp, .recycle=NULL};
    return opt;
}
}