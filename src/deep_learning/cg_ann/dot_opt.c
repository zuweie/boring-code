#include "deep_learning/compute_graph2/cg_znode.h"
#include "base/container_of.h"
#include "dot_opt.h"


static int __fp(cg_znode_t* znode, void*)
{
    // 找到两个 incomp 的节点，然后将它们的 payload 做内集
    void* ptr         = CG_LIST_TOP(znode->vertex.in_vertexes)->ref;
    cg_znode_t* right = container_of(ptr, cg_znode_t, vertex);

    ptr               = CG_LIST_TOP(znode->vertex.in_vertexes)->prev->ref;
    cg_znode_t* left  = container_of(ptr, cg_znode_t, vertex);

    return cg_tensor_dot(znode->payload, right->payload, left->payload);
}

static int __bp(cg_znode_t* znode, void*) 
{

}

cg_opt_base_t* create_dot_opt(void*) 
{
    cg_opt_base_t* opt = (cg_opt_base_t*) malloc (sizeof(dot_opt_t));
    opt->flow_elem = e_opt;
    opt->fp        = __fp;
    opt->bp        = __bp;
    return opt;
}