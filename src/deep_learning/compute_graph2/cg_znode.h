#ifndef __CG_ZNODE_H__
#define __CG_ZNODE_H__

#include "cg_graph.h"
#include "cg_tensor.h"
#include "opt/cg_opt_base.h"
typedef struct cg_znode {
    
    cg_vertex_t vertex;
    cg_tensor_t* payload;
    cg_tensor_t* gradient;
    cg_opt_base* opt;

} cg_znode_t;

#endif